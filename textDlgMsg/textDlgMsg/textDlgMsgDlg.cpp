
// textDlgMsgDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "textDlgMsg.h"
#include "textDlgMsgDlg.h"
#include "afxdialogex.h"
#include "UIThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtextDlgMsgDlg 대화 상자



CtextDlgMsgDlg::CtextDlgMsgDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TEXTDLGMSG_DIALOG, pParent)
{
	m_popMsg = nullptr;
	m_thread = nullptr;
	m_UIThread = nullptr;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CtextDlgMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_1, m_EditBox);
}

BEGIN_MESSAGE_MAP(CtextDlgMsgDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START, &CtextDlgMsgDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_POP, &CtextDlgMsgDlg::OnBnClickedBtnPop)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_PAUSE, &CtextDlgMsgDlg::OnBnClickedBtnPause)
	ON_BN_CLICKED(IDC_BTN_RESUME, &CtextDlgMsgDlg::OnBnClickedBtnResume)
	ON_BN_CLICKED(IDC_BTN_END, &CtextDlgMsgDlg::OnBnClickedBtnEnd)
	ON_MESSAGE(WM_USER_RECEIVE, &CtextDlgMsgDlg::OnUserReceive)
	ON_BN_CLICKED(IDC_BTN_THREAD, &CtextDlgMsgDlg::OnBnClickedBtnThread)
END_MESSAGE_MAP()


// CtextDlgMsgDlg 메시지 처리기

BOOL CtextDlgMsgDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	CtextDlgMsgApp*main = (CtextDlgMsgApp*)AfxGetApp();
	m_thread = main->GetSystem();
	//m_thread = theApp.GetSystem();
	
	/*m_thread->SetHwnd(this->GetSafeHwnd());
	m_thread->ThreadRun();*/
	int closure = 16;
	m_thread->ThreadRun();
	m_thread->SetIRS(this, main->StaticWrapper, &closure);

	main->m_pTimeScheduler->AttachCallbackFunc(this, SetPassTime);
	main->m_pTimeScheduler->Start();
	



	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CtextDlgMsgDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CtextDlgMsgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtextDlgMsgDlg::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
		m_thread->RunThread();

		if (m_thread->m_Continue == FALSE)
			m_thread->m_Continue = TRUE;
}


void CtextDlgMsgDlg::OnBnClickedBtnPop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_popMsg == nullptr)
	{
		m_popMsg = new CpopMsg(this);
		m_popMsg->setter(this);
		m_popMsg->Create(IDD_DLG_CHILD1); //, CWnd::GetDesktopWindow()
	}
	m_popMsg->ShowWindow(SW_SHOW);
}



void CtextDlgMsgDlg::OnBnClickedBtnPause()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_thread->SuspendThread();
	/* 이 상태에서 종료시 정상 종료가 안됨*/
}


void CtextDlgMsgDlg::OnBnClickedBtnResume()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_thread->ResumeThread();
}


void CtextDlgMsgDlg::OnBnClickedBtnEnd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_thread->TerminateThread();
}


afx_msg LRESULT CtextDlgMsgDlg::OnUserReceive(WPARAM wParam, LPARAM lParam)
{
	int no = (int)wParam;
	CString* msg;
	msg = ((CString*)lParam);
	CString result = msg->GetString();

	CString pre_str;
	if (m_EditBox.GetLineCount() < 10)
		GetDlgItemText(IDC_EDIT_1, pre_str);

	m_EditBox.SetWindowText(pre_str + result + L"\r\n");


	return 0;
}


void CtextDlgMsgDlg::OnBnClickedBtnThread()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_UIThread = new CUIThread();
	//m_UIThread->CreateThread();
	AfxBeginThread(RUNTIME_CLASS(CUIThread));


}

void CtextDlgMsgDlg::SetText(CString& str)
{
	CString pre_str;
	if (m_EditBox.GetLineCount() < 10)
		GetDlgItemText(IDC_EDIT_1, pre_str);
	
	m_EditBox.SetWindowText(pre_str + str + L"\r\n");
}

void CtextDlgMsgDlg::SetPassTime(LPVOID pParam)
{
	CtextDlgMsgDlg* pThis;
	pThis = (CtextDlgMsgDlg*)pParam;
	CString pre_str;
	if (pThis->m_EditBox.GetLineCount() < 10)
		pThis->GetDlgItemText(IDC_EDIT_1, pre_str);

	
	pThis->m_EditBox.SetWindowText(pre_str + L"Test" + L"\r\n");

}



void CtextDlgMsgDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CtextDlgMsgApp* main = (CtextDlgMsgApp*)AfxGetApp();
	m_thread = main->GetSystem();

	main->m_pTimeScheduler->Stop();
	main->m_pTimeScheduler->EndThread();


	if (m_UIThread != nullptr)
	{
		m_UIThread->PostThreadMessage(WM_QUIT, 0, 0);

		delete m_UIThread;
		m_UIThread = nullptr;
	}

	if (m_popMsg != nullptr)
	{
		delete m_popMsg;
		m_popMsg = nullptr;
	}

	CtTimer t;
	m_thread->ResumeThread();
	t.Start();
	while (1)	{	
		if (t.MoreThan(1)) break;	
	}

	if (m_thread != nullptr)
	{
		
		m_thread->TerminateThread();
		Sleep(10);
		m_thread = nullptr;
	}

}

BOOL CtextDlgMsgDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DestroyWindow();
}
