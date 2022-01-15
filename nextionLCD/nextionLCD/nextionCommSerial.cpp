#include "pch.h"
#include "nextionCommSerial.h"

nextionCommSerial::nextionCommSerial()
{
  clearVariable();

  m_pEvent = new CEvent(FALSE, TRUE);
}

nextionCommSerial::~nextionCommSerial()
{
  destrory();
}

uint32_t nextionCommSerial::initPort(DWORD baudrate)
{
	DWORD dwError;
	uint32_t err_code = 0;
	COMMTIMEOUTS timeouts = {};
	DCB dcb = {};
	if (m_IsConnected == FALSE)
		return 0;

	//ClearCommError(m_hComm, &dwError, NULL);
	//SetupComm(m_hComm, COMM_BUFF_MAX, COMM_BUFF_MAX);
	//PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);


	//dcb - Serial Port의 특성을 설정
	while (1)
	{
		dcb.DCBlength = sizeof(DCB);
		if (GetCommState(m_hComm, &dcb) == FALSE)
		{
			printf("Error GetCommState\n");
			err_code = 1;
			break;
		}

		// Set baudrate
		dcb.BaudRate = baudrate;
		dcb.ByteSize = 8;               // Data bit = 8bit
		dcb.Parity = NOPARITY;          // No parity
		dcb.StopBits = ONESTOPBIT;      // Stop bit = 1
		dcb.fParity = TRUE;             // Parity check
		dcb.fBinary = TRUE;             // Binary mode
		dcb.fNull = 0;                  // Get Null byte
		dcb.fAbortOnError = 0;
		dcb.fErrorChar = 0;
		// Not using XOn/XOff
		dcb.fOutX = 0;
		dcb.fInX = 0;
		// Not using H/W flow control
		dcb.fDtrControl = DTR_CONTROL_DISABLE;
		dcb.fRtsControl = RTS_CONTROL_DISABLE;
		dcb.fDsrSensitivity = 0;
		dcb.fOutxDsrFlow = 0;
		dcb.fOutxCtsFlow = 0;

		if (SetCommState(m_hComm, &dcb) == FALSE)
		{
			DWORD dwError = GetLastError();
			err_code = 2;

			printf("SetCommState err: %d\n", (int)dwError);
			break;
		}

		if (SetCommMask(m_hComm, 0) == FALSE) // Not using Comm event
		{
			err_code = 3;
			break;
		}
		if (SetupComm(m_hComm, COMM_BUFF_MAX, COMM_BUFF_MAX) == FALSE) // Buffer size (Rx,Tx)
		{
			err_code = 4;
			break;
		}
		if (PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR) == FALSE) // Clear buffer
		{
			err_code = 5;
			break;
		}
		if (ClearCommError(m_hComm, &dwError, NULL) == FALSE)
		{
			err_code = 6;
			break;
		}

		if (GetCommTimeouts(m_hComm, &timeouts) == FALSE)
		{
			err_code = 7;
			break;
		}
		// Timeout (Not using timeout)
		// Immediatly return
		timeouts.ReadIntervalTimeout = MAXDWORD;
		timeouts.ReadTotalTimeoutMultiplier = 0;
		timeouts.ReadTotalTimeoutConstant = 1; // must not be zero.
		timeouts.WriteTotalTimeoutMultiplier = 0;
		timeouts.WriteTotalTimeoutConstant = 1000;
		if (SetCommTimeouts(m_hComm, &timeouts) == FALSE)
		{
			err_code = 8;
			break;
		}
		EscapeCommFunction(m_hComm, SETRTS);
		EscapeCommFunction(m_hComm, SETDTR);
		break;
	}

	SetCommMask(m_hComm, EV_RXCHAR);

	return err_code;
}

DWORD nextionCommSerial::CommWatchProc(LPVOID pParam)
{
	DWORD       dwEvtMask;
	nextionCommSerial* p_this = (nextionCommSerial*)pParam;
	OVERLAPPED  os;
	BOOL ret = TRUE; // 리턴값

	BYTE buff[(COMM_RS232_BUFF_SIZE / 2)]; // InQueue의 데이터를 읽어오기 위한 Input Buffer
	memset(buff, 0x00, COMM_RS232_BUFF_SIZE / 2);
	DWORD dwRead = 0; // 읽은 바이트수

		// Event , Overap Structure 설정 

	memset(&os, 0, sizeof(OVERLAPPED));
	if (!(os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		ret = FALSE;
	if (!SetCommMask(p_this->m_hComm, EV_RXCHAR))
		ret = FALSE;
	if (!ret)
	{
		CString altstr;
		CString portname = p_this->m_PortName;
		altstr.Format(_T("Error while creating ThreadWatchComm, %s"), portname.GetString());
		AfxMessageBox(altstr);
		return FALSE;
	}

	while (p_this->m_IsConnected)
	{
		dwEvtMask = 0;

		// Event가 발생할 때 까지 블럭
		WaitCommEvent(p_this->m_hComm, &dwEvtMask, NULL);

		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			// 포트에서 읽을 수 있는 만큼 읽는다.
			// 
#if 1
			do
			{
				dwRead = p_this->ReadByte(buff, 2048);

				//CommThead의 Read Queue가 꽉 차 있는지 검사
				if (COMM_RS232_BUFF_SIZE - p_this->m_QueueRead.GetSize() > (int)dwRead)
				{
					for (WORD i = 0; i < dwRead; i++)
						p_this->m_QueueRead.PutByte(buff[i]);
				}
				else
					AfxMessageBox(_T("m_QueueRead FULL!"));

			} while (dwRead);
			
			if (p_this->CallFunc)
			{
				(*p_this->CallFunc)(p_this->CallFuncObj);
			}

			if (p_this->m_QueueRead.m_Head != p_this->m_QueueRead.m_Tail)
				::PostMessage(p_this->m_hCommWnd, WM_COMM_RS232_MESSAGE, p_this->m_PortID, 0);

#endif
		}
		else
		{
			printf("<Other Event>\r\n");
		}
	}


	CloseHandle(os.hEvent);

	p_this->m_ThreadId = 0;
	p_this->m_hThreadWatchComm = NULL;

	return TRUE;
}

uint32_t nextionCommSerial::OpenPort(CString portname, WORD port_no, DWORD baudrate, HWND hwnd)
{
	if (m_IsConnected)
		return 0;

	uint32_t err_code = 0;

	m_hCommWnd = hwnd;
	m_Baudrate = baudrate;
	m_PortID = port_no; // COM1 -> 0 , COM2->1,,,,


	CString altstr;

	//1. Open Port
	//portname = L"\\\\.\\" + portname;
	m_PortName = L"//./" + portname;
	m_hComm = CreateFile(m_PortName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		printf("Error opening serial port!\n");
		altstr.Format(_T("Error opening serial port! %s"), portname.GetString());
		AfxMessageBox(altstr);
		err_code = 2;
		goto exit_OpenPort;
	}

	m_IsConnected = TRUE;

	err_code = initPort(baudrate);
	if (err_code != 0)
	{
		altstr.Format(_T("Error port initial! %s"), portname.GetString());
		AfxMessageBox(altstr);
		goto exit_OpenPort;
	}

	// overlapped structure 변수 초기화
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	if (!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_osRead.hEvent != 0) CloseHandle(m_osRead.hEvent);
		altstr.Format(_T("Error overlapped initial! %s"), portname.GetString());
		AfxMessageBox(altstr);
		err_code = 11;
		goto exit_OpenPort;
	}

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_osWrite.hEvent != 0) CloseHandle(m_osWrite.hEvent);
		altstr.Format(_T("Error overlapped initial! %s"), portname.GetString());
		AfxMessageBox(altstr);
		err_code = 12;
		goto exit_OpenPort;
	}

	// 포트 감시 스레드 생성
	m_hThreadWatchComm = CreateThread(
		(LPSECURITY_ATTRIBUTES)NULL,
		0,
		(LPTHREAD_START_ROUTINE)CommWatchProc,
		(LPVOID)this,
		0,
		&m_ThreadId
	);

	if (!m_hThreadWatchComm)
	{
		// 실패하면 포트를 닫는다.
		ClosePort();
		altstr.Format(_T("Error create thread for watch comm! %s"), portname.GetString());
		AfxMessageBox(altstr);
		err_code = 13;
		goto exit_OpenPort;
	}

	// 장치에 DTR(Data-Terminal-Ready)을 알린다
	EscapeCommFunction(m_hComm, SETDTR);


exit_OpenPort:

	return err_code;
}

void nextionCommSerial::ClosePort()
{
	if (m_hComm == nullptr) return;

	m_IsConnected = false;

	// 모든 Event mask를 없앤다.
	SetCommMask(m_hComm, 0);

	while (m_ThreadId != 0);

	EscapeCommFunction(m_hComm, CLRDTR);

	// 통신 Queue들을 초기화 한다.
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	// 파일 핸들을 닫는다.
	CloseHandle(m_hComm);
}

void nextionCommSerial::SetCallback(void* obj, Callback_t cb)
{
	CallFunc = cb;
	CallFuncObj = obj;
}

DWORD nextionCommSerial::ReadByte(BYTE* resp, UINT size)
{
	CSingleLock lockObj((CSyncObject*)m_pEvent, FALSE);

	DWORD dwRead, dwError, dwErrorFlags;
	COMSTAT comstat;

	ClearCommError(m_hComm, &dwError, &comstat);
	// input Queue에 들어와 있는 데이터의 길이
	dwRead = comstat.cbInQue;
	//lockObj.Lock();
	if (dwRead > 0)
	{
		if (!ReadFile(m_hComm, resp, size, &dwRead, &m_osRead))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				// 입출력중인 데이터가 있는 경우 timeout에 정해준 시간만큼 기다려준다.
				while (!GetOverlappedResult(m_hComm, &m_osRead, &dwRead, TRUE))
				{
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)
					{
						ClearCommError(m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else
			{
				dwRead = 0;
				ClearCommError(m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}
	//lockObj.Unlock();

	return dwRead;
}

DWORD nextionCommSerial::WriteByte(BYTE* sendbyte, UINT size)
{
	DWORD dwWritten, dwError, dwErrorFlags;
	COMSTAT comstat;

	if (!WriteFile(m_hComm, sendbyte, (DWORD)size, &dwWritten, &m_osWrite))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			//읽을 문자가 남아 있지거나 전송할 문자가 남아 있으 경우
			// Overapped IO의 
			// 특성에 따라 ERROR_IO_PENDING 에러 메시지가 전달된다.
			//timeout에 정해준 시간만큼 기다려준다.

			while (!GetOverlappedResult(m_hComm, &m_osWrite, &dwWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError(m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError(m_hComm, &dwErrorFlags, &comstat);
		}
	}
	return dwWritten;
}

void nextionCommSerial::AddStringPortList(CComboBox* combobox)
{
	if (combobox == NULL) return;

	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM 에  시리얼포트 번호들이 등록되어 있음.  
	//레지스터 읽어오면 됨. 코드는 아래 링크에서 가져옴  
	HKEY hKey;

	//오픈할 레지스터 키에 대한 기본키 이름  
	//오픈할 레지스터 서브키 이름  
	//레지스터키에 대한 핸들  
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[20] = { 0, }, szName[100] = { 0, };
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	combobox->ResetContent();


	//LPCTSTR sData, sName;

	//hKey - 레지스터키 핸들  
	//index - 값을 가져올 인덱스.. 다수의 값이 있을 경우 필요  
	//szName - 항목값이 저장될 배열  
	//dwSize - 배열의 크기  
	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
	{
		index++;

		//szName-레지터스터 항목의 이름  
		//dwType-항목의 타입, 여기에서는 널로 끝나는 문자열  
		//szData-항목값이 저장될 배열  
		//dwSize2-배열의 크기  
		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		combobox->AddString(CString(szData));

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;
	}

	RegCloseKey(hKey);
}

void nextionCommSerial::AddStringBaudrateList(CComboBox* combobox)
{
	if (combobox == NULL) return;
	combobox->ResetContent();

	int index = 0;
	while (RS_232_BAUDRATE_DATA)
	{
		if (index >= COMM_RS232_BAUD_LIST_MAX) break;
		combobox->AddString(RS_232_BAUDRATE_DATA[index].label);
		combobox->SetItemData(index, RS_232_BAUDRATE_DATA[index].baudrate_data);
		index++;
	}
	combobox->SetCurSel(index - 1);
}
