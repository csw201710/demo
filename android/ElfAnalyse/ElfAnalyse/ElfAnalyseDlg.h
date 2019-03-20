
// ElfAnalyseDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "elf.h"
#include <map>
#include <iostream>
using namespace std;

typedef struct
{
	WCHAR*     szTitle;           //列表的名称
	int		  nWidth;            //列表的宽度

}COLUMNSTRUCT;

// CElfAnalyseDlg 对话框
class CElfAnalyseDlg : public CDialogEx
{
// 构造
public:
	CElfAnalyseDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_ELFANALYSE_DIALOG };
	
	BOOL CElfAnalyseDlg::LoadFileData(WCHAR* wzFilePath,CHAR** szFileData,ULONG* ulLow);
	VOID SetElfHeaderInfo(char* m_szFileData);
	VOID CElfAnalyseDlg::InitListControl(int number,COLUMNSTRUCT* Item);

	VOID CElfAnalyseDlg::InsertPhdr64(Elf64_Phdr Phdr);
	VOID CElfAnalyseDlg::InsertPhdr32(Elf32_Phdr Phdr);

	VOID CElfAnalyseDlg::InsertString32(Elf32_Shdr Shdr);
	VOID CElfAnalyseDlg::InsertString64(Elf64_Shdr Shdr);

	VOID CElfAnalyseDlg::InsertSym32(Elf32_Shdr Shdr,Elf32_Shdr DynString);
	VOID CElfAnalyseDlg::InsertSym64(Elf64_Shdr Shdr,Elf64_Shdr DynString);

	VOID CElfAnalyseDlg::InsertShdr32(Elf32_Shdr Shdr);
	VOID CElfAnalyseDlg::InsertShdr64(Elf64_Shdr Shdr);

	VOID CElfAnalyseDlg::InsertRel32(Elf32_Shdr Shdr);
	VOID CElfAnalyseDlg::InsertRel64(Elf64_Shdr Shdr);

	TCHAR				m_FilePath[1024];
	CHAR*				m_szFileData;
	ULONG				m_ulLow;
	BOOL				is64Bit;
	BOOL				isNoSPARC;

	map<CString,Elf32_Shdr>Map_Shdr32;
	map<CString,Elf64_Shdr>Map_Shdr64;

	map <CString,Elf32_Phdr>Map_Phdr32;
	map <CString,Elf64_Phdr>Map_Phdr64;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	LONGLONG m_phoff;
	LONGLONG m_phnum;
	CString m_type;
	LONGLONG m_shoff;
	LONGLONG m_shentsize;
	CString m_machine;
	LONGLONG m_flags;
	LONGLONG m_shnum;
	CString m_version;
	LONGLONG m_ehsize;
	LONGLONG m_shstrndx;
	LONGLONG m_entry;
	LONGLONG m_phentsize;
	LONGLONG m_total;
	CTreeCtrl m_Tree;
	CListCtrl m_List;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	CString m_EditFilePath;
	CString m_EditFileType;
	CString m_ident;
	afx_msg void OnClickTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkTree(NMHDR *pNMHDR, LRESULT *pResult);
};

