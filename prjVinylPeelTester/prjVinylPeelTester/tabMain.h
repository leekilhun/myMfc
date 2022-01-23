#pragma once


// tabMain 대화 상자

class tabMain : public CDialogEx
{
	DECLARE_DYNAMIC(tabMain)

public:
	tabMain(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~tabMain();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OLE_PROPPAGE_LARGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
