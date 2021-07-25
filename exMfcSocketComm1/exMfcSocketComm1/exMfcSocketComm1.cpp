
// exMfcSocketComm1.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "exMfcSocketComm1.h"
#include "exMfcSocketComm1Dlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexMfcSocketComm1App

BEGIN_MESSAGE_MAP(CexMfcSocketComm1App, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CexMfcSocketComm1App 생성

CexMfcSocketComm1App::CexMfcSocketComm1App()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CexMfcSocketComm1App 개체입니다.

CexMfcSocketComm1App theApp;


// CexMfcSocketComm1App 초기화

BOOL CexMfcSocketComm1App::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	CexMfcSocketComm1Dlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

