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


	//dcb - Serial Port�� Ư���� ����
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


	// dcb���� Serial Port�� Ư���� �����ϱ� ���� ����ü
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

	// Xon , Xoff ��� // Xon , Xoff ������� ���� ���� CriticalSection�� ���� �Ͽ� ��ȣ�� �־�� �Ѵ�.
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

	// overlapped structure ���� �ʱ�ȭ
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

	// ��Ʈ ���� ������ ����
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
		// �����ϸ� ��Ʈ�� �ݴ´�.
		ClosePort();
		altstr.Format(_T("Error create thread for watch comm! %s"), portname.GetString());
		AfxMessageBox(altstr);
		err_code = 13;
		goto exit_OpenPort;
	}

	// ��ġ�� DTR(Data-Terminal-Ready)�� �˸���
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

	// ��� Event mask�� ���ش�.
	SetCommMask(m_hComm, 0);

	while (m_ThreadId != 0);

	EscapeCommFunction(m_hComm, CLRDTR);

	// ��� Queue���� �ʱ�ȭ �Ѵ�.
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	
	// ���� �ڵ��� �ݴ´�.
	CloseHandle(m_hComm);

}

DWORD jL_serialComPort::ReadByte(BYTE* resp, UINT size)
{	
	CSingleLock lockObj((CSyncObject*)m_pEvent, FALSE);

	DWORD dwRead, dwError, dwErrorFlags;
	COMSTAT comstat;

	ClearCommError(m_hComm, &dwError, &comstat);
	// input Queue�� ���� �ִ� �������� ����
	dwRead = comstat.cbInQue;
	//lockObj.Lock();
	if (dwRead > 0)
	{
		if (!ReadFile(m_hComm, resp, size, &dwRead, &m_osRead))
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				// ��������� �����Ͱ� �ִ� ��� timeout�� ������ �ð���ŭ ��ٷ��ش�.
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
			//���� ���ڰ� ���� �����ų� ������ ���ڰ� ���� ���� ���
			// Overapped IO�� 
			// Ư���� ���� ERROR_IO_PENDING ���� �޽����� ���޵ȴ�.
			//timeout�� ������ �ð���ŭ ��ٷ��ش�.

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

	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM ��  �ø�����Ʈ ��ȣ���� ��ϵǾ� ����.  
	//�������� �о���� ��. �ڵ�� �Ʒ� ��ũ���� ������  
	HKEY hKey;

	//������ �������� Ű�� ���� �⺻Ű �̸�  
	//������ �������� ����Ű �̸�  
	//��������Ű�� ���� �ڵ�  
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[20] = { 0, }, szName[100] = { 0, };
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	combobox->ResetContent();


	//LPCTSTR sData, sName;

	//hKey - ��������Ű �ڵ�  
	//index - ���� ������ �ε���.. �ټ��� ���� ���� ��� �ʿ�  
	//szName - �׸��� ����� �迭  
	//dwSize - �迭�� ũ��  
	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
	{
		index++;

		//szName-�����ͽ��� �׸��� �̸�  
		//dwType-�׸��� Ÿ��, ���⿡���� �η� ������ ���ڿ�  
		//szData-�׸��� ����� �迭  
		//dwSize2-�迭�� ũ��  
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



// Event Watch ������
DWORD CommWatchProc(LPVOID pParam)
{
	DWORD       dwEvtMask;
	jL_serialComPort* pp = (jL_serialComPort*)pParam;
	OVERLAPPED  os;
	BOOL ret = TRUE; // ���ϰ�

	BYTE buff[(COMM_RS232_BUFF_SIZE / 2)]; // InQueue�� �����͸� �о���� ���� Input Buffer
	memset(buff, 0x00, COMM_RS232_BUFF_SIZE / 2);
	DWORD dwRead = 0; // ���� ����Ʈ��

		// Event , Overap Structure ���� 

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

		// Event�� �߻��� �� ���� ��
		WaitCommEvent(pp->m_hComm, &dwEvtMask, NULL);

		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			// ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
			// 
#if 1
			//::PostMessage(pComm->m_hCommWnd, WM_COMM_RS232_READ, pComm->m_wPortID, 0);

			do
			{
				dwRead = pp->ReadByte(buff, 2048);

				//CommThead�� Read Queue�� �� �� �ִ��� �˻�
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
			//	//CommThead�� Read Queue�� �� �� �ִ��� �˻�
			//	if (COMM_RS232_BUFF_SIZE - pp->m_QueueRead.GetSize() > (int)dwRead)
			//	{
			//		for (DWORD i = 0; i < dwRead; i++)
			//			pp->m_QueueRead.PutByte(buff[i]);
			//	}
			//	else
			//		AfxMessageBox(_T("m_QueueRead FULL!"));
			//}

			//p->SetWindowTextW(L"test_Ok"/*(LPCTSTR)buff*/);

			//TRACE(traceAppMsg, 0, "�����͸� �޾ҽ��ϴ�.\r\n");
			// �о���� �޽����� ������.
			if (pp->m_QueueRead.m_Head != pp->m_QueueRead.m_Tail)
				::PostMessage(pp->m_hCommWnd, WM_COMM_RS232_MESSAGE, pp->m_PortID, 0);

			//TRACE(traceAppMsg, 0, "���: ��ȭ ���ڸ� ������ �������Ƿ� ���� ���α׷��� ����ġ �ʰ� ����˴ϴ�.\n");
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
