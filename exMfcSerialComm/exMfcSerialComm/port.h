

#include <afxwin.h>
#include <stdio.h>
#include <string.h>

class Port
{
public:

	char port_name[20];
	
	HANDLE idComDev;		// 포트 핸들
	
	// 포트 상태 관련
	BOOL    fConnected, fXonXoff, fLocalEcho, fNewLine, fAutoWrap,
			fUseCNReceive, fDisplayErrors;
	BYTE    bByteSize, bFlowCtrl, bParity, bStopBits ;
	DWORD   dwBaudRate ;

	HANDLE		hWatchThread;	// 쓰레드 핸들
	DWORD       dwThreadID ;	// 쓰레드 ID
	OVERLAPPED  osWrite, osRead ;	// Overlapped I/O를 위한 구조체
	
	// member function
	BOOL OpenPort(long port_number,long baud_rate_select=6);
	BOOL SetupConnection(void);

	long ReadCommBlock(LPSTR lpszBlock, long nMaxLength );
	BOOL WriteCommBlock(LPSTR lpByte , DWORD dwBytesToWrite);

	BOOL ClosePort(void);
};