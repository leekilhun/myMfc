
// exMfcHuskylensDlg.h: 헤더 파일
//

#pragma once
#include "CommRs232.h"
#include "afxdb.h"

#define DEF_BUFF_MAX	4096

class cMemRecordset : public CRecordset
{
public:
	BYTE buff[1024]; //  Buffer
	int m_Head, m_Tail;  // CQueue Head Tail Position

public:
	cMemRecordset() :m_Head(0), m_Tail(0) { Init(); }
	virtual ~cMemRecordset() {
		Init();
	};

public:
	void Init() {
	}



};




// CexMfcHuskylensDlg 대화 상자
class CexMfcHuskylensDlg : public CDialogEx
{
	CCommRs232* m_HwSerialComm;
	BOOL m_IsPortOpen;

	TCHAR m_buff[DEF_BUFF_MAX]; //  Buffer
// 생성입니다.
public:
	CexMfcHuskylensDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual ~CexMfcHuskylensDlg();
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXMFCHUSKYLENS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
public:
	CString GetMiddleString(CString str, int len);

//
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	// 콤포트 List
	CComboBox m_comPort;
	// baudrate list
	CComboBox m_comBaud;
	// 포트 커넥팅
	CButton m_btnOpenClose;
	afx_msg void OnBnClickedButtonOpenclose();
	// 통신결과 표시
	CListBox m_listViewer;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	// 수신데이터
	CEdit m_editRx;
protected:
	afx_msg LRESULT OnUserMessage(WPARAM wParam, LPARAM lParam);


public:
	afx_msg LRESULT OnReceive(WPARAM length, LPARAM lParam);
	afx_msg LRESULT OnReceiveTest(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButton1();

private:
	void clearBuff();


};
