// popMsg.cpp: 구현 파일
//

#include "pch.h"
#include "textDlgMsg.h"
#include "popMsg.h"
#include "afxdialogex.h"


#include "textDlgMsgDlg.h"


// CpopMsg 대화 상자

IMPLEMENT_DYNAMIC(CpopMsg, CDialogEx)

CpopMsg::CpopMsg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CHILD1, pParent)
{
	m_pDlg = nullptr;
}

CpopMsg::~CpopMsg()
{
}

void CpopMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHILD_EDIT, m_msg);
}

void CpopMsg::setter(CtextDlgMsgDlg* parent)
{
	m_pDlg = parent;
}


BEGIN_MESSAGE_MAP(CpopMsg, CDialogEx)
	ON_MESSAGE(WM_POP_RECEIVE, &CpopMsg::OnPopReceive)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CpopMsg 메시지 처리기


afx_msg LRESULT CpopMsg::OnPopReceive(WPARAM wParam, LPARAM lParam)
{
	int no = (int)wParam;
	CString* msg;
	msg = ((CString*)lParam);
	CString result = msg->GetString();

	CString pre_str;
	if (m_msg.GetLineCount() < 10)
		GetDlgItemText(IDC_CHILD_EDIT, pre_str);

	m_msg.SetWindowText(pre_str+result+ L"\r\n");

	return 0;
}


BOOL CpopMsg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	if (m_pDlg != nullptr) 
	{
		m_pDlg->m_thread->SetHwnd(this->GetSafeHwnd());

		//m_pDlg->m_thread->ThreadRun();
	}
	


	return TRUE;  // return TRUE unless you set the focus to a control
								// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CpopMsg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

}
