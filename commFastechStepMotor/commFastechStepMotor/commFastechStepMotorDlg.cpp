
// commFastechStepMotorDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "commFastechStepMotor.h"
#include "commFastechStepMotorDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CcommFastechStepMotorDlg 대화 상자

#define HJJL_RXCMD_TYPE_REQ_BEEP            0x00
#define HJJL_RXCMD_TYPE_WRITE_OUT_REG       0x03
#define HJJL_RXCMD_TYPE_CURR_PAGE_NO        0x04

#define CMD_STX1                    0x48
#define CMD_STX2                    0x4A
#define CMD_ETX1                    0x4A
#define CMD_ETX2                    0x4C

#define CMD_SEND_ETX                0xff 0xff 0xff

#define DEF_MCU_TO_OP_IN_REG_STR(x)        "cc_rcv_in=%d \r",x
#define DEF_MCU_TO_OP_OUT_REG_STR(x)       "cc_rcv_out=%d \r",x
#define DEF_MCU_TO_OP_RESET_LOG            "cc_rcv_out=%d \r"


#define UPDATE_TIMER_DLG 1

CcommFastechStepMotorDlg::CcommFastechStepMotorDlg(CWnd* pParent /*=nullptr*/)
	: m_IsPortOpen(NULL),m_rs232Port(nullptr), m_TimerID(0),
		m_CmdState (cmd_state::wait_stx),m_Checksum(0), m_input_reg(0), m_output_reg(0),
		CDialogEx(IDD_COMMFASTECHSTEPMOTOR_DIALOG, pParent)
{
	m_NextionComm = { 0, };
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CcommFastechStepMotorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PortNo, m_comboComList);
	DDX_Control(pDX, IDC_COMBO_BAUDRATE, m_comboBaudrateList);
	DDX_Control(pDX, IDC_ED_INPUT, m_editReceive);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_btnOpenClose);
	DDX_Control(pDX, IDC_LIST_CONT2, m_sendList);
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

	
}

BEGIN_MESSAGE_MAP(CcommFastechStepMotorDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CcommFastechStepMotorDlg::OnBnClickedBtnConnect)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_COMM_RS232_MESSAGE, &CcommFastechStepMotorDlg::OnRecivemessage)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CcommFastechStepMotorDlg 메시지 처리기

BOOL CcommFastechStepMotorDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	m_rs232Port = new jL_serialComPort;

	m_rs232Port->AddStringPortList(&m_comboComList);
	m_rs232Port->AddStringBaudrateList(&m_comboBaudrateList);


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CcommFastechStepMotorDlg::OnPaint()
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
HCURSOR CcommFastechStepMotorDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CcommFastechStepMotorDlg::OnBnClickedBtnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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

}


void CcommFastechStepMotorDlg::OnDestroy()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CDialogEx::OnDestroy();
	if (m_rs232Port != nullptr)
	{
		delete m_rs232Port;
	}
	m_rs232Port = nullptr;

	if (m_NextionComm.rx_packet.data != nullptr)
	{
		delete m_NextionComm.rx_packet.data;
	}
	m_NextionComm.rx_packet.data = nullptr;

	if (m_TimerID != 0)
	{
		KillTimer(m_TimerID);
		m_TimerID = 0;
	}
 
}


uint16_t CcommFastechStepMotorDlg::getOutReg()
{
	if (m_NextionComm.rx_packet.length == 2)
	{
		uint16_t value = 0;
		value = m_NextionComm.rx_packet.data[0];
		value |= m_NextionComm.rx_packet.data[1] << 8;
		m_output_reg = value;
		
		bool is_on = (value & 32768) >> 15;
		m_led[0].SetCheck(is_on);
		is_on = (value & 16384) >> 14;
		m_led[1].SetCheck(is_on);
		is_on = (value &  8192) >> 13;
		m_led[2].SetCheck(is_on);
		is_on = (value & 4096) >> 12;
		m_led[3].SetCheck(is_on);
		m_led[4].SetCheck((value & 2048) >> 11);
		m_led[5].SetCheck((value & 1024) >> 10);
		m_led[6].SetCheck((value & 512) >> 9);
		m_led[7].SetCheck((value & 256) >> 8);
	}
	else
	{
		m_output_reg = m_led[0].GetCheck() << 15
			| m_led[1].GetCheck() << 14
			| m_led[2].GetCheck() << 13
			| m_led[3].GetCheck() << 12
			| m_led[4].GetCheck() << 11
			| m_led[5].GetCheck() << 10
			| m_led[6].GetCheck() << 9
			| m_led[7].GetCheck() << 8;
	}
	return m_output_reg;
}


uint16_t CcommFastechStepMotorDlg::getInReg()
{
	m_input_reg = m_sensor[0].GetCheck() << 15
		| m_sensor[1].GetCheck() << 14
		| m_sensor[2].GetCheck() << 13
		| m_sensor[3].GetCheck() << 12
		| m_sensor[4].GetCheck() << 11
		| m_sensor[5].GetCheck() << 10
		| m_sensor[6].GetCheck() << 9
		| m_sensor[7].GetCheck() << 8;

	return m_input_reg;
}



void CcommFastechStepMotorDlg::updateReg(uint16_t reg, bool is_in/* = false*/)
{
	uint8_t temp[80] = { 0, };
	size_t len = 0;
	if (is_in)
	{
		sprintf_s((char*)temp, sizeof(temp), "cc_rcv_in=%d", reg);
		len = strnlen_s((char*)temp, 80);
		temp[len] = 0xff;
		temp[len + 1] = 0xff;
		temp[len + 2] = 0xff;
		temp[len + 3] = 0x00;
		temp[len + 4] = 0x00;
	}
	else
	{
		sprintf_s((char*)temp, sizeof(temp), "cc_rcv_out=%d", reg);
		len = strnlen_s((char*)temp, 80);
		temp[len] = 0xff;
		temp[len + 1] = 0xff;
		temp[len + 2] = 0xff;
		temp[len + 3] = 0x00;
		temp[len + 4] = 0x00;
	}
	
	m_rs232Port->WriteByte((BYTE*)&temp[0], (UINT)(len + 3));
}



afx_msg LRESULT CcommFastechStepMotorDlg::OnRecivemessage(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR return_w = _T("\r\n");
	BYTE carrige_return = _T('\r');
	BYTE change_line = _T('\n');
	TCHAR ret_msg[1024] = {};
	BYTE buffer[256] = { 0 };
	BYTE queue_byte = 0;
	int length = -1;
	int packet_size = 0;
	int qbuf_size = 0;	// 맴버 버퍼의 문자열의 길이
	char add_str[256] = { 0 };
	qbuf_size = m_Qbuf.Available();



	size_t len = 0;
	int add_cnt = 0;

	bool wait_next = false;
	bool ret = false;
	// 포트로 들어와 버퍼에 쌓인 데이터 크기
	length = m_rs232Port->m_QueueRead.GetSize();

	if (length == 0)
		goto exit_parsing;
	
	for (int i = 0; i < length; i++)
	{
		if (ret == true)
		{
			ret = false;
			switch (m_NextionComm.rx_packet.type)
			{
			case HJJL_RXCMD_TYPE_REQ_BEEP:
			{
				if (m_beep.GetCheck())
					m_beep.SetCheck(0);
				else
					m_beep.SetCheck(1);
			}
				break;
			case HJJL_RXCMD_TYPE_WRITE_OUT_REG:
			{
				updateReg(getInReg(), true);
				updateReg(getOutReg());
			}
				break;
			case HJJL_RXCMD_TYPE_CURR_PAGE_NO:
				break;

			}
		}
		m_rs232Port->m_QueueRead.GetByte((byte*)&queue_byte);
		switch (m_CmdState)
		{
		case cmd_state::wait_stx:
		{
			if (queue_byte == CMD_STX1)
			{
				packet_size = 0;
				wait_next = true;
				break;
			}
			if (wait_next)
			{
				wait_next = false;
				if (queue_byte == CMD_STX2)
				{
					m_CmdState = cmd_state::wait_page_no;
					m_NextionComm.rx_packet.check_sum = 0xffff;
					m_NextionComm.rx_packet.buffer[packet_size++] = CMD_STX1;
					m_NextionComm.rx_packet.buffer[packet_size++] = CMD_STX2;
				}
				else
				{
					m_CmdState = cmd_state::wait_stx;
				}
			}
		}
			break;
		case cmd_state::wait_page_no:
		{
			m_NextionComm.rx_packet.page_no = (nextionpage_t)queue_byte;
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, queue_byte);

			m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
			m_CmdState = cmd_state::wait_type;
		}
			break;
		case cmd_state::wait_type:
		{
			m_NextionComm.rx_packet.type = queue_byte;
			m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
			packet_size++;
			if (m_NextionComm.rx_packet.type == HJJL_RXCMD_TYPE_REQ_BEEP
				|| m_NextionComm.rx_packet.type == HJJL_RXCMD_TYPE_CURR_PAGE_NO)
			{
				m_CmdState = cmd_state::wait_stx;
				ret = true;
				break;
			}
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, queue_byte);
			m_CmdState = cmd_state::wait_obj_id;
		}
			break;
		case cmd_state::wait_obj_id:
		{
			m_NextionComm.rx_packet.obj_id = queue_byte;
			m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, queue_byte);
			m_CmdState = cmd_state::wiat_length_l;
		}
			break;
		case cmd_state::wiat_length_l:
		{
			m_NextionComm.rx_packet.length = queue_byte;
			m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, queue_byte);
			m_CmdState = cmd_state::wait_length_h;
		}
			break;
		case cmd_state::wait_length_h:
		{
			m_NextionComm.rx_packet.length |= (queue_byte << 8);
			m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, queue_byte);
			if (m_NextionComm.rx_packet.length > 0)
			{
				if (m_NextionComm.rx_packet.data != nullptr)
				{
					delete m_NextionComm.rx_packet.data;
					m_NextionComm.rx_packet.data = nullptr;
				}
				m_NextionComm.rx_packet.data = new uint8_t [m_NextionComm.rx_packet.length];
				m_NextionComm.index = 0;
				m_CmdState = cmd_state::wait_data; 
			}
			else
			{
				m_CmdState = cmd_state::wait_checksum;
			}			
		}
			break;
		case cmd_state::wait_data:
		{
			m_NextionComm.rx_packet.data[m_NextionComm.index] = queue_byte;
			m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
			
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, queue_byte);
			m_NextionComm.index++;
			if (m_NextionComm.index == m_NextionComm.rx_packet.length)
			{
				m_CmdState = cmd_state::wait_checksum;
			}
		}
			break;
		case cmd_state::wait_checksum:
		{
			if (wait_next)
			{
				wait_next = 0;
				m_NextionComm.rx_packet.check_sum_recv |= queue_byte << 8;
				m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
				m_CmdState = cmd_state::wait_etx;
			}
			else
			{
				wait_next = 1;
				m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
				m_NextionComm.rx_packet.check_sum_recv = queue_byte;
			}
		}
			break;
		case cmd_state::wait_etx:
		{
			if (queue_byte == CMD_ETX1)
			{
				wait_next = 1;
				m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
				break;
			}
			if (wait_next)
			{
				wait_next = 0;
				if (queue_byte == CMD_ETX2)
				{
					m_NextionComm.rx_packet.buffer[packet_size++] = queue_byte;
					if (m_NextionComm.rx_packet.check_sum == m_NextionComm.rx_packet.check_sum_recv)
					{
						ret = true;
					}
				}
			}
			m_CmdState = cmd_state::wait_stx;
		}
			break;
		}
	
	}

	if (ret == true)
	{
		CString rec_text;
		TCHAR rec_buff[80] = { 0, };
		char char_temp[80] = { 0, };
		//sprintf_s()
		/*sprintf_s(char_temp, sizeof(char_temp), "Packet :%02X %02X %02X %02X %02X %02X"
			, m_NextionComm.rx_packet.buffer[0]
			, m_NextionComm.rx_packet.buffer[1]
			, m_NextionComm.rx_packet.buffer[2]
			, m_NextionComm.rx_packet.buffer[3]
			, m_NextionComm.rx_packet.buffer[4]
			, m_NextionComm.rx_packet.buffer[5]);*/

		for (int i = 0; i < packet_size; i++)
		{
			sprintf_s(char_temp, sizeof(char_temp), "%s%02X "
				, char_temp
				, m_NextionComm.rx_packet.buffer[i]);
		}

 		trans::CharToLPTSTR((LPSTR)&char_temp[0], &rec_buff[0], 80);

		ret = false;
		switch (m_NextionComm.rx_packet.type)
		{
		case HJJL_RXCMD_TYPE_REQ_BEEP:
		{
			if (m_beep.GetCheck())
				m_beep.SetCheck(0);
			else
				m_beep.SetCheck(1);
		}
		break;
		case HJJL_RXCMD_TYPE_WRITE_OUT_REG:
		{			
			updateReg(getInReg(), true);
			updateReg(getOutReg());
		}
		break;
		case HJJL_RXCMD_TYPE_CURR_PAGE_NO:
			break;

		}

		rec_text.Format(L"Receive : %s \r\n", rec_buff);
		
		if (m_receiveList.GetCount() == 10)
			m_receiveList.ResetContent();
	
		m_receiveList.AddString(rec_text);
		m_editReceive.SetWindowText(rec_text);
	}

	// 데이터 파싱
#if 0
	for (int i = 0; i < length; i++)
	{
		m_rs232Port->m_QueueRead.GetByte((byte*)&queue_byte);

		len = (int)(strlen((const char*)buffer) + sizeof(char) * 2);
		if (queue_byte == change_line)
		{
			if (m_recvList.GetCount() == 1024)
				m_recvList.ResetContent();

			if (qbuf_size > 0)
			{
				m_Qbuf.Read((LPSTR)&add_str, qbuf_size);
				strncat_s(&add_str[0], (size_t)(qbuf_size + add_cnt), (const char*)&buffer[0], add_cnt);
				if (trans::CharToLPTSTR((char*)add_str, &ret_msg[0], 1024) > 0)
				{
					m_recvList.AddString(ret_msg);
				}
				//GetDlgItem(IDC_EDIT_RX)->SetWindowText(t::CharToCString((char*)&add_str[0]));
				memset(add_str, 0x00, 256);
			}
			else
			{
				if (trans::CharToLPTSTR((char*)buffer, &ret_msg[0], 1024) > 0)
				{
					m_recvList.AddString(ret_msg);
				}

				//GetDlgItem(IDC_EDIT_RX)->SetWindowText(t::CharToCString((char*)&buffer[0]));
			}
			m_recvList.SetTopIndex(m_recvList.GetCount() - 1);
			memset(buffer, 0x00, 256);
			add_cnt = 0;
		}
		else
			strncat_s((char*)buffer, (size_t)(len + (add_cnt++)), (const char*)&queue_byte, 1);

	}
#endif // 0


	// 처리하지 않은 문자열을 Q버퍼 저장
	/*len = (int)(strlen((const char*)buffer) + sizeof(char) * 2);
	if (strlen((const char*)buffer) > 0)
	{
		m_Qbuf.Write((const char*)buffer, len);
	}*/


	//updateReg(m_input_reg, true);
	//updateReg(getOutReg());



exit_parsing:

	return 0;
}


void CcommFastechStepMotorDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	updateReg(getOutReg());
	updateReg(getInReg(), true);
	CDialogEx::OnTimer(nIDEvent);
}


void CcommFastechStepMotorDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow == TRUE)
	{
		m_TimerID = SetTimer(UPDATE_TIMER_DLG, 100, NULL);
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

