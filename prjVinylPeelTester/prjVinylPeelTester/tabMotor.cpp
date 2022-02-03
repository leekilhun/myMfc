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
  , m_receiveValue(_T("")), m_cpState(nullptr), m_pDlgPop(nullptr), m_workTimer()
  , m_progressCnt(0)
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
  if (m_pDlgPop != nullptr)
  {
    delete m_pDlgPop;
  }
  m_pDlgPop = nullptr;
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

  DDX_Control(pDX, IDC_LED_SERVO_ON, m_LedServoOn);

  DDX_Control(pDX, IDC_PROGRESS_RCV_DATA, m_progressRcvData);
  DDX_Control(pDX, IDC_COMBO_ORGCMD_TYPE, m_orgMethod);
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
  ON_BN_CLICKED(IDC_BTN_POP_REG, &tabMotor::OnBnClickedBtnPopReg)
  ON_BN_CLICKED(IDC_BTN_JOG_MINUS, &tabMotor::OnBnClickedBtnJogMinus)
  ON_BN_CLICKED(IDC_BTN_JOG_PLUS, &tabMotor::OnBnClickedBtnJogPlus)
  ON_BN_CLICKED(IDC_BTN_LIMIT_MINUS, &tabMotor::OnBnClickedBtnLimitMinus)
  ON_BN_CLICKED(IDC_BTN_LIMIT_PLUS, &tabMotor::OnBnClickedBtnLimitPlus)
  ON_MESSAGE(WM_FASTECH_MOTOR_CMD_MESSAGE, &tabMotor::OnFastechMotorCmdMessage)
  ON_BN_CLICKED(IDC_BTN_MOTOR_STOP, &tabMotor::OnBnClickedBtnMotorStop)
  ON_BN_CLICKED(IDC_BTN_MOTOR_ESTOP2, &tabMotor::OnBnClickedBtnMotorEstop2)
  ON_BN_CLICKED(IDC_BTN_POS_RESET, &tabMotor::OnBnClickedBtnPosReset)
  ON_BN_CLICKED(IDC_CHECK_ISR_ONOFF, &tabMotor::OnBnClickedCheckIsrOnoff)
  ON_BN_CLICKED(IDC_BT_MCMD_MOVE2, &tabMotor::OnBnClickedBtMcmdMove2)
  ON_BN_CLICKED(IDC_BT_MCMD_DEC_MOVE, &tabMotor::OnBnClickedBtMcmdDecMove)
  ON_BN_CLICKED(IDC_BT_MCMD_INC_MOVE, &tabMotor::OnBnClickedBtMcmdIncMove)
  ON_BN_CLICKED(IDC_BT_ORMCMD_START, &tabMotor::OnBnClickedBtOrmcmdStart)
  ON_WM_CTLCOLOR()
  ON_MESSAGE(WM_FASTECH_MOTOR_REG_UPDATE_MESSAGE, &tabMotor::OnFastechMotorRegUpdateMessage)
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
  updateReg();  

  // check work state
  if (m_workTimer.IsStarted())
  {
    if (m_workTimer.MoreThan(1000))
    {
      if (m_cpState->axis_status[REG_BANK(FM_STATE_02)][REG_BIT(REG_MOTIONING)])
        OnBnClickedBtnMotorStop();
      m_workTimer.Stop();
    }
  }

  CDialogEx::OnTimer(nIDEvent);
}


BOOL tabMotor::OnInitDialog()
{
  CDialogEx::OnInitDialog();

  // TODO:  여기에 추가 초기화 작업을 추가합니다.

  m_ToolTip.Create(this);
  m_ToolTip.AddTool(&m_LedServoOn, IDS_TT_LED1);

  //Set images
  m_LedServoOn.SetImage(IDB_GREENBUTTON, 15);
  //m_Btn2.SetImage(IDB_REDBUTTON, 15);

  // set the initial state of buttons
  m_LedServoOn.Depress(false);


  m_progressRcvData.SetRange(0, 100);
  m_progressRcvData.SetPos(0);

  ((CButton*)GetDlgItem(IDC_CHECK_ISR_ONOFF))->SetCheck(true);

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


  GetDlgItem(IDC_MOVECMD_POS)->SetWindowText(_T("1000"));
  GetDlgItem(IDC_MOVECMD_START_SPEED)->SetWindowText(_T("0"));
  GetDlgItem(IDC_MOVECMD_MOVE_SPEED)->SetWindowText(_T("0"));
  GetDlgItem(IDC_MOVECMD_ACCEL_TIME)->SetWindowText(_T("0"));
  GetDlgItem(IDC_MOVECMD_DECEL_TIME)->SetWindowText(_T("0"));

  GetDlgItem(IDC_ORGCMD_SEARCHSPEED)->SetWindowText(_T("0"));
  GetDlgItem(IDC_ORGCMD_SPEED)->SetWindowText(_T("0"));
  GetDlgItem(IDC_ORGCMD_ACCELDECEL)->SetWindowText(_T("0"));

  CComboBox* org_method_combo = (CComboBox*)GetDlgItem(IDC_COMBO_ORGCMD_TYPE);

  int idx = org_method_combo->AddString(L"Z Origin");
  org_method_combo->SetItemData(idx, 1);
  idx = org_method_combo->AddString(L"Limit Origin");
  org_method_combo->SetItemData(idx, 2);
  idx = org_method_combo->AddString(L"Torque Origin");
  org_method_combo->SetItemData(idx, 3);
  idx = org_method_combo->AddString(L"Torque Z Org");
  org_method_combo->SetItemData(idx, 4);
  

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
    uint32_t result = m_pMotor->PortOpen(port_name, (DWORD)baudrate, port_id, hCommWnd);
    if (result != 0)
      AfxMessageBox(L"connect faliled");
    else
    {
      //AfxMessageBox(L"connect successed\n");
      //m_NextionComm.baud = (uint32_t)baudrate;
      //m_NextionComm.is_init = true;
      m_IsPortOpen = TRUE;
      GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("CLOSE"));
      //m_pMotor->LoadingParamterData();
    }
  }
  else
  {
    m_pMotor->PortClose();
    //m_NextionComm.is_init = false;
    m_IsPortOpen = FALSE;
    GetDlgItem(IDC_BTN_CONNECT)->SetWindowText(_T("OPEN"));
  }
  m_progressCnt = 0;
  m_progressRcvData.SetRange(0, 100);

}


afx_msg LRESULT tabMotor::OnFastechMotorSerialCommMessage(WPARAM wParam, LPARAM lParam)
{
  uint32_t addr = (uint32_t)lParam;
  if (!m_pMotor->m_pComm->m_log->Available())
    return -1;
  std::string log_str = m_pMotor->m_pComm->m_log->GetLog();//PopLog(addr);
  TCHAR ret_msg[255] = { 0, };
  trans::CharToLPTSTR((LPSTR)log_str.c_str(), &ret_msg[0], 255);

  if (m_recvList.GetCount() == 24)
    m_recvList.ResetContent();

  m_recvList.AddString(ret_msg);

  /*
  uint32_t addr_data = (uint32_t)wParam;
  std::string data_str = m_pMotor->m_pComm->m_log->GetLog(); //PopLog(addr_data);
  TCHAR ret_data[255] = { 0, };
  trans::CharToLPTSTR((LPSTR)data_str.c_str(), &ret_data[0], 255);

  // edit-box에 데이터 넣는 방법 3가지 중 하나 선택
  m_receiveValue = ret_data;
  //m_eboxReceiveData.SetWindowText(ret_msg);
  //SetDlgItemText(IDC_EDIT_RCV_DATA, ret_msg);
  */
  return 0;
}

void tabMotor::updateReg()
{ 



  if (m_pMotor->m_motorState.is_motor_on)
  {
    GetDlgItem(IDC_BTN_MOTOR_ONOFF)->SetWindowText(L"Servo On");
    m_cpState->is_motor_on = true;
    m_LedServoOn.Depress(true);
  }
  else
  {
    GetDlgItem(IDC_BTN_MOTOR_ONOFF)->SetWindowText(L"Servo Off");
    m_cpState->is_motor_on = false;
    m_LedServoOn.Depress(false);
  }

  CString value;
  value.Format(L"%d", m_cpState->cmd_pos);
  GetDlgItem(IDC_ED_CMD_POS)->SetWindowTextW(value);

  value.Format(L"%d", m_cpState->act_pos);
  GetDlgItem(IDC_ED_ACT_POS)->SetWindowTextW(value);

  value.Format(L"%d", m_cpState->act_vel);
  GetDlgItem(IDC_ED_ACT_VEL)->SetWindowTextW(value);

  value.Format(L"%d", m_cpState->pos_err);
  GetDlgItem(IDC_ED_POS_ERR)->SetWindowTextW(value);

  

  value.Format(L"응답시간 (%dms)", m_pMotor->m_responseTime);
  GetDlgItem(IDC_ST_RESP_MS)->SetWindowTextW(value);

}


void tabMotor::OnLbnDblclkListReceiPack()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  m_recvList.ResetContent();
}


void tabMotor::OnBnClickedBtnSendCmd()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

  if (m_IsPortOpen != true)
  {
    AfxMessageBox(L"Port not opened!");
    return;
  }
  uint32_t cmd = (uint32_t)m_comboCmdList.GetItemData(m_comboCmdList.GetCurSel());
  CString id_str;
  GetDlgItem(IDC_ED_ID)->GetWindowText(id_str);
  int nId = GetDlgItemInt(IDC_ED_ID);

  m_pMotor->SetMotorId(_ttoi(id_str));

  m_pMotor->SendCommand(cmd);
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
  if (m_pMotor->m_motorState.is_motor_on)
    m_pMotor->MotorOnOff(false);
  else
    m_pMotor->MotorOnOff(true);

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
  m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::ALARAM_RESET));

}


BOOL tabMotor::DestroyWindow()
{
  // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

  return CDialogEx::DestroyWindow();
  if (m_pDlgPop != nullptr)
    delete m_pDlgPop;
  m_pDlgPop = nullptr;
}


void tabMotor::OnBnClickedBtnPopReg()
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
}


BOOL tabMotor::PreTranslateMessage(MSG* pMsg)
{
  // TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
  MSG msg = *pMsg;
  msg.hwnd = (HWND)m_ToolTip.SendMessage(TTM_WINDOWFROMPOINT, 0, (LPARAM)&msg.pt);
  CPoint pt = pMsg->pt;
  if (msg.message >= WM_MOUSEFIRST && msg.message <= WM_MOUSELAST)
    ::ScreenToClient(msg.hwnd, &pt);
  msg.lParam = MAKELONG(pt.x, pt.y);

  // now let the tooltip handle the event
  m_ToolTip.RelayEvent(&msg);



  //  key event 
  CString msg_str;

  if (pMsg->message == WM_KEYDOWN)
  {
    switch (pMsg->wParam)
    {
    case VK_RETURN:
    {
      byte data[5] = { 0, };
      if (pMsg->hwnd == GetDlgItem(IDC_MOVECMD_ACCEL_TIME)->m_hWnd){
        msg_str.Format(L"변경된 값-%d(Move accel time)을 저장하시겠습니까?", GetDlgItemInt(IDC_MOVECMD_ACCEL_TIME));
        if (IDYES == AfxMessageBox(msg_str, MB_YESNO))
        {
          int value = GetDlgItemInt(IDC_MOVECMD_ACCEL_TIME);
          data[0] = static_cast<uint16_t>(fm_parameter::param::axis_acc_time);
          data[1] = byte((value >> 0) & 0x000000ff);
          data[2] = byte((value >> 8) & 0x000000ff);
          data[3] = byte((value >> 16) & 0x000000ff);
          data[4] = byte((value >> 24) & 0x000000ff);

          m_pMotor->m_pComm->m_checkSync.keep_id = data[0];
          m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::SET_RAM_PARAM), &data[0], 5);
        }
        return true;
      }

      if (pMsg->hwnd == GetDlgItem(IDC_MOVECMD_DECEL_TIME)->m_hWnd) {
        msg_str.Format(L"변경된 값-%d(Move decel time)을 저장하시겠습니까?", GetDlgItemInt(IDC_MOVECMD_DECEL_TIME));
        if (IDYES == AfxMessageBox(msg_str, MB_YESNO))
        {
          int value = GetDlgItemInt(IDC_MOVECMD_DECEL_TIME);
          data[0] = static_cast<uint16_t>(fm_parameter::param::axis_dec_time);
          data[1] = byte((value >> 0) & 0x000000ff);
          data[2] = byte((value >> 8) & 0x000000ff);
          data[3] = byte((value >> 16) & 0x000000ff);
          data[4] = byte((value >> 24) & 0x000000ff);

          m_pMotor->m_pComm->m_checkSync.keep_id = data[0];
          m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::SET_RAM_PARAM), &data[0], 5);
        }
        return true;
      }

      if (pMsg->hwnd == GetDlgItem(IDC_ED_JOG_MAXSPEED)->m_hWnd) {
        msg_str.Format(L"변경된 값-%d(Jog max speed)을 저장하시겠습니까?", GetDlgItemInt(IDC_ED_JOG_MAXSPEED));
        if (IDYES == AfxMessageBox(msg_str, MB_YESNO))
        {
          int value = GetDlgItemInt(IDC_ED_JOG_MAXSPEED);
          data[0] = static_cast<uint16_t>(fm_parameter::param::jog_speed);
          data[1] = byte((value >> 0) & 0x000000ff);
          data[2] = byte((value >> 8) & 0x000000ff);
          data[3] = byte((value >> 16) & 0x000000ff);
          data[4] = byte((value >> 24) & 0x000000ff);

          m_pMotor->m_pComm->m_checkSync.keep_id = data[0];
          m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::SET_RAM_PARAM), &data[0], 5);
        }
        return true;
      }

      if (pMsg->hwnd == GetDlgItem(IDC_ED_JOG_ACCELDECEL)->m_hWnd) {
        msg_str.Format(L"변경된 값-%d(Jog acc/decel time)을 저장하시겠습니까?", GetDlgItemInt(IDC_ED_JOG_ACCELDECEL));
        if (IDYES == AfxMessageBox(msg_str, MB_YESNO))
        {
          int value = GetDlgItemInt(IDC_ED_JOG_ACCELDECEL);
          data[0] = static_cast<uint16_t>(fm_parameter::param::jog_acc_dec_time);
          data[1] = byte((value >> 0) & 0x000000ff);
          data[2] = byte((value >> 8) & 0x000000ff);
          data[3] = byte((value >> 16) & 0x000000ff);
          data[4] = byte((value >> 24) & 0x000000ff);

          m_pMotor->m_pComm->m_checkSync.keep_id = data[0];
          m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::SET_RAM_PARAM), &data[0], 5);
        }
        return true;
      }

    }
    break;
    default:
      break;
    }
  }

  return CDialogEx::PreTranslateMessage(pMsg);
}

//
//void tabMotor::OnBnClickedLedBtn1()
//{
//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	if (m_LedServoOn.IsDepressed())
//		m_LedServoOn.Depress(false);
//	m_LedServoOn.Depress(true);
//}


void tabMotor::OnBnClickedBtnJogMinus()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  if (m_IsPortOpen != true)
  {
    AfxMessageBox(L"Port not opened!");
    return;
  }

  if (m_pMotor->m_motorState.is_motor_on)
  {
    if (m_cpState->axis_status[REG_BANK(FM_STATE_02)][REG_BIT(REG_MOTIONING)])
    {
      OnBnClickedBtnMotorStop();
      Sleep(100);
    }     

    byte data[5] = { 0, };
    int max_speed = m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::jog_speed));

    data[0] = byte((max_speed >> 0) & 0x000000ff);
    data[1] = byte((max_speed >> 8) & 0x000000ff);
    data[2] = byte((max_speed >> 16) & 0x000000ff);
    data[3] = byte((max_speed >> 24) & 0x000000ff);
    data[4] = 0x00;
    m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::MOVE_VELOCITY), &data[0], 5);
    m_workTimer.Start();
  }
}


void tabMotor::OnBnClickedBtnJogPlus()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  if (m_IsPortOpen != true)
  {
    AfxMessageBox(L"Port not opened!");
    return;
  }

  if (m_pMotor->m_motorState.is_motor_on)
  {
    if (m_cpState->axis_status[REG_BANK(FM_STATE_02)][REG_BIT(REG_MOTIONING)])
    {
      OnBnClickedBtnMotorStop();
      Sleep(100);
    }

    byte data[5] = { 0, };
    int max_speed = m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::jog_speed));

    data[0] = byte((max_speed >> 0) & 0x000000ff); 
    data[1] = byte((max_speed >> 8) & 0x000000ff);
    data[2] = byte((max_speed >> 16) & 0x000000ff);
    data[3] = byte((max_speed >> 24) & 0x000000ff);
    data[4] = 0x01;
    m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::MOVE_VELOCITY), &data[0], 5);
    m_workTimer.Start();
  }
}


void tabMotor::OnBnClickedBtnLimitMinus()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  if (m_IsPortOpen != true)
  {
    AfxMessageBox(L"Port not opened!");
    return;
  }
}


void tabMotor::OnBnClickedBtnLimitPlus()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  if (m_IsPortOpen != true)
  {
    AfxMessageBox(L"Port not opened!");
    return;
  }
}


afx_msg LRESULT tabMotor::OnFastechMotorCmdMessage(WPARAM wParam, LPARAM lParam)
{
  int progress = (int)lParam;
  if (progress == 0)
    m_progressRcvData.SetRange(0, (short)wParam);
  m_progressRcvData.SetPos(++m_progressCnt);
  CString str;
  str.Format(L"progress %d/%d", m_progressCnt, (short)wParam);
  GetDlgItem(IDC_STATIC_ING)->SetWindowText(str);

  return 0;
}


void tabMotor::OnBnClickedBtnMotorStop()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  if (m_IsPortOpen != true)
  {
    AfxMessageBox(L"Port not opened!");
    return;
  }

  m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::MOVE_STOP));

}


void tabMotor::OnBnClickedBtnMotorEstop2()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  if (m_IsPortOpen != true)
  {
    AfxMessageBox(L"Port not opened!");
    return;
  }

  m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::MOVE_EMG_STOP));
}


void tabMotor::OnBnClickedBtnPosReset()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  if (m_IsPortOpen != true)
  {
    AfxMessageBox(L"Port not opened!");
    return;
  }

  m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::CLEAR_POSITION));
}


void tabMotor::OnBnClickedCheckIsrOnoff()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  if (((CButton*)GetDlgItem(IDC_CHECK_ISR_ONOFF))->GetCheck()) {
    m_pMotor->SetUseISR(true);    
  }
  else {
    m_pMotor->SetUseISR(false);

  }
}


void tabMotor::OnBnClickedBtMcmdMove2()
{
    // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  if (m_IsPortOpen != true)
  {
    AfxMessageBox(L"Port not opened!");
    return;
  }

  byte data[8] = { 0, };
  int speed = GetDlgItemInt(IDC_MOVECMD_MOVE_SPEED);
  int position = GetDlgItemInt(IDC_MOVECMD_POS);

  data[0] = byte((position >> 0) & 0x000000ff);
  data[1] = byte((position >> 8) & 0x000000ff);
  data[2] = byte((position >> 16) & 0x000000ff);
  data[3] = byte((position >> 24) & 0x000000ff);

  data[4] = byte((speed >> 0) & 0x000000ff);
  data[5] = byte((speed >> 8) & 0x000000ff);
  data[6] = byte((speed >> 16) & 0x000000ff);
  data[7] = byte((speed >> 24) & 0x000000ff);

  m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::MOVE_ABS_SINGLE_AXIS), &data[0], 8);

}


void tabMotor::OnBnClickedBtMcmdDecMove()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

  byte data[8] = { 0, };
  int speed = GetDlgItemInt(IDC_MOVECMD_MOVE_SPEED);
  int position = GetDlgItemInt(IDC_MOVECMD_POS)*-1;

  data[0] = byte((position >> 0) & 0x000000ff);
  data[1] = byte((position >> 8) & 0x000000ff);
  data[2] = byte((position >> 16) & 0x000000ff);
  data[3] = byte((position >> 24) & 0x000000ff);

  data[4] = byte((speed >> 0) & 0x000000ff);
  data[5] = byte((speed >> 8) & 0x000000ff);
  data[6] = byte((speed >> 16) & 0x000000ff);
  data[7] = byte((speed >> 24) & 0x000000ff);

  m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::MOVE_REL_SINGLE_AXIS), &data[0], 8);
}


void tabMotor::OnBnClickedBtMcmdIncMove()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  byte data[8] = { 0, };
  int speed = GetDlgItemInt(IDC_MOVECMD_MOVE_SPEED);
  int position = GetDlgItemInt(IDC_MOVECMD_POS);

  data[0] = byte((position >> 0) & 0x000000ff);
  data[1] = byte((position >> 8) & 0x000000ff);
  data[2] = byte((position >> 16) & 0x000000ff);
  data[3] = byte((position >> 24) & 0x000000ff);

  data[4] = byte((speed >> 0) & 0x000000ff);
  data[5] = byte((speed >> 8) & 0x000000ff);
  data[6] = byte((speed >> 16) & 0x000000ff);
  data[7] = byte((speed >> 24) & 0x000000ff);

  m_pMotor->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::MOVE_REL_SINGLE_AXIS), &data[0], 8);
}


void tabMotor::OnBnClickedBtOrmcmdStart()
{
  // TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
  int index = m_orgMethod.GetCurSel();
  CString data;
  m_orgMethod.GetLBText(m_orgMethod.GetCurSel(), data.GetBuffer(1024));
  data.ReleaseBuffer();
  int get_data = 0;
  get_data = m_orgMethod.GetItemData(index);
  
  return;
}




HBRUSH tabMotor::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
  HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

  // TODO:  여기서 DC의 특성을 변경합니다.
  if (pWnd->GetDlgCtrlID() == IDC_ED_CMD_POS)
  {
    pDC->SetBkColor(RGB(252, 248, 233));
    hbr = ::CreateSolidBrush(RGB(252, 248, 233));
  }

  if (pWnd->GetDlgCtrlID() == IDC_ED_ACT_POS)
  {
    pDC->SetBkColor(RGB(252, 248, 233));
    hbr = ::CreateSolidBrush(RGB(252, 248, 233));
  }

  if (pWnd->GetDlgCtrlID() == IDC_ED_POS_ERR)
  {
    pDC->SetBkColor(RGB(252, 248, 233));
    hbr = ::CreateSolidBrush(RGB(252, 248, 233));
  }

  if (pWnd->GetDlgCtrlID() == IDC_ED_ACT_VEL)
  {
    pDC->SetBkColor(RGB(252, 248, 233));
    hbr = ::CreateSolidBrush(RGB(252, 248, 233));
  }
  // TODO:  기본값이 적당하지 않으면 다른 브러시를 반환합니다.
  return hbr;
}



afx_msg LRESULT tabMotor::OnFastechMotorRegUpdateMessage(WPARAM wParam, LPARAM lParam)
{
  jL_fastechComm::fastech_cmdList cmd_index = static_cast<jL_fastechComm::fastech_cmdList>(lParam);

  switch (cmd_index)
  {
  case jL_fastechComm::fastech_cmdList::GET_SLAVE_INFO:
    break;
  case jL_fastechComm::fastech_cmdList::GET_MOTOR_INFO:
    break;
  case jL_fastechComm::fastech_cmdList::SAVE_ROM_ALL:
    break;
  case jL_fastechComm::fastech_cmdList::GET_ROM_PARAM:
    break;
  case jL_fastechComm::fastech_cmdList::SET_RAM_PARAM:
    break;
  case jL_fastechComm::fastech_cmdList::GET_RAM_PARAM:
  {
    CString value;
    value.Format(L"%d", m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::jog_speed)));
    GetDlgItem(IDC_ED_JOG_MAXSPEED)->SetWindowTextW(value);

    value.Format(L"%d", m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::jog_acc_dec_time)));
    GetDlgItem(IDC_ED_JOG_ACCELDECEL)->SetWindowTextW(value);

    value.Format(L"%d", m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::axis_acc_time)));
    GetDlgItem(IDC_MOVECMD_ACCEL_TIME)->SetWindowTextW(value);

    value.Format(L"%d", m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::axis_dec_time)));
    GetDlgItem(IDC_MOVECMD_DECEL_TIME)->SetWindowTextW(value);

    value.Format(L"%d", m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::axis_max_speed)));
    GetDlgItem(IDC_MOVECMD_MOVE_SPEED)->SetWindowTextW(value);

    value.Format(L"%d", m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::axis_start_speed)));
    GetDlgItem(IDC_MOVECMD_START_SPEED)->SetWindowTextW(value);

    value.Format(L"%d", m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::org_search_speed)));
    GetDlgItem(IDC_ORGCMD_SEARCHSPEED)->SetWindowTextW(value);

    value.Format(L"%d", m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::org_speed)));
    GetDlgItem(IDC_ORGCMD_SPEED)->SetWindowTextW(value);

    value.Format(L"%d", m_pMotor->m_motorParam.Get(static_cast<uint16_t>(fm_parameter::param::org_acc_dec_time)));
    GetDlgItem(IDC_ORGCMD_ACCELDECEL)->SetWindowTextW(value);    

  }
  break;
  case jL_fastechComm::fastech_cmdList::SET_IO_OUT_REG:
    break;
  case jL_fastechComm::fastech_cmdList::SET_IO_IN_REG:
    break;
  case jL_fastechComm::fastech_cmdList::GET_IO_IN_REG:
    break;
  case jL_fastechComm::fastech_cmdList::GET_IO_OUT_REG:
    break;
  case jL_fastechComm::fastech_cmdList::SET_ASSN_IO_MAP:
    break;
  case jL_fastechComm::fastech_cmdList::GET_ASSN_IO_MAP:
    break;
  case jL_fastechComm::fastech_cmdList::READ_IO_MAP:
    break;
  case jL_fastechComm::fastech_cmdList::TRG_OUT_RUN_A:
    break;
  case jL_fastechComm::fastech_cmdList::TRG_OUT_STATUS:
    break;
  case jL_fastechComm::fastech_cmdList::SERVO_ENABLE:
    break;
  case jL_fastechComm::fastech_cmdList::ALARAM_RESET:
    break;
  case jL_fastechComm::fastech_cmdList::GET_ALARM_INFO:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_STOP:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_EMG_STOP:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_ORG_SINGLE_AXIS:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_ABS_SINGLE_AXIS:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_REL_SINGLE_AXIS:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_TO_LIMIT:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_VELOCITY:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_ABS_OVERRIDE:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_REL_OVERRIDE:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_VELOCITY_OVERRIDE:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_ALL_STOP:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_ALL_EMG_STOP:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_ALL_ORG_STOP:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_ALL_ABS_POS:
    break;
  case jL_fastechComm::fastech_cmdList::MOVE_ALL_REL_POS:
    break;
  case jL_fastechComm::fastech_cmdList::GET_AXIS_STATUS:
    break;
  case jL_fastechComm::fastech_cmdList::GET_AXIS_IO_STATUS:
    break;
  case jL_fastechComm::fastech_cmdList::GET_MOTION_STATUS:
    break;
  case jL_fastechComm::fastech_cmdList::CLEAR_POSITION:
    break;
  case jL_fastechComm::fastech_cmdList::_max:
    break;
  default:
    break;
  }

  return 0;
}
