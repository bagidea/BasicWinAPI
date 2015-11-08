#include <windows.h>

#define ID_FILE_EXIT 9001
#define ID_HELP_ABOUT 9002

const char className[] = "Sample Menu And Icon";

LRESULT CALLBACK W_Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_CREATE:
		{
			HMENU hm, hsm;
			HICON hi, his;

			hm = CreateMenu();
			hsm = CreatePopupMenu();
			AppendMenu(hsm, MF_STRING, ID_FILE_EXIT, "&Exit");
			AppendMenu(hm, MF_STRING | MF_POPUP, (UINT)hsm, "&File");

			hsm = CreatePopupMenu();
			AppendMenu(hsm, MF_STRING, ID_HELP_ABOUT, "&About");
			AppendMenu(hm, MF_STRING | MF_POPUP, (UINT)hsm, "&Help");
		
			SetMenu(hwnd, hm);

			hi = (HICON)LoadImage(NULL, "resource/icon.ico", IMAGE_ICON, 32, 32, LR_LOADFROMFILE);
			if(hi)
				SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hi);
			else 
				MessageBox(hwnd, "Could not load large icon", "Error", MB_OK | MB_ICONERROR);

			his = (HICON)LoadImage(NULL, "resource/icon.ico", IMAGE_ICON, 16, 16, LR_LOADFROMFILE);
			if(his)
				SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)his);
			else
				MessageBox(hwnd, "Could not load small icon", "Error", MB_OK | MB_ICONERROR);
		}
		break;
		case WM_COMMAND:
		{
			switch(LOWORD(wp))
			{
				case ID_FILE_EXIT:
					PostMessage(hwnd, WM_CLOSE, 0, 0);
				break;
				case ID_HELP_ABOUT:
					MessageBox(NULL, "Hello. I'm Samsung BagIdea ^ ^", "About", MB_OK);
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
	wc.hIcon = NULL;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;
	wc.hIconSm = NULL;

	if(!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Register Failed", "Error", MB_OK | MB_ICONEXCLAMATION);
		return 0;
	}

	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		className,
		"Sample Menu And Icon",
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