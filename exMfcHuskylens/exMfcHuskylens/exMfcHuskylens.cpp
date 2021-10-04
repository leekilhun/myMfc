
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


	//const TCHAR s[5] = {'1', '2', '3', '4', '\0'};
	//TCHAR d[100] = {'\0',}; // 문자열로서의 d 는 비어있는 상태임. "" 과 비슷한 상태..
	//_tcscat_s(d, 3*2, s); // 문자열 d 에 문자열 s 를 추가함.


	
	CexMfcHuskylensDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	


	return FALSE;
}

