
// exMfcHuskylens.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "exMfcHuskylens.h"
#include "exMfcHuskylensDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexMfcHuskylensApp

BEGIN_MESSAGE_MAP(CexMfcHuskylensApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CexMfcHuskylensApp 생성

CexMfcHuskylensApp::CexMfcHuskylensApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CexMfcHuskylensApp 개체입니다.

CexMfcHuskylensApp theApp;


// CexMfcHuskylensApp 초기화

BOOL CexMfcHuskylensApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}
	


	/*int len = sizeof(send_msg);
	memset(send_msg, 0x00, 1024 * sizeof(CHAR));*/


	/*
	TQueue q;
	q.Write("12345", 6);
	CHAR read[100] = {0,};
	
	*/

	//CHAR send_msg[1024];
	//memset(send_msg, 0x00, sizeof(send_msg));
	//char* dn;
	//dn = "\r";

	//size_t len = strlen(send_msg)+2;
	//strncat_s(send_msg, len, dn, 1);
	


	CexMfcHuskylensDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();


	return FALSE;
}

