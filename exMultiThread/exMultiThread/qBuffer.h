#pragma once
#include "pch.h"
#include "framework.h"

#include "afxstr.h"
#include <iostream>
#include <vector>

#define QUE_BUF_MAX 4096 

/*
* 작성자 : 이길훈
* 21.10.25 byte 링버퍼 사용
*/
class CQueue
{

	CString m_Buf[QUE_BUF_MAX]; //  Buffer
	int m_Head, m_Tail;  // CQueue Head Tail Position

public:


public:
	CQueue() {
		Init();
	}
	virtual ~CQueue() {
		Init();
	}

	inline void Init()
	{
		m_Head = m_Tail = 0;
		for (int i = 0; i < QUE_BUF_MAX; i++)
		{
			m_Buf[i].Empty();
		}
	}


	inline LPVOID Write(CString* data) {
		LPVOID ret = nullptr;
		uint32_t next_in;

		next_in = (m_Head + 1) % QUE_BUF_MAX;
		if (next_in != m_Tail)
		{
			m_Buf[m_Head] = *data;
			ret = (LPVOID)&m_Buf[m_Head];
			m_Head = next_in;
		}

		return ret;
	}



	inline LPVOID Read(CString& data) {
		LPVOID ret = nullptr;

		if (m_Tail != m_Head)
		{
			data = m_Buf[m_Tail];
			ret = &m_Buf[m_Tail];
			m_Tail = (m_Tail + 1) % QUE_BUF_MAX;
		}
		return ret;
	}

	/// <summary>
	/// 채워진 buffer data size 
	/// </summary>
	/// <returns>0 이면 가져갈 데이터 없음</returns>
	inline int Available() {
		return (m_Head - m_Tail) % QUE_BUF_MAX;
	}



	// 버퍼 내 데이터를 모두 처리
	inline void Flush() {
		m_Head = m_Tail = 0;
	}

	inline CString GetNowString()
	{
		CString date_now;
		SYSTEMTIME	systemDate;

		GetLocalTime(&systemDate);

		date_now.Format(_T("[%02d:%02d:%02d] "),
			systemDate.wHour,
			systemDate.wMinute,
			systemDate.wSecond);

		return date_now;
	}

};


static inline int Constrain(int amt, int low, int high) {
	return ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)));
}

static inline BOOL Between(int amt, int low, int high) {
	BOOL ret = FALSE;
	if (amt >= low)
	{
		if (amt < high)
		{
			ret = TRUE;
		}
	}
	return ret;
}
