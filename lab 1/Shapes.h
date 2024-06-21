#pragma once

#include <Windows.h>
#include <fstream>

class Shapes
{
public:
	Shapes() = default;
	Shapes(const Shapes& other) = delete;
	virtual ~Shapes() = default;

	virtual bool operator==(const Shapes& other) const = 0;

	virtual void show(HDC hdc) const = 0;
	virtual void set_coords(int x1_, int y1_, int x2_, int y2_) = 0;
	virtual void get_coords(int& x1_, int& y1_, int& x2_, int& y2_) const = 0;
	virtual bool coords_in_shape(int x_, int y_, int& square) const = 0;
	virtual Shapes* copy() const = 0;
	virtual void print_to_file(std::ofstream& ofs, bool is_group) const = 0;
};

class Circle : public Shapes
{
private:
	int x;
	int y;
	int r;

public:
	Circle(int x_, int y_, int r_);
	Circle(const Circle& other) = default;
	~Circle() override = default;

	virtual bool operator==(const Shapes& other) const override;

	virtual void show(HDC hdc) const override;
	virtual void set_coords(int x1_, int y1_, int x2_, int y2_) override;
	virtual void get_coords(int& x1_, int& y1_, int& x2_, int& y2_) const override;
	virtual bool coords_in_shape(int x, int y, int& square) const override;
	virtual Shapes* copy() const override;
	virtual void print_to_file(std::ofstream& ofs, bool is_group) const override;
};

class Rect : public Shapes
{
private:
	int x1;
	int y1;
	int x2;
	int y2;

public:
	Rect(int x1_, int y1_, int x2_, int y2_);
	Rect(const Rect& other) = default;
	~Rect() override = default;

	virtual bool operator==(const Shapes& other) const override;

	virtual void show(HDC hdc) const override;
	virtual void set_coords(int x1_, int y1_, int x2_, int y2_) override;
	virtual void get_coords(int& x1_, int& y1_, int& x2_, int& y2_) const override;
	virtual bool coords_in_shape(int x, int y, int& square) const override;
	virtual Shapes* copy() const override;
	virtual void print_to_file(std::ofstream& ofs, bool is_group) const override;
};

#include "variables.h"

extern Shapes* shapes[SIZE];
