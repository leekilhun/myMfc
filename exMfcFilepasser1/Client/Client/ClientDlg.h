
// ClientDlg.h: 헤더 파일
//

#pragma once

class SocketClient;

// CClientDlg 대화 상자
class CClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	SocketClient* m_SocketClient;


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.

private:
	void initComponents();
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	CComboBox m_comboProtocolList;
	CProgressCtrl m_progress;
	CListBox logMessage;
	CComboBox m_comboDataBitsList;
	CComboBox m_comboStopBitsList;
	CComboBox m_comboBaudrateList;
	CComboBox m_comboParityList;
	CEdit m_IpEditBox;
	CEdit m_PortEditBox;
	CComboBox m_comboComList;

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnEnChangeIpEdit();
	afx_msg void OnEnChangePortEdit();
	afx_msg void OnBnClickedButtonFilesend();
	afx_msg void OnBnClickedRadioSocket();
	afx_msg void OnBnClickedRadioSerial();
	afx_msg void OnBnClickedButtonOpenPort();
	afx_msg void OnBnClickedButtonClosePort();

	void Save();
	afx_msg void OnBnClickedBtnOk();
	afx_msg void OnBnClickedBtnClose();
};
