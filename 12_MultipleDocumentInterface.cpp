#include <windows.h>
#include <commctrl.h>
#include "rc/rc7.h"

#define IDC_MAIN 101
#define IDC_CHILD_EDIT	101
#define IDC_TOOL 102
#define IDC_STATUS 103
#define ID_MDI_FIRSTCHILD 50000

HWND g_hMDIClient = NULL;
HWND g_hMainWindow = NULL;

const char className[] = "Sample Multiple Document Interface";
const char classClient[] = "Class Client";

BOOL LoadTextFileToEdit(HWND hwnd, LPCTSTR path)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(path, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwFileSize;

		dwFileSize = GetFileSize(hFile, NULL);

		if(dwFileSize != 0xFFFFFFFF)
		{
			LPSTR pszFileText;
			pszFileText = (LPSTR)GlobalAlloc(GPTR, dwFileSize + 1);

			if(pszFileText != NULL)
			{
				DWORD dwRead;

				if(ReadFile(hFile, pszFileText, dwFileSize, &dwRead, NULL))
				{
					pszFileText[dwFileSize] = 0;

					if(SetWindowText(hwnd, pszFileText))
						bSuccess = TRUE;
				}

				GlobalFree(pszFileText);
			}
		}

		CloseHandle(hFile);
	}

	return bSuccess;
}

void DoFileOpen(HWND hwnd)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
	ofn.lpstrDefExt = "txt";

	if(GetOpenFileName(&ofn))
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_CHILD_EDIT);
		if(LoadTextFileToEdit(hEdit, szFileName))
		{
			SendDlgItemMessage(g_hMainWindow, IDC_STATUS, SB_SETTEXT, 0, (LPARAM)"Opened...");
			SendDlgItemMessage(g_hMainWindow, IDC_STATUS, SB_SETTEXT, 1, (LPARAM)szFileName);

			SetWindowText(hwnd, szFileName);
		}
	}
}

BOOL SaveTextFileFromEdit(HWND hwnd, LPCTSTR path)
{
	HANDLE hFile;
	BOOL bSuccess = FALSE;

	hFile = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if(hFile != INVALID_HANDLE_VALUE)
	{
		DWORD dwTextLength;
		dwTextLength = GetWindowTextLength(hwnd);

		if(dwTextLength > 0)
		{
			LPSTR pszText;
			DWORD dwBufferSize = dwTextLength + 1;

			pszText = (LPSTR)GlobalAlloc(GPTR, dwBufferSize);

			if(pszText != NULL)
			{
				if(GetWindowText(hwnd, pszText, dwBufferSize))
				{
					DWORD dwWritten;

					if(WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
						bSuccess = TRUE;
				}

				GlobalFree(pszText);
			}
		}

		CloseHandle(hFile);
	}
	return bSuccess;
}

void DoFileSave(HWND hwnd)
{
	OPENFILENAME ofn;
	char szFileName[MAX_PATH] = "";

	ZeroMemory(&ofn, sizeof(ofn));

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = "Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
	ofn.lpstrFile = szFileName;
	ofn.nMaxFile = MAX_PATH;
	ofn.lpstrDefExt = "txt";
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;

	if(GetSaveFileName(&ofn))
	{
		HWND hEdit = GetDlgItem(hwnd, IDC_CHILD_EDIT);
		if(SaveTextFileFromEdit(hEdit, szFileName))
		{
			SendDlgItemMessage(g_hMainWindow, IDC_STATUS, SB_SETTEXT, 0, (LPARAM)"Saved...");
			SendDlgItemMessage(g_hMainWindow, IDC_STATUS, SB_SETTEXT, 1, (LPARAM)szFileName);

			SetWindowText(hwnd, szFileName);
		}
	}
}

HWND NewClient(HWND hwnd)
{
	MDICREATESTRUCT mcs;
	HWND hChild;

	mcs.szTitle = "[Untitled]";
	mcs.szClass = classClient;
	mcs.hOwner  = GetModuleHandle(NULL);
	mcs.x = mcs.cx = CW_USEDEFAULT;
	mcs.y = mcs.cy = CW_USEDEFAULT;
	mcs.style = MDIS_ALLCHILDSTYLES;

	hChild = (HWND)SendMessage(hwnd, WM_MDICREATE, 0, (LONG)&mcs);
	if(!hChild)
	{
		MessageBox(hwnd, "MDI Child creation failed.", "Oh Oh...",
			MB_ICONEXCLAMATION | MB_OK);
	}
	return hChild;
}

LRESULT CALLBACK W_Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			HWND ht;
			TBBUTTON tab[3];
			TBADDBITMAP ab;
			HWND hs;
			int statwidths[] = {100, -1};
			CLIENTCREATESTRUCT ccs;

			//Client
			ccs.hWindowMenu  = GetSubMenu(GetMenu(hwnd), 2);
			ccs.idFirstChild = ID_MDI_FIRSTCHILD;

			g_hMDIClient = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				"mdiclient",
				NULL,
				WS_CHILD | WS_CLIPCHILDREN | WS_VSCROLL | WS_HSCROLL | WS_VISIBLE,
				CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
				hwnd, (HMENU)IDC_MAIN, GetModuleHandle(NULL), (LPVOID)&ccs
			);

			if(g_hMDIClient == NULL)
				MessageBox(hwnd, "Could not create MDI client.", "Error", MB_OK | MB_ICONERROR);

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
			tab[2].idCommand = ID_FILE_SAVEAS;

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

			HWND hMDI;
			int iMDIHeight;
			RECT rcClient;

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

			//Client
			GetClientRect(hwnd, &rcClient);

			iMDIHeight = rcClient.bottom - itool - istatus;

			hMDI = GetDlgItem(hwnd, IDC_MAIN);
			SetWindowPos(hMDI, NULL, 0, itool, rcClient.right, iMDIHeight, SWP_NOZORDER);
		}
		break;
		case WM_COMMAND:
		{
			switch(LOWORD(wp))
			{
				case ID_FILE_NEW:
					NewClient(g_hMDIClient);
				break;
				case ID_FILE_LOAD:
				{
					HWND hChild = NewClient(g_hMDIClient);

					if(hChild)
						DoFileOpen(hChild);
				}
				break;
				case ID_FILE_CLOSE:
				{
					HWND hChild = (HWND)SendMessage(g_hMDIClient, WM_MDIGETACTIVE,0,0);

					if(hChild)
						SendMessage(hChild, WM_CLOSE, 0, 0);
				}
				break;
				case ID_WINDOW_TILE:
					SendMessage(g_hMDIClient, WM_MDITILE, 0, 0);
				break;
				case ID_WINDOW_CASCADE:
					SendMessage(g_hMDIClient, WM_MDICASCADE, 0, 0);
				break;
				case ID_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
				default:
				{
					if(LOWORD(wp) >= ID_MDI_FIRSTCHILD)
					{
						DefFrameProc(hwnd, g_hMDIClient, WM_COMMAND, wp, lp);
					}
					else 
					{
						HWND hChild = (HWND)SendMessage(g_hMDIClient, WM_MDIGETACTIVE,0,0);

						if(hChild)
							SendMessage(hChild, WM_COMMAND, wp, lp);
					}
				}
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

LRESULT CALLBACK MDIChildWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			HFONT hfDefault;
			HWND hEdit;

			hEdit = CreateWindowEx(
				WS_EX_CLIENTEDGE,
				"EDIT",
				"",
				WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL,
				0, 0, 100, 100,
				hwnd, (HMENU)IDC_CHILD_EDIT, GetModuleHandle(NULL), NULL
			);

			if(hEdit == NULL)
				MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

			hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
			SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
		}
		break;
		case WM_MDIACTIVATE:
		{
			HMENU hMenu, hFileMenu;
			UINT EnableFlag;

			hMenu = GetMenu(g_hMainWindow);

			if(hwnd == (HWND)lParam)
				EnableFlag = MF_ENABLED;
			else
				EnableFlag = MF_GRAYED;

			EnableMenuItem(hMenu, 1, MF_BYPOSITION | EnableFlag);
			EnableMenuItem(hMenu, 2, MF_BYPOSITION | EnableFlag);

			hFileMenu = GetSubMenu(hMenu, 0);
			EnableMenuItem(hFileMenu, ID_FILE_SAVEAS, MF_BYCOMMAND | EnableFlag);

			EnableMenuItem(hFileMenu, ID_FILE_CLOSE, MF_BYCOMMAND | EnableFlag);
			EnableMenuItem(hFileMenu, ID_FILE_CLOSEALL, MF_BYCOMMAND | EnableFlag);

			DrawMenuBar(g_hMainWindow);
		}
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case ID_FILE_LOAD:
					DoFileOpen(hwnd);
				break;
				case ID_FILE_SAVEAS:
					DoFileSave(hwnd);
				break;
				case ID_EDIT_CUT:
					SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_CUT, 0, 0);
				break;
				case ID_EDIT_COPY:
					SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_COPY, 0, 0);
				break;
				case ID_EDIT_PASTE:
					SendDlgItemMessage(hwnd, IDC_CHILD_EDIT, WM_PASTE, 0, 0);
				break;
			}
		break;
		case WM_SIZE:
		{
			HWND hEdit;
			RECT rcClient;

			GetClientRect(hwnd, &rcClient);

			hEdit = GetDlgItem(hwnd, IDC_CHILD_EDIT);
			SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
		}
		return DefMDIChildProc(hwnd, msg, wParam, lParam);
		default:
			return DefMDIChildProc(hwnd, msg, wParam, lParam);
	}
}

BOOL SetUpMDIChildWindowClass(HINSTANCE hi)
{
	WNDCLASSEX wc;

	wc.cbSize		 = sizeof(WNDCLASSEX);
	wc.style		 = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc	 = MDIChildWndProc;
	wc.cbClsExtra	 = 0;
	wc.cbWndExtra	 = 0;
	wc.hInstance	 = hi;
	wc.hIcon		 = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
	wc.lpszMenuName  = NULL;
	wc.lpszClassName = classClient;
	wc.hIconSm		 = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(0, "Could Not Register Child Window", "Oh Oh...", MB_ICONEXCLAMATION | MB_OK);
		return FALSE;
	}
	else
		return TRUE;
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
	wc.cbWndExtra  = 0;
	wc.hInstance = hi;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_3DFACE+1);
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
	wc.lpszClassName = className;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Register Failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	if(!SetUpMDIChildWindowClass(hi))
		return 0;

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		"Multiple Document Interface",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 400,
		NULL, NULL, hi, NULL
	);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Create Window Ex Failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	g_hMainWindow = hwnd;

	ShowWindow(hwnd, ncs);
	UpdateWindow(hwnd);

	while(GetMessage(&msg, NULL, 0, 0) > 0)
	{
		if (!TranslateMDISysAccel(g_hMDIClient, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}