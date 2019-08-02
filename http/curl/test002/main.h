#pragma once


#include <Windows.h>
#include <CommCtrl.h>
#include "resource.h"
#include "curl/curl.h"
#include <stdio.h>
#include <stdlib.h>



DWORD WINAPI showDownloadDlg(LPVOID lpParam);
void CenterWindow(HWND hwnd);