﻿
// chatServer.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "chatServer.h"
#include "chatServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchatServerApp

BEGIN_MESSAGE_MAP(CchatServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CchatServerApp 생성

CchatServerApp::CchatServerApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CchatServerApp 개체입니다.

CchatServerApp theApp;


// CchatServerApp 초기화

BOOL CchatServerApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	
	CchatServerDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	
	return FALSE;
}
