#pragma once


// CTestMain 대화 상자

class CTestMain : public CDialogEx
{
	DECLARE_DYNAMIC(CTestMain)

public:
	CTestMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTestMain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FORMVIEW };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
};
