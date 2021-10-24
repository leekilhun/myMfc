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
END_MESSAGE_MAP()


// CpopMsg 메시지 처리기
