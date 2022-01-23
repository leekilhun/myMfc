#pragma once
#include "..\CGridListCtrlEx\CGridListCtrlGroups.h"

#include "CListCtrl_LogList.h"

// tab_io 대화 상자

class tab_io : public CDialogEx
{
	DECLARE_DYNAMIC(tab_io)

public:
	tab_io(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~tab_io();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_IO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	//CListCtrl m_listLog;
	CGridListCtrlGroups m_listLog;
	CListCtrl_LogList m_LogData;

	virtual BOOL OnInitDialog();
};
