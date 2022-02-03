#pragma once
#include "popDlg_axisStatus.h"
#include "LedButton.h"


// tabMotor 대화 상자
struct jL_fastechMotor::motor_state;

class tabMotor : public CDialogEx
{
	DECLARE_DYNAMIC(tabMotor)

	timer::_ms m_workTimer;
	CLedButton		m_LedServoOn;
	CToolTipCtrl	m_ToolTip;

	CComboBox m_comboCmdList;
	CEdit m_eboxReceiveData;
	CString m_receiveValue;
	CComboBox m_comboComList;
	CComboBox m_comboBaudrateList;
	CListBox m_recvList;
	CProgressCtrl m_progressRcvData;

	uint8_t m_progressCnt;
public:
	popDlg_axisStatus* m_pDlgPop;
	bool m_IsPortOpen;
	UINT_PTR	m_TimerID;
	jL_fastechMotor* m_pMotor;
	jL_fastechMotor::motor_state* m_cpState;
	
public:
	tabMotor(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~tabMotor();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_MOTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnConnect();
protected:
	afx_msg LRESULT OnFastechMotorSerialCommMessage(WPARAM wParam, LPARAM lParam);

	void updateReg();
public:
	afx_msg void OnLbnDblclkListReceiPack();
	afx_msg void OnBnClickedBtnSendCmd();
	afx_msg void OnBnClickedBtnMotorOnoff();
	afx_msg void OnEnChangeEdId();
	afx_msg void OnBnClickedBtnMotorAlarmReset();
	virtual BOOL DestroyWindow();
	afx_msg void OnBnClickedBtnPopReg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnJogMinus();
	afx_msg void OnBnClickedBtnJogPlus();
	afx_msg void OnBnClickedBtnLimitMinus();
	afx_msg void OnBnClickedBtnLimitPlus();
protected:
	afx_msg LRESULT OnFastechMotorCmdMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnBnClickedBtnMotorStop();
	afx_msg void OnBnClickedBtnMotorEstop2();
	afx_msg void OnBnClickedBtnPosReset();
	afx_msg void OnBnClickedCheckIsrOnoff();
	afx_msg void OnBnClickedBtMcmdMove2();
	afx_msg void OnBnClickedBtMcmdDecMove();
	afx_msg void OnBnClickedBtMcmdIncMove();
	afx_msg void OnBnClickedBtOrmcmdStart();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	afx_msg LRESULT OnFastechMotorRegUpdateMessage(WPARAM wParam, LPARAM lParam);
public:
	CComboBox m_orgMethod;
};
