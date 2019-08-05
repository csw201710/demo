#include "tab_other.h"



static void OnCommand(HWND hwnd, UINT uNotifyCode, int nID, HWND wndCtl){
	if (uNotifyCode == 0){
		//menu
	}
	if (uNotifyCode == 1){
		//accelerator
	}
	if (uNotifyCode == BN_CLICKED){
		switch (nID){
		case 0:{

			break;
		}
		case 1:{

			break;
		}
		}
	}
}

// 对话框过程函数
INT_PTR WINAPI tabOther_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	switch (uMsg)
	{
	case WM_COMMAND:
	{
		OnCommand(hwnd, (UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);
		break;
	}

	case WM_INITDIALOG:
	{

		break;
	}

	case WM_SHOWWINDOW:
	{
		break;
	}
	case WM_ERASEBKGND:
		return TRUE;
	case WM_CTLCOLORSTATIC:
		return (INT_PTR)(HBRUSH)::GetStockObject(WHITE_BRUSH);


	case WM_DESTROY:
	{
		PostQuitMessage(NULL);
		break;
	}

	default:
		break;
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}