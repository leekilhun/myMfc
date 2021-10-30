#pragma once
#include <iostream>
#include "qBuffer.h"



#if 0
참조 https ://202psj.tistory.com/1052

출처 //https://saack.tistory.com/65


MFC에서는 Multithread 두 가지 형태로 지원
- .Worker thread
- .User Interface thread

1. work thread
- ::AfxBeginThread()함수를 이용
- 메세지 루프를 포함하지 않고, 스레드 작업 중 입력을 허가하지 않는 스레드 형식
->스레드 작업 중 별도의 입력이 필요 없을 때 사용

[사용법]
스레드 시작
1) CWinThread* pWorkThread = AfxBeginThread(ThreadFunc, &pParam);
->ThreadFunc가 전역함수인 경우&& Param값을 생성하여 넘겨줄 경우

2) CWinThread* pWorkThread = AfxBeginThread(ThreadFunc, this);
->ThreadFunc가 전역함수인 경우&& Param으로 이 스레드를 시작하는 클래스를 넘겨줄 경우

3) CWinThread* pWorkThread = AfxBeginThread(ThisClass::ThreadFunc, &pParam);
->ThreadFunc이 해당 클래스의 멤버함수인 경우&& pParam값을 생성하여 넘겨줄 경우

4) CWinThread* pWorkThread = AfxBeginThread(ThisClass::ThreadFunc, this);
->ThreadFunc이 해당 클래스의 멤버함수인 경우&& pParam으로 이 스레드를 해당 클래스를 넘겨줄 경우

스레드 일시중지
- 주 Process에서 해야하며, 스레드 자신이 호출 불가.
pWorkThread->SuspendThread();
스레드 재개
- 주 Process에서 해야하며, 스레드 자신이 호출불가.
pWorkThread->ResumeThread();
스레드 동작 검사
- DWORD dwContextID; // 스레드 상태가 저장될 변수
GetExitCodeThread(pWorkThread->m_hThread, &dwContextID);

Sleep() 적용
ex) Sleep(2000) : 제어권을 다른 Process에 넘겨 주게 된다.
ex) Sleep(0) : 우선 순위가 높거나 같은 Process에 넘겨 주고 우선 순위가 높거나
같은 Process가 없을 경우에는 아무 일도 생기지 않음.

스레드 종료
1) TerminateThread(pWorkThread->m_hThread, 0);
->스레드를 강제 종료한다.메모리 릭이 발생할 수 있으므로 추천하지 않음.

2) WaitForSingleObject(); -무한
if (::WaitForSingleObject(pThread->m_hThread, INFINITE))
{
	// 이곳은 스레드를 확실히 종료된 상태임
}
-> INFINITE 구문을 사용하여 스레드의 확실한 종료를 기다린다.

3) WaitForSingleObject(); -시간지정
result = ::WaitForSingleObject(pThread->m_hThread, 1000); // 1초 기다림
if (result == WAIT_OBJECT_0)
{
	// 이곳은 스레드를 확실히 종료된 상태임
}
else if (result == WAIT_TIMEOUT)
{
	// 1초가 지나도 스레도가 종료되지 않음
}
-> 시간 지정을 하여 스레드 종료 시 먹통 예방

스레드 실행중인지 검사
if (::WaitForSingleObject(pThread->m_hThread, 0) == WAIT_TIMEOUT)
{
	// pThread 실행 중
}
else
{
	// pThread 실행 상태가 아님
}

2. UI Thread
- 메시지 루프를 포함하여 스레드 작업 중 사용자의 입력을 메시지 형태로 받아 처리하는 스레드 형식
->스레드 작업 중 별도의 입력이 필요할 때 사용

User Interface Thread는  그 자체로 윈도우와 메시지 루프를 가지고 있다.

class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)
public:
	virtual BOOL InitInstance();
};
이 User Interface Thread는 독자의 윈도우도 가질 수 있다.일반적으로 전용 Dialog를 띄워
Thread를 처리하는 경우가 많으므로 이 User Dialog를 CMYDialog라고 이름 지었다고 가정하면

IMPLEMENT_DYNCREATE(CUIThread, CWinThread)
BOOL CUIThread::InitInstance()
{
	m_pMainWnd = new CMYDialog;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}
와 같이 CMYDialog를 Thread로 띄울 수  있다.그 다음
CWinThread* pThread = ::AfxBeginThread(RUNTIME_CLASS(CUIThread));
와 같이 하면 MFC가 알아서 CUIThread를 생성해서 그 포인터를 pThread에 넘겨 준다.

아래 예제에는 CMyDialog를 띄우고 주 Process는 사용자의
입력을 기다린다.Dialog의 Design 및 생성은 별도로 이야기 하지 않는다.
아래 예제를 사용하기 위해 CMyDialog를 만들고 ID를 ID_MYDIALOG라고 가정하면 CMyDialog의 생성자에 다음과 같이 추가

CMyDialog::CMyDialog(CWind* pParent /*=NULL*/)
	: Dialog(CMyThread::IDD, pParent)
{
	Create(IDD_MYDIALOG, NULL);
}

이제 완전히 별도로 동작하는(Thread로 동작하는) 윈도우를 하나 가지는 것이다.
만약 이것을 DialogFrameWnd라고해도 거의 똑같다.다만 위에서도 언급했듯이 Thread를 이용할 때는 Dialog가
Worker thread에 비해 훨씬 더 많은 기능을 하는 것을 알게 되었을 것이다.

나머지 것들은 위의 Worker thread에 있는 내용과 동일하다

만약 여러 개의 CUIThread를 만들려면

CWinThread* pParent[5];
for (int i = 0; i < 5; i++)
{
	m_pThread[i] = ::AfxBeginThread(RUNTIME_CLASS(CUIThread));
}
와 같이 하면 5개의 Thread가 생성된다.


# Program Execution Priority(프로그램 실행 우선 순위)
	Thread는 0 ~31까지의 priority를 가질 수 있다.
	프로그램의 priority는
	BOOL SetPriorityClass(HANDLE hProcess, DWORD dwPriorityClass);
	함수를 이용해서 조정할 수 있다.
	첫번째 인자는::AfxGetinstanceHandle()로 얻으면 된다.


	dwPriorityClass
	Execution Priority				: Description
	---------------------------------------------------- -
	IDLE_PRIORITY_CLASS				: CPU가 IDLE일 때만 사용 가능
	NORMAL_PRIORITY_CLASS			: 보통
	HIGH_PRIORITY_CLASS				: 높은 우선 순위
	REALTIME_PRIORITY_CLASS		: 최상위의 우선 순위
	------------------------------------------------------

# Thread Execution Priority(스레드 실행 우선 순위)
	::AfxBeginThread()함수의 nPriority를 통해 설정하거나
	CWinThread::SetThreadPriority를 사용
	
	BOOL SetThreadPriority(HANDLE hThread, int nPriority);
	dwPriorityClass
	Execution Priority						: Description
	---------------------------------------------------- -
	THREAD_PRIORITY_IDLE					: REALTIME_PRIORITY_CLASS의 경우 16, 그 외는 1
	THREAD_PRIORITY_LOWEST				: 프로세스의 우선순위보다 2단계 낮은 우선 순위를 가짐
	THREAD_PRIORITY_BELOW_NORMAL	: 프로세스의 우선순위보다 1단계 낮은 우선 순위를 가짐
	THREAD_PRIORITY_NORMAL				: 프로세스의 우선순위와 같은 우선순위
	THREAD_PRIORITY_ABOVE_NORMAL	: 프로세스의 우선순위보다 1단계 높은 우선 순위를 가짐
	THREAD_PRIORITY_HIGHEST				: 프로세스의 우선순위보다 2단계 높은 우선 순위를 가짐
	THREAD_PRIORITY_TIME_CRITICAL	: REALTIME_PRIORITY_CLASS의 경우 31 그 외에는 16
	------------------------------------------------------
	프로그래머가 우선순위를 조정해도 Window Scheduler가 상황에 맞게 조정하기 때문에
	우선순위가 생각보다 조금 다를 수 있다.


# Thread & Memory
	각 Thread의 지역 변수는 모두 별도로 Stack을 만들고 Loacal Variable들을 관리하기 때문에
	CWinThread* pParent[5];
	for (int i = 0; i < 5; i++)
	{
		m_pThread[i] = ::AfxBeginThread(RUNTIME_CLASS(CUIThread));
	}
	위 경우에도 각 Thread가 다른 Thread를 침범하는 일은 없다.



#endif // 0



class CmsgThread
{

	/*
	변수 
	*/

public:
	HWND m_msgDlg;
	CWinThread* m_pThread;
	BOOL m_ThreadLife;
	BOOL m_Continue;
	DWORD m_StateThread;//  dwContextID;
	UINT m_Number;
	CQueue* m_Qbuf;

	CallbackType m_isrFunc;
	void* m_object;
	void* m_closure;


	// 생성입니다.
public:
	CmsgThread() { 
		m_pThread = nullptr;
		m_Qbuf = nullptr;
		m_StateThread = 0;
		m_Number = 0;
		m_Continue = FALSE;
		m_ThreadLife = TRUE;
		m_msgDlg = nullptr;
		m_isrFunc = nullptr;
		m_object = nullptr;
		m_closure = nullptr;

		m_Qbuf = new CQueue;
	}
	virtual ~CmsgThread() { 
		m_Continue = FALSE;
		TerminateThread();
		if (m_pThread != nullptr)
		{
			//delete m_pThread;
			m_pThread = nullptr;
		}

		if (m_Qbuf != nullptr)
		{
			delete m_Qbuf;
			m_Qbuf = nullptr;
		}
	}


	void SetIRS(void* obj, CallbackType cb, void* data)
	{
		m_isrFunc = cb;
		m_object = obj;
		m_closure = data;
	}

	UINT ThreadPriorityNum(LPVOID pParam)
	{
		;
	}

	void ThreadRun()
	{
		if (m_pThread != nullptr) return;

		m_pThread = ::AfxBeginThread(threadFunc, this);

	}
	static UINT threadFunc(LPVOID pParam)
	{
		CmsgThread* pThis;
		pThis = (CmsgThread*)pParam;
		TRACE("\r Start Thread \n");
		while (pThis->m_ThreadLife)
		{
			pThis->threadJob();
			Sleep(10);
		}
		TRACE("\r Exit Thread \n");

		return 0;

	}

	void threadJob()
	{
		doRunStep();
	}

	void doRunStep()
	{
		if (m_Continue == FALSE) return;

		m_Number++;
		CString get_str;
		get_str.Format(L"Get No:%d", m_Number);
		CString* send_msg;
		send_msg = (CString *)m_Qbuf->Write(&get_str);
		SendMsg(send_msg, m_Number);

		if (m_isrFunc != nullptr)
		{
			(*m_isrFunc)(m_object, get_str, m_closure);
		}
	}


	void SuspendThread()
	{
		//GetExitCodeThread(m_pThread->m_hThread, &m_StateThread);
		if (IsRun())
		{
			m_pThread->SuspendThread();
		}
	}

	void ResumeThread()
	{
		if (IsRun())
		{
			m_pThread->ResumeThread();
		}

	}


	void RunThread()
	{
		//if (IsRun()) return;
		m_Continue = TRUE;
	}


	void TerminateThread()
	{
		if (IsRun())
		{
			m_pThread->ResumeThread();
			
			m_ThreadLife = FALSE;
			DWORD result;
			result = ::WaitForSingleObject(m_pThread->m_hThread, 1000); // 1초 기다림
			if (result == WAIT_OBJECT_0)
			{
				// 이곳은 스레드를 확실히 종료된 상태임
				TRACE(L"Terminate Thread OK!");
			}
			else if (result == WAIT_TIMEOUT)
			{
				// 1초가 지나도 스레도가 종료되지 않음
				TRACE(L"Terminate Thread Timeout!");
			}

		}
	}

	BOOL IsRun()
	{
		if (m_pThread == nullptr) return FALSE;
		if (::WaitForSingleObject(m_pThread->m_hThread, 0) == WAIT_TIMEOUT)
		{
			return TRUE;
		}
		return FALSE;
	}

	void SetHwnd(HWND hnd) {
		m_msgDlg = hnd;
	}

	void SendMsg(CString* msg, UINT no) {
		if (m_msgDlg == nullptr) return;
		CString* pmsg;
		CString get_msg;
		while (1)
		{
			if (m_Qbuf->Available() == 0) break;
			pmsg = (CString*)m_Qbuf->Read(get_msg);
			//CString*pMsg = new CString (data.GetLogString ());
			::PostMessage(m_msgDlg, WM_POP_RECEIVE, (WPARAM)no, (LPARAM)pmsg); 
		}
	}

};

