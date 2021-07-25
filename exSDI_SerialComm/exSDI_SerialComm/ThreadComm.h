#pragma once
#include <afxwin.h>

#define BUFF_SIZE 8192 // THREAD와 부모 프로세스가 데이터를 주고 받을 버퍼 크기
#define WM_COMM_READ (WM_USER+1) // THREAD가 부모 프로세스에 보낼 메시지
#define WM_MYMESSAGE (WM_USER+2) // THREAD가 부모 프로세스에 보낼 메시지
//#define ASCII_LF 0x0a
//#define ASCII_CR 0x0d
#define ASCII_XON 0x11
#define ASCII_X0FF 0x13

class CQueue
{
public:
	BYTE buff[BUFF_SIZE]; // CQueue Buffer
	int m_iHead, m_iTail;  // CQueue Head Tail Position
	CQueue() :m_iHead(0), m_iTail(0) { Clear(); }
	virtual ~CQueue() {};
	void Clear() // Queue의 내용을 지운다.
	{
		m_iHead = m_iTail = 0;
		memset(buff, 0, BUFF_SIZE);
	}

	int GetSize() // Queue에 들어 있는 데이터의 길이를 리턴한다.
	{
		return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
	}
	BOOL PutByte(byte b) // Queue에 1byte 넣기
	{		
		if (GetSize() == (BUFF_SIZE - 1)) return FALSE;  // Queue가 꽉 찾는지 검사
		// Queue에 한 바이트를 넣고 Head Pointer를 증가
		buff[m_iHead++] = b;
		m_iHead %= BUFF_SIZE;
		return TRUE;
	}
	BOOL GetByte(byte* pb) // Queue에 1byte 꺼내기
	{
		if (GetSize() == 0) return FALSE; // Queue가 비었는지 검사
		*pb = buff[m_iTail++];
		m_iTail %= BUFF_SIZE;
		return TRUE;
	}

};



class CThreadComm
{

public:

	HANDLE m_hComm; // 통신 포트 파일 핸들
	CString m_PortName; // 포트 이름
	BOOL m_bConnected;  // 포트가 열렸는지 확인
	OVERLAPPED m_osRead, m_osWrite; // 포트 파일 IO를 위한 OverLapped structure

	HANDLE m_hThreadWatchComm; // 포트를 감시할 함수, 스레드의 핸들을 보관

	WORD m_wPortID; // WM_COMM_READ와 함께 보내는 인수로 
									// 여러 개의 포트를 열었을 경우 어떤 포트인지 
									// 식별하는데 사용

	CQueue m_QueueRead; // 스레드와 부모 프로세스간의 통신 버퍼
	// Serial Port를 연다. 인자로 포트명의 String과 속도, 포트 번호를 준다.
	
	
	CThreadComm();
	virtual ~CThreadComm();

	void clear();

	BOOL OpenPort(CString sPortName, DWORD dwBaud, WORD wParam, HWND hwnd = NULL);
	// Serial Port를 닫는다
	void ClosePort();
	// Serial Port에 데이터를 쓴다.
	DWORD WriteComm(LPCTSTR pBuff, int nToWrite);
	// 스레드가 메시지를 보내온 경우 이 함수를 이용해서 데이터를 읽는다.
	DWORD ReadComm(BYTE* pBuff, DWORD nToRead);

};

DWORD ThreadWatchComm(CThreadComm* pComm);


