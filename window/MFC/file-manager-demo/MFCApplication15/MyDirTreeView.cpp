// MyDirTreeView.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication15.h"
#include "MyDirTreeView.h"


// CMyDirTreeView

IMPLEMENT_DYNCREATE(CMyDirTreeView, CTreeView)

CMyDirTreeView::CMyDirTreeView()
{
	m_container = 0;
}

CMyDirTreeView::~CMyDirTreeView()
{
}

BEGIN_MESSAGE_MAP(CMyDirTreeView, CTreeView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(TVN_SELCHANGING, &CMyDirTreeView::OnTvnSelchanging)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, &CMyDirTreeView::OnTvnSelchanged)
END_MESSAGE_MAP()


// CMyDirTreeView 诊断

#ifdef _DEBUG
void CMyDirTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyDirTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyDirTreeView 消息处理程序


int CMyDirTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	m_TreeImageList.Create(16, 16, ILC_COLOR8 | ILC_MASK, 6, 6);
	

	
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_COMPUTER)); // 0
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_CLOSE));//2
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_OPEN));
	m_TreeImageList.Add(AfxGetApp()->LoadIcon(IDI_ICON_DIR_DEVICE));

	
	

	GetTreeCtrl().SetImageList(&m_TreeImageList, TVSIL_NORMAL);


	GetTreeCtrl().ModifyStyle(NULL, TVS_HASLINES |
		TVS_LINESATROOT | TVS_HASBUTTONS | TVS_SHOWSELALWAYS);

	TV_INSERTSTRUCT tvstruct;
	tvstruct.hParent = TVI_ROOT;
	tvstruct.hInsertAfter = TVI_LAST;
	tvstruct.item.iImage = 0;
	tvstruct.item.iSelectedImage = 0;
	
	tvstruct.item.pszText = _T("我的电脑");
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	HTREEITEM m_RootItem = GetTreeCtrl().InsertItem(&tvstruct);

	//GetTreeCtrl().SelectItem(m_RootItem);
	GetTreeCtrl().SetItemData(m_RootItem, 0);

#if 0
	TVITEM tvi;
	tvi.mask = TVIF_CHILDREN; //cChildren成员是有效的。
	tvi.hItem = m_RootItem;
	tvi.cChildren = 1;  //这个项有一个或更多的子项。
	GetTreeCtrl().SetItem(&tvi);
	//GetTreeCtrl().SelectItem(m_RootItem);

	for (int j = 0;j<50;j++) {//增加各产品
		tvstruct.hParent = m_RootItem;
		tvstruct.item.pszText = _T("123");
		//TCItem.item.lParam = (i + 1) * 10 + (j + 1);//子项序号
		GetTreeCtrl().InsertItem(&tvstruct);

	}
#endif

	return 0;
}


void CMyDirTreeView::OnSize(UINT nType, int cx, int cy)
{
	CTreeView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
}


void CMyDirTreeView::OnTvnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CMyDirTreeView::DeleteTreeChild(HTREEITEM hItem)
{
	if (GetTreeCtrl().ItemHasChildren(hItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = GetTreeCtrl().GetChildItem(hItem);
		while (hChildItem != NULL)
		{
			hNextItem = GetTreeCtrl().GetNextItem(hChildItem, TVGN_NEXT);
			GetTreeCtrl().DeleteItem(hChildItem);
			hChildItem = hNextItem;
		}
	}
}


void CMyDirTreeView::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	if (m_container) {
		m_container->PostMessage(WM_TREESELCHANGED, (WPARAM)hItem, NULL);
	}
		
	//AfxGetMainWnd()->PostMessage(WM_TREESELCHANGED, (WPARAM)hItem, NULL);
	

	
}
