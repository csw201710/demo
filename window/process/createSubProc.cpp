#include<iostream>
#include<windows.h>
int main(int argc, char*argv[])
{
    STARTUPINFO si = { sizeof(STARTUPINFO) };//在产生子进程时，子进程的窗口相关信息
    PROCESS_INFORMATION pi;                  //子进程的ID/线程相关信息
    DWORD returnCode;//用于保存子程进的返回值;

    wchar_t commandLine1[] = L"subapp.exe -l";  //测试命令行参数一
    wchar_t commandLine2[] = L"subapp.exe";     //测试命令行参数二

    BOOL bRet = CreateProcess(              //调用失败，返回0；调用成功返回非0；
        NULL,                               //一般都是空；（另一种批处理情况：此参数指定"cmd.exe",下一个命令行参数 "/c otherBatFile")
        commandLine1,                       //命令行参数         
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpProcessAttributes,
        NULL,                               //_In_opt_    LPSECURITY_ATTRIBUTES lpThreadAttributes,
        FALSE,                              //_In_        BOOL                  bInheritHandles,
        CREATE_NEW_CONSOLE,                 //新的进程使用新的窗口。
        NULL,                               //_In_opt_    LPVOID                lpEnvironment,
        NULL,                               //_In_opt_    LPCTSTR               lpCurrentDirectory,
        &si,                                //_In_        LPSTARTUPINFO         lpStartupInfo,
        &pi);                               //_Out_       LPPROCESS_INFORMATION lpProcessInformation

    if (bRet)
    {
        std::cout << "process is running..." << std::endl;
        //等待子进程结束
        WaitForSingleObject(pi.hProcess, -1);
        std::cout << "process is finished"  << std::endl;
        //获取子进程的返回值
        GetExitCodeProcess(pi.hProcess, &returnCode);
        std::cout << "process return code:" << returnCode << std::endl;
    }
    else
    {
        std::cout << "Create Process error!"<<std::endl;
        return 0;
    }

    getchar();
    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    return 0;
}

