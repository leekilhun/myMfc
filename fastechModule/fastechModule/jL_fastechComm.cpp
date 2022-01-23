#include "pch.h"
#include "jL_fastechComm.h"


#define CMD_STX1                    0xAA
#define CMD_STX2                    0xCC
#define CMD_ETX1                    0xAA
#define CMD_ETX2                    0xEE
#define STUFFING_BYTE               0xAA

jL_fastechComm::jL_fastechComm()
{
  m_pRegisterIO = nullptr;

  m_CmdState = cmd_state::wait_stx;
  m_Checksum = 0xFF;
  m_FastechComm = {0, };
  m_bThreadLife = false;
  m_ThreadId = 0;
  m_hThread = {};
  m_IsWiatResponse = true;  

  m_pRegisterIO  = new jL_fastechComm::_Register(4,4);
  this->OpenPort(L"COM2", 2, 115200);
  SetAttachCallbackFunc(this, &jL_fastechComm::WarpFunc);
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

  if (m_pRegisterIO != nullptr)
  {
    delete m_pRegisterIO;
    m_pRegisterIO = nullptr;
  }

}

UINT jL_fastechComm::ioThread(LPVOID pParam)
{
  jL_fastechComm* pThis = (jL_fastechComm*)pParam;

  BYTE request_state[8] = { 0xAA, 0xCC,0x00,0xF1,0xC0, 0x34, 0xAA, 0xEE };
  pThis->m_IsWiatResponse = true;

  pThis->m_FastechComm.pre_time = util::millis();
  while (pThis->m_bThreadLife)
  {
    bool heartbeat = util::millis() - pThis->m_FastechComm.pre_time > 100;
    if (pThis->m_IsWiatResponse || heartbeat)
    {
      pThis->m_IsWiatResponse = false;
      pThis->WriteByte(&request_state[0], 8);
      Sleep(50);
      pThis->UpdateReg();
    } 

    
    Sleep(50);
  }
  return 0;
}

int jL_fastechComm::ParsingPacket()
{
  int length = -1;
  byte data = 0x00;
  
  // 포트로 들어와 버퍼에 쌓인 데이터 크기
  length = m_QueueRead.GetSize();
  if (length == 0)
    goto exit_parsing;

  m_IsWiatResponse = true;

  bool wait_next = false;
  bool ret = false;
  //int packet_size = 0;
  for (int i = 0; i < length; i++)
  {
    if (ret == true)
    {
      ret = false;         
      
      CString rec_text;
      TCHAR rec_buff[80] = { 0, };
      char char_temp[80] = { 0, };

      for (uint16_t i = 0; i < m_FastechComm.packet_size; i++)
      {
        sprintf_s(char_temp, sizeof(char_temp), "%s%02X "
          , char_temp
          , m_FastechComm.rx_packet.buffer[i]);
      }

      trans::CharToLPTSTR((LPSTR)&char_temp[0], &rec_buff[0], 80);

      switch (m_FastechComm.rx_packet.type)
      {
      case 0xF0: // output response
      {
      }
      break;
      case 0xF1: // input status
      {
        m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[0], 0);
        m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[1], 1);
        m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[2], 2);
        m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[3], 3);
      }
      break;
      }

      
    }


    if (m_QueueRead.GetByte(&data) == false)
      goto exit_parsing;

    switch (m_CmdState)
    {
    case jL_fastechComm::cmd_state::wait_stx:
    {
      if (data == CMD_STX1)
      {
        m_FastechComm.packet_size = 0;
        memset(&m_FastechComm.rx_packet.buffer[0], 0x00, FASTECH_CMD_MAX_DATA_LENGTH + 8);
        m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
        wait_next = true;
        break;
      }
      if (wait_next)
      {
        wait_next = false;
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
      m_CmdState = cmd_state::wait_type;
    }
      break;
    case jL_fastechComm::cmd_state::wait_type:
    {
      m_FastechComm.rx_packet.type = data;
      m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
      // TODO: type별 이벤트 정리 필요
      switch (data)
      {
      case 0xF0: // output response
      {
        m_FastechComm.rx_packet.length = 1;// response
        if (m_FastechComm.rx_packet.data != nullptr)
        {
          delete m_FastechComm.rx_packet.data;
          m_FastechComm.rx_packet.data = nullptr;
        }
        m_FastechComm.rx_packet.data = new uint8_t[1]{};
        m_FastechComm.index = 0;

        util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);
        m_CmdState = cmd_state::wait_data;
      }
      break;
      case 0xF1:   // input register
      {        
        m_FastechComm.rx_packet.length = 4;// World IO Data 길이 4
        if (m_FastechComm.rx_packet.data != nullptr)
        {
          delete m_FastechComm.rx_packet.data;
          m_FastechComm.rx_packet.data = nullptr;
        }
        m_FastechComm.rx_packet.data = new uint8_t[FASTECH_CMD_MAX_DATA_LENGTH]{};
        m_FastechComm.index = 0;

        util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);
        m_CmdState = cmd_state::wait_data;
      }
      break;
      default:
        m_CmdState = cmd_state::wait_stx;
        break;
      }

    }
      break;
    case jL_fastechComm::cmd_state::wait_data:
    {
      if (data == STUFFING_BYTE)
      {
        m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
        //util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);
        // Data 넣기를 건너뛴다
        //m_FastechComm.rx_packet.data[m_FastechComm.index++] = data;       
        m_QueueRead.GetByte(&data); 
        m_FastechComm.rx_packet.data[m_FastechComm.index++] = data;
        m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;

        util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);
      }
      else
      {
        m_FastechComm.rx_packet.data[m_FastechComm.index++] = data;
        m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;

        util::crc16_put(&m_FastechComm.rx_packet.check_sum, data);
        
        // AA EE 확인하여 Data 끝을 확인
        bool is_etx = (m_QueueRead[m_QueueRead.m_Tail + 2] == CMD_ETX1) 
                    && (m_QueueRead[m_QueueRead.m_Tail + 3] == CMD_ETX2);
        if (is_etx)  
        {
          wait_next = false;
          m_CmdState = cmd_state::wait_checksum;
        }

        if (m_FastechComm.index == m_FastechComm.rx_packet.length)
        {
          wait_next = false;
          m_CmdState = cmd_state::wait_checksum;
        }
      }

    }
      break;
    case jL_fastechComm::cmd_state::wait_checksum:
    {
      if (wait_next)
      {
        wait_next = 0;
        m_FastechComm.rx_packet.check_sum_recv |= data << 8;
        m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
        m_CmdState = cmd_state::wait_etx;
      }
      else
      {
        wait_next = 1;
        m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
        m_FastechComm.rx_packet.check_sum_recv = data;
      }
    }
      break;
    case jL_fastechComm::cmd_state::wait_etx:
    {
      if (data == CMD_ETX1)
      {
        wait_next = 1;
        m_FastechComm.rx_packet.buffer[m_FastechComm.packet_size++] = data;
        break;
      }
      if (wait_next)
      {
        wait_next = 0;
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
   
    CString rec_text;
    TCHAR rec_buff[80] = { 0, };
    char char_temp[80] = { 0, };

    for (uint16_t i = 0; i < m_FastechComm.packet_size; i++)
    {
      sprintf_s(char_temp, sizeof(char_temp), "%s%02X "
        , char_temp
        , m_FastechComm.rx_packet.buffer[i]);
    }

    trans::CharToLPTSTR((LPSTR)&char_temp[0], &rec_buff[0], 80); 

    switch (m_FastechComm.rx_packet.type)
    {
    case 0xF0: // output response
    {
    }
    break;
    case 0xF1: // input status
    {
      m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[0], 0);
      m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[1], 1);
      m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[2], 2);
      m_pRegisterIO->SetRegisterIn(m_FastechComm.rx_packet.data[3], 3);
    }
    break;
    }
  
  }

exit_parsing:
  return 0;
}

void jL_fastechComm::UpdateReg()
{
  byte packet[80] = { 0 };
  uint32_t crc = 0xffff;
  int index = 0;
  packet[index++] = CMD_STX1;
  packet[index++] = CMD_STX2;

  packet[index] = 0; // id
  util::crc16_put(&crc, packet[index++]);
  packet[index] = 0xf0; // cmd
  util::crc16_put(&crc, packet[index++]);

  packet[index] = m_pRegisterIO->outState[0].GetValue(); // data1
  if (packet[index] == STUFFING_BYTE)
    packet[++index] = STUFFING_BYTE;
  util::crc16_put(&crc, packet[index++]);
  packet[index] = m_pRegisterIO->outState[1].GetValue(); // data1
  if (packet[index] == STUFFING_BYTE)
    packet[++index] = STUFFING_BYTE;
  util::crc16_put(&crc, packet[index++]);
  packet[index] = m_pRegisterIO->outState[2].GetValue(); // data1
  if (packet[index] == STUFFING_BYTE)
    packet[++index] = STUFFING_BYTE;
  util::crc16_put(&crc, packet[index++]);
  packet[index] = m_pRegisterIO->outState[3].GetValue(); // data1
  if (packet[index] == STUFFING_BYTE)
    packet[++index] = STUFFING_BYTE;
  util::crc16_put(&crc, packet[index++]);
#if 0
  packet[index] = 0xff; // data1
  util::crc16_put(&crc, packet[index++]);
  packet[index] = 0xff; // data2
  util::crc16_put(&crc, packet[index++]);
  packet[index] = 0x00; // data4
  util::crc16_put(&crc, packet[index++]);
  packet[index] = 0x00; // data4
  util::crc16_put(&crc, packet[index++]);
#endif
  packet[index++] = crc & 0xff;
  packet[index++] = (crc >> 8) & 0xff;

  packet[index++] = CMD_ETX1;
  packet[index++] = CMD_ETX2;

  WriteByte(&packet[0], index);
}

DWORD jL_fastechComm::ThreadRun()
{
  CString altstr;
  // 포트 감시 스레드 생성
  this->m_hThread = CreateThread(
    (LPSECURITY_ATTRIBUTES)NULL,
    0,
    (LPTHREAD_START_ROUTINE)ioThread,
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
  ClosePort();

  return 0;
}
