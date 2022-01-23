// tab_main.cpp: 구현 파일
//

#include "pch.h"
#include "fastechModule.h"
#include "tab_main.h"
#include "afxdialogex.h"


// tab_main 대화 상자

IMPLEMENT_DYNAMIC(tab_main, CDialogEx)

tab_main::tab_main(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_MAIN, pParent)
{
	m_pDlgPop = nullptr;
}

tab_main::~tab_main()
{
	if (m_pDlgPop != nullptr)
		delete 	m_pDlgPop;
	m_pDlgPop = nullptr;
}

void tab_main::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(tab_main, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &tab_main::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &tab_main::OnBnClickedButton2)
END_MESSAGE_MAP()


// tab_main 메시지 처리기


void tab_main::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_pDlgPop == nullptr)
	{
		m_pDlgPop = new popMsg(this);
		m_pDlgPop->Create(IDD_DIALOG_POP, this);
		m_pDlgPop->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDlgPop->ShowWindow(SW_SHOW);
	}

}


void tab_main::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	popMsg  modalless;
	modalless.DoModal();
}
