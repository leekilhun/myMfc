// popMsg.cpp: 구현 파일
//

#include "pch.h"
#include "fastechModule.h"
#include "popMsg.h"
#include "afxdialogex.h"


// popMsg 대화 상자

IMPLEMENT_DYNAMIC(popMsg, CDialogEx)

popMsg::popMsg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_POP, pParent)
{

}

popMsg::~popMsg()
{
}

void popMsg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(popMsg, CDialogEx)
END_MESSAGE_MAP()


// popMsg 메시지 처리기
