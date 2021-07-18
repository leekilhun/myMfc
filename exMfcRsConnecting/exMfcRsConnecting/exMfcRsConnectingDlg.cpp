
// exMfcRsConnectingDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "exMfcRsConnecting.h"
#include "exMfcRsConnectingDlg.h"
#include "afxdialogex.h"

#include "Serial.h"
#include "ThreadComm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
static int char_to_int(char c);
static CString int_to_CString(int x);
static char int_to_char(int x);

static CString IDInitailize(char count, char count1);
static CString PulseOnOff(CHAR ID, CHAR ONOFF);
static CString PulseWidth(CHAR ID, CHAR PULSEWIDTH);
static CString Gain(CHAR ID, int gain_value);
static CString HighLowPassFilter(CHAR ID, CHAR HIGH, CHAR LOW);
static CString HighVoltage(CHAR ID, int Voltage);
static CString TriggerSelect(CHAR ID, CHAR External_Internal);
static CString DamValueSelect(CHAR ID, int Dam);
static CString ThroughModeSelect(CHAR ID, CHAR MODE);
static CString AcouPnrStaus(CHAR ID);
static CString PrfModeSelect(CHAR ID, CHAR MODE);

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	//((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
	CDialogEx::DoDataExchange(pDX);
}

//afx_msg LRESULT OnReceive(WPARAM wParam, LPARAM IParam)
//{
//	AfxMessageBox(wParam);
//}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	//ON_MESSAGE(WM_COMM_READ, OnReceive)
END_MESSAGE_MAP()



// CexMfcRsConnectingDlg 대화 상자



CexMfcRsConnectingDlg::CexMfcRsConnectingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXMFCRSCONNECTING_DIALOG, pParent)
	, m_str_edit_value(_T(""))
	, m_edit_value(_T(""))
	, m_str_combo(_T(""))
	, radio(0)
	, m_value_radio(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nPort = 0;

	// Set Flag 
	m_bPortInit = FALSE;
	m_bAnsiStart = FALSE;

	for (int i = 0; i < 30; i++) m_strLine[i].Empty();

	m_nLinePos = 0;
	m_nColPos = 0;

}

void CexMfcRsConnectingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT2, m_edit_value);
	//DDX_Control(pDX, IDC_EDIT2, m_edit_control);
	DDX_Control(pDX, IDC_BUTTON1, RUN);
	//DDX_Control(pDX, IDC_COMBO2, m_str_combo2);
	DDX_CBString(pDX, IDC_COMBO1, m_str_combo);
	//DDX_Radio(pDX, IDC_RADIO1, radio);
	DDX_Control(pDX, IDC_COMBO1, m_control_combobox1);
	DDX_Control(pDX, IDC_COMBO2, m_str_control_channel);
	DDX_Control(pDX, IDC_COMBO3, m_str_control_high_filter);
	DDX_Control(pDX, IDC_COMBO4, m_str_control_low_filter);
	DDX_Control(pDX, IDC_COMBO5, m_str_control_trigger);
	DDX_Control(pDX, IDC_SLIDER1, m_control_gain);
	DDX_Control(pDX, IDC_SLIDER2, m_control_voltage);
	DDX_Control(pDX, IDC_SLIDER3, m_control_pulse_width);
	DDX_Control(pDX, IDC_SLIDER4, m_control_damping);
	DDX_Control(pDX, IDC_EDIT3, m_str_comport);
	DDX_Control(pDX, IDC_EDIT4, m_str_total_channel);
	//DDX_Control(pDX, IDC_RADIO1, m_control_radio);
	//DDX_Radio(pDX, IDC_RADIO1, m_value_radio);

	DDX_Control(pDX, IDC_COMBO_COM, serialPort);
}




BEGIN_MESSAGE_MAP(CexMfcRsConnectingDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()

	ON_WM_QUERYDRAGICON()
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_MYMESSAGE, &CexMfcRsConnectingDlg::OnReceive)

	
	ON_BN_CLICKED(IDC_BUTTON1, &CexMfcRsConnectingDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CexMfcRsConnectingDlg::OnBnClickedButton2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN3, &CexMfcRsConnectingDlg::OnDeltaposSpin3)
	//	ON_EN_CHANGE(IDC_EDIT4, &CexMfcRsConnectingDlg::OnEnChangeEdit4)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN4, &CexMfcRsConnectingDlg::OnDeltaposSpin4)
	
	ON_BN_CLICKED(IDC_BUTTON4, &CexMfcRsConnectingDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CexMfcRsConnectingDlg::OnBnClickedButton3)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CexMfcRsConnectingDlg::OnCbnSelchangeCombo2)
	//ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO4, RadioCtrl)
	ON_BN_CLICKED(IDC_RADIO1, &CexMfcRsConnectingDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CexMfcRsConnectingDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CexMfcRsConnectingDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CexMfcRsConnectingDlg::OnBnClickedRadio4)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CexMfcRsConnectingDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBO3, &CexMfcRsConnectingDlg::OnCbnSelchangeCombo3)
	ON_CBN_SELCHANGE(IDC_COMBO4, &CexMfcRsConnectingDlg::OnCbnSelchangeCombo4)
	ON_CBN_SELCHANGE(IDC_COMBO5, &CexMfcRsConnectingDlg::OnCbnSelchangeCombo5)
	
END_MESSAGE_MAP()


// CexMfcRsConnectingDlg 메시지 처리기

BOOL CexMfcRsConnectingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	RUN.EnableWindowsTheming(FALSE);
	//m_edit_control::set

	getSerialPort();


	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		//ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// comport 오픈  실패시 종료하는 코드 


	//if (m_ComuPort.OpenPort(_T("COM3"), 9600, 0, AfxGetApp()->m_pMainWnd->m_hWnd))
	//{
	//	TRACE("This is a debug string of text in MFC"); // console에 뿌리기
	//	//GetDlgItem(IDC_BUTTON1)->SetWindowText("RUNNING");

	//	//RUN.SetFaceColor(RGB(170, 255, 1), TRUE);
	//	//RUN.SetTextColor(RGB(212, 5, 0));
	//	AfxMessageBox(_T("성공"));
	//}
	//else
	//{
	//	AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);
	//	AfxMessageBox(_T("Comport 열기 실패"));
	//}

	// 데이터 구조체
	//id = (ID_DATA*)malloc(sizeof(ID_DATA));
		//new ID_DATA[8];

	for (int i = 0; i < 8; i++)
	{
		id[i].on_off = 0;
		id[i].pulse_width = 0;
		id[i].gain = 0;
		id[i].high_pass_filter = 0;
		id[i].low_pass_filter = 0;
		id[i].high_voltage = 1;
		id[i].trigger_select = 0;
		id[i].damping_values_select = 0;
		id[i].mode_select = 1; // echo
		id[i].prf_mode_select = 0;
	}

	//파일 만들기 
	CStdioFile tip_file;

	CFileStatus status;

	// txt파일이 존재 했을 경우에 해당 부분의 코드를 탄다.

	if (CFile::GetStatus(_T("RAIL.txt"), status))
	{


		setting();


		CString str;
		tip_file.Open(_T("RAIL.txt"), CFile::modeRead, NULL);
		int count = 0;
		int index = 0;

		while (tip_file.ReadString(str))
		{
			if (count == 10)
			{
				count = 0; index++;
				if (index == 8)
				{
					index = 8;
					count = 10;
				}
			}

			TRACE(traceAppMsg, 0, str);
			if (count == 0)
			{
				id[index].on_off = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 1)
			{
				id[index].pulse_width = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 2)
			{
				id[index].gain = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 3)
			{
				id[index].high_pass_filter = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 4)
			{
				id[index].low_pass_filter = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 5)
			{
				id[index].high_voltage = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 6)
			{
				id[index].trigger_select = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 7)
			{
				id[index].damping_values_select = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 8)
			{
				id[index].mode_select = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 9)
			{
				id[index].prf_mode_select = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 10)
			{
				comport_spin_count = _ttoi(TextFileParsing(str, str.GetLength()));
				comport_spin_count_before = _ttoi(TextFileParsing(str, str.GetLength()));
			}
			else if (count == 11)
			{
				total_spin_count = _ttoi(TextFileParsing(str, str.GetLength()));
				total_spin_count_before = _ttoi(TextFileParsing(str, str.GetLength()));

			}
			else if (count == 12)
			{
				select_channel = _ttoi(TextFileParsing(str, str.GetLength()));
				break;
			}

			count++;

			//TRACE(str);
		}


		tip_file.Close();
		// 컴포트가 성공적으로 열리면 성공이라는 메시지 박스가 뜨게 된다/.
		if (m_ComuPort.OpenPort(_T("COM" + int_to_CString(comport_spin_count)), 9600, 0, AfxGetApp()->m_pMainWnd->m_hWnd))
		{
			TRACE("This is a debug string of text in MFC"); // console에 뿌리기
			//GetDlgItem(IDC_BUTTON1)->SetWindowText("RUNNING");

			//RUN.SetFaceColor(RGB(170, 255, 1), TRUE);
			//RUN.SetTextColor(RGB(212, 5, 0));
			AfxMessageBox(_T("성공"));
		}
		else // 컴포트를 열지 못했을 경우에 전체 채널과 컴포트를 설정해주는 부분을 제외 하고는 enable 을 false로 만들어서 설정이 불가능 하도록 만들어준다.
		{
			CString str_comport_spin;
			str_comport_spin.Format(_T("%d"), comport_spin_count);
			SetDlgItemText(IDC_EDIT3, str_comport_spin);

			CString str_total_spin_count;
			str_total_spin_count.Format(_T("%d"), total_spin_count);
			SetDlgItemText(IDC_EDIT4, str_total_spin_count);

			AfxMessageBox(_T("Comport 열기 실패"));

			GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO4)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO5)->EnableWindow(FALSE);

			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLIDER1)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLIDER2)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLIDER3)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLIDER4)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO4)->EnableWindow(FALSE);



			return false;
			//AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);

		}

		m_str_control_channel.SetCurSel(select_channel); // 체널도 저장해야됨
		m_str_control_channel.GetLBText(m_str_control_channel.GetCurSel(), channel_data);

		for (int i = 7; i >= total_spin_count; i--)
		{
			m_str_control_channel.DeleteString(i);
		}

		ID_DATA D = id[_ttoi(channel_data) - 1];

		if (D.on_off == 0)
		{
			CString data = PulseOnOff((CHAR)channel_data[0], '0');
			m_ComuPort.WriteComm(data, 9);
			GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("STOP"));
		}
		else
		{
			CString data = PulseOnOff((CHAR)channel_data[0], '1');
			m_ComuPort.WriteComm(data, 9);
			GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("Running"));
		}

		SetDlgItemText(IDC_STATIC_WIDTH, int_to_CString(D.pulse_width));
		m_control_pulse_width.SetPos(D.pulse_width);

		SetDlgItemText(IDC_STATIC_GAIN, int_to_CString(D.gain - 13) + _T(" dB"));
		m_control_gain.SetPos(D.gain);

		int vol = D.high_voltage;

		SetDlgItemText(IDC_STATIC_VOL, int_to_CString((vol * 10) + 70) + _T(" V"));
		m_control_voltage.SetPos(D.high_voltage - 1);

		SetDlgItemText(IDC_STATIC_DAM, int_to_CString(D.damping_values_select));
		m_control_damping.SetPos(D.damping_values_select);

		m_str_control_high_filter.SetCurSel(D.high_pass_filter);
		m_str_control_low_filter.SetCurSel(D.low_pass_filter);
		m_str_control_trigger.SetCurSel(D.trigger_select);
		m_control_combobox1.SetCurSel(D.mode_select);

		if (D.prf_mode_select == 0)
		{
			CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);
			pButton->SetCheck(true);
			CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO2);
			pButton1->SetCheck(false);
			CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO3);
			pButton2->SetCheck(false);
			CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO4);
			pButton3->SetCheck(false);
		}
		else if (D.prf_mode_select == 1)
		{
			CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
			pButton1->SetCheck(false);
			CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
			pButton2->SetCheck(true);
			CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO3);
			pButton3->SetCheck(false);
			CButton* pButton4 = (CButton*)GetDlgItem(IDC_RADIO4);
			pButton4->SetCheck(false);
		}
		else if (D.prf_mode_select == 2)
		{
			CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
			pButton1->SetCheck(false);
			CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
			pButton2->SetCheck(false);
			CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO3);
			pButton3->SetCheck(true);
			CButton* pButton4 = (CButton*)GetDlgItem(IDC_RADIO4);
			pButton4->SetCheck(false);

		}
		else if (D.prf_mode_select == 3)
		{
			CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
			pButton1->SetCheck(false);
			CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
			pButton2->SetCheck(false);
			CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO3);
			pButton3->SetCheck(false);
			CButton* pButton4 = (CButton*)GetDlgItem(IDC_RADIO4);
			pButton4->SetCheck(true);
		}

		CString str_comport_spin;
		str_comport_spin.Format(_T("%d"), comport_spin_count);
		SetDlgItemText(IDC_EDIT3, str_comport_spin);

		CString str_total_spin_count;
		str_total_spin_count.Format(_T("%d"), total_spin_count);
		SetDlgItemText(IDC_EDIT4, str_total_spin_count);

		m_str_control_channel.GetLBText(m_str_control_channel.GetCurSel(), channel_data);

		CString data = IDInitailize('0', '1');
		m_ComuPort.WriteComm(data, 9);

	}// txt파일을 처음 실행 시켜 줬으므로 파일을 만들어준다/.
	else if (tip_file.Open(_T("RAIL.txt"), CFile::modeCreate | CFile::modeWrite | CFile::typeText))
	{

		CString str;
		CString writeStr;

		for (int i = 0; i < 8; i++)
		{
			str.Format(_T("%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d"), "id", i + 1, " : ", id[i].on_off,
				"\r\npulse_width", i + 1, " : ", id[i].pulse_width,
				"\r\ngain", i + 1, " : ", id[i].gain,
				"\r\nhigh_pass_filter", i + 1, " : ", id[i].high_pass_filter,
				"\r\nlow_pass_filter", i + 1, " : ", id[i].low_pass_filter,
				"\r\nhigh_voltage", i + 1, " : ", id[i].high_voltage,
				"\r\ntrigger_select", i + 1, " : ", id[i].trigger_select,
				"\r\ndamping_values_select ", i + 1, " : ", id[i].damping_values_select,
				"\r\nmode_select ", i + 1, " : ", id[i].mode_select,
				"\r\nprf_mode_select ", i + 1, " : ", id[i].prf_mode_select);
			writeStr = writeStr + str + _T("\r\n");
		}

		writeStr = writeStr + _T("COMPORT : 3\r\n");
		writeStr = writeStr + _T("Total channel : 8\r\n");
		writeStr = writeStr + _T("select channel : 0");
		tip_file.WriteString(writeStr);

		// 성공적으로 파일이 열렸으면 원하는 작업을 한다.

		// 작업을 마친후에 파일을 닫는다.
		tip_file.Close();

		if (m_ComuPort.OpenPort(_T("COM3"), 9600, 0, AfxGetApp()->m_pMainWnd->m_hWnd)) // 파일을 저장 시킨후에 컴포트를 열어서 연결 시키는 코드
		{
			TRACE("This is a debug string of text in MFC"); // console에 뿌리기
			//GetDlgItem(IDC_BUTTON1)->SetWindowText("RUNNING");

			//RUN.SetFaceColor(RGB(170, 255, 1), TRUE);
			//RUN.SetTextColor(RGB(212, 5, 0));
			AfxMessageBox(_T("성공"));
		}
		else
		{
			AfxMessageBox(_T("Comport 열기 실패"));

			GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO4)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO5)->EnableWindow(FALSE);
			GetDlgItem(IDC_BUTTON1)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLIDER1)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLIDER2)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLIDER3)->EnableWindow(FALSE);
			GetDlgItem(IDC_SLIDER4)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO1)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO3)->EnableWindow(FALSE);
			GetDlgItem(IDC_RADIO4)->EnableWindow(FALSE);

			return false;
			//AfxGetMainWnd()->PostMessage(WM_COMMAND, ID_APP_EXIT, 0);

		}

		//((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(1);
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);
		pButton->SetCheck(true);

		setting();
		m_control_combobox1.SetCurSel(1);
		m_str_control_channel.SetCurSel(0);
		m_str_control_high_filter.SetCurSel(0);
		m_str_control_low_filter.SetCurSel(0);
		m_str_control_trigger.SetCurSel(0);

		m_str_control_channel.GetLBText(m_str_control_channel.GetCurSel(), channel_data);

		SetDlgItemText(IDC_STATIC_GAIN, _T("-13 dB"));
		SetDlgItemText(IDC_STATIC_VOL, _T("80 V"));
		SetDlgItemText(IDC_STATIC_WIDTH, _T("0"));
		SetDlgItemText(IDC_STATIC_DAM, _T("0"));

		comport_spin_count = 3;
		CString str_comport_spin_count1;
		str_comport_spin_count1.Format(_T("%d"), comport_spin_count);
		SetDlgItemText(IDC_EDIT3, str_comport_spin_count1);


		total_spin_count = 8;
		CString str1_total_spin_count1;
		str1_total_spin_count1.Format(_T("%d"), total_spin_count);
		SetDlgItemText(IDC_EDIT4, str1_total_spin_count1);

		m_str_control_channel.GetLBText(m_str_control_channel.GetCurSel(), channel_data);

		comport_spin_count_before = 3;
		total_spin_count_before = 8;
		select_channel = 0;

		CString data = IDInitailize('0', '1');
		m_ComuPort.WriteComm(data, 9);

	}



	// 기기 초기화 
	//CString data = IDInitailize('0', '1');
	//m_ComuPort.WriteComm(data, 9);

	// 변수 초기화 















	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}




void CexMfcRsConnectingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_CLOSE)
	{

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}




// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CexMfcRsConnectingDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CexMfcRsConnectingDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CexMfcRsConnectingDlg::ProcessErrorMessage(CString msg)
{
	AfxMessageBox(_T("에러 메시지"));
}



void CexMfcRsConnectingDlg::OnBnClickedButton1()
{
	int id_index = _ttoi(channel_data) - 1;
	if (id[id_index].on_off == 0)
	{
		CString data = PulseOnOff((CHAR)channel_data[0], '1');
		m_ComuPort.WriteComm(data, 9);
		id[id_index].on_off = 1;
		GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("Running"));

		//RUN.SetFaceColor(RGB(170, 255, 1), true);
		//RUN.SetFaceColor(RGB(212, 5, 0));

	}
	else
	{
		CString data = PulseOnOff((CHAR)channel_data[0], '0');
		m_ComuPort.WriteComm(data, 9);
		id[id_index].on_off = 0;

		GetDlgItem(IDC_BUTTON1)->SetWindowText(_T("STOP"));

		//RUN.SetFaceColor(RGB(255, 255, 255), true);
		//RUN.SetFaceColor(RGB(212, 5, 0));
	}



	//if (m_ComuPort.OpenPort(comport, 9600, 0, AfxGetApp()->m_pMainWnd->m_hWnd))
	//{
	//	TRACE("This is a debug string of text in MFC"); // console에 뿌리기
	//	GetDlgItem(IDC_BUTTON1)->SetWindowText("RUNNING");

	//	RUN.SetFaceColor(RGB(170, 255, 1), TRUE);
	//	RUN.SetTextColor(RGB(212, 5, 0));


	//	AfxMessageBox(_T("성공"));
	//}
	//else
	//{
	//	AfxMessageBox(_T("실패"));
	//}


// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

void CexMfcRsConnectingDlg::ClearAll()
{
	CDC* pDC;
	CRect rect;
	CBrush brush;

	pDC = GetDC();
	GetClientRect(rect);

	brush.CreateSolidBrush(RGB(255, 255, 255));

	pDC->FillRect(&rect, &brush);

	Invalidate(TRUE);

	for (int i = 0; i < 30; i++)
		m_strLine[i].Empty();
}

LRESULT CexMfcRsConnectingDlg::OnReceive(WPARAM wParam, LPARAM IParam) // serial 통신을 해서 데이터를 받았을 때 처리해주는 부분 
{
	BYTE buffer;
	CString message;
	int length = -1;

	length = this->m_ComuPort.m_QueueRead.GetSize();

	for (int i = 0; i < length; i++)
	{
		this->m_ComuPort.m_QueueRead.GetByte(&buffer);
		message += buffer;
	}

	//TRACE(traceAppMsg, 0, "데이터를 출력합니다.\r\n");
	if (message == "") {}
	else
	{
		message = GetMiddleString(message, message.GetLength());

		if (message[0] == 'A' && message[1] == 'O' && message[2] == 'K')
		{

			AfxMessageBox(_T("연결 응답 성공"));
			/*	CString data =  IDInitailize('0', '1');
				m_ComuPort.WriteComm(data,9);
				return 0;

	*/

	// 아이디를 부여해야 함
	//if(message[3]== '0' && message[2] == 'M')
	//{ 

	//	/*id = new ID_DATA * [char_to_int(message[5])];

	//	for (int i = 0; i < 2; i++)
	//	{
	//		id[i] = new ID_DATA[8];
	//	}*/

	//	CHAR stx = 0x02;
	//	CHAR etx = 0x03;
	//	m_id_count = char_to_int(message[6]);
	//	//TRACE(traceAppMsg, 0, (CString)message[5]+"\r\n");
	//	//CString SEND = stx + "AOK" + etx;
	//	//id_list = new CString[m_id_count];
	//	CString SEND = "AOK";
	//	m_ComuPort.WriteComm(SEND,3);
	//}
	//else if (message[3] == 'P') // Pulse On/Off 하는 곳 // 4번째 bit : ID // 7번째 비트 bit : ON ='1' OFF = '0'
	//{
	//	if (message[5] == '1')
	//	{
	//		TRACE(traceAppMsg, 0, "ON \r\n");
	//		id[char_to_int(message[0])][0].on_off = 1;
	//	}
	//	else
	//	{
	//		TRACE(traceAppMsg, 0, "OFF \r\n");
	//		id[0][0].on_off = 0;
	//	}
	//}
	//else if (message[3] == 'T')
	//{
	//	id[0][0].pulse_width = char_to_int(message[6]);
	//	TRACE(traceAppMsg, 0, (CString)message[6] + "\r\n");
	//}
	//else if (message[3] == 'G')
	//{
	//	id[0][0].gain = char_to_int(message[4]) * 10 + char_to_int(message[5]) - 13;

	//	CString str;
	//	str.Format(_T("%d"), char_to_int(message[4]) * 10 + char_to_int(message[5]) - 13);
	//	//SetDlgItemText(IDC_EDIT3, str);
	//	TRACE(traceAppMsg, 0, str + "\r\n");
	//}
	//else if (message[3] == 'F')
	//{
	//	float number;

	//	switch (message[4])
	//	{
	//		case '0': id[0][0].high_pass_filter = 0; break;
	//		case '1' : id[0][0].high_pass_filter = 1; break;
	//		case '2': id[0][0].high_pass_filter = 2.5; break;
	//		case '3': id[0][0].high_pass_filter = 5; break;
	//		case '4': id[0][0].high_pass_filter = 7.5; break;
	//		case '5': id[0][0].high_pass_filter = 12.5; break;
	//	default:
	//		break;
	//	}

	//	switch (message[5])
	//	{
	//	case '0': id[0][0].low_pass_filter = 50; break;
	//	case '1': id[0][0].low_pass_filter = 35; break;
	//	case '2': id[0][0].low_pass_filter = 22.5; break;
	//	case '3': id[0][0].low_pass_filter = 15; break;
	//	case '4': id[0][0].low_pass_filter = 10; break;
	//	case '5': id[0][0].low_pass_filter = 5; break;

	//	default:
	//		break;
	//	}
	//}
	//else if (message[3]=='H')
	//{
	//	id[0][0].high_voltage = ((char_to_int( message[4]) * 10) + (char_to_int(message[5]) - 1)) * 10 + 80;   // 0일때 80 1일때 90 값을 맞춰주기 위한 연산과정 
	//}
	//else if (message[3] == 'E')
	//{
	//	if (message[5] == '0')
	//	{
	//		id[0][0].trigger_select = false;
	//	}
	//	else
	//	{
	//		id[0][0].trigger_select = true;
	//	}
	//}
	//else if (message[3] == 'D')
	//{
	//	id[0][0].damping_values_select = char_to_int(message[4]) * 10 + char_to_int(message[5]);
	//}
	//else if (message[3] == 'M')
	//{
	//	id[0][0].mode_select = message[5] == '1' ? true : false;
	//}
	//else if (message[3] == 'S' && message[4]=='T' && message[5] == 'A')
	//{
	//id[0][0].acou_pnr_status = true;
	//}
		}
		else
		{
			AfxMessageBox(_T("연결 응답 실패"));
		}
		//GetDlgItem(IDC_EDIT2)->SetWindowText(message);	 
	}
	return 0;
}

// type 변겨을 할때 사용되는 함수

CString int_to_CString(int x)
{
	CString str;
	str.Format(_T("%d"), x);
	return str;
}

int char_to_int(char c)
{
	int x = (int)c;
	return x - 48;
}

char int_to_char(int x)
{
	CString str;
	str.Format(_T("%d"), x);
	return (CHAR)str[0];
}

// 프로토콜을 보낼때 해당 부분들을 함수화 시켜둔 부분 

CString IDInitailize(CHAR count, CHAR count1)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	str1.Format(_T("%c%s%c%c%c%c"), stx, _T("RAM0"), count, count1, 'Z', etx);
	//str1 = "RAM0";
	return str1;
}

CString PulseOnOff(CHAR ID, CHAR ONOFF)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	str1.Format(_T("%c%s%c%s%c%c%c"), stx, _T("RA"), ID, _T("PU"), ONOFF, 'X', etx);
	return str1;
}

CString PulseWidth(CHAR ID, CHAR PULSEWIDTH)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	str1.Format(_T("%c%s%c%s%c%c%c"), stx, _T("RA"), ID, _T("T0"), PULSEWIDTH, 'X', etx);
	return str1;
}

CString Gain(CHAR ID, int gain_value)
{

	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	int x1 = (char)(gain_value / 10 + 48);
	int x2 = (char)(gain_value % 10 + 48);

	str1.Format(_T("%c%s%c%c%c%c%c%c"), stx, _T("RA"), ID, 'G', x1, x2, 'Z', etx);
	return str1;
}

CString HighLowPassFilter(CHAR ID, CHAR HIGH, CHAR LOW)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	str1.Format(_T("%c%s%c%c%c%c%c%c"), stx, _T("RA"), ID, 'F', HIGH, LOW, 'Q', etx);
	return str1;
}

CString HighVoltage(CHAR ID, int Voltage)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	int x1 = (char)(Voltage / 10 + 48);
	int x2 = (char)(Voltage % 10 + 48);

	str1.Format(_T("%c%s%c%c%c%c%c%c"), stx, _T("RA"), ID, 'H', x1, x2, 'P', etx);
	return str1;
}

CString TriggerSelect(CHAR ID, CHAR External_Internal)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	str1.Format(_T("%c%s%c%s%c%c%c"), stx, _T("RA"), ID, _T("E0"), External_Internal, 'Z', etx);
	return str1;
}

CString DamValueSelect(CHAR ID, int Dam)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	int x1 = (char)(Dam / 10 + 48);
	int x2 = (char)(Dam % 10 + 48);

	str1.Format(_T("%c%s%c%c%c%c%c%c"), stx, _T("RA"), ID, 'D', x1, x2, 'Q', etx);
	return str1;

}

CString ThroughModeSelect(CHAR ID, CHAR MODE)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	str1.Format(_T("%c%s%c%s%c%c%c"), stx, _T("RA"), ID, _T("M0"), MODE, 'R', etx);
	return str1;
}

CString AcouPnrStaus(CHAR ID)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	str1.Format(_T("%c%s%c%s%c"), stx, _T("RA"), ID, _T("STAT"), etx);
	return str1;
}

CString PrfModeSelect(CHAR ID, CHAR MODE)
{
	CString str1;
	CHAR stx = 0x02;
	CHAR etx = 0x03;

	str1.Format(_T("%c%s%c%s%c%c%c"), stx, _T("RA"), ID, _T("M0"), MODE, 'R', etx);
	return str1;
}


CString ConvertMultibyteToUnicode(char* pMultibyte)
{
	int nLen = strlen(pMultibyte);

	WCHAR* pWideChar = new WCHAR[nLen];
	memset(pWideChar, 0x00, (nLen) * sizeof(WCHAR));

	MultiByteToWideChar(CP_ACP, 0, (LPCSTR)pMultibyte, -1, pWideChar, nLen);

	CString strUnicode;
	strUnicode.Format(_T("%s"), pWideChar);

	delete[] pWideChar;

	return strUnicode;
}



void CexMfcRsConnectingDlg::PutChar(unsigned char ch)
{
	CDC* pDC;
	TEXTMETRIC tm;

	pDC = GetDC();
	pDC->GetTextMetrics(&tm);

	if (ch == 10) //LF 문자일 경우
	{
		m_nLinePos++;
		if (m_nLinePos > 25) // 화면 크기가 25 라인을 넘으면 화면 전체를 지우고 처음부터 다시 쓴다.
		{
			m_nLinePos = 0;
			m_nColPos = 0;
			ClearAll();
		}
	}
	else if (ch == 13) // CR
	{
		m_nColPos = 0;
	}
	else
	{
		// 현 커서 위치에 글자를 쓴다.

		m_strLine[m_nLinePos].Insert(m_nColPos, ch);

		pDC->TextOut(0, m_nLinePos * tm.tmHeight, m_strLine[m_nLinePos]);
		m_nColPos++;
	}
}

// 한 줄을 지운다. TYPE이 0이면 뒤쪽을 , TYPE이 1이면 한 줄 전체를 지운다.

void CexMfcRsConnectingDlg::DeleteLine(int pos, int type)
{
	CDC* pDC;
	TEXTMETRIC tm;
	CRect rect;
	CBrush brush;

	pDC = GetDC();
	pDC->GetTextMetrics(&tm);
	GetClientRect(rect);
	brush.CreateSolidBrush(RGB(255, 255, 255));

	if (type == 0)
	{
		rect.top = m_nLinePos * tm.tmHeight;
		rect.bottom = (m_nLinePos + 1) * tm.tmHeight;
	}
	else
	{
		rect.left = m_nColPos * tm.tmHeight;
		rect.top = m_nLinePos * tm.tmHeight;
		rect.bottom = m_nLinePos * (tm.tmHeight + 1);
	}

	pDC->FillRect(&rect, &brush);
	Invalidate(TRUE);

	m_strLine[m_nLinePos].Empty();
	m_nColPos = 0;
	m_nLinePos--;
}

CString CexMfcRsConnectingDlg::TextFileParsing(CString str, INT len)
{
	if (str.IsEmpty())
	{
		return _T("");
	}

	CString result = L"";
	INT _nStx = str.Find(':', 0);
	INT _nEtx = len;

	result = str.Mid(_nStx + 1, (len - _nStx - 1));

	return result;
}


// 가지고 온 데이터를 stx ,etx를 자르는 코드 
CString  CexMfcRsConnectingDlg::GetMiddleString(CString str, INT len)
{
	//Thread.Sleep(50);


	if (str.IsEmpty())
	{

		return L"";
	}

	CString result = L"";
	INT _nStx = str.Find(0x02, 0);
	INT _nEtx = str.Find(0x03, 0);

	result = str.Mid(_nStx + 1, (_nEtx - _nStx - 1));
	return result;
}

void CexMfcRsConnectingDlg::OnSerialInit()
{
	if (m_nPort == 0)
	{
		ProcessErrorMessage(_T("포트를 세팅해주십시오."));
		return;
	}

	CString strTemp;
	if (m_nPort > 0)
		strTemp.Format(m_nPort >= 10 ? _T("\\\\.\\COM%d") : _T("COM%d"), m_nPort);
	else
		strTemp.Empty();

	if (!m_ComuPort.OpenPort(strTemp, m_nBaudRate, m_nPort, AfxGetApp()->m_pMainWnd->m_hWnd))
	{
		ProcessErrorMessage(_T("포트 초기화 실패."));
	}
	else
	{
		CString strTemp;
		strTemp.Format(_T("포트 COM%d 초기화 성공"), m_nPort);
		ProcessErrorMessage(strTemp);

		m_bPortInit = TRUE;
	}
}

void CexMfcRsConnectingDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CString strTemp;

	if (!m_bPortInit)
	{
		ProcessErrorMessage(_T("포트가 열리지 않았습니다."));
		return;
	}
	else
	{
		if (nChar == 10) // Linefedd-Carridge return 이 입력될 경우는 CR만 처리
		{
			strTemp.Format(_T("%c"), 13);
			m_ComuPort.WriteComm((LPCTSTR)strTemp, strTemp.GetLength());
		}
		else
		{
			strTemp.Format(_T("%c"), nChar);
			m_ComuPort.WriteComm((LPCTSTR)strTemp, strTemp.GetLength());

		}
	}
}


BOOL CexMfcRsConnectingDlg::IsAnsiCommand(unsigned char ch)
{

	// ANSI Command는 길이가 11byte 이하이고 다음의 charater로 끝난다.
	unsigned char AnsiCommand[6] = {'m','k','h','i','j','k'};

	for (int i = 0; i < 6; i++)
		if (ch == AnsiCommand[i])	return TRUE;
	return FALSE;
}

//LRESULT SendMessage(HWND hWnd, // handle of destination window
//	UINT Msg, // message to send
//	WPARAM wParam, // first message parameter
//	LPARAM lParam)
//{
//
//}



// totalchannel과 comport 설정의 경우 apply 버튼을 눌렀을 경우에만 변경 되도록 설정되어 있음
// before가 붙여진 변수가 그 역할을 함
// total channel을 변경할 경우 오른 쪽 상단의 channel 리스트가 변경되야 함 
// 전체 list를 지웠다가 다시 그리는 방식으로 구현 
// total channel이 5였을 경우 이전 선택되어져 있는 channel이 7인 경우 선택 한 channel이 5가 되로록 예외 처리를 해둠

void CexMfcRsConnectingDlg::OnBnClickedButton2()
{
	flag = true;

	CString str;
	m_str_comport.GetWindowText(str);
	CString str1;
	m_str_total_channel.GetWindowText(str1);

	comport_spin_count_before = _ttoi(str);
	total_spin_count_before = _ttoi(str1);

	//m_str_control_channel.SetCurSel(select_channel); // 체널도 저장해야됨

	if (m_ComuPort.m_bConnected == false)
	{

	}
	else
	{
		m_str_control_channel.GetLBText(m_str_control_channel.GetCurSel(), channel_data);

		int channel_selected = m_str_control_channel.GetCurSel();

		for (int i = 7; i >= 0; i--)
		{
			m_str_control_channel.DeleteString(i);
		}

		for (int i = 0; i < total_spin_count; i++)
		{
			m_str_control_channel.AddString(int_to_CString(i + 1));
		}


		if (channel_selected > total_spin_count - 1)
		{
			m_str_control_channel.SetCurSel(total_spin_count - 1);
			select_channel = total_spin_count - 1;
		}
		else
		{
			m_str_control_channel.SetCurSel(channel_selected);
			select_channel = channel_selected;
		}
	}
	//m_str_comport.GetWindowTextA(comport);
	//comport = "COM" + comport;

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
//

// 상승 버튼 눌었을 경우 1이 증가 감소 버튼을 눌렀을 경우 1이 감소 
// 최소 값 1 최대값 8로 설정 되어져 있음
void CexMfcRsConnectingDlg::OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (pNMUpDown->iDelta -= 1)
	{
		comport_spin_count++;
		if (comport_spin_count == 9)
		{
			comport_spin_count = 8;
		}
		CString str;
		str.Format(_T("%d"), comport_spin_count);
		SetDlgItemText(IDC_EDIT3, str);
		//GetDlgItem(IDC_EDIT3)->SetWindowText((LPCTSTR)"dsdsdasdsad");
	}
	else
	{
		comport_spin_count--;
		if (comport_spin_count == 0)
		{
			comport_spin_count = 1;
			return;
		}
		CString str;
		str.Format(_T("%d"), comport_spin_count);
		SetDlgItemText(IDC_EDIT3, str);

		//GetDlgItem(IDC_EDIT3)->SetWindowText((LPCTSTR)comport_spin_count);
	}
	*pResult = 0;
}


//void CMfcSerialrailDlg::OnEnChangeEdit4()
//{
//	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
//	// CDialogEx::OnInitDialog() 함수를 재지정 
//	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
//	// 이 알림 메시지를 보내지 않습니다.
//
//	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
//}

// 상승 버튼 눌었을 경우 1이 증가 감소 버튼을 눌렀을 경우 1이 감소 
// 최소 값 1 최대값 8로 설정 되어져 있음
void CexMfcRsConnectingDlg::OnDeltaposSpin4(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);

	if (pNMUpDown->iDelta -= 1)
	{
		total_spin_count++;
		if (total_spin_count == 9)
		{
			total_spin_count = 8;
		}
		CString str;
		str.Format(_T("%d"), total_spin_count);
		SetDlgItemText(IDC_EDIT4, str);
		//GetDlgItem(IDC_EDIT3)->SetWindowText((LPCTSTR)"dsdsdasdsad");
	}
	else
	{
		total_spin_count--;
		if (total_spin_count == 0)
		{
			total_spin_count = 1;
			return;
		}

		CString str;
		str.Format(_T("%d"), total_spin_count);
		SetDlgItemText(IDC_EDIT4, str);

		//GetDlgItem(IDC_EDIT3)->SetWindowText((LPCTSTR)comport_spin_count);
	}


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

// slider 의 값들을 scroll이 변경 되었을 경우에 데이터를 변경하고 serial 통신으로 send 시키는 역할
// 버튼을 때었을 때 데이터 보내는 방식은 구현 안하고 변경 될때마다 보내는 방식으로 구현했음
// 스크롤이 변경 될때 마다 9바이트의 데이터가 보내지는데 별로 상관 없을 듯 해서 그냥 구현함

void CexMfcRsConnectingDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if (pScrollBar)
	{
		int id_index = _ttoi(channel_data) - 1;
		// 어떤 슬라이더인지 검사
		if (pScrollBar == (CScrollBar*)&m_control_gain)
		{
			// 슬라이더의 위치를 검사한다.
			int nPos_g = m_control_gain.GetPos();
			CString data = Gain((CHAR)channel_data[0], nPos_g);
			m_ComuPort.WriteComm(data, 9);
			id[id_index].gain = nPos_g;
			SetDlgItemText(IDC_STATIC_GAIN, int_to_CString(nPos_g - 13) + " dB");

		}
		if (pScrollBar == (CScrollBar*)&m_control_voltage)
		{
			int nPos_v = m_control_voltage.GetPos();
			CString data = HighVoltage((CHAR)channel_data[0], nPos_v + 1);
			m_ComuPort.WriteComm(data, 9);
			id[id_index].high_voltage = nPos_v + 1;
			SetDlgItemText(IDC_STATIC_VOL, int_to_CString(((nPos_v + 1) * 10) + 70) + " V");
		}
		if (pScrollBar == (CScrollBar*)&m_control_pulse_width)
		{
			int nPos_p = m_control_pulse_width.GetPos();
			char x1 = (char)(m_control_pulse_width.GetPos() + 48);
			CString data = PulseWidth((CHAR)channel_data[0], x1);
			m_ComuPort.WriteComm(data, 9);
			id[id_index].pulse_width = nPos_p;
			SetDlgItemText(IDC_STATIC_WIDTH, int_to_CString(nPos_p));

		}
		if (pScrollBar == (CScrollBar*)&m_control_damping)
		{
			int nPos_d = m_control_damping.GetPos();
			CString data = DamValueSelect((CHAR)channel_data[0], nPos_d);
			m_ComuPort.WriteComm(data, 9);
			id[id_index].damping_values_select = nPos_d;
			SetDlgItemText(IDC_STATIC_DAM, int_to_CString(nPos_d));
		}
	}
	else
	{
		// CScrollView를 상속받은 뷰의 경우 프래임의 스크롤롤 동작시 pScrollBar이 NULL된다.
	}



	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

// close 버튼을 눌렀을 경우에 설정되어 있는 데이터들이 txt파일로 저장됨

void CexMfcRsConnectingDlg::OnBnClickedButton4()
{

	CStdioFile tip_file;

	if (tip_file.Open(L"RAIL.txt", CFile::modeCreate | CFile::modeWrite | CFile::typeText) && (id[0].on_off == 1 || id[0].on_off == 0))
	{



		CString str;
		CString writeStr;

		for (int i = 0; i < 8; i++)
		{
			str.Format(_T("%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d"), L"id", i + 1, " : ", id[i].on_off,
				"\r\npulse_width", i + 1, " : ", id[i].pulse_width,
				"\r\ngain", i + 1, " : ", id[i].gain,
				"\r\nhigh_pass_filter", i + 1, " : ", id[i].high_pass_filter,
				"\r\nlow_pass_filter", i + 1, " : ", id[i].low_pass_filter,
				"\r\nhigh_voltage", i + 1, " : ", id[i].high_voltage,
				"\r\ntrigger_select", i + 1, " : ", id[i].trigger_select,
				"\r\ndamping_values_select ", i + 1, " : ", id[i].damping_values_select,
				"\r\nmode_select ", i + 1, " : ", id[i].mode_select,
				"\r\nprf_mode_select ", i + 1, " : ", id[i].prf_mode_select);
			writeStr = writeStr + str + L"\r\n";
		}
		writeStr = writeStr + L"COMPORT : " + int_to_CString(comport_spin_count_before);
		writeStr = writeStr + L"\r\nTotal channel : " + int_to_CString(total_spin_count_before);
		writeStr = writeStr + L"\r\nselect channel : " + int_to_CString(select_channel);
		tip_file.WriteString(writeStr);

		// 성공적으로 파일이 열렸으면 원하는 작업을 한다.
		// 작업을 마친후에 파일을 닫는다.
		tip_file.Close();
	}

	if (m_ComuPort.m_bConnected)
	{
		m_ComuPort.ClosePort();
	}
	//delete[]id;
	AfxGetMainWnd()->SendMessage(WM_COMMAND, ID_APP_EXIT, NULL);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// refresh 버튼의 경우 어떤 역할을 하는 지 모르겠어서 그냥 버튼만 만들어 두었음

void CexMfcRsConnectingDlg::OnBnClickedButton3()
{
	/*CString data = IDInitailize('0', '1');
	m_ComuPort.WriteComm(data, 9);


	data = PulseOnOff(channel_data[0], '1');
	m_ComuPort.WriteComm(data, 9);

	char x1 = (char)(m_control_pulse_width.GetPos() + 48);

	data = PulseWidth('1', x1);
	m_ComuPort.WriteComm(data, 9);

	data = Gain('1', m_control_gain.GetPos());
	m_ComuPort.WriteComm(data, 9);


	char high = (char)(m_str_control_high_filter.GetCurSel() + 48);
	char low = (char)(m_str_control_low_filter.GetCurSel() + 48);

	data = HighLowPassFilter('1', high, low);
	m_ComuPort.WriteComm(data, 9);


	data = HighVoltage('1', m_control_voltage.GetPos() + 1);
	m_ComuPort.WriteComm(data, 9);

	(char)(m_str_control_trigger.GetCurSel() + 48);

	data = TriggerSelect('1', '0');
	m_ComuPort.WriteComm(data, 9);

	data = DamValueSelect('1', 4);
	m_ComuPort.WriteComm(data, 9);

	data = ThroughModeSelect('1', '0');
	m_ComuPort.WriteComm(data, 9);

	data = AcouPnrStaus('1');
	m_ComuPort.WriteComm(data, 9);


	data = PrfModeSelect('1', '0');
	m_ComuPort.WriteComm(data, 9);
*/
// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// channel을 변경 하였을 경우 해당 데이터들이 보여 줘야 하므로 
// channel에 맞는 데이터들을 세팅해주는 부분
void CexMfcRsConnectingDlg::OnCbnSelchangeCombo2()
{

	m_str_control_channel.GetLBText(m_str_control_channel.GetCurSel(), channel_data);
	select_channel = m_str_control_channel.GetCurSel();
	ID_DATA D = id[_ttoi(channel_data) - 1];

	if (D.on_off == 0)
	{
		CString data = PulseOnOff((CHAR)channel_data[0], '0');
		m_ComuPort.WriteComm(data, 9);
		GetDlgItem(IDC_BUTTON1)->SetWindowText(L"STOP");
	}
	else
	{
		CString data = PulseOnOff((CHAR)channel_data[0], '1');
		m_ComuPort.WriteComm(data, 9);
		GetDlgItem(IDC_BUTTON1)->SetWindowText(L"Running");
	}

	SetDlgItemText(IDC_STATIC_WIDTH, int_to_CString(D.pulse_width));
	m_control_pulse_width.SetPos(D.pulse_width);

	SetDlgItemText(IDC_STATIC_GAIN, int_to_CString(D.gain - 13) + L" dB");
	m_control_gain.SetPos(D.gain);

	int vol = D.high_voltage;

	SetDlgItemText(IDC_STATIC_VOL, int_to_CString((vol * 10) + 70) + L" V");
	m_control_voltage.SetPos(D.high_voltage - 1);

	SetDlgItemText(IDC_STATIC_DAM, int_to_CString(D.damping_values_select));
	m_control_damping.SetPos(D.damping_values_select);

	m_str_control_high_filter.SetCurSel(D.high_pass_filter);
	m_str_control_low_filter.SetCurSel(D.low_pass_filter);
	m_str_control_trigger.SetCurSel(D.trigger_select);
	m_control_combobox1.SetCurSel(D.mode_select);

	if (D.prf_mode_select == 0)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO1);
		pButton->SetCheck(true);
		CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton1->SetCheck(false);
		CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO3);
		pButton2->SetCheck(false);
		CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO4);
		pButton3->SetCheck(false);
	}
	else if (D.prf_mode_select == 1)
	{
		CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
		pButton1->SetCheck(false);
		CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton2->SetCheck(true);
		CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO3);
		pButton3->SetCheck(false);
		CButton* pButton4 = (CButton*)GetDlgItem(IDC_RADIO4);
		pButton4->SetCheck(false);
	}
	else if (D.prf_mode_select == 2)
	{
		CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
		pButton1->SetCheck(false);
		CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton2->SetCheck(false);
		CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO3);
		pButton3->SetCheck(true);
		CButton* pButton4 = (CButton*)GetDlgItem(IDC_RADIO4);
		pButton4->SetCheck(false);

	}
	else if (D.prf_mode_select == 3)
	{
		CButton* pButton1 = (CButton*)GetDlgItem(IDC_RADIO1);
		pButton1->SetCheck(false);
		CButton* pButton2 = (CButton*)GetDlgItem(IDC_RADIO2);
		pButton2->SetCheck(false);
		CButton* pButton3 = (CButton*)GetDlgItem(IDC_RADIO3);
		pButton3->SetCheck(false);
		CButton* pButton4 = (CButton*)GetDlgItem(IDC_RADIO4);
		pButton4->SetCheck(true);
	}



	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

// 레디오 버튼이 눌렸을 경우에 데이터가 보내지도록 4개의 버튼을 만듬


void CexMfcRsConnectingDlg::OnBnClickedRadio1()
{
	int id_index = _ttoi(channel_data) - 1;
	id[id_index].prf_mode_select = 0;
	CString data = PrfModeSelect((CHAR)channel_data[0], int_to_char(0));
	m_ComuPort.WriteComm(data, 9);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMfcRsConnectingDlg::OnBnClickedRadio2()
{
	int id_index = _ttoi(channel_data) - 1;
	id[id_index].prf_mode_select = 1;
	CString data = PrfModeSelect((CHAR)channel_data[0], int_to_char(1));
	m_ComuPort.WriteComm(data, 9);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMfcRsConnectingDlg::OnBnClickedRadio3()
{
	int id_index = _ttoi(channel_data) - 1;
	id[id_index].prf_mode_select = 2;
	CString data = PrfModeSelect((CHAR)channel_data[0], int_to_char(2));
	m_ComuPort.WriteComm(data, 9);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMfcRsConnectingDlg::OnBnClickedRadio4()
{
	int id_index = _ttoi(channel_data) - 1;
	id[id_index].prf_mode_select = 3;
	CString data = PrfModeSelect((CHAR)channel_data[0], int_to_char(3));
	m_ComuPort.WriteComm(data, 9);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMfcRsConnectingDlg::OnCbnSelchangeCombo1()
{
	int index = _ttoi(channel_data) - 1;
	id[index].mode_select = m_control_combobox1.GetCurSel();

	if (m_control_combobox1.GetCurSel() == 0)
	{
		CString data = ThroughModeSelect((CHAR)channel_data[0], '1');
		m_ComuPort.WriteComm(data, 9);

	}
	else
	{
		CString data = ThroughModeSelect((CHAR)channel_data[0], '0');
		m_ComuPort.WriteComm(data, 9);
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMfcRsConnectingDlg::OnCbnSelchangeCombo3()
{

	int index = _ttoi(channel_data) - 1;

	id[index].high_pass_filter = m_str_control_high_filter.GetCurSel();

	char high = (char)(m_str_control_high_filter.GetCurSel() + 48);
	char low = (char)(m_str_control_low_filter.GetCurSel() + 48);

	CString data = HighLowPassFilter((CHAR)channel_data[0], high, low);
	m_ComuPort.WriteComm(data, 9);


	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMfcRsConnectingDlg::OnCbnSelchangeCombo4()
{
	int index = _ttoi(channel_data) - 1;

	id[index].low_pass_filter = m_str_control_low_filter.GetCurSel();

	char high = (char)(m_str_control_high_filter.GetCurSel() + 48);
	char low = (char)(m_str_control_low_filter.GetCurSel() + 48);

	CString data = HighLowPassFilter((CHAR)channel_data[0], high, low);
	m_ComuPort.WriteComm(data, 9);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMfcRsConnectingDlg::OnCbnSelchangeCombo5()
{
	int index = _ttoi(channel_data) - 1;

	id[index].trigger_select = m_str_control_trigger.GetCurSel();

	char x1 = (char)(m_str_control_trigger.GetCurSel() + 48);

	CString data = TriggerSelect((CHAR)channel_data[0], x1);
	m_ComuPort.WriteComm(data, 9);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


// ui 초기 세팅 부분을 함수로 만듬
void CexMfcRsConnectingDlg::setting()
{
	m_control_combobox1.AddString(_T("Thru"));
	m_control_combobox1.AddString(_T("Echo"));

	m_str_control_channel.AddString(_T("1"));
	m_str_control_channel.AddString(_T("2"));
	m_str_control_channel.AddString(_T("3"));
	m_str_control_channel.AddString(_T("4"));
	m_str_control_channel.AddString(_T("5"));
	m_str_control_channel.AddString(_T("6"));
	m_str_control_channel.AddString(_T("7"));
	m_str_control_channel.AddString(_T("8"));

	m_str_control_high_filter.AddString(_T("0 MHz"));
	m_str_control_high_filter.AddString(_T("1 MHz"));
	m_str_control_high_filter.AddString(_T("2.5 MHz"));
	m_str_control_high_filter.AddString(_T("5 MHz"));
	m_str_control_high_filter.AddString(_T("7.5 MHz"));
	m_str_control_high_filter.AddString(_T("12.5 MHz"));


	m_str_control_low_filter.AddString(_T("50 MHz"));
	m_str_control_low_filter.AddString(_T("35 MHz"));
	m_str_control_low_filter.AddString(_T("22.5 MHz"));
	m_str_control_low_filter.AddString(_T("15 MHz"));
	m_str_control_low_filter.AddString(_T("10 MHz"));
	m_str_control_low_filter.AddString(_T("5 MHz"));

	m_str_control_trigger.AddString(_T("Internal"));
	m_str_control_trigger.AddString(_T("External"));


	m_control_gain.SetRange(0, 79);
	m_control_gain.SetRangeMin(0);
	m_control_gain.SetRangeMax(79);
	m_control_gain.SetPos(0);
	m_control_gain.SetTicFreq(1);
	m_control_gain.SetLineSize(1);

	m_control_voltage.SetRange(0, 41);
	m_control_voltage.SetRangeMin(0);
	m_control_voltage.SetRangeMax(41);
	m_control_voltage.SetPos(0);
	m_control_voltage.SetTicFreq(1);
	m_control_voltage.SetLineSize(1);

	m_control_pulse_width.SetRange(0, 8);
	m_control_pulse_width.SetRangeMin(0);
	m_control_pulse_width.SetRangeMax(8);
	m_control_pulse_width.SetPos(0);
	m_control_pulse_width.SetTicFreq(1);
	m_control_pulse_width.SetLineSize(1);

	m_control_damping.SetRange(0, 15);
	m_control_damping.SetRangeMin(0);
	m_control_damping.SetRangeMax(15);
	m_control_damping.SetPos(0);
	m_control_damping.SetTicFreq(1);
	m_control_damping.SetLineSize(1);



	//SetDlgItemText(IDC_STATIC_GAIN, "-13 dB");
	//SetDlgItemText(IDC_STATIC_VOL, "80 V");
	//SetDlgItemText(IDC_STATIC_WIDTH, "0");
	//SetDlgItemText(IDC_STATIC_DAM, "0");
}


void CexMfcRsConnectingDlg::getSerialPort()
{


	//http://stackoverflow.com/questions/1388871/how-do-i-get-a-list-of-available-serial-ports-in-win32  
	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM 에  시리얼포트 번호들이 등록되어 있음.  
	//  
	//레지스터 읽어오면 됨. 코드는 아래 링크에서 가져옴  
	//http://www.devpia.com/MAEUL/Contents/Detail.aspx?BoardID=50&MAEULNo=20&no=887656&ref=887655  
	HKEY hKey;

	//https://msdn.microsoft.com/ko-kr/library/windows/desktop/ms724895(v=vs.85).aspx  
	//오픈할 레지스터 키에 대한 기본키 이름  
	//오픈할 레지스터 서브키 이름  
	//레지스터키에 대한 핸들  
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[20], szName[100];
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	serialPort.ResetContent();
	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));


	//https://msdn.microsoft.com/en-us/library/windows/desktop/ms724865(v=vs.85).aspx  
	//hKey - 레지스터키 핸들  
	//index - 값을 가져올 인덱스.. 다수의 값이 있을 경우 필요  
	//szName - 항목값이 저장될 배열  
	//dwSize - 배열의 크기  
	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
	{
		index++;

		//https://msdn.microsoft.com/en-us/library/windows/desktop/ms724911(v=vs.85).aspx  
		//szName-레지터스터 항목의 이름  
		//dwType-항목의 타입, 여기에서는 널로 끝나는 문자열  
		//szData-항목값이 저장될 배열  
		//dwSize2-배열의 크기  
		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		serialPort.AddString(CString(szData));

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;
	}

	RegCloseKey(hKey);

}


/*

afx_msg void CexMfcRsConnectingDlg::OnBnClickedButton4()
{

}

afx_msg void CexMfcRsConnectingDlg::OnBnClickedButton3()
{

}
afx_msg void CexMfcRsConnectingDlg::OnCbnSelchangeCombo2()
{

}

afx_msg void CexMfcRsConnectingDlg::OnBnClickedRadio1()
{

}
afx_msg void CexMfcRsConnectingDlg::OnBnClickedRadio2() 
{

}
afx_msg void CexMfcRsConnectingDlg::OnBnClickedRadio3()
{

}
afx_msg void CexMfcRsConnectingDlg::OnBnClickedRadio4()
{

}
afx_msg void CexMfcRsConnectingDlg::OnCbnSelchangeCombo1()
{

}
afx_msg void CexMfcRsConnectingDlg::OnCbnSelchangeCombo3()
{

}
afx_msg void CexMfcRsConnectingDlg::OnCbnSelchangeCombo4()
{

}
afx_msg void CexMfcRsConnectingDlg::OnCbnSelchangeCombo5()
{

}

*/