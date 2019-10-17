void showErrMessgae(){
    DWORD err = ::GetLastError();
    LPVOID     lpMsgBuffer;
    DWORD dwRet = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_HMODULE,
        GetModuleHandleA("wininet.dll"),
        err,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPTSTR>(&lpMsgBuffer),
        0,
        NULL);

    char szmsg2[1024] = "\0";
    sprintf(szmsg2, "[%d, 0x%08x] %s", err, err, reinterpret_cast<LPCSTR>(lpMsgBuffer));
    OutputDebugStringA(szmsg2);
    ::LocalFree(lpMsgBuffer);
}


