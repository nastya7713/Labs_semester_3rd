#include "stdafx.h"
#include "Bresenham.h"

#define MAX_LOADSTRING 500

// Global Variables:
HINSTANCE hInst;                                    // current instance
TCHAR szClassName[] = L"CG_Example";                // The title bar text
TCHAR szTitle[] = L"Окружность методом Брэзенхема"; // the main window class name

ATOM MyRegisterClass(HINSTANCE hInstance);
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
typedef void(*TESTFUNCTION)(HWND, int, int, int);

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK Input(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

#define WM_MY_MSG WM_USER+1

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) 
{
	MSG msg;

	if (!MyRegisterClass(hInstance))
	{
		MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
		return NULL;
	}
	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
			TranslateMessage(&msg);
			DispatchMessage(&msg); 
	}

	return (int)msg.wParam;
}

//registers the window class.
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = NULL;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_BRESENHAM);
	wcex.hIconSm = NULL;
	wcex.lpszClassName = szClassName;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd;

	hInst = hInstance; 

	hWnd = CreateWindow(szClassName, szTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{
		MessageBox(NULL, L"Не удается создать главное окно!", L"Ошибка", MB_OK);
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	
	switch (message)
	{
	case WM_COMMAND:

		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);//initialization of about box
			break;
		case IDM_INPUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_INPUT), hWnd, Input);//initialization of input box			
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MOUSEMOVE:
		if ((wParam & MK_LBUTTON) | (wParam & MK_RBUTTON))//describing reaction to a click or a move of left and right mousebuttons and mouse
	{
		int x, y;
		HDC hDC;
		hDC = GetDC(hWnd);
		x = LOWORD(lParam); 
		y = HIWORD(lParam);
		DWORD err;
		HINSTANCE hDll = LoadLibrary(L"Bresenham_circle.dll");//Loads library 
		if (hDll != NULL)//sucessful loading
		{
			//MessageBox(NULL, L"Library was loaded", L"Не ошибка", MB_OK);
		}
		else
		{//error case
			err = GetLastError();
			char buf[MAX_LOADSTRING];
			wsprintf((LPWSTR)buf, L"%d", err);
			MessageBox(NULL, (LPWSTR)buf, L"ошибка", MB_OK);

		}
		TESTFUNCTION lpTestFunction;
		lpTestFunction = (TESTFUNCTION)GetProcAddress(hDll, "drawBresenhamCircle");//getting required fuction from the library
		if (lpTestFunction != NULL)
		{
			(*lpTestFunction) (hWnd, x, y, 100);
		}
		FreeLibrary(hDll);		
	}
		break;
	case WM_MY_MSG:
	{	
		int* ptr = (int*)wParam;
		//input box function
		drawBresenhamCircle(hWnd, ptr[0], ptr[1], ptr[2]);
	break;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
//message handler for input box.
INT_PTR CALLBACK Input(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int hX(0), hY(0), hR(0);
	int parametr[3];
	HWND hParent = GetParent(hDlg);
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		if (LOWORD(wParam) == IDOK)
		{
			
			parametr[0] = hX = GetDlgItemInt(hDlg, IDC_EDIT_X, NULL, NULL);//getting data from X window
			parametr[1] = hY = GetDlgItemInt(hDlg, IDC_EDIT_Y, NULL, NULL);//getting data from Y window
			parametr[2] = hR = GetDlgItemInt(hDlg, IDC_EDIT_R, NULL, NULL);//getting data from R window

			SendMessage(hParent, WM_MY_MSG, (WPARAM)parametr, NULL);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
//message handler for about box.
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