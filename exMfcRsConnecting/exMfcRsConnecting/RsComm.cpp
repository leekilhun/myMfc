#include "pch.h"
#include "RsComm.h"

void CRsComm::closeCommPort(void)
{
	if (m_Connect == FALSE)
		return;

	CloseHandle(m_idComDev);
	CloseHandle(osRead.hEvent);
	CloseHandle(osWrite.hEvent);

}

BOOL CRsComm::setupConnection(void)
{
	BOOL fRetVal;
	DCB dcb;
	dcb.DCBlength = sizeof(DCB);
	GetCommState(m_idComDev, &dcb);
	dcb.BaudRate = dcb_setup.BaudRate;
	dcb.ByteSize = dcb_setup.ByteSize;
	dcb.Parity = dcb_setup.Parity;
	dcb.StopBits = dcb_setup.StopBits;
	dcb.fOutxDsrFlow = 0;
	dcb.fDtrControl = DTR_CONTROL_ENABLE;
	dcb.fOutxCtsFlow = 0;
	dcb.fRtsControl = RTS_CONTROL_ENABLE;
	dcb.fInX = dcb.fOutX = 0; // XON/XOFF 
	dcb.XonChar = 0x11; // ASCII_XON; 
	dcb.XoffChar = 0x13; // ASCII_XOFF; 
	dcb.XonLim = 100;
	dcb.XoffLim = 100;
	dcb.fBinary = TRUE;
	dcb.fParity = TRUE;
	fRetVal = SetCommState(m_idComDev, &dcb);

	return fRetVal;

}

void CRsComm::initComport(CString m_portName, DWORD BaudRate /*= CBR_115200*/)
{
	dcb_setup.BaudRate = BaudRate;
	dcb_setup.ByteSize = 8;
	dcb_setup.Parity = NOPARITY;
	dcb_setup.StopBits = ONESTOPBIT;

	COMMTIMEOUTS commTimeOuts;
	m_idComDev = CreateFile((LPCTSTR)m_portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (m_idComDev == (HANDLE)-1)
	{
		CloseHandle(m_idComDev); m_Connect = FALSE;
		AfxMessageBox(_T("WARNING : ��Ʈ�� ���µ� �����Ͽ����ϴ�."));
	}
	else
	{
		SetCommMask(m_idComDev, EV_RXCHAR);
		SetupComm(m_idComDev, 4096, 4096);
		PurgeComm(m_idComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		commTimeOuts.ReadIntervalTimeout = -1;
		commTimeOuts.ReadTotalTimeoutMultiplier = 0;
		commTimeOuts.ReadTotalTimeoutConstant = 1000;
		commTimeOuts.WriteTotalTimeoutMultiplier = 0;
		commTimeOuts.WriteTotalTimeoutConstant = 1000;
		SetCommTimeouts(m_idComDev, &commTimeOuts);
		m_Connect = setupConnection();
		osWrite.Offset = 0;
		osWrite.OffsetHigh = 0;
		osRead.Offset = 0;
		osRead.OffsetHigh = 0;
		osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}


}

int CRsComm::ReadCommPort(unsigned char* message, DWORD length)
{
	COMSTAT ComStat;
	DWORD dwErrorFlags;
	DWORD dwLength;
	DWORD dwReadLength = 0;
	CStringA strTemp;
	strTemp.Format("%s", message);
	if (m_Connect == FALSE) return 0;
	else
	{
		ClearCommError(m_idComDev, &dwErrorFlags, &ComStat);
		dwLength = min((DWORD)length, ComStat.cbInQue);
		ReadFile(m_idComDev, message, dwLength, &dwReadLength, &osRead);
	}
	if (dwReadLength == 0)
	{
		CStringA str; str.Format("%s", message);
		if (strTemp != str)
		{
			return str.GetLength();
		}
	}

	return dwReadLength;
}

int CRsComm::WriteCommPort(unsigned char* message, DWORD dwLength)
{
	int ret_code;
	ret_code = WriteFile(m_idComDev, message, dwLength, &dwLength, &osWrite);

	return ret_code;

}

bool CRsComm::IsCommPortOpen()
{
	if (m_Connect)
		return true;

	return false;
}

