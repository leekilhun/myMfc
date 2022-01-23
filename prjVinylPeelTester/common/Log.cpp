#include "pch.h"
#include "Log.h"



#include <sys/timeb.h>
#include <time.h>

#include "FileUtil.h"

// SystemInfo 데이터에  등록한 Object 클래스 생성시 로그 클래스를
// 생성한다.


MLog::MLog(void) :m_pLogFile(nullptr), m_pLogView(nullptr)
{
	init();
	m_pLogView = new qlistLog();

	/*초기화 성공시 TRUE 반환*/
	if (InitializeCriticalSectionAndSpinCount(&m_csWriteControl, 10) == false)
		TRACE("\n\r LOG.CPP : Failt InitializeCriticalSectionAndSpinCount ");

}

MLog::~MLog(void)
{
	if (m_pLogView != nullptr)
	{
		delete m_pLogView;
		m_pLogView = nullptr;
	}

	DeleteCriticalSection(&m_csWriteControl);
	init();
}

void MLog::init()
{
	m_pLogView = nullptr;
	m_pLogFile = nullptr;
	m_nLogKeepingDays = MLOG_DEFAULT_KEEPING_DAYS;
}

int MLog::SetObjectID(int id)
{
	m_objID = id;
	return DEF_MLOG_SUCCESS;
}

int MLog::GetObjectID(void) const
{
	return cLog::GetObjID();
}

int MLog::SetLogFilePath(CString path)
{
	m_logFilePath = path;
	return DEF_MLOG_SUCCESS;
}

CString MLog::GetLogFilePath(void) const
{
	return m_logFilePath;
}

int MLog::SetLogFileName(CString name)
{
	m_logFileName = name;
	return DEF_MLOG_SUCCESS;
}

CString MLog::GetLogFileName(void) const
{
	return m_logFileName;
}

int MLog::SetLogLevel(level level)
{
	return cLog::SetLogLevel(level);
}

cLog::level MLog::GetLogLevel(void) const
{
	return cLog::GetLogLevel();
}

int MLog::SetLogKeepingDays(int days)
{
	if (days < 10)
		return ERR_MLOG_TOO_SHORT_KEEPING_DAYS;
	m_nLogKeepingDays = days;
	return DEF_MLOG_SUCCESS;
}

int MLog::GetLogKeepingDays(void) const
{
	return m_nLogKeepingDays;
}

int MLog::SetLogAttribute(data_t* aguments, int keep_days)
{
	int found, length;
	MFileUtil	FileUtil;
	cLog::SetData(aguments);
	CString full_string = aguments->FileInfor;
	length = full_string.GetLength();
	found = full_string.ReverseFind(_T('\\'));
	if (found == -1)
		found = full_string.ReverseFind(_T('/'));
	m_logFilePath = full_string.Left(found + 1);

	if (length - found <= 1)
		m_logFileName.Empty();
	else
		m_logFileName = full_string.Right(length - found - 1);

	m_nLogKeepingDays = keep_days;
	if (FileUtil.MakeDir(m_logFilePath.GetString()))
		return -1;

	return DEF_MLOG_SUCCESS;
}

void MLog::GetLogAttribute(data_t* aguments, int* keep_days) const
{
	cLog::GetData(aguments);
	*keep_days = m_nLogKeepingDays;
}

qlistLog* MLog::GetLogViewItem(void) const
{
	return m_pLogView;
}

cLog::data_t MLog::GetLogLastMsg() const
{
	return m_pLogView->LastMsg();
}


int MLog::WriteLog(level level, CString log, CString error_file, int error_line)
{
	EnterCriticalSection(&m_csWriteControl);

	CString strLogMsg = log;

	struct tm tmNewTime;
	time_t timetClock;
	struct _timeb timebCurrentTime;
	unsigned short usMiliSec;
	TCHAR szTime[128];
	CString strLevel, strLogFile, strFullName;

	int	iFound;
	CString strFileNameOnly;
	CString	strFileNameResult;

	// File Name이 없는 경우는 Log 하지 않고 넘어간다.
	if (m_logFileName.IsEmpty())
	{
		//m_csWrite.Unlock();
		LeaveCriticalSection(&m_csWriteControl);
		return DEF_MLOG_SUCCESS;
	}

	switch (level)
	{
	case cLog::level::None:
		LeaveCriticalSection(&m_csWriteControl);
		return DEF_MLOG_SUCCESS;
		break;
	case cLog::level::Error:
		strLevel = _T("[1-ERR]");
		break;
	case cLog::level::Warning:
		strLevel = _T("[2-WRN]");
		break;
	case cLog::level::Normal:
		strLevel = _T("[3-NOR]");
		break;
	case cLog::level::TactTime:
		strLevel = _T("[4-TACT]");
		break;
	default:
		strLevel = _T("[UNKNOWN]");
		break;
	}


	// Get time in seconds 
	time(&timetClock);
	// Convert time to struct  tm form 
	localtime_s(&tmNewTime,&timetClock);

	strLogFile.Format(_T("%s%04d%02d%02d-%s")
		, m_logFilePath.GetString()
		, tmNewTime.tm_year + 1900
		, tmNewTime.tm_mon + 1
		, tmNewTime.tm_mday
		, m_logFileName.GetString()
	);

	_tstrtime_s(szTime);
	_ftime_s(&timebCurrentTime);

	usMiliSec = (timebCurrentTime.millitm / 10);

	strFullName.Format(_T("[%04d-%02d-%02d %s.%02d] [%03d]")
		, tmNewTime.tm_year + 1900
		, tmNewTime.tm_mon + 1
		, tmNewTime.tm_mday
		, szTime
		, usMiliSec
		, cLog::GetObjID()
	);

	CString str_date;
	str_date.Format(_T("%s.%02d"), szTime, usMiliSec);
	strFileNameOnly = error_file;
	iFound = strFileNameOnly.ReverseFind(_T('/'));
	if (iFound == -1)
		iFound = strFileNameOnly.ReverseFind(_T('\\'));

	strFileNameResult = strFileNameOnly.Mid(iFound + 1);

	cLog::data_t data;

	data.Order = 0;
	data.Date = str_date;
	data.ObjID = cLog::GetObjID();
	data.Level = level;
	data.LogMsg = strLogMsg;
	data.FileInfor = strFileNameResult;
	data.LineNumber = error_line;

	m_pLogView->PostMsg(&data);

	if ((_tfopen_s(&m_pLogFile,strLogFile.GetString(), _T("a"))) == NULL)
	{
		LeaveCriticalSection(&m_csWriteControl);
		return ERR_MLOG_FILE_OPEN_ERROR;
	}
	if (m_pLogFile == nullptr)
		goto _exit;
	_ftprintf_s(m_pLogFile
		, _T("%s %s %s   (%s, %d)\n")
		, strFullName.GetString()
		, strLevel.GetString()
		, strLogMsg.GetString()
		, strFileNameResult.GetString()
		, error_line
	);

	fclose(m_pLogFile);

_exit:
	LeaveCriticalSection(&m_csWriteControl);
	return DEF_MLOG_SUCCESS;
}


int MLog::DeleteOldLogFiles(void)
{
	int pDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	struct tm tmNewTime ;
	time_t timetClock;
	int iLeapYear;
	int iDDay, iDMonth, iDYear, iDElapsedDay, iDiffYear;
	TCHAR szLogFile[256];
	//TCHAR szDeleteFile[256];

	CString strWildCardFileName, strLogFile;

	HANDLE				handleFirstFile;
	WIN32_FIND_DATA		finddataFile;

	CString	currentFile;

	CString str_path, str_name;
	str_path = m_logFilePath;
	str_name = m_logFileName;

	// File Name이 없는 경우는 Log 하지 않고 넘어간다.
	if (m_logFileName.GetLength() == 0)
		return DEF_MLOG_SUCCESS;

	// Get time in seconds 
	time(&timetClock);
	// Convert time to struct  tm form 
	localtime_s(&tmNewTime , &timetClock);

	iLeapYear = tmNewTime.tm_year + 1900;
	if ((!(iLeapYear % 4) && (iLeapYear % 100)) || !(iLeapYear % 400))
		pDays[1] = 29;

	if (str_path.ReverseFind(_T('\\')) == str_path.GetLength() - 1)
	{
		strWildCardFileName.Format(_T("%s*%s")
			, m_logFilePath.GetString()
			, m_logFileName.GetString()
		);
		strLogFile = str_path;
	}
	else if (str_path.ReverseFind(_T('/')) == str_path.GetLength() - 1)
	{
		strWildCardFileName.Format(_T("%s*%s")
			, m_logFilePath.GetString()
			, m_logFileName.GetString()
		);
		strLogFile = str_path;
	}
	else
	{
		strWildCardFileName.Format(_T("%s\\*%s")
			, m_logFilePath.GetString()
			, m_logFileName.GetString()
		);
		strLogFile = str_path + _T('\\');
	}

	//	sprintf(szToday, "%04d%02d%02d", tmNewTime->tm_year+1900, tmNewTime->tm_mon+1, tmNewTime->tm_mday );
	//	iToday = atoi(szToday);

	handleFirstFile = FindFirstFile(strWildCardFileName.GetString(), &finddataFile);
	if (handleFirstFile == INVALID_HANDLE_VALUE)
		return DEF_MLOG_SUCCESS;
	else
	{
		currentFile.Format(finddataFile.cFileName);
		iDYear = _tstoi(currentFile.Left(4));
		iDMonth = _tstoi(currentFile.Mid(4, 2));
		iDDay = _tstoi(currentFile.Mid(6, 2));

		iDiffYear = tmNewTime.tm_year + 1900 - iDYear;

		if (iDiffYear == 0)
		{
			iDElapsedDay = 0;
			for (int i = iDMonth; i < tmNewTime.tm_mon + 2; i++)
			{
				if (iDMonth == tmNewTime.tm_mon + 1)
				{
					iDElapsedDay = tmNewTime.tm_mday - iDDay;
					break;
				}

				if (i == iDMonth)
					iDElapsedDay += (pDays[iDMonth - 1] - iDDay);
				else if (i == tmNewTime.tm_mon + 1)
					iDElapsedDay += tmNewTime.tm_mday;
				else
					iDElapsedDay += pDays[i - 1];
			}

			// 21.07.28 szLogFile 할당을 안됨, 경로 정보가 맞는지 확인필요 _이길훈
			if (iDElapsedDay > m_nLogKeepingDays)
			{
				CString delete_file;
				delete_file.Format(_T("%s%s"), szLogFile, finddataFile.cFileName);
				//sprintf(szDeleteFile, "%s%s", szLogFile, finddataFile.cFileName);
				//DeleteFile(CA2CT(szDeleteFile));
				DeleteFile(delete_file.GetString());
			}
		}
		else
		{
			iDElapsedDay = 0;
			for (int i = iDMonth; i < 13; i++)
			{
				if (i == iDMonth)
					iDElapsedDay += (pDays[iDMonth - 1] - iDDay);
				else
					iDElapsedDay += pDays[i - 1];
			}

			for (int i = 1; i < tmNewTime.tm_mon + 2; i++)
			{
				if (i == tmNewTime.tm_mon + 1)
					iDElapsedDay += tmNewTime.tm_mday;
				else
					iDElapsedDay += pDays[i];
			}

			iDElapsedDay += (iDiffYear - 1) * 365;
			if (iDElapsedDay > m_nLogKeepingDays)
			{
				CString delete_file;
				delete_file.Format(_T("%s%s"), szLogFile, finddataFile.cFileName);
				//sprintf(szDeleteFile, "%s%s", szLogFile, finddataFile.cFileName);
				//DeleteFile(CA2CT(szDeleteFile));
				DeleteFile(delete_file.GetString());
			}
		}
	}
	while (FindNextFile(handleFirstFile, &finddataFile))
	{
		currentFile.Format(finddataFile.cFileName);
		iDYear = _tstoi(currentFile.Left(4));
		iDMonth = _tstoi(currentFile.Mid(4, 2));
		iDDay = _tstoi(currentFile.Mid(6, 2));

		iDiffYear = tmNewTime.tm_year + 1900 - iDYear;

		if (iDiffYear == 0)
		{
			iDElapsedDay = 0;
			for (int i = iDMonth; i < tmNewTime.tm_mon + 2; i++)
			{
				if (iDMonth == tmNewTime.tm_mon + 1)
				{
					iDElapsedDay = tmNewTime.tm_mday - iDDay;
					break;
				}

				if (i == iDMonth)
					iDElapsedDay += (pDays[iDMonth - 1] - iDDay);
				else if (i == tmNewTime.tm_mon + 1)
					iDElapsedDay += tmNewTime.tm_mday;
				else
					iDElapsedDay += pDays[i - 1];
			}

			if (iDElapsedDay > m_nLogKeepingDays)
			{
				CString delete_file;
				delete_file.Format(_T("%s%s"), szLogFile, finddataFile.cFileName);
				//sprintf(szDeleteFile, "%s%s", szLogFile, finddataFile.cFileName);
				//DeleteFile(CA2CT(szDeleteFile));
				DeleteFile(delete_file.GetString());

			}
		}
		else
		{
			iDElapsedDay = 0;
			for (int i = iDMonth; i < 13; i++)
			{
				if (i == iDMonth)
					iDElapsedDay += (pDays[iDMonth - 1] - iDDay);
				else
					iDElapsedDay += pDays[i - 1];
			}

			for (int i = 1; i < tmNewTime.tm_mon + 2; i++)
			{
				if (i == tmNewTime.tm_mon + 1)
					iDElapsedDay += tmNewTime.tm_mday;
				else
					iDElapsedDay += pDays[i];
			}

			iDElapsedDay += (iDiffYear - 1) * 365;
			if (iDElapsedDay > m_nLogKeepingDays)
			{
				CString delete_file;
				delete_file.Format(_T("%s%s"), szLogFile, finddataFile.cFileName);
				//sprintf(szDeleteFile, "%s%s", szLogFile, finddataFile.cFileName);
				//DeleteFile(CA2CT(szDeleteFile));
				DeleteFile(delete_file.GetString());

			}
		}
	}
	return DEF_MLOG_SUCCESS;
}






CString MLog::GetCurrDateAndTime()
{
	CTime tCurrent = CTime::GetCurrentTime();// 현재 시간을 얻는다.
	CString strDate;
	strDate = tCurrent.Format(L"%Y-%m-%d %H:%M:%S");


	return strDate;
}

