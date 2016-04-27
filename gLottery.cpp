
// gLottery.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "gLottery.h"
#include "gLotteryDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CgLotteryApp

BEGIN_MESSAGE_MAP(CgLotteryApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CgLotteryApp ����

CgLotteryApp::CgLotteryApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CgLotteryApp ����

CgLotteryApp theApp;

DWORD g_WaitForSingleObject(HANDLE hHandle)
{
	while (WaitForSingleObject(hHandle, 0) != WAIT_OBJECT_0)
	{
		MSG msg;
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	} // while (WaitForSingleObject(hHandle, 0) != WAIT_OBJECT_0)

	return 0;
};

// CgLotteryApp ��ʼ��

BOOL CgLotteryApp::InitInstance()
{
	m_hMutex = CreateMutex(NULL, TRUE, TEXT("gLottery_2016.4.15"));
	if (GetLastError() == ERROR_ALREADY_EXISTS)
	{
		HWND hwnd = ::FindWindow(NULL, TEXT("gLottery"));
		if (hwnd)
		{
			::SetForegroundWindow(hwnd);
			::SetFocus(hwnd);
			::ShowWindow(hwnd, SW_NORMAL);
		}

		return FALSE;
	}

	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	CoInitialize(NULL);

	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��

	CgLotteryDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

	CoUninitialize();


	ReleaseMutex(m_hMutex);
	CloseHandle(m_hMutex);
	m_hMutex = NULL;

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

