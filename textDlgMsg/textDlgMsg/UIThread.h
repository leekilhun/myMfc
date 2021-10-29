#pragma once

class CmyDialog;

// CUIThread

class CUIThread : public CWinThread
{
	DECLARE_DYNCREATE(CUIThread)

public:
	CmyDialog* m_pMainWnd;


public:
	CUIThread();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CUIThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

protected:
	DECLARE_MESSAGE_MAP()
public:
	int Run();
};


