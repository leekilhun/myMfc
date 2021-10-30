
// textDlgMsg.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "textDlgMsg.h"
#include "textDlgMsgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static void TestAdd() {
	AfxMessageBox(L"Test Add \n");
}
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
	m_pTimeScheduler = nullptr;
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

	m_pTimeScheduler = new CtimeScheduler(CtimeScheduler::eTickType::ms_1000);

	//m_pTimeScheduler->AttachCallbackFunc(TestAdd);


	CtextDlgMsgDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		AfxMessageBox(L"메인 Dialog가 예기치 않게 종료 되었습니다.");
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}

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

	if (m_pTimeScheduler != nullptr)
	{
		
		delete m_pTimeScheduler;
		m_pTimeScheduler = nullptr;
	}

	return CWinApp::ExitInstance();
}

void CtextDlgMsgApp::StaticWrapper(void* obj, const CString& name, void* data)
{
	CtextDlgMsgDlg* self = (CtextDlgMsgDlg*)obj;
	CString tmp;
	tmp = name;

	self->SetText(tmp);
}