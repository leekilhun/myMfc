
// exMfcPositon.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "exMfcPositon.h"
#include "exMfcPositonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexMfcPositonApp

BEGIN_MESSAGE_MAP(CexMfcPositonApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CexMfcPositonApp 생성

CexMfcPositonApp::CexMfcPositonApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CexMfcPositonApp 개체입니다.

CexMfcPositonApp theApp;


// CexMfcPositonApp 초기화

BOOL CexMfcPositonApp::InitInstance()
{
	CWinApp::InitInstance();

	CexMfcPositonDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();

	return FALSE;
}

