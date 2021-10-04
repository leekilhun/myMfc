#include "pch.h"
#include "Mycomm.h"

#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>


#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif


CMycomm::CMycomm(CString port, CString baudrate, CString parity, CString databit, CString stopbit)
{
	Init();

	m_sComPort = port;
	m_sBaudRate = baudrate;
	m_sParity = parity;
	m_sDataBit = databit;
	m_sStopBit = stopbit;
	m_bFlowChk = 1;
	m_bIsOpenned = FALSE;
	m_nLength = 0;
	m_BaudRate = CBR_115200;
	m_Parity = NOPARITY;
	m_ByteSize = 8;
	m_StopBit = ONESTOPBIT;
	memset(m_sInBuf, 0x00, MAXBUF * 2);


	m_pEvent = new CEvent(FALSE, TRUE);
	
}

void CMycomm::ResetSerial()
{
	if (m_bIsOpenned == FALSE) return;

	DWORD dwError;

	ClearCommError(m_hComDev, &dwError, NULL);
	SetupComm(m_hComDev, InBufSize, OutBufSize);
	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

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
	SetCommTimeouts(m_hComDev, &timeouts);


	// dcb설정 Serial Port의 특성을 설정하기 위한 구조체
	DCB dcb;
	memset(&dcb, 0x00, sizeof(DCB));
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_hComDev, &dcb);

	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
	dcb.BaudRate = m_BaudRate;

	dcb.Parity = m_Parity;// NOPARITY;// 0;
	dcb.ByteSize = m_ByteSize;// 8;
	dcb.StopBits = m_StopBit;// ONESTOPBIT;// 1;

	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fOutxCtsFlow = FALSE;

	// Xon , Xoff 사용 // Xon , Xoff 사용하지 않을 경우는 CriticalSection을 설정 하여 보호해 주어야 한다.
	if (m_bFlowChk)
	{
		dcb.fInX = FALSE;
		dcb.fOutX = FALSE;
		dcb.XonLim = 2048;
		dcb.XoffLim = 1024;

	}
	else
	{
		dcb.fOutxCtsFlow = TRUE;
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE;
	}
	
	//dcb.XonChar = COMM_RS232_ASCII_XON;
	//dcb.XonChar = COMM_RS232_ASCII_XOFF;


	SetCommState(m_hComDev, &dcb);

	SetCommMask(m_hComDev, EV_RXCHAR);

}

void CMycomm::AddPortList(CComboBox* combobox)
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
	combobox->SetCurSel(index - 1);

	RegCloseKey(key);
}

void CMycomm::AddBaudrateList(CComboBox* combobox)
{
	if (combobox == NULL) return;
	int index = 0;
	while (RS_232_BAUDRATE_DATA)
	{
		if (index >= RS232_BAUD_LIST_MAX) break;
		combobox->AddString(RS_232_BAUDRATE_DATA[index].label);
		combobox->SetItemData(index, RS_232_BAUDRATE_DATA[index].baudrate_data);
		index++;
	}
	combobox->SetCurSel(index - 1);
}

void CMycomm::Close()
{
	if (m_bIsOpenned == FALSE) return;

	m_bIsOpenned = FALSE;
	SetCommMask(m_hComDev, 0);
	EscapeCommFunction(m_hComDev, CLRDTR);
	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	Sleep(500);

}

void CMycomm::Clear()
{
	PurgeComm(m_hComDev, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	memset(m_sInBuf, 0x00, MAXBUF * 2);
	m_nLength = 0;
}

int CMycomm::Receive(LPSTR inbuf, int len)
{
	CSingleLock lockObj((CSyncObject*)m_pEvent, FALSE);
	// argument value is not valid
	if (len == 0)
		return -1;
	else if (len > MAXBUF)
		return -1;

	if (m_nLength == 0)
	{
		inbuf[0] = '\0';
		return 0;
	}
	else if (m_nLength <= len)
	{
		lockObj.Lock();
		memcpy(inbuf, m_sInBuf, m_nLength);
		memset(m_sInBuf, 0x00, MAXBUF * 2);
		int tmp = m_nLength;
		m_nLength = 0;
		lockObj.Unlock();
		return tmp;
	}
	else
	{
		lockObj.Lock();
		memcpy(inbuf, m_sInBuf, len);
		memmove(m_sInBuf, m_sInBuf + len, MAXBUF * 2 - len);
		m_nLength -= len;
		lockObj.Unlock();
		return len;
	}
}

BOOL CMycomm::Send(LPCTSTR outbuf, int len)
{
	BOOL ret = TRUE;
	DWORD ErrorFlags;
	COMSTAT ComStat;

	DWORD BytesWritten;
	DWORD BytesSent = 0;

	ClearCommError(m_hComDev, &ErrorFlags, &ComStat);
	if (!WriteFile(m_hComDev, outbuf, len, &BytesWritten, &m_OLW))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			if (WaitForSingleObject(m_OLW.hEvent, 1000) != WAIT_OBJECT_0)
				ret = FALSE;
			else
				GetOverlappedResult(m_hComDev, &m_OLW, &BytesWritten, FALSE);

		}
		else /* IO Error*/
			ret = FALSE; /* ignore error*/
	}

	ClearCommError(m_hComDev, &ErrorFlags, &ComStat);

	return ret;
}

BOOL CMycomm::Open(CString port_name, DWORD baud,HWND hwnd)
{
	// 포트 열기

	m_sComPort = L"//./" + port_name;
	m_hComDev = CreateFile(m_sComPort,
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,
		NULL);

	if (m_hComDev != INVALID_HANDLE_VALUE)
		m_bIsOpenned = TRUE;
	else
		return FALSE; // 포트 열기 실패

	ResetSerial();

	// overlapped structure 변수 초기화
	m_OLR.Offset = 0;
	m_OLR.OffsetHigh = 0;
	if (!(m_OLR.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_OLR.hEvent != 0) CloseHandle(m_OLR.hEvent);
		return FALSE;
	}

	m_OLW.Offset = 0;
	m_OLW.OffsetHigh = 0;
	if (!(m_OLW.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		if (m_OLW.hEvent != 0) CloseHandle(m_OLW.hEvent);
		return FALSE;
	}

	AfxBeginThread(CommThread, (LPVOID)this);
	EscapeCommFunction(m_hComDev, SETDTR);

	return TRUE;
}

void CMycomm::HandleClose()
{
	CloseHandle(m_hComDev);
	CloseHandle(m_OLR.hEvent);
	CloseHandle(m_OLW.hEvent);
}




UINT CommThread(LPVOID lpData)
{
	//extern short g_nRemoteStatus;

	DWORD ErrorFlags;
	COMSTAT ComStat;
	DWORD EvtMask;
	char buf[MAXBUF];
	DWORD Length;
	int size;
	int insize = 0;

	CMycomm* Comm = (CMycomm*)lpData;
	while (Comm->m_bIsOpenned)
	{
		EvtMask = 0;
		Length = 0;
		insize = 0;
		memset(buf, '\0', MAXBUF);
		WaitCommEvent(Comm->m_hComDev, &EvtMask, NULL);
		ClearCommError(Comm->m_hComDev, &ErrorFlags, &ComStat);
		if ((EvtMask & EV_RXCHAR) && ComStat.cbInQue)
		{
			if (ComStat.cbInQue > MAXBUF)
				size = MAXBUF;
			else
				size = ComStat.cbInQue;
			do
			{
				ClearCommError(Comm->m_hComDev, &ErrorFlags, &ComStat);
				if (!ReadFile(Comm->m_hComDev, buf + insize, size, &Length, &(Comm->m_OLR)))
				{
					// Error
					TRACE("Error in ReadFile \n");
					if (GetLastError() == ERROR_IO_PENDING)
					{
						if (WaitForSingleObject(Comm->m_OLR.hEvent, 1000) != WAIT_OBJECT_0)
							Length = 0;
						else
							GetOverlappedResult(Comm->m_hComDev, &(Comm->m_OLR), &Length, FALSE);
					}
					else
						Length = 0;
				}
				insize += Length;
			} while ((Length != 0) && (insize < size));
			ClearCommError(Comm->m_hComDev, &ErrorFlags, &ComStat);

			if (Comm->m_nLength + insize > MAXBUF * 2)
				insize = (Comm->m_nLength + insize) - MAXBUF * 2;

			Comm->m_pEvent->ResetEvent();
			memcpy(Comm->m_sInBuf + Comm->m_nLength, buf, insize);
			Comm->m_nLength += insize;
			Comm->m_pEvent->SetEvent();
			LPARAM temp = (LPARAM)Comm;
			SendMessage(Comm->m_hWnd, WM_MYRECEIVE, Comm->m_nLength, temp);
		}
	}
	PurgeComm(Comm->m_hComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	LPARAM temp = (LPARAM)Comm;
	SendMessage(Comm->m_hWnd, WM_MYCLOSE, 0, temp);

	return 0;
}
