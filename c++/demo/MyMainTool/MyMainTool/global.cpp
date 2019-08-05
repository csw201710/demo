#include "global.h"



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
	SetWindowPos(hwnd, HWND_TOP, rect.left+10, rect.top+20, rect.right, rect.bottom, SWP_SHOWWINDOW);

}
