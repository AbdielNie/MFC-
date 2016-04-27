// gLotteryDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "gLottery.h"
#include "gLotteryDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern DWORD g_WaitForSingleObject(HANDLE hHandle);


CgLotteryDlg::CgLotteryDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CgLotteryDlg::IDD, pParent)
{
	m_bSpace		= FALSE;
	m_filterGraph	= NULL;
	m_hLotteryThrd	= NULL;
	m_bLottering	= FALSE;
	m_realGetNum	= 0;
	m_numOfPlayer	= 1;
	for (int i = 0; i < 8; i ++)
	{
		m_pName[i] = NULL;
	}

	m_hIcon			= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgLotteryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COPYRIGHT, m_copyRight);
	DDX_Control(pDX, IDC_PLAYER_NUM, m_playerNum);
	DDX_Control(pDX, IDC_TITLE, m_title);
	DDX_Control(pDX, IDC_TITLE_1, m_title_1);
	DDX_Control(pDX, IDC_PLAYER_1, m_player_1);
	DDX_Control(pDX, IDC_PLAYER_2, m_player_2);
	DDX_Control(pDX, IDC_PLAYER_3, m_player_3);
	DDX_Control(pDX, IDC_PLAYER_4, m_player_4);
	DDX_Control(pDX, IDC_PLAYER_5, m_player_5);
	DDX_Control(pDX, IDC_PLAYER_6, m_player_6);
	DDX_Control(pDX, IDC_PLAYER_7, m_player_7);
	DDX_Control(pDX, IDC_PLAYER_8, m_player_8);
	DDX_Control(pDX, IDC_HELP, m_help);
}

BEGIN_MESSAGE_MAP(CgLotteryDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_GRAPHNOTIFY, OnGraphNotify)
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_PLAYER_NUM, &CgLotteryDlg::OnStnClickedPlayerNum)
	ON_WM_HELPINFO()
	ON_STN_CLICKED(IDC_HELP, &CgLotteryDlg::OnStnClickedHelp)
END_MESSAGE_MAP()


// CgLotteryDlg 消息处理程序

BOOL CgLotteryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetWindowText(TEXT("gLottery"));
	m_bSpace = TRUE;

	InitCtrl();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CgLotteryDlg::OnPaint()
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
HCURSOR CgLotteryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CgLotteryDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

}

BOOL CgLotteryDlg::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	m_bLottering = FALSE;
	if (m_hLotteryThrd)
	{
		g_WaitForSingleObject(m_hLotteryThrd);
		CloseHandle(m_hLotteryThrd);
		m_hLotteryThrd = NULL;
	}

	gName *pName = NULL;
	list<gName *>::iterator it;
	for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
	{
		pName = (gName *)(*it);
		delete pName;
		pName = NULL;
	}

	m_nameList.clear();

	return CDialogEx::DestroyWindow();
}

BOOL CgLotteryDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
	{
		//判断是否按下键盘CTRL + 1键
		if (GetAsyncKeyState(VK_CONTROL) && m_bSpace)
		{
			int preNum = m_numOfPlayer;

			switch (pMsg->wParam)
			{
			case 0x30:
				{
					ResetPlayers();
				}
				break;
			case 0x31:
				{
					m_numOfPlayer = 1;
					m_playerNum.SetWindowText(TEXT("1"));
				}
				break;
			case 0x32:
				{
					m_numOfPlayer = 2;
					m_playerNum.SetWindowText(TEXT("2"));
				}
				break;
			case 0x33:
				{
					m_numOfPlayer = 3;
					m_playerNum.SetWindowText(TEXT("3"));
				}
				break;
			case 0x34:
				{
					m_numOfPlayer = 4;
					m_playerNum.SetWindowText(TEXT("4"));
				}
				break;
			case 0x35:
				{
					m_numOfPlayer = 5;
					m_playerNum.SetWindowText(TEXT("5"));
				}
				break;
			case 0x36:
				{
					m_numOfPlayer = 6;
					m_playerNum.SetWindowText(TEXT("6"));
				}
				break;
			case 0x37:
				{
					m_numOfPlayer = 7;
					m_playerNum.SetWindowText(TEXT("7"));
				}
				break;
			case 0x38:
				{
					m_numOfPlayer = 8;
					m_playerNum.SetWindowText(TEXT("8"));
				}
				break;
			}

			if (pMsg->wParam >= 0x31 &&
				pMsg->wParam <= 0x38 &&
				preNum != m_numOfPlayer)
			{
				RefreshCtrl(m_playerNum.GetDlgCtrlID());

				ShowPlayers();
			}
		}
	}

	if (pMsg->message == WM_KEYDOWN)
	{
		int nVirtKey = (int)pMsg->wParam;
		if (nVirtKey == VK_RETURN)
		{
			//如果是回车在这里什么也不作
			return TRUE;
		}
		else if (nVirtKey == VK_ESCAPE)
		{
			//如果是ESC
			return TRUE;
		}
		else if (nVirtKey == VK_SPACE)
		{
			// 空格
			m_bSpace = !m_bSpace;
			DoSthIfSpace();
		}
		else if (nVirtKey == VK_UP)
		{
		}
		else if (nVirtKey == VK_DOWN)
		{
		}


	} // if (pMsg->message == WM_KEYDOWN)

	return CDialogEx::PreTranslateMessage(pMsg);
}

LRESULT CgLotteryDlg::OnGraphNotify(WPARAM inWParam, LPARAM inLParam)
{
	IMediaEventEx * pEvent = NULL;
	if (m_filterGraph && (pEvent = m_filterGraph->GetEventHandle()))
	{
		LONG   eventCode = 0, eventParam1 = 0, eventParam2 = 0;
		while (SUCCEEDED(pEvent->GetEvent(&eventCode, &eventParam1, &eventParam2, 0)))
		{
			// Spin through the events
			pEvent->FreeEventParams(eventCode, eventParam1, eventParam2);
			switch (eventCode)
			{
			case EC_COMPLETE:
				m_filterGraph->SetCurrentPosition(0);
				break;

			case EC_USERABORT:
			case EC_ERRORABORT:
				break;

			default:
				break;
			}
		}
	}
	return 0;
}

HBRUSH CgLotteryDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == m_title.GetDlgCtrlID()||
		pWnd->GetDlgCtrlID() == m_title_1.GetDlgCtrlID())
	{
		pDC->SetTextColor(RGB(m_title_r, m_title_g, m_title_b));
		// return (HBRUSH)GetStockObject(HOLLOW_BRUSH);
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CgLotteryDlg::OnStnClickedPlayerNum()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!m_bSpace)
	{
		return;
	}

	if (m_numOfPlayer >= 8)
	{
		m_numOfPlayer = 1;
	}
	else
	{
		m_numOfPlayer ++;
	}

	RefreshCtrl(m_playerNum.GetDlgCtrlID());

	CString strNum;
	strNum.Format(TEXT("%d"), m_numOfPlayer);
	m_playerNum.SetWindowText(strNum);

	ShowPlayers();
}

BOOL CgLotteryDlg::OnHelpInfo(HELPINFO* pHelpInfo)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strHelp;
	strHelp.Format(TEXT("\t\t----- ★软件说明★ -----\n\n"));
	strHelp += TEXT("\t1、背景音乐是软件目录下的bk.mp3，可以替换或者删除。\n");
	strHelp += TEXT("\t2、背景图片是软件目录下的bk.bmp（注：24位），可以替换或者删除。\n");
	strHelp += TEXT("\t3、软件目录下的Config.ini文件定义了标题的相关信息，其中R、G、B为字体对应的颜色，3D表示是否为3D字体，Text为标题内容。Text内容可以为空。InitialNum表示默认抽奖人数。\n");
	strHelp += TEXT("\t4、软件目录下的Names.txt中是所有要抽奖人员的名单，一个名字一行。\n");
	strHelp += TEXT("\t5、按空格键抽选或者继续随机，抽取后的人在软件重新启动后不会再次被抽到，可以按Ctrl+0重置数据。重置之前的数据备份在Names.bk.txt中。\n");
	strHelp += TEXT("\t6、最多支持随机抽取8个人。在非随机状态下，按Ctrl+1-8可以定义同时抽取的人的个数，这个个数在软件的左下角显示。也可以在非随机状态下用鼠标点击这个数字。\n");
	strHelp += TEXT("\t7、按Alt+F4关闭程序。\n\n");
	strHelp += TEXT("\t\t\t\t----- AbdielNie-----");
	MessageBox(strHelp, TEXT("使用帮助"), MB_OK);

	return TRUE;
	// return CDialogEx::OnHelpInfo(pHelpInfo);
}

void CgLotteryDlg::OnStnClickedHelp()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strHelp;
	strHelp.Format(TEXT("\t\t----- ★软件说明★ -----\n\n"));
	strHelp += TEXT("\t1、背景音乐是软件目录下的bk.mp3，可以替换或者删除。\n");
	strHelp += TEXT("\t2、背景图片是软件目录下的bk.bmp（注：24位），可以替换或者删除。\n");
	strHelp += TEXT("\t3、软件目录下的Config.ini文件定义了标题的相关信息，其中R、G、B为字体对应的颜色，3D表示是否为3D字体，Text为标题内容。Text内容可以为空。Text内容可以为空。InitialNum表示默认抽奖人数。\n");
	strHelp += TEXT("\t4、软件目录下的Names.txt中是所有要抽奖人员的名单，一个名字一行。\n");
	strHelp += TEXT("\t5、按空格键抽选或者继续随机，抽取后的人在软件重新启动后不会再次被抽到，可以按Ctrl+0重置数据。重置之前的数据备份在Names.bk.txt中。\n");
	strHelp += TEXT("\t6、最多支持随机抽取8个人。在非随机状态下，按Ctrl+1-8可以定义同时抽取的人的个数，这个个数在软件的左下角显示。也可以在非随机状态下用鼠标点击这个数字。\n");
	strHelp += TEXT("\t7、按Alt+F4关闭程序。\n\n");
	MessageBox(strHelp, TEXT("使用帮助"), MB_OK);
}


void CgLotteryDlg::InitCtrl()
{
	TCHAR sFilename[_MAX_PATH];
	TCHAR sDrive[_MAX_DRIVE];
	TCHAR sDir[_MAX_DIR];
	TCHAR sFname[_MAX_FNAME];
	TCHAR sExt[_MAX_EXT];
	GetModuleFileName(NULL, sFilename, _MAX_PATH);
	_tsplitpath(sFilename, sDrive, sDir, sFname, sExt);
	_tcscpy(m_tszPath, sDrive);
	_tcscat(m_tszPath, sDir);
	long nLen = _tcslen(m_tszPath);
	if (m_tszPath[nLen - 1] != TEXT('\\'))
	{
		_tcscat(m_tszPath, TEXT("\\"));
	}

	TCHAR tszBKPath[MAX_PATH];
	StrCpy(tszBKPath, m_tszPath);
	StrCat(tszBKPath, TEXT("BK.bmp"));
	m_hBKBtimap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), tszBKPath,
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

	BITMAP bm;
	GetObject(m_hBKBtimap, sizeof(bm), &bm);
	MoveWindow(0, 0, bm.bmWidth, bm.bmHeight);
	SetBackgroundImage(m_hBKBtimap);

	ModifyStyle(WS_CAPTION, WS_MINIMIZEBOX, SWP_DRAWFRAME);
	CenterWindow();
	MoveWindow(0, 0, GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

	// 读取所有的配置文件
	StrCpy(m_tszCfgPath, m_tszPath);
	StrCat(m_tszCfgPath, TEXT("config.ini"));
	if (GetFileAttributes(m_tszCfgPath) == INVALID_FILE_ATTRIBUTES)
	{
		// 不存在，自动生成吧
		m_title_r = 0;
		m_title_g = 0;
		m_title_b = 255;
		m_title_3D = 1;

		WritePrivateProfileString(TEXT("Title"), TEXT("R"), TEXT("0"), m_tszCfgPath);
		WritePrivateProfileString(TEXT("Title"), TEXT("G"), TEXT("0"), m_tszCfgPath);
		WritePrivateProfileString(TEXT("Title"), TEXT("B"), TEXT("255"), m_tszCfgPath);
		WritePrivateProfileString(TEXT("Title"), TEXT("3D"), TEXT("1"), m_tszCfgPath);

		StrCpy(m_tszTitle, TEXT("抽奖系统"));
		WritePrivateProfileString(TEXT("Title"), TEXT("Text"), m_tszTitle, m_tszCfgPath);

		m_numOfPlayer = 1;
		WritePrivateProfileString(TEXT("InitialNum"), TEXT("Value"), TEXT("1"), m_tszCfgPath);
	}
	else
	{
		// 读取
		m_title_r = GetPrivateProfileInt(TEXT("Title"), TEXT("R"), 0, m_tszCfgPath);
		m_title_g = GetPrivateProfileInt(TEXT("Title"), TEXT("G"), 0, m_tszCfgPath);
		m_title_b = GetPrivateProfileInt(TEXT("Title"), TEXT("B"), 255, m_tszCfgPath);
		m_title_3D = GetPrivateProfileInt(TEXT("Title"), TEXT("3D"), 1, m_tszCfgPath);
		GetPrivateProfileString(TEXT("Title"), TEXT("Text"), TEXT("抽奖系统"), m_tszTitle, MAX_PATH, m_tszCfgPath);

		m_numOfPlayer = GetPrivateProfileInt(TEXT("InitialNum"), TEXT("Value"), 1, m_tszCfgPath);
	}

	// 读取所有的人名信息
	CString strNamePath;
	strNamePath.Format(TEXT("%sNames.txt"), m_tszPath);
	CStdioFileEx  fileEx;
	CString strLine;
	int index = 0;
	if (fileEx.Open(strNamePath, CFile::modeRead | CFile::typeText))
	{
		while (fileEx.ReadString(strLine))
		{
			if (strLine.IsEmpty())
			{
				continue;
			}

			gName *pName		= new gName;
			pName->index		= index;
			pName->bSelected	= FALSE;

			int delimiterPos = strLine.Find(TEXT('|'));
			if (delimiterPos >= 0)
			{
				StrCpy(pName->tszName, strLine.Left(delimiterPos));
				pName->bSelected	= TRUE;
			}
			else
			{
				StrCpy(pName->tszName, strLine);
			}

			index ++;

			m_nameList.push_back(pName);
		}
	}

	m_filterGraph = new CDXGraph();
	if (m_filterGraph->Create())
	{
		TCHAR tszMusicPath[MAX_PATH];

		StrCpy(tszMusicPath, m_tszPath);
		StrCat(tszMusicPath, TEXT("BK.mp3"));

		m_filterGraph->RenderFile(tszMusicPath);
		m_filterGraph->SetNotifyWindow(GetSafeHwnd());

		m_filterGraph->Run();
		m_filterGraph->SetAudioVolume(0);
	}

	// 版权
	CString strFont;
	RECT rect;

	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));

	lf.lfHeight				= 38;
	lf.lfWeight				= FW_HEAVY;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("Segoe Script"));
	VERIFY(m_copyRightFont.CreateFontIndirect(&lf));
	m_copyRight.SetFont(&m_copyRightFont);
	m_copyRight.SetWindowText(TEXT("Made by AbdielNie"));
	m_copyRight.GetWindowRect(&rect);
	m_copyRight.MoveWindow(	GetSystemMetrics(SM_CXSCREEN) - rect.right	+ rect.left,
		GetSystemMetrics(SM_CYSCREEN) - rect.bottom	+ rect.top,
		rect.right - rect.left, rect.bottom - rect.top);


	CString strNum;
	strNum.Format(TEXT("%d"), m_numOfPlayer);
	lf.lfHeight				= 38;
	lf.lfWeight				= FW_HEAVY;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("Segoe Script"));
	VERIFY(m_playerNumFont.CreateFontIndirect(&lf));
	m_playerNum.SetFont(&m_playerNumFont);
	m_playerNum.SetWindowText(strNum);
	m_playerNum.GetWindowRect(&rect);
	m_playerNum.MoveWindow(0, GetSystemMetrics(SM_CYSCREEN) - rect.bottom	+ rect.top,
		rect.right - rect.left, rect.bottom - rect.top);

	lf.lfHeight				= 80;
	lf.lfWeight				= FW_HEAVY;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("黑体"));
	VERIFY(m_titleFont.CreateFontIndirect(&lf));
	m_title.SetFont(&m_titleFont);
	m_title.SetWindowText(m_tszTitle);
	m_title.GetWindowRect(&rect);
	m_title.MoveWindow(0, 40, GetSystemMetrics(SM_CXSCREEN), rect.bottom - rect.top);

	if (m_title_3D == 1)
	{
		m_title_1.SetFont(&m_titleFont);
		m_title_1.SetWindowText(m_tszTitle);
		m_title_1.GetWindowRect(&rect);
		m_title_1.MoveWindow(2, 42, GetSystemMetrics(SM_CXSCREEN), rect.bottom - rect.top);
	}
	else
	{
		m_title_1.ShowWindow(SW_HIDE);
	}

	// 1 个 字体
	lf.lfHeight				= 64;
	lf.lfWeight				= 900;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("楷体"));
	VERIFY(m_player1Font.CreateFontIndirect(&lf));

	// 2 个 字体
	lf.lfHeight				= 64;
	lf.lfWeight				= 900;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("楷体"));
	VERIFY(m_player2Font.CreateFontIndirect(&lf));

	// 3 个 字体
	lf.lfHeight				= 64;
	lf.lfWeight				= 900;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("楷体"));
	VERIFY(m_player3Font.CreateFontIndirect(&lf));

	// 4 个 字体
	lf.lfHeight				= 64;
	lf.lfWeight				= 900;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("楷体"));
	VERIFY(m_player4Font.CreateFontIndirect(&lf));

	// 5 个 字体
	lf.lfHeight				= 64;
	lf.lfWeight				= 900;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("楷体"));
	VERIFY(m_player5Font.CreateFontIndirect(&lf));

	// 6 个 字体
	lf.lfHeight				= 64;
	lf.lfWeight				= 900;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("楷体"));
	VERIFY(m_player6Font.CreateFontIndirect(&lf));

	// 7 个 字体
	lf.lfHeight				= 64;
	lf.lfWeight				= 900;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("楷体"));
	VERIFY(m_player7Font.CreateFontIndirect(&lf));

	// 8 个 字体
	lf.lfHeight				= 64;
	lf.lfWeight				= 900;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("楷体"));
	VERIFY(m_player8Font.CreateFontIndirect(&lf));

	// Help
	lf.lfHeight				= 30;
	lf.lfWeight				= FW_HEAVY;
	lf.lfQuality			= CLEARTYPE_QUALITY;
	lf.lfPitchAndFamily		= FF_DONTCARE;
	StrCpy(lf.lfFaceName, TEXT("Segoe Script"));
	VERIFY(m_helpFont.CreateFontIndirect(&lf));
	m_help.SetFont(&m_helpFont);
	m_help.GetWindowRect(&rect);
	m_help.SetWindowText(TEXT("HELP"));
	m_help.MoveWindow(GetSystemMetrics(SM_CXSCREEN) - rect.right + rect.left - 5, 5,
					rect.right - rect.left, rect.bottom - rect.top);

	RefreshCtrl(m_player_1.GetDlgCtrlID());
	RefreshCtrl(m_player_2.GetDlgCtrlID());
	RefreshCtrl(m_player_3.GetDlgCtrlID());
	RefreshCtrl(m_player_4.GetDlgCtrlID());
	RefreshCtrl(m_player_5.GetDlgCtrlID());
	RefreshCtrl(m_player_6.GetDlgCtrlID());
	RefreshCtrl(m_player_7.GetDlgCtrlID());
	RefreshCtrl(m_player_8.GetDlgCtrlID());
	m_player_1.SetWindowText(TEXT(""));
	m_player_2.SetWindowText(TEXT(""));
	m_player_3.SetWindowText(TEXT(""));
	m_player_4.SetWindowText(TEXT(""));
	m_player_5.SetWindowText(TEXT(""));
	m_player_6.SetWindowText(TEXT(""));
	m_player_7.SetWindowText(TEXT(""));
	m_player_8.SetWindowText(TEXT(""));
	ShowPlayers();

	if (!m_bSpace)
	{
		StartLottery();
	}

}

void CgLotteryDlg::DoSthIfSpace()
{
	if (m_bSpace)
	{
		if (m_filterGraph)
		{
			m_filterGraph->Pause();
		}

		// 停止刷新窗体上的名字
		StopLottery();

		for (int i = 0; i < m_realGetNum; i ++)
		{
			if (m_pName[i] != NULL)
			{
				m_pName[i]->bSelected = TRUE;
			}
		}

		SavePlayers();
	}
	else
	{
		if (m_filterGraph)
		{
			m_filterGraph->Run();
		}

		// 刷新窗体上的名字
		StartLottery();
	}
}

void CgLotteryDlg::ShowPlayers()
{
	m_player_1.ShowWindow(SW_HIDE);
	m_player_2.ShowWindow(SW_HIDE);
	m_player_3.ShowWindow(SW_HIDE);
	m_player_4.ShowWindow(SW_HIDE);
	m_player_5.ShowWindow(SW_HIDE);
	m_player_6.ShowWindow(SW_HIDE);
	m_player_7.ShowWindow(SW_HIDE);
	m_player_8.ShowWindow(SW_HIDE);

	switch (m_numOfPlayer)
	{
	case 1:
		{
			RECT rect;
			rect.left		= 0;
			rect.right		= GetSystemMetrics(SM_CXSCREEN);
			rect.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 35;
			rect.bottom		= GetSystemMetrics(SM_CYSCREEN) / 2 + 35;
			m_player_1.MoveWindow(&rect);
			m_player_1.SetFont(&m_player1Font);
			//m_player_1.SetWindowText(TEXT(""));

			m_player_1.ShowWindow(SW_SHOW);
		}
		break;
	case 2:
		{
			RECT rect1;
			rect1.left		= 200;
			rect1.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect1.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 35;
			rect1.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 35;
			m_player_1.MoveWindow(&rect1);
			m_player_1.SetFont(&m_player2Font);
			//m_player_1.SetWindowText(TEXT(""));

			RECT rect2;
			rect2.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect2.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect2.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 35;
			rect2.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 35;
			m_player_2.MoveWindow(&rect2);
			m_player_2.SetFont(&m_player2Font);
			//m_player_2.SetWindowText(TEXT(""));

			m_player_1.ShowWindow(SW_SHOW);
			m_player_2.ShowWindow(SW_SHOW);
		}
		break;
	case 3:
		{
			RECT rect1;
			rect1.left		= 0;
			rect1.right		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect1.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 35;
			rect1.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 35;
			m_player_1.MoveWindow(&rect1);
			m_player_1.SetFont(&m_player3Font);
			//m_player_1.SetWindowText(TEXT(""));

			RECT rect2;
			rect2.left		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect2.right		= GetSystemMetrics(SM_CXSCREEN) / 3 * 2;
			rect2.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 35;
			rect2.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 35;
			m_player_2.MoveWindow(&rect2);
			m_player_2.SetFont(&m_player3Font);
			//m_player_2.SetWindowText(TEXT(""));

			RECT rect3;
			rect3.left		= GetSystemMetrics(SM_CXSCREEN) / 3 * 2;
			rect3.right		= GetSystemMetrics(SM_CXSCREEN);
			rect3.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 35;
			rect3.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 35;
			m_player_3.MoveWindow(&rect3);
			m_player_3.SetFont(&m_player3Font);
			//m_player_3.SetWindowText(TEXT(""));

			m_player_1.ShowWindow(SW_SHOW);
			m_player_2.ShowWindow(SW_SHOW);
			m_player_3.ShowWindow(SW_SHOW);
		}
		break;
	case 4:
		{
			RECT rect1;
			rect1.left		= 200;
			rect1.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect1.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect1.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_1.MoveWindow(&rect1);
			m_player_1.SetFont(&m_player4Font);
			//m_player_1.SetWindowText(TEXT(""));

			RECT rect2;
			rect2.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect2.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect2.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect2.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_2.MoveWindow(&rect2);
			m_player_2.SetFont(&m_player4Font);
			//m_player_2.SetWindowText(TEXT(""));

			RECT rect3;
			rect3.left		= 200;
			rect3.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect3.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect3.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_3.MoveWindow(&rect3);
			m_player_3.SetFont(&m_player4Font);
			//m_player_3.SetWindowText(TEXT(""));

			RECT rect4;
			rect4.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect4.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect4.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect4.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_4.MoveWindow(&rect4);
			m_player_4.SetFont(&m_player4Font);
			//m_player_4.SetWindowText(TEXT(""));

			m_player_1.ShowWindow(SW_SHOW);
			m_player_2.ShowWindow(SW_SHOW);
			m_player_3.ShowWindow(SW_SHOW);
			m_player_4.ShowWindow(SW_SHOW);
		}
		break;
	case 5:
		{
			RECT rect1;
			rect1.left		= 0;
			rect1.right		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect1.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect1.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_1.MoveWindow(&rect1);
			m_player_1.SetFont(&m_player5Font);
			//m_player_1.SetWindowText(TEXT(""));

			RECT rect2;
			rect2.left		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect2.right		= GetSystemMetrics(SM_CXSCREEN) / 3 * 2;
			rect2.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect2.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_2.MoveWindow(&rect2);
			m_player_2.SetFont(&m_player5Font);
			//m_player_2.SetWindowText(TEXT(""));

			RECT rect3;
			rect3.left		= GetSystemMetrics(SM_CXSCREEN) / 3 * 2;
			rect3.right		= GetSystemMetrics(SM_CXSCREEN);
			rect3.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect3.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_3.MoveWindow(&rect3);
			m_player_3.SetFont(&m_player5Font);
			//m_player_3.SetWindowText(TEXT(""));

			RECT rect4;
			rect4.left		= 0;
			rect4.right		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect4.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect4.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_4.MoveWindow(&rect4);
			m_player_4.SetFont(&m_player5Font);
			//m_player_4.SetWindowText(TEXT(""));

			RECT rect5;
			rect5.left		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect5.right		= GetSystemMetrics(SM_CXSCREEN) / 3 * 2;
			rect5.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect5.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_5.MoveWindow(&rect5);
			m_player_5.SetFont(&m_player5Font);
			//m_player_5.SetWindowText(TEXT(""));

			m_player_1.ShowWindow(SW_SHOW);
			m_player_2.ShowWindow(SW_SHOW);
			m_player_3.ShowWindow(SW_SHOW);
			m_player_4.ShowWindow(SW_SHOW);
			m_player_5.ShowWindow(SW_SHOW);
		}
		break;
	case 6:
		{
			RECT rect1;
			rect1.left		= 0;
			rect1.right		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect1.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect1.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_1.MoveWindow(&rect1);
			m_player_1.SetFont(&m_player6Font);
			//m_player_1.SetWindowText(TEXT(""));

			RECT rect2;
			rect2.left		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect2.right		= GetSystemMetrics(SM_CXSCREEN) / 3 * 2;
			rect2.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect2.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_2.MoveWindow(&rect2);
			m_player_2.SetFont(&m_player6Font);
			//m_player_2.SetWindowText(TEXT(""));

			RECT rect3;
			rect3.left		= GetSystemMetrics(SM_CXSCREEN) / 3 * 2;
			rect3.right		= GetSystemMetrics(SM_CXSCREEN);
			rect3.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect3.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_3.MoveWindow(&rect3);
			m_player_3.SetFont(&m_player6Font);
			//m_player_3.SetWindowText(TEXT(""));

			RECT rect4;
			rect4.left		= 0;
			rect4.right		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect4.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect4.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_4.MoveWindow(&rect4);
			m_player_4.SetFont(&m_player6Font);
			//m_player_4.SetWindowText(TEXT(""));

			RECT rect5;
			rect5.left		= GetSystemMetrics(SM_CXSCREEN) / 3;
			rect5.right		= GetSystemMetrics(SM_CXSCREEN) / 3 * 2;
			rect5.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect5.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_5.MoveWindow(&rect5);
			m_player_5.SetFont(&m_player6Font);
			//m_player_5.SetWindowText(TEXT(""));

			RECT rect6;
			rect6.left		= GetSystemMetrics(SM_CXSCREEN) / 3 * 2;
			rect6.right		= GetSystemMetrics(SM_CXSCREEN);
			rect6.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect6.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_6.MoveWindow(&rect6);
			m_player_6.SetFont(&m_player6Font);
			//m_player_6.SetWindowText(TEXT(""));

			m_player_1.ShowWindow(SW_SHOW);
			m_player_2.ShowWindow(SW_SHOW);
			m_player_3.ShowWindow(SW_SHOW);
			m_player_4.ShowWindow(SW_SHOW);
			m_player_5.ShowWindow(SW_SHOW);
			m_player_6.ShowWindow(SW_SHOW);
		}
		break;
	case 7:
		{
			RECT rect1;
			rect1.left		= 200;
			rect1.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect1.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 140;
			rect1.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_1.MoveWindow(&rect1);
			m_player_1.SetFont(&m_player7Font);
			//m_player_1.SetWindowText(TEXT(""));

			RECT rect2;
			rect2.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect2.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect2.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 140;
			rect2.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_2.MoveWindow(&rect2);
			m_player_2.SetFont(&m_player7Font);
			//m_player_2.SetWindowText(TEXT(""));

			RECT rect3;
			rect3.left		= 200;
			rect3.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect3.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect3.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_3.MoveWindow(&rect3);
			m_player_3.SetFont(&m_player7Font);
			//m_player_3.SetWindowText(TEXT(""));

			RECT rect4;
			rect4.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect4.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect4.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect4.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_4.MoveWindow(&rect4);
			m_player_4.SetFont(&m_player7Font);
			//m_player_4.SetWindowText(TEXT(""));

			RECT rect5;
			rect5.left		= 200;
			rect5.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect5.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect5.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_5.MoveWindow(&rect5);
			m_player_5.SetFont(&m_player7Font);
			//m_player_5.SetWindowText(TEXT(""));

			RECT rect6;
			rect6.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect6.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect6.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect6.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_6.MoveWindow(&rect6);
			m_player_6.SetFont(&m_player7Font);
			//m_player_6.SetWindowText(TEXT(""));

			RECT rect7;
			rect7.left		= 200;
			rect7.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect7.top		= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			rect7.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 140;
			m_player_7.MoveWindow(&rect7);
			m_player_7.SetFont(&m_player7Font);
			//m_player_7.SetWindowText(TEXT(""));

			m_player_1.ShowWindow(SW_SHOW);
			m_player_2.ShowWindow(SW_SHOW);
			m_player_3.ShowWindow(SW_SHOW);
			m_player_4.ShowWindow(SW_SHOW);
			m_player_5.ShowWindow(SW_SHOW);
			m_player_6.ShowWindow(SW_SHOW);
			m_player_7.ShowWindow(SW_SHOW);
		}
		break;
	case 8:
		{
			RECT rect1;
			rect1.left		= 200;
			rect1.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect1.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 140;
			rect1.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_1.MoveWindow(&rect1);
			m_player_1.SetFont(&m_player8Font);
			//m_player_1.SetWindowText(TEXT(""));

			RECT rect2;
			rect2.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect2.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect2.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 140;
			rect2.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_2.MoveWindow(&rect2);
			m_player_2.SetFont(&m_player8Font);
			//m_player_2.SetWindowText(TEXT(""));

			RECT rect3;
			rect3.left		= 200;
			rect3.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect3.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect3.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_3.MoveWindow(&rect3);
			m_player_3.SetFont(&m_player8Font);
			//m_player_3.SetWindowText(TEXT(""));

			RECT rect4;
			rect4.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect4.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect4.top		= GetSystemMetrics(SM_CYSCREEN) / 2 - 70;
			rect4.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2;
			m_player_4.MoveWindow(&rect4);
			m_player_4.SetFont(&m_player8Font);
			//m_player_4.SetWindowText(TEXT(""));

			RECT rect5;
			rect5.left		= 200;
			rect5.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect5.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect5.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_5.MoveWindow(&rect5);
			m_player_5.SetFont(&m_player8Font);
			//m_player_5.SetWindowText(TEXT(""));

			RECT rect6;
			rect6.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect6.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect6.top		= GetSystemMetrics(SM_CYSCREEN) / 2;
			rect6.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			m_player_6.MoveWindow(&rect6);
			m_player_6.SetFont(&m_player8Font);
			//m_player_6.SetWindowText(TEXT(""));

			RECT rect7;
			rect7.left		= 200;
			rect7.right		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect7.top		= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			rect7.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 140;
			m_player_7.MoveWindow(&rect7);
			m_player_7.SetFont(&m_player8Font);
			//m_player_7.SetWindowText(TEXT(""));

			RECT rect8;
			rect8.left		= GetSystemMetrics(SM_CXSCREEN) / 2;
			rect8.right		= GetSystemMetrics(SM_CXSCREEN) - 200;
			rect8.top		= GetSystemMetrics(SM_CYSCREEN) / 2 + 70;
			rect8.bottom	= GetSystemMetrics(SM_CYSCREEN) / 2 + 140;
			m_player_8.MoveWindow(&rect8);
			m_player_8.SetFont(&m_player8Font);
			//m_player_8.SetWindowText(TEXT(""));

			m_player_1.ShowWindow(SW_SHOW);
			m_player_2.ShowWindow(SW_SHOW);
			m_player_3.ShowWindow(SW_SHOW);
			m_player_4.ShowWindow(SW_SHOW);
			m_player_5.ShowWindow(SW_SHOW);
			m_player_6.ShowWindow(SW_SHOW);
			m_player_7.ShowWindow(SW_SHOW);
			m_player_8.ShowWindow(SW_SHOW);
		}
		break;
	}
}

void CgLotteryDlg::ResetPlayers()
{
	if (MessageBox(TEXT("确认重置所有数据吗？"), TEXT("确定"), MB_OKCANCEL) != IDOK)
	{
		return;
	}

	gName *pName = NULL;
	list<gName *>::iterator it;
	for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
	{
		pName = (gName *)(*it);
		pName->bSelected = FALSE;
	}

	// 保存文件中
	SavePlayers();

	RefreshCtrl(m_player_1.GetDlgCtrlID());
	RefreshCtrl(m_player_2.GetDlgCtrlID());
	RefreshCtrl(m_player_3.GetDlgCtrlID());
	RefreshCtrl(m_player_4.GetDlgCtrlID());
	RefreshCtrl(m_player_5.GetDlgCtrlID());
	RefreshCtrl(m_player_6.GetDlgCtrlID());
	RefreshCtrl(m_player_7.GetDlgCtrlID());
	RefreshCtrl(m_player_8.GetDlgCtrlID());

	m_player_1.SetWindowText(TEXT(""));
	m_player_2.SetWindowText(TEXT(""));
	m_player_3.SetWindowText(TEXT(""));
	m_player_4.SetWindowText(TEXT(""));
	m_player_5.SetWindowText(TEXT(""));
	m_player_6.SetWindowText(TEXT(""));
	m_player_7.SetWindowText(TEXT(""));
	m_player_8.SetWindowText(TEXT(""));

	for (int i = 0; i < 8; i ++)
	{
		m_pName[i] = NULL;
	}

	MessageBox(TEXT("数据已重置。"), TEXT("消息"), MB_OK);
}

void CgLotteryDlg::SavePlayers()
{
	// 读取所有的人名信息
	CString strNamePath;
	strNamePath.Format(TEXT("%sNames.txt"), m_tszPath);

	// 备份
	CString strNamePath_BK;
	strNamePath_BK.Format(TEXT("%sNames_bk.txt"), m_tszPath);
	CopyFile(strNamePath, strNamePath_BK, FALSE);

	CStdioFileEx file;
	CString strLine;
	int index = 0;
	if (file.Open(strNamePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{
		gName *pName = NULL;
		list<gName *>::iterator it;
		for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
		{
			pName = (gName *)(*it);
			strLine.Format(TEXT("%s"), pName->tszName);
			if (pName->bSelected)
			{
				strLine += TEXT("|1");
			}

			file.WriteString(strLine);
			file.WriteString(TEXT("\r\n"));
		}
	}

	file.Flush();
}

void CgLotteryDlg::StartLottery()
{
	if (m_hLotteryThrd)
	{
		m_bLottering = FALSE;
		g_WaitForSingleObject(m_hLotteryThrd);
		CloseHandle(m_hLotteryThrd);
		m_hLotteryThrd = NULL;
	}

	m_bLottering = TRUE;
	m_hLotteryThrd = CreateThread(NULL, 0, lotteryThrd, this, 0, NULL);
}

void CgLotteryDlg::StopLottery()
{
	m_bLottering = FALSE;
	if (m_hLotteryThrd)
	{
		g_WaitForSingleObject(m_hLotteryThrd);
		CloseHandle(m_hLotteryThrd);
		m_hLotteryThrd = NULL;
	}
}

DWORD WINAPI CgLotteryDlg::lotteryThrd(LPVOID lpParam)
{
	CgLotteryDlg *pDlg = (CgLotteryDlg *)lpParam;

	while (pDlg->m_bLottering)
	{
		Sleep(10);

		// 同时抽出来这几个
		pDlg->m_numOfPlayer;

		// 实际抽出来的个数
		pDlg->m_realGetNum = pDlg->Lottery();
		if (pDlg->m_realGetNum < pDlg->m_numOfPlayer)
		{
			// 所有的数据都随机完了
			pDlg->m_bLottering = FALSE;
		}
	}

	return 0;
}

int CgLotteryDlg::Lottery()
{
	int tmp = m_numOfPlayer;
	int retNum = 0;

	int listLastNum = 0;
	gName *pName = NULL;
	list<gName *>::iterator it;
	for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
	{
		pName = (gName *)(*it);
		if (pName->bSelected == FALSE)
		{
			listLastNum ++;
		}

		pName = NULL;
	}

	m_player_1.SetWindowText(TEXT(""));
	m_player_2.SetWindowText(TEXT(""));
	m_player_3.SetWindowText(TEXT(""));
	m_player_4.SetWindowText(TEXT(""));
	m_player_5.SetWindowText(TEXT(""));
	m_player_6.SetWindowText(TEXT(""));
	m_player_7.SetWindowText(TEXT(""));
	m_player_8.SetWindowText(TEXT(""));

	RefreshCtrl(m_player_1.GetDlgCtrlID());
	RefreshCtrl(m_player_2.GetDlgCtrlID());
	RefreshCtrl(m_player_3.GetDlgCtrlID());
	RefreshCtrl(m_player_4.GetDlgCtrlID());
	RefreshCtrl(m_player_5.GetDlgCtrlID());
	RefreshCtrl(m_player_6.GetDlgCtrlID());
	RefreshCtrl(m_player_7.GetDlgCtrlID());
	RefreshCtrl(m_player_8.GetDlgCtrlID());

	for (int i = 0; i < 8; i ++)
	{
		m_pName[i] = NULL;
	}

	do
	{
		// 1
		if (listLastNum == 0)
		{
			break;
		}

		int index = m_random.random() % listLastNum;
		for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
		{
			pName = (gName *)(*it);
			if (pName->bSelected == FALSE && pName->bSelectedTmp == FALSE)
			{
				if (index == 0)
				{
					break;
				}

				index --;
			}

			pName = NULL;
		}

		if (pName == NULL)
		{
			break;
		}

		retNum ++;
		pName->bSelectedTmp = TRUE;
		m_pName[0] = pName;
		m_player_1.SetWindowText(pName->tszName);
		tmp --;
		listLastNum --;
		if (tmp == 0)
		{
			break;
		}

		// 2
		if (listLastNum == 0)
		{
			break;
		}

		index = m_random.random() % listLastNum;
		for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
		{
			pName = (gName *)(*it);
			if (pName->bSelected == FALSE && pName->bSelectedTmp == FALSE)
			{
				if (index == 0)
				{
					break;
				}

				index --;
			}

			pName = NULL;
		}

		if (pName == NULL)
		{
			break;
		}

		retNum ++;
		pName->bSelectedTmp = TRUE;
		m_pName[1] = pName;
		m_player_2.SetWindowText(pName->tszName);
		tmp --;
		listLastNum --;
		if (tmp == 0)
		{
			break;
		}

		// 3
		if (listLastNum == 0)
		{
			break;
		}

		index = m_random.random() % listLastNum;
		for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
		{
			pName = (gName *)(*it);
			if (pName->bSelected == FALSE && pName->bSelectedTmp == FALSE)
			{
				if (index == 0)
				{
					break;
				}

				index --;
			}

			pName = NULL;
		}

		if (pName == NULL)
		{
			break;
		}

		retNum ++;
		pName->bSelectedTmp = TRUE;
		m_pName[2] = pName;
		m_player_3.SetWindowText(pName->tszName);
		tmp --;
		listLastNum --;
		if (tmp == 0)
		{
			break;
		}

		// 4
		if (listLastNum == 0)
		{
			break;
		}

		index = m_random.random() % listLastNum;
		for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
		{
			pName = (gName *)(*it);
			if (pName->bSelected == FALSE && pName->bSelectedTmp == FALSE)
			{
				if (index == 0)
				{
					break;
				}

				index --;
			}

			pName = NULL;
		}

		if (pName == NULL)
		{
			break;
		}

		retNum ++;
		pName->bSelectedTmp = TRUE;
		m_pName[3] = pName;
		m_player_4.SetWindowText(pName->tszName);
		tmp --;
		listLastNum --;
		if (tmp == 0)
		{
			break;
		}

		// 5
		if (listLastNum == 0)
		{
			break;
		}

		index = m_random.random() % listLastNum;
		for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
		{
			pName = (gName *)(*it);
			if (pName->bSelected == FALSE && pName->bSelectedTmp == FALSE)
			{
				if (index == 0)
				{
					break;
				}

				index --;
			}

			pName = NULL;
		}

		if (pName == NULL)
		{
			break;
		}

		retNum ++;
		pName->bSelectedTmp = TRUE;
		m_pName[4] = pName;
		m_player_5.SetWindowText(pName->tszName);
		tmp --;
		listLastNum --;
		if (tmp == 0)
		{
			break;
		}

		// 6
		if (listLastNum == 0)
		{
			break;
		}

		index = m_random.random() % listLastNum;
		for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
		{
			pName = (gName *)(*it);
			if (pName->bSelected == FALSE && pName->bSelectedTmp == FALSE)
			{
				if (index == 0)
				{
					break;
				}

				index --;
			}

			pName = NULL;
		}

		if (pName == NULL)
		{
			break;
		}

		retNum ++;
		pName->bSelectedTmp = TRUE;
		m_pName[5] = pName;
		m_player_6.SetWindowText(pName->tszName);
		tmp --;
		listLastNum --;
		if (tmp == 0)
		{
			break;
		}


		// 7
		if (listLastNum == 0)
		{
			break;
		}

		index = m_random.random() % listLastNum;
		for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
		{
			pName = (gName *)(*it);
			if (pName->bSelected == FALSE && pName->bSelectedTmp == FALSE)
			{
				if (index == 0)
				{
					break;
				}

				index --;
			}

			pName = NULL;
		}

		if (pName == NULL)
		{
			break;
		}

		retNum ++;
		pName->bSelectedTmp = TRUE;
		m_pName[6] = pName;
		m_player_7.SetWindowText(pName->tszName);
		tmp --;
		listLastNum --;
		if (tmp == 0)
		{
			break;
		}

		// 8
		if (listLastNum == 0)
		{
			break;
		}

		index = m_random.random() % listLastNum;
		for (it = m_nameList.begin(); it != m_nameList.end(); it ++)
		{
			pName = (gName *)(*it);
			if (pName->bSelected == FALSE && pName->bSelectedTmp == FALSE)
			{
				if (index == 0)
				{
					break;
				}

				index --;
			}

			pName = NULL;
		}

		if (pName == NULL)
		{
			break;
		}

		retNum ++;
		pName->bSelectedTmp = TRUE;
		m_pName[7] = pName;
		m_player_8.SetWindowText(pName->tszName);
		tmp --;
		listLastNum --;
		if (tmp == 0)
		{
			break;
		}

	} while (FALSE);

	for (int i = 0; i < retNum; i ++)
	{
		m_pName[i]->bSelectedTmp = FALSE;
	}

	return retNum;
}

