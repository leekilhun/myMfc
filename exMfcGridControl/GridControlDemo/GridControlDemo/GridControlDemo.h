
// GridControlDemo.h : GridControlDemo ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.


// CGridControlDemoApp:
// �� Ŭ������ ������ ���ؼ��� GridControlDemo.cpp�� �����Ͻʽÿ�.
//

class CGridControlDemoApp : public CWinApp
{
public:
	CGridControlDemoApp();


// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// �����Դϴ�.
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGridControlDemoApp theApp;
