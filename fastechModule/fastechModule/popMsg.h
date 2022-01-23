#pragma once


// popMsg 대화 상자

class popMsg : public CDialogEx
{
	DECLARE_DYNAMIC(popMsg)

public:
	popMsg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~popMsg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_POP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
