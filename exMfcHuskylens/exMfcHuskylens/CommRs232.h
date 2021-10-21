#pragma once

#define COMM_BUFF_MAX	4096
#define COMM_USE_MY_QUEUE	1

#define COMM_RS232_BUFF_SIZE		8192 // THREAD�� �θ� ���μ����� �����͸� �ְ� ���� ���� ũ��
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

	// ���� �� �����͸� ��� ó��
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

	// Queue �ʱ�ȭ.
	inline void Clear() {
		m_Head = m_Tail = 0;
		memset(buff, 0x00, COMM_RS232_BUFF_SIZE);
	}

	// Queue�� ��� �ִ� �������� ���̸� �����Ѵ�.
	inline int GetSize() const 	{
		return (COMM_RS232_BUFF_SIZE+ (m_Head - m_Tail) ) % COMM_RS232_BUFF_SIZE;
	}

	// Queue�� 1byte �ֱ�
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

	// Queue�� 1byte ������
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

	// ��� ��Ʈ ���� �ڵ�
	HANDLE		m_hComm; 
	
	// ��Ʈ �̸�
	CString		m_PortName; 

	DWORD		m_Baudrate;
	
	// ��Ʈ�� ���ȴ��� Ȯ��
	BOOL		m_IsConnected;

	// ��Ʈ ���� IO�� ���� OverLapped structure
	OVERLAPPED	m_osRead, m_osWrite; 

	// ��Ʈ�� ������ �Լ�, �������� �ڵ��� ����
	HANDLE		m_hThreadWatchComm; 

	WORD		 m_wPortID; // WM_COMM_READ�� �Բ� ������ �μ��� 
									// ���� ���� ��Ʈ�� ������ ��� � ��Ʈ���� 
									// �ĺ��ϴµ� ���

	CCommRs232Queue m_QueueRead; // ������� �θ� ���μ������� ��� ����
	// Serial Port�� ����. ���ڷ� ��Ʈ���� String�� �ӵ�, ��Ʈ ��ȣ�� �ش�.


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

	// Serial Port�� ����
	BOOL OpenPort(CString port_name, DWORD baud, WORD port_no, HWND hwnd = NULL);

	// Serial Port�� �ݴ´�
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

	// Serial Port�� buff������ write�� ��ŭ ����
	// ������ �� ����Ʈ ���� �����Ѵ�
	DWORD WriteComm(LPCSTR buff, int write_len);
	
	// ��Ʈ�κ��� buff�� read��ŭ �д´�.
	// ������ ������ Byte���� �����Ѵ�.
	DWORD ReadComm(BYTE* pbuff, DWORD read_len);

	inline void MsgTest(HWND hwnd)
	{
		SendMessage(hwnd, UWM_USER_MSG, 0, 0);
	}
};

DWORD ThreadWatchComm(CCommRs232* pComm);