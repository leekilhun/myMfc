
// textDlgMsgDlg.h: 헤더 파일
//

#pragma once
#include "popMsg.h"

// CtextDlgMsgDlg 대화 상자
class CtextDlgMsgDlg : public CDialogEx
{

public:
	CpopMsg* m_popMsg;
	CmsgThread* m_thread;


// 생성입니다.
public:
	CtextDlgMsgDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TEXTDLGMSG_DIALOG };
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
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnPop();
	virtual BOOL DestroyWindow();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnPause();
	afx_msg void OnBnClickedBtnResume();
	afx_msg void OnBnClickedBtnEnd();
};
