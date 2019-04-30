// ActiveXDemoPropPage.cpp : CActiveXDemoPropPage 属性页类的实现。

#include "stdafx.h"
#include "ActiveXDemo.h"
#include "ActiveXDemoPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(CActiveXDemoPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(CActiveXDemoPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(CActiveXDemoPropPage, "ACTIVEXDEMO.ActiveXDemoPropPage.1",
	0x2ee91c54, 0x8004, 0x4238, 0xa8, 0x12, 0x85, 0xe4, 0x8c, 0x37, 0x96, 0xcb)



// CActiveXDemoPropPage::CActiveXDemoPropPageFactory::UpdateRegistry -
// 添加或移除 CActiveXDemoPropPage 的系统注册表项

BOOL CActiveXDemoPropPage::CActiveXDemoPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_ACTIVEXDEMO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// CActiveXDemoPropPage::CActiveXDemoPropPage - 构造函数

CActiveXDemoPropPage::CActiveXDemoPropPage() :
	COlePropertyPage(IDD, IDS_ACTIVEXDEMO_PPG_CAPTION)
{
}



// CActiveXDemoPropPage::DoDataExchange - 在页和属性间移动数据

void CActiveXDemoPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// CActiveXDemoPropPage 消息处理程序
