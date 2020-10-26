void CemuleDlg::AddLogLine(bool addtostatusbar,char* line,...){
	va_list argptr;
	char bufferline[500];
	va_start(argptr, line);
	_vsnprintf(bufferline, 500, line, argptr);
	va_end(argptr);
	logtext += bufferline;
	logtext += "\r\n";
	if (ready){
		// logbox CEdit
		serverwnd.logbox.SetWindowText(logtext);
		serverwnd.logbox.LineScroll(serverwnd.logbox.GetLineCount());
	if (addtostatusbar)
		statusbar.SetText(bufferline,0,0);
	}

}
void CemuleDlg::AddServerMessageLine(char* line,...){
	CString content;
	va_list argptr;
	char bufferline[500];
	va_start(argptr, line);
	_vsnprintf(bufferline, 500, line, argptr);
	va_end(argptr);
	//servermsgbox CEdit
	serverwnd.servermsgbox.GetWindowText(content);
	content += bufferline;
	content += "\r\n";
	serverwnd.servermsgbox.SetWindowText(content);
	serverwnd.servermsgbox.LineScroll(serverwnd.servermsgbox.GetLineCount());
}




//=========================================================================================//
#define MAX_LOG_LINES		500	/* Max number of lines in LogWindow */
#define DEL_LOG_LINES		10	/* Number of lines to delete from LogWindow */
/*
 * Write a line to the status log window and optionally to the log file
 */
void
WriteStatusLog (connection_t *c, const WCHAR *prefix, const WCHAR *line, BOOL fileio)
{
    HWND logWnd = GetDlgItem(c->hwndStatus, ID_EDT_LOG);
    FILE *log_fd;
    time_t now;
    WCHAR datetime[26];

    time (&now);
    /* TODO: change this to use _wctime_s when mingw supports it */
    wcsncpy (datetime, _wctime(&now), _countof(datetime));
    datetime[24] = L' ';

    /* Remove lines from log window if it is getting full */
    if (SendMessage(logWnd, EM_GETLINECOUNT, 0, 0) > MAX_LOG_LINES)//获取编辑框的行数
    {
		// wParam 指定从零开始的行号。 值–1指定当前行号
		//返回值是line参数中指定的行的字符索引，如果指定的行数大于edit控件中的行数，则返回值为–1。
        int pos = SendMessage(logWnd, EM_LINEINDEX, DEL_LOG_LINES, 0);
        SendMessage(logWnd, EM_SETSEL, 0, pos);// 对Edit输入控件中设置文本选取
        SendMessage(logWnd, EM_REPLACESEL, FALSE, (LPARAM) _T(""));
    }
    /* Append line to log window */
    SendMessage(logWnd, EM_SETSEL, (WPARAM) -1, (LPARAM) -1);
    SendMessage(logWnd, EM_REPLACESEL, FALSE, (LPARAM) datetime);// WPARAM FALSE 标识无法撤销
    SendMessage(logWnd, EM_REPLACESEL, FALSE, (LPARAM) prefix);
    SendMessage(logWnd, EM_REPLACESEL, FALSE, (LPARAM) line);
    SendMessage(logWnd, EM_REPLACESEL, FALSE, (LPARAM) L"\n");

    if (!fileio) return;

    log_fd = _tfopen (c->log_path, TEXT("at+,ccs=UTF-8"));
    if (log_fd)
    {
        fwprintf (log_fd, L"%s%s%s\n", datetime, prefix, line);
        fclose (log_fd);
    }
}

//=========================================================================================//









//===========================根据进程id获取窗口句柄 start======================================//
#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include <Psapi.h>
#include <string.h>
#pragma comment ( lib, "Psapi.lib" )


///< 枚举窗口参数
typedef struct
{
	HWND hwndWindow; // 窗口句柄
	DWORD dwProcessID; // 进程ID
}EnumWindowsArg;
///< 枚举窗口回调函数
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	EnumWindowsArg *pArg = (EnumWindowsArg *)lParam;
	DWORD dwProcessID = 0;
	// 通过窗口句柄取得进程ID
	::GetWindowThreadProcessId(hwnd, &dwProcessID);
	if (dwProcessID == pArg->dwProcessID)
	{
		pArg->hwndWindow = hwnd;
		// 找到了返回FALSE
		return FALSE;
	}
	// 没找到，继续找，返回TRUE
	return TRUE;
}
// 通过进程ID获取窗口句柄
HWND GetWindowHwndByPID(DWORD dwProcessID)
{
	HWND hwndRet = NULL;
	EnumWindowsArg ewa;
	ewa.dwProcessID = dwProcessID;
	ewa.hwndWindow = NULL;
	EnumWindows(EnumWindowsProc, (LPARAM)&ewa);
	if (ewa.hwndWindow)
	{
		hwndRet = ewa.hwndWindow;
	}
	return hwndRet;
}



int main() {
	HWND hwnd = 0;
	HWND htmp = 0;
	hwnd = GetWindowHwndByPID(19192);
	printf("hwnd: %x", hwnd);
        //maybe 忽略
	while (htmp = GetParent(hwnd))
	{
		hwnd = htmp;
	}
	printf("hwnd: %x", hwnd);
	SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	system("pause");
	return 9;
}

//===========================根据进程id获取窗口句柄 end======================================//













