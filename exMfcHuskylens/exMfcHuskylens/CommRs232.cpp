#include "pch.h"
#include "CommRs232.h"

// 메시지를 받을 윈도 핸들, 이 클래스를 사용하는 윈도에서 HWND hCommwnd this-> m_hwnd 로 설정
//CWnd* pWnd = AfxGetMainWnd();
//HWND hCommWnd ;

CCommRs232::CCommRs232()
{
	clearVariable();

	m_pEvent = new CEvent(FALSE, TRUE);
}

void CCommRs232::AddPortList(CComboBox* combobox)
{
	if (combobox == NULL) return;

	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM 에  시리얼포트 번호들이 등록되어 있음.  
	//레지스터 읽어오면 됨. 코드는 아래 링크에서 가져옴  
	HKEY key;

	//오픈할 레지스터 키에 대한 기본키 이름  
	//오픈할 레지스터 서브키 이름  
	//레지스터키에 대한 핸들  
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &key);

	TCHAR reg_data[20], reg_name[100];
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	combobox->ResetContent();
	memset(reg_data, 0x00, sizeof(reg_data));
	memset(reg_name, 0x00, sizeof(reg_name));



	//LPCTSTR sData, sName;

	//Key		- 레지스터키 핸들  
	//index		- 값을 가져올 인덱스.. 다수의 값이 있을 경우 필요  
	//reg_Name	- 항목값이 저장될 배열  
	//dwSize	- 배열의 크기  
	while (ERROR_SUCCESS == RegEnumValue(key, index, reg_name, &dwSize, NULL, NULL, NULL, NULL))
	{
		index++;

		//reg_name	-레지터스터 항목의 이름  
		//dwType	-항목의 타입, 여기에서는 널로 끝나는 문자열  
		//reg_data	-항목값이 저장될 배열  
		//dwSize2	-배열의 크기  
		RegQueryValueEx(key, reg_name, NULL, &dwType, (LPBYTE)reg_data, &dwSize2);
		combobox->AddString(CString(reg_data));

		memset(reg_data, 0x00, sizeof(reg_data));
		memset(reg_name, 0x00, sizeof(reg_name));
		dwSize = 100;
		dwSize2 = 20;
	}
	combobox->SetCurSel(index-1);

	RegCloseKey(key);
}

void CCommRs232::AddBaudrateList(CComboBox* combobox)
{
	if (combobox == NULL) return;
	int index = 0;
	while(RS_232_BAUDRATE_DATA)
	{
		if (index >= COMM_RS232_BAUD_LIST_MAX) break;
		combobox->AddString(RS_232_BAUDRATE_DATA[index].label);
		combobox->SetItemData(index, RS_232_BAUDRATE_DATA[index].baudrate_data);
		index++;
	}
	combobox->SetCurSel(index -1);
}

void CCommRs232::ResetPort()
{
	if (m_IsConnected == FALSE) return;

	DWORD dwError;
	
	ClearCommError(m_hComm, &dwError, NULL);
	SetupComm(m_hComm, COMM_BUFF_MAX, COMM_BUFF_MAX);
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// Timeout Value를 설정하기 위한 구조체
	COMMTIMEOUTS timeouts = {};
	// set up for overlapped I/O
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	// CBR_9600 is approximately 1byte/ms. For our purposes, allow
	// double the expected time per character for a fudge factor.
	timeouts.WriteTotalTimeoutConstant = 1000;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	SetCommTimeouts(m_hComm, &timeouts);
	

	// dcb설정 Serial Port의 특성을 설정하기 위한 구조체
	DCB dcb;
	memset(&dcb, 0x00, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComm, &dcb);

	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
	dcb.BaudRate = m_Baudrate;

	dcb.Parity = NOPARITY;// 0;
	dcb.ByteSize = 8;	
	dcb.StopBits = ONESTOPBIT;// 1;

	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fOutxCtsFlow = FALSE;

	// Xon , Xoff 사용 // Xon , Xoff 사용하지 않을 경우는 CriticalSection을 설정 하여 보호해 주어야 한다.
	dcb.fInX = FALSE;
	dcb.fOutX = FALSE;
	dcb.XonLim = 2048;
	dcb.XoffLim = 1024;

	//dcb.XonChar = COMM_RS232_ASCII_XON;
	//dcb.XonChar = COMM_RS232_ASCII_XOFF;
	

	SetCommState(m_hComm, &dcb);

	SetCommMask(m_hComm, EV_RXCHAR);

}

BOOL CCommRs232::OpenPort(CString port_name, DWORD baud, WORD port_no, HWND hwnd)
{
	m_hCommWnd = hwnd;
	m_Baudrate = baud;

#if COMM_USE_MY_QUEUE
	DWORD dwThreadID; // Thread ID를 얻음
	m_IsConnected = FALSE;
	m_wPortID = port_no; // COM1 -> 0 , COM2->1,,,,

	// 포트 열기
	m_PortName = L"//./" + port_name;
	m_hComm = CreateFile(m_PortName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (m_hComm != INVALID_HANDLE_VALUE)
		m_IsConnected = TRUE;
	else
		return FALSE; // 포트 열기 실패

	ResetPort();

	// overlapped structure 변수 초기화
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	if (!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_osRead.hEvent != 0) CloseHandle(m_osRead.hEvent);
		return FALSE;
	}

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_osWrite.hEvent != 0) CloseHandle(m_osWrite.hEvent);
		return FALSE;
	}

	// 포트 감시 스레드 생성
	m_hThreadWatchComm = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadWatchComm,
		this,
		0,
		&dwThreadID
	);
	EscapeCommFunction(m_hComm, SETDTR);

	if (!m_hThreadWatchComm)
	{
		// 실패하면 포트를 닫는다.
		ClosePort();
		return FALSE;
	}


#if 0

	// 변수 설정

	// overlapped structure 변수 초기화
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;

	if (!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_osRead.hEvent != 0) CloseHandle(m_osRead.hEvent);
		return FALSE;
	}

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_osWrite.hEvent != 0) CloseHandle(m_osWrite.hEvent);
		return FALSE;
	}

	// 포트 열기
	m_PortName = L"//./" + port_name;
	m_hComm = CreateFile(m_PortName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (m_hComm == (HANDLE)-1) return FALSE; // 포트 열기 실패

	// EV_RXCHAR event 설정
	SetCommMask(m_hComm, EV_RXCHAR);

	// Serail Port 장치의 InQueue, OutQueue 크기 설정
	SetupComm(m_hComm, COMM_BUFF_MAX, COMM_BUFF_MAX);

	// 포트 InQueue , OutQueue 초기화
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// Timeout Value를 설정하기 위한 구조체
	COMMTIMEOUTS timeouts = {};
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 2 * CBR_9600 / baud;
	if (SetCommTimeouts(m_hComm, &timeouts))
	{
		ClosePort();
		return FALSE;
	}

	// dcb설정 Serial Port의 특성을 설정하기 위한 구조체
	DCB dcb = {};
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComm, &dcb);
	dcb.BaudRate = baud;
	dcb.ByteSize = 8;
	dcb.Parity = 0;
	dcb.StopBits = 1;
	dcb.fInX = dcb.fOutX = 1; // Xon , Xoff 사용 // Xon , Xoff 사용하지 않을 경우는 CriticalSection을 설정 하여 보호해 주어야 한다.

	dcb.XonChar = COMM_RS232_ASCII_XON;
	dcb.XonChar = COMM_RS232_ASCII_XOFF;
	dcb.XonLim = 100;
	dcb.XoffLim = 100;

	if (!SetCommState(m_hComm, &dcb))
	{
		ClosePort();
		return FALSE;
	}

	// 포트 감시 스레드 생성
	m_IsConnected = TRUE; // 연결이 성립되었음을 세팅
	m_hThreadWatchComm = CreateThread(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)ThreadWatchComm,
		this,
		0,
		&dwThreadID
	);
	EscapeCommFunction(m_hComm, SETDTR);

	if (!m_hThreadWatchComm)
	{
		// 실패하면 포트를 닫는다.
		ClosePort();
		return FALSE;
	}

#endif

#else
	// 포트 열기
	m_PortName = L"//./" + port_name;
	m_hComm = CreateFile(m_PortName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (m_hComm != INVALID_HANDLE_VALUE)
		m_IsConnected = TRUE;
	else
		return FALSE; // 포트 열기 실패

	ResetPort();

	// overlapped structure 변수 초기화
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	if (!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_osRead.hEvent != 0) CloseHandle(m_osRead.hEvent);
		return FALSE;
	}

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_osWrite.hEvent != 0) CloseHandle(m_osWrite.hEvent);
		return FALSE;
	}

	AfxBeginThread(CommThread, (LPVOID)this);
	EscapeCommFunction(m_hComm, SETDTR);


#endif
	// ####

	return TRUE;
}

void CCommRs232::ClosePort()
{

	if (m_hComm == nullptr) return;

	m_IsConnected = FALSE;
	// 모든 Event mask를 없앤다.
	SetCommMask(m_hComm, 0);
	// 통신 Queue들을 초기화 한다.
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	// 파일 핸들을 닫는다.
	CloseHandle(m_hComm);

}



DWORD CCommRs232::WriteComm(LPCTSTR buff, int write_len)
{

	DWORD dwWritten, dwError, dwErrorFlags;
	COMSTAT comstat;

	if (!WriteFile(m_hComm, buff, write_len, &dwWritten, &m_osWrite))
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

DWORD CCommRs232::ReadComm(BYTE* pbuff, DWORD read_len)
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
		if (!ReadFile(m_hComm, pbuff, read_len, &dwRead, &m_osRead))
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




// 포트를 감시하고, Serial Port에서 메시지가 발생하면 입력 메시지인가 검사하고 내용이
// 있으면 m_ReadData에 저장한 뒤에 MainWnd에 메시지를 보내어 버퍼의 내용을
// 읽어 가라고 WM_COMM_READ 메시지를 보낸다.

DWORD ThreadWatchComm(CCommRs232* pComm)
{
	DWORD dwEvent;
	OVERLAPPED os;
	BOOL ret = TRUE; // 리턴값

	BYTE buff[(COMM_RS232_BUFF_SIZE/2)]; // InQueue의 데이터를 읽어오기 위한 Input Buffer
	memset(buff, 0x00, COMM_RS232_BUFF_SIZE/2);
	DWORD dwRead = 0; // 읽은 바이트수

	// Event , Overap Structure 설정 

	memset(&os, 0, sizeof(OVERLAPPED));
	if (!(os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		ret = FALSE;
	if (!SetCommMask(pComm->m_hComm, EV_RXCHAR))
		ret = FALSE;
	if (!ret)
	{
		CString altstr;
		CString portname = pComm->m_PortName;
		altstr.Format(_T("Error while creating ThreadWatchComm, %s"), portname.GetString());
		AfxMessageBox(altstr);
		return FALSE;
	}

	// 포트를 감시하는 루프

	while (pComm->m_IsConnected) // 주 프로세스에서 이 값을 PALSE로 바꿔주면 스레드가 끝난다.
	{
		dwEvent = 0;

		// 포트에서 메시지가 발생할 때까지 기다린다.
		WaitCommEvent(pComm->m_hComm, &dwEvent, NULL);


		if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
			// 포트에서 읽을 수 있는 만큼 읽는다.
			//::PostMessage(pComm->m_hCommWnd, WM_COMM_RS232_READ, pComm->m_wPortID, 0);
			do
			{
				dwRead = pComm->ReadComm(buff, 2048);

				//CommThead의 Read Queue가 꽉 차 있는지 검사
				if (COMM_RS232_BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for (WORD i = 0; i < dwRead; i++)
						pComm->m_QueueRead.PutByte(buff[i]);
				}
				else
					AfxMessageBox(_T("m_QueueRead FULL!"));

			} while (dwRead);

			//CEdit* p = (CEdit*)GetDlgItem(hCommWnd, 1004);

			//p->SetWindowTextW(L"test_Ok"/*(LPCTSTR)buff*/);

			TRACE(traceAppMsg, 0, "데이터를 받았습니다.\r\n");
			// 읽어가도록 메시지를 보낸다.
			::PostMessage(pComm->m_hCommWnd, WM_COMM_RS232_MESSAGE, pComm->m_wPortID, 0);
			//TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 응용 프로그램이 예기치 않게 종료됩니다.\n");
		}
	}
	// 포트가 ClosePort에 의해 닫히면 m_bConnected가 FALSE가 되어 종료

	CloseHandle(os.hEvent);
	pComm->m_hThreadWatchComm = NULL;

	return TRUE;
}


