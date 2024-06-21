#pragma once

#include <Windows.h>

#include "Shapes.h"

class Group
{
private:
	Shapes** groupShapes;
	int n;
	Rect* groupRect;
	int numGroup;

public:
	Group() = default;
	Group(const Group& other) = delete;
	~Group();

	Group& operator=(const Group& other) = delete;

	void addShape(Shapes& shape);
	void addRect(Rect& rect);
	void show(HWND hwnd, HDC hdc) const;
	int get_n() const;
	void drawText(HWND hwnd) const;
	void set_num(int num);
	int get_num() const;
	bool is_shape_exist(const Shapes& shape) const;
	void del_shape_by_coords(int x, int y);
	void add_shape_by_coords(int x, int y);
	bool is_coords_in_rect(int x, int y) const;
	Shapes** get_group_shapes() const;
	Rect* get_group_rect() const;
	Group* copy_group() const;
};

class GroupMenu
{
private:
	Group* gr;

public:
	GroupMenu() = default;
	GroupMenu(const GroupMenu& other) = default;
	~GroupMenu() = default;

	GroupMenu& operator=(const GroupMenu& other) = default;

	void addGroupMenu(HWND hwnd, Group& group, int menupos);
};

#include "variables.h"

extern Group* groups[GRSZ];
extern GroupMenu* grMenu[GRSZ];

