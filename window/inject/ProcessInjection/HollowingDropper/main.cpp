#include "stdafx.h"

int main()
{
	cout << "Process Hollowing" << endl;
	LPSTR lpCommandLine = (LPSTR)"C:\\Program Files\\Internet Explorer\\iexplore.exe";

#ifdef _WIN64
	LPSTR lpSourceFile = (LPSTR)"C:\\Windows\\System32\\calc.exe";
#else
	LPSTR lpSourceFile = (LPSTR)"C:\\Windows\\System32\\calc.exe";
#endif // _WIN64

	HANDLE hProcess = CreateHollowedProcess(lpCommandLine, lpSourceFile);
	system("pause");

	if (hProcess) 
	{	
		TerminateProcess(hProcess, 4);
		WaitForSingleObject(hProcess, 0);
	}
	return 0;
}