#pragma once
#include "pch.h"

#define DEF_TICK_TIMER_MAX 				32

const int DEF_TIMER_SUCCESS = 0;
const int ERR_TIMER_NOT_STARTED = 1;
const int ERR_TIMER_ALREADY_STARTED = 2;


/// <summary>
/// Timer Ŭ���� 
/// inline ��
/// 21.10.30 �̱���
/// </summary>
class CtTimer
{
	/*************************************************
	 * data
	 *************************************************/
public:

private:
	BOOL m_isStart;
	ULONGLONG m_vluStart; // vlu = value 

public:

	/*************************************************
	 * constructor (ctor)
	 *************************************************/
	CtTimer()
		:m_isStart(FALSE),m_vluStart(NULL)
	{
	}
	virtual ~CtTimer(){
		;
	}

private:
	
public:
	/// <summary>
	/// Timer ����
	/// </summary>
	/// <returns> 0 : ����, others : ���� </returns>
	int Start() { 
		m_vluStart = GetTickCount64();
		m_isStart = TRUE;
		return DEF_TIMER_SUCCESS;
	}
	
	/// <summary>
	/// Timer ����
	/// </summary>
	/// <returns> 0 : ����, others : ���� </returns>
	int Stop() {
		if (!m_isStart) return ERR_TIMER_NOT_STARTED;
		m_isStart = FALSE;
		return DEF_TIMER_SUCCESS;
	}

	/// <summary>
	/// ��� �ð��� �����Ѵ�.
	/// </summary>
	/// <returns>��� �ð� (��)</returns>
	double GetElaspTime() { 
		ULONGLONG stop_vlu; // Stop���� ��
		ULONGLONG elasp_vlu; // ��� �ð� 

		stop_vlu = GetTickCount64();

		if (!m_isStart) return 0.;

		if (m_vluStart <= stop_vlu)  //check for wrap around condition
			elasp_vlu = stop_vlu - m_vluStart; //no wrap
		else	{
			elasp_vlu = (0xFFFFFFFFL - m_vluStart); //calculate time from start up to wrap
			elasp_vlu += 1;  //time to wrap from FFFFFFFF to 0 is 1 tick
			elasp_vlu += stop_vlu; //add in time after  0
		}
		return (double)elasp_vlu / 1000.;
	}

	/// <summary>
	/// ��� �ð��� �����Ѵ�.
	/// </summary>
	/// <returns>��� �ð�</returns>
	ULONGLONG GetElaspTime_ms() {
		ULONGLONG stop_vlu; // Stop���� ��
		ULONGLONG elasp_vlu; // ��� �ð� 

		stop_vlu = GetTickCount64();

		if (!m_isStart) return 0;

		if (m_vluStart <= stop_vlu)  //check for wrap around condition
			elasp_vlu = stop_vlu - m_vluStart; //no wrap
		else {
			elasp_vlu = (0xFFFFFFFFL - m_vluStart); //calculate time from start up to wrap
			elasp_vlu += 1;  //time to wrap from FFFFFFFF to 0 is 1 tick
			elasp_vlu += stop_vlu; //add in time after  0
		}
		return elasp_vlu;
	}


	/// <summary>
	/// ���� �ð��� �ʰ����� �ʾҴ��� üũ
	/// </summary>
	/// <param name="sec">�����ð� ��(sec)</param>
	/// <returns>TRUE : ����� , FALSE: ������� ����</returns>
	BOOL LessThan(double sec) {
		if (GetElaspTime() < sec) return TRUE;
		else return FALSE;
	}

	/// <summary>
	/// ���� �ð��� �ʰ� �ߴ��� üũ
	/// </summary>
	/// <param name="sec">�����ð� ��(sec)</param>
	/// <returns>TRUE : ������� ���� , FALSE: ����� </returns>
	BOOL MoreThan(double sec) {
		if (GetElaspTime() > sec) return TRUE;
		else return FALSE;
	}

	/// <summary>
	/// Timer ���� ���θ� �˷���
	/// </summary>
	/// <returns></returns>
	BOOL IsStartTimer() const {
		return m_isStart;
	}

};






class CtimeScheduler
{

public:
	enum class eTickType
	{
		none = -1,
		ms_10,
		ms_20,
		ms_100,
		ms_1000,

		ms_max
	};


	/*************************************************
	 * data
	 *************************************************/
private:
	eTickType m_type;
	void (*func_Callback)(LPVOID);
	CallbackType m_callback;

	BOOL m_isStart;
	ULONGLONG m_vluPass; // vlu = value 
	LPVOID m_obj;

	CtTimer m_timer;
	CWinThread* m_thread;
	BOOL m_threadLife;
public:

	/*************************************************
	 * constructor (ctor)
	 *************************************************/
	CtimeScheduler(eTickType type)
		:m_type(eTickType::none), m_isStart(FALSE), m_vluPass(NULL), m_thread(nullptr)
		, m_obj(nullptr), m_callback(nullptr)
	{
		m_type = type;
		m_threadLife = TRUE;
		
		m_timer.Start();
		run();
	}
	virtual ~CtimeScheduler() {

		terminateThread();
	}

private:
	void run() {
		if (m_thread != nullptr) return;
		m_thread = ::AfxBeginThread(timeThread, this);

	}
	static UINT timeThread(LPVOID pParam)	{
		CtimeScheduler* pThis;
		pThis = (CtimeScheduler*)pParam;
		TRACE("\r Start Time Thread \n");
		while (pThis->m_threadLife)
		{
			pThis->timerISR();
			Sleep(1);
		}
		TRACE("\r Exit Time Thread \n");

		return 0;

	}

	void timerISR(void){
		if (m_isStart != TRUE) return;

		ULONGLONG value = 0;
		m_vluPass = m_timer.GetElaspTime_ms() ;


		if (func_Callback != nullptr)		{
			switch (m_type)
			{
			case eTickType::ms_10:
			{
				if (m_vluPass > 10) {
					(*func_Callback)(m_obj);
					m_timer.Start();
				}
			}
			break;
			case eTickType::ms_20:
			{
				if (m_vluPass > 20) {
					(*func_Callback)(m_obj);
					m_timer.Start();
				}
			}
			break;
			case eTickType::ms_100:
			{
				if (m_vluPass > 100) {
					(*func_Callback)(m_obj);
					m_timer.Start();
				}
			}
			break;
			case eTickType::ms_1000:
			{
				if (m_vluPass > 1000) {
					(*func_Callback)(m_obj);
					m_timer.Start();
				}
			}
			break;
			} // switch
		}
	}

	void terminateThread() {

		m_threadLife = FALSE;
		
		/*
		CtTimer t;
		t.Start();
		while (1) {
			if (t.MoreThan(0.5)) break;
		}
		*/
		if (m_thread == nullptr) 
			return;
		
		DWORD result;
		result = ::WaitForSingleObject(m_thread->m_hThread, 1000); // 1�� ��ٸ�
		if (result == WAIT_OBJECT_0)
		{
			// �̰��� �����带 Ȯ���� ����� ������
			TRACE(L"Terminate Thread OK!");
		}
		else if (result == WAIT_TIMEOUT)
		{
			// 1�ʰ� ������ �������� ������� ����
			TRACE(L"Terminate Thread Timeout!");
			::TerminateThread(m_thread->m_hThread, result);
		}
	}

public:
	int Start() { 
		m_isStart = TRUE;
		m_vluPass = 1;

		return DEF_TIMER_SUCCESS;
	}

	int Stop() { 
		m_isStart = FALSE;
		m_vluPass = 0;
		return DEF_TIMER_SUCCESS;
	}
	
	ULONGLONG GetElaspTime() {
		return m_vluPass;
	}


	void EndThread() {
		terminateThread();
	}

	void AttachCallbackFunc(LPVOID obj, void (*func)(LPVOID)){
		func_Callback = func;
		m_obj = obj;
	}

	void AddISRFunc(CallbackType cb) {
		m_callback = cb;
	}


};
