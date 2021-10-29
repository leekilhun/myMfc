
// exThreadProgressDlg.h: 헤더 파일
//

#pragma once


// CexThreadProgressDlg 대화 상자
class CexThreadProgressDlg : public CDialogEx
{

	int m_i;
	int m_per;

public:
	CWinThread* m_threadWT;
	CWinThread* m_threadUI;
	BOOL m_lifeThreadWT;
	BOOL m_lifeThreadUI;


// 생성입니다.
public:
	CexThreadProgressDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CexThreadProgressDlg();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXTHREADPROGRESS_DIALOG };
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
	CProgressCtrl m_progressWT;
	CProgressCtrl m_progressUI;
	CStatic m_strWT;
	CStatic m_strUI;
	afx_msg void OnBnClickedBtnStartWt();
	afx_msg void OnBnClickedBtnStartUi();



	static UINT WorkThread(LPVOID lParam);
	static UINT ThreadFunc(LPVOID lParam);
	void StopThread();
	void threadJob();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedBtnQuit();
};

