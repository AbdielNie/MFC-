
// gLotteryDlg.h : 头文件
//

#pragma once

#include "DXGraph.h"
#include "StdioFileEx/StdioFileEx.h"
#include "UniqueRandom/UniqueRandom.h"
#include <locale.h>
#include <list>
#include "afxwin.h"
using namespace std;

struct gName
{
	gName()
	{
		ZeroMemory(this, sizeof(gName));

		index			= -1;
		bSelected		= FALSE;
		bSelectedTmp	= FALSE;
	};

	int		index;
	TCHAR	tszName[64];
	BOOL	bSelected;

	// 在每一轮的循环中，都做一个记录，否则可能会导致重复抽取
	BOOL	bSelectedTmp;
};

// CgLotteryDlg 对话框
class CgLotteryDlg : public CDialogEx
{
// 构造
public:
	CgLotteryDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_GLOTTERY_DIALOG };
	CStatic m_copyRight;
	CStatic m_playerNum;
	CStatic m_title;
	CStatic m_title_1;

	CStatic m_player_1;
	CStatic m_player_2;
	CStatic m_player_3;
	CStatic m_player_4;
	CStatic m_player_5;
	CStatic m_player_6;
	CStatic m_player_7;
	CStatic m_player_8;

	CStatic m_help;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDestroy();
	virtual BOOL DestroyWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedPlayerNum();
	afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnStnClickedHelp();
	afx_msg LRESULT OnGraphNotify(WPARAM inWParam, LPARAM inLParam);
	DECLARE_MESSAGE_MAP()

public:
	TCHAR			m_tszPath[MAX_PATH];
	TCHAR			m_tszCfgPath[MAX_PATH];
	HBITMAP			m_hBKBtimap;
	CDXGraph*		m_filterGraph;
	BOOL			m_bSpace; // 是否空格
	INT				m_numOfPlayer;

	list<gName *>	m_nameList;

	void			InitCtrl();
	void			DoSthIfSpace();
	void			ShowPlayers();	// 这里显示显示人名的控件
	void			ResetPlayers(); // 重置所有数据，抽奖后的人可以重新再次抽取
	void			SavePlayers();	// 保存信息
	void			StartLottery();
	void			StopLottery();

	BOOL			m_bLottering;	// lottery 是个名字，这里就作为动词使用吧，，，
	HANDLE			m_hLotteryThrd;
	static DWORD WINAPI lotteryThrd(LPVOID lpParam);
	int				Lottery();		// 返回值是实际抽出来的人数

	// Title 的一些信息
	TCHAR			m_tszTitle[MAX_PATH];
	int				m_title_r;
	int				m_title_g;
	int				m_title_b;
	int				m_title_3D;		// 是否立体字
	
	// CopyRight 的字体
	CFont			m_copyRightFont;
	// player num 的字体
	CFont			m_playerNumFont;
	// Title 的字体
	CFont			m_titleFont;

	// 1 个的时候字体
	CFont			m_player1Font;
	// 2 个的时候字体
	CFont			m_player2Font;
	// 3 个的时候字体
	CFont			m_player3Font;
	// 4 个的时候字体
	CFont			m_player4Font;
	// 5 个的时候字体
	CFont			m_player5Font;
	// 6 个的时候字体
	CFont			m_player6Font;
	// 7 个的时候字体
	CFont			m_player7Font;
	// 8 个的时候字体
	CFont			m_player8Font;

	// Help 的字体
	CFont			m_helpFont;
	
	// 随机类
	CUniqueRandom	m_random;

	int				m_realGetNum; // 随机得到的真正的个数
	gName			*m_pName[8];

	void RefreshCtrl(UINT uCtlID)
	{
		CRect rect;
		GetDlgItem(uCtlID)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
	};
};
