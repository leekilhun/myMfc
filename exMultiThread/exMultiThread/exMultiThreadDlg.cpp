
// exMultiThreadDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "exMultiThread.h"
#include "exMultiThreadDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexMultiThreadDlg 대화 상자



CexMultiThreadDlg::CexMultiThreadDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXMULTITHREAD_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CexMultiThreadDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listBd);
	DDX_Control(pDX, IDC_EDIT_NUM, m_playerCnt);
	DDX_Control(pDX, IDC_SPIN_CNT, m_spCnt);
}

BEGIN_MESSAGE_MAP(CexMultiThreadDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_TERMINATE, &CexMultiThreadDlg::OnBnClickedBtnTerminate)
	ON_BN_CLICKED(IDC_BTN_STOP, &CexMultiThreadDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_START, &CexMultiThreadDlg::OnBnClickedBtnStart)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_CNT, &CexMultiThreadDlg::OnDeltaposSpinCnt)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CexMultiThreadDlg 메시지 처리기

BOOL CexMultiThreadDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_playerCnt.SetWindowTextW(L"0");
	m_spCnt.SetRange(0, 10);
	m_spCnt.SetPos(0);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CexMultiThreadDlg::OnPaint()
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
HCURSOR CexMultiThreadDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CexMultiThreadDlg::OnBnClickedBtnTerminate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	::SendMessage(this->m_hWnd, WM_CLOSE, NULL, NULL);
}


void CexMultiThreadDlg::OnBnClickedBtnStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMultiThreadDlg::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMultiThreadDlg::OnDeltaposSpinCnt(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int val = pNMUpDown->iPos + pNMUpDown->iDelta;
	if (Between(val,0,10) == TRUE)
	{
		CString str_vlu;
		str_vlu.Format(L"%d\n", val);
		m_playerCnt.SetWindowText(str_vlu);
	}
	*pResult = 0;
}


void CexMultiThreadDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CreateWindow(_T("static"), _T("Only Text"), WS_CHILD | WS_VISIBLE,
		20, 85, 100, 25, this->m_hWnd, (HMENU)-1/*(HMENU)109*/, NULL, NULL);

}
