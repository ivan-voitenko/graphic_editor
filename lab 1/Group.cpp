#include "Group.h"

#include <cstdio>

Group* groups[GRSZ];
GroupMenu* grMenu[GRSZ];


// GROUP

Group::~Group()
{
	delete[] groupShapes;
	delete groupRect;
}

void Group::addShape(Shapes& shape)
{
	Shapes** temp = new Shapes*[n + 1];
	for (int i = 0; i < n; ++i)
	{
		temp[i] = groupShapes[i];
	}
	delete[] groupShapes;
	temp[n] = &shape;
	++n;
	groupShapes = temp;
}

void Group::addRect(Rect& rect)
{
	groupRect = &rect;
}

void Group::show(HWND hwnd, HDC hdc) const
{
	showGroup = true;
	groupRect->show(hdc);
	drawText(hwnd);
	showGroup = false;
}

int Group::get_n() const { return n; }

void Group::drawText(HWND hwnd) const
{
	HDC hdc = GetDC(hwnd);

	int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
	groupRect->get_coords(x1, y1, x2, y2);

	wchar_t text[15];
	swprintf_s(text, 15, L"Group %d", numGroup);

	TextOut(hdc, x1 + 5, y1 + 5, text, wcslen(text));

	ReleaseDC(hwnd, hdc);
}

void Group::set_num(int num) { numGroup = num; }

int Group::get_num() const { return numGroup; }

bool Group::is_shape_exist(const Shapes& shape) const
{
	for (int i = 0; i < n; ++i)
	{
		if (dynamic_cast<const Circle*>(groupShapes[i]) && dynamic_cast<const Circle*>(&shape) ||
			dynamic_cast<Rect*>(groupShapes[i]) && dynamic_cast<const Rect*>(&shape))
		{
			if (*groupShapes[i] == shape)
			{
				return true;
			}
		}
	}
	return false;
}

void Group::del_shape_by_coords(int x, int y)
{
	int square = 0;
	int min_square = INT_MAX;
	int min_index = -1;

	for (int i = 0; i < n; ++i)
	{
		if (groupShapes[i]->coords_in_shape(x, y, square) && min_square > square)
		{
			min_square = square;
			min_index = i;
		}
	}

	if (min_index != -1)
	{
		Shapes** temp = new Shapes * [n - 1];
		int j = 0;
		for (int i = 0; i < n; ++i)
		{
			if (min_index != i)
			{
				temp[j++] = groupShapes[i];
			}
		}
		delete[] groupShapes;
		--n;
		groupShapes = temp;
	}
}

void Group::add_shape_by_coords(int x, int y)
{
	int square = 0;
	int min_square = INT_MAX;
	Shapes* shape = NULL;

	for (int i = 0; i < pos; ++i)
	{
		if (shapes[i]->coords_in_shape(x, y, square) && min_square > square)
		{
			for (int j = 0; j < grPos; ++j)
			{
				if (groups[j]->is_shape_exist(*shapes[i]))
					return;
			}
			min_square = square;
			shape = shapes[i];
		}
	}

	if (shape)
	{
		addShape(*shape);
	}
}

bool Group::is_coords_in_rect(int x, int y) const
{
	int temp = 0;
	return groupRect->coords_in_shape(x, y, temp);
}

Shapes** Group::get_group_shapes() const
{
	return groupShapes;
}

Rect* Group::get_group_rect() const
{
	return groupRect;
}

Group* Group::copy_group() const
{
	int offset = 30;
	int xs1 = 0, ys1 = 0, xs2 = 0, ys2 = 0;
	groups[grPos] = new Group;
	groups[grPos]->numGroup = grPos + 1;
	groups[grPos]->n = n;
	groups[grPos]->groupShapes = new Shapes * [n];
	for (int i = 0; i < n; ++i)
	{
		groups[grPos]->groupShapes[i] = groupShapes[i]->copy();
		shapes[pos++] = groups[grPos]->groupShapes[i];

		groups[grPos]->groupShapes[i]->get_coords(xs1, ys1, xs2, ys2);
		if (ys2 == INT_MIN)
		{
			groups[grPos]->groupShapes[i]->set_coords(xs1 + offset, ys1 + offset, xs2, ys2);
		}
		else
		{
			groups[grPos]->groupShapes[i]->set_coords(xs1 + offset, ys1 + offset, xs2 + offset, ys2 + offset);
		}
	}
	groups[grPos]->groupRect = dynamic_cast<Rect*>(groupRect->copy());
	groups[grPos]->groupRect->get_coords(xs1, ys1, xs2, ys2);
	groups[grPos]->groupRect->set_coords(xs1 + offset, ys1 + offset, xs2 + offset, ys2 + offset);
	return groups[grPos];
}


// GROUP MENU

void GroupMenu::addGroupMenu(HWND hwnd, Group& group, int menupos)
{
	HMENU oldmenu = GetMenu(hwnd);
	HMENU submenu = CreatePopupMenu();

	gr = &group;

	wchar_t menutext[15];
	swprintf_s(menutext, 15, L"Group %d", gr->get_num());

	AppendMenu(submenu, MF_STRING, COPYGR + menupos * 3, L"Copy group");
	AppendMenu(submenu, MF_STRING, DELGR + menupos * 3, L"Delete group");
	AppendMenu(submenu, MF_STRING, DELSGR + menupos * 3, L"Delete shape from group");

	AppendMenu(oldmenu, MF_POPUP, (UINT_PTR)submenu, (LPWSTR)menutext);

	SetMenu(hwnd, oldmenu);
	DrawMenuBar(hwnd);
}
