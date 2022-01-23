#pragma once


// popDlg_axisStatus 대화 상자

class popDlg_axisStatus : public CDialogEx
{
	DECLARE_DYNAMIC(popDlg_axisStatus)

	UINT_PTR m_TimerID;

public:
	popDlg_axisStatus(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~popDlg_axisStatus();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_POPDLG_STATUS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CButton m_state[32];
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);


private:
	void update();
};
