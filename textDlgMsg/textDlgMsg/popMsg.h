#pragma once
class CtextDlgMsgDlg;

// CpopMsg 대화 상자

class CpopMsg : public CDialogEx
{
	DECLARE_DYNAMIC(CpopMsg)


public:
	CtextDlgMsgDlg* m_pDlg;

public:
	CpopMsg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CpopMsg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CHILD1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_msg;



public:
	void setter(CtextDlgMsgDlg* parent);
protected:
	afx_msg LRESULT OnPopReceive(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
