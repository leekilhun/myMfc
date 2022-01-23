#include "pch.h"
#include "jL_fastechComm.h"



/*

| Type | Means[bt = byte] | Send Data | Resp Data|
| -- - | ------------------------------------ | :------ - | : ------ - |
| 0x01 | Get Slave Information | 0 bt | 1~248 bt[Slave종류 + 내용]|
| 0x05 | Get Motor Information | 0 bt | 1~248 bt[Motor id + 내용]|
| 0x10 | Save ROM All Parameters | 0 bt | 1 bt[state]     |
| 0x11 | Get ROM Parameter | 1 bt parameter no | 5 bt[state + Parameter] |
| 0x12 | Set RAM Parameter | 5 bt[Param No + Parameter] | 1 bt[state] |
| 0x13 | Get RAM Parameter | 1 bt parameter no | 5 bt[state + Parameter] |
| 0x20 | Set IO Output | 8 bt[4bt set_mask + 4bt clear_mask] | 1 bt[state] |
| 0x21 | Set IO Input | 8 bt[4bt set_mask + 4bt clear_mask] | 1 bt[state] |
| 0x22 | Get IO Input | 0 bt | 5 bt [[state + input reg]] |
| 0x23 | Get IO Output | 0 bt | 5 bt [[state + output reg]] |
| 0x24 | Set IO Map Assign | 6 bte[1bt ioNo + 4bt mask_data + 1bt level] | 1 bt[state] |
| 0x25 | Get IO Map Assign | 1 bt | 6 bt[1bt ioNo + 4bt mask_data + 1bt level]|
| 0x26 | Read ROM - IO Map | 0 bt | 2 bt[state, 0 완료 or 이외 Error |
| 0x27 | Trigger Output Run A | 18 bt[참조 Frame 1] | 2 bt[state, 0 완료 or 이외 Error |
| 0x28 | Trigger Output Status | 0 bt | 2 bt[state, 1 Busy - 0 종료 |
| 0x2A | Servo Enable | 1 bt[0 off - 1 on] | 1 bt[state]        |
| 0x2B | Alarm Reset | 0 bt | 1 bt[state]     |
| 0x2B | Get Alarm Information | o bt | 2 bt[0 no_alarm, 이외 alarm번호]      |
| 0x31 | Move Stop | 0 bt | 1 bt[state]     |
| 0x32 | Emergency Move Stop | 0 bt | 1 bt[state]     |
| 0x33 | Moving Origin Single Axis | 0 bt | 1 bt[state]     |
| 0x34 | Moving Single Axis Absolute Pos | 8 bt[4bt 절대위치 4bt 속도] | 1 bt[state] |
| 0x35 | Moving Single Axis Relative Pos | 8 bt[4bt 상대위치 4bt 속도] | 1 bt[state] |
| 0x36 | Moving to limit | 5 bt[4bt 속도, 1bt 방향(0 - , 1 + )] | 1 bt[state] |
| 0x37 | Moving Velocity | 5 bt[4bt 속도, 1bt 방향(0 - , 1 + )] | 1 bt[state] |
| 0x38 | Abs Pos Override | 4 bt[4bt 변경 위치(pluse)] | 1 bt[state] |
| 0x39 | Rel Pos Override | 4 bt[4bt 변경 위치(pluse)] | 1 bt[state] |
| 0x3A | Velocity Override | 4 bt[4bt 변경 속도(pps)] | 1 bt[state] |
| 0x3B | All Move Stop[slave '99' 지정] | 0bt | 0 bt |
| 0x3C | All Move E - Stop[slave '99' 지정] | 0bt | 0 bt |
| 0x3D | All Move Origin[slave '99' 지정] | 0bt | 0 bt |
| 0x3E | All Move Abs Pos[slave '99' 지정] | 8 bt | 0 bt |
| 0x3F | All Move Rel Pos[slave '99' 지정] | 8 bt | 0 bt |

*/


#if 1
#define DEF_FASTECH_COMM_TYPE_GET_SLAVE_INFO          0x01
#define DEF_FASTECH_COMM_TYPE_GET_MOTOR_INFO          0x05
#define DEF_FASTECH_COMM_TYPE_SAVE_ROM_ALL            0x10
#define DEF_FASTECH_COMM_TYPE_GET_ROM_PARAM           0x11
#define DEF_FASTECH_COMM_TYPE_SET_RAM_PARAM           0x12
#define DEF_FASTECH_COMM_TYPE_GET_RAM_PARAM           0x13
#define DEF_FASTECH_COMM_TYPE_SET_IO_OUT_REG          0x20
#define DEF_FASTECH_COMM_TYPE_SET_IO_IN_REG           0x21
#define DEF_FASTECH_COMM_TYPE_GET_IO_IN_REG           0x22
#define DEF_FASTECH_COMM_TYPE_GET_IO_OUT_REG          0x23
#define DEF_FASTECH_COMM_TYPE_SET_ASSN_IO_MAP         0x24
#define DEF_FASTECH_COMM_TYPE_GET_ASSN_IO_MAP         0x25
#define DEF_FASTECH_COMM_TYPE_READ_IO_MAP             0x26
#define DEF_FASTECH_COMM_TYPE_TRG_OUT_RUN_A           0x27
#define DEF_FASTECH_COMM_TYPE_TRG_OUT_STATUS          0x28
#define DEF_FASTECH_COMM_TYPE_SERVO_ENABLE            0x2A
#define DEF_FASTECH_COMM_TYPE_ALARM_RESET             0x2B
#define DEF_FASTECH_COMM_TYPE_GET_ALARM_INFO          0x2B
#define DEF_FASTECH_COMM_TYPE_MOVE_STOP               0x31
#define DEF_FASTECH_COMM_TYPE_MOVE_EMG_STOP           0x32
#define DEF_FASTECH_COMM_TYPE_MOVE_ORG_SINGLE_AXIS    0x33
#define DEF_FASTECH_COMM_TYPE_MOVE_ABS_SINGLE_AXIS    0x34
#define DEF_FASTECH_COMM_TYPE_MOVE_REL_SINGLE_AXIS    0x35
#define DEF_FASTECH_COMM_TYPE_MOVE_TO_LIMIT           0x36
#define DEF_FASTECH_COMM_TYPE_MOVE_VELOCITY           0x37
#define DEF_FASTECH_COMM_TYPE_MOVE_ABS_OVERRIDE       0x38
#define DEF_FASTECH_COMM_TYPE_MOVE_REL_OVERRIDE       0x39
#define DEF_FASTECH_COMM_TYPE_MOVE_VELOCITY_OVERRIDE  0x3A
#define DEF_FASTECH_COMM_TYPE_MOVE_ALL_STOP           0x3B
#define DEF_FASTECH_COMM_TYPE_MOVE_ALL_EMG_STOP       0x3C
#define DEF_FASTECH_COMM_TYPE_MOVE_ALL_ORG_STOP       0x3D
#define DEF_FASTECH_COMM_TYPE_MOVE_ALL_ABS_POS        0x3E
#define DEF_FASTECH_COMM_TYPE_MOVE_ALL_REL_POS        0x3F
#endif

#define CMD_STX1                    0xAA
#define CMD_STX2                    0xCC
#define CMD_ETX1                    0xAA
#define CMD_ETX2                    0xEE
#define STUFFING_BYTE               0xAA


#define DEF_FASTECH_MOTOR_TO_PC_DATA        0
#define DEF_FASTECH_PC_TO_MOTOR_DATA        1


jL_fastechComm::jL_fastechComm()
{
	m_CmdState = cmd_state::wait_stx;
	m_Checksum = 0xFF;
	m_FastechComm = { 0, };
	m_bThreadLife = false;
	m_ThreadId = 0;
	m_hThread = {};
	m_IsWiatResponse = true;
	m_log = nullptr;

	m_CmdList = nullptr;
	m_CmdList = new _Cmd;

	// insert cmd 
	fastech_cmd cmd;
	cmd.cmd = DEF_FASTECH_COMM_TYPE_GET_SLAVE_INFO;
	cmd.name = L"GET_SLAVE_INFO";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_GET_MOTOR_INFO;
	cmd.name = L"GET_MOTOR_INFO";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_SAVE_ROM_ALL;
	cmd.name = L"SAVE_ROM_ALL";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_GET_ROM_PARAM;
	cmd.name = L"GET_ROM_PARAM";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_SET_RAM_PARAM;
	cmd.name = L"SET_RAM_PARAM";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_GET_RAM_PARAM;
	cmd.name = L"GET_RAM_PARAM";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_SET_IO_OUT_REG;
	cmd.name = L"SET_IO_OUT_REG";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_SET_IO_IN_REG;
	cmd.name = L"SET_IO_IN_REG";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_GET_IO_IN_REG;
	cmd.name = L"GET_IO_IN_REG";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_GET_IO_OUT_REG;
	cmd.name = L"GET_IO_OUT_REG";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_SET_ASSN_IO_MAP;
	cmd.name = L"SET_ASSN_IO_MAP";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_GET_ASSN_IO_MAP;
	cmd.name = L"GET_ASSN_IO_MAP";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_READ_IO_MAP;
	cmd.name = L"READ_IO_MAP";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_TRG_OUT_RUN_A;
	cmd.name = L"TRG_OUT_RUN_A";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_TRG_OUT_STATUS;
	cmd.name = L"TRG_OUT_STATUS";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_SERVO_ENABLE;
	cmd.name = L"SERVO_ENABLE";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_ALARM_RESET;
	cmd.name = L"ALARM_RESET";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_GET_ALARM_INFO;
	cmd.name = L"GET_ALARM_INFO";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_STOP;
	cmd.name = L"MOVE_STOP";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_EMG_STOP;
	cmd.name = L"MOVE_EMG_STOP";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_ORG_SINGLE_AXIS;
	cmd.name = L"MOVE_ORG_SINGLE_AXIS";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_ABS_SINGLE_AXIS;
	cmd.name = L"MOVE_ABS_SINGLE_AXIS";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_REL_SINGLE_AXIS;
	cmd.name = L"MOVE_REL_SINGLE_AXIS";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_TO_LIMIT;
	cmd.name = L"MOVE_TO_LIMIT";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_VELOCITY;
	cmd.name = L"MOVE_VELOCITY";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_ABS_OVERRIDE;
	cmd.name = L"MOVE_ABS_OVERRIDE";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_REL_OVERRIDE;
	cmd.name = L"MOVE_REL_OVERRIDE";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_VELOCITY_OVERRIDE;
	cmd.name = L"MOVE_VELOCITY_OVERRIDE";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_ALL_STOP;
	cmd.name = L"MOVE_ALL_STOP";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_ALL_EMG_STOP;
	cmd.name = L"MOVE_ALL_EMG_STOP";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_ALL_ORG_STOP;
	cmd.name = L"MOVE_ALL_ORG_STOP";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_ALL_ABS_POS;
	cmd.name = L"MOVE_ALL_ABS_POS";
	m_CmdList->InsertCmd(cmd);
	cmd.cmd = DEF_FASTECH_COMM_TYPE_MOVE_ALL_REL_POS;
	cmd.name = L"MOVE_ALL_REL_POS";
	m_CmdList->InsertCmd(cmd);


	m_log = new jL_logManager;
}

jL_fastechComm::~jL_fastechComm()
{
	this->ClosePort();
	if (m_bThreadLife)
	{
		m_bThreadLife = false;

		while (m_ThreadId != 0);
	}
	if (m_FastechComm.rx_packet.data != nullptr)
	{
		delete m_FastechComm.rx_packet.data;
		m_FastechComm.rx_packet.data = nullptr;
	}

	if (m_log != nullptr)
		delete m_log;
	m_log = nullptr;

	 if (m_CmdList != nullptr)
		 delete[] m_CmdList;
	 m_CmdList = nullptr;

}

UINT jL_fastechComm::commThread(LPVOID pParam)
{
	jL_fastechComm* pThis = (jL_fastechComm*)pParam;

	BYTE request_state[8] = { 0xAA, 0xCC, 0x00, 0x01, 0xC0, 0x70, 0xAA, 0xEE };
	pThis->m_IsWiatResponse = true;
	TRACE("Fastech Comm Thread start\n");
	pThis->m_FastechComm.pre_time = util::millis();
	while (pThis->m_bThreadLife)
	{
		bool heartbeat = util::millis() - pThis->m_FastechComm.pre_time > 1000;
		if (pThis->m_IsWiatResponse || heartbeat)
		{
			pThis->m_IsWiatResponse = false;
			pThis->WriteByte(&request_state[0], 8);
			Sleep(20);
			//pThis->UpdateReg();
		}


		Sleep(50);
	}
	TRACE("Fastech Comm Thread Terminatet\n");
	pThis->m_ThreadId = 0;
	return 0;
}

void jL_fastechComm::doTypeComm(uint8_t type)
{
	CString rec_text;
	TCHAR rec_buff[FASTECH_CMD_MAX_DATA_LENGTH] = { 0, };
	char char_temp[FASTECH_CMD_MAX_DATA_LENGTH] = { 0, };

	TCHAR rec_asc[FASTECH_CMD_MAX_DATA_LENGTH] = { 0, };
	char char_asc[FASTECH_CMD_MAX_DATA_LENGTH] = { 0, };


	for (uint16_t i = 0; i < m_FastechComm.packet_size; i++)
	{
		sprintf_s(char_temp, sizeof(char_temp), "%s%02X "
			, char_temp
			, m_FastechComm.rx_packet.buffer[i]);
	}

	for (uint16_t i = 0; i < m_FastechComm.index; i++)
	{
		sprintf_s(char_asc, sizeof(char_asc), "%s%c"
			, char_asc
			, m_FastechComm.rx_packet.data[i]);
	}

	size_t len = strnlen_s((char*)char_temp, FASTECH_CMD_MAX_DATA_LENGTH);

	string  strTmp((char*)&char_temp[0], len);
	trans::CharToLPTSTR((LPSTR)&char_temp[0], &rec_buff[0], FASTECH_CMD_MAX_DATA_LENGTH);

	string  str_asc((char*)&char_asc[0], len);
	trans::CharToLPTSTR((LPSTR)&char_asc[0], &rec_asc[0], FASTECH_CMD_MAX_DATA_LENGTH);


	switch (m_FastechComm.rx_packet.type)
	{
	case DEF_FASTECH_COMM_TYPE_GET_SLAVE_INFO: // 
	{
	}
	break;
	case 0xff: // 
	{
		/*m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[0], 0);
		m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[1], 1);
		m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[2], 2);
		m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[3], 3);*/
	}
	break;
	}


	if (m_hCommWnd != nullptr)
	{
		if (m_lastLog[DEF_FASTECH_MOTOR_TO_PC_DATA] != strTmp)
		{
			m_lastLog[DEF_FASTECH_MOTOR_TO_PC_DATA] = strTmp;

			uint32_t data_addr = m_log->PutLog_info("%s", str_asc.c_str());

			strTmp.push_back('\0');
			uint32_t pop_addr = m_log->PutLog_info("[MOTOR -> PC] : %s", strTmp.c_str());
			::PostMessage(m_hCommWnd, WM_FASTECH_MOTOR_SERIAL_COMM_MESSAGE, data_addr, pop_addr);
		}
	}


}

void jL_fastechComm::ResetData()
{
	m_CmdState = cmd_state::wait_stx;
	m_FastechComm.wait_next = false;
	m_FastechComm.rx_packet.length = 0;
	m_FastechComm.packet_size = 0;
}

int jL_fastechComm::ParsingPacket()
{
	int length = -1;
	byte data = 0x00;

	// 포트로 들어와 버퍼에 쌓인 데이터 크기
	length = m_pQueueRead->GetSize();
	if (length == 0)
		goto exit_parsing;

	m_IsWiatResponse = true;

	bool ret = false;
	//int packet_size = 0;
	for (int i = 0; i < length; i++)
	{
		if (ret == true)
		{
			ret = false;
			doTypeComm(m_FastechComm.rx_packet.type);
		}


		if (m_pQueueRead->GetByte(&data) == false)
			goto exit_parsing;

		switch (m_CmdState)
		{
		case jL_fastechComm::cmd_state::wait_stx:
		{
			if (data == CMD_STX1)
			{
				m_FastechComm.rx_packet.length = 0;
				m_FastechComm.packet_size = 0;
				memset(&m_FastechComm.rx_packet.buffer[0], 0x00, FASTECH_CMD_MAX_DATA_LENGTH + 8);
				m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
				m_FastechComm.wait_next = true;
				break;
			}
			if (m_FastechComm.wait_next)
			{
				m_FastechComm.wait_next = false;
				if (data == CMD_STX2)
				{
					m_CmdState = cmd_state::wait_id;
					m_FastechComm.rx_packet.check_sum = 0xffff;
					m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
				}
				else
				{
					m_CmdState = cmd_state::wait_stx;
				}
			}
		}
		break;
		case jL_fastechComm::cmd_state::wait_id:
		{
			m_FastechComm.rx_packet.id = (uint8_t)data;
			util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);

			m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
			m_CmdState = cmd_state::wait_sync;
		}
		break;
		case jL_fastechComm::cmd_state::wait_sync:
		{
			m_FastechComm.rx_packet.sync_no = (uint8_t)data;
			util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);

			m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
			m_CmdState = cmd_state::wait_type;
		}
		break;

		case jL_fastechComm::cmd_state::wait_type:
		{
			m_FastechComm.rx_packet.type = data;
			m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;			

			util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);
			//TODO: type별 데이터 길이  
			fastech_cmdList cmd_type = m_CmdList->GetIndex(data);
			switch (cmd_type)
			{
			case fastech_cmdList::SERVO_ENABLE: // 성공
			{
				m_FastechComm.rx_packet.length = 0;
			}
			break;
			case fastech_cmdList::ALARAM_RESET: // 
			{
				m_FastechComm.rx_packet.length = 0;
			}
			break;
			default:
			{
				m_FastechComm.rx_packet.length = FASTECH_CMD_MAX_DATA_LENGTH;
			}
			break;
			}

			m_CmdState = cmd_state::wiat_response;
		}
		break;
		case jL_fastechComm::cmd_state::wiat_response:
		{
			m_FastechComm.rx_packet.response = data;
			m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
			util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);

			// 버퍼에 있는 데이터가 Data 끝인지 확인 ( AA EE )
			int next_no = 1;
			byte __data = m_pQueueRead->PopByte(m_pQueueRead->m_Tail + (next_no));
			byte ___data = m_pQueueRead->PopByte(m_pQueueRead->m_Tail + (next_no + 1));
			bool is_etx = (__data == CMD_ETX1 && ___data == CMD_ETX2);
			if (is_etx)
			{
				m_FastechComm.wait_next = false;
				m_CmdState = cmd_state::wait_checksum;
			}

			//TODO: response 결과에 대한 처리 
			switch (data)
			{
			case 0x00: // 성공
			{

			}
			break;
			default:
			{
				m_FastechComm.error++;
			}
			break;
			}

			// length 
			if (m_FastechComm.rx_packet.length == 0)
			{
				m_FastechComm.wait_next = false;
				m_CmdState = cmd_state::wait_checksum;
			}
			else
			{
				if (m_FastechComm.rx_packet.data != nullptr)
				{
					delete m_FastechComm.rx_packet.data;
					m_FastechComm.rx_packet.data = nullptr;
				}
				m_FastechComm.rx_packet.data = new uint8_t[FASTECH_CMD_MAX_DATA_LENGTH]{};
				m_FastechComm.index = 0;
				m_CmdState = cmd_state::wait_data;
			}
			
		}
		break;

		case jL_fastechComm::cmd_state::wait_data:
		{
			// AA EE 확인하여 Data 끝을 확인
			int next_no = 1;
			byte __data = m_pQueueRead->PopByte(m_pQueueRead->m_Tail + (next_no));
			byte ___data = m_pQueueRead->PopByte(m_pQueueRead->m_Tail + (next_no + 1));
			bool is_etx = (__data == CMD_ETX1 && ___data == CMD_ETX2);
			if (is_etx)
			{
				m_FastechComm.wait_next = true;
				m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
				m_FastechComm.rx_packet.check_sum_recv = data;
				m_CmdState = cmd_state::wait_checksum;
				break;
			}
			else
			{
				__data = m_pQueueRead->PopByte(m_pQueueRead->m_Tail + 1);
				___data = m_pQueueRead->PopByte(m_pQueueRead->m_Tail + 2);
				if (__data == CMD_ETX1 && ___data == CMD_ETX2)
				{
					m_FastechComm.wait_next = true;
					m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
					m_FastechComm.rx_packet.check_sum_recv = data;
					m_CmdState = cmd_state::wait_checksum;
					break;
				}
			}

			// 입력할 데이터 넣기

			if (data == STUFFING_BYTE)
			{
				m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
				//util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);
				// Data 넣기를 건너뛴다
				//m_FastechComm.rx_packet.data[m_FastechComm.index++] = data;       
				m_pQueueRead->GetByte(&data);
				m_FastechComm.rx_packet.data[m_FastechComm.index++] = data;
				m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;

				util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);
			}
			else
			{
				m_FastechComm.rx_packet.data[m_FastechComm.index++] = data;
				m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;

				util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);				
			}

		}
		break;
		case jL_fastechComm::cmd_state::wait_checksum:
		{
			if (m_FastechComm.wait_next)
			{
				m_FastechComm.wait_next = false;
				m_FastechComm.rx_packet.check_sum_recv |= data << 8;
				m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
				m_CmdState = cmd_state::wait_etx;
			}
			else
			{
				m_FastechComm.wait_next = true;
				m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
				m_FastechComm.rx_packet.check_sum_recv = data;
			}
		}
		break;
		case jL_fastechComm::cmd_state::wait_etx:
		{
			if (data == CMD_ETX1)
			{
				m_FastechComm.wait_next = true;
				m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
				break;
			}
			if (m_FastechComm.wait_next)
			{
				m_FastechComm.wait_next = false;
				if (data == CMD_ETX2)
				{
					m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
					if (m_FastechComm.rx_packet.check_sum == m_FastechComm.rx_packet.check_sum_recv)
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
		ret = false;

		doTypeComm(m_FastechComm.rx_packet.type);

	}

exit_parsing:
	return 0;
}

void jL_fastechComm::ReadyPort()
{
	m_FastechComm.is_init = true;
	m_FastechComm.baud = m_Baudrate;
	// Callbac function attach
	SetAttachCallbackFunc(this, WarpFunc);
}

void jL_fastechComm::UpdateReg()
{
	byte packet[FASTECH_CMD_MAX_DATA_LENGTH] = { 0 };
	uint32_t crc = 0xffff;
	int index = 0;
	packet[index++] = CMD_STX1;
	packet[index++] = CMD_STX2;

	packet[index] = 0; // id
	util::crc16_put(&crc, packet[index++]);
	packet[index] = 0xf0; // cmd
	util::crc16_put(&crc, packet[index++]);


	//packet[index] = m_pRegisterIO->outState[0].GetValue(); // data1
	//if (packet[index] == STUFFING_BYTE)
	//  packet[++index] = STUFFING_BYTE;
	//util::crc16_put(&crc, packet[index++]);
	//packet[index] = m_pRegisterIO->outState[1].GetValue(); // data1
	//if (packet[index] == STUFFING_BYTE)
	//  packet[++index] = STUFFING_BYTE;
	//util::crc16_put(&crc, packet[index++]);
	//packet[index] = m_pRegisterIO->outState[2].GetValue(); // data1
	//if (packet[index] == STUFFING_BYTE)
	//  packet[++index] = STUFFING_BYTE;
	//util::crc16_put(&crc, packet[index++]);
	//packet[index] = m_pRegisterIO->outState[3].GetValue(); // data1
	//if (packet[index] == STUFFING_BYTE)
	//  packet[++index] = STUFFING_BYTE;
	//util::crc16_put(&crc, packet[index++]);



	packet[index++] = crc & 0xff;
	packet[index++] = (crc >> 8) & 0xff;

	packet[index++] = CMD_ETX1;
	packet[index++] = CMD_ETX2;

	WriteByte(&packet[0], index);
}

int jL_fastechComm::SetMotorId(uint8_t id)
{
	m_FastechComm.rx_packet.id = id;
	return 0;
}

int jL_fastechComm::SendCmd(uint32_t cmd_index)
{
	ResetData();

	fastech_cmd cmd;
	cmd = m_CmdList->GetCmd(static_cast<fastech_cmdList>(cmd_index));

	byte packet[FASTECH_CMD_MAX_DATA_LENGTH] = { 0 };
	uint32_t crc = 0xffff;
	int index = 0;
	packet[index++] = CMD_STX1;
	packet[index++] = CMD_STX2;

	packet[index] = m_FastechComm.rx_packet.id; // id
	util::crc16_put(&crc, packet[index++]);

	packet[index] = (m_FastechComm.rx_packet.sync_no + 1) % 255; // sync_no
	util::crc16_put(&crc, packet[index++]);

	packet[index] = cmd.cmd; // cmd
	util::crc16_put(&crc, packet[index++]);

	packet[index++] = crc & 0xff;
	packet[index++] = (crc >> 8) & 0xff;

	packet[index++] = CMD_ETX1;
	packet[index++] = CMD_ETX2;

	WriteByte(&packet[0], index);


	TCHAR rec_buff[FASTECH_CMD_MAX_DATA_LENGTH] = { 0, };
	char char_temp[FASTECH_CMD_MAX_DATA_LENGTH] = { 0, };
	for (uint16_t i = 0; i < index; i++)
	{
		sprintf_s(char_temp, sizeof(char_temp), "%s%02X "
			, char_temp
			, packet[i]);
	}
	size_t len = strnlen_s((char*)char_temp, FASTECH_CMD_MAX_DATA_LENGTH);

	string  strTmp((char*)&char_temp[0], len);
	trans::CharToLPTSTR((LPSTR)&char_temp[0], &rec_buff[0], FASTECH_CMD_MAX_DATA_LENGTH);


	if (m_hCommWnd != nullptr)
	{
		if (m_lastLog[DEF_FASTECH_PC_TO_MOTOR_DATA] != strTmp)
		{
			m_lastLog[DEF_FASTECH_PC_TO_MOTOR_DATA] = strTmp;
			strTmp.push_back('\0');
			uint32_t pop_addr = m_log->PutLog_info("[PC -> MOTOR] : %s", strTmp.c_str());
			::PostMessage(m_hCommWnd, WM_FASTECH_MOTOR_SERIAL_COMM_MESSAGE, 0, pop_addr);
		}
	}


	return 0;
}

int jL_fastechComm::SendCmd(fastech_cmdList cmdenum, uint8_t* data, uint8_t length)
{
	ResetData();

	fastech_cmd cmd;
	cmd = m_CmdList->GetCmd(cmdenum);

	byte packet[FASTECH_CMD_MAX_DATA_LENGTH] = { 0 };
	uint32_t crc = 0xffff;
	int index = 0;
	packet[index++] = CMD_STX1;
	packet[index++] = CMD_STX2;

	packet[index] = m_FastechComm.rx_packet.id; // id
	util::crc16_put(&crc, packet[index++]);

	packet[index] = (m_FastechComm.rx_packet.sync_no +1) % 255 ; // sync_no
	util::crc16_put(&crc, packet[index++]);


	packet[index] = cmd.cmd; // cmd
	util::crc16_put(&crc, packet[index++]);

	if (length > 0)
	{
		for (size_t i = 0; i < length; i++)
		{
			packet[index] = data[i]; // data
			util::crc16_put(&crc, packet[index++]);
		}
	}
	
	packet[index++] = crc & 0xff;
	packet[index++] = (crc >> 8) & 0xff;

	packet[index++] = CMD_ETX1;
	packet[index++] = CMD_ETX2;

	WriteByte(&packet[0], index);


	TCHAR rec_buff[FASTECH_CMD_MAX_DATA_LENGTH] = { 0, };
	char char_temp[FASTECH_CMD_MAX_DATA_LENGTH] = { 0, };
	for (uint16_t i = 0; i < index; i++)
	{
		sprintf_s(char_temp, sizeof(char_temp), "%s%02X "
			, char_temp
			, packet[i]);
	}
	size_t len = strnlen_s((char*)char_temp, FASTECH_CMD_MAX_DATA_LENGTH);

	string  strTmp((char*)&char_temp[0], len);
	trans::CharToLPTSTR((LPSTR)&char_temp[0], &rec_buff[0], FASTECH_CMD_MAX_DATA_LENGTH);


	if (m_hCommWnd != nullptr)
	{
		if (m_lastLog[DEF_FASTECH_PC_TO_MOTOR_DATA] != strTmp)
		{
			m_lastLog[DEF_FASTECH_PC_TO_MOTOR_DATA] = strTmp;
			strTmp.push_back('\0');
			uint32_t pop_addr = m_log->PutLog_info("[PC -> MOTOR] : %s", strTmp.c_str());
			::PostMessage(m_hCommWnd, WM_FASTECH_MOTOR_SERIAL_COMM_MESSAGE, 0, pop_addr);
		}
	}


	return 0;
}

DWORD jL_fastechComm::ThreadRun()
{
	CString altstr;

	// 포트 감시 스레드 생성
	this->m_hThread = CreateThread(
		(LPSECURITY_ATTRIBUTES)NULL,
		0,
		(LPTHREAD_START_ROUTINE)commThread,
		(LPVOID)this,
		0,
		&this->m_ThreadId
	);

	if (!m_hThreadWatchComm)
	{
		// 실패하면 포트를 닫는다.
		ClosePort();
		altstr.Format(_T("Error create thread fastech io! %s"), L"Fastech Module");
		AfxMessageBox(altstr);
	}
	else
	{
		m_bThreadLife = true;
	}

	return this->m_ThreadId;
}

int jL_fastechComm::Terminate()
{
	m_bThreadLife = false;

	while (m_ThreadId != 0);


	ClosePort();

	return 0;
}


void jL_fastechComm::AddStringCmdList(CComboBox* combobox)
{
	if (combobox == NULL) return;	
	combobox->ResetContent();
	int index = 0;	
	/*combobox->AddString(L"Get slave infor");
	combobox->SetItemData(0, 1);

	combobox->AddString(L"Get motor infor");
	combobox->SetItemData(1, 2);

	combobox->AddString(L"test infor");
	combobox->SetItemData(2, 3);*/
	fastech_cmd cmd;
	for (int i = 0; i < (int)m_CmdList->count; i++)
	{
		cmd = m_CmdList->GetCmd(static_cast<fastech_cmdList>(i));
		combobox->AddString(cmd.name);
		combobox->SetItemData(i, i /*cmd.cmd*/);
		index++;
	}
	
	
	combobox->SetCurSel(index - 1);
}

