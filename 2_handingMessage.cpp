#include <windows.h>

const char className[] = "Sample Handing Message";

LRESULT CALLBACK W_Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_LBUTTONDOWN:
		{
			MessageBox(NULL, "You Click Left", "Show Message", MB_OK);
		}
		break;
		case WM_RBUTTONDOWN:
		{
			MessageBox(NULL, "You Click Right", "Show Message", MB_OK);
		}
		break;
		case WM_MBUTTONDOWN:
		{
			char filePath[MAX_PATH];
			HINSTANCE hi = GetModuleHandle(NULL);
			GetModuleFileName(hi, filePath, MAX_PATH);
			MessageBox(NULL, filePath, "Show Message", MB_OK);
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
	wc.lpszMenuName = NULL;
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
		"Sample Handing Message",
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