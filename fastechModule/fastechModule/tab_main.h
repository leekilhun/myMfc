#pragma once
#include "popMsg.h"

// tab_main 대화 상자

class tab_main : public CDialogEx
{
	popMsg* m_pDlgPop;

	DECLARE_DYNAMIC(tab_main)

public:
	tab_main(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~tab_main();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_MAIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
};
