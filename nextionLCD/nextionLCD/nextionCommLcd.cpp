#include "pch.h"
#include "nextionCommLcd.h"
#include "vector"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define HJJL_RXCMD_TYPE_REQ_BEEP            0x00
#define HJJL_RXCMD_TYPE_WRITE_OUT_REG       0x03
#define HJJL_RXCMD_TYPE_CURR_PAGE_NO        0x04

#define CMD_STX1                    0x48
#define CMD_STX2                    0x4A
#define CMD_ETX1                    0x4A
#define CMD_ETX2                    0x4C

#define CMD_SEND_ETX                0xff 0xff 0xff

#define DEF_MCU_TO_OP_IN_REG        0
#define DEF_MCU_TO_OP_OUT_REG       1
#define DEF_MCU_TO_OP_RESET_LOG            "cc_rcv_out=%d \r"


#define DEF_NEXTION_LOG_LIST_MAX          17


nextionCommLcd::nextionCommLcd()
{
	m_NextionComm = { 0, };
	m_CmdState = cmd_state::wait_stx;
	m_Checksum = 0xffff;

	m_ComPort = nullptr;;
	m_IsPortOpen = false;

	m_hThread = nullptr;
	m_idThread = 0;
	m_lifeThread = false;
	m_armStatus = { 0, };
	m_hCommWnd = nullptr;

	m_log = nullptr;
	m_logCursor = 0;

	m_ComPort = new nextionCommSerial;
	m_ComPort->SetCallback(this, WrapFunc);

	m_log = new jL_logManager;

	/*
	m_log->PutLog("01 test %d", 12);
	m_log->PutLog("02 test %d", 12);

	string test;
	test = m_log->GetLog();
	test = m_log->GetLog();

	*/
}


nextionCommLcd::~nextionCommLcd()
{

	ThreadStop();

	if (m_NextionComm.rx_packet.data != nullptr)
	{
		delete m_NextionComm.rx_packet.data;
		m_NextionComm.rx_packet.data = nullptr;
	}

	if (m_log != nullptr)
	{
		delete m_log;
	}
	m_log = nullptr;

	if (m_ComPort != nullptr)
	{
		delete m_ComPort;
	}
	m_ComPort = nullptr;


	if (m_hThread != nullptr)
	{
		ThreadStop();
		m_hThread = nullptr;
	}

	if (m_hCommWnd != nullptr)
	{
		m_hCommWnd = nullptr;
	}

}

uint16_t nextionCommLcd::getOutReg()
{
	return m_armStatus.output_reg;
}

uint16_t nextionCommLcd::getInReg()
{
	return m_armStatus.input_reg;
}

DWORD nextionCommLcd::threadFunction(LPVOID pParam)
{
	nextionCommLcd* pThis;
	pThis = (nextionCommLcd*)pParam;

	TRACE(_T("Thread Run\n"));

	while (pThis->m_lifeThread)
	{
		pThis->threadJob();
		Sleep(50);
	}
	pThis->m_idThread = 0;
	pThis->m_hThread = nullptr;

	TRACE(_T("Thread Stop\n"));

	return 0;
}

void nextionCommLcd::threadJob()
{
	if (m_IsPortOpen)
	{
		sendIoReg();
		Sleep(20);
		updateOutReg();
	}

}

void nextionCommLcd::sendIoReg()
{
	uint8_t temp[255] = { 0, };
	size_t len = 0;
	sprintf_s((char*)temp, sizeof(temp), "cc_rcv_in=%d", getInReg());
	len = strnlen_s((char*)temp, 255);
	//vector<char> buffer(len);
	string  strTmp ((char*)&temp[0], len);
	temp[255 - 1] = '\0';	
	temp[len++ % 255] = 0xff;
	temp[len++ % 255] = 0xff;
	temp[len++ % 255] = 0xff;	

	if (m_hCommWnd != nullptr)
	{		
		if (m_lastLog[DEF_MCU_TO_OP_IN_REG] != strTmp)
		{
			m_lastLog[DEF_MCU_TO_OP_IN_REG] = strTmp;
			strTmp.push_back('\0');
			uint32_t pop_addr = m_log->PutLog_info("[MCU -> LCD] : %s", strTmp.c_str());
			::PostMessage(m_hCommWnd, WM_NEXTION_LCD_MAIN_DLG_MESSAGE, 0, pop_addr);
		}
	}
	m_ComPort->WriteByte((BYTE*)&temp[0], (UINT)(len));

}

void nextionCommLcd::updateOutReg()
{
	uint16_t value = 0;
	if (m_NextionComm.rx_packet.length == 2)
	{
		value = m_NextionComm.rx_packet.data[0];
		value |= m_NextionComm.rx_packet.data[1] << 8;
		m_armStatus.output_reg = value;
	}
	else
	{
		value = m_armStatus.output_reg;
	}

	uint8_t temp[255] = { 0, };
	size_t len = 0;
	sprintf_s((char*)temp, sizeof(temp), "cc_rcv_out=%d", value);
	len = strnlen_s((char*)temp, 255);

	string  strTmp((char*)&temp[0], len);
	temp[255 - 1] = '\0';
	temp[len++ % 255] = 0xff;
	temp[len++ % 255] = 0xff;
	temp[len++ % 255] = 0xff;

	if (m_hCommWnd != nullptr)
	{
		if (m_lastLog[DEF_MCU_TO_OP_OUT_REG] != strTmp)
		{
			m_lastLog[DEF_MCU_TO_OP_OUT_REG] = strTmp;
			strTmp.push_back('\0');
			uint32_t pop_addr = m_log->PutLog_info("[MCU -> LCD] : %s", strTmp.c_str());
			::PostMessage(m_hCommWnd, WM_NEXTION_LCD_MAIN_DLG_MESSAGE, 0, pop_addr);
		}
	}

	m_ComPort->WriteByte((BYTE*)&temp[0], (UINT)(len));
}

void nextionCommLcd::sendLog()
{
	uint8_t temp[255] = { 0, };
	size_t len = 0;

	while (m_log->Available(LOG_ERROR_LEVEL))
	{
		sprintf_s((char*)temp, sizeof(temp), "cc_log_index_curr=%d", (++m_logCursor % DEF_NEXTION_LOG_LIST_MAX));
		len = strnlen_s((char*)temp, 255);
		temp[255 - 1] = '\0';
		temp[len++ % 255] = 0xff;
		temp[len++ % 255] = 0xff;
		temp[len++ % 255] = 0xff;

		m_ComPort->WriteByte((BYTE*)&temp[0], (UINT)(len));

		// intervel
		Sleep(50);

		memset(&temp[0], 0, 255);
		string string_temp = m_log->GetLog();
		
		sprintf_s((char*)temp, sizeof(temp), "vLogTemp.txt=\"%s\"", string_temp.c_str());

		len = strnlen_s((char*)temp, 255);
		temp[255 - 1] = '\0';
		temp[len++ % 255] = 0xff;
		temp[len++ % 255] = 0xff;
		temp[len++ % 255] = 0xff;

		m_ComPort->WriteByte((BYTE*)&temp[0], (UINT)(len));

	}

}




int nextionCommLcd::ReceiveData()
{
	size_t len = 0;
	int length, packet_size = 0;

	bool wait_next = false;
	bool ret = false;
	// 포트로 들어와 버퍼에 쌓인 데이터 크기
	length = m_ComPort->m_QueueRead.GetSize();
	char data = 0;
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
				//TODO:: Beep sound
			}
			break;
			case HJJL_RXCMD_TYPE_WRITE_OUT_REG:
			{
				//updateReg(getInReg(), true);
				updateOutReg();
			}
			break;
			case HJJL_RXCMD_TYPE_CURR_PAGE_NO:
			{
				if (m_NextionComm.rx_packet.page_no == nextionpage_t::NEX_PAGE_LOG)
				{
					sendLog();
				}
			}
			break;

			}
		}
		m_ComPort->m_QueueRead.GetByte((byte*)&data);
		switch (m_CmdState)
		{
		case cmd_state::wait_stx:
		{
			// error check
			if (data == CMD_STX1)
			{
				packet_size = 0;
				m_NextionComm.rx_packet.buffer[packet_size++] = data;
				wait_next = true;
				break;
			}
			else if (data == CMD_STX2)
			{
				if (wait_next)
				{
					wait_next = false;
					m_CmdState = cmd_state::wait_page_no;
					m_NextionComm.rx_packet.check_sum = 0xffff;
					m_NextionComm.rx_packet.buffer[packet_size++] = data;
					break;
				}
			}
			else if (data == 0x00) // invail instruction
			{
				m_NextionComm.error++;
				for (int i = 0; i < 3; i++) {
					m_ComPort->m_QueueRead.GetByte((byte*)&data);
				}
			}
			else if (data == 0x01) // Instruction successful 
			{
				for (int i = 0; i < 3; i++) {
					m_ComPort->m_QueueRead.GetByte((byte*)&data);
				}
			}
			else if (data == 0x02) // invail Component ID
			{
				m_NextionComm.error++;
				for (int i = 0; i < 3; i++) {
					m_ComPort->m_QueueRead.GetByte((byte*)&data);
				}
			}
			else if (data == 0x03) // invail Page ID
			{
				m_NextionComm.error++;
				for (int i = 0; i < 3; i++) {
					m_ComPort->m_QueueRead.GetByte((byte*)&data);
				}
			}
			else if (data == 0x1a) // invail Variable name or attribute
			{
				m_NextionComm.error++;
				for (int i = 0; i < 3; i++) {
					m_ComPort->m_QueueRead.GetByte((byte*)&data);
				}
			}
			m_CmdState = cmd_state::wait_stx;
		}
		break;
		case cmd_state::wait_page_no:
		{
			m_NextionComm.rx_packet.page_no = (nextionpage_t)data;
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, data);

			m_NextionComm.rx_packet.buffer[packet_size++] = data;
			m_CmdState = cmd_state::wait_type;
		}
		break;
		case cmd_state::wait_type:
		{
			m_NextionComm.rx_packet.type = data;
			m_NextionComm.rx_packet.buffer[packet_size++] = data;
			if (m_NextionComm.rx_packet.type == HJJL_RXCMD_TYPE_REQ_BEEP
				|| m_NextionComm.rx_packet.type == HJJL_RXCMD_TYPE_CURR_PAGE_NO)
			{
				m_CmdState = cmd_state::wait_stx;
				ret = true;
				break;
			}
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, data);
			m_CmdState = cmd_state::wait_obj_id;
		}
		break;
		case cmd_state::wait_obj_id:
		{
			m_NextionComm.rx_packet.obj_id = data;
			m_NextionComm.rx_packet.buffer[packet_size++] = data;
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, data);
			m_NextionComm.rx_packet.length = 0x0000;
			m_CmdState = cmd_state::wiat_length_l;
		}
		break;
		case cmd_state::wiat_length_l:
		{
			m_NextionComm.rx_packet.length = data;
			m_NextionComm.rx_packet.length &= 0x00FF;
			m_NextionComm.rx_packet.buffer[packet_size++] = data;
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, data);
			m_CmdState = cmd_state::wait_length_h;
		}
		break;
		case cmd_state::wait_length_h:
		{
			m_NextionComm.rx_packet.length |= (data << 8);
			m_NextionComm.rx_packet.buffer[packet_size++] = data;
			util::crc16_put(&m_NextionComm.rx_packet.check_sum, data);
			if (m_NextionComm.rx_packet.length > 0)
			{
				if (m_NextionComm.rx_packet.data != nullptr)
				{
					delete m_NextionComm.rx_packet.data;
					m_NextionComm.rx_packet.data = nullptr;
				}
				m_NextionComm.rx_packet.data = new uint8_t[m_NextionComm.rx_packet.length];
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
			m_NextionComm.rx_packet.data[m_NextionComm.index++] = data;
			m_NextionComm.rx_packet.buffer[packet_size++] = data;

			util::crc16_put(&m_NextionComm.rx_packet.check_sum, data);
			if (m_NextionComm.index == m_NextionComm.rx_packet.length)
			{
				m_NextionComm.rx_packet.check_sum_recv = 0x00;
				m_CmdState = cmd_state::wait_checksum;
			}
		}
		break;
		case cmd_state::wait_checksum:
		{
			if (wait_next)
			{
				wait_next = 0;
				m_NextionComm.rx_packet.check_sum_recv |= data << 8;
				m_NextionComm.rx_packet.buffer[packet_size++] = data;
				m_CmdState = cmd_state::wait_etx;
			}
			else
			{
				wait_next = 1;
				m_NextionComm.rx_packet.buffer[packet_size++] = data;
				m_NextionComm.rx_packet.check_sum_recv = data;
				m_NextionComm.rx_packet.check_sum_recv &= 0x00FF;
			}
		}
		break;
		case cmd_state::wait_etx:
		{
			if (data == CMD_ETX1)
			{
				wait_next = 1;
				m_NextionComm.rx_packet.buffer[packet_size++] = data;
				break;
			}
			if (wait_next)
			{
				wait_next = 0;
				if (data == CMD_ETX2)
				{
					m_NextionComm.rx_packet.buffer[packet_size++] = data;
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
		char char_temp[80] = { 0, };

		for (int i = 0; i < packet_size; i++)
		{
			sprintf_s(char_temp, sizeof(char_temp), "%s%02X "
				, char_temp
				, m_NextionComm.rx_packet.buffer[i]);
		}

		//trans::CharToLPTSTR((LPSTR)&char_temp[0], &rec_buff[0], 80);

		ret = false;
		switch (m_NextionComm.rx_packet.type)
		{
		case HJJL_RXCMD_TYPE_REQ_BEEP:
		{
			//TODO:
		}
		break;
		case HJJL_RXCMD_TYPE_WRITE_OUT_REG:
		{
			//TODO:
			updateOutReg();
		}
		break;
		case HJJL_RXCMD_TYPE_CURR_PAGE_NO:
		{
			if (m_NextionComm.rx_packet.page_no == nextionpage_t::NEX_PAGE_LOG)
			{
				sendLog();
			}
		}
		break;

		}

		if (m_hCommWnd != nullptr)
		{
			uint32_t pop_addr = m_log->PutLog_info("[LCD -> MCD] : %s", char_temp);
			::PostMessage(m_hCommWnd, WM_NEXTION_LCD_MAIN_DLG_MESSAGE, 0, pop_addr);
		}

	}

	// 데이터 파싱

exit_parsing:

	return 0;
}

int nextionCommLcd::OpenComPort(CString portname, DWORD baudrate /*= CBR_9600*/, WORD port_no /*= 0*/)
{
	if (m_ComPort == nullptr)
		return -1;
	int ret = 0;
	if (m_ComPort->OpenPort(portname, port_no, baudrate) != 0)
	{
		// open err
		ret = 1;
		AfxMessageBox(L"open faliled");
	}
	return ret;
}

int nextionCommLcd::CloseComPort()
{
	if (m_ComPort == nullptr)
		return -1;
	m_ComPort->ClosePort();
	return 0;
}

void nextionCommLcd::ThreadRun()
{
	m_lifeThread = true;

	m_hThread = CreateThread(
		(LPSECURITY_ATTRIBUTES)NULL,
		0,
		(LPTHREAD_START_ROUTINE)threadFunction,
		(LPVOID)this,
		0,
		&m_idThread
	);

	if (!m_hThread)
	{
		// 실패하면 포트를 닫는다.
		ThreadStop();
		AfxMessageBox(_T("Error create thread for comm! %s"));
	}
}

void nextionCommLcd::ThreadStop()
{
	m_lifeThread = false;

	while (m_idThread != 0) {};

	m_hThread = nullptr;
	m_idThread = 0;
	TRACE(_T("Thread Stop\n"));
}

int nextionCommLcd::GetLCDPageNo()
{
	return static_cast<int>(m_NextionComm.rx_packet.page_no);
}

void nextionCommLcd::SetOutputReg(uint16_t value)
{
	m_armStatus.output_reg = value;
}

void nextionCommLcd::ChangeLCDPage(int page_no)
{
	if (page_no > DEF_NEX_PAGE_LOG && page_no < DEF_NEX_PAGE_INIT)
		return;

	uint8_t temp[255] = { 0, };
	size_t len = 0;
	sprintf_s((char*)temp, sizeof(temp), "page %d", page_no);
	len = strnlen_s((char*)temp, 255);
	//vector<char> buffer(len);
	string  strTmp((char*)&temp[0], len);
	temp[255 - 1] = '\0';
	temp[len++ % 255] = 0xff;
	temp[len++ % 255] = 0xff;
	temp[len++ % 255] = 0xff;

	if (m_hCommWnd != nullptr)
	{
		strTmp.push_back('\0');
		uint32_t pop_addr = m_log->PutLog_info("[MCU -> LCD] : %s", strTmp.c_str());
		::PostMessage(m_hCommWnd, WM_NEXTION_LCD_MAIN_DLG_MESSAGE, 0, pop_addr);
	}
	m_ComPort->WriteByte((BYTE*)&temp[0], (UINT)(len));
}
