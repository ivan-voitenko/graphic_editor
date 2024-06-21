#include "Editors.h"
#include "resource.h"

#include "ShapeEditor.h"
#include "Shapes.h"
#include "Group.h"

#include <math.h>

// EDITOR

Editor::Editor() : x1(0), y1(0), x2(0), y2(0) {}

void Editor::LButtonDown(HWND hwnd)
{
	pressed = true;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	x1 = x2 = pt.x;
	y1 = y2 = pt.y;
}

void Editor::LButtonUp(HWND hwnd)
{
	pressed = false;
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	x2 = pt.x;
	y2 = pt.y;
}

void Editor::onMouseMove(HWND hwnd) 
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(hwnd, &pt);
	x2 = pt.x;
	y2 = pt.y;
}

void Editor::Paint(HWND hwnd)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps);
	for (int i = 0; i < pos; ++i)
	{
		shapes[i]->show(hdc);
	}
	for (int i = 0; i < grPos; ++i)
	{
		groups[i]->show(hwnd, hdc);
	}
	EndPaint(hwnd, &ps);
}

void Editor::onInitMenuPopUp(HWND hwnd, WPARAM wparams) {}


// CIRCLE

void CircleEditor::LButtonDown(HWND hwnd)
{
	__super::LButtonDown(hwnd);
}

void CircleEditor::LButtonUp(HWND hwnd)
{
	__super::LButtonUp(hwnd);

	if (pos == SIZE)
	{
		MessageBox(
			hwnd,
			L"Error. Maximum number of shapes reached\n",
			L"ERROR",
			MB_OK | MB_ICONERROR
		);
		return;
	}

	int radius = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

	Circle* circle = new Circle(x1, y1, radius);
	shapes[pos++] = circle;

	InvalidateRect(hwnd, NULL, TRUE);
}

void CircleEditor::onMouseMove(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);

	SetROP2(hdc, R2_NOTXORPEN);

	HPEN pen, penOld;
	pen = CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	penOld = static_cast<HPEN>(SelectObject(hdc, pen));

	int radius = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

	Arc(hdc, x1 - radius, y1 - radius, x1 + radius, y1 + radius, 0, 0, 0, 0);

	__super::onMouseMove(hwnd);

	radius = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));

	Arc(hdc, x1 - radius, y1 - radius, x1 + radius, y1 + radius, 0, 0, 0, 0);

	SelectObject(hdc, penOld);
	DeleteObject(pen);

	ReleaseDC(hwnd, hdc);
}

void CircleEditor::onInitMenuPopUp(HWND hwnd, WPARAM wparams)
{
	HMENU hMenu, hSubMenu;
	hMenu = GetMenu(hwnd);
	hSubMenu = GetSubMenu(hMenu, 2);
	if ((HMENU)wparams == hSubMenu) {
		CheckMenuItem(hSubMenu, ID_OBJECTS_CIRCLE, MF_CHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_RECTANGLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_GROUP, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_MOVE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_COPY, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_DELETE, MF_UNCHECKED);
	}
}


// RECTANGLE

void RectEditor::LButtonDown(HWND hwnd)
{
	__super::LButtonDown(hwnd);
}

void RectEditor::LButtonUp(HWND hwnd)
{
	__super::LButtonUp(hwnd);

	if (pos == SIZE)
	{
		MessageBox(
			hwnd,
			L"Error. Maximum number of shapes reached\n",
			L"ERROR",
			MB_OK | MB_ICONERROR
		);
		return;
	}

	Rect* rect = new Rect(x1, y1, x2, y2);
	shapes[pos++] = rect;

	InvalidateRect(hwnd, NULL, TRUE);
}

void RectEditor::onMouseMove(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);

	SetROP2(hdc, R2_NOTXORPEN);

	HPEN pen, penOld;
	HBRUSH brush, brushOld;
	pen = CreatePen(PS_DOT, 1, RGB(0, 0, 255));
	brush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

	penOld = static_cast<HPEN>(SelectObject(hdc, pen));
	brushOld = static_cast<HBRUSH>(SelectObject(hdc, brush));

	Rectangle(hdc, x1, y1, x2, y2);
	__super::onMouseMove(hwnd);
	Rectangle(hdc, x1, y1, x2, y2);

	SelectObject(hdc, brushOld);
	DeleteObject(brush);

	SelectObject(hdc, penOld);
	DeleteObject(pen);
	
	ReleaseDC(hwnd, hdc);
}

void RectEditor::onInitMenuPopUp(HWND hwnd, WPARAM wparams)
{
	HMENU hMenu, hSubMenu;
	hMenu = GetMenu(hwnd);
	hSubMenu = GetSubMenu(hMenu, 2);
	if ((HMENU)wparams == hSubMenu) {
		CheckMenuItem(hSubMenu, ID_OBJECTS_CIRCLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_RECTANGLE, MF_CHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_GROUP, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_MOVE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_COPY, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_DELETE, MF_UNCHECKED);
	}
}


// GROUP 

void GroupEditor::LButtonDown(HWND hwnd)
{
	__super::LButtonDown(hwnd);
}

bool check_shape_in_other_group(int j)
{
	for (int i = 0; i < grPos; ++i)
	{
		if (groups[i]->is_shape_exist(*shapes[j]))
		{
			return true;
		}
	}
	return false;
}

void GroupEditor::LButtonUp(HWND hwnd)
{
	__super::LButtonUp(hwnd);
	
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

	Rect* rect = new Rect(x1, y1, x2, y2);
	Group* group = new Group;
	GroupMenu* grmenu = new GroupMenu;

	int sx1 = 0, sy1 = 0, sx2 = 0, sy2 = 0;
	bool is_shape_exist_in_other_group = false;
	
	for (int i = 0; i < pos; ++i)
	{
		shapes[i]->get_coords(sx1, sy1, sx2, sy2);
		if (sy2 == INT_MIN)
		{
			if (x1 < sx1 - sx2 && x2 > sx1 + sx2 && y1 < sy1 - sx2 && y2 > sy1 + sx2) // sx2 = radius
			{
				if ((is_shape_exist_in_other_group = check_shape_in_other_group(i))) break;
				group->addShape(*shapes[i]);
			}
			else if (x1 < sx1 - sx2 && x2 > sx1 + sx2 && y1 > sy1 - sx2 && y2 < sy1 + sx2)
			{
				if ((is_shape_exist_in_other_group = check_shape_in_other_group(i))) break;
				group->addShape(*shapes[i]);
			}
		}
		else
		{
			if (x1 < sx1 && x2 > sx2 && y1 < sy1 && y2 > sy2)
			{
				if ((is_shape_exist_in_other_group = check_shape_in_other_group(i))) break;
				group->addShape(*shapes[i]);
			}
			else if (x1 < sx1 && x2 > sx2 && y1 > sy1 && y2 < sy2)
			{
				if ((is_shape_exist_in_other_group = check_shape_in_other_group(i))) break;
				group->addShape(*shapes[i]);
			}
		}
	}

	if (group->get_n() == 0 || is_shape_exist_in_other_group)
	{
		delete rect;
		delete group;
		delete grmenu;
		InvalidateRect(hwnd, NULL, TRUE);
		return;
	}

	group->set_num(grPos + 1);
	grmenu->addGroupMenu(hwnd, *group, grPos);
	group->addRect(*rect);
	group->drawText(hwnd);
	groups[grPos] = group;
	grMenu[grPos++] = grmenu;

	maxPos = grPos;

	InvalidateRect(hwnd, NULL, TRUE);
}

void GroupEditor::onMouseMove(HWND hwnd)
{
	HDC hdc = GetDC(hwnd);

	SetROP2(hdc, R2_NOTXORPEN);

	HPEN pen, penOld;
	HBRUSH brush, brushOld;
	pen = CreatePen(PS_DOT, 1, RGB(0, 0, 255));
	brush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));

	penOld = static_cast<HPEN>(SelectObject(hdc, pen));
	brushOld = static_cast<HBRUSH>(SelectObject(hdc, brush));

	Rectangle(hdc, x1, y1, x2, y2);
	__super::onMouseMove(hwnd);
	Rectangle(hdc, x1, y1, x2, y2);

	SelectObject(hdc, brushOld);
	DeleteObject(brush);

	SelectObject(hdc, penOld);
	DeleteObject(pen);

	ReleaseDC(hwnd, hdc);
}

void GroupEditor::onInitMenuPopUp(HWND hwnd, WPARAM wparams)
{
	HMENU hMenu, hSubMenu;
	hMenu = GetMenu(hwnd);
	hSubMenu = GetSubMenu(hMenu, 2);
	if ((HMENU)wparams == hSubMenu) {
		CheckMenuItem(hSubMenu, ID_OBJECTS_CIRCLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_RECTANGLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_GROUP, MF_CHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_MOVE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_COPY, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_DELETE, MF_UNCHECKED);
	}
}


// MOVE SHAPE EDITOR

MoveShapeEditor::MoveShapeEditor() :
	moveGroup(false), moveShape(false), groupIndex(-1), shapeIndex(-1) {}

void MoveShapeEditor::LButtonDown(HWND hwnd)
{
	if (pos == 0)
	{
		MessageBox(
			hwnd,
			L"Error. You havent any shapes to move\n",
			L"ERROR",
			MB_OK | MB_ICONERROR
		);
		return;
	}

	__super::LButtonDown(hwnd);

	int square = 0;
	int min_square = INT_MAX;

	for (int j = 0; j < grPos; ++j)
	{
		if (groups[j]->is_coords_in_rect(x1, y1))
		{
			moveGroup = true;
			groupIndex = j;
			return;
		}
	}

	for (int i = 0; i < pos; ++i)
	{
		if (shapes[i]->coords_in_shape(x1, y1, square) && min_square > square)
		{
			min_square = square;
			shapeIndex = i;
		}
	}

	if (shapeIndex == -1)
	{
		return;
	}

	for (int j = 0; j < grPos; ++j)
	{
		if (groups[j]->is_shape_exist(*shapes[shapeIndex]))
		{
			moveGroup = true;
			groupIndex = j;
			return;
		}
	}
	moveShape = true;
}

int DisplayConfirmSaveAsMessageBox(HWND hwnd, Group& group, Shapes& shape)
{
	int msgboxID = MessageBox(
		hwnd,
		L"Do you want to add this shape to group?",
		L"Add shape to group",
		MB_ICONEXCLAMATION | MB_YESNO
	);

	if (msgboxID == IDYES)
	{
		group.addShape(shape);
	}

	return msgboxID;
}

void MoveShapeEditor::LButtonUp(HWND hwnd)
{
	__super::LButtonUp(hwnd);

	if (moveShape)
	{
		for (int i = 0; i < grPos; ++i)
		{
			if (groups[i]->is_coords_in_rect(x2, y2))
			{
				if (DisplayConfirmSaveAsMessageBox(hwnd, *groups[i], *shapes[shapeIndex]) == IDYES)
				{
					InvalidateRect(hwnd, NULL, TRUE);
				}
				break;
			}
		}
	}

	moveShape = moveGroup = false;
	shapeIndex = groupIndex = -1;
}

void MoveShapeEditor::onMouseMove(HWND hwnd)
{
	__super::onMouseMove(hwnd);

	int offsetX = x1 - x2;
	int offsetY = y1 - y2;
	int xs1 = 0, xs2 = 0, ys1 = 0, ys2 = 0;
	int minX = 0, maxX = 0;
	int minY = 0, maxY = 0;

	if (moveShape)
	{
		shapes[shapeIndex]->get_coords(xs1, ys1, xs2, ys2);

		if (ys2 != INT_MIN)
		{
			minX = min(xs1, xs2), maxX = max(xs1, xs2);
			minY = min(ys1, ys2), maxY = max(ys1, ys2);
			shapes[shapeIndex]->set_coords(minX - offsetX, minY - offsetY, maxX - offsetX, maxY - offsetY);
		}
		else
		{
			shapes[shapeIndex]->set_coords(xs1 - offsetX, ys1 - offsetY, xs2, 0);
		}
	}
	else if (moveGroup)
	{
		Shapes** GrShapes = groups[groupIndex]->get_group_shapes();
		Rect* GrRect = groups[groupIndex]->get_group_rect();
		for (int i = 0; i < groups[groupIndex]->get_n(); ++i)
		{
			GrShapes[i]->get_coords(xs1, ys1, xs2, ys2);

			if (ys2 != INT_MIN)
			{
				minX = min(xs1, xs2), maxX = max(xs1, xs2);
				minY = min(ys1, ys2), maxY = max(ys1, ys2);
				GrShapes[i]->set_coords(minX - offsetX, minY - offsetY, maxX - offsetX, maxY - offsetY);
			}
			else
			{
				GrShapes[i]->set_coords(xs1 - offsetX, ys1 - offsetY, xs2, 0);
			}
		}
		GrRect->get_coords(xs1, ys1, xs2, ys2);
		minX = min(xs1, xs2), maxX = max(xs1, xs2);
		minY = min(ys1, ys2), maxY = max(ys1, ys2);
		GrRect->set_coords(minX - offsetX, minY - offsetY, maxX - offsetX, maxY - offsetY);
		groups[groupIndex]->drawText(hwnd);
	}

	x1 = x2;
	y1 = y2;

	InvalidateRect(hwnd, NULL, TRUE);
}

void MoveShapeEditor::onInitMenuPopUp(HWND hwnd, WPARAM wparams)
{
	HMENU hMenu, hSubMenu;
	hMenu = GetMenu(hwnd);
	hSubMenu = GetSubMenu(hMenu, 2);
	if ((HMENU)wparams == hSubMenu) {
		CheckMenuItem(hSubMenu, ID_OBJECTS_CIRCLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_RECTANGLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_GROUP, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_MOVE, MF_CHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_COPY, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_DELETE, MF_UNCHECKED);
	}
}


// COPY EDITOR

void CopyEditor::LButtonDown(HWND hwnd)
{
	if (pos == 0)
	{
		MessageBox(
			hwnd,
			L"Error. You havent any shapes to copy\n",
			L"ERROR",
			MB_OK | MB_ICONERROR
		);
		return;
	}

	__super::LButtonDown(hwnd);
}

void CopyEditor::LButtonUp(HWND hwnd)
{
	if (pos == SIZE)
	{
		MessageBox(
			hwnd,
			L"Error. Maximum number of shapes reached\n",
			L"ERROR",
			MB_OK | MB_ICONERROR
		);
		return;
	}

	int offset = 30;
	int xs1 = 0, ys1 = 0, xs2 = 0, ys2 = 0;
	int square = 0;
	int min_square = INT_MAX;
	int index = -1;

	for (int i = 0; i < pos; ++i)
	{
		if (shapes[i]->coords_in_shape(x1, y1, square) && min_square > square)
		{
			min_square = square;
			index = i;
		}
	}

	if (index != -1)
	{
		for (int j = 0; j < grPos; ++j)
		{
			if (groups[j]->is_shape_exist(*shapes[index]))
			{
				pressed = false;
				return;
			}
		}
		Shapes* copy_shape = shapes[index]->copy();
		shapes[pos++] = copy_shape;
		copy_shape->get_coords(xs1, ys1, xs2, ys2);
		if (ys2 == INT_MIN)
		{
			copy_shape->set_coords(xs1 + offset, ys1 + offset, xs2, ys2);
		}
		else
		{
			copy_shape->set_coords(xs1 + offset, ys1 + offset, xs2 + offset, ys2 + offset);
		}
		InvalidateRect(hwnd, NULL, TRUE);
	}
	else
		pressed = false;
}

void CopyEditor::onMouseMove(HWND hwnd) {}

void CopyEditor::onInitMenuPopUp(HWND hwnd, WPARAM wparams)
{
	HMENU hMenu, hSubMenu;
	hMenu = GetMenu(hwnd);
	hSubMenu = GetSubMenu(hMenu, 2);
	if ((HMENU)wparams == hSubMenu) {
		CheckMenuItem(hSubMenu, ID_OBJECTS_CIRCLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_RECTANGLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_GROUP, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_MOVE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_COPY, MF_CHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_DELETE, MF_UNCHECKED);
	}
}

void DelEditor::LButtonDown(HWND hwnd)
{
	if (pos == 0)
	{
		MessageBox(
			hwnd,
			L"Error. You havent any shapes to delete\n",
			L"ERROR",
			MB_OK | MB_ICONERROR
		);
		return;
	}

	__super::LButtonDown(hwnd);
}

void DelEditor::LButtonUp(HWND hwnd)
{
	__super::LButtonUp(hwnd);

	int square = 0;
	int min_square = INT_MAX;
	int delIndex = -1;

	for (int i = 0; i < pos; ++i)
	{
		if (shapes[i]->coords_in_shape(x1, y1, square) && min_square > square)
		{
			min_square = square;
			delIndex = i;
		}
	}

	if (delIndex == -1)
	{
		return;
	}

	for (int j = 0; j < grPos; ++j)
	{
		if (groups[j]->is_shape_exist(*shapes[delIndex]))
		{
			return;
		}
	}

	delete shapes[delIndex];

	for (int i = delIndex + 1; i < pos; ++i)
	{
		shapes[i - 1] = shapes[i];
	}
	--pos;

	InvalidateRect(hwnd, NULL, TRUE);
}

void DelEditor::onMouseMove(HWND hwnd) {}

void DelEditor::onInitMenuPopUp(HWND hwnd, WPARAM wparams)
{
	HMENU hMenu, hSubMenu;
	hMenu = GetMenu(hwnd);
	hSubMenu = GetSubMenu(hMenu, 2);
	if ((HMENU)wparams == hSubMenu) {
		CheckMenuItem(hSubMenu, ID_OBJECTS_CIRCLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_RECTANGLE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_GROUP, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_MOVE, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_COPY, MF_UNCHECKED);
		CheckMenuItem(hSubMenu, ID_OBJECTS_DELETE, MF_CHECKED);
	}
}
