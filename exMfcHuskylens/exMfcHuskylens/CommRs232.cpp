#include "pch.h"
#include "CommRs232.h"

// �޽����� ���� ���� �ڵ�, �� Ŭ������ ����ϴ� �������� HWND hCommwnd this-> m_hwnd �� ����
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

	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM ��  �ø�����Ʈ ��ȣ���� ��ϵǾ� ����.  
	//�������� �о���� ��. �ڵ�� �Ʒ� ��ũ���� ������  
	HKEY key;

	//������ �������� Ű�� ���� �⺻Ű �̸�  
	//������ �������� ����Ű �̸�  
	//��������Ű�� ���� �ڵ�  
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &key);

	TCHAR reg_data[20], reg_name[100];
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	combobox->ResetContent();
	memset(reg_data, 0x00, sizeof(reg_data));
	memset(reg_name, 0x00, sizeof(reg_name));



	//LPCTSTR sData, sName;

	//Key		- ��������Ű �ڵ�  
	//index		- ���� ������ �ε���.. �ټ��� ���� ���� ��� �ʿ�  
	//reg_Name	- �׸��� ����� �迭  
	//dwSize	- �迭�� ũ��  
	while (ERROR_SUCCESS == RegEnumValue(key, index, reg_name, &dwSize, NULL, NULL, NULL, NULL))
	{
		index++;

		//reg_name	-�����ͽ��� �׸��� �̸�  
		//dwType	-�׸��� Ÿ��, ���⿡���� �η� ������ ���ڿ�  
		//reg_data	-�׸��� ����� �迭  
		//dwSize2	-�迭�� ũ��  
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

	// Timeout Value�� �����ϱ� ���� ����ü
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
	

	// dcb���� Serial Port�� Ư���� �����ϱ� ���� ����ü
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

	// Xon , Xoff ��� // Xon , Xoff ������� ���� ���� CriticalSection�� ���� �Ͽ� ��ȣ�� �־�� �Ѵ�.
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
	DWORD dwThreadID; // Thread ID�� ����
	m_IsConnected = FALSE;
	m_wPortID = port_no; // COM1 -> 0 , COM2->1,,,,

	// ��Ʈ ����
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
		return FALSE; // ��Ʈ ���� ����

	ResetPort();

	// overlapped structure ���� �ʱ�ȭ
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

	// ��Ʈ ���� ������ ����
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
		// �����ϸ� ��Ʈ�� �ݴ´�.
		ClosePort();
		return FALSE;
	}


#if 0

	// ���� ����

	// overlapped structure ���� �ʱ�ȭ
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

	// ��Ʈ ����
	m_PortName = L"//./" + port_name;
	m_hComm = CreateFile(m_PortName,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (m_hComm == (HANDLE)-1) return FALSE; // ��Ʈ ���� ����

	// EV_RXCHAR event ����
	SetCommMask(m_hComm, EV_RXCHAR);

	// Serail Port ��ġ�� InQueue, OutQueue ũ�� ����
	SetupComm(m_hComm, COMM_BUFF_MAX, COMM_BUFF_MAX);

	// ��Ʈ InQueue , OutQueue �ʱ�ȭ
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// Timeout Value�� �����ϱ� ���� ����ü
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

	// dcb���� Serial Port�� Ư���� �����ϱ� ���� ����ü
	DCB dcb = {};
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComm, &dcb);
	dcb.BaudRate = baud;
	dcb.ByteSize = 8;
	dcb.Parity = 0;
	dcb.StopBits = 1;
	dcb.fInX = dcb.fOutX = 1; // Xon , Xoff ��� // Xon , Xoff ������� ���� ���� CriticalSection�� ���� �Ͽ� ��ȣ�� �־�� �Ѵ�.

	dcb.XonChar = COMM_RS232_ASCII_XON;
	dcb.XonChar = COMM_RS232_ASCII_XOFF;
	dcb.XonLim = 100;
	dcb.XoffLim = 100;

	if (!SetCommState(m_hComm, &dcb))
	{
		ClosePort();
		return FALSE;
	}

	// ��Ʈ ���� ������ ����
	m_IsConnected = TRUE; // ������ �����Ǿ����� ����
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
		// �����ϸ� ��Ʈ�� �ݴ´�.
		ClosePort();
		return FALSE;
	}

#endif

#else
	// ��Ʈ ����
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
		return FALSE; // ��Ʈ ���� ����

	ResetPort();

	// overlapped structure ���� �ʱ�ȭ
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
	// ��� Event mask�� ���ش�.
	SetCommMask(m_hComm, 0);
	// ��� Queue���� �ʱ�ȭ �Ѵ�.
	PurgeComm(m_hComm, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	// ���� �ڵ��� �ݴ´�.
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

DWORD CCommRs232::ReadComm(BYTE* pbuff, DWORD read_len)
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
		if (!ReadFile(m_hComm, pbuff, read_len, &dwRead, &m_osRead))
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




// ��Ʈ�� �����ϰ�, Serial Port���� �޽����� �߻��ϸ� �Է� �޽����ΰ� �˻��ϰ� ������
// ������ m_ReadData�� ������ �ڿ� MainWnd�� �޽����� ������ ������ ������
// �о� ����� WM_COMM_READ �޽����� ������.

DWORD ThreadWatchComm(CCommRs232* pComm)
{
	DWORD dwEvent;
	OVERLAPPED os;
	BOOL ret = TRUE; // ���ϰ�

	BYTE buff[(COMM_RS232_BUFF_SIZE/2)]; // InQueue�� �����͸� �о���� ���� Input Buffer
	memset(buff, 0x00, COMM_RS232_BUFF_SIZE/2);
	DWORD dwRead = 0; // ���� ����Ʈ��

	// Event , Overap Structure ���� 

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

	// ��Ʈ�� �����ϴ� ����

	while (pComm->m_IsConnected) // �� ���μ������� �� ���� PALSE�� �ٲ��ָ� �����尡 ������.
	{
		dwEvent = 0;

		// ��Ʈ���� �޽����� �߻��� ������ ��ٸ���.
		WaitCommEvent(pComm->m_hComm, &dwEvent, NULL);


		if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
			// ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
			//::PostMessage(pComm->m_hCommWnd, WM_COMM_RS232_READ, pComm->m_wPortID, 0);
			do
			{
				dwRead = pComm->ReadComm(buff, 2048);

				//CommThead�� Read Queue�� �� �� �ִ��� �˻�
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

			TRACE(traceAppMsg, 0, "�����͸� �޾ҽ��ϴ�.\r\n");
			// �о���� �޽����� ������.
			::PostMessage(pComm->m_hCommWnd, WM_COMM_RS232_MESSAGE, pComm->m_wPortID, 0);
			//TRACE(traceAppMsg, 0, "���: ��ȭ ���ڸ� ������ �������Ƿ� ���� ���α׷��� ����ġ �ʰ� ����˴ϴ�.\n");
		}
	}
	// ��Ʈ�� ClosePort�� ���� ������ m_bConnected�� FALSE�� �Ǿ� ����

	CloseHandle(os.hEvent);
	pComm->m_hThreadWatchComm = NULL;

	return TRUE;
}


