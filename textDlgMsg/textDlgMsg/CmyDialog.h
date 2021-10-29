#pragma once


// CmyDialog 대화 상자

class CmyDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CmyDialog)

public:
	CmyDialog(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CmyDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MYDIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progressWT;
	virtual BOOL OnInitDialog();
	CEdit m_edit;
	CStatic m_strWT;
};
