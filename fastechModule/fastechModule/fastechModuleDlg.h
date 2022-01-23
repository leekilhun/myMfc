
// fastechModuleDlg.h: 헤더 파일
//

#pragma once
#include "tab_main.h"
#include "tab_io.h"
#include "tab_test.h"



// CfastechModuleDlg 대화 상자
class CfastechModuleDlg : public CDialogEx
{

	tab_main* m_pTabMain;
	tab_io* m_pTabIo;
	tab_test* m_pTabTest;


// 생성입니다.
public:
	CfastechModuleDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FASTECHMODULE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;


	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabBody;
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTcnSelchangeTabBody(NMHDR* pNMHDR, LRESULT* pResult);
};
