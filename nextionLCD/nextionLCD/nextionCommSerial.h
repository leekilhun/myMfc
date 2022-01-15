#pragma once


#define COMM_BUFF_MAX	4096
#define COMM_RS232_BUFF_SIZE		8192 // THREAD와 부모 프로

class nextionCommSerial
{
	typedef int(*Callback_t)(void* obj);

	class _Queue
	{
	public:
		BYTE buff[COMM_RS232_BUFF_SIZE]; //  Buffer
		int m_Head, m_Tail;  // CQueue Head Tail Position
		_Queue() :m_Head(0), m_Tail(0) { Clear(); }
		virtual ~_Queue() {
			Clear();
		};

		// Queue 초기화.
		inline void Clear() {
			m_Head = m_Tail = 0;
			memset(buff, 0x00, COMM_RS232_BUFF_SIZE);
		}

		// Queue에 들어 있는 데이터의 길이를 리턴한다.
		inline int GetSize() const {
			return (COMM_RS232_BUFF_SIZE + (m_Head - m_Tail)) % COMM_RS232_BUFF_SIZE;
		}

		// Queue에 1byte 넣기
		inline BOOL PutByte(byte b) {
			BOOL ret = TRUE;
			uint32_t next_in;
			next_in = (m_Head + 1) % COMM_RS232_BUFF_SIZE;
			if (next_in != m_Tail) {
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
			if (m_Tail != m_Head) {
				*pb = buff[m_Tail];
				m_Tail = (m_Tail + 1) % COMM_RS232_BUFF_SIZE;
			}
			else
				ret = FALSE;

			return ret;
		}

	};



#define COMM_RS232_BAUD_LIST_MAX	8
	/****************************************************
	 *	data
	 ****************************************************/
	struct sRs232_baudrate_tbl { DWORD baudrate_data; LPCTSTR label; }
	RS_232_BAUDRATE_DATA[COMM_RS232_BAUD_LIST_MAX] =
	{
		{CBR_9600,		_T("9600")},
		{CBR_14400,		_T("14400")},
		{CBR_19200,		_T("19200")},
		{CBR_38400,		_T("38400")},
		{CBR_56000,		_T("56000")},
		{CBR_115200,	_T("115200")},
		{CBR_128000,	_T("128000")},
		{CBR_256000,	_T("256000")},
	};

	struct cfg_t
	{
		DWORD BaudRate;
		BYTE WordLength;
		DWORD fParity;
		BYTE  Parity;
		BYTE StopBits;
	};

	struct timeout_t
	{
		DWORD ReadIntervalTimeout;
		DWORD ReadTotalTimeoutMultiplier;
		DWORD ReadTotalTimeoutConstant;
		DWORD WriteTotalTimeoutMultiplier;
		DWORD WriteTotalTimeoutConstant;
	};


private:


public:
	// gui 윈도우 핸들
	HWND m_hCommWnd;

	// 통신 포트 파일 핸들
	HANDLE  m_hComm;

	// 포트가 열렸는지 확인
	bool		m_IsConnected;

	// 포트 네임
	CString m_PortName;

	// 통신 속도
	DWORD		m_Baudrate;

	// 스레드와 부모 프로세스간의 통신 버퍼
	_Queue m_QueueRead;

	// WM_COMM_READ와 함께 보내는 인수로 
	// 여러 개의 포트를 열었을 경우 어떤 포트인지 
	// 식별하는데 사용
	WORD		 m_PortID;


	DWORD m_ThreadId;

	// 포트를 감시할 함수, 스레드의 핸들을 보관
	HANDLE m_hThreadWatchComm;

	// 포트 파일 IO를 위한 OverLapped structure
	OVERLAPPED m_osWrite, m_osRead;

	DWORD   m_BytesWritten;

	char m_InBuf[COMM_BUFF_MAX * 2];
	uint8_t m_ReceivedData;
	int  m_Length;
	CEvent* m_pEvent;

	
	Callback_t CallFunc;
	void* CallFuncObj;





	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	nextionCommSerial();
	virtual ~nextionCommSerial();


	/****************************************************
	 *	func
	 ****************************************************/
private:

	/// <summary>
	/// 
	/// </summary>
	inline void	clearVariable() {
		m_hComm = nullptr;
		m_hCommWnd = nullptr;
		m_PortID = NULL;
		m_PortName.Empty();
		m_Baudrate = 0;
		m_IsConnected = FALSE;
		m_osRead = {};
		m_osWrite = {};
		m_hThreadWatchComm = {};
		m_QueueRead.Clear();
		memset(m_InBuf, 0x00, COMM_BUFF_MAX * 2);
		m_Length = 0;
		m_pEvent = nullptr;
		CallFunc = nullptr;
		CallFuncObj = nullptr;
	}

	/// <summary>
	/// 
	/// </summary>
	void	destrory() {
		if (m_IsConnected) ClosePort();
		if (m_pEvent != nullptr) {
			delete m_pEvent;
			m_pEvent = nullptr;
		}
		clearVariable();
	}


	uint32_t initPort(DWORD baudrate);

	//Event Watch용 Thread Func   static UINT threadFunc(LPVOID pParam);
	static DWORD CommWatchProc(LPVOID pParam);

public:

	/// <summary>
	/// 
	/// </summary>
	inline void HandleClose() {
		if (m_IsConnected) {
			ClosePort();
			//CloseHandle(m_hComm);
		}
		CloseHandle(m_osRead.hEvent);
		CloseHandle(m_osWrite.hEvent);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="portname"></param>
	/// <param name="port_no"></param>
	/// <param name="baudrate"></param>
	/// <param name="hwnd"></param>
	/// <returns></returns>
	uint32_t OpenPort(CString portname, WORD port_no, DWORD baudrate = CBR_9600, HWND hwnd = NULL);

	/// <summary>
	/// 
	/// </summary>
	void ClosePort();

	void SetCallback(void* obj, Callback_t cb);

	DWORD ReadByte(BYTE* resp, UINT size);
	DWORD WriteByte(BYTE* sendbyte, UINT size);

	void	AddStringPortList(CComboBox* combobox);
	void	AddStringBaudrateList(CComboBox* combobox);
};