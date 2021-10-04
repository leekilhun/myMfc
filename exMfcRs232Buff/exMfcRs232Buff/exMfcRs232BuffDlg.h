
// exMfcRs232BuffDlg.h: 헤더 파일
//

#pragma once
#include "Mycomm.h"

// CexMfcRs232BuffDlg 대화 상자
class CexMfcRs232BuffDlg : public CDialogEx
{
	CMycomm* m_comm;
	BOOL comport_state;

// 생성입니다.
public:
	CexMfcRs232BuffDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	virtual ~CexMfcRs232BuffDlg() {
		if (m_comm != nullptr) {
			delete m_comm; 
			m_comm = nullptr;
		}
	}
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXMFCRS232BUFF_DIALOG };
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
	CComboBox m_combo_comport_list;
	CString m_str_comport;
	CComboBox m_combo_baudrate_list;
	CString m_str_baudrate;
	CEdit m_edit_rcv_view;
	CEdit m_edit_send_data;


public:
	LRESULT OnThreadClosed(WPARAM length, LPARAM lpara);
	LRESULT OnReceive(WPARAM length, LPARAM lpara);
	afx_msg void OnBnClickedBtConnect();
	afx_msg void OnBnClickedBtClear();
	afx_msg void OnBnClickedBtSend();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
