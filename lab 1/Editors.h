#pragma once

#include <Windows.h>

#include "variables.h"

// EDITOR
class Editor
{
protected:
	int x1;
	int y1;
	int x2;
	int y2;

public:
	Editor();
	Editor(const Editor& other) = delete;
	~Editor() = default;

	Editor& operator=(const Editor& other) = delete;

	virtual void LButtonDown(HWND hwnd);
	virtual void LButtonUp(HWND hwnd);
	virtual void onMouseMove(HWND hwnd);
	void Paint(HWND hwnd);
	virtual void onInitMenuPopUp(HWND hwnd, WPARAM wparams);
};

//CIRCLE EDITOR
class CircleEditor : public Editor
{
public:
	CircleEditor() = default;
	CircleEditor(const CircleEditor& other) = delete;
	~CircleEditor() = default;

	CircleEditor& operator=(const CircleEditor& other) = delete;

	virtual void LButtonDown(HWND hwnd) override;
	virtual void LButtonUp(HWND hwnd) override;
	virtual void onMouseMove(HWND hwnd) override;
	virtual void onInitMenuPopUp(HWND hwnd, WPARAM wparams) override;
};

// RECTANGLE EDITOR
class RectEditor : public Editor
{
public:
	RectEditor() = default;
	RectEditor(const RectEditor& other) = delete;
	~RectEditor() = default;

	RectEditor& operator=(const RectEditor& other) = delete;

	virtual void LButtonDown(HWND hwnd) override;
	virtual void LButtonUp(HWND hwnd) override;
	virtual void onMouseMove(HWND hwnd) override;
	virtual void onInitMenuPopUp(HWND hwnd, WPARAM wparams) override;
};

// GROUP EDITOR
class GroupEditor : public Editor
{
public:
	GroupEditor() = default;
	GroupEditor(const GroupEditor& other) = delete;
	~GroupEditor() = default;

	GroupEditor& operator=(const GroupEditor& other) = delete;

	virtual void LButtonDown(HWND hwnd) override;
	virtual void LButtonUp(HWND hwnd) override;
	virtual void onMouseMove(HWND hwnd) override;
	virtual void onInitMenuPopUp(HWND hwnd, WPARAM wparams) override;
};

// MOVE SHAPE EDITOR
class MoveShapeEditor : public Editor
{
private:
	bool moveGroup;
	bool moveShape;
	int groupIndex;
	int shapeIndex;

public:
	MoveShapeEditor();
	MoveShapeEditor(const MoveShapeEditor& other) = delete;
	~MoveShapeEditor() = default;

	MoveShapeEditor& operator=(const MoveShapeEditor& other) = delete;

	virtual void LButtonDown(HWND hwnd) override;
	virtual void LButtonUp(HWND hwnd) override;
	virtual void onMouseMove(HWND hwnd) override;
	virtual void onInitMenuPopUp(HWND hwnd, WPARAM wparams) override;
};

// COPY EDITOR
class CopyEditor : public Editor
{
public:
	CopyEditor() = default;
	CopyEditor(const CopyEditor& other) = delete;
	~CopyEditor() = default;

	CopyEditor& operator=(const CopyEditor& other) = delete;

	virtual void LButtonDown(HWND hwnd) override;
	virtual void LButtonUp(HWND hwnd) override;
	virtual void onMouseMove(HWND hwnd) override;
	virtual void onInitMenuPopUp(HWND hwnd, WPARAM wparams) override;
};


// DELETE EDITOR
class DelEditor : public Editor
{
public:
	DelEditor() = default;
	DelEditor(const DelEditor& other) = delete;
	~DelEditor() = default;

	DelEditor& operator=(const DelEditor& other) = delete;

	virtual void LButtonDown(HWND hwnd) override;
	virtual void LButtonUp(HWND hwnd) override;
	virtual void onMouseMove(HWND hwnd) override;
	virtual void onInitMenuPopUp(HWND hwnd, WPARAM wparams) override;
};
