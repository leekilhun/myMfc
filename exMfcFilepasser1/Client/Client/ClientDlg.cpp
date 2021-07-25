
// ClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"


#include "SocketClient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS 

#define TTL 64
#define BUF_SIZE 104857600

struct sockaddr_in bcast_group;
struct sockaddr_in mcast_group;

HANDLE idComDev;
DCB dcb;
OVERLAPPED osWrite;


// CClientDlg 대화 상자



CClientDlg::CClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_SocketClient = nullptr;
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_PROTOCOL_COMBO, m_comboProtocolList);
	DDX_Control(pDX, IDC_PROGRESS, m_progress);
	DDX_Control(pDX, IDC_LOG_LIST, logMessage);
	DDX_Control(pDX, IDC_DATA_COMBO, m_comboDataBitsList);
	DDX_Control(pDX, IDC_STOP_COMBO, m_comboStopBitsList);
	DDX_Control(pDX, IDC_BAUDRATE_COMBO, m_comboBaudrateList);
	DDX_Control(pDX, IDC_PARITY_COMBO, m_comboParityList);
	DDX_Control(pDX, IDC_IP_EDIT, m_IpEditBox);
	DDX_Control(pDX, IDC_PORT_EDIT, m_PortEditBox);
	DDX_Control(pDX, IDC_COM_COMBO, m_comboComList);

}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SYSCOMMAND()
	ON_EN_CHANGE(IDC_IP_EDIT, &CClientDlg::OnEnChangeIpEdit)
	ON_EN_CHANGE(IDC_PORT_EDIT, &CClientDlg::OnEnChangePortEdit)
	ON_BN_CLICKED(IDC_BUTTON_FILESEND, &CClientDlg::OnBnClickedButtonFilesend)
	ON_BN_CLICKED(IDC_RADIO_Socket, &CClientDlg::OnBnClickedRadioSocket)
	ON_BN_CLICKED(IDC_RADIO_SERIAL, &CClientDlg::OnBnClickedRadioSerial)
	ON_BN_CLICKED(IDC_BUTTON_OPEN_PORT, &CClientDlg::OnBnClickedButtonOpenPort)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_PORT, &CClientDlg::OnBnClickedButtonClosePort)
	ON_BN_CLICKED(IDC_BTN_OK, &CClientDlg::OnBnClickedBtnOk)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CClientDlg::OnBnClickedBtnClose)
END_MESSAGE_MAP()


// CClientDlg 메시지 처리기

void CClientDlg::initComponents()
{
	m_comboComList.AddString(_T("COM1"));
	m_comboComList.AddString(_T("COM2"));
	m_comboComList.AddString(_T("COM3"));
	m_comboComList.AddString(_T("COM4"));
	m_comboComList.AddString(_T("COM5"));

	m_comboBaudrateList.AddString(_T("1200"));
	m_comboBaudrateList.AddString(_T("2400"));
	m_comboBaudrateList.AddString(_T("4800"));
	m_comboBaudrateList.AddString(_T("9600"));
	m_comboBaudrateList.AddString(_T("14400"));
	m_comboBaudrateList.AddString(_T("19200"));
	m_comboBaudrateList.AddString(_T("38400"));
	m_comboBaudrateList.AddString(_T("57600"));
	m_comboBaudrateList.AddString(_T("115200"));
	m_comboBaudrateList.AddString(_T("128000"));
	m_comboBaudrateList.AddString(_T("256000"));

	m_comboDataBitsList.AddString(_T("5"));
	m_comboDataBitsList.AddString(_T("6"));
	m_comboDataBitsList.AddString(_T("7"));
	m_comboDataBitsList.AddString(_T("8"));

	m_comboStopBitsList.AddString(_T("1"));
	m_comboStopBitsList.AddString(_T("1.5"));
	m_comboStopBitsList.AddString(_T("2"));

	m_comboParityList.AddString(_T("None"));
	m_comboParityList.AddString(_T("Odd"));
	m_comboParityList.AddString(_T("Even"));

	GetDlgItem(IDC_IP_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_PORT_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_PROTOCOL_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);

	GetDlgItem(IDC_COM_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BAUDRATE_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATA_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PARITY_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLOSE_PORT)->EnableWindow(FALSE);

	SetBackgroundColor(RGB(255, 255, 255));
}

BOOL CClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	initComponents();


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnSysCommand(nID, lParam);
}


void CClientDlg::OnEnChangeIpEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CClientDlg::OnEnChangePortEdit()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CClientDlg::OnBnClickedButtonFilesend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int nCheck = GetCheckedRadioButton(IDC_RADIO_Socket, IDC_RADIO_SERIAL);
	int index = m_comboProtocolList.GetCurSel();
	index += 1;
	CString strIp;
	int strPort;
	GetDlgItemText(IDC_IP_EDIT, strIp);
	strPort = GetDlgItemInt(IDC_PORT_EDIT);

	int NameLength_send;
	int fileSize;
	int* fSize;

	CStringA strFileName_send;
	CString strFilePath_send;
	CFile sendFile_send;
	CString  strFileName;

	byte* data_send = new byte[BUF_SIZE];
	DWORD dwRead_send;

	time_t t = time(NULL);
	struct tm tm;
	errno_t err_t = localtime_s(&tm, &t);
	if (err_t != 0)
	{
		logMessage.AddString(L"Time ERROR!");
	}
	CString strTime;
	strTime.Format(L"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	if (index == 1)
	{
		CFileDialog fd1(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);
		if (fd1.DoModal() == IDOK)
		{
			strFilePath_send = fd1.GetPathName();
			sendFile_send.Open(strFilePath_send, CFile::modeRead | CFile::typeBinary);

			strFileName_send = sendFile_send.GetFileName();
			NameLength_send = fd1.GetFileName().GetLength();
			char* strName_send = new char[NameLength_send];
			strFileName_send = sendFile_send.GetFileName();
			strName_send = strFileName_send.GetBuffer(NameLength_send);

			strFileName = strFileName_send;
			logMessage.AddString(strFileName);


			m_SocketClient->Send(strName_send, NameLength_send);
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Name send");

			dwRead_send = sendFile_send.Read(data_send, BUF_SIZE);

			m_progress.SetRange(0, (short)dwRead_send);

			m_SocketClient->Send(data_send, dwRead_send);
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Data send Completion !");

			logMessage.SetCurSel(logMessage.GetCount() - 1);

			m_progress.SetPos(dwRead_send);
			if (AfxMessageBox(_T("File Transfer Complete!"), MB_OK | MB_ICONINFORMATION) == IDOK)
			{
				m_progress.SetPos(0);
			}

			sendFile_send.Close();
			strFileName_send.ReleaseBuffer(-1);
			strName_send = NULL;
			delete data_send;
		}
	}
	else if (index == 2)
	{
		CFileDialog fd1(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);
		if (fd1.DoModal() == IDOK)
		{
			strFilePath_send = fd1.GetPathName();
			sendFile_send.Open(strFilePath_send, CFile::modeRead | CFile::typeBinary);

			strFileName_send = sendFile_send.GetFileName();
			NameLength_send = fd1.GetFileName().GetLength();
			char* strName_send = new char[NameLength_send];
			strFileName_send = sendFile_send.GetFileName();
			strName_send = strFileName_send.GetBuffer(NameLength_send);

			strFileName = strFileName_send;
			logMessage.AddString(strFileName);


			m_SocketClient->SendTo(strName_send, NameLength_send, (struct sockaddr*)&bcast_group, sizeof(bcast_group));
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Name send");

			dwRead_send = sendFile_send.Read(data_send, BUF_SIZE);

			fileSize = dwRead_send;
			m_progress.SetRange(0, fileSize);

			fSize = &fileSize;
			m_SocketClient->SendTo(fSize, 4, (struct sockaddr*)&bcast_group, sizeof(bcast_group));
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Size send");

			char buf[1500];
			memset(buf, 0, sizeof(buf));

			int size = 0;
			int sendsize = 1500;
			int progressSize = 0;

			logMessage.AddString(strTime);
			logMessage.AddString(L"File Data sending ...");
			while (sendsize > 0)
			{
				if (dwRead_send < (DWORD)sendsize)
					sendsize = dwRead_send;

				memcpy(buf, &data_send[size], sendsize);
				sendsize = m_SocketClient->SendTo(buf, sendsize, (struct sockaddr*)&bcast_group, sizeof(bcast_group));

				if (sendsize == 0)
					continue;

				dwRead_send -= sendsize;
				size += sendsize;

				progressSize += sendsize;
				m_progress.SetPos(progressSize);
			}
			if (progressSize == fileSize)
			{
				if (AfxMessageBox(_T("File Transfer Complete!"), MB_OK | MB_ICONINFORMATION) == IDOK)
				{
					m_progress.SetPos(0);
				}
			}
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Data send Completion !");

			logMessage.SetCurSel(logMessage.GetCount() - 1);

			sendFile_send.Close();
			strFileName_send.ReleaseBuffer(-1);
			strName_send = NULL;
			delete data_send;
		}
	}

	else if (index == 3)
	{
		CFileDialog fd1(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);
		if (fd1.DoModal() == IDOK)
		{
			strFilePath_send = fd1.GetPathName();
			sendFile_send.Open(strFilePath_send, CFile::modeRead | CFile::typeBinary);

			strFileName_send = sendFile_send.GetFileName();
			NameLength_send = fd1.GetFileName().GetLength();
			char* strName_send = new char[NameLength_send];
			strFileName_send = sendFile_send.GetFileName();
			strName_send = strFileName_send.GetBuffer(NameLength_send);

			strFileName = strFileName_send;
			logMessage.AddString(strFileName);
			logMessage.AddString(L" ");

			m_SocketClient->SendTo(strName_send, NameLength_send, (struct sockaddr*)&mcast_group, sizeof(mcast_group));
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Name send");

			dwRead_send = sendFile_send.Read(data_send, BUF_SIZE);

			fileSize = dwRead_send;
			m_progress.SetRange(0, fileSize);

			fSize = &fileSize;
			m_SocketClient->SendTo(fSize, 4, (struct sockaddr*)&mcast_group, sizeof(mcast_group));
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Size send");

			char buf[1500];
			memset(buf, 0, sizeof(buf));

			int size = 0;
			int sendsize = 1500;
			int progressSize = 0;

			logMessage.AddString(strTime);
			logMessage.AddString(L"File Data sending ...");
			while (sendsize > 0)
			{
				if (dwRead_send < (DWORD)sendsize)
					sendsize = dwRead_send;

				memcpy(buf, &data_send[size], sendsize);
				sendsize = m_SocketClient->SendTo(buf, sendsize, (struct sockaddr*)&mcast_group, sizeof(mcast_group));

				if (sendsize == 0)
					continue;

				dwRead_send -= sendsize;
				size += sendsize;

				progressSize += sendsize;
				m_progress.SetPos(progressSize);
			}
			if (progressSize == fileSize)
			{
				if (AfxMessageBox(_T("File Transfer Complete!"), MB_OK | MB_ICONINFORMATION) == IDOK)
				{
					m_progress.SetPos(0);
				}
			}
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Data send Completion !");

			logMessage.SetCurSel(logMessage.GetCount() - 1);

			sendFile_send.Close();
			strFileName_send.ReleaseBuffer(-1);
			strName_send = NULL;
			delete data_send;
		}
	}

	else if (index == 4)
	{
		CFileDialog fd1(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);
		if (fd1.DoModal() == IDOK)
		{
			strFilePath_send = fd1.GetPathName();
			sendFile_send.Open(strFilePath_send, CFile::modeRead | CFile::typeBinary);

			strFileName_send = sendFile_send.GetFileName();
			NameLength_send = fd1.GetFileName().GetLength();
			char* strName_send = new char[NameLength_send];
			strFileName_send = sendFile_send.GetFileName();
			strName_send = strFileName_send.GetBuffer(NameLength_send);

			strFileName = strFileName_send;
			logMessage.AddString(strFileName);


			m_SocketClient->SendTo(strName_send, NameLength_send, strPort, strIp);
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Name send");

			dwRead_send = sendFile_send.Read(data_send, BUF_SIZE);

			fileSize = dwRead_send;
			m_progress.SetRange(0, fileSize);

			fSize = &fileSize;
			m_SocketClient->SendTo(fSize, 4, strPort, strIp);
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Size send");

			char buf[1500];
			memset(buf, 0, sizeof(buf));

			int size = 0;
			int sendsize = 1500;
			int progressSize = 0;

			logMessage.AddString(strTime);
			logMessage.AddString(L"File Data sending ...");
			while (sendsize > 0)
			{
				if (dwRead_send < (DWORD)sendsize)
					sendsize = dwRead_send;

				memcpy(buf, &data_send[size], sendsize);
				sendsize = m_SocketClient->SendTo(buf, sendsize, strPort, strIp);

				if (sendsize == 0)
					continue;

				dwRead_send -= sendsize;
				size += sendsize;

				progressSize += sendsize;
				m_progress.SetPos(progressSize);
			}
			if (progressSize == fileSize)
			{
				if (AfxMessageBox(_T("File Transfer Complete!"), MB_OK | MB_ICONINFORMATION) == IDOK)
				{
					m_progress.SetPos(0);
				}
			}
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Data send Completion !");
			logMessage.SetCurSel(logMessage.GetCount() - 1);

			sendFile_send.Close();
			strFileName_send.ReleaseBuffer(-1);
			strName_send = NULL;
		}
	}

	else if (nCheck == IDC_RADIO_SERIAL)
	{

		CFileDialog fd1(TRUE, NULL, NULL, OFN_HIDEREADONLY, NULL, NULL);
		if (fd1.DoModal() == IDOK)
		{
			strFilePath_send = fd1.GetPathName();
			sendFile_send.Open(strFilePath_send, CFile::modeRead | CFile::typeBinary);

			strFileName_send = sendFile_send.GetFileName();
			NameLength_send = fd1.GetFileName().GetLength();
			char* strName_send = new char[NameLength_send];
			strFileName_send = sendFile_send.GetFileName();
			strName_send = strFileName_send.GetBuffer(NameLength_send);

			strFileName = strFileName_send;
			logMessage.AddString(strFileName);


			WriteFile(idComDev, strName_send, NameLength_send, NULL, &osWrite);
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Name send");

			char null[2] = "?";
			WriteFile(idComDev, null, 1, NULL, &osWrite);

			dwRead_send = sendFile_send.Read(data_send, BUF_SIZE);

			m_progress.SetRange(0, (short)dwRead_send);

			WriteFile(idComDev, data_send, dwRead_send, NULL, &osWrite);

			PurgeComm(idComDev, PURGE_TXCLEAR | PURGE_RXCLEAR);

			m_progress.SetPos(dwRead_send);
			if (AfxMessageBox(_T("File Transfer Complete!"), MB_OK | MB_ICONINFORMATION) == IDOK)
			{
				m_progress.SetPos(0);
			}
			logMessage.AddString(strTime);
			logMessage.AddString(L"File Data send Completion !");
			logMessage.SetCurSel(logMessage.GetCount() - 1);
		}
	}
}


void CClientDlg::OnBnClickedRadioSocket()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	logMessage.AddString(L"Socket Mode");
	logMessage.SetCurSel(logMessage.GetCount() - 1);

	CloseHandle(idComDev);

	m_comboProtocolList.AddString(_T("TCP/IP"));
	m_comboProtocolList.AddString(_T("UDP Unicast"));
	m_comboProtocolList.AddString(_T("UDP Multicast"));
	m_comboProtocolList.AddString(_T("UDP broadcast"));

	GetDlgItem(IDC_IP_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_PORT_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_PROTOCOL_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);

	GetDlgItem(IDC_COM_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BAUDRATE_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATA_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PARITY_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OPEN_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLOSE_PORT)->EnableWindow(FALSE);
}


void CClientDlg::OnBnClickedRadioSerial()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	logMessage.AddString(L"Serial Mode");
	logMessage.SetCurSel(logMessage.GetCount() - 1);

	if (m_SocketClient != NULL) m_SocketClient->Close();

	m_IpEditBox.SetSel(0, -1);
	m_IpEditBox.Clear();
	m_PortEditBox.SetSel(0, -1);
	m_PortEditBox.Clear();
	m_comboProtocolList.ResetContent();

	GetDlgItem(IDC_COM_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_BAUDRATE_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_DATA_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_PARITY_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_OPEN_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLOSE_PORT)->EnableWindow(FALSE);

	GetDlgItem(IDC_IP_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_PORT_EDIT)->EnableWindow(FALSE);
	GetDlgItem(IDC_PROTOCOL_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_OK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);
}


void CClientDlg::OnBnClickedButtonOpenPort()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString strCom;
	CString strBaudRate;
	CString strDataBits;
	CString strStopBits;
	CString strParity;

	time_t t = time(NULL);
	struct tm tm;
	errno_t err_t = localtime_s(&tm, &t);
	if (err_t != 0)
	{
		logMessage.AddString(L"Time ERROR!");
	}
	CString strTime;
	strTime.Format(L"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	GetDlgItem(IDC_BUTTON_OPEN_PORT)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLOSE_PORT)->EnableWindow(TRUE);

	GetDlgItemText(IDC_COM_COMBO, strCom);
	GetDlgItemText(IDC_BAUDRATE_COMBO, strBaudRate);
	GetDlgItemText(IDC_DATA_COMBO, strDataBits);
	GetDlgItemText(IDC_STOP_COMBO, strStopBits);
	GetDlgItemText(IDC_PARITY_COMBO, strParity);

	idComDev = CreateFile(strCom, GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	SetupComm(idComDev, 1000000, 1000000);
	PurgeComm(idComDev, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	GetCommState(idComDev, &dcb);

	if (strBaudRate == "1200")   // 전송 속도 체크
		dcb.BaudRate = CBR_1200;
	else if (strBaudRate == "2400")
		dcb.BaudRate = CBR_2400;
	else if (strBaudRate == "4800")
		dcb.BaudRate = CBR_4800;
	else if (strBaudRate == "9600")
		dcb.BaudRate = CBR_9600;
	else if (strBaudRate == "14400")
		dcb.BaudRate = CBR_14400;
	else if (strBaudRate == "19200")
		dcb.BaudRate = CBR_19200;
	else if (strBaudRate == "38400")
		dcb.BaudRate = CBR_38400;
	else if (strBaudRate == "57600")
		dcb.BaudRate = CBR_57600;
	else if (strBaudRate == "115200")
		dcb.BaudRate = CBR_115200;
	else if (strBaudRate == "128000")
		dcb.BaudRate = CBR_128000;
	else if (strBaudRate == "256000")
		dcb.BaudRate = CBR_256000;
	else
		dcb.BaudRate = CBR_1200;

	if (strDataBits == "5")   // 데이터 비트 체크
		dcb.ByteSize = 5;
	else if (strDataBits == "6")
		dcb.ByteSize = 6;
	else if (strDataBits == "7")
		dcb.ByteSize = 7;
	else if (strDataBits == "8")
		dcb.ByteSize = 8;
	else
		dcb.ByteSize = 8;

	if (strStopBits == "1")   // 스톱 비트 체크
		dcb.StopBits = ONESTOPBIT;
	else if (strStopBits == "1.5")
		dcb.StopBits = ONE5STOPBITS;
	else if (strStopBits == "2")
		dcb.StopBits = TWOSTOPBITS;
	else
		dcb.StopBits = ONESTOPBIT;

	if (strParity == "None")   // 페리티 체크
		dcb.Parity = NOPARITY;
	else if (strParity == "Even")
		dcb.Parity = EVENPARITY;
	else if (strParity == "Odd")
		dcb.Parity = ODDPARITY;
	else
		dcb.Parity = NOPARITY;

	SetCommState(idComDev, &dcb);   // DCB를 idComDev에 연결


	osWrite.Offset = 0;
	osWrite.OffsetHigh = 0;
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	AfxMessageBox(_T(" COM Port Open Complete !"), MB_OK | MB_ICONINFORMATION);
	logMessage.AddString(strTime);
	logMessage.AddString(L"COM Port Open Complete !");

	logMessage.SetCurSel(logMessage.GetCount() - 1);

	GetDlgItem(IDC_COM_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_BAUDRATE_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_DATA_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_STOP_COMBO)->EnableWindow(FALSE);
	GetDlgItem(IDC_PARITY_COMBO)->EnableWindow(FALSE);
}


void CClientDlg::OnBnClickedButtonClosePort()
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
	strTime.Format(L"%d-%02d-%02d 02%d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	GetDlgItem(IDC_BUTTON_OPEN_PORT)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLOSE_PORT)->EnableWindow(FALSE);

	GetDlgItem(IDC_COM_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_BAUDRATE_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_DATA_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_STOP_COMBO)->EnableWindow(TRUE);
	GetDlgItem(IDC_PARITY_COMBO)->EnableWindow(TRUE);

	CloseHandle(idComDev);
	AfxMessageBox(_T("Successful COM Port Close"), MB_OK | MB_ICONINFORMATION);
	logMessage.AddString(strTime);
	logMessage.AddString(L"COM Port Close !!!");
	logMessage.SetCurSel(logMessage.GetCount() - 1);

}

void CClientDlg::Save()
{
	time_t t = time(NULL);
	struct tm tm;
	errno_t err_t = localtime_s(&tm, &t);
	if (err_t != 0)
	{
		logMessage.AddString(L"Time ERROR!");
	}
	CString strFileName;
	strFileName.Format(L".\\Client_%d-%02d-%02d-%02d-%02d-%02d.txt", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
	CString name = strFileName;

	CStdioFile logFile;

	logFile.Open(name, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
	CString log;
	for (int i = 0; i < logMessage.GetCount(); i++)
	{
		logMessage.GetText(i, log);
		logFile.WriteString(log + "\n");
	}
	logFile.Close();
}


void CClientDlg::OnBnClickedBtnOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_comboProtocolList.GetCurSel();
	index += 1;

	CString strIp;
	int strPort;
	GetDlgItemText(IDC_IP_EDIT, strIp);
	strPort = GetDlgItemInt(IDC_PORT_EDIT);

	time_t t = time(NULL);
	struct tm tm;
	errno_t err_t = localtime_s(&tm, &t);
	if (err_t != 0)
	{
		logMessage.AddString(L"Time ERROR!");
	}
	CString strTime;
	strTime.Format(L"%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	GetDlgItem(IDC_BUTTON_OK)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(TRUE);

	if (index == 1)
	{
		m_SocketClient->SetWnd(this->m_hWnd);   // Sendmessage 활용을 위한 메인의 핸들을 받는 함수
		m_SocketClient->Create();
		if (m_SocketClient->Connect(strIp, strPort) == FALSE)
		{
			if (IDOK == AfxMessageBox(_T("※ERROR : Failed to connect Server"), MB_OK | MB_ICONERROR))
			{
				m_SocketClient->Close();
				logMessage.AddString(strTime);
				logMessage.AddString(L"※ERROR : Failed to connect TCP Socket");
				logMessage.SetCurSel(logMessage.GetCount() - 1);
			}
		}
		else
		{
			AfxMessageBox(_T("Successfully connected to Server"), MB_OK | MB_ICONINFORMATION);
			logMessage.AddString(strTime);
			logMessage.AddString(L"TCP Socket Create & Connect !");
			logMessage.SetCurSel(logMessage.GetCount() - 1);
		}
	}

	else if (index == 2)
	{
		char broadcast = '1';

		m_SocketClient->SetWnd(this->m_hWnd);
		if (m_SocketClient->Create(AF_INET, SOCK_DGRAM, 0) < 0)
		{
			AfxMessageBox(_T("※ERROR : Can't create send Socket"), MB_OK | MB_ICONERROR);
			logMessage.AddString(strTime);
			logMessage.AddString(L"※ERROR : Can't create  UDP Broadcast Socket");
			logMessage.SetCurSel(logMessage.GetCount() - 1);
			PostQuitMessage(0);
			return;
		}
		else
		{
			AfxMessageBox(_T("Successfully create send Socket"), MB_OK | MB_ICONINFORMATION);
			logMessage.AddString(strTime);
			logMessage.AddString(L"UDP Broadcast Socket Create !");
			logMessage.SetCurSel(logMessage.GetCount() - 1);
		}
		m_SocketClient->SetSockOpt(SO_BROADCAST, &broadcast, sizeof(broadcast), SOL_SOCKET);
		memset(&bcast_group, 0, sizeof(bcast_group));
		bcast_group.sin_family = AF_INET;
		bcast_group.sin_addr.S_un.S_addr = inet_addr("255.255.255.255");
		bcast_group.sin_port = htons(strPort);
		m_IpEditBox.SetSel(0, -1);
		m_IpEditBox.Clear();
		GetDlgItem(IDC_IP_EDIT)->EnableWindow(FALSE);
	}

	else if (index == 3)
	{
		int multi_TTL = TTL;

		m_SocketClient->SetWnd(this->m_hWnd);
		if (m_SocketClient->Create(AF_INET, SOCK_DGRAM, 0) < 0)
		{
			AfxMessageBox(_T("※ERROR : Can't create send Socket"), MB_OK | MB_ICONERROR);
			logMessage.AddString(strTime);
			logMessage.AddString(L"※ERROR : Can't create  UDP Multicast Socket");
			logMessage.SetCurSel(logMessage.GetCount() - 1);
			PostQuitMessage(0);
			return;
		}
		else
		{
			AfxMessageBox(_T("Successfully create send Socket"), MB_OK | MB_ICONINFORMATION);
			logMessage.AddString(strTime);
			logMessage.AddString(L"UDP Multicast Socket Create !");
			logMessage.SetCurSel(logMessage.GetCount() - 1);
		}
		m_SocketClient->SetSockOpt(IP_MULTICAST_TTL, &multi_TTL, sizeof(multi_TTL), IPPROTO_IP);
		memset(&mcast_group, 0, sizeof(mcast_group));
		mcast_group.sin_family = AF_INET;
		mcast_group.sin_addr.S_un.S_addr = inet_addr("235.0.0.27");
		mcast_group.sin_port = htons(strPort);
		GetDlgItem(IDC_IP_EDIT)->EnableWindow(FALSE);
	}

	else if (index == 4)
	{
		m_SocketClient->SetWnd(this->m_hWnd);
		if (m_SocketClient->Create(strPort, SOCK_DGRAM, 0) < 0)
		{
			AfxMessageBox(_T("※ERROR : Can't create send Socket"), MB_OK | MB_ICONERROR);
			logMessage.AddString(strTime);
			logMessage.AddString(L"※ERROR : Can't create  UDP Unicast Socket");
			logMessage.SetCurSel(logMessage.GetCount() - 1);
			PostQuitMessage(0);
			return;
		}
		else
		{
			AfxMessageBox(_T("Successfully create send Socket"), MB_OK | MB_ICONINFORMATION);
			logMessage.AddString(strTime);
			logMessage.AddString(L"UDP Unicast Socket Create !");
			logMessage.SetCurSel(logMessage.GetCount() - 1);
		}
	}
}


void CClientDlg::OnBnClickedBtnClose()
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

	GetDlgItem(IDC_BUTTON_OK)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUTTON_CLOSE)->EnableWindow(FALSE);
	GetDlgItem(IDC_IP_EDIT)->EnableWindow(TRUE);
	GetDlgItem(IDC_PORT_EDIT)->EnableWindow(TRUE);

	if (m_SocketClient != NULL) m_SocketClient->Close();
	AfxMessageBox(_T("Successful Socket close"), MB_OK | MB_ICONINFORMATION);
	logMessage.AddString(strTime);
	logMessage.AddString(L"Socket Close !!!");
	logMessage.SetCurSel(logMessage.GetCount() - 1);
}
