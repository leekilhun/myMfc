// popDash.cpp: 구현 파일
//

#include "pch.h"
#include "exMultiThread.h"
#include "popDash.h"
#include "afxdialogex.h"


// CpopDash 대화 상자

IMPLEMENT_DYNAMIC(CpopDash, CDialogEx)

CpopDash::CpopDash(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FORMVIEW, pParent)
{

}

CpopDash::~CpopDash()
{
}

void CpopDash::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CpopDash, CDialogEx)
END_MESSAGE_MAP()


// CpopDash 메시지 처리기
