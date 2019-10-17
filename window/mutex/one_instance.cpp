
bool isOneInstance()
{
    HANDLE mutex = CreateMutexW(NULL, TRUE, L"Hello xxxx");
    if ((mutex != NULL) && (GetLastError() == ERROR_ALREADY_EXISTS)) {
        ReleaseMutex(mutex);
        return false;
    }
    return true;
}




