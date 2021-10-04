
// exMfcHuskylensDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "exMfcHuskylens.h"
#include "exMfcHuskylensDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexMfcHuskylensDlg 대화 상자



CexMfcHuskylensDlg::CexMfcHuskylensDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXMFCHUSKYLENS_DIALOG, pParent)
{
	m_HwSerialComm = nullptr;
	m_IsPortOpen = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	memset(m_buff, 0x00, DEF_BUFF_MAX);

}

CexMfcHuskylensDlg::~CexMfcHuskylensDlg()
{
	if (m_HwSerialComm != nullptr)
	{
		delete m_HwSerialComm;
		m_HwSerialComm = nullptr;
	}
}

void CexMfcHuskylensDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_comPort);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_comBaud);
	DDX_Control(pDX, IDC_BUTTON_OPENCLOSE, m_btnOpenClose);
	DDX_Control(pDX, IDC_LIST_CONT, m_listViewer);
	DDX_Control(pDX, IDC_EDIT_RX, m_editRx);
}



BEGIN_MESSAGE_MAP(CexMfcHuskylensDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_OPENCLOSE, &CexMfcHuskylensDlg::OnBnClickedButtonOpenclose)
	ON_WM_SHOWWINDOW()

	ON_MESSAGE(WM_COMM_RS232_MESSAGE, &CexMfcHuskylensDlg::OnUserMessage)

	ON_MESSAGE(UWM_USER_MSG, &CexMfcHuskylensDlg::OnReceiveTest)
	ON_BN_CLICKED(IDC_BUTTON1, &CexMfcHuskylensDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CexMfcHuskylensDlg 메시지 처리기

void CexMfcHuskylensDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

BOOL CexMfcHuskylensDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	//ShowWindow(SW_MAXIMIZE);

	//ShowWindow(SW_MINIMIZE);

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_HwSerialComm = new CCommRs232();
	


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CexMfcHuskylensDlg::OnPaint()
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
HCURSOR CexMfcHuskylensDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}






void CexMfcHuskylensDlg::OnBnClickedButtonOpenclose()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_IsPortOpen == FALSE)
	{
		CString port_name;//_T("COM11");
		m_comPort.GetLBText(m_comPort.GetCurSel(), port_name.GetBuffer(1024));
		port_name.ReleaseBuffer();

		DWORD baudrate = m_comBaud.GetItemData(m_comBaud.GetCurSel());
		WORD port_id = m_comPort.GetCurSel();


		CWnd* pWnd = AfxGetMainWnd();
		HWND hCommWnd = pWnd->m_hWnd;

		if (!m_HwSerialComm->OpenPort(port_name, baudrate, port_id, hCommWnd))
			AfxMessageBox(L"connect faliled");
		else
		{
			AfxMessageBox(L"connect successed\n");
			m_IsPortOpen = TRUE;
			GetDlgItem(IDC_BUTTON_OPENCLOSE)->SetWindowText(_T("CLOSE"));
		}
	}
	else
	{
		m_HwSerialComm->ClosePort();
		m_IsPortOpen = FALSE;
		GetDlgItem(IDC_BUTTON_OPENCLOSE)->SetWindowText(_T("OPEN"));
	}

	
		
	
}


void CexMfcHuskylensDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_HwSerialComm->AddPortList(&m_comPort);
	m_HwSerialComm->AddBaudrateList(&m_comBaud);
}


afx_msg LRESULT CexMfcHuskylensDlg::OnUserMessage(WPARAM wParam, LPARAM lParam)
{




	LPCTSTR return_w = _T("\r\n");
	BYTE carrige_return = _T('\r');
	BYTE change_line = _T('\n');
	enum class state
	{
		start,
		wait,
		clear,
	} step;

	BYTE buffer = {};
	CString message;
	int length = -1;

	length = m_HwSerialComm->m_QueueRead.GetSize();

	for (int i = 0; i < length; i++)
	{
		m_HwSerialComm->m_QueueRead.GetByte(&buffer);
		//message += buffer;
		if (buffer == carrige_return)
		{

		}
		else if (buffer == change_line)
		{
			m_buff;
		}
		else
		{			
			_tcscat_s((TCHAR*)m_buff, _tcslen((TCHAR*)m_buff)*2, (TCHAR*)buffer);
			
			//strncat_s((char*)m_buff, 2, (char*)buffer, DEF_BUFF_MAX);
			//strncat_s((char*)m_buff, (char*)buffer, 1);
		}
		
	}

	//TRACE(traceAppMsg, 0, "데이터를 출력합니다.\r\n");
	if (message == "") {}
	else
	{
		GetDlgItem(IDC_EDIT_RX)->SetWindowText(message);
	}

	return 0;
}


afx_msg LRESULT CexMfcHuskylensDlg::OnReceive(WPARAM length, LPARAM lParam)
{
	CString str;
	BYTE data[2000];
	if (m_HwSerialComm)
	{
		m_HwSerialComm->ReadComm(data, length);
		data[length] = _T('\0');
		str += _T("\r\n");
		for (int i = 0; i < (int)length; i++)
		{
			str += data[i];
		}
		GetDlgItem(IDC_EDIT_RX)->SetWindowText(str);
		str.Empty();

	}
	return 0;
}

LRESULT CexMfcHuskylensDlg::OnReceiveTest(WPARAM wParam, LPARAM lParam)
{
	CString* msg = (CString*)wParam;
	AfxMessageBox(L"Call");
	return LRESULT();
}


CString CexMfcHuskylensDlg::GetMiddleString(CString str, int len)
{
	if (str.IsEmpty())
	{

		return _T("");
	}

	CString result = _T("");
	INT _nStx = str.Find(0x02, 0);
	INT _nEtx = str.Find(0x03, 0);

	result = str.Mid(_nStx + 1, (_nEtx - _nStx - 1));
	return result;

}

void CexMfcHuskylensDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_HwSerialComm->MsgTest(GetSafeHwnd());
	
}

void CexMfcHuskylensDlg::clearBuff()
{
	memset(m_buff, 0x00, DEF_BUFF_MAX);
}
