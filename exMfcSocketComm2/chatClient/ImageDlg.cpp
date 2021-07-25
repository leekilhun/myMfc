// ImageDlg.cpp: 구현 파일
//

#include "pch.h"
#include "chatClient.h"
#include "ImageDlg.h"
#include "afxdialogex.h"


// CImageDlg 대화 상자

IMPLEMENT_DYNAMIC(CImageDlg, CDialogEx)

CImageDlg::CImageDlg(const CString& file_name, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_IMG, pParent),
	m_fileName(file_name)
{

}

CImageDlg::~CImageDlg()
{
}

void CImageDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_pic);
}


BEGIN_MESSAGE_MAP(CImageDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CImageDlg 메시지 처리기

BOOL CImageDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	return TRUE;
}

void CImageDlg::OnPaint()
{
	CRect rect;
	m_pic.GetClientRect(&rect);
	CDC* pDC = m_pic.GetDC();

	if (!m_img.IsNull())
	{
		::SetStretchBltMode(pDC->m_hDC, HALFTONE);
		m_img.Draw(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, m_img.GetWidth(), m_img.GetHeight());
	}
	else
	{
		wchar_t path[MAX_PATH] = {0};
		GetCurrentDirectory(_MAX_PATH, path);
		CString strpath = path;

		if (m_img.Load(strpath + _T("\\") + m_fileName) == S_OK)
		{
			::SetStretchBltMode(pDC->m_hDC, HALFTONE);
			m_img.Draw(pDC->m_hDC, 0, 0, rect.Width(), rect.Height(), 0, 0, m_img.GetWidth(), m_img.GetHeight());
		}
	}
}
