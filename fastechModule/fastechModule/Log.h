
#pragma once
/**
 *  클래스
 */

#define DEF_MLOG_NUM_VIEW_DISPLAY_MAX  100

#define LOG_LEVEL(x)	cLog::level::x

#define WriteErrorLog(x)		WriteLog(LOG_LEVEL(Error),		x, trans::CharToCString(__FILE__), __LINE__ )
#define WriteNormalLog(x)		WriteLog(LOG_LEVEL(Normal),		x, trans::CharToCString(__FILE__), __LINE__ )
#define WriteTactTimeLog(x)	WriteLog(LOG_LEVEL(TactTime), x, trans::CharToCString(__FILE__), __LINE__ )

 //#define DEF_MLOG_LEVEL_NONE			0x00;
 //#define DEF_MLOG_LEVEL_ERROR		0x01;
 //#define DEF_MLOG_LEVEL_WARNING  0x02;
 //#define DEF_MLOG_LEVEL_NOMAL		0x04;
 //#define DEF_MLOG_LEVEL_TACTTIME 0x08;


const int DEF_MLOG_SUCCESS = 0;

const int ERR_MLOG_FILE_OPEN_ERROR = 1;
const int MLOG_DEFAULT_KEEPING_DAYS = 30;
const int ERR_MLOG_TOO_SHORT_KEEPING_DAYS = 2;


//const BYTE DEF_MLOG_NONE_LOG_LEVEL			= 0x00;	// Log 안 함
//const BYTE DEF_MLOG_ERROR_LOG_LEVEL			= 0x01;	// Error관련 Log
//const BYTE DEF_MLOG_WARNING_LOG_LEVEL		= 0x02;	// Warning 관련 Log
//const BYTE DEF_MLOG_NORMAL_LOG_LEVEL		= 0x04;	// 정상 동작 관련 Log
//const BYTE DEF_MLOG_TACT_TIME_LOG_LEVEL		= 0x10;

const int DEF_MLOG_NUM_FILES_TOBE_DELETED = 20;
const int DEF_MLOG_NUM_VIEW_DISPLAY_LOG = 100;


class cLog
{
	/****************************************************
	 *	data
	 ****************************************************/
public:
	enum class level
	{
		None = 0,
		Error,
		Warning,
		Normal,
		TactTime,
	};

	struct data_t
	{
		int				Order;
		CString		Date;
		int				ObjID;
		level			Level;
		CString		LogMsg;
		CString		FileInfor;
		int				LineNumber;
	};

protected:
	int				m_order;
	CString		m_date;
	int				m_objID;
	level			m_level;
	CString		m_logMsg;
	CString		m_fileInfor; /* 경로,디렉토리,파일명 정보를 포함*/
	int				m_lineNo;




	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	cLog() { Init(); }
	cLog(const cLog& origin)
	{
		m_order = origin.m_order;
		m_date = origin.m_date;
		m_objID = origin.m_objID;
		m_level = origin.m_level;
		m_logMsg = origin.m_logMsg;
		m_fileInfor = origin.m_fileInfor;
		m_lineNo = origin.m_lineNo;
	}
	virtual ~cLog() { Init(); }
	inline void Init() {
		m_order = 0;
		m_date.Empty();
		m_objID = 0;
		m_level = level::None;
		m_logMsg.Empty();
		m_fileInfor.Empty();
		m_lineNo = 0;
	};


	/****************************************************
	 *	func
	 ****************************************************/
	inline int SetData(cLog::data_t* data) {
		m_order = data->Order;
		m_date = data->Date;
		m_objID = data->ObjID;
		m_level = data->Level;
		m_logMsg = data->LogMsg;
		m_fileInfor = data->FileInfor;
		m_lineNo = data->LineNumber;

		return 0;
	}

	inline void GetData(cLog::data_t* data) const {
		data->Order = m_order;
		data->Date = m_date;
		data->ObjID = m_objID;
		data->Level = m_level;
		data->LogMsg = m_logMsg;
		data->FileInfor = m_fileInfor;
		data->LineNumber = m_lineNo;
	}

	inline int GetObjID() const {
		return m_objID;
	}

	inline int GetOrder() const {
		return m_order;
	}

	inline CString GetLogDate() const {
		return m_date;
	}

	inline CString GetLogFileInfor() const {
		return m_fileInfor;
	}

	inline CString GetLogMsg() const {
		return m_logMsg;
	}

	inline level GetLogLevel() const {
		return m_level;
	}

	inline int GetLineNumber() const {
		return m_lineNo;
	}

	inline int SetLogTime(CString date) {
		m_date = date;
		return 0;
	}

	inline int SetLogLevel(cLog::level level) {
		m_level = level;
		return 0;
	}

	inline int SetObjectID(int id) {
		m_objID = id;
		return 0;
	}

	inline int SetLogFileInfor(CString name) {
		m_fileInfor = name;
		return 0;
	}
};


class qlistLog
{
public:

private:
	uint16_t current_node;
	cLog log_list[DEF_MLOG_NUM_VIEW_DISPLAY_MAX];

public:
	// 생성자
	qlistLog() { Init(); };
	~qlistLog() { Init(); };



public:
	inline void Init() {
		current_node = 0;
		for (size_t i = 0; i < DEF_MLOG_NUM_VIEW_DISPLAY_MAX; i++)
			log_list[i].Init();
	}


	inline void ClearMsg() {
		Init();
	}

	inline void PostMsg(cLog::data_t* data) {
		current_node++;
		if (current_node >= DEF_MLOG_NUM_VIEW_DISPLAY_MAX)
			current_node = 0;

		log_list[current_node].SetData(data);
	}

	inline void GetMsg(cLog::data_t* data) {
		if (current_node == 0)
			return;
		log_list[current_node - 1].GetData(data);
	}

	inline cLog::data_t LastMsg() {
		cLog::data_t data = {};
		if (current_node > 0)
			log_list[current_node - 1].GetData(&data);

		return data;
	}

	int GetListSize() {
		return current_node;
	}

};



class MLog :public cLog
{
	/****************************************************
	 *	data
	 ****************************************************/
	 /** Support view data */
	qlistLog* m_pLogView;

	/** Log File Pointer */
	FILE* m_pLogFile;

	CString m_logFilePath;
	CString m_logFileName;

	/** Keeping days of log files
		* Required because of the limited hard disk capacity
		*/
	int m_nLogKeepingDays;

	CRITICAL_SECTION	m_csWriteControl;

	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	/*default*/
	MLog(void);


	virtual ~MLog(void);

	/****************************************************
	 *	func
	 ****************************************************/
private:
	void init();

public:
	/// <summary>
	/// Object ID를 set한다.
	/// </summary>
	/// <param name="id">정수 값으로 Component가 가지는 Object ID</param>
	/// <returns> 0 = Success, 그외= Error</returns>
	int SetObjectID(int id);

	/// <summary>
	/// Object ID를 반환한다.
	/// </summary>
	/// <param name=""></param>
	/// <returns>SetObjectID()를 통하여 set된 Object ID를 반환</returns>
	int GetObjectID(void) const;

	/// <summary>
	/// Log File Path를 set한다.
	/// File Path는 반드시 '\\'(backslash)를 포함해야 한다.
	/// </summary>
	/// <param name="path">CString type의 File Path</param>
	/// <returns>0 = Success, 그외= Error</returns>
	int SetLogFilePath(CString path);

	/// <summary>
	/// Log File path를 반환한다.
	/// </summary>
	/// <param name=""></param>
	/// <returns>File path를 반환한다.</returns>
	CString GetLogFilePath(void) const;

	/// <summary>
	/// Log File 이름를 set한다.
	/// 해당이름은 실제 파일이름과 다르다.
	/// 실제이름은 날짜 정보를 포함한다.
	/// </summary>
	/// <param name="name">CString type의 File 이름</param>
	/// <returns>0 = Success, 그외= Error</returns>
	int SetLogFileName(CString name);

	/// <summary>
	/// Log File 이름을 반환한다.
	/// </summary>
	/// <param name=""></param>
	/// <returns>File 이름을 반환한다.</returns>
	CString GetLogFileName(void) const;

	/// <summary>
	/// Log Level을 set한다.
	/// </summary>
	/// <param name="level">레벨 정보</param>
	/// <returns>0 = Success, 그외= Error</returns>
	int SetLogLevel(level level);

	/// <summary>
	/// Log Level을 get한다.
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	level GetLogLevel(void) const;

	/// <summary>
	/// Log file을 저장하고 보관하는 일자를 set한다.
	/// </summary>
	/// <param name="days">(OPTION=30) log file 저장 일</param>
	/// <returns> 0 = Success, 그외= Error</returns>
	int	SetLogKeepingDays(int days = MLOG_DEFAULT_KEEPING_DAYS);

	/// <summary>
	/// Log file을 저장하고 보관하는 일자를 get한다.
	/// </summary>
	/// <param name="">현재 set되어 있는 log file 저장일</param>
	/// <returns></returns>
	int GetLogKeepingDays(void) const;

	/// <summary>
	/// Log file과 관련된 attribute를 한번에 지정한다.
	/// </summary>
	/// <param name="aguments">log data 구조체 주소</param>
	/// <param name="keep_days">유지 기한</param>
	/// <returns>0 = Success, 그외= Error</returns>
	int SetLogAttribute(data_t* aguments, int keep_days = MLOG_DEFAULT_KEEPING_DAYS);

	/// <summary>
	/// Log file과 관련된 attribute를 한번에 얻어온다.
	/// </summary>
	/// <param name="aguments">attribute 구조체 주소</param>
	/// <param name="keep_days">보존 기간</param>
	void GetLogAttribute(data_t* aguments, int* keep_days) const;

	/// <summary>
	/// Get latest log items stored in memory
	/// </summary>
	/// <param name=""></param>
	/// <returns></returns>
	qlistLog* GetLogViewItem(void) const;

	/// <summary>
	/// Get last log item
	/// </summary>
	/// <returns></returns>
	data_t GetLogLastMsg() const;

	/// <summary>
	/// Log file에 log를 기록한다.
	/// </summary>
	/// <param name="level">저정하고자 하는 log level</param>
	/// <param name="log">기록하고자 하는 log</param>
	/// <param name="error_file">log를 기록하고자 호출하는 파일이름</param>
	/// <param name="error_line">log를 기록하고자 호출하는 파일 line</param>
	/// <returns>0 = Success, 그외= Error</returns>
	int WriteLog(level level, CString log, CString error_file, int error_line);

	/// <summary>
	/// 오래된 Log file을 삭제한다.
	/// </summary>
	/// <param name=""></param>
	/// <returns>0 = Success, 그외= Error</returns>
	int DeleteOldLogFiles(void);


	/// <summary>
	/// 현재의 날짜와 시간을 스트링으로 리턴한다.
	/// </summary>
	/// <returns></returns>
	CString GetCurrDateAndTime();

};