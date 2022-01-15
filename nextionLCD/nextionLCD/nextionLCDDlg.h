
// nextionLCDDlg.h: 헤더 파일
//

#pragma once


// CnextionLCDDlg 대화 상자
class CnextionLCDDlg : public CDialogEx
{
	UINT_PTR	m_TimerID;
	nextionCommLcd* m_pLcd;

// 생성입니다.
public:
	CnextionLCDDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEXTIONLCD_DIALOG };
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
	CComboBox m_comboComList;
	CComboBox m_comboBaudrateList;
	CButton m_sensor[8];
	CButton m_led[8];


	afx_msg void OnBnClickedBtnConnect();

	uint16_t GetInputReg();
	uint16_t SetOutputReg();

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
protected:
	afx_msg LRESULT OnUpdateCommLogList(WPARAM wParam, LPARAM lParam);
public:
	CListBox m_recvList;
	afx_msg void OnLbnDblclkListRcvLog();
};
