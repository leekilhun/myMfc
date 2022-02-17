
// chatClientDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "chatClient.h"
#include "chatClientDlg.h"
#include "afxdialogex.h"
#include "Client.h"

#include <string>
#include <fstream>
#include "ImageDlg.h"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CchatClientDlg 대화 상자



CchatClientDlg::CchatClientDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CHATCLIENT_DIALOG, pParent)
	, m_port(0)
	, m_sendMsg(_T(""))
	, m_pClient(nullptr)
	, m_nick(_T(""))
	, m_imgType(IMAGE_TYPE::_BMP)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CchatClientDlg::~CchatClientDlg()
{
	closeClient();
}

void CchatClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS, m_ip);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDV_MinMaxInt(pDX, m_port, 1024, 65535);
	DDX_Control(pDX, IDC_LIST_RECV, m_listRecv);
	DDX_Text(pDX, IDC_EDIT_SEND, m_sendMsg);
	DDX_Text(pDX, IDC_EDIT_NICK, m_nick);
	DDV_MaxChars(pDX, m_nick, 128);

}

BEGIN_MESSAGE_MAP(CchatClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_LBN_DBLCLK(IDC_LIST_RECV, &CchatClientDlg::OnLbnDblclkListRecv)
	ON_BN_CLICKED(IDC_BTN_FILESEND, &CchatClientDlg::OnBnClickedBtnFilesend)
	ON_BN_CLICKED(IDC_BTN_SEND, &CchatClientDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CchatClientDlg::OnBnClickedBtnClear)
	ON_BN_CLICKED(IDC_BTN_DISCONNECT, &CchatClientDlg::OnBnClickedBtnDisconnect)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CchatClientDlg::OnBnClickedBtnConnect)
	ON_MESSAGE(UM_DISCONNECT_SERVER, &CchatClientDlg::OnDisconnectServer)
	ON_MESSAGE(UM_RECV_TEXT, &CchatClientDlg::OnRecevieText)
	ON_MESSAGE(UM_RECV_IMAGE, &CchatClientDlg::OnRecevieImage)
	ON_MESSAGE(UM_RECV_FILE, &CchatClientDlg::OnRecevieFile)
	ON_MESSAGE(UM_RECV_IMAGE_NAME, &CchatClientDlg::OnRecevieImageName)
	ON_MESSAGE(UM_RECV_FILE_NAME, &CchatClientDlg::OnRecevieFileName)
END_MESSAGE_MAP()


// CchatClientDlg 메시지 처리기

BOOL CchatClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	// ip init
	std::wstring ip = Client::getMyip();
	m_ip.SetWindowText(ip.c_str());
	m_port = 7000;
	m_nick = _T("익명");

	UpdateData(false);

	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(false);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CchatClientDlg::OnPaint()
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
HCURSOR CchatClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CchatClientDlg::OnLbnDblclkListRecv()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	int index = m_listRecv.GetCurSel();
	DATA_TYPE type = static_cast<DATA_TYPE>(m_listRecv.GetItemData(index));

	CString str;
	m_listRecv.GetText(index, str);

	switch (type)
	{
	case DATA_TYPE::_IMAGE_NAME:
	{
		CString file_name, file_ext;
		getFileNameAndExt(str, file_name, file_ext);
		CImageDlg dlg(file_name);
		dlg.DoModal();
	}
	break;
	case DATA_TYPE::_FILE_NAME:
	{
		CString file_name, file_ext;
		getFileNameAndExt(str, file_name, file_ext);
		::ShellExecute(NULL, _T("open"), file_name, NULL, NULL, SW_SHOW);
	}
	break;
	default:
		return;
	}
}


void CchatClientDlg::OnBnClickedBtnFilesend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pClient == nullptr)
		return;

	static TCHAR BASED_CODE szFilter[] = _T(" Image Files (*.bmp;*.jpg;*.png) |*.bmp;*.jpg;*.png| All Files(*.*)|*.*||");

	CFileDialog dlg(true, nullptr, nullptr, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, this);

	if (dlg.DoModal() == IDOK)
	{
		CString file_name = dlg.GetFileName();
		CString file_path = dlg.GetPathName();
		CString file_ext = dlg.GetFileExt();
		file_ext = file_ext.MakeUpper();

		DATA_TYPE type = DATA_TYPE::_UNKNOWN;
		if (file_ext == _T("BMP") || file_ext == _T("JPG") || file_ext == _T("PNG"))
			type = DATA_TYPE::_IMAGE;
		else
			type = DATA_TYPE::_FILE;

		m_pClient->sendFile(file_name.operator LPCWSTR(), file_path.operator LPCWSTR(), file_ext.operator LPCWSTR(), type);
	}
}


void CchatClientDlg::OnBnClickedBtnSend()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pClient)
	{
		UpdateData(true);
		if (m_sendMsg.GetLength() > 0)
		{
			m_pClient->sendText(m_sendMsg.operator LPCWSTR());
			m_sendMsg = _T("");
			UpdateData(false);

			GetDlgItem(IDC_EDIT_SEND)->SetFocus();
		}
	}
}


void CchatClientDlg::OnBnClickedBtnClear()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_listRecv.ResetContent();
}


void CchatClientDlg::OnBnClickedBtnDisconnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	closeClient();
	GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(true);
	GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_NICK)->EnableWindow(true);
}


void CchatClientDlg::OnBnClickedBtnConnect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pClient == nullptr)
	{
		UpdateData(true);

		CString ip;
		m_ip.GetWindowTextW(ip);

		if (m_pClient == nullptr)
		{
			m_pClient = new Client(this);
			if (m_pClient->connectServer(ip.operator LPCWSTR(), m_port))
			{
				GetDlgItem(IDC_BTN_CONNECT)->EnableWindow(false);
				GetDlgItem(IDC_BTN_DISCONNECT)->EnableWindow(true);
				GetDlgItem(IDC_EDIT_NICK)->EnableWindow(false);

				m_pClient->sendNick(m_nick.operator LPCWSTR());
			}
			else
			{
				CString str;
				str.Format(_T("Connect Error : %d"), WSAGetLastError());
				MessageBox(str, _T("Error"), MB_OK);
				closeClient();
			}
		}
	}
}

void CchatClientDlg::closeClient()
{
	if (m_pClient)
	{
		m_pClient->disconnectServer();
		delete m_pClient;
		m_pClient = nullptr;
	}
}

void CchatClientDlg::addListMsg(const CString& msg, const DATA_TYPE& type)
{
	int index = m_listRecv.AddString(msg);
	m_listRecv.SetItemData(index, static_cast<DWORD_PTR>(type));
	m_listRecv.SetTopIndex(m_listRecv.GetCount() - 1);
}

void CchatClientDlg::getFileNameAndExt(const CString& str, CString& file_name, CString& file_ext)
{
	// parsing file name
	CString token;
	token.Format(L"[%s] ", m_nick.GetBuffer());
	CString msg(str);
	msg.Replace(token, _T(""));
	file_name = file_ext = msg;

	// parsing file ext
	int index = m_fileExt.ReverseFind('.');
	file_ext = m_fileExt.Mid(++index);
	file_ext = m_fileExt.MakeUpper();
}

LRESULT CchatClientDlg::OnDisconnectServer(WPARAM wp, LPARAM lp)
{
	OnBnClickedBtnDisconnect();
	return 0;
}

LRESULT CchatClientDlg::OnRecevieText(WPARAM wp, LPARAM lp)
{
	wstring wstr;
	
	//= Client::MultibyteToUnicode(CP_UTF8, (char*)wp, (int)lp);
	trans::CharToLPTSTR((LPSTR)wp, (LPTSTR)wstr.c_str(), (int)wstr.capacity());
	CString cstr(wstr.c_str());
	addListMsg(cstr);
	return LRESULT();
}

LRESULT CchatClientDlg::OnRecevieImage(WPARAM wp, LPARAM lp)
{
	const char* pImg = reinterpret_cast<const char*>(wp);
	size_t img_size = static_cast<size_t>(lp);

	HGLOBAL hg = ::GlobalAlloc(GMEM_MOVEABLE, img_size);
	if (hg != nullptr)
	{
		LPVOID pBuf = ::GlobalLock(hg);
		if (pBuf)
			memcpy(pBuf, pImg, img_size);
		::GlobalUnlock(hg);

		IStream* pStream = NULL;
		if (::CreateStreamOnHGlobal(hg, false, &pStream) == S_OK)
		{
			CImage img;
			HRESULT result;

			if (!img.IsNull())
				img.Destroy();

			result = img.Load(pStream);

			switch (m_imgType)
			{
			case IMAGE_TYPE::_BMP:
				result = img.Save(m_fileName, Gdiplus::ImageFormatBMP);
				break;
			case IMAGE_TYPE::_JPG:
				result = img.Save(m_fileName, Gdiplus::ImageFormatJPEG);
				break;
			case IMAGE_TYPE::_PNG:
				result = img.Save(m_fileName, Gdiplus::ImageFormatPNG);
				break;
			case IMAGE_TYPE::_GIF:
				result = img.Save(m_fileName, Gdiplus::ImageFormatGIF);
				break;
			}

			pStream->Release();
			::GlobalFree(hg);
			return true;
		}
		::GlobalFree(hg);
		return false;
	}
	::GlobalFree(hg);

	/*CImage img2;
	int img_w = 1468;
	int img_h = 733;
	int img_bpp = 24;

	img2.Create(img_w, img_h, img_bpp, 0);
	int i = 0;
	for (unsigned int y = 0; y < img_h; y++)
	{
		for (unsigned int x = 0; x < img_w; x++)
		{
			unsigned char r = pImg[i++];
			unsigned char g = pImg[i++];
			unsigned char b = pImg[i++];
			img2.SetPixel(x, y, RGB(r, g, b));
		}
	}

	img2.Save(_T("test.bmp"), Gdiplus::ImageFormatBMP);*/
	return LRESULT();
}

LRESULT CchatClientDlg::OnRecevieFile(WPARAM wp, LPARAM lp)
{
	const char* pFile = reinterpret_cast<const char*>(wp);
	size_t file_size = static_cast<size_t>(lp);

	ofstream file(m_fileName, ios::binary | ios::out);
	file.write(pFile, file_size);
	file.close();
	return LRESULT();
}

LRESULT CchatClientDlg::OnRecevieImageName(WPARAM wp, LPARAM lp)
{
	wstring wstr;// = Client::MultibyteToUnicode(CP_UTF8, (char*)wp, (int)lp);
	trans::CharToLPTSTR((LPSTR)wp, (LPTSTR)wstr.c_str(), (int)wstr.capacity());
	CString cstr(wstr.c_str());
	addListMsg(cstr, DATA_TYPE::_IMAGE_NAME);

	// get file name and ext from recv msg
	getFileNameAndExt(cstr, m_fileName, m_fileExt);
	// img type
	if (m_fileExt == _T("BMP"))
		m_imgType = IMAGE_TYPE::_BMP;
	else if (m_fileExt == _T("JPG"))
		m_imgType = IMAGE_TYPE::_JPG;
	else if (m_fileExt == _T("PNG"))
		m_imgType = IMAGE_TYPE::_PNG;
	else if (m_fileExt == _T("GIF"))
		m_imgType = IMAGE_TYPE::_GIF;
	return LRESULT();
}

LRESULT CchatClientDlg::OnRecevieFileName(WPARAM wp, LPARAM lp)
{
	wstring wstr;// = Client::MultibyteToUnicode(CP_UTF8, (char*)wp, (int)lp);
	trans::CharToLPTSTR((LPSTR)wp, (LPTSTR)wstr.c_str(), (int)wstr.capacity());
	CString cstr(wstr.c_str());
	addListMsg(cstr, DATA_TYPE::_FILE_NAME);

	// get file name and ext from recv msg
	getFileNameAndExt(cstr, m_fileName, m_fileExt);

	return LRESULT();
}
