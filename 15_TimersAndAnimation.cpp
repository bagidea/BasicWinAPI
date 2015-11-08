#include <windows.h>
#include "rc/rc8.h"

const char className[] = "Sample Timers and Animation";

const int ID_TIMER = 1;
const int BALL_MOVE_DELTA = 10;

typedef struct _BALLINFO 
{
	int width;
	int height;
	int x;
	int y;

	int dx;
	int dy;
}BALLINFO;

BALLINFO g_ballInfo;

HBITMAP image = NULL;
HBITMAP imageMask = NULL;

HBITMAP CreateBitmapMask(HBITMAP hbmColour, COLORREF crTransparent)
{
	HDC hdcMem, hdcMem2;
	HBITMAP hbmMask;
	BITMAP bm;

	GetObject(hbmColour, sizeof(BITMAP), &bm);
	hbmMask = CreateBitmap(bm.bmWidth, bm.bmHeight, 1, 1, NULL);

	hdcMem = CreateCompatibleDC(0);
	hdcMem2 = CreateCompatibleDC(0);

	SelectObject(hdcMem, hbmColour);
	SelectObject(hdcMem2, hbmMask);

	SetBkColor(hdcMem, crTransparent);

	BitBlt(hdcMem2, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem, 0, 0, SRCCOPY);

	BitBlt(hdcMem, 0, 0, bm.bmWidth, bm.bmHeight, hdcMem2, 0, 0, SRCINVERT);

	DeleteDC(hdcMem);
	DeleteDC(hdcMem2);

	return hbmMask;
}

void DrawBall(HDC hdc, RECT* prc)
{
	HDC hdcBuffer = CreateCompatibleDC(hdc);
	HBITMAP hbmBuffer = CreateCompatibleBitmap(hdc, prc->right, prc->bottom);
	HBITMAP hbmOldBuffer = (HBITMAP)SelectObject(hdcBuffer, hbmBuffer);

	HDC hdcMem = CreateCompatibleDC(hdc);
	HBITMAP hbmOld = (HBITMAP)SelectObject(hdcMem, imageMask);

	FillRect(hdcBuffer, prc, (HBRUSH)GetStockObject(WHITE_BRUSH));

	BitBlt(hdcBuffer, g_ballInfo.x, g_ballInfo.y, g_ballInfo.width, g_ballInfo.height, hdcMem, 0, 0, SRCAND);

	SelectObject(hdcMem, image);
	BitBlt(hdcBuffer, g_ballInfo.x, g_ballInfo.y, g_ballInfo.width, g_ballInfo.height, hdcMem, 0, 0, SRCPAINT);

	BitBlt(hdc, 0, 0, prc->right, prc->bottom, hdcBuffer, 0, 0, SRCCOPY);

	SelectObject(hdcMem, hbmOld);
	DeleteDC(hdcMem);

	SelectObject(hdcBuffer, hbmOldBuffer);
	DeleteDC(hdcBuffer);
	DeleteObject(hbmBuffer);
}

void UpdateBall(RECT* prc)
{
	g_ballInfo.x += g_ballInfo.dx;
	g_ballInfo.y += g_ballInfo.dy;

	if(g_ballInfo.x < 0)
	{
		g_ballInfo.x = 0;
		g_ballInfo.dx = BALL_MOVE_DELTA;
	}
	else if(g_ballInfo.x + g_ballInfo.width > prc->right)
	{
		g_ballInfo.x = prc->right - g_ballInfo.width;
		g_ballInfo.dx = -BALL_MOVE_DELTA;
	}

	if(g_ballInfo.y < 0)
	{
		g_ballInfo.y = 0;
		g_ballInfo.dy = BALL_MOVE_DELTA;
	}
	else if(g_ballInfo.y + g_ballInfo.height > prc->bottom)
	{
		g_ballInfo.y = prc->bottom - g_ballInfo.height;
		g_ballInfo.dy = -BALL_MOVE_DELTA;
	}
}

LRESULT CALLBACK W_Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_CREATE:
			UINT ret;
			BITMAP bm;

			image = LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BALL));

			if(image == NULL)
				MessageBox(hwnd, "Could not load IDB_BALL!", "Error", MB_OK | MB_ICONEXCLAMATION);

			imageMask = CreateBitmapMask(image, RGB(0, 0, 0));

			if(imageMask == NULL)
				MessageBox(hwnd, "Could not create mask!", "Error", MB_OK | MB_ICONEXCLAMATION);

			GetObject(image, sizeof(bm), &bm);

			ZeroMemory(&g_ballInfo, sizeof(g_ballInfo));

			g_ballInfo.width = bm.bmWidth;
			g_ballInfo.height = bm.bmHeight;
			g_ballInfo.dx = BALL_MOVE_DELTA;
			g_ballInfo.dy = BALL_MOVE_DELTA;

			ret = SetTimer(hwnd, ID_TIMER, 50, NULL);
			
			if(ret == 0)
				MessageBox(hwnd, "Could not SetTimer()!", "Error", MB_OK | MB_ICONEXCLAMATION);
		break;
		case WM_PAINT:
		{
			RECT rcClient;
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hwnd, &ps);

			GetClientRect(hwnd, &rcClient);
			DrawBall(hdc, &rcClient);

			EndPaint(hwnd, &ps);
		}
		break;
		case WM_TIMER:
		{
			RECT rcClient;
			HDC hdc = GetDC(hwnd);

			GetClientRect(hwnd, &rcClient);

			UpdateBall(&rcClient);
			DrawBall(hdc, &rcClient);

			ReleaseDC(hwnd, hdc);
		}
		break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
		break;
		case WM_DESTROY:
			KillTimer(hwnd, ID_TIMER);
			DeleteObject(imageMask);
			DeleteObject(image);
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
		"Sample Timers and Animation",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 600, 440,
		NULL, NULL, hi, NULL
	);

	if(hwnd == NULL)
	{
		MessageBox(NULL, "Create WindowEx Failed", "Error", MB_OK | MB_ICONEXCLAMATION);
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