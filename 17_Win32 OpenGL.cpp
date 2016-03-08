#include <Windows.h>
#include <GL\glew.h>

const int screenWidth = 800;
const int screenHeight = 600;

const char* className = "Sample Win32 OpenGL";
HWND _hwnd = NULL;
MSG _msg;

HDC _hdc;
HGLRC _hrc;

bool running = true;

HRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		running = false;
		break;
	default:
		return DefWindowProc(hwnd, msg, wp, lp);
	}
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR, int)
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	wc.cbWndExtra = 0;
	wc.cbClsExtra = 0;
	wc.hInstance = hi;
	wc.hIcon = LoadIcon(hi, IDI_APPLICATION);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(hi, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpfnWndProc = WndProc;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = className;

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Error! Can't register class ex.", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	_hwnd = CreateWindow(
		className, 
		className,
		WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX,
		(GetSystemMetrics(SM_CXSCREEN)-screenWidth)/2, (GetSystemMetrics(SM_CYSCREEN)-screenHeight)/2, screenWidth, screenHeight,
		NULL, NULL, hi, NULL
	);

	if (_hwnd == NULL)
	{
		MessageBox(NULL, "Error! Can't Create window.", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}

	int pixelFormat;

	_hdc = GetDC(_hwnd);

	PIXELFORMATDESCRIPTOR pfd;

	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.dwFlags = PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER;
	pfd.cDepthBits = 32;
	pfd.cColorBits = 24;
	pfd.iPixelType = PFD_TYPE_RGBA;

	pixelFormat = ChoosePixelFormat(_hdc, &pfd);

	if (!SetPixelFormat(_hdc, pixelFormat, &pfd))
		MessageBox(NULL, "Error! Set Pixel Format.", "Error", MB_OK | MB_ICONERROR);

	_hrc = wglCreateContext(_hdc);
	wglMakeCurrent(_hdc, _hrc);

	while (running)
	{
		if (PeekMessage(&_msg, NULL, 0, 0, PM_REMOVE))
		{
			DispatchMessage(&_msg);
			TranslateMessage(&_msg);
		}else{
			//Render
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
			SwapBuffers(_hdc);
		}
	}

	wglMakeCurrent(_hdc, _hrc);
	wglDeleteContext(_hrc);

	if (ReleaseDC(_hwnd, _hdc))
		MessageBox(NULL, "Error! Can not Release device context.", "Error", MB_OK | MB_ICONERROR);

	return _msg.wParam;
}