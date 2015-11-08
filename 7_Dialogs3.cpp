#include <windows.h>
#include "rc/rc4.h"

HBRUSH dialog_color = NULL;

BOOL CALLBACK D_Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			dialog_color = CreateSolidBrush(RGB(0, 0, 0));
			SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)));
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(NULL, MAKEINTRESOURCE(IDI_APPLICATION)));
		}
		break;
		case WM_CLOSE:
			EndDialog(hwnd, 0);
		break;
		case WM_CTLCOLORDLG:
			return (LONG)dialog_color;
		break;
		case WM_CTLCOLORSTATIC:
		{
			HDC hdc = (HDC)wp;
			SetTextColor(hdc, RGB(255, 255, 255));
			SetBkMode(hdc, TRANSPARENT);
			return (LONG)dialog_color;
		}
		break;
		case WM_COMMAND:
		{
			switch(LOWORD(wp))
			{
				case IDOK:
					EndDialog(hwnd, 0);
				break;
			}
		}
		break;
		case WM_DESTROY:
			DeleteObject(dialog_color);
		break;
		default:
			return FALSE;
	}
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE hpi, LPSTR lp, int ncs)
{
	return DialogBox(hi, MAKEINTRESOURCE(IDD_MAIN), NULL, D_Proc);
}