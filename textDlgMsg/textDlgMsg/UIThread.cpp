// UIThread.cpp: 구현 파일
//

#include "pch.h"
#include "textDlgMsg.h"
#include "UIThread.h"
#include "CmyDialog.h"


// CUIThread

IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

CUIThread::CUIThread()
{
	m_pMainWnd = nullptr;
}

CUIThread::~CUIThread()
{
	if (m_pMainWnd != nullptr)
	{
		delete m_pMainWnd;
		m_pMainWnd = nullptr;
	}
}

BOOL CUIThread::InitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 초기화를 수행합니다.
	m_pMainWnd = new CmyDialog;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	return TRUE;
}

int CUIThread::ExitInstance()
{
	// TODO:  여기에서 각 스레드에 대한 정리를 수행합니다.
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CUIThread, CWinThread)
END_MESSAGE_MAP()


// CUIThread 메시지 처리기


int CUIThread::Run()
{
	// TODO: 여기에 구현 코드 추가.
	CString str;
	int per = 0;
	for (int a = 1; a < 1001; a++)
	{
		m_pMainWnd->m_progressWT.SetPos(a);
		if (a % 10 == 0)
		{
			per++;
			str.Format(L"%d%%", per);
			m_pMainWnd->m_edit.SetWindowText(str);
		}
	}
	return 0;
}
