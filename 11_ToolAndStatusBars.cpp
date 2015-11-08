#include <windows.h>
#include <commctrl.h>
#include "rc/rc6.h"

#define IDC_EDIT 1001
#define IDC_TOOL 1002
#define IDC_STATUS 1003

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
		if(OpenTextFileFromEdit(he, path))
		{
			SendDlgItemMessage(hwnd, IDC_STATUS, SB_SETTEXT, 0, (LPARAM)"Opened...");
			SendDlgItemMessage(hwnd, IDC_STATUS, SB_SETTEXT, 1, (LPARAM)path);
		}
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
		if(SaveTextFileFromEdit(he, path))
		{
			SendDlgItemMessage(hwnd, IDC_STATUS, SB_SETTEXT, 0, (LPARAM)"Saved...");
			SendDlgItemMessage(hwnd, IDC_STATUS, SB_SETTEXT, 1, (LPARAM)path);
		}
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
			HWND ht;
			TBBUTTON tab[3];
			TBADDBITMAP ab;
			HWND hs;
			int statwidths[] = {100, -1};

			//Edit
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

			//Tool
			ht = CreateWindowEx(
				0,
				TOOLBARCLASSNAME,
				NULL,
				WS_CHILD | WS_VISIBLE,
				0, 0, 0, 0,
				hwnd, (HMENU)IDC_TOOL, GetModuleHandle(NULL), NULL
			);

			if(ht == NULL)
				MessageBox(hwnd, "Could not create tool bar.", "Error", MB_OK | MB_ICONERROR);

			SendMessage(ht, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);
			
			ab.hInst = HINST_COMMCTRL;
			ab.nID = IDB_STD_SMALL_COLOR;
			SendMessage(ht, TB_ADDBITMAP, 0, (LPARAM)&ab);

			ZeroMemory(tab, sizeof(tab));

			tab[0].iBitmap = STD_FILENEW;
			tab[0].fsState = TBSTATE_ENABLED;
			tab[0].fsStyle = TBSTYLE_BUTTON;
			tab[0].idCommand = ID_FILE_NEW;

			tab[1].iBitmap = STD_FILEOPEN;
			tab[1].fsState = TBSTATE_ENABLED;
			tab[1].fsStyle = TBSTYLE_BUTTON;
			tab[1].idCommand = ID_FILE_LOAD;

			tab[2].iBitmap = STD_FILESAVE;
			tab[2].fsState = TBSTATE_ENABLED;
			tab[2].fsStyle = TBSTYLE_BUTTON;
			tab[2].idCommand = ID_FILE_SAVE;

			SendMessage(ht, TB_ADDBUTTONS, sizeof(tab)/sizeof(TBBUTTON), (LPARAM)&tab);

			//Status
			hs = CreateWindowEx(
				0,
				STATUSCLASSNAME,
				NULL,
				WS_CHILD | WS_VISIBLE | SBARS_SIZEGRIP,
				0, 0, 0, 0,
				hwnd, (HMENU)IDC_STATUS, GetModuleHandle(NULL), NULL
			);

			SendMessage(hs, SB_SETPARTS, sizeof(statwidths)/sizeof(int), (LPARAM)statwidths);
			SendMessage(hs, SB_SETTEXT, 0, (LPARAM)"Hi there :)");
		}
		break;
		case WM_SIZE:
		{
			HWND ht;
			RECT rct;
			int itool;

			HWND hs;
			RECT rcs;
			int istatus;

			HWND he;
			RECT rcc;
			int iedit;

			//Tool
			ht = GetDlgItem(hwnd, IDC_TOOL);
			SendMessage(ht, TB_AUTOSIZE, 0, 0);

			GetWindowRect(ht, &rct);
			itool = rct.bottom - rct.top;

			//Status
			hs = GetDlgItem(hwnd, IDC_STATUS);
			SendMessage(hs, WM_SIZE, 0, 0);

			GetWindowRect(hs, &rcs);
			istatus = rcs.bottom - rcs.top;

			//Edit
			GetClientRect(hwnd, &rcc);

			iedit = rcc.bottom - itool - istatus;

			he = GetDlgItem(hwnd, IDC_EDIT);
			SetWindowPos(he, NULL, 0, itool, rcc.right, iedit, SWP_NOZORDER);
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