
// gLottery.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CgLotteryApp:
// �йش����ʵ�֣������ gLottery.cpp
//

class CgLotteryApp : public CWinApp
{
public:
	CgLotteryApp();

	HANDLE m_hMutex;

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CgLotteryApp theApp;