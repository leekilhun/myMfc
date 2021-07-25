
// ServerDlg.h: 헤더 파일
//

#pragma once

class SocketServer;
class SerialPort;
class TCPServer;
class UDPUnicastServer;
class UDPBroadcastServer;
class UDPMulticastServer;


// CServerDlg 대화 상자
class CServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERVER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.
private:

	int m_netType;
	int m_baudrate, m_databit, m_stopbit, m_paritybit;
	CString m_strTime;

	SocketServer* m_socketServer;
	SerialPort* m_serialPort;
	TCPServer* m_tcpServer;
	UDPUnicastServer* m_udpUnicastServer;
	UDPBroadcastServer* m_udpBroadcastServer;
	UDPMulticastServer* m_udpMulticastServer;

// 구현입니다.
protected:
	HICON m_hIcon;
	CComboBox* m_combo_protocol, * m_combo_baudrate, * m_combo_databit,
		* m_combo_stopbit, * m_combo_paritybit, * m_combo_portName;


	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	void LoadListBox();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	void SaveLog();
	CListBox logMessage;
	CProgressCtrl fileProgress;
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnNMCustomdrawDownload(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnBnClickedRadioSocket();
	afx_msg void OnBnClickedRadioRs232();
	afx_msg void OnCbnSelchangeComboComport();
	afx_msg void OnCbnSelchangeComboBaudrate();
	afx_msg void OnCbnSelchangeComboDatabit();
	afx_msg void OnCbnSelchangeComboStopbit();
	afx_msg void OnCbnSelchangeComboParitybit();
	afx_msg void OnBnClickedButtonOpen();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnCbnSelchangeComboProtocol();
	afx_msg void OnBnClickedConnectButton();
};
