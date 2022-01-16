
// nextionLCDDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "nextionLCD.h"
#include "nextionLCDDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CnextionLCDDlg 대화 상자

#define UPDATE_TIMER_NEXTION_LCD_DLG 1



#define IO_REG_0_Pos                     (0U)                              
#define IO_REG_0_Msk                     (0x1UL << IO_REG_0_Pos)        /*!< 0x00000001 */
#define IO_REG_0                         IO_REG_0_Msk                  /*!< Filter bit 0 */
#define IO_REG_1_Pos                     (1U)                              
#define IO_REG_1_Msk                     (0x1UL << IO_REG_1_Pos)        /*!< 0x00000002 */
#define IO_REG_1                         IO_REG_1_Msk                  /*!< Filter bit 1 */
#define IO_REG_2_Pos                     (2U)                              
#define IO_REG_2_Msk                     (0x1UL << IO_REG_2_Pos)        /*!< 0x00000004 */
#define IO_REG_2                         IO_REG_2_Msk                  /*!< Filter bit 2 */
#define IO_REG_3_Pos                     (3U)                              
#define IO_REG_3_Msk                     (0x1UL << IO_REG_3_Pos)        /*!< 0x00000008 */
#define IO_REG_3                         IO_REG_3_Msk                  /*!< Filter bit 3 */
#define IO_REG_4_Pos                     (4U)                              
#define IO_REG_4_Msk                     (0x1UL << IO_REG_4_Pos)        /*!< 0x00000010 */
#define IO_REG_4                         IO_REG_4_Msk                  /*!< Filter bit 4 */
#define IO_REG_5_Pos                     (5U)                              
#define IO_REG_5_Msk                     (0x1UL << IO_REG_5_Pos)        /*!< 0x00000020 */
#define IO_REG_5                         IO_REG_5_Msk                  /*!< Filter bit 5 */
#define IO_REG_6_Pos                     (6U)                              
#define IO_REG_6_Msk                     (0x1UL << IO_REG_6_Pos)        /*!< 0x00000040 */
#define IO_REG_6                         IO_REG_6_Msk                  /*!< Filter bit 6 */
#define IO_REG_7_Pos                     (7U)                              
#define IO_REG_7_Msk                     (0x1UL << IO_REG_7_Pos)        /*!< 0x00000080 */
#define IO_REG_7                         IO_REG_7_Msk                  /*!< Filter bit 7 */

#define IO_REG_8_Pos                     (8U)                              
#define IO_REG_8_Msk                     (0x1UL << IO_REG_8_Pos)        /*!< 0x00000100 */
#define IO_REG_8                         IO_REG_8_Msk                  /*!< Filter bit 8 */
#define IO_REG_9_Pos                     (9U)                              
#define IO_REG_9_Msk                     (0x1UL << IO_REG_9_Pos)        /*!< 0x00000100 */
#define IO_REG_9                         IO_REG_9_Msk                  /*!< Filter bit 9 */
#define IO_REG_10_Pos                    (10U)                              
#define IO_REG_10_Msk                    (0x1UL << IO_REG_10_Pos)        /*!< 0x00000100 */
#define IO_REG_10                        IO_REG_10_Msk                  /*!< Filter bit 10 */
#define IO_REG_11_Pos                    (11U)                              
#define IO_REG_11_Msk                    (0x1UL << IO_REG_11_Pos)        /*!< 0x00000100 */
#define IO_REG_11                        IO_REG_11_Msk                  /*!< Filter bit 11 */
#define IO_REG_12_Pos                    (12U)                              
#define IO_REG_12_Msk                    (0x1UL << IO_REG_12_Pos)        /*!< 0x00000100 */
#define IO_REG_12                        IO_REG_12_Msk                  /*!< Filter bit 12 */
#define IO_REG_13_Pos                    (13U)                              
#define IO_REG_13_Msk                    (0x1UL << IO_REG_13_Pos)        /*!< 0x00000100 */
#define IO_REG_13                        IO_REG_13_Msk                  /*!< Filter bit 13 */
#define IO_REG_14_Pos                    (14U)                              
#define IO_REG_14_Msk                    (0x1UL << IO_REG_14_Pos)        /*!< 0x00000100 */
#define IO_REG_14                        IO_REG_14_Msk                  /*!< Filter bit 14 */
#define IO_REG_15_Pos                    (15U)                              
#define IO_REG_15_Msk                    (0x1UL << IO_REG_15_Pos)        /*!< 0x00000100 */
#define IO_REG_15                        IO_REG_15_Msk                  /*!< Filter bit 15 */




CnextionLCDDlg::CnextionLCDDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_NEXTIONLCD_DIALOG, pParent)
{
	m_pLcd = nullptr;
	m_TimerID = 0;
	m_isAutoReady = false;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnextionLCDDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PortNo, m_comboComList);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBaudrateList);
	int i = 0;
	DDX_Control(pDX, IDC_SW_1, m_sensor[i++]);
	DDX_Control(pDX, IDC_SW_2, m_sensor[i++]);
	DDX_Control(pDX, IDC_SW_3, m_sensor[i++]);
	DDX_Control(pDX, IDC_SW_4, m_sensor[i++]);
	DDX_Control(pDX, IDC_SW_5, m_sensor[i++]);
	DDX_Control(pDX, IDC_SW_6, m_sensor[i++]);
	DDX_Control(pDX, IDC_SW_7, m_sensor[i++]);
	DDX_Control(pDX, IDC_SW_8, m_sensor[i++]);

	i = 0;
	DDX_Control(pDX, IDC_LED_1, m_led[i++]);
	DDX_Control(pDX, IDC_LED_2, m_led[i++]);
	DDX_Control(pDX, IDC_LED_3, m_led[i++]);
	DDX_Control(pDX, IDC_LED_4, m_led[i++]);
	DDX_Control(pDX, IDC_LED_5, m_led[i++]);
	DDX_Control(pDX, IDC_LED_6, m_led[i++]);
	DDX_Control(pDX, IDC_LED_7, m_led[i++]);
	DDX_Control(pDX, IDC_LED_8, m_led[i++]);

	DDX_Control(pDX, IDC_LIST_RCV_LOG, m_recvList);
	DDX_Control(pDX, IDC_BTN_START, m_btnStart);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
	DDX_Control(pDX, IDC_BTN_RESET, m_btnReset);

	i = 0;
	DDX_Control(pDX, IDC_ST_BIT0, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT1, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT2, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT3, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT4, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT5, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT6, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT7, m_bitState[i++]);

	DDX_Control(pDX, IDC_ST_BIT8, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT9, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT10, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT11, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT12, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT13, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT14, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT15, m_bitState[i++]);

	DDX_Control(pDX, IDC_ST_BIT16, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT17, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT18, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT19, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT20, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT21, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT22, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT23, m_bitState[i++]);

	DDX_Control(pDX, IDC_ST_BIT24, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT25, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT26, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT27, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT28, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT29, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT30, m_bitState[i++]);
	DDX_Control(pDX, IDC_ST_BIT31, m_bitState[i++]);
}

BEGIN_MESSAGE_MAP(CnextionLCDDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CnextionLCDDlg::OnBnClickedBtnConnect)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_NEXTION_LCD_MAIN_DLG_MESSAGE, &CnextionLCDDlg::OnUpdateCommLogList)
	ON_LBN_DBLCLK(IDC_LIST_RCV_LOG, &CnextionLCDDlg::OnLbnDblclkListRcvLog)
	ON_BN_CLICKED(IDC_BTN_START, &CnextionLCDDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CnextionLCDDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_RESET, &CnextionLCDDlg::OnBnClickedBtnReset)
END_MESSAGE_MAP()


// CnextionLCDDlg 메시지 처리기

BOOL CnextionLCDDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	CnextionLCDApp* pMain = (CnextionLCDApp*)AfxGetApp();
	m_pLcd = pMain->Get_nextionObj();

	m_pLcd->m_ComPort->AddStringPortList(&m_comboComList);
	m_pLcd->m_ComPort->AddStringBaudrateList(&m_comboBaudrateList);

	m_pLcd->m_hCommWnd = this-> GetSafeHwnd();

	m_pLcd->ThreadRun();

	AutoReady(false);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CnextionLCDDlg::OnPaint()
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
HCURSOR CnextionLCDDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CnextionLCDDlg::OnBnClickedBtnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pLcd == nullptr)
		return;

	if (m_pLcd->m_IsPortOpen)
	{
		m_pLcd->CloseComPort();
		m_pLcd->m_NextionComm.is_init = false;
		m_pLcd->m_IsPortOpen = false;
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("OPEN"));
	}
	else
	{
		CString port_name;//_T("COM11");
		m_comboComList.GetLBText(m_comboComList.GetCurSel(), port_name.GetBuffer(1024));
		port_name.ReleaseBuffer();

		DWORD_PTR baudrate = m_comboBaudrateList.GetItemData(m_comboBaudrateList.GetCurSel());
		WORD port_id = m_comboComList.GetCurSel();


		CWnd* pWnd = AfxGetMainWnd();
		HWND hCommWnd = pWnd->m_hWnd;
		
		uint32_t result = m_pLcd->OpenComPort(port_name, (DWORD)baudrate, port_id);
		if (result != 0)
			AfxMessageBox(L"connect faliled");
		else
		{
			//AfxMessageBox(L"connect successed\n");
			m_pLcd->m_NextionComm.baud = (uint32_t)baudrate;
			m_pLcd->m_NextionComm.is_init = true;
			m_pLcd->m_IsPortOpen = TRUE;
			GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("CLOSE"));


			m_pLcd->ChangeLCDPage(DEF_NEX_PAGE_INIT);
		}

	}
}

uint16_t CnextionLCDDlg::GetInputReg()
{
	uint16_t input_reg = 0;

	if (m_pLcd == nullptr)
		return 0;

	input_reg = m_sensor[0].GetCheck() << 15
			| m_sensor[1].GetCheck() << 14
			| m_sensor[2].GetCheck() << 13
			| m_sensor[3].GetCheck() << 12
			| m_sensor[4].GetCheck() << 11
			| m_sensor[5].GetCheck() << 10
			| m_sensor[6].GetCheck() << 9
			| m_sensor[7].GetCheck() << 8;

	m_pLcd->m_armStatus.input_reg = input_reg;	
		
	CString page_name;
	switch (m_pLcd->GetLCDPageNo())
	{
	case DEF_NEX_PAGE_INIT: 
	{
		page_name = L"Page Initial";	
	}
	break;
	case DEF_NEX_PAGE_MAIN:	
	{
		page_name = L"Page Main";
		if (m_isAutoReady)
		{
			m_isAutoReady = false;
			AutoReady(true);
		}
	}
	break;
	case DEF_NEX_PAGE_MANUAL:
	{
		page_name = L"Page Manual"; 
	}
	break;
	case DEF_NEX_PAGE_TEACH:	
	{
		page_name = L"Page Teaching"; 
	}
	break;
	case DEF_NEX_PAGE_IO:
	{
		page_name = L"Page IO Table"; 
	}
	break;
	case DEF_NEX_PAGE_DATA:
	{
		page_name = L"Page Data";
	}
	break;
	case DEF_NEX_PAGE_LOG:	
	{
		page_name = L"Page Log";
	}
	break;
	default:
		break;
	}
	SetDlgItemText(IDC_STAT_PAGE_NO, page_name);

	return input_reg;
}

uint16_t CnextionLCDDlg::SetOutputReg()
{
	if (m_pLcd == nullptr)
		return 0;

	m_led[0].SetCheck((m_pLcd->m_armStatus.output_reg & IO_REG_15) >> 15);
	m_led[1].SetCheck((m_pLcd->m_armStatus.output_reg & IO_REG_14) >> 14);
	m_led[2].SetCheck((m_pLcd->m_armStatus.output_reg & IO_REG_13) >> 13);
	m_led[3].SetCheck((m_pLcd->m_armStatus.output_reg & IO_REG_12) >> 12);
	m_led[4].SetCheck((m_pLcd->m_armStatus.output_reg & IO_REG_11) >> 11);
	m_led[5].SetCheck((m_pLcd->m_armStatus.output_reg & IO_REG_10) >> 10);
	m_led[6].SetCheck((m_pLcd->m_armStatus.output_reg & IO_REG_9) >> 9);
	m_led[7].SetCheck((m_pLcd->m_armStatus.output_reg & IO_REG_8) >> 8);

	return uint16_t();
}


void CnextionLCDDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_pLcd->m_IsPortOpen)
	{	
		if (m_pLcd->GetLCDPageNo() == DEF_NEX_PAGE_INIT)
			m_pLcd->ChangeLCDPage(DEF_NEX_PAGE_IO);
		GetInputReg();
		SetOutputReg();
	}
	

	CDialogEx::OnTimer(nIDEvent);
}


void CnextionLCDDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow == TRUE)
	{
		m_TimerID = SetTimer(UPDATE_TIMER_NEXTION_LCD_DLG, 100, NULL);
	}
	else
	{
		if (m_TimerID != 0)
		{
			KillTimer(m_TimerID);
			m_TimerID = 0;			
		}
	}
	
}


afx_msg LRESULT CnextionLCDDlg::OnUpdateCommLogList(WPARAM wParam, LPARAM lParam)
{
	uint32_t addr = (uint32_t)lParam;
	std::string log_str =  m_pLcd->m_log->PopLog(addr);
	//char temp[100];
	//strcpy(temp, log_str.c_str());
	TCHAR ret_msg[255] = { 0, };
	trans::CharToLPTSTR((LPSTR)log_str.c_str(), &ret_msg[0], 255);

	if (m_recvList.GetCount() == 24)
		m_recvList.ResetContent();

	m_recvList.AddString(ret_msg);
	return 0;
}

void CnextionLCDDlg::AutoReady(bool on_off)
{
	if (on_off)
	{
		CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_START);
		pBtn->EnableWindow(TRUE);
		pBtn = (CButton*)GetDlgItem(IDC_BTN_STOP);
		pBtn->EnableWindow(TRUE);
		pBtn = (CButton*)GetDlgItem(IDC_BTN_RESET);
		pBtn->EnableWindow(TRUE);
		return;
	}
	//m_btnStart.

	CButton* pBtn = (CButton*)GetDlgItem(IDC_BTN_START);
	pBtn->EnableWindow(FALSE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_STOP);
	pBtn->EnableWindow(FALSE);
	pBtn = (CButton*)GetDlgItem(IDC_BTN_RESET);
	pBtn->EnableWindow(FALSE);

}


void CnextionLCDDlg::OnLbnDblclkListRcvLog()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_recvList.ResetContent();
}


void CnextionLCDDlg::OnBnClickedBtnStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CnextionLCDDlg::OnBnClickedBtnStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CnextionLCDDlg::OnBnClickedBtnReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
