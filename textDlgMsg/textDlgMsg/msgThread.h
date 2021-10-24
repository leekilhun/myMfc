#pragma once
#include <iostream>



#if 0

��ó //https://saack.tistory.com/65


MFC������ Multithread �� ���� ���·� ����
- .Worker thread
- .User Interface thread

1. work thread
- ::AfxBeginThread()�Լ��� �̿�
- �޼��� ������ �������� �ʰ�, ������ �۾� �� �Է��� �㰡���� �ʴ� ������ ����
->������ �۾� �� ������ �Է��� �ʿ� ���� �� ���

[����]
������ ����
1) CWinThread* pWorkThread = AfxBeginThread(ThreadFunc, &pParam);
->ThreadFunc�� �����Լ��� ���&& Param���� �����Ͽ� �Ѱ��� ���

2) CWinThread* pWorkThread = AfxBeginThread(ThreadFunc, this);
->ThreadFunc�� �����Լ��� ���&& Param���� �� �����带 �����ϴ� Ŭ������ �Ѱ��� ���

3) CWinThread* pWorkThread = AfxBeginThread(ThisClass::ThreadFunc, &pParam);
->ThreadFunc�� �ش� Ŭ������ ����Լ��� ���&& pParam���� �����Ͽ� �Ѱ��� ���

4) CWinThread* pWorkThread = AfxBeginThread(ThisClass::ThreadFunc, this);
->ThreadFunc�� �ش� Ŭ������ ����Լ��� ���&& pParam���� �� �����带 �ش� Ŭ������ �Ѱ��� ���

������ �Ͻ�����
- �� Process���� �ؾ��ϸ�, ������ �ڽ��� ȣ�� �Ұ�.
pWorkThread->SuspendThread();
������ �簳
- �� Process���� �ؾ��ϸ�, ������ �ڽ��� ȣ��Ұ�.
pWorkThread->ResumeThread();
������ ���� �˻�
- DWORD dwContextID; // ������ ���°� ����� ����
GetExitCodeThread(pWorkThread->m_hThread, &dwContextID);

Sleep() ����
ex) Sleep(2000) : ������� �ٸ� Process�� �Ѱ� �ְ� �ȴ�.
ex) Sleep(0) : �켱 ������ ���ų� ���� Process�� �Ѱ� �ְ� �켱 ������ ���ų�
���� Process�� ���� ��쿡�� �ƹ� �ϵ� ������ ����.

������ ����
1) TerminateThread(pWorkThread->m_hThread, 0);
->�����带 ���� �����Ѵ�.�޸� ���� �߻��� �� �����Ƿ� ��õ���� ����.

2) WaitForSingleObject(); -����
if (::WaitForSingleObject(pThread->m_hThread, INFINITE))
{
	// �̰��� �����带 Ȯ���� ����� ������
}
-> INFINITE ������ ����Ͽ� �������� Ȯ���� ���Ḧ ��ٸ���.

3) WaitForSingleObject(); -�ð�����
result = ::WaitForSingleObject(pThread->m_hThread, 1000); // 1�� ��ٸ�
if (result == WAIT_OBJECT_0)
{
	// �̰��� �����带 Ȯ���� ����� ������
}
esle if (result == WAIT_TIMEOUT)
{
	// 1�ʰ� ������ �������� ������� ����
}
-> �ð� ������ �Ͽ� ������ ���� �� ���� ����

������ ���������� �˻�
if (::WaitForSingleObject(pThread->m_hThread, 0) == WAIT_TIMEOUT)
{
	// pThread ���� ��
}
else
{
	// pThread ���� ���°� �ƴ�
}

2. UI Thread
- �޽��� ������ �����Ͽ� ������ �۾� �� ������� �Է��� �޽��� ���·� �޾� ó���ϴ� ������ ����
->������ �۾� �� ������ �Է��� �ʿ��� �� ���

User Interface Thread��  �� ��ü�� ������� �޽��� ������ ������ �ִ�.

class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)
public:
	virtual BOOL InitInstance();
};
�� User Interface Thread�� ������ �����쵵 ���� �� �ִ�.�Ϲ������� ���� Dialog�� ���
Thread�� ó���ϴ� ��찡 �����Ƿ� �� User Dialog�� CMYDialog��� �̸� �����ٰ� �����ϸ�

IMPLEMENT_DYNCREATE(CUIThread, CWinThread)
BOOL CUIThread::InitInstance()
{
	m_pMainWnd = new CMYDialog;
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdataWindow();
	return TRUE;
}
�� ���� CMYDialog�� Thread�� ��� ��  �ִ�.�� ����
CWinThread* pThread = ::AfxBeginThread(RUNTIME_CLASS(CUIThread));
�� ���� �ϸ� MFC�� �˾Ƽ� CUIThread�� �����ؼ� �� �����͸� pThread�� �Ѱ� �ش�.

�Ʒ� �������� CMyDialog�� ���� �� Process�� �������
�Է��� ��ٸ���.Dialog�� Design �� ������ ������ �̾߱� ���� �ʴ´�.
�Ʒ� ������ ����ϱ� ���� CMyDialog�� ����� ID�� ID_MYDIALOG��� �����ϸ� CMyDialog�� �����ڿ� ������ ���� �߰�

CMyDialog::CMyDialog(CWind* pParent /*=NULL*/)
	: Dialog(CMyThread::IDD, pParent)
{
	Create(IDD_MYDIALOG, NULL);
}

���� ������ ������ �����ϴ�(Thread�� �����ϴ�) �����츦 �ϳ� ������ ���̴�.
���� �̰��� DialogFrameWnd����ص� ���� �Ȱ���.�ٸ� �������� ����ߵ��� Thread�� �̿��� ���� Dialog��
Worker thread�� ���� �ξ� �� ���� ����� �ϴ� ���� �˰� �Ǿ��� ���̴�.

������ �͵��� ���� Worker thread�� �ִ� ����� �����ϴ�

���� ���� ���� CUIThread�� �������

CWinThread* pParent[5];
for (int i = 0; i < 5; i++)
{
	m_pThread[i] = ::AfxBeginThread(RUNTIME_CLASS(CUIThread));
}
�� ���� �ϸ� 5���� Thread�� �����ȴ�.


# Program Execution Priority(���α׷� ���� �켱 ����)
	Thread�� 0 ~31������ priority�� ���� �� �ִ�.
	���α׷��� priority��
	BOOL SetPriorityClass(HANDLE hProcess, DWORD dwPriorityClass);
	�Լ��� �̿��ؼ� ������ �� �ִ�.
	ù��° ���ڴ�::AfxGetinstanceHandle()�� ������ �ȴ�.


	dwPriorityClass
	Execution Priority				: Description
	---------------------------------------------------- -
	IDLE_PRIORITY_CLASS				: CPU�� IDLE�� ���� ��� ����
	NORMAL_PRIORITY_CLASS			: ����
	HIGH_PRIORITY_CLASS				: ���� �켱 ����
	REALTIME_PRIORITY_CLASS		: �ֻ����� �켱 ����
	------------------------------------------------------

# Thread Execution Priority(������ ���� �켱 ����)
	::AfxBeginThread()�Լ��� nPriority�� ���� �����ϰų�
	CWinThread::SetThreadPriority�� ���
	
	BOOL SetThreadPriority(HANDLE hThread, int nPriority);
	dwPriorityClass
	Execution Priority						: Description
	---------------------------------------------------- -
	THREAD_PRIORITY_IDLE					: REALTIME_PRIORITY_CLASS�� ��� 16, �� �ܴ� 1
	THREAD_PRIORITY_LOWEST				: ���μ����� �켱�������� 2�ܰ� ���� �켱 ������ ����
	THREAD_PRIORITY_BELOW_NORMAL	: ���μ����� �켱�������� 1�ܰ� ���� �켱 ������ ����
	THREAD_PRIORITY_NORMAL				: ���μ����� �켱������ ���� �켱����
	THREAD_PRIORITY_ABOVE_NORMAL	: ���μ����� �켱�������� 1�ܰ� ���� �켱 ������ ����
	THREAD_PRIORITY_HIGHEST				: ���μ����� �켱�������� 2�ܰ� ���� �켱 ������ ����
	THREAD_PRIORITY_TIME_CRITICAL	: REALTIME_PRIORITY_CLASS�� ��� 31 �� �ܿ��� 16
	------------------------------------------------------
	���α׷��Ӱ� �켱������ �����ص� Window Scheduler�� ��Ȳ�� �°� �����ϱ� ������
	�켱������ �������� ���� �ٸ� �� �ִ�.


# Thread & Memory
	�� Thread�� ���� ������ ��� ������ Stack�� ����� Loacal Variable���� �����ϱ� ������
	CWinThread* pParent[5];
	for (int i = 0; i < 5; i++)
	{
		m_pThread[i] = ::AfxBeginThread(RUNTIME_CLASS(CUIThread));
	}
	�� ��쿡�� �� Thread�� �ٸ� Thread�� ħ���ϴ� ���� ����.



#endif // 0



class CmsgThread
{

	/*
	���� 
	*/

public:
	CWinThread* m_pThread;
	BOOL				m_Continue;
	DWORD m_StateThread;//  dwContextID;

	// �����Դϴ�.
public:
	CmsgThread() { 
		m_pThread = nullptr;
		m_StateThread = 0;
		m_Continue = TRUE;
	}
	~CmsgThread() { 
		m_Continue = FALSE;
	}

	UINT ThreadPriorityNum(LPVOID pParam)
	{
		;
	}

	void RunThread()
	{
		m_pThread = ::AfxBeginThread(StartThread, &m_Continue);

	}
	static UINT StartThread(LPVOID pParam)
	{
		BOOL* pbContinue = (BOOL*)pParam;
		int count = 0;
		while (pbContinue)
		{
			count++;
			Sleep(10);
		}
		std::cout << "Exit Thread" << std::endl;

		return 0;

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



	void TerminateThread()
	{
		if (IsRun())
		{
			m_Continue = FALSE;
		}
	}

	BOOL IsRun()
	{
		if (::WaitForSingleObject(m_pThread->m_hThread, 0) == WAIT_TIMEOUT)
		{
			return TRUE;
		}
		return FALSE;
	}





};

