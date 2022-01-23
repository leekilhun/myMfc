#include "pch.h"
#include "resource.h"

#include "jL_serialComPort.h"
//#include "jL_utility.h"


jL_serialComPort::jL_serialComPort()
{
	clearVariable();

	m_pEvent = new CEvent(FALSE, TRUE);
}

jL_serialComPort::~jL_serialComPort()
{
	destrory();
}



uint32_t jL_serialComPort::initPort(DWORD baudrate)
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


#if 1


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



#endif // 0



#if 0

	ClearCommError(m_hComm, &dwError, NULL);
	SetupComm(m_hComm, COMM_BUFF_MAX, COMM_BUFF_MAX);
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

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

#endif

	return err_code;
}

uint32_t jL_serialComPort::OpenPort(CString portname, WORD port_no, DWORD baudrate /*= CBR_9600*/, HWND hwnd /*= NULL*/)
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

uint32_t jL_serialComPort::AvailableData()
{
	uint32_t ret = 0;
	int32_t length = 0;
	DWORD dwRead = 0;
	uint8_t data;


	if (m_IsConnected != true)
	{
		return 0;
	}


	if (ReadFile(m_hComm, &data, (DWORD)1, &dwRead, NULL) == TRUE)
	{
		if (dwRead != 1)
		{
			length = 0;
		}
		else
		{
			length = 1;
			m_ReceivedData = data;
		}
	}
	ret = length;		

	return ret;
}

uint8_t jL_serialComPort::Read()
{
	return m_ReceivedData;
}

uint32_t jL_serialComPort::Write(uint8_t* p_data, uint32_t length)
{
	uint32_t ret = 0;
	DWORD dwWrite = 0;
	if (WriteFile(m_hComm, p_data, (DWORD)length, &dwWrite, NULL) == FALSE)
	{
		ret = 0;
	}
	else
	{
		ret = dwWrite;
	}

	return ret;
}

uint32_t jL_serialComPort::Printf(char* fmt, ...)
{
	char buf[256];
	va_list args;
	int len;
	uint32_t ret;

	va_start(args, fmt);
	len = vsnprintf(buf, 256, fmt, args);
	ret = this->Write((uint8_t*)buf, len);
	va_end(args);

	return ret;
}


void jL_serialComPort::ClosePort()
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

DWORD jL_serialComPort::ReadByte(BYTE* resp, UINT size)
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

#if 0
	BYTE buff[256] = { 0, };

	if (ReadFile(m_hComm, buff, size, &dwRead, 0))
	{
		if (dwRead == size) {
			memcpy(resp, buff, size);
			return dwRead;
		}
	}
	else
		dwRead = 0;

#endif // 0


	return dwRead;

}

DWORD jL_serialComPort::WriteByte(BYTE* sendbyte, UINT size)
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


	/*if (WriteFile(m_hComm, sendbyte, size, &m_BytesWritten, NULL) == 0)
		return false;
	else
		return true;*/
}


#if 0


bool jL_serialComPort::SetCommunicationTimeouts(timeout_t* cfg_timeout)
	//DWORD ReadIntervalTimeout, DWORD ReadTotalTimeoutMultiplier, DWORD ReadTotalTimeoutConstant, DWORD WriteTotalTimeoutMultiplier, DWORD WriteTotalTimeoutConstant)
{
	if ((m_bPortReady = GetCommTimeouts(m_hComm, &m_CommTimeouts)) == 0)
		return false;

	m_CommTimeouts.ReadIntervalTimeout					= cfg_timeout->ReadIntervalTimeout;
	m_CommTimeouts.ReadTotalTimeoutConstant			= cfg_timeout->ReadTotalTimeoutConstant;
	m_CommTimeouts.ReadTotalTimeoutMultiplier		= cfg_timeout->ReadTotalTimeoutMultiplier;
	m_CommTimeouts.WriteTotalTimeoutConstant		= cfg_timeout->WriteTotalTimeoutConstant;
	m_CommTimeouts.WriteTotalTimeoutMultiplier	= cfg_timeout->WriteTotalTimeoutMultiplier;

	m_bPortReady = SetCommTimeouts(m_hComm, &m_CommTimeouts);

	if (m_bPortReady == 0)
	{
		//MessageBox(L"StCommTimeouts function failed",L"Com Port Error",MB_OK+MB_ICONERROR);  
		printf("\nStCommTimeouts function failed\n");
		CloseHandle(m_hComm);
		return false;
	}

	return true;
}

bool jL_serialComPort::ConfigurePort(cfg_t* cfg_setup)
	//DWORD BaudRate, BYTE ByteSize, DWORD fParity, BYTE Parity, BYTE StopBits)
{
	if ((m_bPortReady = GetCommState(m_hComm, &m_dcb)) == 0)
	{
		printf("\nGetCommState Error\n");
		//"MessageBox(L, L"Error", MB_OK + MB_ICONERROR);  
		CloseHandle(m_hComm);
		return false;
	}

	m_dcb.BaudRate					= cfg_setup->BaudRate;
	m_dcb.ByteSize					= cfg_setup->WordLength;
	m_dcb.Parity						= cfg_setup->Parity;
	m_dcb.StopBits					= cfg_setup->StopBits;
	m_dcb.fBinary						= true;
	m_dcb.fDsrSensitivity		= false;
	m_dcb.fParity						= cfg_setup->fParity;
	m_dcb.fOutX							= false;
	m_dcb.fInX							= false;
	m_dcb.fNull							= false;
	m_dcb.fAbortOnError			= true;
	m_dcb.fOutxCtsFlow			= false;
	m_dcb.fOutxDsrFlow			= false;
	m_dcb.fDtrControl				= DTR_CONTROL_DISABLE;
	m_dcb.fDsrSensitivity		= false;
	m_dcb.fRtsControl				= RTS_CONTROL_DISABLE;
	m_dcb.fOutxCtsFlow			= false;
	m_dcb.fOutxCtsFlow			= false;

	m_bPortReady = SetCommState(m_hComm, &m_dcb);

	if (m_bPortReady == 0)
	{
		//MessageBox(L"SetCommState Error");  
		printf("SetCommState Error");
		CloseHandle(m_hComm);
		return false;
	}

	return true;
}

#endif



void jL_serialComPort::AddStringPortList(CComboBox* combobox)
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

void jL_serialComPort::AddStringBaudrateList(CComboBox* combobox)
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



// Event Watch 쓰레드
DWORD CommWatchProc(LPVOID pParam)
{
	DWORD       dwEvtMask;
	jL_serialComPort* pp = (jL_serialComPort*)pParam;
	OVERLAPPED  os;
	BOOL ret = TRUE; // 리턴값

	BYTE buff[(COMM_RS232_BUFF_SIZE / 2)]; // InQueue의 데이터를 읽어오기 위한 Input Buffer
	memset(buff, 0x00, COMM_RS232_BUFF_SIZE / 2);
	DWORD dwRead = 0; // 읽은 바이트수

		// Event , Overap Structure 설정 

	memset(&os, 0, sizeof(OVERLAPPED));
	if (!(os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		ret = FALSE;
	if (!SetCommMask(pp->m_hComm, EV_RXCHAR))
		ret = FALSE;
	if (!ret)
	{
		CString altstr;
		CString portname = pp->m_PortName;
		altstr.Format(_T("Error while creating ThreadWatchComm, %s"), portname.GetString());
		AfxMessageBox(altstr);
		return FALSE;
	}

	while (pp->m_IsConnected)
	{
		dwEvtMask = 0;

		// Event가 발생할 때 까지 블럭
		WaitCommEvent(pp->m_hComm, &dwEvtMask, NULL);

		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			// 포트에서 읽을 수 있는 만큼 읽는다.
			// 
#if 1
			//::PostMessage(pComm->m_hCommWnd, WM_COMM_RS232_READ, pComm->m_wPortID, 0);

			do
			{
				dwRead = pp->ReadByte(buff, 2048);

				//CommThead의 Read Queue가 꽉 차 있는지 검사
				if (COMM_RS232_BUFF_SIZE - pp->m_QueueRead.GetSize() > (int)dwRead)
				{
					for (WORD i = 0; i < dwRead; i++)
						pp->m_QueueRead.PutByte(buff[i]);
				}
				else
					AfxMessageBox(_T("m_QueueRead FULL!"));

			} while (dwRead);
			//dwRead = pp->ReadByte(buff, 2048);
			//bool is_get_bytes = dwRead > 0;
			//while (dwRead)
			//{
			//	//CommThead의 Read Queue가 꽉 차 있는지 검사
			//	if (COMM_RS232_BUFF_SIZE - pp->m_QueueRead.GetSize() > (int)dwRead)
			//	{
			//		for (DWORD i = 0; i < dwRead; i++)
			//			pp->m_QueueRead.PutByte(buff[i]);
			//	}
			//	else
			//		AfxMessageBox(_T("m_QueueRead FULL!"));
			//}

			//p->SetWindowTextW(L"test_Ok"/*(LPCTSTR)buff*/);

			//TRACE(traceAppMsg, 0, "데이터를 받았습니다.\r\n");
			// 읽어가도록 메시지를 보낸다.
			if (pp->m_QueueRead.m_Head != pp->m_QueueRead.m_Tail)
				::PostMessage(pp->m_hCommWnd, WM_COMM_RS232_MESSAGE, pp->m_PortID, 0);

			//TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 응용 프로그램이 예기치 않게 종료됩니다.\n");
#endif

#if 0
			BYTE abIn[80] = { 0 };
			do
			{
				if (dwRead = pp->ReadByte(&abIn[0], 80))
				{
					CString preText;
					CEdit* pEdit;
					char* temp = new char[dwRead + 1];
					strncpy_s(temp, 1024, (const char*)&abIn[0], (int)dwRead);
					temp[dwRead] = '\0';

					pEdit = (CEdit*)AfxGetMainWnd()->GetDlgItem(IDC_ED_INPUT);

					pEdit->GetWindowText(preText);

					/*if (pEdit->GetLineCount() > 300) {
						preText = preText.Right(strlen(preText) - (preText.FindOneOf("\n\r") + 2));
					}*/
					TCHAR lpTchar[80] = { 0, };
					trans::CharToLPTSTR(temp, lpTchar, 80);
					preText = preText + lpTchar;
					preText.Replace(L"\n\r", L"\r\n");
					pEdit->SetWindowText(preText);
					pEdit->SetSel(0, -1);
					pEdit->SetSel(-1, -1);
					//					pEdit->SetFocus();

					delete temp;
				}
			} while (dwRead);

#endif // 0


		}
		else
		{
			printf("<Other Event>\r\n");
		}
	}


	CloseHandle(os.hEvent);

	pp->m_ThreadId = 0;
	pp->m_hThreadWatchComm = NULL;

	return TRUE;

}
