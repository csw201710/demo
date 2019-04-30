#pragma once

// ActiveXDemoCtrl.h : CActiveXDemoCtrl ActiveX 控件类的声明。


// CActiveXDemoCtrl : 有关实现的信息，请参阅 ActiveXDemoCtrl.cpp。

class CActiveXDemoCtrl : public COleControl
{
	DECLARE_DYNCREATE(CActiveXDemoCtrl)

// 构造函数
public:
	CActiveXDemoCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CActiveXDemoCtrl();

	DECLARE_OLECREATE_EX(CActiveXDemoCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(CActiveXDemoCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CActiveXDemoCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CActiveXDemoCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidcb_add = 1L,
		dispidshowString = 2L,
		dispidAdd = 1L
	};
protected:
	LONG Add(LONG a1, LONG a2);
	BSTR showString(LPCTSTR msg);
	void cb_add(DOUBLE a1, DOUBLE a2);
};

