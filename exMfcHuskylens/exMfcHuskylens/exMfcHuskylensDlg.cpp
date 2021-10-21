
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

#define MAX_BUFF  1024
// CexMfcHuskylensDlg 대화 상자
static CHAR receive_buffer[MAX_BUFF] = {0,};


CexMfcHuskylensDlg::CexMfcHuskylensDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXMFCHUSKYLENS_DIALOG, pParent)
	, m_editTxValue(_T(""))
{
	m_HwSerialComm = nullptr;
	m_IsPortOpen = FALSE;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

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
	DDX_Control(pDX, IDC_EDIT_TX, m_editTx);
	DDX_Text(pDX, IDC_EDIT_TX, m_editTxValue);
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
	ON_BN_CLICKED(IDC_BUTTON2, &CexMfcHuskylensDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT_TX, &CexMfcHuskylensDlg::OnEnChangeEditTx)
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
			//AfxMessageBox(L"connect successed\n");
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
	TCHAR ret_msg[1024] = {};
	BYTE buffer[256] = {0};
	BYTE queue_byte = 0;
	int length = -1;

	int qbuf_size = 0;	// 맴버 버퍼의 문자열의 길이
	char add_str[256] = {0};
	qbuf_size = m_Qbuf.Available();


	int len = 0;
	
	int add_cnt = 0;

	// 포트로 들어와 버퍼에 쌓인 데이터 크기
	length = m_HwSerialComm->m_QueueRead.GetSize();

	// 데이터 파싱
	for (int i = 0; i < length; i++)
	{
		m_HwSerialComm->m_QueueRead.GetByte((byte*)&queue_byte);

		len = strlen((const char*)buffer) + sizeof(char) * 2;
		if (queue_byte == change_line)
		{
			if (m_listViewer.GetCount() == 1024)
				m_listViewer.ResetContent();

			if (qbuf_size > 0)
			{
				m_Qbuf.Read((LPSTR)&add_str, qbuf_size);
				strncat_s(&add_str[0], qbuf_size + add_cnt, (const char*)&buffer[0], add_cnt);
				if (t::CharToLPTSTR((char*)add_str, ret_msg) > 0)
				{
					m_listViewer.AddString(ret_msg);
				}
				//GetDlgItem(IDC_EDIT_RX)->SetWindowText(t::CharToCString((char*)&add_str[0]));
				memset(add_str, 0x00, 256);
			}
			else
			{ 
				if (t::CharToLPTSTR((char*)buffer, ret_msg) > 0) 
				{
					m_listViewer.AddString(ret_msg);
				}
							
				//GetDlgItem(IDC_EDIT_RX)->SetWindowText(t::CharToCString((char*)&buffer[0]));
			}
			m_listViewer.SetTopIndex(m_listViewer.GetCount() - 1);
			memset(buffer, 0x00, 256);
			add_cnt = 0;
		}
		else
			strncat_s((char*)buffer, len + (add_cnt++), (const char*)&queue_byte, 1);

	}

	// 처리하지 않은 문자열을 Q버퍼 저장
	len = strlen((const char*)buffer) + sizeof(char) * 2;
	if (strlen((const char*)buffer) > 0)
	{
		m_Qbuf.Write((const char*)buffer, len);
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
		GetDlgItem(IDC_EDIT_TX)->SetWindowText(str);
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



void CexMfcHuskylensDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CexMfcHuskylensDlg::OnEnChangeEditTx()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString buf;
	GetDlgItemText(IDC_EDIT_TX, buf);




}


BOOL CexMfcHuskylensDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
#if 1
	CString buf;
 	GetDlgItemText(IDC_EDIT_TX, buf);
	CHAR send_msg[1024];
	memset(send_msg, 0x00, sizeof(send_msg));
	int len = t::CStringToLPSTR(buf, send_msg);

	if (pMsg->message == WM_KEYDOWN
		&& pMsg->hwnd == GetDlgItem(IDC_EDIT_TX)->m_hWnd
		&& pMsg->wParam == VK_RETURN
		)
	{
		send_msg[strlen(send_msg)] = '\r';
		m_HwSerialComm->WriteComm(send_msg, strlen(send_msg));
		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_TX);
		edit->SetSel(0, -1, TRUE);
		edit->Clear();
	}
#endif // 1



	//if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	//{
	//	if (GetDlgItem(IDC_EDIT_TX) == GetFocus())
	//	{
	//		//에디트 컨트롤 엔터 누를시 개행.
	//		CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT_TX);
	//		edit->ReplaceSel(_T("\r\n"));
	//	}
	//}


	return FALSE;//CDialogEx::PreTranslateMessage(pMsg);
}
