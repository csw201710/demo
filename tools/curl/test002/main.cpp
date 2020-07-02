#include "main.h"
#include "resource.h"


#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")

//这个用来保存对话框的窗口句柄，因为后面要向这个窗口发消息，必须知道其窗口句柄
HWND	g_hDlgWnd = NULL;

#define TT_HANDLE_DLGMSG(hWnd, message, fn)                 \
   case (message): return (SetDlgMsgResult(hWnd, uMsg,     \
      HANDLE_##message((hWnd), (wParam), (lParam), (fn))))


enum {
	HANDLE_WM_COMMAND
};
/*
WM_COMMAND 消息来源	WPARAM 高位	WPARAM 低位	LPARAM
菜单 				0 		菜单 ID 	0
快捷键 				1 		快捷键对应菜单 ID 	0
控件 				响应 Code（如BN_CLICKED） 	控件 ID 	控件句柄
*/
static void OnCommand(HWND hwnd,UINT uNotifyCode, int nID, HWND wndCtl){
	if (uNotifyCode == 0){ 
		//menu
		

	}
	if (uNotifyCode == 1){
		//accelerator
	}
	if (uNotifyCode == BN_CLICKED){
		switch (nID){
		case ID_BTN_DOWNLOAD:{
			HANDLE hThread = CreateThread(NULL, 0, showDownloadDlg, 0, 0, NULL);
			CloseHandle(hThread);
			break;
		}
		case ID_BTN_UPLOAD:{

			break;
		}
		}
	}
}

// 对话框过程函数
static INT_PTR WINAPI MainDlg_Proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;

	switch (uMsg)
	{
	case WM_COMMAND:
	{
		OnCommand(hwnd,(UINT)HIWORD(wParam), (int)LOWORD(wParam), (HWND)lParam);
		break;
	}

	case WM_INITDIALOG:
	{
		CenterWindow(hwnd);
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
	
	case WM_NCACTIVATE:
	{
		break;
	}
	
	case WM_NCPAINT:
	{
		break;
	}
	
	case WM_CTLCOLORBTN:
	{

		break;
	}
	
	case WM_PAINT:
	{


		break;
	}
	
	case WM_CREATE:
	{
		break;
	}
	
	
	
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

int CALLBACK WinMain(
	_In_  HINSTANCE hInstance,
	_In_  HINSTANCE hPrevInstance,
	_In_  LPSTR lpCmdLine,
	_In_  int nCmdShow
	)
{
	//弹出对话框，知道对话框关闭才会执行退出
	//DialogBoxParam(NULL, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, MainDlg_Proc, 0);
	DialogBox(NULL, MAKEINTRESOURCE(IDD_DLG_MAIN), NULL, MainDlg_Proc);
	return 0;

}







void CenterWindow(HWND hwnd){
	int scrWidth, scrHeight;
	RECT rect;
	//获得屏幕尺寸
	scrWidth = GetSystemMetrics(SM_CXSCREEN);
	scrHeight = GetSystemMetrics(SM_CYSCREEN);
	//取得窗口尺寸
	GetWindowRect(hwnd, &rect);
	//重新设置rect里的值
	rect.left = (scrWidth - rect.right) / 2;
	rect.top = (scrHeight - rect.bottom) / 2;
	//移动窗口到指定的位置
	SetWindowPos(hwnd, HWND_TOP, rect.left, rect.top, rect.right, rect.bottom, SWP_SHOWWINDOW);

}

