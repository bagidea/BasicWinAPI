#include <windows.h>
#include "rc/rc3.h"

const char className[] = "Sample Dialogs Box 2";
HWND dialog_pop = NULL;

BOOL CALLBACK D_Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_COMMAND:
		{
			switch(LOWORD(wp))
			{
				case IDC_PRESSBUTTON1:
					MessageBox(hwnd, "Hello Samsung BagIdea.", "Press Button 1", MB_OK | MB_ICONEXCLAMATION);
				break;
				case IDC_PRESSBUTTON2:
					MessageBox(hwnd, "Sample Press Button 2.", "Press Button 2", MB_OK | MB_ICONEXCLAMATION);
				break;
			}
		}
		break;
		default:
			return FALSE;
	}
}

LRESULT CALLBACK W_Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			dialog_pop = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_POPUPMENU), hwnd, D_Proc);

			if(dialog_pop !=  NULL)
				ShowWindow(dialog_pop, SW_SHOW);
			else
				MessageBox(hwnd, "Create Dialog Box Failed.", "Error", MB_OK | MB_ICONINFORMATION);
		}
		break;
		case WM_COMMAND:
		{
			switch(LOWORD(wp))
			{
				case ID_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
				case ID_DIALOG_SHOW:
					ShowWindow(dialog_pop, SW_SHOW);
				break;
				case ID_DIALOG_HIDE:
					ShowWindow(dialog_pop, SW_HIDE);
				break;
			}
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			DestroyWindow(dialog_pop);
			PostQuitMessage(0);
		break;
		default:
			return DefWindowProc(hwnd, msg, wp, lp);
	}
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE hpi, LPSTR lp, int ncs)
{
	WNDCLASSEX wc;
	HWND hwnd;
	MSG msg;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = W_Proc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hi;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MYMENU);
	wc.lpszClassName = className;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Register Failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		"Sample Dialogs 2",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
		NULL, NULL, hi, NULL
	);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Create Window Ex Failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	ShowWindow(hwnd, ncs);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if(!IsDialogMessage(dialog_pop, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}