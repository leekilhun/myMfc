#pragma once


#define COMM_BUFF_MAX	4096
#define COMM_RS232_BUFF_SIZE		8192 // THREAD�� �θ� ����

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

		// Queue �ʱ�ȭ.
		inline void Clear() {
			m_Head = m_Tail = 0;
			memset(buff, 0x00, COMM_RS232_BUFF_SIZE);
		}

		// Queue�� ��� �ִ� �������� ���̸� �����Ѵ�.
		inline int GetSize() const {
			return (COMM_RS232_BUFF_SIZE + (m_Head - m_Tail)) % COMM_RS232_BUFF_SIZE;
		}

		// Queue�� 1byte �ֱ�
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

		// Queue�� 1byte ������
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
	// gui ������ �ڵ�
	HWND m_hCommWnd;

	// ��� ��Ʈ ���� �ڵ�
	HANDLE  m_hComm;

	// ��Ʈ�� ���ȴ��� Ȯ��
	bool		m_IsConnected;

	// ��Ʈ ����
	CString m_PortName;

	// ��� �ӵ�
	DWORD		m_Baudrate;

	// ������� �θ� ���μ������� ��� ����
	_Queue m_QueueRead;

	// WM_COMM_READ�� �Բ� ������ �μ��� 
	// ���� ���� ��Ʈ�� ������ ��� � ��Ʈ���� 
	// �ĺ��ϴµ� ���
	WORD		 m_PortID;


	DWORD m_ThreadId;

	// ��Ʈ�� ������ �Լ�, �������� �ڵ��� ����
	HANDLE m_hThreadWatchComm;

	// ��Ʈ ���� IO�� ���� OverLapped structure
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

	//Event Watch�� Thread Func   static UINT threadFunc(LPVOID pParam);
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