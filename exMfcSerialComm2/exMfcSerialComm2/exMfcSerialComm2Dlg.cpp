
// exMfcSerialComm2Dlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "exMfcSerialComm2.h"
#include "exMfcSerialComm2Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "serialcomm.h"

#define BUFF_SIZE 256
#define BAUDRATE_DROPBOX_MAX  12
// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

struct { DWORD baudrate_data; TCHAR label[32];} BAUDRATE_DROPBOX_DATA[BAUDRATE_DROPBOX_MAX] =
{
	{CBR_110,  TEXT("110")},
	{CBR_300,  TEXT("300")},
	{CBR_600,  TEXT("600")},
	{CBR_1200,  TEXT("1200")},
	{CBR_2400,  TEXT("2400")},
	{CBR_4800,  TEXT("4800")},
	{CBR_9600,  TEXT("9600")},
	{CBR_14400,  TEXT("14400")},
	{CBR_19200,  TEXT("19200")},
	{CBR_38400,  TEXT("38400")},
	{CBR_56000,  TEXT("56000")},
	{CBR_115200,  TEXT("115200")},

};


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
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CexMfcSerialComm2Dlg 대화 상자



CexMfcSerialComm2Dlg::CexMfcSerialComm2Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXMFCSERIALCOMM2_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CexMfcSerialComm2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MFCBUTTON1, m_btnConnect);
	DDX_Control(pDX, IDC_COMBO1, m_DropList);
	DDX_Control(pDX, IDC_COMBO2, m_DropBaudrate);
	DDX_Control(pDX, IDC_STATIC_INFO, m_infoText);
}

BEGIN_MESSAGE_MAP(CexMfcSerialComm2Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MFCBUTTON1, &CexMfcSerialComm2Dlg::OnBnClickedMfcbutton1)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CexMfcSerialComm2Dlg::OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CexMfcSerialComm2Dlg 메시지 처리기

BOOL CexMfcSerialComm2Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	getSerialPort();

	//char port_s[] = "COM7";
	//openPort(port_s, 7);

	initComponents();


	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CexMfcSerialComm2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CexMfcSerialComm2Dlg::OnPaint()
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
HCURSOR CexMfcSerialComm2Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CexMfcSerialComm2Dlg::OnBnClickedMfcbutton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString port_name;
	int nIndex= m_DropList.GetCurSel();
	
	m_DropList.GetLBText(nIndex, port_name);	

	wchar_t* wchar_str;     //첫번째 단계(CString to wchar_t*)를 위한 변수
	char* char_str;			//char* 형의 변수
	int      char_str_len;  //char* 형 변수의 길이를 위한 변수
	//1. CString to wchar_t* conversion
	wchar_str = port_name.GetBuffer(port_name.GetLength());

	//2. wchar_t* to char* conversion
	//char* 형에 대한길이를 구함
	char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
	char_str = new char[char_str_len];  //메모리 할당
	//wchar_t* to char* conversion
	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, char_str, char_str_len, 0, 0);


	openPort(char_str, 7);
}


int CexMfcSerialComm2Dlg::openPort(char* portname, int no)
{
	CSerialComm serialComm;

	BYTE buff[BUFF_SIZE] = {0,};

	int op = 0;
	int size = 0;
	char port_s[20] = "";



	sprintf_s(port_s, "COM%d", no);

	// STEP 1. SerialPort Connect
	if (!serialComm.connect(portname/*port_s*/))
	{
		//AfxMessageBox(L"connect faliled");
		return -1;
	}
	else
	{
		AfxMessageBox(L"connect successed\n");
	}


	serialComm.disconnect();

	return 0;
}


void CexMfcSerialComm2Dlg::getSerialPort()
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
	m_DropList.ResetContent();
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
		m_DropList.AddString(CString(szData));

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;
	}

	RegCloseKey(hKey);



}


int CexMfcSerialComm2Dlg::initComponents()
{
	// TODO: 여기에 구현 코드 추가.
	for (int i = 0; i < BAUDRATE_DROPBOX_MAX; i++)
	{
		m_DropBaudrate.AddString(BAUDRATE_DROPBOX_DATA[i].label);
		m_DropBaudrate.SetItemData(i, BAUDRATE_DROPBOX_DATA[i].baudrate_data);
	}
	m_DropBaudrate.SetCurSel(1);
	

	return 0;
}


void CexMfcSerialComm2Dlg::OnCbnSelchangeCombo2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	TCHAR szTemp1[512];
	TCHAR szTemp2[512];

	int index;
	index = this->m_DropBaudrate.GetCurSel();
	this->m_DropBaudrate.GetWindowTextW(szTemp1, sizeof(szTemp1) / sizeof(TCHAR));
	//index = this->m_DropBaudrate.FindStringExact(-1, szTemp1);

	if ((index >= 0) && (index != CB_ERR))
	{
		if (m_DropBaudrate.GetLBText(index, szTemp1) > 0)
		{
			wsprintf(szTemp2, TEXT("'%s'의 정보는 '%d'입니다"), szTemp1, (LPCSTR)m_DropBaudrate.GetItemData(index));
		}
	}
	else
	{
		wsprintf(szTemp2, TEXT("'%s'의 정보를 모릅니다"), szTemp1);
	}
	this->m_infoText.SetWindowTextW(szTemp2);
}
