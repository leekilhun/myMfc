
// chatServerDlg.h: 헤더 파일
//

#pragma once

class Server;

// CchatServerDlg 대화 상자
class CchatServerDlg : public CDialogEx
{
// 생성입니다.
public:
	CchatServerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	~CchatServerDlg();

private:
	Server* m_pServer;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATSERVER_DIALOG };
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
	afx_msg void OnBnClickedBtnStart();
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnClear();

private:
	CIPAddressCtrl m_ip;
	int m_port;
	CListCtrl m_list;
	CListBox m_listMsg;
	CString m_sendMsg;

public:
	void closeServer();
	void initListClient();
	void addListClient(const CString& ip, const CString& port);
	void addListNick(const CString& nick);
	void addListText(const CString& msg);

	LRESULT OnConnectClient(WPARAM wp, LPARAM lp);
	LRESULT OnDisconnectClient(WPARAM wp, LPARAM lp);
	LRESULT OnRecevieNick(WPARAM wp, LPARAM lp);
	LRESULT OnRecevieText(WPARAM wp, LPARAM lp);

};
