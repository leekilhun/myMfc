
// exMfcRs232BuffDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "exMfcRs232Buff.h"
#include "exMfcRs232BuffDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexMfcRs232BuffDlg 대화 상자



CexMfcRs232BuffDlg::CexMfcRs232BuffDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_EXMFCRS232BUFF_DIALOG, pParent)
	, m_str_comport(_T(""))
	, m_str_baudrate(_T(""))
	, comport_state(FALSE)
	, m_comm(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CexMfcRs232BuffDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_PORT, m_combo_comport_list);
	DDX_CBString(pDX, IDC_COMBO_PORT, m_str_comport);
	DDX_Control(pDX, IDC_COMBO_BAUD, m_combo_baudrate_list);
	DDX_CBString(pDX, IDC_COMBO_BAUD, m_str_baudrate);
	DDX_Control(pDX, IDC_EDIT_RCV_VIEW, m_edit_rcv_view);
	DDX_Control(pDX, IDC_EDIT_SEND_DATA, m_edit_send_data);
}

BEGIN_MESSAGE_MAP(CexMfcRs232BuffDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_MYCLOSE,&CexMfcRs232BuffDlg::OnThreadClosed)
	ON_MESSAGE(WM_MYRECEIVE, &CexMfcRs232BuffDlg::OnReceive)
	ON_BN_CLICKED(IDC_BT_CONNECT, &CexMfcRs232BuffDlg::OnBnClickedBtConnect)
	ON_BN_CLICKED(IDC_BT_CLEAR, &CexMfcRs232BuffDlg::OnBnClickedBtClear)
	ON_BN_CLICKED(IDC_BT_SEND, &CexMfcRs232BuffDlg::OnBnClickedBtSend)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CexMfcRs232BuffDlg 메시지 처리기

BOOL CexMfcRs232BuffDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	m_combo_comport_list.AddString(L"COM11");
	m_combo_baudrate_list.AddString(L"115200");

	comport_state = FALSE;
	GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
	m_str_comport = _T("COM11");
	m_str_baudrate = _T("115200");

	UpdateData(FALSE);

	m_comm = new CMycomm(GetSafeHwnd());

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CexMfcRs232BuffDlg::OnPaint()
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
HCURSOR CexMfcRs232BuffDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CexMfcRs232BuffDlg::OnThreadClosed(WPARAM length, LPARAM lpara)
{
	//overlapped i/o 핸들을 닫는다.
	((CMycomm*)lpara)->HandleClose();
	//delete ((CMycomm*)lpara);

	return 0;
}

LRESULT CexMfcRs232BuffDlg::OnReceive(WPARAM length, LPARAM lpara)
{
	CString str;
	char data[10000];
	if (m_comm)
	{
		m_comm->Receive(data, length);
		data[length] = _T('\0');
		str += _T("\r\n");
		for (int i = 0; i < (int)length; i++)
		{
			str += data[i];
		}
		m_edit_rcv_view.ReplaceSel(str);
		//GetDlgItem(IDC_EDIT_RCV_VIEW)->SetWindowText(str);
		str.Empty();
		m_edit_rcv_view.LineScroll(m_edit_rcv_view.GetLineCount());
	}
	return 0;
}



void CexMfcRs232BuffDlg::OnBnClickedBtConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (comport_state)
	{
		if (m_comm)
		{
			m_comm->Close();
			//m_comm = NULL;
			AfxMessageBox(_T("COM포트 닫힘"));
			comport_state = FALSE;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("OPEN"));
			GetDlgItem(IDC_BT_SEND)->EnableWindow(FALSE);
		}
	}
	else
	{
		//  _T("\\\\.\\")     L"//./"
		CString port_name;//_T("COM11");
		m_combo_comport_list.GetLBText(m_combo_comport_list.GetCurSel(), port_name.GetBuffer(1024));
		port_name.ReleaseBuffer();
		CString test = m_str_comport;

		DWORD baudrate = m_combo_baudrate_list.GetItemData(m_combo_baudrate_list.GetCurSel());
		test = m_str_baudrate;

		m_str_comport = _T("COM11");
		m_str_baudrate = _T("115200");


		if (m_comm->Open(m_str_comport, CBR_115200) != 0)
		{
			AfxMessageBox(_T("COM포트 열림"));
			comport_state = TRUE;
			GetDlgItem(IDC_BT_CONNECT)->SetWindowText(_T("CLOSE"));
			GetDlgItem(IDC_BT_SEND)->EnableWindow(TRUE);
		}
		else
		{
			AfxMessageBox(_T("ERROR!"));
		}
	}

}


void CexMfcRs232BuffDlg::OnBnClickedBtClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	GetDlgItem(IDC_EDIT_RCV_VIEW)->SetWindowText(_T(" "));
}


void CexMfcRs232BuffDlg::OnBnClickedBtSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	GetDlgItem(IDC_EDIT_SEND_DATA)->GetWindowText(str);
	str += "\r\n";
	m_comm->Send(str, str.GetLength());
}


void CexMfcRs232BuffDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_comm->AddPortList(&m_combo_comport_list);
	m_comm->AddBaudrateList(&m_combo_baudrate_list);
}
