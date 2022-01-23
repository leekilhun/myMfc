
// fastechModuleDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "fastechModule.h"
#include "fastechModuleDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CfastechModuleDlg 대화 상자


CfastechModuleDlg::CfastechModuleDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FASTECHMODULE_DIALOG, pParent)
{
	m_pTabMain = nullptr;
	m_pTabIo = nullptr;
	m_pTabTest = nullptr;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CfastechModuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_BODY, m_tabBody);
}

BEGIN_MESSAGE_MAP(CfastechModuleDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_BODY, &CfastechModuleDlg::OnTcnSelchangeTabBody)
END_MESSAGE_MAP()


// CfastechModuleDlg 메시지 처리기

BOOL CfastechModuleDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	SetWindowText(L"Fastech Module");
	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	int idx = 0;
	m_tabBody.DeleteAllItems();

	int index = 0;
#if 0
	m_tabHead.InsertItem(index++, L"Main Tab");
	m_tabHead.InsertItem(index++, L"Manual Tab");
	m_tabHead.InsertItem(index++, L"IO Tab");
	m_tabHead.InsertItem(index++, L"Log Tab");

#endif // 0

	CImageList img_list;
	img_list.Create(IDB_BITMAP1, 16, 5, RGB(255, 255, 255));
	////Create(IDB_BITMAP3, 16, 10, RGB(255, 255, 255));
	CBitmap bm;
	bm.LoadBitmap(IDB_BITMAP2);
	img_list.Add(&bm, RGB(255, 255, 255));


	m_tabBody.SetImageList(&img_list);
	img_list.Detach();

	m_tabBody.InsertItem(idx++, L"Main Tab", 0);
	m_tabBody.InsertItem(idx++, L"io Tab", 1);
	m_tabBody.InsertItem(idx++, L"Test Tab", 1);

	CRect rec_item;
	//m_tabBody.GetWindowRect(&rec_item);

	m_tabBody.GetItemRect(0, &rec_item);
	m_pTabMain = new tab_main;
	m_pTabMain->Create(IDD_TAB_MAIN, &m_tabBody);
	m_pTabMain->GetWindowRect(&rec_item);
	m_pTabMain->MoveWindow(5, 25, rec_item.Width(), rec_item.Height());
	m_pTabMain->ShowWindow(SW_SHOW);


	m_pTabIo = new tab_io;
	m_pTabIo->Create(IDD_TAB_IO, &m_tabBody);
	m_pTabIo->MoveWindow(5, 25, rec_item.Width(), rec_item.Height());
	m_pTabIo->ShowWindow(SW_HIDE);

	m_pTabTest = new tab_test;
	m_pTabTest->Create(IDD_TAB_TEST, &m_tabBody);
	m_pTabTest->MoveWindow(5, 25, rec_item.Width(), rec_item.Height());
	m_pTabTest->ShowWindow(SW_HIDE);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CfastechModuleDlg::OnPaint()
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
HCURSOR CfastechModuleDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//BOOL CfastechModuleDlg::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
//{
//	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
//
//	return CDialogEx::Create(lpszTemplateName, pParentWnd);
//}


void CfastechModuleDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_pTabMain != nullptr)
	{
		delete m_pTabMain;
	}
	m_pTabMain = nullptr;


	if (m_pTabIo != nullptr)
	{
		delete m_pTabIo;
	}
	m_pTabIo = nullptr;

	if (m_pTabTest != nullptr)
	{
		delete m_pTabTest;
	}
	m_pTabTest = nullptr;
}


int CfastechModuleDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_pTabMain = nullptr;
	return 0;
}


void CfastechModuleDlg::OnTcnSelchangeTabBody(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	int curr_tab = this->m_tabBody.GetCurSel();
	if (curr_tab == 0)
	{
		m_pTabMain->ShowWindow(SW_SHOW);
		m_pTabIo->ShowWindow(SW_HIDE);
		m_pTabTest->ShowWindow(SW_HIDE);
	}
	else if (curr_tab == 1)
	{
		m_pTabMain->ShowWindow(SW_HIDE);
		m_pTabIo->ShowWindow(SW_SHOW);
		m_pTabTest->ShowWindow(SW_HIDE);
	}
	else if (curr_tab == 2)
	{
		m_pTabMain->ShowWindow(SW_HIDE);
		m_pTabIo->ShowWindow(SW_HIDE);
		m_pTabTest->ShowWindow(SW_SHOW);
	}
}
