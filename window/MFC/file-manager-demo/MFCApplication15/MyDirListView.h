#pragma once


// CMyDirListView 视图

class CMyDirListView : public CListView
{
	DECLARE_DYNCREATE(CMyDirListView)

protected:
	CMyDirListView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyDirListView();

	CImageList m_TreeImageList;
public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

	
protected:
	DECLARE_MESSAGE_MAP()
public:
	CWnd * m_container;
	void SelectListHeader(int a);
	int InsertListData(int type,char* path);


	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);


	
protected:
	afx_msg LRESULT OnListinsertdata(WPARAM wParam, LPARAM lParam);
};


