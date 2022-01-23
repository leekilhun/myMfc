#pragma once
//#include "ICommon.h"

#define WM_COMM_POST_USER_MAX				100


class jL_CommSerial //: public virtual ICommon
{

#define COMM_BUFF_MAX	4096
#define COMM_RS232_BUFF_SIZE		8192 // THREAD와 부모 프로

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
		inline bool PutByte(byte b) {
			bool ret = TRUE;
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
		inline bool GetByte(byte* pb) {
			bool ret = true;
			if (m_Tail != m_Head) {
				*pb = buff[m_Tail];
				m_Tail = (m_Tail + 1) % COMM_RS232_BUFF_SIZE;
			}
			else
				ret = false;

			return ret;
		}

		BYTE& operator[](const uint32_t index)
		{
			if ((index % COMM_RS232_BUFF_SIZE) < (uint32_t)m_Head)
				return buff[index];
			return buff[m_Tail];
		}
		
		BYTE PopByte(const uint32_t index)
		{
			if ((index % COMM_RS232_BUFF_SIZE) < (uint32_t)m_Head)
				return buff[index];
			return buff[m_Tail];
		}
		/*BOOL operator==(BYTE text) {
			return  ;
		}*/


		inline int Available() {
			return m_Head - m_Tail;
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
	enum class error_code
	{
		Success,
		GetCommState,
		SerCommState,
		SetCommMask,
		SetupComm,
		PurgeComm,
		ClearCommError,
		GetCommTimeouts,
		SetCommTimeouts,
		existPort,
		emptyPort,
		invalidHandleValue,
		failCreateEventRead,
		failCreateEventWrite,
		failCreateThread,
	};

public:

	typedef int(*Callback_t)(void* obj);

	Callback_t CallFunc;
	void* CallFuncObj;

	// gui 윈도우 핸들
	HWND m_hCommWnd;

	// 통신 포트 파일 핸들
	HANDLE m_hComm;

	// 포트가 열렸는지 확인
	bool	m_IsConnected;

	// 포트 네임
	CString m_PortName;

	// 통신 속도
	DWORD		m_Baudrate;

	// 스레드와 부모 프로세스간의 통신 버퍼
	_Queue* m_pQueueRead;

	// WM_COMM_READ와 함께 보내는 인수로 
	// 여러 개의 포트를 열었을 경우 어떤 포트인지 
	// 식별하는데 사용
	WORD		 m_PortID;

	DWORD m_ThreadId;

	// 포트를 감시할 함수, 스레드의 핸들을 보관
	HANDLE m_hThreadWatchComm;

	// 포트 파일 IO를 위한 OverLapped structure
	OVERLAPPED m_osWrite, m_osRead;

	// 수신 감시 Event 객체 
	CEvent* m_pEvent;

	DWORD m_UserMessage;

	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	jL_CommSerial();
	virtual ~jL_CommSerial();

	/****************************************************
	 *	func
	 ****************************************************/
private:
	error_code initPort(DWORD baudrate);
	static UINT CommWatchThread(LPVOID pParam);

public:
	void SetMessage(UINT user_msg);
	void HandleClose();
	void ClosePort();
	int OpenPort(CString portname,  DWORD baudrate = CBR_9600, WORD port_no = NULL, HWND hwnd = NULL);
	DWORD ReadByte(BYTE* resp, UINT size);
	DWORD WriteByte(BYTE* sendbyte, UINT size);

	void SetAttachCallbackFunc(void* obj, Callback_t cb);

	void	AddStringPortList(CComboBox* combobox);
	void	AddStringBaudrateList(CComboBox* combobox);




};

