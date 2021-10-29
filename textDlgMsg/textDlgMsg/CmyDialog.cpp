// CmyDialog.cpp: 구현 파일
//

#include "pch.h"
#include "textDlgMsg.h"
#include "CmyDialog.h"
#include "afxdialogex.h"


// CmyDialog 대화 상자

IMPLEMENT_DYNAMIC(CmyDialog, CDialogEx)

CmyDialog::CmyDialog(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MYDIALOG, pParent)
{
	Create(IDD_MYDIALOG); // 추가 함
}

CmyDialog::~CmyDialog()
{
}

void CmyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_progressWT);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	DDX_Control(pDX, IDC_STAT_WT, m_strWT);
}


BEGIN_MESSAGE_MAP(CmyDialog, CDialogEx)
END_MESSAGE_MAP()


// CmyDialog 메시지 처리기


BOOL CmyDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_progressWT.SetRange(0, 1000);



	return TRUE;  // return TRUE unless you set the focus to a control
								// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
