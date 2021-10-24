
// textDlgMsg.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "textDlgMsg.h"
#include "textDlgMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtextDlgMsgApp

BEGIN_MESSAGE_MAP(CtextDlgMsgApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CtextDlgMsgApp 생성

CtextDlgMsgApp::CtextDlgMsgApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.

	m_system = nullptr;
}


// 유일한 CtextDlgMsgApp 개체입니다.

CtextDlgMsgApp theApp;


// CtextDlgMsgApp 초기화

BOOL CtextDlgMsgApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	m_system = new CmsgThread;


	CtextDlgMsgDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();


	return FALSE;
}



int CtextDlgMsgApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_system != nullptr)
	{
		delete m_system;
		m_system = nullptr;
	}

	return CWinApp::ExitInstance();
}
