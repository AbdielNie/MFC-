
// gLotteryDlg.h : ͷ�ļ�
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

	// ��ÿһ�ֵ�ѭ���У�����һ����¼��������ܻᵼ���ظ���ȡ
	BOOL	bSelectedTmp;
};

// CgLotteryDlg �Ի���
class CgLotteryDlg : public CDialogEx
{
// ����
public:
	CgLotteryDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
	BOOL			m_bSpace; // �Ƿ�ո�
	INT				m_numOfPlayer;

	list<gName *>	m_nameList;

	void			InitCtrl();
	void			DoSthIfSpace();
	void			ShowPlayers();	// ������ʾ��ʾ�����Ŀؼ�
	void			ResetPlayers(); // �����������ݣ��齱����˿��������ٴγ�ȡ
	void			SavePlayers();	// ������Ϣ
	void			StartLottery();
	void			StopLottery();

	BOOL			m_bLottering;	// lottery �Ǹ����֣��������Ϊ����ʹ�ðɣ�����
	HANDLE			m_hLotteryThrd;
	static DWORD WINAPI lotteryThrd(LPVOID lpParam);
	int				Lottery();		// ����ֵ��ʵ�ʳ����������

	// Title ��һЩ��Ϣ
	TCHAR			m_tszTitle[MAX_PATH];
	int				m_title_r;
	int				m_title_g;
	int				m_title_b;
	int				m_title_3D;		// �Ƿ�������
	
	// CopyRight ������
	CFont			m_copyRightFont;
	// player num ������
	CFont			m_playerNumFont;
	// Title ������
	CFont			m_titleFont;

	// 1 ����ʱ������
	CFont			m_player1Font;
	// 2 ����ʱ������
	CFont			m_player2Font;
	// 3 ����ʱ������
	CFont			m_player3Font;
	// 4 ����ʱ������
	CFont			m_player4Font;
	// 5 ����ʱ������
	CFont			m_player5Font;
	// 6 ����ʱ������
	CFont			m_player6Font;
	// 7 ����ʱ������
	CFont			m_player7Font;
	// 8 ����ʱ������
	CFont			m_player8Font;

	// Help ������
	CFont			m_helpFont;
	
	// �����
	CUniqueRandom	m_random;

	int				m_realGetNum; // ����õ��������ĸ���
	gName			*m_pName[8];

	void RefreshCtrl(UINT uCtlID)
	{
		CRect rect;
		GetDlgItem(uCtlID)->GetWindowRect(&rect);
		ScreenToClient(&rect);
		InvalidateRect(&rect);
	};
};
