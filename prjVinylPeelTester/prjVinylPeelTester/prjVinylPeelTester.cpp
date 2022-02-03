
// prjVinylPeelTester.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "prjVinylPeelTester.h"
#include "prjVinylPeelTesterDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CprjVinylPeelTesterApp

BEGIN_MESSAGE_MAP(CprjVinylPeelTesterApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


void test_timer()
{

	long sum = 0;
	timer::_us timer;
	timer.Start();
	chrono::system_clock::time_point StartTime = chrono::system_clock::now();
	for (int i = 0; i < 10000000; i++) {
		sum += i;
	}

	chrono::system_clock::time_point EndTime = chrono::system_clock::now();
	uint64_t ret = timer.Elaps();

	chrono::duration<double> DefaultSec = EndTime - StartTime;
	chrono::nanoseconds nano = EndTime - StartTime;
	chrono::microseconds micro = chrono::duration_cast<chrono::microseconds>(EndTime - StartTime);
	chrono::milliseconds mill = chrono::duration_cast<chrono::milliseconds>(EndTime - StartTime);
	chrono::seconds sec = chrono::duration_cast<chrono::seconds>(EndTime - StartTime);
	chrono::minutes min = chrono::duration_cast<chrono::minutes>(EndTime - StartTime);
	chrono::hours hour = chrono::duration_cast<chrono::hours>(EndTime - StartTime);

	double result = static_cast<double>(DefaultSec.count()) ;
	result = static_cast<double>(nano.count()) ;
	result = static_cast<double>(micro.count()) ;
	result = static_cast<double>(mill.count()) ;
	result = static_cast<double>(sec.count()) ;
	result = static_cast<double>(min.count()) ;
	result = static_cast<double>(hour.count()) ;


}


// CprjVinylPeelTesterApp 생성

CprjVinylPeelTesterApp::CprjVinylPeelTesterApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	m_system = nullptr;
}


// 유일한 CprjVinylPeelTesterApp 개체입니다.

CprjVinylPeelTesterApp theApp;


// CprjVinylPeelTesterApp 초기화

BOOL CprjVinylPeelTesterApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	//Test

	//test_timer();

















	m_system = new jL_system;
	m_system->Initialize();


	CprjVinylPeelTesterDlg dlg;
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
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}


	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}



int CprjVinylPeelTesterApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_system != nullptr)
	{
		delete m_system;
		m_system = nullptr;
	}

	return CWinApp::ExitInstance();
}


CWnd* CprjVinylPeelTesterApp::GetMainWnd()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CWinApp::GetMainWnd();
}

jL_system* CprjVinylPeelTesterApp::GetSystem()
{
	return m_system;
}
