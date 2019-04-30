// ActiveXDemoCtrl.cpp : CActiveXDemoCtrl ActiveX 控件类的实现。

#include "stdafx.h"
#include "ActiveXDemo.h"
#include "ActiveXDemoCtrl.h"
#include "ActiveXDemoPropPage.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CActiveXDemoCtrl, COleControl)



// 消息映射

BEGIN_MESSAGE_MAP(CActiveXDemoCtrl, COleControl)
	ON_OLEVERB(AFX_IDS_VERB_PROPERTIES, OnProperties)
END_MESSAGE_MAP()



// 调度映射

BEGIN_DISPATCH_MAP(CActiveXDemoCtrl, COleControl)
	DISP_FUNCTION_ID(CActiveXDemoCtrl, "AboutBox", DISPID_ABOUTBOX, AboutBox, VT_EMPTY, VTS_NONE)
	DISP_FUNCTION_ID(CActiveXDemoCtrl, "Add", dispidAdd, Add, VT_I4, VTS_I4 VTS_I4)
	DISP_FUNCTION_ID(CActiveXDemoCtrl, "showString", dispidshowString, showString, VT_BSTR, VTS_BSTR)
	DISP_FUNCTION_ID(CActiveXDemoCtrl, "cb_add", dispidcb_add, cb_add, VT_EMPTY, VTS_R8 VTS_R8)
END_DISPATCH_MAP()



// 事件映射

BEGIN_EVENT_MAP(CActiveXDemoCtrl, COleControl)
END_EVENT_MAP()



// 属性页

// TODO: 按需要添加更多属性页。请记住增加计数!
BEGIN_PROPPAGEIDS(CActiveXDemoCtrl, 1)
	PROPPAGEID(CActiveXDemoPropPage::guid)
END_PROPPAGEIDS(CActiveXDemoCtrl)



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CActiveXDemoCtrl, "ACTIVEXDEMO.ActiveXDemoCtrl.1",
	0x153809b, 0x60ed, 0x402b, 0x9e, 0xf2, 0x8f, 0x46, 0xe2, 0x17, 0x6c, 0x6d)



// 键入库 ID 和版本

IMPLEMENT_OLETYPELIB(CActiveXDemoCtrl, _tlid, _wVerMajor, _wVerMinor)



// 接口 ID

const IID IID_DActiveXDemo = { 0x7FA6A843, 0x6D85, 0x4825, { 0xBB, 0x0, 0xE0, 0xEF, 0x3F, 0x48, 0xBE, 0xFB } };
const IID IID_DActiveXDemoEvents = { 0x2B04342B, 0xBC5E, 0x442B, { 0x9E, 0xC2, 0x84, 0x7, 0x6F, 0x8A, 0x2C, 0xE5 } };


// 控件类型信息

static const DWORD _dwActiveXDemoOleMisc =
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST |
	OLEMISC_INSIDEOUT |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_RECOMPOSEONRESIZE;

IMPLEMENT_OLECTLTYPE(CActiveXDemoCtrl, IDS_ACTIVEXDEMO, _dwActiveXDemoOleMisc)



// CActiveXDemoCtrl::CActiveXDemoCtrlFactory::UpdateRegistry -
// 添加或移除 CActiveXDemoCtrl 的系统注册表项

BOOL CActiveXDemoCtrl::CActiveXDemoCtrlFactory::UpdateRegistry(BOOL bRegister)
{
	// TODO: 验证您的控件是否符合单元模型线程处理规则。
	// 有关更多信息，请参考 MFC 技术说明 64。
	// 如果您的控件不符合单元模型规则，则
	// 必须修改如下代码，将第六个参数从
	// afxRegApartmentThreading 改为 0。

	if (bRegister)
		return AfxOleRegisterControlClass(
			AfxGetInstanceHandle(),
			m_clsid,
			m_lpszProgID,
			IDS_ACTIVEXDEMO,
			IDB_ACTIVEXDEMO,
			afxRegApartmentThreading,
			_dwActiveXDemoOleMisc,
			_tlid,
			_wVerMajor,
			_wVerMinor);
	else
		return AfxOleUnregisterClass(m_clsid, m_lpszProgID);
}



// CActiveXDemoCtrl::CActiveXDemoCtrl - 构造函数

CActiveXDemoCtrl::CActiveXDemoCtrl()
{
	InitializeIIDs(&IID_DActiveXDemo, &IID_DActiveXDemoEvents);
	// TODO: 在此初始化控件的实例数据。
}



// CActiveXDemoCtrl::~CActiveXDemoCtrl - 析构函数

CActiveXDemoCtrl::~CActiveXDemoCtrl()
{
	// TODO: 在此清理控件的实例数据。
}



// CActiveXDemoCtrl::OnDraw - 绘图函数

void CActiveXDemoCtrl::OnDraw(
			CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid)
{
	if (!pdc)
		return;

	// TODO: 用您自己的绘图代码替换下面的代码。
	pdc->FillRect(rcBounds, CBrush::FromHandle((HBRUSH)GetStockObject(WHITE_BRUSH)));
	pdc->Ellipse(rcBounds);
}



// CActiveXDemoCtrl::DoPropExchange - 持久性支持

void CActiveXDemoCtrl::DoPropExchange(CPropExchange* pPX)
{
	ExchangeVersion(pPX, MAKELONG(_wVerMinor, _wVerMajor));
	COleControl::DoPropExchange(pPX);

	// TODO: 为每个持久的自定义属性调用 PX_ 函数。
}



// CActiveXDemoCtrl::OnResetState - 将控件重置为默认状态

void CActiveXDemoCtrl::OnResetState()
{
	COleControl::OnResetState();  // 重置 DoPropExchange 中找到的默认值

	// TODO: 在此重置任意其他控件状态。
}



// CActiveXDemoCtrl::AboutBox - 向用户显示“关于”框

void CActiveXDemoCtrl::AboutBox()
{
	CDialogEx dlgAbout(IDD_ABOUTBOX_ACTIVEXDEMO);
	dlgAbout.DoModal();
}



// CActiveXDemoCtrl 消息处理程序


LONG CActiveXDemoCtrl::Add(LONG a1, LONG a2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	this->cb_add(a1,a2);
	return a1 + a2;
}


BSTR CActiveXDemoCtrl::showString(LPCTSTR msg)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString strResult(msg);

	// TODO: 在此添加调度处理程序代码
	MessageBox(msg, msg, MB_OK);
	return strResult.AllocSysString();
}


void CActiveXDemoCtrl::cb_add(DOUBLE a1, DOUBLE a2)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// TODO: 在此添加调度处理程序代码
	CString str;
	str.Format(TEXT(" %f + %f = %f"),a1,a2,a1+a2);
	MessageBox(str, str, MB_OK);
	FireEvent(dispidcb_add, EVENT_PARAM(VTS_R8 VTS_R8), a1, a2);
	


}
