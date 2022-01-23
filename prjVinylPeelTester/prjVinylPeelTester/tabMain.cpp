// tabMain.cpp: 구현 파일
//

#include "pch.h"
#include "prjVinylPeelTester.h"
#include "tabMain.h"
#include "afxdialogex.h"


// tabMain 대화 상자

IMPLEMENT_DYNAMIC(tabMain, CDialogEx)

tabMain::tabMain(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_MAIN, pParent)
{

}

tabMain::~tabMain()
{
}

void tabMain::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(tabMain, CDialogEx)
END_MESSAGE_MAP()


// tabMain 메시지 처리기
