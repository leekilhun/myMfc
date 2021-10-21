#pragma once

#define COMM_BUFF_MAX	4096
#define COMM_USE_MY_QUEUE	1

#define COMM_RS232_BUFF_SIZE		8192 // THREAD와 부모 프로세스가 데이터를 주고 받을 버퍼 크기
#define COMM_RS232_ASCII_XON		0x11
#define COMM_RS232_ASCII_XOFF		0x13
#define COMM_RS232_BAUD_LIST_MAX	12

#define WM_COMM_RS232_MESSAGE	(WM_USER+2)		//

#if 0


class CCommRingQueue
{
public:
	BYTE buff[COMM_RS232_BUFF_SIZE]; //  Buffer
	int m_Head, m_Tail;  // CQueue Head Tail Position
	int m_Length;
public:
	CCommRingQueue() {
		Init();
	}
	virtual ~CCommRingQueue() {
		Init();
	}

	inline void Init()
	{
		m_Head = m_Tail = m_Length = COMM_RS232_BUFF_SIZE;
		memset(buff, 0x00, COMM_RS232_BUFF_SIZE);
	}

	inline BOOL Write(LPCSTR data, uint32_t length) {
		BOOL ret = true;
		uint32_t next_in;

		for (int i = 0; i < (int)length; i++)
		{
			next_in = (m_Head + 1) % m_Length;
			if (next_in != m_Tail)
			{
				buff[m_Tail] = data[i];
				m_Head = next_in;
			}
			else
			{
				ret = FALSE;
				break;
			}
		}
		return ret;
	}


	inline BOOL Read(LPSTR data, uint32_t length) {
		BOOL ret = TRUE;
		for (int i = 0; i < (int)length; i++)
		{
			data[i] = buff[m_Tail];
			if (m_Tail != m_Head)
			{
				m_Tail = (m_Tail + 1) % m_Length;
			}
			else
			{
				ret = false;
				break;
			}
		}

		return ret;
	}
	inline int Available() {
		return (m_Head - m_Tail) % m_Length;
	}

	// 버퍼 내 데이터를 모두 처리
	inline void Flush() {
		m_Head = m_Tail = 0;
	}

};


#endif // 0




class CCommRs232Queue
{
public:
	BYTE buff[COMM_RS232_BUFF_SIZE]; //  Buffer
	int m_Head, m_Tail;  // CQueue Head Tail Position
	CCommRs232Queue() :m_Head(0), m_Tail(0) { Clear(); }
	virtual ~CCommRs232Queue() {
		Clear();
	};

	// Queue 초기화.
	inline void Clear() {
		m_Head = m_Tail = 0;
		memset(buff, 0x00, COMM_RS232_BUFF_SIZE);
	}

	// Queue에 들어 있는 데이터의 길이를 리턴한다.
	inline int GetSize() const 	{
		return (COMM_RS232_BUFF_SIZE+ (m_Head - m_Tail) ) % COMM_RS232_BUFF_SIZE;
	}

	// Queue에 1byte 넣기
	inline BOOL PutByte(byte b) {
		BOOL ret = TRUE;
		uint32_t next_in;
		next_in = (m_Head + 1) % COMM_RS232_BUFF_SIZE;
		if (next_in != m_Tail)
		{
			buff[m_Head] = b;
			m_Head = next_in;
		}
		else
			ret = FALSE;

		return ret;
	}

	// Queue에 1byte 꺼내기
	inline BOOL GetByte(byte* pb) {
		BOOL ret = TRUE;
		if (m_Tail != m_Head)
		{
			*pb = buff[m_Tail];
			m_Tail = (m_Tail + 1) % COMM_RS232_BUFF_SIZE;
		}
		else
			ret = FALSE;

		return ret;
	}

};


class CCommRs232
{

public:


	struct sRs232_baudrate_tbl { DWORD baudrate_data; LPCTSTR label; }
	RS_232_BAUDRATE_DATA[COMM_RS232_BAUD_LIST_MAX] =
	{
		{CBR_110,		_T("110")},
		{CBR_300,		_T("300")},
		{CBR_600,		_T("600")},
		{CBR_1200,		_T("1200")},
		{CBR_2400,		_T("2400")},
		{CBR_4800,		_T("4800")},
		{CBR_9600,		_T("9600")},
		{CBR_14400,		_T("14400")},
		{CBR_19200,		_T("19200")},
		{CBR_38400,		_T("38400")},
		{CBR_56000,		_T("56000")},
		{CBR_115200,	_T("115200")},

	};

	HWND m_hCommWnd;

	// 통신 포트 파일 핸들
	HANDLE		m_hComm; 
	
	// 포트 이름
	CString		m_PortName; 

	DWORD		m_Baudrate;
	
	// 포트가 열렸는지 확인
	BOOL		m_IsConnected;

	// 포트 파일 IO를 위한 OverLapped structure
	OVERLAPPED	m_osRead, m_osWrite; 

	// 포트를 감시할 함수, 스레드의 핸들을 보관
	HANDLE		m_hThreadWatchComm; 

	WORD		 m_wPortID; // WM_COMM_READ와 함께 보내는 인수로 
									// 여러 개의 포트를 열었을 경우 어떤 포트인지 
									// 식별하는데 사용

	CCommRs232Queue m_QueueRead; // 스레드와 부모 프로세스간의 통신 버퍼
	// Serial Port를 연다. 인자로 포트명의 String과 속도, 포트 번호를 준다.


	char m_InBuf[COMM_BUFF_MAX * 2];
	int  m_Length;
	CEvent* m_pEvent;

public:
	CCommRs232();
	virtual ~CCommRs232() {
		destrory();
	}


private:
	/**
	*
	*/
	inline void	clearVariable() {
		m_hComm = nullptr;
		m_hCommWnd = nullptr;
		m_wPortID = NULL;
		m_PortName.Empty();
		m_Baudrate = 0;
		m_IsConnected = FALSE;
		m_osRead = {};
		m_osWrite = {};
		m_hThreadWatchComm = {};
		m_QueueRead.Clear();
		memset(m_InBuf,0x00,COMM_BUFF_MAX * 2);
		m_Length = 0;
		m_pEvent = nullptr;
	}
	inline void	destrory() {
		if (m_IsConnected) ClosePort();
		if (m_pEvent != nullptr) { 
			delete m_pEvent; 
			m_pEvent = nullptr;
		}
		clearVariable();
	}

public:
	void	AddPortList(CComboBox* combobox);
	void	AddBaudrateList(CComboBox* combobox);

	// Serial Port를 연다
	BOOL OpenPort(CString port_name, DWORD baud, WORD port_no, HWND hwnd = NULL);

	// Serial Port를 닫는다
	void ClosePort();
	
	inline void HandleClose() {
		if (m_IsConnected){
			ClosePort();
			//CloseHandle(m_hComm);
		}
		CloseHandle(m_osRead.hEvent);
		CloseHandle(m_osWrite.hEvent);
	}

	void ResetPort();

	// Serial Port에 buff내용을 write수 만큼 쓴다
	// 실제로 쓴 바이트 수를 리턴한다
	DWORD WriteComm(LPCSTR buff, int write_len);
	
	// 포트로부터 buff에 read만큼 읽는다.
	// 실제로 읽혀진 Byte수를 리턴한다.
	DWORD ReadComm(BYTE* pbuff, DWORD read_len);

	inline void MsgTest(HWND hwnd)
	{
		SendMessage(hwnd, UWM_USER_MSG, 0, 0);
	}
};

DWORD ThreadWatchComm(CCommRs232* pComm);