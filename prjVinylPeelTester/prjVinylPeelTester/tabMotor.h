#pragma once
#include "popDlg_axisStatus.h"

// tabMotor 대화 상자
struct jL_fastechMotor::motor_state;

class tabMotor : public CDialogEx
{
	DECLARE_DYNAMIC(tabMotor)

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
	CComboBox m_comboComList;
	CComboBox m_comboBaudrateList;
	afx_msg void OnBnClickedBtnConnect();
	CListBox m_recvList;
protected:
	afx_msg LRESULT OnFastechMotorSerialCommMessage(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnLbnDblclkListReceiPack();
	afx_msg void OnBnClickedBtnSendCmd();
	CComboBox m_comboCmdList;
	CEdit m_eboxReceiveData;
	CString m_receiveValue;
	afx_msg void OnBnClickedBtnMotorOnoff();
	afx_msg void OnEnChangeEdId();
	afx_msg void OnBnClickedBtnMotorAlarmReset();
	virtual BOOL DestroyWindow();
};
