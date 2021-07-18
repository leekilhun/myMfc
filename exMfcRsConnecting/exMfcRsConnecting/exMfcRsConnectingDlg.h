
// exMfcRsConnectingDlg.h: 헤더 파일
//

#pragma once
#include "ThreadComm.h"

static int comport_spin_count_before;
static int total_spin_count_before;
static int select_channel;


// CexMfcRsConnectingDlg 대화 상자
class CexMfcRsConnectingDlg : public CDialogEx
{
// 생성입니다.
public:
	CexMfcRsConnectingDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	/*#####################################*/
	// flags 
	BOOL m_bPortInit; // Com Port가 성공적으로 초기화되었는지 표시
	BOOL m_bAnsiStart; // ANSI Escape Character가 왔는지 표시

	// Com Port Variables 
	UINT m_nPort;
	UINT m_nBaudRate;
	CThreadComm m_ComuPort; // 앞에서 만든 통신용 클래스

	//Buffer 
	CString m_strAnsi; // ANSI String이 들어올 경우 입력되는 데이터를 저장 

	CString m_strLine[30]; // Screen에 출력하기 위한 Line Buffer // 본 예제에서는 라인 단위로 출력한다.
	//INT m_id_count = 0;
	CString* id_list;

	// Text Position
	int m_nAnsiCount; // ANSI Escape Character가 들어온 후부터 입력 데이터수
	int m_nLinePos; // 현재 화면에서 출력하고 있는 라인
	int m_nColPos; // 현재 화면에서 출력하고 있는 라인 캐릭터 위치

	// simple Edit Function
	void ClearAll(); // 입력 화면 전체를 지운다.
	void PutChar(unsigned char ch); // m_nLinePos , m_nColPos에 한 글자를 출력한다.

	void DeleteLine(int pos, int type); // 한 줄을 지운다.
	void DelChar(); // 한 글자를 지운다.
	void setting();
	void OnSerialInit();

	void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	CString GetMiddleString(CString str, INT len);
	CString TextFileParsing(CString str, INT len);
	void ProcessErrorMessage(CString msg); // 현재 상태를 상태바에 출력 

	// ANSI Procssing 
	BOOL IsAnsiCommand(unsigned char ch);
	//ANSI Escape Character 이후 입력되는 문자가
	//ANSI Command인가를 검사
	void ProcessAnsi(); // ANSI Command를 처리한다.



	typedef struct MyStruct
	{
		int on_off;
		int pulse_width;
		int gain; // 값의 범위는 -13db ~ 66db인데 음수를 표현하기 힘드므로 0 ~ 79 까지의 값이 들어오면 -13을 해서 값을 맞춰준다 .
		int high_pass_filter; // high_Pass_filter의 경우 범위가 0 ~ 5 이다   0 : 0Mhz ,1 : 1Mhz , 2 : 2.5Mhz , 3 : 5Mhz , 4 : 7.5Mhz , 5 : 12.5Mhz
		int low_pass_filter;	// low_pass_filter의 경우 범위가 0 ~ 5 이다   0 : 50Mhz , 1 : 35Mhz , 2 : 22.5Mhz , 3 : 15Mhz , 4 : 10Mhz , 5 : 5Mhz
		int high_voltage; // 제어 범위 80v ~ 490v (1 ~ 42) 프로토콜 범위 : 0 ~ 42step ( 10v/step) 예 01인데이터가 온경우 80v , 02의 데이터가 온 경우 90v
		int trigger_select; // External : 1 , Internal : 0
		int damping_values_select; // 범위 0 ~ 15 숫자가 높을 수록 pulse의 damping이 작아 집니다.
		int mode_select; // T/R MODE : 1 , Trough Mode : 0
		//int acou_pnr_status; // 먼지 모르지만 설정해주는 값
		int prf_mode_select; // 원하는 모드를 선택 0 : 5KHz, 1 : 3KHz , 2 : 2KHz , 3 : 1KHz
	}ID_DATA;


	/*#####################################*/
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXMFCRSCONNECTING_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	bool m_isConnect;
	int comport_spin_count;
	int total_spin_count;

	bool flag; 
	// apply 적용시켰을 경우 데이터 저장 할때 사용 이유 : 데이터만 변경하고 apply를 안눌었을 경우에 대한 예외를 잡아 주기 위해 사용
	//close버튼을 눌렀을 경우에 모든 데이터를 저장하는데 apply를 적용 안시키면 이전의 데이터가 저장되어 져야함

	CString comport;

	ID_DATA id[8];

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();

	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM IParam);
	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedButton1();
	afx_msg void ReadMessage();
	void OnBnClickedButton2();

	CString m_str_edit_value;
	CString m_edit_value;
	CEdit m_edit_control;
	CMFCButton RUN;
	CString m_str_combo2;
	CString m_str_combo;
	CString channel_data;

	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
	//	afx_msg void OnEnChangeEdit4();
	afx_msg void OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult);


	//afx_msg void RadioCtrl(UINT ID);

	int radio;
	CComboBox serialPort;
	CComboBox m_control_combobox1;
	CComboBox m_str_control_channel;
	CComboBox m_str_control_high_filter;
	CComboBox m_str_control_low_filter;
	CComboBox m_str_control_trigger;
	CSliderCtrl m_control_gain;
	CSliderCtrl m_control_voltage;
	CSliderCtrl m_control_pulse_width;
	CSliderCtrl m_control_damping;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CEdit m_str_comport;
	CEdit m_str_total_channel;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnCbnSelchangeCombo2();
	CButton m_control_radio;
	BOOL m_value_radio;
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnCbnSelchangeCombo3();
	afx_msg void OnCbnSelchangeCombo4();
	afx_msg void OnCbnSelchangeCombo5();

	void getSerialPort();
};
