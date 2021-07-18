/*******************************************************************************
*                         태 양 추 적 프 로 젝 트                              *
********************************************************************************
* 개 발 자 : FlowerMJ                                                          *
*                                                                              *
* 최초 완료일 : 060517                                                         *
*                                                                              *
*	포트로 Input 되는 Character를 화면상에 출력                                *
*	일정 시간마다 특정 길이의 문자열을 포트로 Output                           *
*	두 포트를 연결하여 테스트 할때는 Baud_Rate가 일치해야만 정상적인           *
*	결과를 얻을 수 있다                                                        *
* --------------------------- 주요 코드 ---------------------------------------*
* 코드명      의미                                                             *
* ----------- -----------------------------------------------------------------*
*                                                                              *
*                                                                              *
* --------------------------- 수정 이력 ---------------------------------------*
* 일시        수정내용                                                         *
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
#define FC_DTRDSR       0x01   //데이터 단말기(DTR) 대기,데이터 세트(DSR) 대기를 위한 신호
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

// Event Watch용 쓰레드 함수
DWORD CommWatchProc( LPSTR lpData );


BOOL Port::OpenPort(long port_number,long baud_rate_select)
{

	// 포트 상태 관련 변수들을 초기화 한다

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
	
	
	// Overlapped I/O에 쓰이는 Event 객체들을 생성

	// Read를 위한 Event 객체 생성
	osRead.hEvent = CreateEvent(
		NULL,    
		TRUE,    
		FALSE,   
		NULL ) ;
	if (osRead.hEvent==NULL)
	{
		return FALSE ;
	}
	
	// Write를 위한 Event 객체 생성
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

	
	// 포트를 생성한다
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
	SetupComm( idComDev, 4096, 4096 ) ;	// 버퍼 설정
	PurgeComm( idComDev, PURGE_TXABORT | PURGE_RXABORT |
		PURGE_TXCLEAR | PURGE_RXCLEAR ) ;	// 버퍼의 모든 데이타를 지운다
	

	// Overlapped I/O를 위한 Time Out 설정
	COMMTIMEOUTS  CommTimeOuts ;

	CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
	CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
	CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;

	CommTimeOuts.WriteTotalTimeoutMultiplier = 2*CBR_9600/dwBaudRate ; // CBR_9600 기준 ms당 바이트를 두배까지 설정
	CommTimeOuts.WriteTotalTimeoutConstant = 0 ;
	SetCommTimeouts( idComDev, &CommTimeOuts ) ;
	
	// 포트를 사용가능 상태로 만들고 Event를 감시할 쓰레드를 생성한다
	if(SetupConnection()==TRUE)
	{
		fConnected = TRUE ;	
		
		// 쓰레드 생성
		hWatchThread = CreateThread(
			(LPSECURITY_ATTRIBUTES) NULL,
			0,
			(LPTHREAD_START_ROUTINE) CommWatchProc,
			(LPVOID)this,
			0,
			&dwThreadID );

		if(hWatchThread==NULL)	// 쓰레드 생성 실패
		{
			fConnected = FALSE ;

			CloseHandle(osRead.hEvent);
			CloseHandle(osWrite.hEvent);
			CloseHandle( idComDev ) ;
			return FALSE;
		}
		else
		{
			// 장치에 DTR(Data-Terminal-Ready)을 알린다
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
	// DCB 구조체를 이용하여 포트를 셋팅한다
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
	
	// 큐에서 읽어야 할 데이타 크기를 가져온다
	ClearCommError( idComDev, &dwErrorFlags, &ComStat ) ;
	dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;
	
	if (dwLength > 0)	// 읽어야 할 데이타가 있는 경우
	{
		// 데이타를 읽는다. Overlapped I/O임을 주의.
		fReadStat = ReadFile( idComDev, lpszBlock,
			dwLength, &dwLength, &osRead) ;
		if (!fReadStat)	// 읽어야 할 바이트를 다 읽지 못했다
		{
			if (GetLastError() == ERROR_IO_PENDING)	// I/O Pending에 의해 다 읽지 못한 경우
			{
				// I/O가 완료되기를 기다린다.
				while(!GetOverlappedResult( idComDev,
					&osRead, &dwLength, TRUE ))
				{
					dwError = GetLastError();
					if(dwError == ERROR_IO_INCOMPLETE)	// I/O가 아직 끝나지 않았다
						continue;
					else	// 에러 발생
					{
						wsprintf( (LPWSTR)szError, L"<CE-%u>\n\r", dwError ) ;
						printf(szError);
						// 에러를 클리어 하고 다른 I/O가 가능하도록 한다
						ClearCommError(idComDev, &dwErrorFlags, &ComStat ) ;
						break;
					}
					
				}
				
			}
			else // I/O Pending이 아닌 다른 에러가 발생한 경우
			{
				dwLength = 0 ;
				// 에러를 클리어 하고 다른 I/O가 가능하도록 한다
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
	
	if (!fWriteStat)	// 써야할 바이트를 다 쓰지 못했다
	{
		if(GetLastError() == ERROR_IO_PENDING)	// I/O Pending에 의한 경우
		{
			// I/O가 완료되기를 기다린다
			while(!GetOverlappedResult( idComDev,
				&osWrite, &dwBytesWritten, TRUE ))
			{
				dwError = GetLastError();
				if(dwError == ERROR_IO_INCOMPLETE)
				{
					// 보낸 전체 바이트 수를 체크
					dwBytesSent += dwBytesWritten;
					continue;
				}
				else
				{
					// 에러 발생
					wsprintf((LPWSTR)szError, L"<CE-%u>", dwError ) ;
					printf("%s\r\n",szError);
					//WriteTTYBlock( hWnd, szError, lstrlen( szError ) ) ;
					ClearCommError( idComDev, &dwErrorFlags, &ComStat ) ;
					break;
				}
			}
			
			dwBytesSent += dwBytesWritten;
			
			if( dwBytesSent != dwBytesToWrite )	// 보내야 할 바이트와 보낸 바이트가 일치하지 않는 경우
				wsprintf((LPWSTR)szError,L"\nProbable Write Timeout: Total of %ld bytes sent", dwBytesSent);
			else	// 성공적으로 모두 보낸 경우
				wsprintf((LPWSTR)szError,L"\n%ld bytes written", dwBytesSent);
			
			OutputDebugString((LPCWSTR)szError);
		}
		else // I/O Pending 외의 다른 에러
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

	// 이벤트 발생을 중지한다
	SetCommMask(idComDev, 0 ) ;
	
	// Event Watch 쓰레드가 중지되기를 기다린다
	while(dwThreadID != 0);
	

	// DTR(Data-Terminal-Ready) 시그널을 Clear 한다
	EscapeCommFunction( idComDev, CLRDTR ) ;
	
	// 대기중인 모든 데이타를 지운다	
	PurgeComm( idComDev, PURGE_TXABORT | PURGE_RXABORT |
		PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

	// 핸들을 반환한다
	CloseHandle(osRead.hEvent);
	CloseHandle(osWrite.hEvent);
	CloseHandle( idComDev) ;
	
	return TRUE;

}

// Event Watch 쓰레드
DWORD CommWatchProc( LPSTR lpData )
{
	DWORD       dwEvtMask ;
	Port		*pp=(Port *)lpData;
	OVERLAPPED  os ;
	long        nLength ;
	BYTE       abIn[ MAXBLOCK + 1] ;
	
	memset( &os, 0, sizeof( OVERLAPPED ) ) ;
	
	// Event 객체 생성
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

		// Event가 발생할 때 까지 블럭
		WaitCommEvent( pp->idComDev, &dwEvtMask, NULL );
		// MSDN 상의 설명과는 달리 WaitCommEvent의 마지막 매개 변수가 NULL이다
		// 설명에 의하면 FILE_FLAG_OVERLAPPED로 생성된 경우 반드시 NULL이 아닌 Overlapped
		// 구조체를 사용해야 한다고 되어있으나 몇몇 샘플을 검토한 결과 오히려 Code 87 에러가
		// 발생한다
		// 이런 이유로 os의 Event 객체는 별 의미가 없다
		// 또한 이런 이유로 멀티 쓰레드가 아닌 단일 쓰레드에서는 Overlapped I/O를 사용하는
		// 의미가 없을 것 같다 ( Polling 시 블럭되기 때문)

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
