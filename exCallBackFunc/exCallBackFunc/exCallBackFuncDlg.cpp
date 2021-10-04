
// exCallBackFuncDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "exCallBackFunc.h"
#include "exCallBackFuncDlg.h"
#include "afxdialogex.h"

#include "b.h"
#include "a.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexCallBackFuncDlg 대화 상자



CexCallBackFuncDlg::CexCallBackFuncDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXCALLBACKFUNC_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pB = nullptr;
}

void CexCallBackFuncDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CexCallBackFuncDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON1, &CexCallBackFuncDlg::OnBnClickedButton1)
	ON_MESSAGE(WM_USER_CALLBACK_1, &CexCallBackFuncDlg::OnUserCallback)
END_MESSAGE_MAP()


// CexCallBackFuncDlg 메시지 처리기

BOOL CexCallBackFuncDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_pB = theApp.GetBClass();

	int closure = 16;

	// winddow handler를 얻는 방법
	CWnd* pWnd = AfxGetMainWnd();
	HWND hCommWnd = pWnd->m_hWnd;//HWND ha = GetSafeHwnd(); //this->m_hWnd
	ma a(hCommWnd);

	// 윈도우 메세지 받기
	a.WindowMassgeMethod();

	// 일반 멤버함수 콜백
	//a.StaticWrapper(this, "test", &closure);
	// 콜백 함수 등록( a 객체의 주소도 함께 전달 ) 
	m_pB->SetCallback(&a, ma::StaticWrapper, &closure); 
	m_pB->DoAction(); // 등록된 콜백 함수 호출

	// 전역함수 콜백
	m_pB->SetCallbackStatic(ma::StaticMethod, &closure);
	m_pB->DoActionStatic();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CexCallBackFuncDlg::OnPaint()
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
HCURSOR CexCallBackFuncDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CexCallBackFuncDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CexCallBackFuncDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


}


afx_msg LRESULT CexCallBackFuncDlg::OnUserCallback(WPARAM wParam, LPARAM lParam)
{
	AfxMessageBox(L"OK");
	return 0;
}

void CexCallBackFuncDlg::Callback()
{
	AfxMessageBox(L"Callback");
}
