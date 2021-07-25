
// chatClient.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "chatClient.h"
#include "chatClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchatClientApp

BEGIN_MESSAGE_MAP(CchatClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CchatClientApp 생성

CchatClientApp::CchatClientApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CchatClientApp 개체입니다.

CchatClientApp theApp;


// CchatClientApp 초기화

BOOL CchatClientApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	CchatClientDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	
	return FALSE;
}

