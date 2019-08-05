#ifndef SPOOFMAC_H
#define SPOOFMAC_H

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))
#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383
#define _WIN32_WINNT 0x0500
#define TOTALBYTES 8192
#define BYTEINCREMENT 4096

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>
#include <string>
#include <Windows.h>
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include <malloc.h>
#include <ctime>
#include <atlbase.h>
#include <msclr\marshal_cppstd.h>
#include <iomanip>
#include <sstream>
#include <atlstr.h>
#include <locale>

#pragma comment(lib,"IPHlpApi.Lib")

using namespace std;

class spoofMac{

	public:
		static void setNewMac(string); //Set the "Wi-Fi" adapter's new mac address

		static void revertToOriginalMac(); //change the mac Address back to the original

		static string getCurrentMAcAddress();//returns the curernt MAC Address of the active nic

		static string randomizeMAC();//returns a randomized MAC address

		static wstring queryKey();//Locates the subkey which holds the active "Wi-Fi" adapter

		static bool queryRegValue(wstring); //Find the subkey where the where the active "Wi-Fi" is located

		static LPCSTR getNetworkInfo();//Locate the active "Wi-Fi" adapter

		static string getNicFriendlyName();//returns the nic friendlt name



};
#endif