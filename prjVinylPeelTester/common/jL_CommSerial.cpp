#include "pch.h"
#include "jL_CommSerial.h"

jL_CommSerial::jL_CommSerial()
{
	m_hComm = nullptr;
	m_hCommWnd = nullptr;
	m_PortID = NULL;
	m_PortName.Empty();
	m_Baudrate = 0;
	m_IsConnected = FALSE;
	m_osRead = {};
	m_osWrite = {};

	m_hThreadWatchComm = {};
	m_ThreadId = 0;

	m_UserMessage = 0;
	m_pQueueRead = nullptr;
	m_pEvent = nullptr;

	CallFunc = nullptr;
	CallFuncObj = nullptr;

	//
	m_pQueueRead  = new jL_CommSerial::_Queue;
	// ���� �̺�Ʈ 
	m_pEvent = new CEvent(FALSE, TRUE);
}

jL_CommSerial::~jL_CommSerial()
{
	if (m_IsConnected)
		ClosePort();

	if (m_pEvent != nullptr) 
		delete m_pEvent;

	if (m_pQueueRead != nullptr)
		delete m_pQueueRead;

	m_hComm = nullptr;
	m_hCommWnd = nullptr;
	m_PortID = NULL;
	m_PortName.Empty();
	m_Baudrate = 0;
	m_IsConnected = FALSE;
	m_osRead = {};
	m_osWrite = {};
	m_hThreadWatchComm = {};
	m_ThreadId = 0;
	m_UserMessage = 0;

	m_pQueueRead = nullptr;
	m_pEvent = nullptr;
}

jL_CommSerial::error_code jL_CommSerial::initPort(DWORD baudrate)
{
	DWORD dwError;
	error_code err_code = error_code::Success;
	COMMTIMEOUTS timeouts = {};
	DCB dcb = {};
	if (m_IsConnected == FALSE)
		return error_code::emptyPort;

	//dcb - Serial Port�� Ư���� ����
	while (1)
	{
		dcb.DCBlength = sizeof(DCB);
		if (GetCommState(m_hComm, &dcb) == FALSE)
		{
			printf("Error GetCommState\n");
			err_code = error_code::GetCommState;
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
			err_code = error_code::SerCommState;

			printf("SetCommState err: %d\n", (int)dwError);
			break;
		}

		if (SetCommMask(m_hComm, 0) == FALSE) // Not using Comm event
		{
			err_code = error_code::SetCommMask;
			break;
		}
		if (SetupComm(m_hComm, COMM_BUFF_MAX, COMM_BUFF_MAX) == FALSE) // Buffer size (Rx,Tx)
		{
			err_code = error_code::SetupComm;
			break;
		}
		if (PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR) == FALSE) // Clear buffer
		{
			err_code = error_code::PurgeComm;
			break;
		}
		if (ClearCommError(m_hComm, &dwError, NULL) == FALSE)
		{
			err_code = error_code::ClearCommError;
			break;
		}

		if (GetCommTimeouts(m_hComm, &timeouts) == FALSE)
		{
			err_code = error_code::GetCommTimeouts;
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
			err_code = error_code::SetCommTimeouts;
			break;
		}
		EscapeCommFunction(m_hComm, SETRTS);
		EscapeCommFunction(m_hComm, SETDTR);
		break;
	}
	SetCommMask(m_hComm, EV_RXCHAR);


	return err_code;
}

UINT jL_CommSerial::CommWatchThread(LPVOID pParam)
{
	DWORD       dwEvtMask;
	jL_CommSerial* pThis = (jL_CommSerial*)pParam;
	OVERLAPPED  os;
	BOOL ret = TRUE; // ���ϰ�

	BYTE buff[(COMM_RS232_BUFF_SIZE / 2)]; // InQueue�� �����͸� �о���� ���� Input Buffer
	memset(buff, 0x00, COMM_RS232_BUFF_SIZE / 2);
	DWORD dwRead = 0; // ���� ����Ʈ��

		// Event , Overap Structure ���� 

	memset(&os, 0, sizeof(OVERLAPPED));
	if (!(os.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
		ret = FALSE;
	if (!SetCommMask(pThis->m_hComm, EV_RXCHAR))
		ret = FALSE;
	if (!ret)
	{
		CString altstr;
		CString portname = pThis->m_PortName;
		altstr.Format(_T("Error while creating ThreadWatchComm, %s"), portname.GetString());
		AfxMessageBox(altstr);
		return FALSE;
	}
	TRACE("CommWatch Thread Start\n");
	while (pThis->m_IsConnected)
	{
		dwEvtMask = 0;

		// Event�� �߻��� �� ���� ��
		WaitCommEvent(pThis->m_hComm, &dwEvtMask, NULL);

		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			do
			{
				dwRead = pThis->ReadByte(buff, 2048);

				//CommThead�� Read Queue�� �� �� �ִ��� �˻�
				if (COMM_RS232_BUFF_SIZE - pThis->m_pQueueRead->GetSize() > (int)dwRead)
				{
					for (WORD i = 0; i < dwRead; i++)
						pThis->m_pQueueRead->PutByte(buff[i]);
				}
				else
					AfxMessageBox(_T("m_QueueRead FULL!"));

			} while (dwRead);

			if (pThis->m_UserMessage >= 0x0400 && pThis->m_UserMessage < (0x0400 + WM_COMM_POST_USER_MAX))
			{
				if (pThis->m_pQueueRead->m_Head != pThis->m_pQueueRead->m_Tail)
					::PostMessage(pThis->m_hCommWnd, pThis->m_UserMessage, pThis->m_PortID, 0);
			}
			if (pThis->CallFunc)
			{
				(*pThis->CallFunc)(pThis->CallFuncObj);
			}

		}
		else
		{
			TRACE("Other Event\n");
		}
	}


	CloseHandle(os.hEvent);
	TRACE("CommWatch Thread Terminatet\n");
	pThis->m_ThreadId = 0;
	pThis->m_hThreadWatchComm = NULL;

	return TRUE;
}

void jL_CommSerial::SetMessage(UINT user_msg)
{
	m_UserMessage = user_msg;
}

void jL_CommSerial::HandleClose()
{
	if (m_IsConnected) {
		ClosePort();
	}
	CloseHandle(m_osRead.hEvent);
	CloseHandle(m_osWrite.hEvent);
}

void jL_CommSerial::ClosePort()
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
	if (m_hComm != nullptr)
	{
		CloseHandle(m_hComm);
		m_hComm = nullptr;
	}
}

int jL_CommSerial::OpenPort(CString portname,  DWORD baudrate, WORD port_no, HWND hwnd)
{
	if (m_IsConnected)
		return static_cast<int>(error_code::existPort);

	error_code err_code = error_code::Success;

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
		err_code = error_code::invalidHandleValue;
		goto exit_OpenPort;
	}

	m_IsConnected = TRUE;

	err_code = initPort(baudrate);
	if (err_code != error_code::Success)
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
		err_code = error_code::failCreateEventRead;
		goto exit_OpenPort;
	}

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_osWrite.hEvent != 0) CloseHandle(m_osWrite.hEvent);
		altstr.Format(_T("Error overlapped initial! %s"), portname.GetString());
		AfxMessageBox(altstr);
		err_code = error_code::failCreateEventWrite;
		goto exit_OpenPort;
	}

	// ��Ʈ ���� ������ ����
	m_hThreadWatchComm = CreateThread(
		(LPSECURITY_ATTRIBUTES)NULL,
		0,
		(LPTHREAD_START_ROUTINE)CommWatchThread,
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
		err_code = error_code::failCreateThread;
		goto exit_OpenPort;
	}

	// ��ġ�� DTR(Data-Terminal-Ready)�� �˸���
	EscapeCommFunction(m_hComm, SETDTR);


exit_OpenPort:

	return static_cast<int>(err_code);
}

DWORD jL_CommSerial::ReadByte(BYTE* resp, UINT size)
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

	return dwRead;
}

DWORD jL_CommSerial::WriteByte(BYTE* sendbyte, UINT size)
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
}

void jL_CommSerial::SetAttachCallbackFunc(void* obj, Callback_t cb)
{
	CallFunc = cb;
	CallFuncObj = obj;
}

void jL_CommSerial::AddStringPortList(CComboBox* combobox)
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
	combobox->SetCurSel(index - 1);
	RegCloseKey(hKey);
}

void jL_CommSerial::AddStringBaudrateList(CComboBox* combobox)
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
