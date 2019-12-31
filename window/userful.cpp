//////////////////////////////////////////////
//函数说明：修改EXE图标
//
//参    数：IconFile 图标文件
//              ExeFile 被修改的EXE文件
//
//返回值： 成功为True，否则False
/////////////////////////////////////////////
bool ChangeExeIcon(LPCTSTR ExeFile,LPCTSTR IconFile)
{
	ICONDIR stID;
	ICONDIRENTRY stIDE;
	GRPICONDIR stGID;
	HANDLE hFile;
	DWORD nSize, nGSize, dwReserved;
	HANDLE hUpdate;
	PBYTE pIcon, pGrpIcon;
	BOOL ret;

	hFile = CreateFile(IconFile, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	ZeroMemory(&stID, sizeof(ICONDIR));
	ret = ReadFile(hFile, &stID, sizeof(ICONDIR), &dwReserved, NULL);

	ZeroMemory(&stIDE, sizeof(ICONDIRENTRY));
	ret = ReadFile(hFile, &stIDE, sizeof(ICONDIRENTRY), &dwReserved, NULL);

	nSize = stIDE.dwBytesInRes;
	pIcon = (PBYTE)malloc(nSize);
	SetFilePointer(hFile, stIDE.dwImageOffset, NULL, FILE_BEGIN);
	ret = ReadFile(hFile, (LPVOID)pIcon, nSize, &dwReserved, NULL);
	if (!ret)
	{
		CloseHandle(hFile);
		return false;
	}

	ZeroMemory(&stGID, sizeof(GRPICONDIR));
	stGID.idCount = stID.idCount;
	stGID.idReserved = 0;
	stGID.idType = 1;
	CopyMemory(&stGID.idEntries, &stIDE, 12);
	stGID.idEntries.nID = 0;

	nGSize = sizeof(GRPICONDIR);
	pGrpIcon = (PBYTE)malloc(nGSize);
	CopyMemory(pGrpIcon, &stGID, nGSize);


	hUpdate = BeginUpdateResource(ExeFile, false);
	ret = UpdateResource(hUpdate, RT_GROUP_ICON, MAKEINTRESOURCE(1), 0, (LPVOID)pGrpIcon, nGSize);
	ret = UpdateResource(hUpdate, RT_ICON, MAKEINTRESOURCE(1), 0, (LPVOID)pIcon, nSize);
	EndUpdateResource(hUpdate, false);
	if (!ret)
	{
		CloseHandle(hFile);
		return false;
	}

	CloseHandle(hFile);
	return true;
}




