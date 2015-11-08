#include <windows.h>
#include "rc/rc6.h"

#define IDC_EDIT 101

const char className[] = "Using File and Common dialogs";

BOOL OpenTextFileFromEdit(HWND hwnd, LPCTSTR path)
{
	HANDLE hf;
	BOOL success = FALSE;

	hf = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if(hf != INVALID_HANDLE_VALUE)
	{
		DWORD dw;

		dw = GetFileSize(hf, NULL);

		if(dw != 0xFFFFFFFF)
		{
			LPSTR pt;

			pt = (LPSTR)GlobalAlloc(GPTR, dw+1);

			if(pt != NULL)
			{
				DWORD dwr;

				if(ReadFile(hf, pt, dw, &dwr, NULL))
				{
					pt[dw] = 0;

					if(SetWindowText(hwnd, pt))
						success = TRUE;
				}
				
				GlobalFree(pt);
			}
		}

		CloseHandle(hf);
	}

	return success;
}

void DoFileOpen(HWND hwnd)
{
	OPENFILENAME op;
	char path[MAX_PATH] = "";

	ZeroMemory(&op, sizeof(op));

	op.lStructSize = sizeof(OPENFILENAME);
	op.hwndOwner = hwnd;
	op.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	op.lpstrFile = path;
	op.nMaxFile = MAX_PATH;
	op.lpstrDefExt = "txt";
	op.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if(GetOpenFileName(&op))
	{
		HWND he = GetDlgItem(hwnd, IDC_EDIT);
		OpenTextFileFromEdit(he, path);
	}
}

BOOL SaveTextFileFromEdit(HWND hwnd, LPCTSTR path)
{
	HANDLE hf;
	BOOL success = FALSE;

	hf = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hf != INVALID_HANDLE_VALUE)
	{
		DWORD dw;

		dw = GetWindowTextLength(hwnd);

		if(dw > 0)
		{
			LPSTR pt;
			DWORD dwb = dw + 1;

			pt = (LPSTR)GlobalAlloc(GPTR, dwb);

			if(pt != NULL)
			{
				if(GetWindowText(hwnd, pt, dwb))
				{
					DWORD dww;

					if(WriteFile(hf, pt, dw, &dww, NULL))
						success = TRUE;
				}
				GlobalFree(pt);
			}
		}

		CloseHandle(hf);
	}

	return success;
}

void DoFileSave(HWND hwnd)
{
	OPENFILENAME op;
	char path[MAX_PATH] = "";

	ZeroMemory(&op, sizeof(op));

	op.lStructSize = sizeof(OPENFILENAME);
	op.hwndOwner = hwnd;
	op.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	op.lpstrFile = path;
	op.nMaxFile = MAX_PATH;
	op.lpstrDefExt = "txt";
	op.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if(GetSaveFileName(&op))
	{
		HWND he = GetDlgItem(hwnd, IDC_EDIT);
		SaveTextFileFromEdit(he, path);
	}
}

LRESULT CALLBACK W_Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			HFONT hf;
			HWND he;

			he = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				"Edit",
				"",
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
				0, 0, 100, 100,
				hwnd, (HMENU)IDC_EDIT, GetModuleHandle(NULL), NULL
			);

			if(he == NULL)
				MessageBox(NULL, "Create Window Ex Edit Failed", "Error", MB_OK | MB_ICONEXCLAMATION);

			hf = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(he, WM_SETFONT, (WPARAM)hf, MAKELPARAM(FALSE, 0));
		}
		break;
		case WM_SIZE:
		{
			HWND he;
			RECT rc;

			GetClientRect(hwnd, &rc);

			he = GetDlgItem(hwnd, IDC_EDIT);
			SetWindowPos(he, NULL, 0, 0, rc.right, rc.bottom, SWP_NOZORDER);
		}
		break;
		case WM_COMMAND:
		{
			switch(LOWORD(wp))
			{
				case ID_FILE_NEW:
					SetDlgItemText(hwnd, IDC_EDIT, "");
				break;
				case ID_FILE_LOAD:
					DoFileOpen(hwnd);
				break;
				case ID_FILE_SAVE:
					DoFileSave(hwnd);
				break;
				case ID_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
			}
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
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
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);
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
		"Using File and Common Dialogs",
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
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}