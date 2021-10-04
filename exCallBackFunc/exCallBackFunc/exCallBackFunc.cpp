﻿
// exCallBackFunc.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "exCallBackFunc.h"
#include "exCallBackFuncDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexCallBackFuncApp

BEGIN_MESSAGE_MAP(CexCallBackFuncApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CexCallBackFuncApp 생성

CexCallBackFuncApp::CexCallBackFuncApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	m_pB = nullptr;
}


// 유일한 CexCallBackFuncApp 개체입니다.

CexCallBackFuncApp theApp;


// CexCallBackFuncApp 초기화

BOOL CexCallBackFuncApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	m_pB = new mb;

	CexCallBackFuncDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}

mb* CexCallBackFuncApp::GetBClass() const
{
	return m_pB;
}



LPUNKNOWN CexCallBackFuncApp::GetInterfaceHook(const void* /*piid*/)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return NULL;
}


int CexCallBackFuncApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_pB != nullptr) { 
		delete m_pB; 
	}
	return CWinApp::ExitInstance();
}
