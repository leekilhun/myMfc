
// exCallBackFuncDlg.h: 헤더 파일
//

#pragma once
class mb;

// CexCallBackFuncDlg 대화 상자
class CexCallBackFuncDlg : public CDialogEx
{
public:
	mb* m_pB;


// 생성입니다.
public:
	CexCallBackFuncDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXCALLBACKFUNC_DIALOG };
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
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButton1();
protected:
	afx_msg LRESULT OnUserCallback(WPARAM wParam, LPARAM lParam);

public:
	void Callback();
};
