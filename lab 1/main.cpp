#include "framework.h"
#include "ShapeEditor.h"
#include "Group.h"
#include "variables.h"
#include "resource.h"

#define MAX_LOADSTRING 100

ShapeEditor se;

int temp = 0;
const std::string fileName = "ShapesGroupsInfo.txt";

// Global Variables:
HINSTANCE hInst;                                // ,current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB1, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB1));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);

            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
                
            case ID_OBJECTS_CIRCLE:
                se.StartCircleEditor();
                break;

            case ID_OBJECTS_RECTANGLE:
                se.StartRectEditor();
                break;

            case ID_OBJECTS_GROUP:
                se.StartGroupEditor();
                break;

            case ID_OBJECTS_MOVE:
                se.StartMoveEditor();
                break;

            case ID_OBJECTS_COPY:
                se.StartCopyEditor();
                break;

            case ID_OBJECTS_DELETE:
                se.StartDelEditor();
                break;

            case ID_FILE_OUTPUTINFOINTOFILE:
                if (!se.OutputInfoToFile(fileName))
                {
                    MessageBox(
                        hWnd,
                        L"Error. You havent any shapes and groups or file cant be opened\n",
                        L"ERROR",
                        MB_OK | MB_ICONERROR
                    );
                }
                else
                {
                    MessageBox(
                        hWnd,
                        L"Information about shapes and groups added to file successfully\n",
                        L"Success",
                        MB_OK | MB_ICONINFORMATION
                    );
                }
                break;

            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            }

            temp = 0;
            del_shape_from_group = false;
            while (temp != maxPos)
            {
                if (wmId == COPYGR + temp * 3)
                {
                    se.CopyGroup(hWnd, COPYGR + temp * 3);
                    break;
                }
                else if (wmId == DELSGR + temp * 3)
                {
                    se.unCheckedMenuPopUp(hWnd);
                    del_shape_from_group = true;
                    break;
                }
                else if (wmId == DELGR + temp * 3)
                {
                    se.unCheckedMenuPopUp(hWnd);
                    se.DelGroup(hWnd, DELGR + temp * 3);
                }
                ++temp;
            }

            return DefWindowProc(hWnd, message, wParam, lParam);
        }
        break;

    case WM_LBUTTONDOWN:
        if (del_shape_from_group)
            se.DelShapeFromGroup(hWnd, DELSGR + temp * 3);
        else
            se.LButtonDown(hWnd);
        break;

    case WM_LBUTTONUP:
        if (!del_shape_from_group)
            se.LButtonUp(hWnd);
        break;

    case WM_MOUSEMOVE:
        se.onMouseMove(hWnd);
        break;

    case WM_INITMENUPOPUP:
        se.onInitMenuPopUp(hWnd, wParam);
        break;

    case WM_PAINT:
        se.Paint(hWnd);
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
