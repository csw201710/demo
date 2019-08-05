#include "spoofMac.h"

/**
A method which returns the computer physical address back to the original

The sub registry key "NetworkAddress" which was added to the active network 
adapter is deleted, restoring the original physical address once the active 
adapter is disabled/enabled

*/

void spoofMac::revertToOriginalMac(){

	HKEY hKey;
	wstring activeRegKey = queryKey();
	LPCWSTR sk = activeRegKey.c_str();
	USES_CONVERSION;
	LPCWSTR activeRegKeyW = sk;

	LPWSTR netAddr = TEXT("NetworkAddress");
	LONG retval = RegOpenKeyEx(HKEY_LOCAL_MACHINE, activeRegKeyW, 0, KEY_ALL_ACCESS, &hKey);

	if (retval == ERROR_SUCCESS) {
		//printf("Success opening key.");
	}
	else {
		printf("Error opening key.");
	}
	retval = RegDeleteValue(hKey, netAddr);

	retval = RegCloseKey(hKey);

	string nicFriendlyName = getNicFriendlyName();

	string disableNic = "netsh interface set interface " + nicFriendlyName + " DISABLED";
	system(disableNic.c_str());

	string enableNic = "netsh interface set interface " + nicFriendlyName + " ENABLED";
	system(enableNic.c_str());

}

/**
A method which returns the current MAC address of the active network adapter

@return A string containing the network adapter's MAC address
*/
string spoofMac::getCurrentMAcAddress(){
	/* Declare and initialize variables */

	PWCHAR networkAdap = NULL;
	PCHAR driverDesc = NULL;

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0;

	string currentMAC;

	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

	// default to unspecified address family (both)
	ULONG family = AF_UNSPEC;

	LPVOID lpMsgBuf = NULL;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	ULONG Iterations = 0;

	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;

	// Allocate a 15 KB buffer to start with.
	outBufLen = WORKING_BUFFER_SIZE;

	do {

		pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
		if (pAddresses == NULL) {
			printf
				("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
			exit(1);
		}

		dwRetVal = GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = NULL;
		}
		else {
			break;
		}

		Iterations++;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		pCurrAddresses = pAddresses;

		//Cycles through the Network Adapters, ouputs name, type and operational status
		while (pCurrAddresses) {

			//check to see if network adapter is "Wi-fi"
			networkAdap = pCurrAddresses->FriendlyName;
			if (pCurrAddresses->OperStatus == 1){//87 == "Wi-Fi",69 == "Ethernet"
				if (*networkAdap == 87 || *networkAdap == 69){
					if (pCurrAddresses->PhysicalAddressLength != 0) {
						//printf("\tPhysical address: ");
						for (i = 0; i < (int)pCurrAddresses->PhysicalAddressLength; i++) {
							if (i == (pCurrAddresses->PhysicalAddressLength - 1)){
								//printf("%.2X\n", (int)pCurrAddresses->PhysicalAddress[i]);

								//Convert int to hex
								stringstream stream;
								stream << setw(2) << uppercase << hex << std::setfill('0')
									<< (int)pCurrAddresses->PhysicalAddress[i];
								currentMAC += stream.str();
							}
							else{
								//printf("%.2X-", (int)pCurrAddresses->PhysicalAddress[i]);

								//Convert int to hex
								stringstream stream;
								stream << setw(2) << uppercase << hex << std::setfill('0')
									<< (int)pCurrAddresses->PhysicalAddress[i];
								currentMAC += stream.str();
								currentMAC += "-";
							}
						}
					}
				}
			}
			pCurrAddresses = pCurrAddresses->Next;
		}

	}
	else {
		printf("Call to GetAdaptersAddresses failed with error: %d\n",
			dwRetVal);
		if (dwRetVal == ERROR_NO_DATA)
			printf("\tNo addresses were found for the requested parameters\n");
		else {

			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				// Default language
				(LPTSTR)& lpMsgBuf, 0, NULL)) {
				printf("\tError: %s", lpMsgBuf);
				LocalFree(lpMsgBuf);
				if (pAddresses)
					FREE(pAddresses);
				exit(1);
			}
		}
	}

	if (pAddresses) {
		FREE(pAddresses);
	}

	if (currentMAC == "")
		currentMAC = "No NetWork Found";

	return currentMAC;
}


/**
A method which returns a randomly generated MAC adrress

Uses the rand() function seeded with system time
The second nibble must be either 'A', 'E', '2' or '6'
e.g. xy-xx-xx-xx-xx-xx, where y must be = ('A', 'E', '2' or '6')
x can be represented with any hexadecimal value

@return A string containing a randomized MAC adrress
*/
string spoofMac::randomizeMAC(){

	srand(time(0));
	string newMAC;
	char secondNibble[] = { 'A', 'E', '2', '6' };
	char newValArray[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	for (int i = 0; i < 6; i++){

		newMAC += newValArray[rand() % 16];

		//Windows physical address second nibble is limited to 2, 6, A ,E
		if (i == 0)
			newMAC += secondNibble[rand() % 4];
		else
			newMAC += newValArray[rand() % 16];

		if (i < 5)
			newMAC += '-';

	}

	return newMAC;
}

/**
 A method sets the active network adapter with a new MAC address

 Adds the subkey "NetworkAddress" to the active network adapter's register

@param A string containing the new MAC address
*/
void spoofMac::setNewMac(string finalMac){

	HKEY hKey;
	wstring key = queryKey();

	string newMAC = finalMac;
	LPCSTR keyData = newMAC.c_str();

	//convert LPCSTR to LPCWSTR
	USES_CONVERSION;
	LPCWSTR skw = key.c_str();
	LPCWSTR keyDataW = A2W(keyData);

	LONG retval = RegOpenKeyEx(HKEY_LOCAL_MACHINE, skw, 0, KEY_ALL_ACCESS, &hKey);

	if (retval == ERROR_SUCCESS) {
		printf("Success opening key.");

	}
	else {
		printf("Error opening key.");
		cout << retval;
	}
	retval = (RegSetValueEx(hKey, TEXT("NetworkAddress"), 0, REG_SZ, (BYTE*)keyDataW, lstrlen(keyDataW) + 17));

	if (retval == ERROR_SUCCESS){
		printf("Success setting key. ");
	}
	else {
		printf("Error setting key.");
	}
	retval = RegCloseKey(hKey);

	string nicFriendlyName = getNicFriendlyName();

	string disableNic = "netsh interface set interface " + nicFriendlyName + " DISABLED";
	system(disableNic.c_str());

	string enableNic = "netsh interface set interface " + nicFriendlyName + " ENABLED";
	system(enableNic.c_str());

}

/**
* A method which returns the key which contains the full path to the active adapter's key
*
*
*@return A wide string containing the full key of the active network adapter
*/
wstring spoofMac::queryKey()
{

	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys = 0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 

	DWORD i, retCode;

	TCHAR  achValue[MAX_VALUE_NAME];
	DWORD cchValue = MAX_VALUE_NAME;

	HKEY hKey;
	LPCWSTR dir = TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}");
	LONG checkReg = RegOpenKeyEx(HKEY_LOCAL_MACHINE, dir, 0, KEY_READ, &hKey);

	if (checkReg == ERROR_SUCCESS)
	{
		cout << "Success opening key" << endl;
	}
	// Get the class name and the value count. 

	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 

	// Enumerate the subkeys, until RegEnumKeyEx fails.

	if (cSubKeys)
	{
		//printf("\nNumber of subkeys: %d\n", cSubKeys);

		for (i = 0; i < cSubKeys; i++)
		{
			cbName = MAX_KEY_LENGTH;
			retCode = RegEnumKeyEx(hKey, i, achKey, &cbName, NULL, NULL, NULL, &ftLastWriteTime);
			wstring key;

			if (retCode == ERROR_SUCCESS)
			{

				//_tprintf(TEXT("(%d) %s\n"), i + 1, achKey);

				wstring subKey = achKey;

				wstring key = TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\" 
					+ subKey);

				if (queryRegValue(subKey) == true)
					return key;
			};
		}
	}
	wstring netFail = L"fail";
	return netFail;
}

/**
* A method which returns a bool if the subkey passed contains the active network adapter
*
*
* @return A boolean value 
*/
bool spoofMac::queryRegValue(wstring subKey){

	//set up our variables and buffers.
	DWORD dwType = REG_SZ;
	TCHAR NetCfgInstanceId[1024];
	DWORD dwDataSize = MAX_VALUE_NAME;
	memset(NetCfgInstanceId, 0, 32);
	wstring key = TEXT("SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}\\" + subKey);

	//Convert string to windows data type
	LPCWSTR findKey = key.c_str();

	// open the key for reading.
	HKEY hkeyDXVer;
	long lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, findKey, 0, KEY_READ, &hkeyDXVer);

	if (ERROR_SUCCESS == lResult)
	{

		LPCWSTR type = TEXT("NetCfgInstanceId");

		// read the NetCfgInstanceId value
		lResult = RegQueryValueExW(hkeyDXVer, type, 0, &dwType, (PBYTE)NetCfgInstanceId, &dwDataSize);

		if (ERROR_SUCCESS == lResult)
		{
			string netInfo = getNetworkInfo();
			TCHAR netInfoW[1024];
			_tcscpy_s(netInfoW, CA2T(netInfo.c_str()));

			//compare the NetCfgInstanceId to evaluate whether the sub key is correct
			if (_tcscmp(netInfoW, NetCfgInstanceId) == 0){
				return true;
			}
		}
	}
	return false;
}

/**
* A method which returns the active adapter's name
* This value os used in determining the which subkey holds the active network adapter
* Accesses the network Information (operStatus, friendlyname, adaptername)
*
* @return A const char* containing the active adapter name
*/


LPCSTR spoofMac::getNetworkInfo(){

	/* Declare and initialize variables */

	PWCHAR networkAdap = NULL;
	PCHAR driverDesc = NULL;

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0;

	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

	// default to unspecified address family (both)
	ULONG family = AF_UNSPEC;

	LPVOID lpMsgBuf = NULL;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	ULONG Iterations = 0;

	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;

	// Allocate a 15 KB buffer to start with.
	outBufLen = WORKING_BUFFER_SIZE;

	//Name of the active network adapter
	LPCSTR adapName = NULL;
	do {

		pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
		if (pAddresses == NULL) {
			printf
				("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
			exit(1);
		}

		dwRetVal =
			GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = NULL;
		}
		else {
			break;
		}

		Iterations++;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		pCurrAddresses = pAddresses;

		//Cycles through the Network Adapters, ouputs name, type and operational status
		while (pCurrAddresses) {
			
			/*
			printf("\tAdapter name: %s\n", pCurrAddresses->AdapterName);
			printf("\tDescription: %wS\n", pCurrAddresses->Description);
			printf("\tFriendly name: %wS\n", pCurrAddresses->FriendlyName);

			if (pCurrAddresses->PhysicalAddressLength != 0) {
				printf("\tPhysical address: ");
				for (i = 0; i < (int)pCurrAddresses->PhysicalAddressLength;
					i++) {
					if (i == (pCurrAddresses->PhysicalAddressLength - 1))
						printf("%.2X\n", (int)pCurrAddresses->PhysicalAddress[i]);
					else
						printf("%.2X-",(int)pCurrAddresses->PhysicalAddress[i]);
				}
			}

			//check to see if network adapter is "Wi-fi"
			printf("\tOperStatus: %ld\n", pCurrAddresses->OperStatus);
			*/
			networkAdap = pCurrAddresses->FriendlyName;
			if (pCurrAddresses->OperStatus == 1){//87 == "wireless", 69 == "Ethernet"
				if (*networkAdap == 87 || *networkAdap == 69){																			//69 == "Ethernet"
				
					driverDesc = pCurrAddresses->AdapterName;
					adapName = pCurrAddresses->AdapterName;
					PWCHAR friendlyName = pCurrAddresses->FriendlyName;
					wstring friendlyNameW(friendlyName);

					//Convert wstring to string UTF-16 to UTF-8
					int len;
					int slength = (int)friendlyNameW.length();
					len = WideCharToMultiByte(CP_ACP, 0, friendlyNameW.c_str(), slength, 0, 0, 0, 0);
					string nicFriendlyName(len, '\0');
					WideCharToMultiByte(CP_ACP, 0, friendlyNameW.c_str(), slength, &nicFriendlyName[0], len, 0, 0);

					return adapName;
				}
			}
			pCurrAddresses = pCurrAddresses->Next;
		}

	}
	else {
		printf("Call to GetAdaptersAddresses failed with error: %d\n",
			dwRetVal);
		if (dwRetVal == ERROR_NO_DATA)
			printf("\tNo addresses were found for the requested parameters\n");
		else {

			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				// Default language
				(LPTSTR)& lpMsgBuf, 0, NULL)) {
				printf("\tError: %s", lpMsgBuf);
				LocalFree(lpMsgBuf);
				if (pAddresses)
					FREE(pAddresses);
				exit(1);
			}
		}
	}

	if (pAddresses) {
		FREE(pAddresses);
	}
	LPCSTR netFail = "fail";
	return netFail;

}

/**
* A method which returns the network adapter's friendly name
* The friendly name is passes as an argument to a powershell command which will
* disable/enable the active network adapter allwowing the spoofed MAC address to take effect
*
* @return A string containing the nic's friendly name
*/
string spoofMac::getNicFriendlyName(){

	/* Declare and initialize variables */

	PWCHAR networkAdap = NULL;
	PCHAR driverDesc = NULL;

	DWORD dwSize = 0;
	DWORD dwRetVal = 0;

	unsigned int i = 0;

	// Set the flags to pass to GetAdaptersAddresses
	ULONG flags = GAA_FLAG_INCLUDE_PREFIX;

	// default to unspecified address family (both)
	ULONG family = AF_UNSPEC;

	LPVOID lpMsgBuf = NULL;

	PIP_ADAPTER_ADDRESSES pAddresses = NULL;
	ULONG outBufLen = 0;
	ULONG Iterations = 0;

	PIP_ADAPTER_ADDRESSES pCurrAddresses = NULL;

	// Allocate a 15 KB buffer to start with.
	outBufLen = WORKING_BUFFER_SIZE;

	//Name of the active Wi-Fi adapter
	LPCSTR adapName = NULL;
	do {

		pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
		if (pAddresses == NULL) {
			printf
				("Memory allocation failed for IP_ADAPTER_ADDRESSES struct\n");
			exit(1);
		}

		dwRetVal =
			GetAdaptersAddresses(family, flags, NULL, pAddresses, &outBufLen);

		if (dwRetVal == ERROR_BUFFER_OVERFLOW) {
			FREE(pAddresses);
			pAddresses = NULL;
		}
		else {
			break;
		}

		Iterations++;

	} while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (Iterations < MAX_TRIES));

	if (dwRetVal == NO_ERROR) {
		// If successful, output some information from the data we received
		pCurrAddresses = pAddresses;

		//Cycles through the Network Adapters, ouputs name, type and operational status
		while (pCurrAddresses) {

			networkAdap = pCurrAddresses->FriendlyName;
			if (pCurrAddresses->OperStatus == 1){
				if (*networkAdap == 87 || *networkAdap == 69){//87 == "Wi-Fi",69 == "Ethernet"
					driverDesc = pCurrAddresses->AdapterName;
					adapName = pCurrAddresses->AdapterName;
					PWCHAR friendlyName = pCurrAddresses->FriendlyName;
					wstring friendlyNameW(friendlyName);

					//Convert wstring to string, UTF-16 to UTF-8
					int len;
					int slength = (int)friendlyNameW.length();
					len = WideCharToMultiByte(CP_ACP, 0, friendlyNameW.c_str(), slength, 0, 0, 0, 0);
					string nicFriendlyName(len, '\0');
					WideCharToMultiByte(CP_ACP, 0, friendlyNameW.c_str(), slength, &nicFriendlyName[0], len, 0, 0);

					return nicFriendlyName;
				}
			}
			pCurrAddresses = pCurrAddresses->Next;
		}

	}
	else {
		printf("Call to GetAdaptersAddresses failed with error: %d\n",
			dwRetVal);
		if (dwRetVal == ERROR_NO_DATA)
			printf("\tNo addresses were found for the requested parameters\n");
		else {

			if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
				// Default language
				(LPTSTR)& lpMsgBuf, 0, NULL)) {
				printf("\tError: %s", lpMsgBuf);
				LocalFree(lpMsgBuf);
				if (pAddresses)
					FREE(pAddresses);
				exit(1);
			}
		}
	}

	if (pAddresses) {
		FREE(pAddresses);
	}
	string noName = "Error";
	return noName;
}