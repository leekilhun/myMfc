#pragma once
#include "qBuffer.h"



// tab_test 대화 상자

class tab_test : public CDialogEx
{
	DECLARE_DYNAMIC(tab_test)

	BOOL m_IsPortOpen;

	TQueue m_Qbuf; //  Buffer

	CComboBox m_comboComList;
	CComboBox m_comboBaudrateList;
	CEdit m_editReceive;
	CListBox m_sendList;
	CListBox m_receiveList;

	CButton m_btnOpenClose;
	CButton m_beep;
	CButton m_sensor[8];
	CButton m_led[8];

	CButton m_wsIn[16];
	CButton m_wsOut[16];

	uint16_t m_input_reg;
	uint16_t m_output_reg;


	UINT_PTR	m_TimerID;





public:
	tab_test(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~tab_test();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_TEST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


private:
	void updateReg(uint16_t reg, bool is_in = false);
	uint16_t getOutReg();
	uint16_t getInReg();


public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnConnect();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
};
