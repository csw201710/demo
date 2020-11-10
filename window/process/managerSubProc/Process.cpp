#include "Process.h"
#include <windows.h>


#ifndef freez
#define freez(X)  { if(X) { free((void*)X); X = NULL ; } }
#endif

char *strdup_or_die(const char *str)
{
	char *new_str;
	if (!str) return 0;
	new_str = strdup(str);

	if (new_str == NULL)
	{
		printf("Out of memory in strdup_or_die().");
		exit(1);
	}
	return(new_str);
}

CProcess::CProcess(const char * fileName, const char * args, int isHidden)
{
	m_fileName = strdup_or_die(fileName);
	m_args = strdup_or_die(args);
	m_isHidden = isHidden;

	m_hProcess = 0;
}

int CProcess::start()
{
	char buffer[1024];
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	if (m_hProcess) {
		return 0;
	}

	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = (m_isHidden != 0 ? SW_SHOWNORMAL : SW_HIDE);

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	;
	snprintf(buffer, sizeof(buffer) - 1 ,"\"%s\" %s", m_fileName, m_args);
	if (!CreateProcess(0, (LPTSTR)(LPCTSTR)buffer, NULL, NULL, FALSE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi))
	{
		printf("CreateProcess failed");
		return -1;
	}
	m_hProcess = pi.hProcess;

	CloseHandle(pi.hThread); // 关闭子进程的主线程句柄

	//WaitForSingleObject(pi.hProcess, INFINITE); // 等待子进程的退出
	return 0;
}

int CProcess::stop()
{
	TerminateProcess(m_hProcess, 0);
	WaitForSingleObject(m_hProcess, INFINITE); // 等待子进程的退出
	m_hProcess = 0;
	return 0;
}

void CProcess::restart()
{
	this->stop();
	this->start();
}

CProcess::~CProcess()
{
	freez(m_fileName);
	freez(m_args);
}




void testProc() {
	CProcess obj("C:\\Windows\\System32\\cmd.exe", "-h");
	obj.start();
	printf("obj.start()\n");
	Sleep(1000 * 5);

	obj.stop();
	printf("obj.stop()\n");
	Sleep(1000 * 3);

	obj.restart();
	printf("obj.restart()\n");
}