#pragma once

#include <vector>
#include <iostream>
#include <algorithm>

using namespace std;

struct CListCtrl_LogRecord
{
	CListCtrl_LogRecord()
	{}

	CListCtrl_LogRecord(const CString& DateTime, const CString& level_str,const CString& context)
		:m_DateTime(DateTime), m_LogLevel(level_str), m_LogContext(context)
	{}

	CString	m_DateTime;
	CString	m_LogLevel;
	CString	m_LogContext;

	CString GetCellText_Struct(int col, bool title) const
	{
		switch (col)
		{
		case 0: { static const CString title0(_T("DateTime")); return title ? title0 : m_DateTime; }
		case 1: { static const CString title1(_T("Level")); return title ? title1 : m_LogLevel; }
		case 2: { static const CString title2(_T("Context")); return title ? title2 : m_LogContext; }
		default: { static const CString emptyStr; return emptyStr; }
		}
	}

	int  GetColCount() const { return 2; } //set column number, Rex
};

class CListCtrl_LogList
{
	vector<CListCtrl_LogRecord> m_Records;
	int	m_LookupTime;
	int m_RowMultiplier;

public:
	CListCtrl_LogList()
		:m_RowMultiplier(0)
		, m_LookupTime(0)
	{
		InitDataModel();
	}

	void InitDataModel()
	{
		m_Records.clear();
		m_Records.push_back(CListCtrl_LogRecord(_T("2022-01-02 10:30:40 40"), _T("Info"), _T("please check it!")));

		if (m_RowMultiplier > 0)
		{
			vector<CListCtrl_LogRecord> rowset(m_Records);
			m_Records.reserve((m_RowMultiplier - 1) * rowset.size());
			for (int i = 0; i < m_RowMultiplier; ++i)
			{
				m_Records.insert(m_Records.end(), rowset.begin(), rowset.end());
			}
		}
	}

	void ShowAllDataModel()
	{
		CString s0, s1, s2;

		for (int i = 0; i <(int) m_Records.size(); i++) {
			s0 = m_Records.at(i).GetCellText_Struct(0, false);
			s1 = m_Records.at(i).GetCellText_Struct(1, false);
			s2 = m_Records.at(i).GetCellText_Struct(2, false);
			//m_Records.at(i).m_City
			TRACE(L"%d = %s, %s, %s\n", i, s0, s1, s2
			);
		}
	}

	CString GetCellText(size_t lookupId, int col) const
	{
		if (lookupId >= m_Records.size())
		{
			static CString oob(_T("Out of Bound"));
			return oob;
		}
		// How many times should we search sequential for the row ?
		for (int i = 0; i < m_LookupTime; ++i)
		{
			for (size_t rowId = 0; rowId < m_Records.size(); ++rowId)
			{
				if (rowId == lookupId)
					break;
			}
		}
		return m_Records.at(lookupId).GetCellText_Struct(col, false);
	}

	vector<CString> GetLogMenu() const
	{
		vector<CString> log_menu;
		for (size_t rowId = 0; rowId < m_Records.size(); ++rowId)
			log_menu.push_back(m_Records[rowId].m_LogLevel);
		sort(log_menu.begin(), log_menu.end());
		log_menu.erase(unique(log_menu.begin(), log_menu.end()), log_menu.end());
		return log_menu;
	}

	size_t GetRowIds() const { return m_Records.size(); }
	int GetColCount() const { return CListCtrl_LogRecord().GetColCount(); }
	CString GetColTitle(int col) const { return CListCtrl_LogRecord().GetCellText_Struct(col, true); }

	vector<CListCtrl_LogRecord>& GetRecords() { return m_Records; }
	void SetLookupTime(int lookupTimes) { m_LookupTime = lookupTimes; }
	void SetRowMultiplier(int multiply) { if (m_RowMultiplier != multiply) { m_RowMultiplier = multiply; InitDataModel(); } }
};