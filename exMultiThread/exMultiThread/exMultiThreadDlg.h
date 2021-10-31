
// exMultiThreadDlg.h: 헤더 파일
//

#pragma once


// CexMultiThreadDlg 대화 상자
class CexMultiThreadDlg : public CDialogEx
{
// 생성입니다.
public:
	CexMultiThreadDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXMULTITHREAD_DIALOG };
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
	afx_msg void OnBnClickedBtnTerminate();
	CListBox m_listBd;
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnStart();
	CEdit m_playerCnt;
	CSpinButtonCtrl m_spCnt;
	afx_msg void OnDeltaposSpinCnt(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
