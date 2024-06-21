#define _USE_MATH_DEFINES

#include "Shapes.h"
#include "Group.h"

#include <math.h>

Shapes* shapes[SIZE];

// CIRCLE

Circle::Circle(int x_, int y_, int r_) : x(x_), y(y_), r(r_) {}

bool Circle::operator==(const Shapes& other) const
{
	const Circle& another = dynamic_cast<const Circle&>(other);
	return x == another.x && y == another.y && r == another.r;
}

void Circle::show(HDC hdc) const
{
	HPEN pen, penOld;

	bool is_shape_in_group = false;
	for (int i = 0; i < grPos; ++i)
	{
		if (groups[i]->is_shape_exist(*this))
		{
			is_shape_in_group = true;
			break;
		}
	}

	if (!is_shape_in_group)
		pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	else
		pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	penOld = static_cast<HPEN>(SelectObject(hdc, pen));

	Arc(hdc, x - r, y - r, x + r, y + r, 0, 0, 0, 0);

	SelectObject(hdc, penOld);
	DeleteObject(pen);
}

void Circle::set_coords(int x1_, int y1_, int x2_, int y2_)
{
	x = x1_;
	y = y1_;
	r = x2_;
}

void Circle::get_coords(int& x1_, int& y1_, int& x2_, int& y2_) const
{
	x1_ = x;
	y1_ = y;
	x2_ = r;
	y2_ = INT_MIN;
}

bool Circle::coords_in_shape(int x_, int y_, int& square) const
{
	square = M_PI * r * r;

	return (pow(x - x_, 2) + pow(y - y_, 2)) <= r * r;
}

Shapes* Circle::copy() const
{
	return new Circle(x, y, r);
}

void Circle::print_to_file(std::ofstream& ofs, bool is_group) const
{
	if (!is_group)
		ofs << "Circle (x: " << x << ", y: " << y << ") R = " << r << std::endl;
	else
		ofs << "  +Circle (x: " << x << ", y: " << y << ") R = " << r << std::endl;
}


// RECT

Rect::Rect(int x1_, int y1_, int x2_, int y2_) :
	x1(x1_), y1(y1_), x2(x2_), y2(y2_) {}

bool Rect::operator==(const Shapes& other) const
{
	const Rect& another = dynamic_cast<const Rect&>(other);
	return x1 == another.x1 && x2 == another.x2 && y1 == another.y1 && y2 == another.y2;
}

void Rect::show(HDC hdc) const
{
	HPEN pen, penOld;
	HBRUSH brush, brushOld;

	bool is_shape_in_group = false;
	for (int i = 0; i < grPos; ++i)
	{
		if (groups[i]->is_shape_exist(*this))
		{
			is_shape_in_group = true;
			break;
		}
	}

	if (!showGroup && !is_shape_in_group)
		pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
	else if (showGroup)
		pen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	else
		pen = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	brush = static_cast<HBRUSH>(GetStockObject(NULL_BRUSH));;

	penOld = static_cast<HPEN>(SelectObject(hdc, pen));
	brushOld = static_cast<HBRUSH>(SelectObject(hdc, brush));

	Rectangle(hdc, x1, y1, x2, y2);

	SelectObject(hdc, brushOld);
	DeleteObject(brush);

	SelectObject(hdc, penOld);
	DeleteObject(pen);
}

void Rect::set_coords(int x1_, int y1_, int x2_, int y2_)
{
	x1 = x1_;
	y1 = y1_;
	x2 = x2_;
	y2 = y2_;
}

void Rect::get_coords(int& x1_, int& y1_, int& x2_, int& y2_) const
{
	x1_ = x1;
	x2_ = x2;
	y1_ = y1;
	y2_ = y2;
}

bool Rect::coords_in_shape(int x, int y, int& square) const
{
	square = abs(x1 - x2) * abs(y1 - y2);

	int minX = min(x1, x2);
	int maxX = max(x1, x2);
	int minY = min(y1, y2);
	int maxY = max(y1, y2);

	return (x >= minX && x <= maxX && y >= minY && y <= maxY);
}

Shapes* Rect::copy() const
{
	return new Rect(x1, y1, x2, y2);
}

void Rect::print_to_file(std::ofstream& ofs, bool is_group) const
{
	int minX = min(x1, x2);
	int maxX = max(x1, x2);
	int minY = min(y1, y2);
	int maxY = max(y1, y2);
	if (!is_group)
		ofs << "Rectangle (w: " << maxX - minX << ", h: " << maxY - minY << ")" << std::endl;
	else
		ofs << "  +Rectangle (w: " << maxX - minX << ", h: " << maxY - minY << ")" << std::endl;
}
