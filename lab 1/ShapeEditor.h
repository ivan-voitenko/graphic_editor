#pragma once

#include "Windows.h"
#include "Shapes.h"
#include "Editors.h"

#include <string>
#include <fstream>

class ShapeEditor
{
private:
	Editor* ed;

	int getPosCheckedGroupMenu(HWND hwnd, int ID);

public:
	ShapeEditor() = default;
	ShapeEditor(const ShapeEditor& other) = default;
	~ShapeEditor();

	bool OutputInfoToFile(const std::string& fileName);

	void LButtonDown(HWND hwnd);
	void LButtonUp(HWND hwnd);
	void onMouseMove(HWND hwnd);
	void Paint(HWND hwnd);
	void onInitMenuPopUp(HWND hwnd, WPARAM wparams);

	void StartCircleEditor();
	void StartRectEditor();
	void StartGroupEditor();
	void StartMoveEditor();
	void StartCopyEditor();
	void StartDelEditor();

	void CopyGroup(HWND hwnd, int pos);
	void DelShapeFromGroup(HWND hwnd, int pos);
	void DelGroup(HWND hwnd, int pos);
	void unCheckedMenuPopUp(HWND hwnd);
};
