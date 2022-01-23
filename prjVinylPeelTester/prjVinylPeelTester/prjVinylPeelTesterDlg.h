
// prjVinylPeelTesterDlg.h: 헤더 파일
//

#pragma once
#include "tabMain.h"
#include "tabMotor.h"


// CprjVinylPeelTesterDlg 대화 상자
class CprjVinylPeelTesterDlg : public CDialogEx
{
	tabMain* m_pTabMain;
	tabMotor* m_pTabMotor;
	UINT_PTR	m_TimerID;

// 생성입니다.
public:
	CprjVinylPeelTesterDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PRJVINYLPEELTESTER_DIALOG };
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
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangeTabBody(NMHDR* pNMHDR, LRESULT* pResult);
};
