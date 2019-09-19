// MyDirView.cpp : 实现文件
//

#include "stdafx.h"
#include "MFCApplication15.h"
#include "MyDirView.h"
#include "MyDirTreeView.h"
#include "MyDirListView.h"

// CMyDirView

IMPLEMENT_DYNCREATE(CMyDirView, CView)

CMyDirView::CMyDirView()
{

}

CMyDirView::~CMyDirView()
{
}



BEGIN_MESSAGE_MAP(CMyDirView, CView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_MESSAGE(WM_TREESELCHANGED, &CMyDirView::OnTreeselchanged)
END_MESSAGE_MAP()


// CMyDirView 绘图

void CMyDirView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO:  在此添加绘制代码
}


// CMyDirView 诊断

#ifdef _DEBUG
void CMyDirView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyDirView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyDirView 消息处理程序
void CMyDirView::FileSizeToFormat(ULONGLONG ulSize, CString & fileSzie)
{
	TCHAR strSize[20] = {0};
	

	double i = pow((double)2, 10);

	if (ulSize < pow((double)2, 10))//dwSize < 1024
	{
		_stprintf_s(strSize, 20, TEXT("%dB"), ulSize);//文件大小 B
	}
	else if (pow((double)2, 10) <= ulSize &&  ulSize< pow((double)2, 20))// 1024 <= dwSize < 1024*1024
	{
		float fSize = (float)(ulSize * 100 / 1024) / 100;
		_stprintf_s(strSize, 20, TEXT("%.2fKB"), fSize);
	}
	else if (pow((double)2, 20) <= ulSize && ulSize < pow((double)2, 30))// 1024*1024 <= dwSize < 1024*1024*1024
	{
		float fSize = (float)(ulSize / 1024 * 100 / 1024) / 100;
		_stprintf_s(strSize, 20, TEXT("%.2fM"), fSize);//文件大小 M
	}
	else if (pow((double)2, 30) <= ulSize && ulSize < pow((double)2, 40)) // 1024*1024*1024 <= dwSize < 1024*1024*1024*1024
	{
		float fSize = (float)(ulSize / 1024 * 100 / 1024 / 1024) / 100;
		_stprintf_s(strSize, 20, TEXT("%.2fG"), fSize);//文件大小 G 
	}
	else
	{
		float fSize = (float)(ulSize / 1024 * 100 / 1024 / 1024 / 1024) / 100;
		_stprintf_s(strSize, 20, TEXT("%.2fT"), fSize);//文件大小 T
	}
	fileSzie = strSize;
}

void CMyDirView::GetDiskInfo(HTREEITEM hItem)
{
	m_TreeView->DeleteTreeChild(hItem);
	char m_SmallBuf[512];
	memset(m_SmallBuf, 0, sizeof(m_SmallBuf));
	DWORD  m_len = GetLogicalDriveStrings(512, m_SmallBuf);
	for (DWORD i = 0; i < m_len; i += 4)
	{
		char * p = &m_SmallBuf[i];
		int pLen = strlen(p) -1;
		char c = p[pLen];

		if (c == '\\' || c == '/') {
			p[pLen] = '\0';
		}

		GetDriveType(&m_SmallBuf[i]);
		TV_INSERTSTRUCT tvstruct;
		tvstruct.hParent = hItem;
		tvstruct.hInsertAfter = TVI_LAST;
		tvstruct.item.iImage = 4;
		tvstruct.item.iSelectedImage = 4;
		tvstruct.item.pszText = p;
		tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		HTREEITEM hItem = m_TreeView->GetTreeCtrl().InsertItem(&tvstruct);

		/*
		ULARGE_INTEGER m_Size[3] = { 0 };
		if (GetDiskFreeSpaceEx(&m_SmallBuf[i], &m_Size[0],
		&m_Size[1], &m_Size[2]))
		{
		printf("%d MB",
		(m_Size[1].QuadPart / (1024 * 1024)));
		printf("%d MB",
		(m_Size[2].QuadPart / (1024 * 1024)));
		}
		else
		{
		//strcpy("0 MB");
		//strcpy( "0 MB");
		}
		*/

	}
}

void CMyDirView::GetDirInfo(HTREEITEM hItem,CString &path)
{
	char m_Path[512] = { 0 };
	sprintf(m_Path, "%s\\*", path);
	CFileFind m_Find;
	if (!m_Find.FindFile(m_Path))
	{
		m_Find.Close();
		return ;
	}
	while (1)
	{
		BOOL ret = m_Find.FindNextFile();
		{

			if ((m_Find.IsDirectory()) && (!m_Find.IsDots()))
			{
				TV_INSERTSTRUCT tvstruct;
				strcpy(m_Path, (LPCTSTR)m_Find.GetFileName());
				tvstruct.hParent = hItem;
				tvstruct.hInsertAfter = TVI_LAST;
				tvstruct.item.iImage = 2;
				tvstruct.item.iSelectedImage = 3;
				tvstruct.item.pszText = m_Path;
				tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
				HTREEITEM hChildItem = m_TreeView->GetTreeCtrl().InsertItem(&tvstruct);
			}
			else if (!m_Find.IsDirectory())
			{

			}
		}
		if (!ret) {
			break;
		}
		
		
		
	}
	m_Find.Close();
	return ;
}

void CMyDirView::GetFileInfo(CString & path)
{
	char m_Path[512] = { 0 };
	sprintf(m_Path, "%s\\*", path);
	CFileFind m_Find;
	if (!m_Find.FindFile(m_Path))
	{
		m_Find.Close();
		return;
	}
	m_ListView->SelectListHeader(0);
	while (1)
	{
		BOOL ret = m_Find.FindNextFile();
		{
			char * fileType = "";
			char fileSize[256] = { 0 };
			char fileTime[256] = { 0 };
			int imgType = 0;
			if (m_Find.IsDots()) {
				continue;
			}
			CString filePath = m_Find.GetFilePath();
			if ((m_Find.IsDirectory()) )
			{
				fileType = "文件夹";
				imgType = 2;
			}
			else if (!m_Find.IsDirectory())
			{
				fileType = "文件";
				
			}
			CFileStatus		fileStatus;
			if (CFile::GetStatus(filePath, fileStatus))
			{
				if (imgType != 2) {
#if 0
					if (fileStatus.m_size > (1024 * 1024 * 1024)) {
						sprintf(fileSize, "%.2f GB", ((float)fileStatus.m_size) / (1024 * 1024 * 1024));
					}
					else if (fileStatus.m_size > (1024 * 1024)) {
						sprintf(fileSize, "%.2f MB", ((float)fileStatus.m_size)   / (1024 * 1024));
					}
					else if (fileStatus.m_size > 1024) {
						sprintf(fileSize, "%.2f KB", ((float)fileStatus.m_size) / 1024);
					}
					else {
						sprintf(fileSize, "%ld B", fileStatus.m_size);
					}
#else
					CString sFileSize;
					FileSizeToFormat(fileStatus.m_size, sFileSize);
					strcpy(fileSize, (LPCTSTR)sFileSize);
#endif
				}
				CString strTime = fileStatus.m_mtime.Format("%Y年%m月%d日 %H:%M:%S");
				strcpy(fileTime, (LPCTSTR)strTime);

			}
			
			int nItem = m_ListView->GetListCtrl().InsertItem(m_ListView->GetListCtrl().GetItemCount(), "", imgType);
			if (nItem == -1) {
				m_Find.Close();
				return;
			}
			m_ListView->GetListCtrl().SetItemText(nItem, 0, (LPCTSTR)m_Find.GetFileName());
			m_ListView->GetListCtrl().SetItemText(nItem, 1, fileType);
			m_ListView->GetListCtrl().SetItemText(nItem, 2, fileSize);

			m_ListView->GetListCtrl().SetItemText(nItem, 3, fileTime);

			m_ListView->GetListCtrl().SetItemText(nItem, 4, (LPCTSTR)filePath.Left(filePath.ReverseFind('\\')));
		}
		if (!ret) {
			break;
		}

	}
	m_Find.Close();

}

BOOL CMyDirView::GetMyDirLeftPath(HTREEITEM hItem, CString & path)
{

	CTreeCtrl * pList = &m_TreeView->GetTreeCtrl();
	if (hItem == NULL) return FALSE;
	if (hItem == pList->GetRootItem())
	{
		path = "";
		return TRUE;
	}
	
	char m_ItemName[512] = "";
	char m_KeyName[2048] = { 0 };
	char m_TmpName[2048] = { 0 };
	HTREEITEM hCurItem = hItem;
	HTREEITEM hParent = NULL;

	while (1)
	{
		wsprintf(m_ItemName, "%s\\", pList->GetItemText(hCurItem));
		sprintf(m_TmpName, "%s%s", m_ItemName, m_KeyName);
		strcpy(m_KeyName, m_TmpName);
		hCurItem = pList->GetParentItem(hCurItem);
		if (hCurItem == pList->GetRootItem())
			break;
	}
	path = m_KeyName;
	return TRUE;
}

int CMyDirView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_WndSplitter.CreateStatic(this, 1, 2);
	CCreateContext *pContext = (CCreateContext*)lpCreateStruct->lpCreateParams;
	m_WndSplitter.CreateView(0, 0, RUNTIME_CLASS(CMyDirTreeView), CSize(0, 0), pContext);
	m_WndSplitter.CreateView(0, 1, RUNTIME_CLASS(CMyDirListView), CSize(0, 0), pContext);

	m_TreeView = (CMyDirTreeView*)m_WndSplitter.GetPane(0, 0);
	m_TreeView->m_container = this;


	m_ListView = (CMyDirListView*)m_WndSplitter.GetPane(0, 1);
	m_ListView->m_container = this;

	return 0;
}


void CMyDirView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (m_WndSplitter.GetSafeHwnd())
	{
		m_WndSplitter.MoveWindow(-2, -2, cx+2 , cy+2 ); // 去掉分隔条的边框
				//指定一个分隔器窗口列。
				//指定分隔器窗口列的以像素表示的理想宽度。
				// 	指定分隔器窗口列以像素表示的最小宽度。
		m_WndSplitter.SetColumnInfo(0, cx * 1 / 4, 0); //执行第一列宽度
		m_WndSplitter.RecalcLayout();//重新布置窗口。
	}
}


CTreeView *CMyDirView::GetTreeView()
{
	return (CTreeView*)m_WndSplitter.GetPane(0, 0);
}

CListView *CMyDirView::GetListView()
{
	return (CListView*)m_WndSplitter.GetPane(0, 1);
}



afx_msg LRESULT CMyDirView::OnTreeselchanged(WPARAM wParam, LPARAM lParam)
{
	HTREEITEM hItem = (HTREEITEM)wParam;
	if (!m_TreeView || !m_ListView) {
		return FALSE;
	}
	if (m_TreeView->GetTreeCtrl().ItemHasChildren(hItem)) {
		return TRUE;
	}
	if (hItem == m_TreeView->GetTreeCtrl().GetRootItem()){
		GetDiskInfo(hItem);
		m_TreeView->GetTreeCtrl().Expand(hItem, TVE_EXPAND);
		return TRUE;
	}
	CString path;
	//取当前所在目录
	if (!GetMyDirLeftPath(hItem, path))
		return FALSE;

	m_TreeView->DeleteTreeChild(hItem);
	GetDirInfo(hItem, path);

	m_TreeView->GetTreeCtrl().Expand(hItem, TVE_EXPAND);

	//update list
	//char szTemp[500];
	//memset(szTemp, 0, sizeof(szTemp));
	//sprintf(szTemp, "%s", path);
	//m_ListView->m_container->SendMessage(WM_LISTINSERTDATA, (WPARAM)szTemp, 0);
	GetFileInfo(path);
	




	return TRUE;
}
