

#include <afxwin.h>
#include <stdio.h>
#include <string.h>

class Port
{
public:

	char port_name[20];
	
	HANDLE idComDev;		// ��Ʈ �ڵ�
	
	// ��Ʈ ���� ����
	BOOL    fConnected, fXonXoff, fLocalEcho, fNewLine, fAutoWrap,
			fUseCNReceive, fDisplayErrors;
	BYTE    bByteSize, bFlowCtrl, bParity, bStopBits ;
	DWORD   dwBaudRate ;

	HANDLE		hWatchThread;	// ������ �ڵ�
	DWORD       dwThreadID ;	// ������ ID
	OVERLAPPED  osWrite, osRead ;	// Overlapped I/O�� ���� ����ü
	
	// member function
	BOOL OpenPort(long port_number,long baud_rate_select=6);
	BOOL SetupConnection(void);

	long ReadCommBlock(LPSTR lpszBlock, long nMaxLength );
	BOOL WriteCommBlock(LPSTR lpByte , DWORD dwBytesToWrite);

	BOOL ClosePort(void);
};