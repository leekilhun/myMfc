
// exMfcRs232Buff.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "exMfcRs232Buff.h"
#include "exMfcRs232BuffDlg.h"


// CexMfcRs232BuffApp

BEGIN_MESSAGE_MAP(CexMfcRs232BuffApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CexMfcRs232BuffApp 생성

CexMfcRs232BuffApp::CexMfcRs232BuffApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CexMfcRs232BuffApp 개체입니다.

CexMfcRs232BuffApp theApp;


// CexMfcRs232BuffApp 초기화

BOOL CexMfcRs232BuffApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}



	CexMfcRs232BuffDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.

	return FALSE;
}

