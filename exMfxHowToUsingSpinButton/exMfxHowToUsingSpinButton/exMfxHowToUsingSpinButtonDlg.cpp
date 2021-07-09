
// exMfxHowToUsingSpinButtonDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "exMfxHowToUsingSpinButton.h"
#include "exMfxHowToUsingSpinButtonDlg.h"
#include "afxdialogex.h"

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


// CexMfxHowToUsingSpinButtonDlg 대화 상자



CexMfxHowToUsingSpinButtonDlg::CexMfxHowToUsingSpinButtonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXMFXHOWTOUSINGSPINBUTTON_DIALOG, pParent)
	, m_float(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CexMfxHowToUsingSpinButtonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN_NUMBERS, m_spinNumbers);
	DDX_Text(pDX, IDC_EDIT_FLOAT, m_float);
	DDX_Control(pDX, IDC_SPIN_RECTANGLE, m_spinRectangle);
}

BEGIN_MESSAGE_MAP(CexMfxHowToUsingSpinButtonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDCANCEL, &CexMfxHowToUsingSpinButtonDlg::OnBnClickedCancel)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_NUMBERS, &CexMfxHowToUsingSpinButtonDlg::OnDeltaposSpinNumbers)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_RECTANGLE, &CexMfxHowToUsingSpinButtonDlg::OnDeltaposSpinRectangle)
END_MESSAGE_MAP()


// CexMfxHowToUsingSpinButtonDlg 메시지 처리기

BOOL CexMfxHowToUsingSpinButtonDlg::OnInitDialog()
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
	// Set m_spinNumbers to have values from 0 to 10, first value from 0.
	m_spinNumbers.SetRange(LOWEST_SPIN_VALUE, HIGHEST_SPIN_VALUE);
	m_spinNumbers.SetPos(LOWEST_SPIN_VALUE);

	m_spinRectangle.SetRange(LOWEST_SPIN_VALUE, HIGHEST_SPIN_VALUE);
	m_spinRectangle.SetPos(LOWEST_SPIN_VALUE);
	


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CexMfxHowToUsingSpinButtonDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CexMfxHowToUsingSpinButtonDlg::OnPaint()
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
		CPaintDC dc(this); 
		//lets draw the moving rectangle shape when spinner changes
		//values:

		// the left-top and right-botto, of the rectangle
		int left = 150;
		int top = m_rectBottomPos - 100;
		int right = 150 + 50;
		int bottom = m_rectBottomPos;

		// draws a green rectangle at the size and position above:
		dc.FillSolidRect(CRect{left,top,right,bottom}, RGB(0, 255, 0));

		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CexMfxHowToUsingSpinButtonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CexMfxHowToUsingSpinButtonDlg::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CDialogEx::OnCancel();
}


void CexMfxHowToUsingSpinButtonDlg::OnDeltaposSpinNumbers(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	
	//GetPos(); "the 16 bit current position in the low-order word"
	//So, we use LOWORD() macro to get that low-order word.
	int currentSpinPosition = LOWORD(m_spinNumbers.GetPos());

	// If the spin is at the lowest position or at the highest position
	// and the arrow button is pressed to nothing because the range limit
	// has been reached.

	// iDelta: how much the change in position will be the next change in spin value.
	// Positive means that up-arrow was pressed, negative that down-arrow pressed.
	// This section protecting that we do not go over the range limits:
	if ((currentSpinPosition == LOWEST_SPIN_VALUE && pNMUpDown->iDelta < 0) ||
		(currentSpinPosition == HIGHEST_SPIN_VALUE && pNMUpDown->iDelta > 0))
	{
		return; // over the limit, so do nothing
	}
	// lets create the decimal number as a CString (string) with one decimal.
	
	// if spin value range is from 0 to 10 and the current spin value is 3 then
	// print 5.0 + 3/10 = 5.3
	m_float.Format(_T("%.1f"), 5.0 + static_cast<float>(pNMUpDown->iPos + pNMUpDown->iDelta)
		/ static_cast<float>(HIGHEST_SPIN_VALUE - LOWEST_SPIN_VALUE));

	// update the float edit box view according to the  new m_float value:
	UpdateData(FALSE);

}


void CexMfxHowToUsingSpinButtonDlg::OnDeltaposSpinRectangle(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	int currentSpinPosition = LOWORD(m_spinRectangle.GetPos());
	if ((currentSpinPosition == LOWEST_SPIN_VALUE && pNMUpDown->iDelta < 0) ||
		(currentSpinPosition == HIGHEST_SPIN_VALUE && pNMUpDown->iDelta > 0))
	{
		return; // over the limit, so do nothing
	}

	// re-calculate the bottom of the rectangle
	// iPos goes from 0 to 10. On each arrow press iDelta is either 1 or -1.
	// 30 so it moves up a little more quickly
	m_rectBottomPos = RECT_START_BOTTOM_POS - (pNMUpDown->iPos + pNMUpDown->iDelta) * 30;

	// re-draw the dialog box
	Invalidate();
	UpdateWindow();


}
