
// exMfcHowToUsingDateTimePickerControlDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "exMfcHowToUsingDateTimePickerControl.h"
#include "exMfcHowToUsingDateTimePickerControlDlg.h"
#include "afxdialogex.h"

#include "stdlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CexMfcHowToUsingDateTimePickerControlDlg 대화 상자



CexMfcHowToUsingDateTimePickerControlDlg::CexMfcHowToUsingDateTimePickerControlDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXMFCHOWTOUSINGDATETIMEPICKERCONTROL_DIALOG, pParent)
	, m_year1(_T(""))
	, m_year2(_T(""))
	, m_month1(_T(""))
	, m_month2(_T(""))
	, m_day1(_T(""))
	, m_day2(_T(""))
	, m_radioBookingSelection(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CexMfcHowToUsingDateTimePickerControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BOOKING, m_bookingDateTimeCtrl);
	DDX_Text(pDX, IDC_EDIT_YEAR1, m_year1);
	DDX_Text(pDX, IDC_EDIT_YEAR2, m_year2);
	DDX_Text(pDX, IDC_EDIT_MONTH1, m_month1);
	DDX_Text(pDX, IDC_EDIT_MONTH2, m_month2);
	DDX_Text(pDX, IDC_EDIT_DAY1, m_day1);
	DDX_Text(pDX, IDC_EDIT_DAY2, m_day2);
	DDX_Radio(pDX, IDC_RADIO1, m_radioBookingSelection);
}

BEGIN_MESSAGE_MAP(CexMfcHowToUsingDateTimePickerControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BOOK, &CexMfcHowToUsingDateTimePickerControlDlg::OnBnClickedButtonBook)
	ON_BN_CLICKED(IDC_RADIO1, &CexMfcHowToUsingDateTimePickerControlDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CexMfcHowToUsingDateTimePickerControlDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()


// CexMfcHowToUsingDateTimePickerControlDlg 메시지 처리기

BOOL CexMfcHowToUsingDateTimePickerControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CexMfcHowToUsingDateTimePickerControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CexMfcHowToUsingDateTimePickerControlDlg::OnPaint()
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
HCURSOR CexMfcHowToUsingDateTimePickerControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CexMfcHowToUsingDateTimePickerControlDlg::OnBnClickedButtonBook()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// when booking button is clicked lets take the date from the control
	// and place it into the selected edit boxes
	// The redio button tells which is selected

	// First get the date from the control:

	CTime bookingDate;
	m_bookingDateTimeCtrl.GetTime(bookingDate);
	// update the m_radioBookingSelection variable according
	// to the radio button selection : if the first is selected
	// it has a value 0, if the second then 1.
	UpdateData();
	if (m_radioBookingSelection == 0)
	{
		m_day1.Format(_T("%d"), bookingDate.GetDay());
		m_month1.Format(_T("%d"), bookingDate.GetMonth());
		m_year1.Format(_T("%d"), bookingDate.GetYear());
	}
	else
	{
		m_day2.Format(_T("%d"), bookingDate.GetDay());
		m_month2.Format(_T("%d"), bookingDate.GetMonth());
		m_year2.Format(_T("%d"), bookingDate.GetYear());
	}
	// and lastly place the new values from the m_day1, m_day2,...
	// variables into the corresponding edit controls:
	UpdateData(FALSE);
}



void CexMfcHowToUsingDateTimePickerControlDlg::OnBnClickedRadio1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// we need to take now the values from the edit box controls
	// into the string variables linked to them, so update them by:
	UpdateData();
	// check that each field fas a value (if not, we connot update
	// date control):
	if (m_year1.IsEmpty() || m_month1.IsEmpty() || m_day1.IsEmpty())
		return; // one field did not have value, so do nothing

	// we need to convert strings to integers. use stdlib.h
	// _ttoi to do that:
	CTime bookingDate{_ttoi(m_year1),_ttoi(m_month1),_ttoi(m_day1),0,0,0};
	// bookingDate contains now the time we want the date control to have, so assign
	// it to the control:
	m_bookingDateTimeCtrl.SetTime(&bookingDate);
}


void CexMfcHowToUsingDateTimePickerControlDlg::OnBnClickedRadio2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	// check that each field fas a value (if not, we connot update
	// date control):
	if (m_year2.IsEmpty() || m_month2.IsEmpty() || m_day2.IsEmpty())
		return; // one field did not have value, so do nothing

	// we need to convert strings to integers. use stdlib.h
	// _ttoi to do that:
	CTime bookingDate{_ttoi(m_year2),_ttoi(m_month2),_ttoi(m_day2),0,0,0};
	// bookingDate contains now the time we want the date control to have, so assign
	// it to the control:
	m_bookingDateTimeCtrl.SetTime(&bookingDate);
}
