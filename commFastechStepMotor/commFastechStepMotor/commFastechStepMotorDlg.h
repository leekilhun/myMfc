
// commFastechStepMotorDlg.h: 헤더 파일
//

#pragma once
#include "jL_serialComPort.h"
#include "qBuffer.h"

#include <mutex> 
#include <thread>

#define CMD_MAX_DATA_LENGTH 24

// CcommFastechStepMotorDlg 대화 상자
class CcommFastechStepMotorDlg : public CDialogEx
{
	enum class cmd_state
	{
		wait_stx,
		wait_page_no,
		wait_type,
		wait_obj_id,
		wiat_length_l,
		wait_length_h,
		wait_data,
		wait_checksum,
		wait_etx,
	};


	cmd_state m_CmdState;
	uint16_t m_Checksum;


	typedef enum
	{
		NEX_PAGE_INIT,
		NEX_PAGE_MAIN,
		NEX_PAGE_MANUAL,
		NEX_PAGE_TEACH,
		NEX_PAGE_IO,
		NEX_PAGE_DATA,
		NEX_PAGE_LOG,
	}nextionpage_t;

	typedef struct
	{
		nextionpage_t   page_no;
		uint8_t         type;
		uint8_t         obj_id;
		uint16_t        length;
		uint32_t        check_sum;
		uint16_t        check_sum_recv;
		uint8_t         buffer[CMD_MAX_DATA_LENGTH + 8];
		uint8_t*				data;
	} nextion_packet_t;

	typedef struct
	{
		uint8_t   ch;
		bool      is_init;
		uint32_t  baud;
		uint8_t   state;
		uint32_t  pre_time;
		uint32_t  index;
		uint8_t   error;
		nextion_packet_t  rx_packet;
	} nextion_t;



	nextion_t m_NextionComm;

	jL_serialComPort* m_rs232Port;
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

	uint16_t m_input_reg;
	uint16_t m_output_reg;


	UINT_PTR	m_TimerID;



// 생성입니다.
public:
	CcommFastechStepMotorDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMMFASTECHSTEPMOTOR_DIALOG };
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
	afx_msg void OnBnClickedBtnConnect();
	afx_msg void OnDestroy();
protected:
	afx_msg LRESULT OnRecivemessage(WPARAM wParam, LPARAM lParam);

private:
	void updateReg(uint16_t reg, bool is_in=false);
	uint16_t getOutReg();
	uint16_t getInReg();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
