// tab_test.cpp: 구현 파일
//

#include "pch.h"
#include "fastechModule.h"
#include "tab_test.h"
#include "afxdialogex.h"
#include "jL_CommSerial.h"

// tab_test 대화 상자

IMPLEMENT_DYNAMIC(tab_test, CDialogEx)

tab_test::tab_test(CWnd* pParent /*=nullptr*/)
	: m_IsPortOpen(NULL),  m_TimerID(0),
	m_input_reg(0), m_output_reg(0), CDialogEx(IDD_TAB_TEST, pParent)
{

}

tab_test::~tab_test()
{
}

void tab_test::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PortNo, m_comboComList);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBaudrateList);
	DDX_Control(pDX, IDC_ED_INPUT, m_editReceive);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnOpenClose);
	DDX_Control(pDX, IDC_LIST_RECEI_PACK, m_receiveList);

	DDX_Control(pDX, IDC_BEEP, m_beep);
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

	i = 0;
	DDX_Control(pDX, IDC_IN_01, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_02, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_03, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_04, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_05, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_06, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_07, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_08, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_09, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_10, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_11, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_12, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_13, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_14, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_15, m_wsIn[i++]);
	DDX_Control(pDX, IDC_IN_16, m_wsIn[i++]);

	i = 0;
	DDX_Control(pDX, IDC_OUT_01, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_02, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_03, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_04, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_05, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_06, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_07, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_08, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_09, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_10, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_11, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_12, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_13, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_14, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_15, m_wsOut[i++]);
	DDX_Control(pDX, IDC_OUT_16, m_wsOut[i++]);

}



BEGIN_MESSAGE_MAP(tab_test, CDialogEx)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &tab_test::OnBnClickedBtnConnect)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// tab_test 메시지 처리기


void tab_test::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


void tab_test::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}





void tab_test::updateReg(uint16_t reg, bool is_in)
{
}

uint16_t tab_test::getOutReg()
{
	return uint16_t();
}

uint16_t tab_test::getInReg()
{
	return uint16_t();
}

void tab_test::OnBnClickedBtnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
#if 0
	if (m_IsPortOpen == FALSE)
	{
		CString port_name;//_T("COM11");
		m_comboComList.GetLBText(m_comboComList.GetCurSel(), port_name.GetBuffer(1024));
		port_name.ReleaseBuffer();

		DWORD_PTR baudrate = m_comboBaudrateList.GetItemData(m_comboBaudrateList.GetCurSel());
		WORD port_id = m_comboComList.GetCurSel();


		CWnd* pWnd = AfxGetMainWnd();
		HWND hCommWnd = pWnd->m_hWnd;
		uint32_t result = m_rs232Port->OpenPort(port_name, port_id, (DWORD)baudrate, hCommWnd);
		if (result != 0)
			AfxMessageBox(L"connect faliled");
		else
		{
			//AfxMessageBox(L"connect successed\n");
			m_NextionComm.baud = (uint32_t)baudrate;
			m_NextionComm.is_init = true;
			m_IsPortOpen = TRUE;
			GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("CLOSE"));
		}
	}
	else
	{
		m_rs232Port->ClosePort();
		m_NextionComm.is_init = false;
		m_IsPortOpen = FALSE;
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("OPEN"));
	}
#endif
}


BOOL tab_test::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	jL_system* system = theApp.GetSystem();
	jL_ioManager* p_io = (jL_ioManager*)system->GetIOComponent();

	jL_CommSerial comm;

	comm.AddStringPortList(&m_comboComList);
	comm.AddStringBaudrateList(&m_comboBaudrateList);

	return TRUE;  // return TRUE unless you set the focus to a control
								// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void tab_test::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (m_TimerID != 0)
	{
		KillTimer(m_TimerID);
		m_TimerID = 0;
	}
}
