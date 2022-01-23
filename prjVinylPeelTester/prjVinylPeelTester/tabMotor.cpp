// tabMotor.cpp: 구현 파일
//

#include "pch.h"
#include "prjVinylPeelTester.h"
#include "tabMotor.h"
#include "afxdialogex.h"

//#include "jL_fastechComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// tabMotor 대화 상자

IMPLEMENT_DYNAMIC(tabMotor, CDialogEx)

#define UPDATE_TIMER_DLG 2  // timer id

tabMotor::tabMotor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_MOTOR, pParent)
	, m_TimerID(0), m_pMotor(nullptr), m_IsPortOpen(false)
	, m_receiveValue(_T("")), m_cpState(nullptr), m_pDlgPop(nullptr)
{
	//m_receiveValue = L"첫 번째 줄 \r\n두 번째 줄";
}

tabMotor::~tabMotor()
{
	if (m_TimerID != 0)
	{
		KillTimer(m_TimerID);
		m_TimerID = 0;
	}

	m_pMotor = nullptr;
}

void tabMotor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PortNo, m_comboComList);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBaudrateList);
	DDX_Control(pDX, IDC_LIST_RECEI_PACK, m_recvList);
	DDX_Control(pDX, IDC_COMBO1, m_comboCmdList);
	DDX_Control(pDX, IDC_EDIT_RCV_DATA, m_eboxReceiveData);

	DDX_Text(pDX, IDC_EDIT_RCV_DATA, m_receiveValue);
}


BEGIN_MESSAGE_MAP(tabMotor, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &tabMotor::OnBnClickedBtnConnect)
	ON_MESSAGE(WM_FASTECH_MOTOR_SERIAL_COMM_MESSAGE, &tabMotor::OnFastechMotorSerialCommMessage)
	ON_LBN_DBLCLK(IDC_LIST_RECEI_PACK, &tabMotor::OnLbnDblclkListReceiPack)
	ON_BN_CLICKED(IDC_BTN_SEND_CMD, &tabMotor::OnBnClickedBtnSendCmd)
	ON_BN_CLICKED(IDC_BTN_MOTOR_ONOFF, &tabMotor::OnBnClickedBtnMotorOnoff)
	ON_EN_CHANGE(IDC_ED_ID, &tabMotor::OnEnChangeEdId)
	ON_BN_CLICKED(IDC_BTN_MOTOR_ALARM_RESET, &tabMotor::OnBnClickedBtnMotorAlarmReset)
END_MESSAGE_MAP()


// tabMotor 메시지 처리기


void tabMotor::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow == TRUE)
	{
		m_TimerID = SetTimer(UPDATE_TIMER_DLG, 100, NULL);
	
		m_cpState->id = GetDlgItemInt(IDC_ED_ID);
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


void tabMotor::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);
}


BOOL tabMotor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_ED_ID)->SetWindowText(_T("0"));
	GetDlgItem(IDC_ED_STATE)->SetWindowText(_T("none"));


	jL_CommSerial comm;

	comm.AddStringPortList(&m_comboComList);
	comm.AddStringBaudrateList(&m_comboBaudrateList);

	jL_system* system = theApp.GetSystem();
	m_pMotor = (jL_fastechMotor*)system->GetMotorComponent();

	m_pMotor->m_pComm->AddStringCmdList(&m_comboCmdList);

	m_comboCmdList.SetCurSel(0);

	m_cpState = m_pMotor->GetMotorState();


	return TRUE;  // return TRUE unless you set the focus to a control
								// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void tabMotor::OnBnClickedBtnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_pMotor->PortOpen()
	if (m_IsPortOpen == FALSE)
	{
		CString port_name;//_T("COM11");
		m_comboComList.GetLBText(m_comboComList.GetCurSel(), port_name.GetBuffer(1024));
		port_name.ReleaseBuffer();

		DWORD_PTR baudrate = m_comboBaudrateList.GetItemData(m_comboBaudrateList.GetCurSel());
		WORD port_id = m_comboComList.GetCurSel();


		CWnd* pWnd = AfxGetMainWnd();
		HWND hCommWnd = this->m_hWnd;//pWnd->m_hWnd;
		uint32_t result = m_pMotor->PortOpen(port_name, port_id, (WORD)baudrate, hCommWnd);
		if (result != 0)
			AfxMessageBox(L"connect faliled");
		else
		{
			//AfxMessageBox(L"connect successed\n");
			//m_NextionComm.baud = (uint32_t)baudrate;
			//m_NextionComm.is_init = true;
			m_IsPortOpen = TRUE;
			GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("CLOSE"));
		}
	}
	else
	{
		m_pMotor->PortClose();
		//m_NextionComm.is_init = false;
		m_IsPortOpen = FALSE;
		GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("OPEN"));
	}


}


afx_msg LRESULT tabMotor::OnFastechMotorSerialCommMessage(WPARAM wParam, LPARAM lParam)
{
	uint32_t addr = (uint32_t)lParam;
	std::string log_str = m_pMotor->m_pComm->m_log->PopLog(addr);
	TCHAR ret_msg[255] = { 0, };
	trans::CharToLPTSTR((LPSTR)log_str.c_str(), &ret_msg[0], 255);

	uint32_t addr_data = (uint32_t)wParam;
	std::string data_str = m_pMotor->m_pComm->m_log->PopLog(addr_data);
	TCHAR ret_data[255] = { 0, };
	trans::CharToLPTSTR((LPSTR)data_str.c_str(), &ret_data[0], 255);
	

	if (m_recvList.GetCount() == 24)
		m_recvList.ResetContent();

	m_recvList.AddString(ret_msg);

	// edit-box에 데이터 넣는 방법 3가지 중 하나 선택
	m_receiveValue = ret_data;
	/*m_eboxReceiveData.SetWindowText(ret_msg);
	SetDlgItemText(IDC_EDIT_RCV_DATA, ret_msg);*/

	return 0;
}


void tabMotor::OnLbnDblclkListReceiPack()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_recvList.ResetContent();
}


void tabMotor::OnBnClickedBtnSendCmd()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pDlgPop == nullptr)
	{
		m_pDlgPop = new popDlg_axisStatus(this);
		m_pDlgPop->Create(IDD_POPDLG_STATUS, this);
		m_pDlgPop->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgPop->ShowWindow(SW_SHOW);
	}





	if (m_IsPortOpen != true)
	{
		AfxMessageBox(L"Port not opened!");
		return;
	}
	uint32_t cmd = (uint32_t)m_comboCmdList.GetItemData(m_comboCmdList.GetCurSel());
	CString id_str;
	GetDlgItem(IDC_ED_ID)->GetWindowText(id_str);
	int nId = GetDlgItemInt(IDC_ED_ID);



	m_pMotor->SendCommand(cmd, _ttoi(id_str));
	return;
}


void tabMotor::OnBnClickedBtnMotorOnoff()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_IsPortOpen != true)
	{
		AfxMessageBox(L"Port not opened!");
		return;
	}

	if (m_cpState->is_motor_on)
	{
		GetDlgItem(IDC_BTN_MOTOR_ONOFF)->SetWindowText(L"Servo Off");
		m_cpState->is_motor_on = false;
		m_pMotor->MotorOnOff(false);
	}
	else
	{
		GetDlgItem(IDC_BTN_MOTOR_ONOFF)->SetWindowText(L"Servo On");
		m_cpState->is_motor_on = true;
		m_pMotor->MotorOnOff(true);

	}


}


void tabMotor::OnEnChangeEdId()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_MotoeState.id = GetDlgItemInt(IDC_ED_ID);
	if (m_cpState == nullptr)
		return;
	m_cpState->id = GetDlgItemInt(IDC_ED_ID);
}


void tabMotor::OnBnClickedBtnMotorAlarmReset()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_IsPortOpen != true)
	{
		AfxMessageBox(L"Port not opened!");
		return;
	}
	int nId = GetDlgItemInt(IDC_ED_ID);
	m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::ALARAM_RESET), (uint8_t)nId);

}


BOOL tabMotor::DestroyWindow()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	return CDialogEx::DestroyWindow();
	if (m_pDlgPop != nullptr)
		delete m_pDlgPop;
	m_pDlgPop = nullptr;
}
