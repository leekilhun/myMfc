
// exThreadProgressDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "exThreadProgress.h"
#include "exThreadProgressDlg.h"
#include "afxdialogex.h"
#include "UIThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexThreadProgressDlg 대화 상자



CexThreadProgressDlg::CexThreadProgressDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXTHREADPROGRESS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_threadWT = nullptr;
	m_threadUI = nullptr;
	m_lifeThreadWT = FALSE;
	m_lifeThreadUI = FALSE;
	m_i = 0;
	m_per = 0;
}

CexThreadProgressDlg::~CexThreadProgressDlg()
{
	//StopThread();

}

void CexThreadProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_WT, m_progressWT);
	DDX_Control(pDX, IDC_PROGRESS_UI, m_progressUI);
	DDX_Control(pDX, IDC_STATIC_WT, m_strWT);
	DDX_Control(pDX, IDC_STATIC_UI, m_strUI);
}

BEGIN_MESSAGE_MAP(CexThreadProgressDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_START_WT, &CexThreadProgressDlg::OnBnClickedBtnStartWt)
	ON_BN_CLICKED(IDC_BTN_START_UI, &CexThreadProgressDlg::OnBnClickedBtnStartUi)
	ON_BN_CLICKED(IDC_BTN_QUIT, &CexThreadProgressDlg::OnBnClickedBtnQuit)
END_MESSAGE_MAP()


// CexThreadProgressDlg 메시지 처리기

BOOL CexThreadProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_progressWT.SetRange(0, 1000);
	m_progressUI.SetRange(0, 1000);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CexThreadProgressDlg::OnPaint()
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
HCURSOR CexThreadProgressDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CexThreadProgressDlg::OnBnClickedBtnStartWt()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (m_threadWT != nullptr) return;

	
	m_threadWT = AfxBeginThread(ThreadFunc, this);
	m_lifeThreadWT = TRUE;

	//아래 코드를 사용하니 스레드 종료가 안됨
	//GetDlgItem(IDC_BTN_START_WT)->EnableWindow(FALSE);

}


void CexThreadProgressDlg::OnBnClickedBtnStartUi()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_threadUI != nullptr) return;
	
	m_threadUI = AfxBeginThread(RUNTIME_CLASS(CUIThread));
	m_lifeThreadUI = TRUE;
	//아래 코드를 사용하니 스레드 종료가 안됨
	//etDlgItem(IDC_BTN_START_UI)->EnableWindow(FALSE);
}

UINT CexThreadProgressDlg::WorkThread(LPVOID lParam)
{
	CexThreadProgressDlg* dlg = (CexThreadProgressDlg*)lParam;
	int per = 0;
	CString str;


	int i = 0;
	while (dlg->m_lifeThreadWT)
	{

		dlg->m_progressWT.SetPos(i % 1000);
		if (i % 10 == 0)
		{
			per++;
			str.Format(L"%d%%", per % 100);
			dlg->m_strWT.SetWindowText(str);
			//dlg->SetDlgItemTextW(IDC_STATIC_UI, str);
		}
		i++;
		Sleep(1);

	}
	dlg->GetDlgItem(IDC_BTN_START_WT)->EnableWindow(TRUE);
	TRACE("\r Exit Work Thread \n");
	return 0;
}

UINT CexThreadProgressDlg::ThreadFunc(LPVOID lParam)
{
	CexThreadProgressDlg* pThis;
	pThis = (CexThreadProgressDlg*)lParam;
	TRACE(_T("threadWT Run\n"));
	// Thread Loop
	while (pThis->m_lifeThreadWT)
	{
		pThis->threadJob();
		Sleep(1);
	}
	pThis->GetDlgItem(IDC_BTN_START_WT)->EnableWindow(TRUE);
	TRACE("\r Exit Work Thread \n");
	return 0;
}

void CexThreadProgressDlg::StopThread()
{
	m_lifeThreadWT = FALSE;
	m_lifeThreadUI = FALSE;

	DWORD result;
	if (m_threadWT != nullptr)
	{
		result = ::WaitForSingleObject(m_threadWT->m_hThread, 1 * 1000);
		if (result == WAIT_OBJECT_0)
		{
			// 이곳은 스레드를 확실히 종료된 상태임
			TRACE(L"\r Terminate Work Thread OK!");
		}
		else if (result == WAIT_TIMEOUT)
		{
			// 1초가 지나도 스레도가 종료되지 않음
			TRACE(L"\r Terminate Work Thread Timeout!");
			::TerminateThread(m_threadWT->m_hThread, result); // 강제 종료
			//::GetExitCodeThread(m_threadWT->m_hThread, &result);
		}
 		//delete m_threadWT;
		m_threadWT = nullptr;
	}

	result = 0;
	if (m_threadUI != nullptr)
	{
		result = ::WaitForSingleObject(m_threadUI->m_hThread, 1 * 1000);
		if (result == WAIT_OBJECT_0)
		{
			// 이곳은 스레드를 확실히 종료된 상태임
			TRACE(L"\r Terminate UI Thread OK!");
		}
		else if (result == WAIT_TIMEOUT)
		{
			// 1초가 지나도 스레도가 종료되지 않음
			TRACE(L"\r Terminate UI Thread Timeout!");
			::TerminateThread(m_threadUI->m_hThread, result); // 강제 종료
			//::GetExitCodeThread(m_threadUI->m_hThread, &result);
		}
		//delete  m_threadUI;
		m_threadUI = nullptr;

	}
	




}

void CexThreadProgressDlg::threadJob()
{
	CString str;
	m_progressWT.SetPos(m_i % 1000);
	if (m_i % 10 == 0)
	{
		m_per++;
		str.Format(L"%d%%", m_per % 100);
		m_strWT.SetWindowText(str);
		//dlg->SetDlgItemTextW(IDC_STATIC_UI, str);
	}
	m_i++;
}


BOOL CexThreadProgressDlg::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	StopThread();
	return CDialogEx::DestroyWindow();
}


void CexThreadProgressDlg::OnBnClickedBtnQuit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	StopThread();
	//OnDestroy();
}
