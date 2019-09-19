#pragma once


#include "MyDirTreeView.h"
#include "MyDirListView.h"
// CMyDirView 视图

class CMyDirView : public CView
{
	DECLARE_DYNCREATE(CMyDirView)

protected:
	CSplitterWnd	m_WndSplitter;

	CMyDirView();           // 动态创建所使用的受保护的构造函数
	virtual ~CMyDirView();

	

public:
	CListView *CMyDirView::GetListView();
	CTreeView *GetTreeView();

	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

private:
	CMyDirTreeView *m_TreeView;
	CMyDirListView *m_ListView;

public:
	void FileSizeToFormat(ULONGLONG ulSize , CString & fileSzie);
	void GetDiskInfo(HTREEITEM hItem);
	void GetDirInfo(HTREEITEM hItem, CString &path);
	void GetFileInfo(CString &path);
	
	BOOL GetMyDirLeftPath(HTREEITEM hItem, CString &path);



	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
protected:
	afx_msg LRESULT OnTreeselchanged(WPARAM wParam, LPARAM lParam);
};


