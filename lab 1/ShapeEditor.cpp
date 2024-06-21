#include "ShapeEditor.h"
#include "Group.h"

#include "resource.h"

// SHAPE EDITOR

int ShapeEditor::getPosCheckedGroupMenu(HWND hwnd, int ID)
{
	int menupos = 3;
	HMENU menu = GetMenu(hwnd);
	HMENU submenu = GetSubMenu(menu, menupos);
	int item_id1 = GetMenuItemID(submenu, 0);
	int item_id2 = GetMenuItemID(submenu, 1);
	int item_id3 = GetMenuItemID(submenu, 2);

	while (item_id1 != ID && item_id2 != ID && item_id3 != ID)
	{
		submenu = GetSubMenu(menu, ++menupos);
		item_id1 = GetMenuItemID(submenu, 0);
		item_id2 = GetMenuItemID(submenu, 1);
		item_id3 = GetMenuItemID(submenu, 2);
	}

	return menupos;
}

void ShapeEditor::unCheckedMenuPopUp(HWND hwnd)
{
	delete ed;
	ed = NULL;
	HMENU hMenu, hSubMenu;
	hMenu = GetMenu(hwnd);
	hSubMenu = GetSubMenu(hMenu, 2);
	CheckMenuItem(hSubMenu, ID_OBJECTS_CIRCLE, MF_UNCHECKED);
	CheckMenuItem(hSubMenu, ID_OBJECTS_RECTANGLE, MF_UNCHECKED);
	CheckMenuItem(hSubMenu, ID_OBJECTS_GROUP, MF_UNCHECKED);
	CheckMenuItem(hSubMenu, ID_OBJECTS_MOVE, MF_UNCHECKED);
	CheckMenuItem(hSubMenu, ID_OBJECTS_COPY, MF_UNCHECKED);
	CheckMenuItem(hSubMenu, ID_OBJECTS_DELETE, MF_UNCHECKED);
}

ShapeEditor::~ShapeEditor()
{
	for (int i = 0; i < pos; ++i)
	{
		delete shapes[i];
	}

	for (int i = 0; i < grPos; ++i)
	{
		delete groups[i];
		delete grMenu[i];
	}
}

bool ShapeEditor::OutputInfoToFile(const std::string& fileName)
{
	if (pos == 0) return false;

	std::ofstream ofs(fileName);
	if (!ofs) return false;

	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;

	for (int i = 0; i < pos; ++i)
	{
		bool is_shape_in_group = false;
		for (int j = 0; j < grPos; ++j)
		{
			if (groups[j]->is_shape_exist(*shapes[i]))
			{
				is_shape_in_group = true;
				break;
			}
		}
		if (!is_shape_in_group)
		{
			shapes[i]->print_to_file(ofs, false);
		}
	}

	ofs << std::endl;

	for (int j = 0; j < grPos; ++j)
	{
		Shapes** grShapes = groups[j]->get_group_shapes();
		ofs << "Group #" << groups[j]->get_num() << std::endl;
		for (int i = 0; i < groups[j]->get_n(); ++i)
		{
			grShapes[i]->print_to_file(ofs, true);
		}
		ofs << std::endl;
	}

	ofs.close();
	return true;
}

void ShapeEditor::onInitMenuPopUp(HWND hwnd, WPARAM wparams)
{
	if (ed) ed->onInitMenuPopUp(hwnd, wparams);
}

void ShapeEditor::LButtonDown(HWND hwnd)
{
	if (ed) ed->LButtonDown(hwnd);
}

void ShapeEditor::LButtonUp(HWND hwnd)
{
	if (ed) ed->LButtonUp(hwnd);
}

void ShapeEditor::onMouseMove(HWND hwnd)
{
	if (ed && pressed) ed->onMouseMove(hwnd);
}

void ShapeEditor::Paint(HWND hwnd)
{
	Editor* draw = new Editor;
	draw->Paint(hwnd);
	delete draw;
}

void ShapeEditor::StartCircleEditor()
{
	if (ed) delete ed;
	ed = new CircleEditor;
}

void ShapeEditor::StartRectEditor()
{
	if (ed) delete ed;
	ed = new RectEditor;
}

void ShapeEditor::StartGroupEditor()
{
	if (ed) delete ed;
	ed = new GroupEditor;
}

void ShapeEditor::StartMoveEditor()
{
	if (ed) delete ed;
	ed = new MoveShapeEditor;
}

void ShapeEditor::StartCopyEditor()
{
	if (ed) delete ed;
	ed = new CopyEditor;
}

void ShapeEditor::StartDelEditor()
{
	if (ed) delete ed;
	ed = new DelEditor;
}

void ShapeEditor::CopyGroup(HWND hwnd, int ID)
{
	if (grPos == GRSZ)
	{
		MessageBox(
			hwnd,
			L"Error. Maximum number of groups reached\n",
			L"ERROR",
			MB_OK | MB_ICONERROR
		);
		return;
	}

	int numGroup = getPosCheckedGroupMenu(hwnd, ID);

	Group* new_group = groups[numGroup - 3]->copy_group();
	GroupMenu* new_menu = new GroupMenu;
	new_menu->addGroupMenu(hwnd, *new_group, grPos);
	grMenu[grPos] = new_menu;

	maxPos = ++grPos;

	InvalidateRect(hwnd, NULL, TRUE);
}

void ShapeEditor::DelShapeFromGroup(HWND hwnd, int ID)
{
	int numGroup = getPosCheckedGroupMenu(hwnd, ID);
	int x = 0, y = 0;

	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	x = pt.x;
	y = pt.y;

	groups[numGroup - 3]->del_shape_by_coords(x, y);
	del_shape_from_group = false;

	if (!groups[numGroup - 3]->get_n())
		DelGroup(hwnd, ID);

	InvalidateRect(hwnd, NULL, TRUE);
}

void ShapeEditor::DelGroup(HWND hwnd, int ID)
{
	int counter = 0;
	int numGroup = getPosCheckedGroupMenu(hwnd, ID);
	HMENU menu = GetMenu(hwnd);
	HMENU submenu = GetSubMenu(menu, counter);

	while (counter != grPos)
	{
		RemoveMenu(menu, 3, MF_BYPOSITION);
		DestroyMenu(submenu);
		submenu = GetSubMenu(menu, 3);
		++counter;
	}
	DrawMenuBar(hwnd);

	delete groups[numGroup - 3];
	delete grMenu[numGroup - 3];

	for (int i = numGroup - 2; i < grPos; ++i)
	{
		groups[i - 1] = groups[i];
		grMenu[i - 1] = grMenu[i];
		groups[i - 1]->set_num(i);
	}

	--grPos;

	for (int i = 0; i < grPos; ++i)
	{
		grMenu[i]->addGroupMenu(hwnd, *groups[i], i);
	}

	InvalidateRect(hwnd, NULL, TRUE);
}
