/*******************************************************************************
*                         �� �� �� �� �� �� �� Ʈ                              *
********************************************************************************
* �� �� �� : FlowerMJ                                                          *
*                                                                              *
* ���� �Ϸ��� : 060517                                                         *
*                                                                              *
*	��Ʈ�� Input �Ǵ� Character�� ȭ��� ���                                *
*	���� �ð����� Ư�� ������ ���ڿ��� ��Ʈ�� Output                           *
*	�� ��Ʈ�� �����Ͽ� �׽�Ʈ �Ҷ��� Baud_Rate�� ��ġ�ؾ߸� ��������           *
*	����� ���� �� �ִ�                                                        *
* --------------------------- �ֿ� �ڵ� ---------------------------------------*
* �ڵ��      �ǹ�                                                             *
* ----------- -----------------------------------------------------------------*
*                                                                              *
*                                                                              *
* --------------------------- ���� �̷� ---------------------------------------*
* �Ͻ�        ��������                                                         *
* ----------- -----------------------------------------------------------------*
*                                                                              *
*                                                                              *
*******************************************************************************/

#include "pch.h"
#include "framework.h"

#include "string.h"
#include "Port.h"
#include "resource.h"

#define MAXBLOCK        80

DWORD BAUD_RATE_TABLE[] = {	CBR_110,CBR_300,CBR_600,CBR_1200,CBR_2400,CBR_4800,CBR_9600,CBR_14400,
							CBR_19200,CBR_38400,CBR_56000,CBR_57600,CBR_115200,230400,460800,921600,0};

// Flow control flags
#define FC_DTRDSR       0x01   //������ �ܸ���(DTR) ���,������ ��Ʈ(DSR) ��⸦ ���� ��ȣ
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

// Event Watch�� ������ �Լ�
DWORD CommWatchProc( LPSTR lpData );


BOOL Port::OpenPort(long port_number,long baud_rate_select)
{

	// ��Ʈ ���� ���� �������� �ʱ�ȭ �Ѵ�

	idComDev				=0 ;
	fConnected				= FALSE ;
	fLocalEcho				= FALSE ;
	fAutoWrap				= TRUE ;
	dwBaudRate				= BAUD_RATE_TABLE[baud_rate_select];
	bByteSize				= 8 ;
	bFlowCtrl				= FC_XONXOFF ;
	bParity					= NOPARITY ;
	bStopBits				= ONESTOPBIT ;
	fXonXoff				= FALSE ;
	fUseCNReceive			= TRUE ;
	fDisplayErrors			= TRUE ;
	osWrite.Offset			= 0 ;
	osWrite.OffsetHigh		= 0 ;
	osRead.Offset			= 0 ;
	osRead.OffsetHigh		= 0 ;
	
	
	// Overlapped I/O�� ���̴� Event ��ü���� ����

	// Read�� ���� Event ��ü ����
	osRead.hEvent = CreateEvent(
		NULL,    
		TRUE,    
		FALSE,   
		NULL ) ;
	if (osRead.hEvent==NULL)
	{
		return FALSE ;
	}
	
	// Write�� ���� Event ��ü ����
	osWrite.hEvent = CreateEvent(
		NULL,   
		TRUE,   
		FALSE,  
		NULL ) ;
	
	if (osWrite.hEvent==NULL)
	{
		CloseHandle( osRead.hEvent ) ;
		return FALSE;
	}

	
	// ��Ʈ�� �����Ѵ�
	char temp[20];
	wchar_t COMPORT_Rx[10];// = L"COM";
	//strcpy_s(temp,"\\\\.\\");
	wsprintf(COMPORT_Rx,L"COM%d",port_number);
	//strcat_s(temp, port_name);
	idComDev = CreateFile(/*(LPCWSTR)temp*/COMPORT_Rx,
		GENERIC_READ | GENERIC_WRITE,
		0,							
		NULL,						
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// Overlapped I/O
		NULL);
	
	if (idComDev == INVALID_HANDLE_VALUE)
	{
		CloseHandle(osRead.hEvent);
		CloseHandle(osWrite.hEvent);
		return FALSE;
	}
	
	SetCommMask( idComDev, EV_RXCHAR ) ;
	SetupComm( idComDev, 4096, 4096 ) ;	// ���� ����
	PurgeComm( idComDev, PURGE_TXABORT | PURGE_RXABORT |
		PURGE_TXCLEAR | PURGE_RXCLEAR ) ;	// ������ ��� ����Ÿ�� �����
	

	// Overlapped I/O�� ���� Time Out ����
	COMMTIMEOUTS  CommTimeOuts ;

	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
	CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;

	CommTimeOuts.WriteTotalTimeoutMultiplier = 2*CBR_9600/dwBaudRate ; // CBR_9600 ���� ms�� ����Ʈ�� �ι���� ����
	CommTimeOuts.WriteTotalTimeoutConstant = 0 ;
	SetCommTimeouts( idComDev, &CommTimeOuts ) ;
	
	// ��Ʈ�� ��밡�� ���·� ����� Event�� ������ �����带 �����Ѵ�
	if(SetupConnection()==TRUE)
	{
		fConnected = TRUE ;	
		
		// ������ ����
		hWatchThread = CreateThread(
			(LPSECURITY_ATTRIBUTES) NULL,
			0,
			(LPTHREAD_START_ROUTINE) CommWatchProc,
			(LPVOID)this,
			0,
			&dwThreadID );

		if(hWatchThread==NULL)	// ������ ���� ����
		{
			fConnected = FALSE ;

			CloseHandle(osRead.hEvent);
			CloseHandle(osWrite.hEvent);
			CloseHandle( idComDev ) ;
			return FALSE;
		}
		else
		{
			// ��ġ�� DTR(Data-Terminal-Ready)�� �˸���
			EscapeCommFunction( idComDev, SETDTR ) ;
		}
	}
	else
	{
		fConnected = FALSE ;

		CloseHandle(osRead.hEvent);
		CloseHandle(osWrite.hEvent);
		CloseHandle( idComDev ) ;
		return FALSE;
	}
	return TRUE;
}

BOOL Port::SetupConnection(void)
{
	// DCB ����ü�� �̿��Ͽ� ��Ʈ�� �����Ѵ�
	BYTE       bSet ;
	DCB        dcb ;
	
	dcb.DCBlength = sizeof( DCB ) ;
	
	GetCommState( idComDev, &dcb ) ;
	
	dcb.BaudRate = dwBaudRate ;
	dcb.ByteSize = bByteSize;
	dcb.Parity = bParity;
	dcb.StopBits = bStopBits;
	
	// setup hardware flow control
	
	bSet = (BYTE) ((bFlowCtrl & FC_DTRDSR) != 0) ;
	dcb.fOutxDsrFlow = bSet ;
	if (bSet)
		dcb.fDtrControl = DTR_CONTROL_HANDSHAKE ;
	else
		dcb.fDtrControl = DTR_CONTROL_ENABLE ;
	
	bSet = (BYTE) ((bFlowCtrl & FC_RTSCTS) != 0) ;
	dcb.fOutxCtsFlow = bSet ;
	if (bSet)
		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE ;
	else
		dcb.fRtsControl = RTS_CONTROL_ENABLE ;
	
	// setup software flow control
	
	bSet = (BYTE) ((bFlowCtrl & FC_XONXOFF) != 0) ;
	
	dcb.fInX = dcb.fOutX = bSet ;
	dcb.XonChar = ASCII_XON ;
	dcb.XoffChar = ASCII_XOFF ;
	dcb.XonLim = 100 ;
	dcb.XoffLim = 100 ;
	
	// other various settings
	
	dcb.fBinary = TRUE ;
	dcb.fParity = TRUE ;
	
	return SetCommState( idComDev, &dcb ) ;
}

long Port::ReadCommBlock(LPSTR lpszBlock, long nMaxLength )
{
	BOOL       fReadStat ;
	COMSTAT    ComStat ;
	DWORD      dwErrorFlags;
	DWORD      dwLength;
	DWORD      dwError;
	char       szError[ 10 ] ;
	
	// ť���� �о�� �� ����Ÿ ũ�⸦ �����´�
	ClearCommError( idComDev, &dwErrorFlags, &ComStat ) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;
	
	if (dwLength > 0)	// �о�� �� ����Ÿ�� �ִ� ���
	{
		// ����Ÿ�� �д´�. Overlapped I/O���� ����.
		fReadStat = ReadFile( idComDev, lpszBlock,
			dwLength, &dwLength, &osRead) ;
		if (!fReadStat)	// �о�� �� ����Ʈ�� �� ���� ���ߴ�
		{
			if (GetLastError() == ERROR_IO_PENDING)	// I/O Pending�� ���� �� ���� ���� ���
			{
				// I/O�� �Ϸ�Ǳ⸦ ��ٸ���.
				while(!GetOverlappedResult( idComDev,
					&osRead, &dwLength, TRUE ))
				{
					dwError = GetLastError();
					if(dwError == ERROR_IO_INCOMPLETE)	// I/O�� ���� ������ �ʾҴ�
						continue;
					else	// ���� �߻�
					{
						wsprintf( (LPWSTR)szError, L"<CE-%u>\n\r", dwError ) ;
						printf(szError);
						// ������ Ŭ���� �ϰ� �ٸ� I/O�� �����ϵ��� �Ѵ�
						ClearCommError(idComDev, &dwErrorFlags, &ComStat ) ;
						break;
					}
					
				}
				
			}
			else // I/O Pending�� �ƴ� �ٸ� ������ �߻��� ���
			{
				dwLength = 0 ;
				// ������ Ŭ���� �ϰ� �ٸ� I/O�� �����ϵ��� �Ѵ�
				ClearCommError( idComDev, &dwErrorFlags, &ComStat ) ;
			}
		}
	}
	
	return ( dwLength ) ;
	
}

BOOL Port::WriteCommBlock(LPSTR lpByte , DWORD dwBytesToWrite)
{
	
	BOOL        fWriteStat ;
	DWORD       dwBytesWritten ;
	DWORD       dwErrorFlags;
	DWORD   	dwError;
	DWORD       dwBytesSent=0;
	COMSTAT     ComStat;
	char        szError[ 128 ] ;
	
	fWriteStat = WriteFile( idComDev, lpByte, dwBytesToWrite,
		&dwBytesWritten, &osWrite) ;
	
	if (!fWriteStat)	// ����� ����Ʈ�� �� ���� ���ߴ�
	{
		if(GetLastError() == ERROR_IO_PENDING)	// I/O Pending�� ���� ���
		{
			// I/O�� �Ϸ�Ǳ⸦ ��ٸ���
			while(!GetOverlappedResult( idComDev,
				&osWrite, &dwBytesWritten, TRUE ))
			{
				dwError = GetLastError();
				if(dwError == ERROR_IO_INCOMPLETE)
				{
					// ���� ��ü ����Ʈ ���� üũ
					dwBytesSent += dwBytesWritten;
					continue;
				}
				else
				{
					// ���� �߻�
					wsprintf((LPWSTR)szError, L"<CE-%u>", dwError ) ;
					printf("%s\r\n",szError);
					//WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
					ClearCommError( idComDev, &dwErrorFlags, &ComStat ) ;
					break;
				}
			}
			
			dwBytesSent += dwBytesWritten;
			
			if( dwBytesSent != dwBytesToWrite )	// ������ �� ����Ʈ�� ���� ����Ʈ�� ��ġ���� �ʴ� ���
				wsprintf((LPWSTR)szError,L"\nProbable Write Timeout: Total of %ld bytes sent", dwBytesSent);
			else	// ���������� ��� ���� ���
				wsprintf((LPWSTR)szError,L"\n%ld bytes written", dwBytesSent);
			
			OutputDebugString((LPCWSTR)szError);
		}
		else // I/O Pending ���� �ٸ� ����
		{
			ClearCommError(idComDev, &dwErrorFlags, &ComStat ) ;
			return FALSE;
		}
	}
	

	return TRUE;
	
}

BOOL Port::ClosePort(void)
{
	fConnected = FALSE ;

	// �̺�Ʈ �߻��� �����Ѵ�
	SetCommMask(idComDev, 0 ) ;
	
	// Event Watch �����尡 �����Ǳ⸦ ��ٸ���
	while(dwThreadID != 0);
	

	// DTR(Data-Terminal-Ready) �ñ׳��� Clear �Ѵ�
	EscapeCommFunction( idComDev, CLRDTR ) ;
	
	// ������� ��� ����Ÿ�� �����	
	PurgeComm( idComDev, PURGE_TXABORT | PURGE_RXABORT |
		PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

	// �ڵ��� ��ȯ�Ѵ�
	CloseHandle(osRead.hEvent);
	CloseHandle(osWrite.hEvent);
	CloseHandle( idComDev) ;
	
	return TRUE;

}

// Event Watch ������
DWORD CommWatchProc( LPSTR lpData )
{
	DWORD       dwEvtMask ;
	Port		*pp=(Port *)lpData;
	OVERLAPPED  os ;
	long        nLength ;
	BYTE       abIn[ MAXBLOCK + 1] ;
	
	memset( &os, 0, sizeof( OVERLAPPED ) ) ;
	
	// Event ��ü ����
	os.hEvent = CreateEvent(
		NULL,    
		TRUE,    
		FALSE,   
		NULL ) ; 
	
	if (os.hEvent == NULL)
		return FALSE;
	
	if (!SetCommMask( pp->idComDev, EV_RXCHAR ))
	{
		CloseHandle(os.hEvent);
		return FALSE;
	}
	
	while ( pp->fConnected )
	{
		dwEvtMask = 0 ;

		// Event�� �߻��� �� ���� ��
		WaitCommEvent( pp->idComDev, &dwEvtMask, NULL );
		// MSDN ���� ������� �޸� WaitCommEvent�� ������ �Ű� ������ NULL�̴�
		// ���� ���ϸ� FILE_FLAG_OVERLAPPED�� ������ ��� �ݵ�� NULL�� �ƴ� Overlapped
		// ����ü�� ����ؾ� �Ѵٰ� �Ǿ������� ��� ������ ������ ��� ������ Code 87 ������
		// �߻��Ѵ�
		// �̷� ������ os�� Event ��ü�� �� �ǹ̰� ����
		// ���� �̷� ������ ��Ƽ �����尡 �ƴ� ���� �����忡���� Overlapped I/O�� ����ϴ�
		// �ǹ̰� ���� �� ���� ( Polling �� ���Ǳ� ����)

		if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
		{
			do
			{
				if (nLength = pp->ReadCommBlock((LPSTR) abIn, MAXBLOCK ))
				{
					CString preText;
					CEdit *pEdit;
					char *temp=new char[nLength+1];
					//strncpy_s(temp, nLength,"test"/*abIn*/);
					strncpy_s(temp, nLength, (const char*)abIn, MAXBLOCK);
					temp[nLength]='\0';

					pEdit=(CEdit*)AfxGetMainWnd()->GetDlgItem(IDC_ED_INPUT);
					
					pEdit->GetWindowText(preText);
					
					if(pEdit->GetLineCount()>300){
						preText=preText.Right(preText.GetLength()-(preText.FindOneOf(L"\n\r")+2));

					}
					preText.Append((CString)temp,strlen(temp));
					preText.Replace(L"\n\r",L"\r\n");
					pEdit->SetWindowText(preText);
					pEdit->SetSel(0,-1);
					pEdit->SetSel(-1,-1);
//					pEdit->SetFocus();

					delete temp;
				}
			}
			while ( nLength > 0 ) ;
		}
		else
		{
			printf("<Other Event>\r\n");
		}
	}
	
	CloseHandle( os.hEvent ) ;
	
	pp->dwThreadID = 0 ;
	pp->hWatchThread = NULL ;
	
	return TRUE;
	
}
