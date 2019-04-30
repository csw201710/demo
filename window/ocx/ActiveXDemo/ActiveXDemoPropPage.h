#pragma once

// ActiveXDemoPropPage.h : CActiveXDemoPropPage 属性页类的声明。


// CActiveXDemoPropPage : 有关实现的信息，请参阅 ActiveXDemoPropPage.cpp。

class CActiveXDemoPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CActiveXDemoPropPage)
	DECLARE_OLECREATE_EX(CActiveXDemoPropPage)

// 构造函数
public:
	CActiveXDemoPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_ACTIVEXDEMO };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

