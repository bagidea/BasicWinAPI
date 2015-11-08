#include <windows.h>
#include "rc/rc5.h"

BOOL CALLBACK D_Proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch(msg)
	{
		case WM_INITDIALOG:
		{
			SetDlgItemText(hwnd, IDC_TEXT, "");
			SetDlgItemInt(hwnd, IDC_NUMBER, 1, FALSE);
		}
		break;
		case WM_COMMAND:
		{
			switch(LOWORD(wp))
			{
				case IDC_ADD:
				{
					BOOL success;
					int num = GetDlgItemInt(hwnd, IDC_NUMBER, &success, FALSE);

					if(success)
					{
						int l = GetWindowTextLength(GetDlgItem(hwnd, IDC_TEXT));

						if(l > 0)
						{
							int i;
							char* buf;

							buf = (char*)GlobalAlloc(GPTR, l+1);
							GetDlgItemText(hwnd, IDC_TEXT, buf, l+1);

							for(i = 0; i < num; i++)
							{
								int ind = SendDlgItemMessage(hwnd, IDC_LIST, LB_ADDSTRING, 0, (LPARAM)buf);
								SendDlgItemMessage(hwnd, IDC_LIST, LB_SETITEMDATA, (WPARAM)ind, (LPARAM)num);
							}

							GlobalFree((HANDLE)buf);
						}else{
							MessageBox(hwnd, "Please your enter text.", "Warning", MB_OK);
						}
					}else{
						MessageBox(hwnd, "Couldn't translate that number :(", "Warning", MB_OK);
					}

					SetDlgItemText(hwnd, IDC_TEXT, "");
					SetDlgItemText(hwnd, IDC_SHOWCOUNT, "-");
				}
				break;
				case IDC_REMOVE:
				{
					HWND lis = GetDlgItem(hwnd, IDC_LIST);
					int count = SendMessage(lis, LB_GETSELCOUNT, 0, 0);

					if(count != LB_ERR)
					{
						if(count > 0)
						{
							int i;
							int* buf = (int*)GlobalAlloc(GPTR, sizeof(int)*count);
							SendMessage(lis, LB_GETSELITEMS, (WPARAM)count, (LPARAM)buf);

							for(i = count - 1; i >= 0; i--)
							{
								SendMessage(lis, LB_DELETESTRING, (WPARAM)buf[i], 0);
							}

							GlobalFree(buf);
						}else{
							MessageBox(hwnd, "No items selected.", "Warning", MB_OK);
						}
					}else{
						MessageBox(hwnd, "Error counting items :(", "Warning", MB_OK);
					}

					SetDlgItemText(hwnd, IDC_SHOWCOUNT, "-");
				}
				break;
				case IDC_CLEAR:
				{
					SendDlgItemMessage(hwnd, IDC_LIST, LB_RESETCONTENT, 0, 0);
					SetDlgItemText(hwnd, IDC_SHOWCOUNT, "-");
				}
				break;
				case IDC_LIST:
				{
					switch(HIWORD(wp))
					{
						case LBN_SELCHANGE:
						{
							HWND lis = GetDlgItem(hwnd, IDC_LIST);
							int count = SendMessage(lis, LB_GETSELCOUNT, 0, 0);
							
							if(count != LB_ERR)
							{
								if(count == 1)
								{
									int ind;
									int err = SendMessage(lis, LB_GETSELITEMS, (WPARAM)1, (LPARAM)&ind);

									if(err != LB_ERR)
									{
										int data = SendMessage(lis, LB_GETITEMDATA, (WPARAM)ind, 0);
										SetDlgItemInt(hwnd, IDC_SHOWCOUNT, data, FALSE);
									}else{
										MessageBox(hwnd, "Error getting selected item :(", "Warning", MB_OK);
									}
								}else{
									SetDlgItemText(hwnd, IDC_SHOWCOUNT, "-");
								}
							}else{
								MessageBox(hwnd, "Error counting items :(", "Warning", MB_OK);
							}
						}
						break;
					}
				}
				break;
			}
		}
		break;
		case WM_CLOSE:
			EndDialog(hwnd, 0);
		break;
		default:
			return FALSE;
	}
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE hpi, LPSTR lp, int ncs)
{
	return DialogBox(hi, MAKEINTRESOURCE(IDD_MAIN), NULL, D_Proc);	
}