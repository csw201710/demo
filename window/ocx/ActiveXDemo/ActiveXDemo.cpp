// ActiveXDemo.cpp : CActiveXDemoApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "ActiveXDemo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CActiveXDemoApp theApp;

const GUID CDECL _tlid = { 0x6C8A24C1, 0x5F75, 0x4555, { 0xB8, 0x93, 0x5, 0x1B, 0x3A, 0x7D, 0x25, 0x5A } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CActiveXDemoApp::InitInstance - DLL 初始化

BOOL CActiveXDemoApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
	}

	return bInit;
}



// CActiveXDemoApp::ExitInstance - DLL 终止

int CActiveXDemoApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
