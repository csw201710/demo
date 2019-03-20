
#include "stdafx.h"
#include "ElfAnalyse.h"
#include "ElfAnalyseDlg.h"
#include "afxdialogex.h"
#include "elf.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString e_machine1[]={
	L"No machine",   //=0
	L"AT&T WE 32100", //=1
	L"SPARC", //=2
	L"Intel 80386", //=3
	L"Motorola 68000", //=4
	L"Motorola 88000", //=5
	L"Reserved for future use (was EM_486)", //=6
	L"Intel 80860", //=7
	L"MIPS I Architecture", //=8
	L"IBM System/370 Processor", //=9
	L"MIPS RS3000 Little-endian", //=10
	L"Reserved for future use", //=11
	L"Reserved for future use", //=12
	L"Reserved for future use", //=13
	L"Reserved for future use", //=14
	L"Hewlett-Packard PA-RISC", //=15
	L"Reserved for future use", //=16
	L"Fujitsu VPP500", //=17
	L"Enhanced instruction set SPARC", //=18
	L"Intel 80960", //=19
	L"PowerPC", //=20
	L"64-bit PowerPC", //=21
	L"IBM System/390 Processor", //=22
	L"Reserved for future use", //=23
	L"Reserved for future use", //=24
	L"Reserved for future use", //=25
	L"Reserved for future use", //=26
	L"Reserved for future use", //=27
	L"Reserved for future use", //=28
	L"Reserved for future use", //=29
	L"Reserved for future use", //=30
	L"Reserved for future use", //=31
	L"Reserved for future use", //=32
	L"Reserved for future use", //=33
	L"Reserved for future use", //=34
	L"Reserved for future use", //=35
	L"NEC V800", //=36
	L"Fujitsu FR20", //=37
	L"TRW RH-32", //=38
	L"Motorola RCE", //=39
	L"Advanced RISC Machines ARM", //=40
	L"Digital Alpha", //=41
	L"Hitachi SH", //=42
	L"SPARC Version 9", //=43
	L"Siemens TriCore embedded processor", //=44
	L"Argonaut RISC Core, Argonaut Technologies Inc.", //=45
	L"Hitachi H8/300", //=46
	L"Hitachi H8/300H", //=47
	L"Hitachi H8S", //=48
	L"Hitachi H8/500", //=49
	L"Intel IA-64 processor architecture", //=50
	L"Stanford MIPS-X", //=51
	L"Motorola ColdFire", //=52
	L"Motorola M68HC12", //=53
	L"Fujitsu MMA Multimedia Accelerator", //=54
	L"Siemens PCP", //=55
	L"Sony nCPU embedded RISC processor", //=56
	L"Denso NDR1 microprocessor", //=57
	L"Motorola Star*Core processor", //=58
	L"Toyota ME16 processor", //=59
	L"STMicroelectronics ST100 processor", //=60
	L"Advanced Logic Corp. TinyJ embedded processor family", //=61
	L"AMD x86-64 architecture", //=62
	L"Sony DSP Processor", //=63
	L"Digital Equipment Corp. PDP-10", //=64
	L"Digital Equipment Corp. PDP-11", //=65
	L"Siemens FX66 microcontroller", //=66
	L"STMicroelectronics ST9+ 8/16 bit microcontroller", //=67
	L"STMicroelectronics ST7 8-bit microcontroller", //=68
	L"Motorola MC68HC16 Microcontroller", //=69
	L"Motorola MC68HC11 Microcontroller", //=70
	L"Motorola MC68HC08 Microcontroller", //=71
	L"Motorola MC68HC05 Microcontroller", //=72
	L"Silicon Graphics SVx", //=73
	L"",//74
	L"Digital VAX", //=75
	L"Axis Communications 32-bit embedded processor", //=76
	L"Infineon Technologies 32-bit embedded processor", //=77
	L"Element 14 64-bit DSP Processor", //=78
	L"LSI Logic 16-bit DSP Processor", //=79
	L"Donald Knuth's educational 64-bit processor", //=80
	L"Harvard University machine-independent object files", //=81
	L"SiTera Prism", //=82
	L"Atmel AVR 8-bit microcontroller", //=83
	L"Fujitsu FR30", //=84
	L"Mitsubishi D10V", //=85
	L"Mitsubishi D30V", //=86
	L"NEC v850", //=87
	L"Mitsubishi M32R", //=88
	L"Matsushita MN10300", //=89
	L"Matsushita MN10200", //=90
	L"picoJava", //=91
	L"OpenRISC 32-bit embedded processor", //=92
	L"ARC Cores Tangent-A5", //=93
	L"Tensilica Xtensa Architecture", //=94
	L"Alphamosaic VideoCore processor", //=95
	L"Thompson Multimedia General Purpose Processor", //=96
	L"National Semiconductor 32000 series", //=97
	L"Tenor Network TPC processor", //=98
	L"Trebia SNP 1000 processor", //=99
	L"STMicroelectronics (www.st.com) ST200 microcontroller", //=100
	L"Ubicom IP2xxx microcontroller family", //=101
	L"MAX Processor", //=102
	L"National Semiconductor CompactRISC microprocessor", //=103
	L"Fujitsu F2MC16", //=104
	L"Texas Instruments embedded microcontroller msp430", //=105
	L"Analog Devices Blackfin (DSP) processor", //=106
	L"S1C33 Family of Seiko Epson processors", //=107
	L"Sharp embedded microprocessor", //=108
	L"Arca RISC Microprocessor", //=109
	L"Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University"//110
};
CString e_type1[]={
	L"No file type",
	L"Relocatable file",
	L"Executable file",
	L"Shared object file",
	L"Core file"
	L"",
	L"",
	L"",
	L""
};

CString st_info[]={
	L"STB_LOCAL",//0
		L"STB_GOBAL",
		L"STB_WEAK",//2
		L"STB_NUM",
		L"",
		L"",
		L"",
		L"",
		L"",
		L"",
		L"STB_LOOS",//010
		L"",
		L"STB_HIOS",//012
		L"STB_LOPROC",//013
		L"",
		L"STB_HIPROC"//015
};
CString sym_info_type_e[]={
	L"STT_NOTYPE",      //= 0x0,
	L"STT_OBJECT",     // = 0x1,
	L"STT_FUNC",      //  = 0x2,
	L"STT_SECTION",   //  = 0x3,
	L"STT_FILE",      //  = 0x4,
	L"STT_COMMON",    //  = 0x5,
	L"STT_TLS",      //   = 0x6,
	L"STT_NUM",       //  = 0x7,
	L"",
	L"",
	L"STT_LOOS",      //  = 0xA,
	L"STT_GNU_IFUNC", //  = 0xA,
	L"STT_HIOS",       // = 0xB,
	L"STT_LOPROC",    //  = 0xC,
	L"STT_HIPROC"     // = 0xD
} ;


COLUMNSTRUCT g_Column_Section_Header[] = 
{
	{L"项目",			    300	},
	{L"值",			300	},
};

//重定位表
COLUMNSTRUCT g_Column_REL_Table[] = 
{
	{L"重定位地址 - r_offset",			    148	},
	{L"重定位类型和相关符号 - r_info",			150	},
};

//符号表
COLUMNSTRUCT g_Column_Dynsm[] = 
{
	{L"符号表索引 - st_name",			    148	},
	{L"地址 - st_value",			150	},
	{L"代码长度 - st_size",	160	},
	{L"绑定属性和类型 - st_info",		250	},
	{L"未用 - st_other",			    80	},
	{L"所在节的节索引号 - st_shndx",		   150	}
};
//字符串
COLUMNSTRUCT g_Column_String_Table[] = 
{
	{L"字符串",			    300	}
};


int g_Column_Section_Header_Number = 2; //列表的个数
int g_Column_REL_Table_Number = 2; //列表的个数
int g_Column_Dynsm_Number = 6;
int g_Column_String_Table_Number = 1;
int g_Column_Width = 0; 



CHAR*	g_szFileData = NULL;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CElfAnalyseDlg 对话框




CElfAnalyseDlg::CElfAnalyseDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CElfAnalyseDlg::IDD, pParent)
	, m_phnum(0)
	, m_type(_T(""))
	, m_shoff(0)
	,m_phoff(0)
	, m_shentsize(0)
	, m_machine(_T(""))
	, m_flags(0)
	, m_shnum(0)
	, m_version(_T(""))
	, m_ehsize(0)
	, m_shstrndx(0)
	, m_entry(0)
	, m_phentsize(0)
	, m_total(0)
	, m_EditFilePath(_T(""))
	, m_EditFileType(_T(""))
	, m_ident(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	isNoSPARC = FALSE;
}

void CElfAnalyseDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_phoff, m_phoff);
	DDX_Text(pDX, IDC_EDIT_phnum, m_phnum);
	DDX_Text(pDX, IDC_EDIT_type, m_type);
	DDX_Text(pDX, IDC_EDIT_shoff, m_shoff);
	DDX_Text(pDX, IDC_EDIT_shentsize, m_shentsize);
	DDX_Text(pDX, IDC_EDIT_machine, m_machine);
	DDX_Text(pDX, IDC_EDIT_flags, m_flags);
	DDX_Text(pDX, IDC_EDIT_shnum, m_shnum);
	DDX_Text(pDX, IDC_EDIT_version, m_version);
	DDX_Text(pDX, IDC_EDIT_ehsize, m_ehsize);
	DDX_Text(pDX, IDC_EDIT_shstrndx, m_shstrndx);
	DDX_Text(pDX, IDC_EDIT_entry, m_entry);
	DDX_Text(pDX, IDC_EDIT_phentsize, m_phentsize);
	DDX_Text(pDX, IDC_EDIT_total, m_total);
	DDX_Control(pDX, IDC_TREE, m_Tree);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_EDIT_FILEPATH, m_EditFilePath);
	DDX_Text(pDX, IDC_EDIT_FILETYPE, m_EditFileType);
	DDX_Text(pDX, IDC_EDIT_ident, m_ident);
}

BEGIN_MESSAGE_MAP(CElfAnalyseDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DROPFILES()
	ON_NOTIFY(NM_CLICK, IDC_TREE, &CElfAnalyseDlg::OnClickTree)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, &CElfAnalyseDlg::OnDblclkTree)
END_MESSAGE_MAP()


// CElfAnalyseDlg 消息处理程序

BOOL CElfAnalyseDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码


	m_Tree.SetItemHeight(20);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CElfAnalyseDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CElfAnalyseDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CElfAnalyseDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CElfAnalyseDlg::OnDropFiles(HDROP hDropInfo)
{
	
	UpdateData(TRUE);

	m_Tree.DeleteAllItems();
	DragQueryFile(hDropInfo,0,(LPWSTR)(LPCTSTR)m_FilePath,MAX_PATH);//取得第一个文件的路径
	if(_wcsicmp(m_FilePath,m_EditFilePath.GetBuffer()) == 0)
	{
		goto END;
	}
	m_EditFilePath = m_FilePath;
	if(LoadFileData(m_FilePath,&m_szFileData,&m_ulLow)==FALSE)
	{
		free(m_szFileData);
		goto END;
	}


	if (!(
		(((char*)m_szFileData)[EI_MAG0] == 0x7F) &&
		(((char*)m_szFileData)[EI_MAG1] == 'E') &&
		(((char*)m_szFileData)[EI_MAG2] == 'L') &&
		(((char*)m_szFileData)[EI_MAG3] == 'F'))) {
			MessageBox(L"非ELF 格式文件或者文件已被损坏!",L"Error");
			goto END;
	}


	g_szFileData = m_szFileData;
	if(((char*)m_szFileData)[EI_CLASS] == ELFCLASS64)
	{
		m_EditFileType = L"64位 .ELF";
		is64Bit = TRUE;
	}
	else if(((char*)m_szFileData)[EI_CLASS] == ELFCLASS32)
	{
		m_EditFileType = L"32位 .ELF";
		is64Bit = FALSE;
	}
	else if(((char*)m_szFileData)[EI_CLASS] == ELFCLASSNONE)
	{
		MessageBox(L"无效类",L"Error");
		goto END;
	}

	SetElfHeaderInfo(m_szFileData);  //解析ELF_HEAD






	UpdateData(FALSE);


END:
	CDialogEx::OnDropFiles(hDropInfo);
}


BOOL CElfAnalyseDlg::LoadFileData(WCHAR* wzFilePath,CHAR** szFileData,ULONG* ulLow)
{

	HANDLE hFile;
	ULONG ulHigh = 0;
	ULONG ulReturn = 0;

	if (wzFilePath==NULL)
	{
		return FALSE;
	}

	hFile = CreateFileW(wzFilePath,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if (hFile==INVALID_HANDLE_VALUE)
	{
		return -1;
	}

	*ulLow = GetFileSize(hFile,&ulHigh); 

	*szFileData = new char[*ulLow + 20];

	if (ReadFile(hFile,*szFileData,*ulLow,&ulReturn,NULL)==0)
	{
		CloseHandle(hFile);
		delete *szFileData;
		return FALSE;
	}


	return TRUE;
}


WCHAR*  CharToWchar(char* szSour)   //Hello\0           h 00 e 00 l 00 l 00 o 00 00 00 
{
	WCHAR* wzBuffer = NULL;
	if (szSour)
	{
		size_t  szLen = strlen(szSour); //5
		size_t  wzLen = (size_t)MultiByteToWideChar(CP_ACP,0,szSour,int(szLen),NULL,0);  
		wzBuffer = new WCHAR[wzLen+1];
		memset(wzBuffer,0,sizeof(WCHAR)*(wzLen+1));
		MultiByteToWideChar(CP_ACP,0,szSour,int(szLen),wzBuffer,int(wzLen));   
	}
	return wzBuffer;
}
VOID CElfAnalyseDlg::SetElfHeaderInfo(char* m_szFileData)
{
	if(is64Bit==TRUE)
	{
		Elf64_Ehdr* Elf_Ehdr = (Elf64_Ehdr*)(m_szFileData);
		m_ident = L".ELF";

		m_type.Format(L"%d :",Elf_Ehdr->e_type);
		if(Elf_Ehdr->e_type>=0&&Elf_Ehdr->e_type<=4)
		{
			m_EditFileType += L"(";
			m_EditFileType += e_type1[Elf_Ehdr->e_type];
			m_EditFileType += L")";
			m_type += e_type1[Elf_Ehdr->e_type];
		}
		else
		{
			m_EditFileType += L"";
			m_type += L"";
		}


		m_machine.Format(L"%d :",Elf_Ehdr->e_machine);
		if(Elf_Ehdr->e_machine==EM_SPARCV9)
		{
			isNoSPARC = TRUE;
		}
		else
		{
			isNoSPARC = FALSE;
		}
		if(Elf_Ehdr->e_machine>=0&&Elf_Ehdr->e_machine<=110)
		{
			m_machine += e_machine1[Elf_Ehdr->e_machine];
		}
		else
		{
			m_machine += L"";
		}



		m_version.Format(L"%d :",Elf_Ehdr->e_version);
		if(Elf_Ehdr->e_version==0) //EV_NONE
		{
			m_version += L"无效版本";
		}
		else if(Elf_Ehdr->e_version>=0)//EV_CURRENT
		{
			m_version += L"当前版本";
		}
		m_entry = Elf_Ehdr->e_entry;
		m_phoff = Elf_Ehdr->e_phoff;
		m_shoff = Elf_Ehdr->e_shoff;
		m_flags = Elf_Ehdr->e_flags;
		m_ehsize = Elf_Ehdr->e_ehsize;
		m_phentsize = Elf_Ehdr->e_phentsize;
		m_phnum = Elf_Ehdr->e_phnum;
		m_shentsize = Elf_Ehdr->e_shentsize;
		m_phnum = Elf_Ehdr->e_phnum;
		m_shentsize = Elf_Ehdr->e_shentsize;
		m_shnum = Elf_Ehdr->e_shnum;
		m_shstrndx = Elf_Ehdr->e_shstrndx;


		Elf64_Phdr* Phdr = (Elf64_Phdr*)(m_szFileData+Elf_Ehdr->e_phoff);
		for(int i = 0;i<Elf_Ehdr->e_phnum;i++,Phdr++)
		{
			CString item;
			item.Format(L"Program Header %d",i);
			m_Tree.InsertItem(item,0,2);

			Map_Phdr64.insert(map<CString,Elf64_Phdr>::value_type(item,*(Elf64_Phdr*)Phdr));
		}



		char * pStrTable;	// 用以取得每个 section 的名字
		Elf64_Shdr * ShdrStringTable = (Elf64_Shdr *)(m_szFileData + Elf_Ehdr->e_shoff) + Elf_Ehdr->e_shstrndx;
		pStrTable = (char *)(m_szFileData + ShdrStringTable->sh_offset);

		Elf64_Shdr* Shdr = (Elf64_Shdr*)(m_szFileData+Elf_Ehdr->e_shoff); 
		for(int i = 0;i< Elf_Ehdr->e_shnum; i++,Shdr++)
		{
			WCHAR* Temp = CharToWchar(pStrTable + Shdr->sh_name);
			CString item;
			item.Format(L"%s",Temp);
			HTREEITEM Handle = m_Tree.InsertItem(item,0,2);
			if(item==L".shstrtab"||item==L".strtab"||item==L".dynstr")
			{
				m_Tree.InsertItem(L"String Table 字符串表",1,3,Handle);
			}
			else if(item==L".symtab")
			{
				m_Tree.InsertItem(L"Symtab 符号表",1,3,Handle);
			}
			else if(item==L".rel.dyn"||item==L".rel.plt")
			{
				m_Tree.InsertItem(L"REL Table 重定位表",1,3,Handle);
			}
			else if(item==L".dynsym")
			{
				m_Tree.InsertItem(L"Dynsym 符号表",1,3,Handle);
			}
			Map_Shdr64.insert(map<CString,Elf64_Shdr>::value_type(item,*(Elf64_Shdr*)Shdr));


		}
	}	
	else
	{
		Elf32_Ehdr* Elf_Ehdr = (Elf32_Ehdr*)(m_szFileData);
		m_ident = L".ELF";

		m_type.Format(L"%d :",Elf_Ehdr->e_type);
		if(Elf_Ehdr->e_type>=0&&Elf_Ehdr->e_type<=4)
		{
			m_EditFileType += L"(";
			m_EditFileType += e_type1[Elf_Ehdr->e_type];
			m_EditFileType += L")";
			m_type += e_type1[Elf_Ehdr->e_type];
		}
		else
		{
			m_EditFileType += L"";
			m_type += L"";
		}


		m_machine.Format(L"%d :",Elf_Ehdr->e_machine);
		if(Elf_Ehdr->e_machine>=0&&Elf_Ehdr->e_machine<=110)
		{
			m_machine += e_machine1[Elf_Ehdr->e_machine];
		}
		else
		{
			m_machine += L"";
		}



		m_version.Format(L"%d :",Elf_Ehdr->e_version);
		if(Elf_Ehdr->e_version==0) //EV_NONE
		{
			m_version += L"无效版本";
		}
		else if(Elf_Ehdr->e_version>=0)//EV_CURRENT
		{
			m_version += L"当前版本";
		}
		m_entry = Elf_Ehdr->e_entry;
		m_phoff = Elf_Ehdr->e_phoff;
		m_shoff = Elf_Ehdr->e_shoff;
		m_flags = Elf_Ehdr->e_flags;
		m_ehsize = Elf_Ehdr->e_ehsize;
		m_phentsize = Elf_Ehdr->e_phentsize;
		m_phnum = Elf_Ehdr->e_phnum;
		m_shentsize = Elf_Ehdr->e_shentsize;
		m_phnum = Elf_Ehdr->e_phnum;
		m_shentsize = Elf_Ehdr->e_shentsize;
		m_shnum = Elf_Ehdr->e_shnum;
		m_shstrndx = Elf_Ehdr->e_shstrndx;


		Elf32_Phdr* Phdr = (Elf32_Phdr*)(m_szFileData+Elf_Ehdr->e_phoff);
		for(int i = 0;i<Elf_Ehdr->e_phnum;i++,Phdr++)
		{
			CString item;
			item.Format(L"Program Header %d",i);
			m_Tree.InsertItem(item,0,2);
			Map_Phdr32.insert(map<CString,Elf32_Phdr>::value_type(item,*(Elf32_Phdr*)Phdr));
		}



		char * pStrTable;	// 用以取得每个 section 的名字
		Elf32_Shdr * ShdrStringTable = (Elf32_Shdr *)(m_szFileData + Elf_Ehdr->e_shoff) + Elf_Ehdr->e_shstrndx;
		pStrTable = (char *)(m_szFileData + ShdrStringTable->sh_offset);

		Elf32_Shdr* Shdr = (Elf32_Shdr*)(m_szFileData+Elf_Ehdr->e_shoff); 
		for(int i = 0;i< Elf_Ehdr->e_shnum; i++,Shdr++)
		{
			WCHAR* Temp = CharToWchar(pStrTable + Shdr->sh_name);
			CString item;
			item.Format(L"%s",Temp);
// 			if(item==L"")
// 			{
// 				continue;
// 			}
			HTREEITEM Handle = m_Tree.InsertItem(item,0,2);
			if(item==L".shstrtab"||item==L".strtab"||item==L".dynstr")
			{
				m_Tree.InsertItem(L"String Table 字符串表",1,3,Handle);
			}
			else if(item==L".symtab")
			{
				m_Tree.InsertItem(L"Symtab 符号表",1,3,Handle);
			}
			else if(item==L".rel.dyn"||item==L".rel.plt")
			{
				m_Tree.InsertItem(L"REL Table 重定位表",1,3,Handle);
			}
			else if(item==L".dynsym")
			{
				m_Tree.InsertItem(L"Dynsym 符号表",1,3,Handle);
			}
			Map_Shdr32.insert(map<CString,Elf32_Shdr>::value_type(item,*(Elf32_Shdr*)Shdr));
		}
	}
	UpdateData(FALSE);
}

void CElfAnalyseDlg::OnClickTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hCurItem=m_Tree.GetSelectedItem();   
	if (hCurItem==NULL)  
	{  
		return;  
	}  
	CString pPath=m_Tree.GetItemText(hCurItem);  

	if(is64Bit)
	{
		if(pPath.Find(L"Program Header")==0)
		{
			map<CString,Elf64_Phdr>::iterator it;
			it = Map_Phdr64.find(pPath);
			Elf64_Phdr Temp = it->second;


			InitListControl(g_Column_Section_Header_Number,g_Column_Section_Header);

			InsertPhdr64(Temp);


		}
		else if(pPath==L"String Table 字符串表")
		{
			//MessageBox(L"1",L"1");
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);


			InitListControl(g_Column_String_Table_Number,g_Column_String_Table);

			map<CString,Elf64_Shdr>::iterator it;
			it = Map_Shdr64.find(pParentPath);
			Elf64_Shdr Temp = it->second;

			InsertString64(Temp);

		}
		else if(pPath==L"Dynsym 符号表"||pPath==L"Symtab 符号表")
		{
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);


			InitListControl(g_Column_Dynsm_Number,g_Column_Dynsm);


			map<CString,Elf64_Shdr>::iterator it;
			it = Map_Shdr64.find(pParentPath);
			Elf64_Shdr ParentShdr = it->second;

			Elf64_Shdr DynString;
			if(pPath==L"Symtab 符号表")
			{
				it = Map_Shdr64.find(L".strtab");
				DynString = it->second;
			}
			else if(pPath==L"Dynsym 符号表")
			{
				it = Map_Shdr64.find(L".dynstr");
				DynString = it->second;
			}

			InsertSym64(ParentShdr,DynString);


		}
		else if(pPath==L"REL Table 重定位表")
		{
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);

			InitListControl(g_Column_REL_Table_Number,g_Column_REL_Table);

			map<CString,Elf64_Shdr>::iterator it;
			it = Map_Shdr64.find(pParentPath);
			Elf64_Shdr ParentShdr = it->second;

			InsertRel64(ParentShdr);


		}
		else
		{
			map<CString,Elf64_Shdr>::iterator it;
			it = Map_Shdr64.find(pPath);
			Elf64_Shdr Temp = it->second;

			InitListControl(g_Column_Section_Header_Number,g_Column_Section_Header);

			InsertShdr64(Temp);


		}	

	}
	else
	{
		if(pPath.Find(L"Program Header")==0)
		{
			map<CString,Elf32_Phdr>::iterator it;
			it = Map_Phdr32.find(pPath);
			Elf32_Phdr Temp = it->second;


			InitListControl(g_Column_Section_Header_Number,g_Column_Section_Header);

			InsertPhdr32(Temp);


		}
		else if(pPath==L"String Table 字符串表")
		{
			//MessageBox(L"1",L"1");
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);


			InitListControl(g_Column_String_Table_Number,g_Column_String_Table);

			map<CString,Elf32_Shdr>::iterator it;
			it = Map_Shdr32.find(pParentPath);
			Elf32_Shdr Temp = it->second;

			InsertString32(Temp);

		}
		else if(pPath==L"Dynsym 符号表"||pPath==L"Symtab 符号表")
		{
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);


			InitListControl(g_Column_Dynsm_Number,g_Column_Dynsm);


			map<CString,Elf32_Shdr>::iterator it;
			it = Map_Shdr32.find(pParentPath);
			Elf32_Shdr ParentShdr = it->second;

			Elf32_Shdr DynString;
			if(pPath==L"Symtab 符号表")
			{
				it = Map_Shdr32.find(L".strtab");
				DynString = it->second;
			}
			else if(pPath==L"Dynsym 符号表")
			{
				it = Map_Shdr32.find(L".dynstr");
				DynString = it->second;
			}

			InsertSym32(ParentShdr,DynString);


		}
		else if(pPath==L"REL Table 重定位表")
		{
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);

			InitListControl(g_Column_REL_Table_Number,g_Column_REL_Table);

			map<CString,Elf32_Shdr>::iterator it;
			it = Map_Shdr32.find(pParentPath);
			Elf32_Shdr ParentShdr = it->second;

			InsertRel32(ParentShdr);


		}
		else
		{
			map<CString,Elf32_Shdr>::iterator it;
			it = Map_Shdr32.find(pPath);
			Elf32_Shdr Temp = it->second;

			InitListControl(g_Column_Section_Header_Number,g_Column_Section_Header);

			InsertShdr32(Temp);


		}	
	}
}


void CElfAnalyseDlg::OnDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	HTREEITEM hCurItem=m_Tree.GetSelectedItem();   
	if (hCurItem==NULL)  
	{  
		return;  
	}  
	CString pPath=m_Tree.GetItemText(hCurItem);  

	if(is64Bit)
	{
		if(pPath.Find(L"Program Header")==0)
		{
			map<CString,Elf64_Phdr>::iterator it;
			it = Map_Phdr64.find(pPath);
			Elf64_Phdr Temp = it->second;


			InitListControl(g_Column_Section_Header_Number,g_Column_Section_Header);

			InsertPhdr64(Temp);


		}
		else if(pPath==L"String Table 字符串表")
		{
			//MessageBox(L"1",L"1");
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);


			InitListControl(g_Column_String_Table_Number,g_Column_String_Table);

			map<CString,Elf64_Shdr>::iterator it;
			it = Map_Shdr64.find(pParentPath);
			Elf64_Shdr Temp = it->second;

			InsertString64(Temp);

		}
		else if(pPath==L"Dynsym 符号表"||pPath==L"Symtab 符号表")
		{
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);


			InitListControl(g_Column_Dynsm_Number,g_Column_Dynsm);


			map<CString,Elf64_Shdr>::iterator it;
			it = Map_Shdr64.find(pParentPath);
			Elf64_Shdr ParentShdr = it->second;

			Elf64_Shdr DynString;
			if(pPath==L"Symtab 符号表")
			{
				it = Map_Shdr64.find(L".strtab");
				DynString = it->second;
			}
			else if(pPath==L"Dynsym 符号表")
			{
				it = Map_Shdr64.find(L".dynstr");
				DynString = it->second;
			}

			InsertSym64(ParentShdr,DynString);


		}
		else if(pPath==L"REL Table 重定位表")
		{
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);

			InitListControl(g_Column_REL_Table_Number,g_Column_REL_Table);

			map<CString,Elf64_Shdr>::iterator it;
			it = Map_Shdr64.find(pParentPath);
			Elf64_Shdr ParentShdr = it->second;

			InsertRel64(ParentShdr);


		}
		else//其他节表信息
		{
			map<CString,Elf64_Shdr>::iterator it;
			it = Map_Shdr64.find(pPath);
			Elf64_Shdr Temp = it->second;

			InitListControl(g_Column_Section_Header_Number,g_Column_Section_Header);

			InsertShdr64(Temp);


		}	

	}
	else
	{
		if(pPath.Find(L"Program Header")==0)
		{
			map<CString,Elf32_Phdr>::iterator it;
			it = Map_Phdr32.find(pPath);
			Elf32_Phdr Temp = it->second;

		
			InitListControl(g_Column_Section_Header_Number,g_Column_Section_Header);
		
			InsertPhdr32(Temp);
			

		}
		else if(pPath==L"String Table 字符串表")
		{
			//MessageBox(L"1",L"1");
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);


			InitListControl(g_Column_String_Table_Number,g_Column_String_Table);

			map<CString,Elf32_Shdr>::iterator it;
			it = Map_Shdr32.find(pParentPath);
			Elf32_Shdr Temp = it->second;

			InsertString32(Temp);
		
		}
		else if(pPath==L"Dynsym 符号表"||pPath==L"Symtab 符号表")
		{
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);


			InitListControl(g_Column_Dynsm_Number,g_Column_Dynsm);
			

			map<CString,Elf32_Shdr>::iterator it;
			it = Map_Shdr32.find(pParentPath);
			Elf32_Shdr ParentShdr = it->second;

			Elf32_Shdr DynString;
			if(pPath==L"Symtab 符号表")
			{
				it = Map_Shdr32.find(L".strtab");
				DynString = it->second;
			}
			else if(pPath==L"Dynsym 符号表")
			{
				it = Map_Shdr32.find(L".dynstr");
				DynString = it->second;
			}
		
			InsertSym32(ParentShdr,DynString);

		
		}
		else if(pPath==L"REL Table 重定位表")
		{
			HTREEITEM hParent = m_Tree.GetParentItem(hCurItem);
			CString pParentPath  = m_Tree.GetItemText(hParent);

			InitListControl(g_Column_REL_Table_Number,g_Column_REL_Table);
			
			map<CString,Elf32_Shdr>::iterator it;
			it = Map_Shdr32.find(pParentPath);
			Elf32_Shdr ParentShdr = it->second;

			InsertRel32(ParentShdr);


		}
		else
		{
			map<CString,Elf32_Shdr>::iterator it;
			it = Map_Shdr32.find(pPath);
			Elf32_Shdr Temp = it->second;
		
			InitListControl(g_Column_Section_Header_Number,g_Column_Section_Header);
	
			InsertShdr32(Temp);

	
		}	
	}
	

}


VOID CElfAnalyseDlg::InitListControl(int number,COLUMNSTRUCT* Item)
{


	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	g_Column_Width = 0;
	m_List.DeleteAllItems();

	//删除所有标题
	CHeaderCtrl* pHeaderCtrl = m_List.GetHeaderCtrl();
	if (pHeaderCtrl != NULL)
	{
		int  nColumnCount = pHeaderCtrl->GetItemCount();
		for (int i=0; i<nColumnCount; i++)
		{
			m_List.DeleteColumn(0);
		}
	}

	//初始化
	for (int i = 0; i < number; i++)
	{
		m_List.InsertColumn(i, Item[i].szTitle,LVCFMT_CENTER,Item[i].nWidth);

		g_Column_Width+=Item[i].nWidth;  
	}
	
}

VOID CElfAnalyseDlg::InsertPhdr32(Elf32_Phdr Phdr)
{
	CString p_type,p_offset,p_vaddr,p_paddr,p_filesz,p_memsz,p_flags,p_align;
	p_type.Format(L"%d",Phdr.p_type);
	p_offset.Format(L"0x%x",Phdr.p_offset);
	p_vaddr.Format(L"0x%x",Phdr.p_vaddr);
	p_paddr.Format(L"0x%x",Phdr.p_paddr);
	p_filesz.Format(L"0x%x",Phdr.p_filesz);
	p_memsz.Format(L"0x%x",Phdr.p_memsz);
	p_flags.Format(L"%d",Phdr.p_flags);
	p_align.Format(L"%d",Phdr.p_align);

	m_List.InsertItem(0,L"段类型--p_type");
	m_List.SetItemText(0,1,p_type);

	m_List.InsertItem(1,L"段相对文件偏移--------------------p_offset");
	m_List.SetItemText(1,1,p_offset);

	m_List.InsertItem(2,L"段在虚拟内存地址-------------------p_vaddr");
	m_List.SetItemText(2,1,p_vaddr);

	m_List.InsertItem(3,L"段在物理内存地址----p_paddr");
	m_List.SetItemText(3,1,p_paddr);

	m_List.InsertItem(4,L"段在文件中长度----------p_filesz");
	m_List.SetItemText(4,1,p_filesz);

	m_List.InsertItem(5,L"段在内存中长度------p_memsz");
	m_List.SetItemText(5,1,p_memsz);

	m_List.InsertItem(6,L"段标记--p_flags");
	m_List.SetItemText(6,1,p_flags);

	m_List.InsertItem(7,L"段在内存和文件对齐标志------------p_align");
	m_List.SetItemText(7,1,p_align);
}
VOID CElfAnalyseDlg::InsertPhdr64(Elf64_Phdr Phdr)
{
	CString p_type,p_offset,p_vaddr,p_paddr,p_filesz,p_memsz,p_flags,p_align;
	p_type.Format(L"%d",Phdr.p_type);
	p_offset.Format(L"0x%x",Phdr.p_offset);
	p_vaddr.Format(L"0x%x",Phdr.p_vaddr);
	p_paddr.Format(L"0x%x",Phdr.p_paddr);
	p_filesz.Format(L"0x%x",Phdr.p_filesz);
	p_memsz.Format(L"0x%x",Phdr.p_memsz);
	p_flags.Format(L"%d",Phdr.p_flags);
	p_align.Format(L"%d",Phdr.p_align);

	m_List.InsertItem(0,L"段类型--p_type");
	m_List.SetItemText(0,1,p_type);

	m_List.InsertItem(1,L"段相对文件偏移--------------------p_offset");
	m_List.SetItemText(1,1,p_offset);

	m_List.InsertItem(2,L"段在虚拟内存地址-------------------p_vaddr");
	m_List.SetItemText(2,1,p_vaddr);

	m_List.InsertItem(3,L"段在物理内存地址----p_paddr");
	m_List.SetItemText(3,1,p_paddr);

	m_List.InsertItem(4,L"段在文件中长度----------p_filesz");
	m_List.SetItemText(4,1,p_filesz);

	m_List.InsertItem(5,L"段在内存中长度------p_memsz");
	m_List.SetItemText(5,1,p_memsz);

	m_List.InsertItem(6,L"段标记--p_flags");
	m_List.SetItemText(6,1,p_flags);

	m_List.InsertItem(7,L"段在内存和文件对齐标志------------p_align");
	m_List.SetItemText(7,1,p_align);
}


VOID CElfAnalyseDlg::InsertString32(Elf32_Shdr Shdr)
{
	char* szString = g_szFileData+Shdr.sh_offset; 
	int i =0;
	char* oriAddr = szString;
	while(TRUE)
	{
		WCHAR* wzTemp = CharToWchar(szString);
		CString CsTemp = wzTemp;
		m_List.InsertItem(i,CsTemp);
		szString = szString + strlen(szString) + 1;
		if(szString>oriAddr+Shdr.sh_size)
		{
			break;
		}
		i++;	
	}
}
VOID CElfAnalyseDlg::InsertString64(Elf64_Shdr Shdr)
{
	char* szString = g_szFileData+Shdr.sh_offset;  //越过内存中 一个""
	int i =0;
	char* oriAddr = szString;
	while(TRUE)
	{
		WCHAR* wzTemp = CharToWchar(szString);
		CString CsTemp = wzTemp;
		m_List.InsertItem(i,CsTemp);
		szString = szString + strlen(szString) + 1;
		if(szString>oriAddr+Shdr.sh_size)
		{
			break;
		}
		i++;	
	}
}

VOID CElfAnalyseDlg::InsertSym32(Elf32_Shdr Shdr,Elf32_Shdr DynString)
{
	Elf32_Sym* Sym = (Elf32_Sym*)(g_szFileData + Shdr.sh_offset);

	for(int i=0;i<Shdr.sh_size/sizeof(Elf32_Sym);i++)
	{
		CString st_name,st_value,st_size,st_info1,st_other,st_shndx;

		char* szName = g_szFileData + DynString.sh_offset + Sym->st_name;
		WCHAR* wzName = CharToWchar(szName);
		st_name = wzName;

		if(st_name==L"_end")
		{
			int a = 0;
		}
		st_value.Format(L"0x%x",Sym->st_value);
		st_size.Format(L"0x%x",Sym->st_size);


		st_info1.Format(L"0x%x  ",Sym->st_info);

		//sym_info_bind:4
		//sym_info_type:4
		st_info1 += st_info[(Sym->st_info)>>4]; //高4位
		st_info1 += L"  ";
		st_info1 += sym_info_type_e[(Sym->st_info)&0xF]; //低4位

		st_other.Format(L"0x%x",Sym->st_other);

		if(Sym->st_shndx==SHN_UNDEF)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_UNDEF";
		}
		else if(Sym->st_shndx==SHN_LORESERVE)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_LOPROC";
		}
		else if(Sym->st_shndx==SHN_AFTER)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_AFTER";
		}
		else if(Sym->st_shndx==SHN_AMD64_LCOMMON)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_AMD64_LCOMMON";
		}
		else if(Sym->st_shndx==SHN_HIPROC)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_HIPROC";
		}
		else if(Sym->st_shndx==SHN_LOOS)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_LOOS";
		}
		else if(Sym->st_shndx==SHN_LOSUNW)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_LOSUNW";
		}
		else if(Sym->st_shndx==SHN_ABS)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_ABS";
		}
		else if(Sym->st_shndx==SHN_COMMON)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_COMMON";
		}
		else if(Sym->st_shndx==SHN_XINDEX)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_XINDEX";
		}
		else
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
		}

		m_List.InsertItem(i,st_name);
		m_List.SetItemText(i,1,st_value);
		m_List.SetItemText(i,2,st_size);
		m_List.SetItemText(i,3,st_info1);
		m_List.SetItemText(i,4,st_other);


		m_List.SetItemText(i,5,st_shndx);

		(Elf32_Sym*)Sym++;
	}
}
VOID CElfAnalyseDlg::InsertSym64(Elf64_Shdr Shdr,Elf64_Shdr DynString)
{
	Elf64_Sym* Sym = (Elf64_Sym*)(g_szFileData + Shdr.sh_offset);

	for(int i =0;i<Shdr.sh_size/sizeof(Elf64_Sym);i++)
	{
		CString st_name,st_value,st_size,st_info1,st_other,st_shndx;

		char* szName = g_szFileData + DynString.sh_offset + Sym->st_name;
		WCHAR* wzName = CharToWchar(szName);
		st_name = wzName;


		st_value.Format(L"0x%x",Sym->st_value);
		st_size.Format(L"0x%x",Sym->st_size);


		st_info1.Format(L"0x%x  ",Sym->st_info);

		//sym_info_bind:4
		//sym_info_type:4
		st_info1 += st_info[(Sym->st_info)>>4]; //高4位
		st_info1 += L"  ";
		st_info1 += sym_info_type_e[(Sym->st_info)&0xF]; //低4位

		st_other.Format(L"0x%x",Sym->st_other);

		if(Sym->st_shndx==SHN_UNDEF)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_UNDEF";
		}
		else if(Sym->st_shndx==SHN_LORESERVE)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_LOPROC";
		}
		else if(Sym->st_shndx==SHN_AFTER)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_AFTER";
		}
		else if(Sym->st_shndx==SHN_AMD64_LCOMMON)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_AMD64_LCOMMON";
		}
		else if(Sym->st_shndx==SHN_HIPROC)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_HIPROC";
		}
		else if(Sym->st_shndx==SHN_LOOS)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_LOOS";
		}
		else if(Sym->st_shndx==SHN_LOSUNW)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_LOSUNW";
		}
		else if(Sym->st_shndx==SHN_ABS)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_ABS";
		}
		else if(Sym->st_shndx==SHN_COMMON)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_COMMON";
		}
		else if(Sym->st_shndx==SHN_XINDEX)
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
			st_shndx += L"SHN_XINDEX";
		}
		else
		{
			st_shndx.Format(L"0x%x  ",Sym->st_shndx);
		}

		m_List.InsertItem(i,st_name);
		m_List.SetItemText(i,1,st_value);
		m_List.SetItemText(i,2,st_size);
		m_List.SetItemText(i,3,st_info1);
		m_List.SetItemText(i,4,st_other);


		m_List.SetItemText(i,5,st_shndx);

		(Elf64_Sym*)Sym++;
	}
}

VOID CElfAnalyseDlg::InsertShdr32(Elf32_Shdr Shdr)
{
	CString sh_name,sh_type,sh_flags,sh_addr,sh_offset,sh_size,sh_link,sh_info,sh_addralign,sh_entsize;
	sh_name.Format(L"%d",Shdr.sh_name);
	sh_type.Format(L"%d",Shdr.sh_type);
	sh_flags.Format(L"%d",Shdr.sh_flags);
	sh_addr.Format(L"0x%x",Shdr.sh_addr);
	sh_offset.Format(L"0x%x",Shdr.sh_offset);
	sh_size.Format(L"0x%x",Shdr.sh_size);
	sh_link.Format(L"0x%x",Shdr.sh_link);
	sh_info.Format(L"0x%x",Shdr.sh_info);
	sh_addralign.Format(L"0x%x",Shdr.sh_addralign);
	sh_entsize.Format(L"%d",Shdr.sh_entsize);



	m_List.InsertItem(0,L"节名在字符串表中的索引--sh_name");
	m_List.SetItemText(0,1,sh_name);

	m_List.InsertItem(1,L"类型--------------------sh_type");
	m_List.SetItemText(1,1,sh_type);

	m_List.InsertItem(2,L"属性-------------------sh_flags");
	m_List.SetItemText(2,1,sh_flags);

	m_List.InsertItem(3,L"节在内存中的虚拟地址----sh_addr");
	m_List.SetItemText(3,1,sh_addr);

	m_List.InsertItem(4,L"节的文件偏移----------sh_offset");
	m_List.SetItemText(4,1,sh_offset);

	m_List.InsertItem(5,L"节的大小，单位字节------sh_size");
	m_List.SetItemText(5,1,sh_size);

	m_List.InsertItem(6,L"连接的另外一个节的索引--sh_link");
	m_List.SetItemText(6,1,sh_link);

	m_List.InsertItem(7,L"附加信息----------------sh_info");
	m_List.SetItemText(7,1,sh_info);

	m_List.InsertItem(8,L"对齐-----------------sh_addralign");
	m_List.SetItemText(8,1,sh_addralign);

	m_List.InsertItem(9,L"每个元素大小-----------sh_entsize");
	m_List.SetItemText(9,1,sh_entsize);
}
VOID CElfAnalyseDlg::InsertShdr64(Elf64_Shdr Shdr)
{
	CString sh_name,sh_type,sh_flags,sh_addr,sh_offset,sh_size,sh_link,sh_info,sh_addralign,sh_entsize;
	sh_name.Format(L"%d",Shdr.sh_name);
	sh_type.Format(L"%d",Shdr.sh_type);
	sh_flags.Format(L"%d",Shdr.sh_flags);
	sh_addr.Format(L"0x%x",Shdr.sh_addr);
	sh_offset.Format(L"0x%x",Shdr.sh_offset);
	sh_size.Format(L"0x%x",Shdr.sh_size);
	sh_link.Format(L"0x%x",Shdr.sh_link);
	sh_info.Format(L"0x%x",Shdr.sh_info);
	sh_addralign.Format(L"0x%x",Shdr.sh_addralign);
	sh_entsize.Format(L"%d",Shdr.sh_entsize);



	m_List.InsertItem(0,L"节名在字符串表中的索引--sh_name");
	m_List.SetItemText(0,1,sh_name);

	m_List.InsertItem(1,L"类型--------------------sh_type");
	m_List.SetItemText(1,1,sh_type);

	m_List.InsertItem(2,L"属性-------------------sh_flags");
	m_List.SetItemText(2,1,sh_flags);

	m_List.InsertItem(3,L"节在内存中的虚拟地址----sh_addr");
	m_List.SetItemText(3,1,sh_addr);

	m_List.InsertItem(4,L"节的文件偏移----------sh_offset");
	m_List.SetItemText(4,1,sh_offset);

	m_List.InsertItem(5,L"节的大小，单位字节------sh_size");
	m_List.SetItemText(5,1,sh_size);

	m_List.InsertItem(6,L"连接的另外一个节的索引--sh_link");
	m_List.SetItemText(6,1,sh_link);

	m_List.InsertItem(7,L"附加信息----------------sh_info");
	m_List.SetItemText(7,1,sh_info);

	m_List.InsertItem(8,L"对齐-----------------sh_addralign");
	m_List.SetItemText(8,1,sh_addralign);

	m_List.InsertItem(9,L"每个元素大小-----------sh_entsize");
	m_List.SetItemText(9,1,sh_entsize);
}

VOID CElfAnalyseDlg::InsertRel32(Elf32_Shdr Shdr)
{
	if(!isNoSPARC)
	{
		Elf32_Rel* Rel = (Elf32_Rel*)(g_szFileData + Shdr.sh_offset);


		for(int i = 0;i<Shdr.sh_size/sizeof(Elf32_Rel);i++)
		{
			CString r_offset,r_info;
			r_offset.Format(L"0x%x",Rel->r_offset);
			r_info.Format(L"0x%x",Rel->r_info);

			m_List.InsertItem(i,r_offset);
			m_List.SetItemText(i,1,r_info);

			Rel++;
		}
	}
	else
	{

	}
}
//!!
VOID CElfAnalyseDlg::InsertRel64(Elf64_Shdr Shdr)
{

		Elf64_Rela* Rel = (Elf64_Rela*)(g_szFileData + Shdr.sh_offset);


		for(int i = 0;i<Shdr.sh_size/sizeof(Elf64_Rela);i++)
		{
			CString r_offset,r_info;
			r_offset.Format(L"0x%x",Rel->r_offset);
			r_info.Format(L"0x%x",Rel->r_info);

			m_List.InsertItem(i,r_offset);
			m_List.SetItemText(i,1,r_info);

			Rel++;
		}

}