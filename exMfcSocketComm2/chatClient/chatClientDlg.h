
// chatClientDlg.h: 헤더 파일
//

#pragma once

class Client;

// CchatClientDlg 대화 상자
class CchatClientDlg : public CDialogEx
{
// 생성입니다.
public:
	CchatClientDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATCLIENT_DIALOG };
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
	afx_msg void OnLbnDblclkListRecv();
	afx_msg void OnBnClickedBtnFilesend();
	afx_msg void OnBnClickedBtnSend();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnDisconnect();
	afx_msg void OnBnClickedBtnConnect();

	DECLARE_MESSAGE_MAP()
public:


private:
	void closeClient();
	void addListMsg(const CString& msg, const DATA_TYPE& type = _TEXT);
	void getFileNameAndExt(const CString& str, CString& file_name, CString& file_ext);

	LRESULT OnDisconnectServer(WPARAM wp, LPARAM lp);
	LRESULT OnRecevieText(WPARAM wp, LPARAM lp);
	LRESULT OnRecevieImage(WPARAM wp, LPARAM lp);
	LRESULT OnRecevieFile(WPARAM wp, LPARAM lp);
	LRESULT OnRecevieImageName(WPARAM wp, LPARAM lp);
	LRESULT OnRecevieFileName(WPARAM wp, LPARAM lp);



private:
	CIPAddressCtrl m_ip;
	int m_port;
	CString m_nick;
	CListBox m_listRecv;
	CString m_sendMsg;
	Client* m_pClient;
	CString m_fileName, m_fileExt;
	IMAGE_TYPE m_imgType;
	
};
