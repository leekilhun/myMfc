
// ServerDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Server.h"
#include "ServerDlg.h"
#include "afxdialogex.h"

#include "SerialPort.h"
#include "TCPServer.h"
#include "UDPUnicastServer.h"
#include "UDPBroadcastServer.h"
#include "UDPMulticastServer.h"
#include <thread>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


enum NetType { TYPE_NONE = 0, TYPE_TCP = 1, TYPE_UDP_UNI = 2, TYPE_UDP_BROAD = 3, TYPE_UDP_MULTI = 4 };
enum BaudrateType {
	BDR_1200 = 0, BDR_2400 = 1, BDR_4800 = 2, BDR_9600 = 3, BDR_14400 = 4,
	BDR_19200 = 5, BDR_38400 = 6, BDR_57600 = 7, BDR_115200 = 8, BDR_128000 = 9, BDR_256000 = 10,
};
enum DatabitType { DATABIT_5 = 0, DATABIT_6 = 1, DATABIT_7 = 2, DATABIT_8 = 3 };
enum StopbitType { STOPBIT_1 = 0, STOPBIT_ONEHALF = 1, STOPBIT_2 = 2 };
enum ParitybitType { PARITYBIT_NO = 0, PARITYBIT_ODD = 1, PARITYBIT_EVEN = 2 };


// CServerDlg 대화 상자



CServerDlg::CServerDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SERVER__LOGLIST, logMessage);
	DDX_Control(pDX, IDC_DOWNLOAD__PROGRESS, fileProgress);
}

BEGIN_MESSAGE_MAP(CServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_DOWNLOAD__PROGRESS, &CServerDlg::OnNMCustomdrawDownload)
	ON_BN_CLICKED(IDC_RADIO_SOCKET, &CServerDlg::OnBnClickedRadioSocket)
	ON_BN_CLICKED(IDC_RADIO_RS232, &CServerDlg::OnBnClickedRadioRs232)
	ON_CBN_SELCHANGE(IDC_COMBO_COMPORT, &CServerDlg::OnCbnSelchangeComboComport)
	ON_CBN_SELCHANGE(IDC_COMBO_BAUDRATE, &CServerDlg::OnCbnSelchangeComboBaudrate)
	ON_CBN_SELCHANGE(IDC_COMBO_DATABIT, &CServerDlg::OnCbnSelchangeComboDatabit)
	ON_CBN_SELCHANGE(IDC_COMBO_STOPBIT, &CServerDlg::OnCbnSelchangeComboStopbit)
	ON_CBN_SELCHANGE(IDC_COMBO_PARITYBIT, &CServerDlg::OnCbnSelchangeComboParitybit)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, &CServerDlg::OnBnClickedButtonOpen)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, &CServerDlg::OnBnClickedButtonClose)
	ON_CBN_SELCHANGE(IDC_COMBO_PROTOCOL, &CServerDlg::OnCbnSelchangeComboProtocol)
	ON_BN_CLICKED(IDC_CONNECT_BUTTON, &CServerDlg::OnBnClickedConnectButton)
END_MESSAGE_MAP()


// CServerDlg 메시지 처리기

BOOL CServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	GetDlgItem(IDC_COMBO_COMPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_BAUDRATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_DATABIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_STOPBIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_PARITYBIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);

	GetDlgItem(IDC_COMBO_PROTOCOL)->EnableWindow(FALSE);
	GetDlgItem(IDC_CONNECT_BUTTON)->EnableWindow(FALSE);

	m_netType = TYPE_NONE;
	m_baudrate = -1;
	m_databit = -1;
	m_stopbit = -1;
	m_paritybit = -1;

	SetBackgroundColor(RGB(255, 255, 255));


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}



void CServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
	
}



// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CServerDlg::OnPaint()
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
HCURSOR CServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServerDlg::SaveLog()
{
	time_t t = time(NULL);
	struct tm tm;
	errno_t err_t = localtime_s(&tm, &t);
	if (err_t != 0)
	{
		AfxMessageBox(L"Error: localtime set Error");
		exit(1);
	}
	else
	{
		CString strFilename;
		strFilename.Format(L".\\Server_%d-%02d-%02d-%02d-%02d-%02d.txt",
			tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

		CStdioFile logFile;

		logFile.Open(strFilename, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		CString log;
		for (int i = 0; i < logMessage.GetCount(); i++)
		{
			logMessage.GetText(i, log);
			logFile.WriteString(log + "\n");
		}
		logFile.Close();
	}
}

void CServerDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// WindowSize 크기 설정
	lpMMI->ptMinTrackSize.x = 800;
	lpMMI->ptMinTrackSize.y = 600;

	lpMMI->ptMaxTrackSize.x = 800;
	lpMMI->ptMaxTrackSize.y = 600;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}


std::thread threadObj;
// Start 버튼 클릭 시 소켓 open
void CServerDlg::OnBnClickedConnectButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	time_t t = time(NULL);
	struct tm tm;
	errno_t err_t = localtime_s(&tm, &t);
	if (err_t != 0)
	{
		logMessage.AddString(L"Time ERROR!");
	}
	CString strTime;
	strTime.Format(L"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	if (m_netType == NetType::TYPE_TCP)
	{
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(L"Stop");
		GetDlgItem(IDC_COMBO_PROTOCOL)->EnableWindow(false);

		m_tcpServer = new TCPServer(logMessage, fileProgress);
		threadObj = std::thread([&]() { m_tcpServer->TCPServerStart(); });
		logMessage.AddString(L" ");
		logMessage.AddString(strTime);
		logMessage.AddString(L"TCP Server Open");
		logMessage.AddString(L" ");

		m_netType = -1;
	}
	else if (m_netType == NetType::TYPE_UDP_UNI)
	{
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(L"Stop");
		GetDlgItem(IDC_COMBO_PROTOCOL)->EnableWindow(false);

		m_udpUnicastServer = new UDPUnicastServer(logMessage, fileProgress);
		threadObj = std::thread([&]() { m_udpUnicastServer->UnicastStart(); });
		logMessage.AddString(L" ");
		logMessage.AddString(strTime);
		logMessage.AddString(L"UDP Unicast Server Open");
		logMessage.AddString(L" ");

		m_netType = -1;
	}
	else if (m_netType == NetType::TYPE_UDP_BROAD)
	{
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(L"Stop");
		GetDlgItem(IDC_COMBO_PROTOCOL)->EnableWindow(false);

		m_udpBroadcastServer = new UDPBroadcastServer(logMessage, fileProgress);
		threadObj = std::thread([&]() { m_udpBroadcastServer->BroadcastStart(); });
		logMessage.AddString(L" ");
		logMessage.AddString(strTime);
		logMessage.AddString(L"UDP Broadcast Server Open");
		logMessage.AddString(L" ");

		m_netType = -1;
	}
	else if (m_netType == NetType::TYPE_UDP_MULTI)
	{
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(L"Stop");
		GetDlgItem(IDC_COMBO_PROTOCOL)->EnableWindow(false);

		m_udpMulticastServer = new UDPMulticastServer(logMessage, fileProgress);
		threadObj = std::thread([&]() { m_udpMulticastServer->MulticastStart(); });
		logMessage.AddString(L" ");
		logMessage.AddString(strTime);
		logMessage.AddString(L"UDP Multicast Server Open");
		logMessage.AddString(L" ");

		m_netType = -1;
	}
	else if (m_netType == NetType::TYPE_NONE)
	{
		AfxMessageBox(L"Please select Protocol to open server");
	}
	else
	{
		GetDlgItem(IDC_CONNECT_BUTTON)->SetWindowText(L"Start");
		GetDlgItem(IDC_COMBO_PROTOCOL)->EnableWindow(true);

		if (m_tcpServer != nullptr)
		{
			m_tcpServer->~TCPServer();
		}
		else if (m_udpUnicastServer != nullptr)
		{
			m_udpUnicastServer->~UDPUnicastServer();
		}
		else if (m_udpBroadcastServer != nullptr)
		{
			m_udpBroadcastServer->~UDPBroadcastServer();
		}
		else if (m_udpMulticastServer != nullptr)
		{
			m_udpMulticastServer->~UDPMulticastServer();
		}

		if (threadObj.joinable())
		{
			threadObj.join();
		}

		m_tcpServer = nullptr;
		m_udpMulticastServer = nullptr;
		m_udpBroadcastServer = nullptr;
		m_udpUnicastServer = nullptr;

		m_combo_protocol->SetCurSel(0);
		m_netType = TYPE_NONE;

		logMessage.AddString(L" ");
		logMessage.AddString(strTime);
		logMessage.AddString(L"Server Close Connection");
		logMessage.AddString(L" ");

		GetDlgItem(IDC_RADIO_RS232)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_SOCKET)->EnableWindow(TRUE);
	}
}



void CServerDlg::OnNMCustomdrawDownload(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}



void CServerDlg::OnBnClickedRadioSocket()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_COMPORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_BAUDRATE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_DATABIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_STOPBIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_PARITYBIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);

	GetDlgItem(IDC_CONNECT_BUTTON)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_PROTOCOL)->EnableWindow(TRUE);
	logMessage.AddString(L" ");
	logMessage.AddString(L"Socket mode");
	logMessage.AddString(L" ");
}


void CServerDlg::OnBnClickedRadioRs232()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_COMBO_COMPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_BAUDRATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_DATABIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_STOPBIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_PARITYBIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);

	GetDlgItem(IDC_CONNECT_BUTTON)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_PROTOCOL)->EnableWindow(FALSE);
	logMessage.AddString(L" ");
	logMessage.AddString(L"Serial mode");
	logMessage.AddString(L" ");
}


void CServerDlg::OnCbnSelchangeComboComport()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CServerDlg::OnCbnSelchangeComboBaudrate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_combo_baudrate = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_BAUDRATE));
	int baudrate = static_cast<short>(m_combo_baudrate->GetCurSel());

	if (baudrate == BaudrateType::BDR_1200) { m_baudrate = 1200; }
	else if (baudrate == BaudrateType::BDR_2400) { m_baudrate = 2400; }
	else if (baudrate == BaudrateType::BDR_4800) { m_baudrate = 4800; }
	else if (baudrate == BaudrateType::BDR_9600) { m_baudrate = 9600; }
	else if (baudrate == BaudrateType::BDR_14400) { m_baudrate = 14400; }
	else if (baudrate == BaudrateType::BDR_19200) { m_baudrate = 19200; }
	else if (baudrate == BaudrateType::BDR_38400) { m_baudrate = 38400; }
	else if (baudrate == BaudrateType::BDR_57600) { m_baudrate = 57600; }
	else if (baudrate == BaudrateType::BDR_115200) { m_baudrate = 115200; }
	else if (baudrate == BaudrateType::BDR_128000) { m_baudrate = 128000; }
	else if (baudrate == BaudrateType::BDR_256000) { m_baudrate = 256000; }
	else
	{
		AfxMessageBox(L"Error while selecting baudrate");
		return;
	}
}


void CServerDlg::OnCbnSelchangeComboDatabit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_combo_databit = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_DATABIT));
	int databit = static_cast<short>(m_combo_databit->GetCurSel());

	if (databit == DatabitType::DATABIT_5) { m_databit = 5; }
	else if (databit == DatabitType::DATABIT_6) { m_databit = 6; }
	else if (databit == DatabitType::DATABIT_7) { m_databit = 7; }
	else if (databit == DatabitType::DATABIT_8) { m_databit = 8; }
	else
	{
		AfxMessageBox(L"Error while selecting databit");
		return;
	}
}


void CServerDlg::OnCbnSelchangeComboStopbit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_combo_stopbit = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_STOPBIT));
	int stopbit = static_cast<short>(m_combo_stopbit->GetCurSel());

	if (stopbit == StopbitType::STOPBIT_1) { m_stopbit = 0; }
	else if (stopbit == StopbitType::STOPBIT_ONEHALF) { m_stopbit = 1; }
	else if (stopbit == StopbitType::STOPBIT_2) { m_stopbit = 2; }
	else
	{
		AfxMessageBox(L"Error while selecting stopbit");
		return;
	}
}


void CServerDlg::OnCbnSelchangeComboParitybit()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_combo_paritybit = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_PARITYBIT));
	int paritybit = static_cast<short>(m_combo_paritybit->GetCurSel());

	if (paritybit == ParitybitType::PARITYBIT_NO) { m_paritybit = 0; }
	else if (paritybit == ParitybitType::PARITYBIT_ODD) { m_paritybit = 1; }
	else if (paritybit == ParitybitType::PARITYBIT_EVEN) { m_paritybit = 2; }
	else
	{
		AfxMessageBox(L"Error while selecting paritybit");
		return;
	}
}


void CServerDlg::OnBnClickedButtonOpen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_combo_portName = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_COMPORT));
	int nSel = m_combo_portName->GetCurSel();
	CString portName;
	m_combo_portName->GetLBText(nSel, portName);

	time_t t = time(NULL);
	struct tm tm;
	errno_t err_t = localtime_s(&tm, &t);
	CString strTime;
	strTime.Format(L"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	if (err_t != 0)
	{
		AfxMessageBox(L"Error: localtime set Error");
		exit(1);
	}
	else
	{
		CString strFilename;

		if (m_combo_portName == nullptr)
		{
			AfxMessageBox(L"Please insert Comport Name");
			return;
		}
		else if (m_baudrate == -1)
		{
			AfxMessageBox(L"Please select Baudrate");
			return;
		}
		else if (m_databit == -1)
		{
			AfxMessageBox(L"Please select Databit");
			return;
		}
		else if (m_stopbit == -1)
		{
			AfxMessageBox(L"Please select Stopbit");
			return;
		}
		else if (m_paritybit == -1)
		{
			AfxMessageBox(L"Please select Paritybit");
			return;
		}


		if (!m_serialPort->openPort(portName))
		{
			AfxMessageBox(L"Error: Can not open serial port");
			return;
		}


		if (!m_serialPort->configureSerialSet(m_baudrate, m_databit, m_stopbit, m_paritybit))
		{
			AfxMessageBox(L"Error: Configure Serial Set Error");
			return;
		}
		else
		{
			logMessage.AddString(L" ");
			logMessage.AddString(strTime);
			logMessage.AddString(L"Serial Port Open!");
			logMessage.AddString(L" ");
		}

		GetDlgItem(IDC_COMBO_COMPORT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_BAUDRATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_DATABIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_STOPBIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_PARITYBIT)->EnableWindow(FALSE);

		GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(TRUE);
		GetDlgItem(IDC_RADIO_RS232)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_SOCKET)->EnableWindow(FALSE);

		threadObj = std::thread([&]() { m_serialPort->readFile(); });

	}
}


void CServerDlg::OnBnClickedButtonClose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	time_t t = time(NULL);
	struct tm tm;
	errno_t err_t = localtime_s(&tm, &t);
	if (err_t != 0)
	{
		logMessage.AddString(L"Time ERROR!");
	}
	CString strTime;
	strTime.Format(L"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	m_serialPort->~SerialPort();
	if (threadObj.joinable())
	{
		threadObj.join();
	}
	logMessage.AddString(L" ");
	logMessage.AddString(strTime);
	logMessage.AddString(L"Serial Port Close!");
	logMessage.AddString(L" ");

	GetDlgItem(IDC_COMBO_COMPORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_BAUDRATE)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_DATABIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_STOPBIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_COMBO_PARITYBIT)->EnableWindow(TRUE);

	GetDlgItem(IDC_BUTTON_OPEN)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_RADIO_RS232)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_SOCKET)->EnableWindow(TRUE);
}



void CServerDlg::OnCbnSelchangeComboProtocol()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_combo_protocol = static_cast<CComboBox*>(GetDlgItem(IDC_COMBO_PROTOCOL));
	m_netType = static_cast<short>(m_combo_protocol->GetCurSel());
}

