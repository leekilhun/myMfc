
// cylinder.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "cylinder.h"
#include "cylinderDlg.h"
#include "iCylinder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcylinderApp

BEGIN_MESSAGE_MAP(CcylinderApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CcylinderApp 생성

CcylinderApp::CcylinderApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	IO = nullptr;
}


// 유일한 CcylinderApp 개체입니다.

CcylinderApp theApp;
void Test() {
	//int ret = 0;
	//CylinderData cyl(L"TestCyl", L"./Cylinder.txt");
	//performanceTimer timer;
	//timer.StartTimer();
	//TRACE("\n\r");
	//TRACE("test Time : 0 [%7.6f] \n\r", timer.GetElaspTime());
	//
	//TRACE("test Time : 1 [%7.6f] \n\r", timer.GetElaspTime());
	//virtualRS rs(1024);
	//TRACE("test Time : 2 [%7.6f] \n\r", timer.GetElaspTime());
	//rs.ReadFile("./io.dat");
	//
	//char r_str[10] = { 0, };

	////rs.GetItem(10, r_str, 10);

	

	//CString ret = IO->GetTitle(10005);
	//ret = IO->GetTitle(30005);
	//Table_IO* IO_TBL = nullptr;
	//IO_TBL = IO.GetIOTable();


	//CString item(L"");
	//item = IO_TBL->ioGetInItem(10004);


	/*char* ret_str = new char[32768];
	rs.ReadFile("./io.dat", ret_str, 32768);
	TRACE("test Time : 3 [%7.6f] \n\r", timer.GetElaspTime());
	rs.WriteFile("./io_write.dat");
	TRACE("test Time : 4 [%7.6f] \n\r", timer.GetElaspTime());
	delete[] ret_str;*/

}

// CcylinderApp 초기화

BOOL CcylinderApp::InitInstance()
{
	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}


	IO = new DTIO();

	CcylinderDlg dlg;
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
	

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고  응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}



int CcylinderApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (IO != nullptr)
		delete IO;

	IO = nullptr;
	return CWinApp::ExitInstance();
}
