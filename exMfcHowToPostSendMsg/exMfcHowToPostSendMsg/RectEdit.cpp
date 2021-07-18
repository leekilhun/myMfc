// RectEdit.cpp: 구현 파일
//

#include "pch.h"
#include "exMfcHowToPostSendMsg.h"
#include "RectEdit.h"


// CRectEdit

IMPLEMENT_DYNAMIC(CRectEdit, CEdit)

CRectEdit::CRectEdit()
{

}

CRectEdit::~CRectEdit()
{
}

void CRectEdit::Show(CPoint point, const CRect& rect)//const Info& info)
{


	//CString text;
	//text.Format(_T("%s: %d, %d"), info.text, info.mousePoint.x, info.mousePoint.y);

	//SetWindowTextW(text);

	//MoveWindow(info.rectWindow);
	//ShowWindow(SW_SHOW);

}


BEGIN_MESSAGE_MAP(CRectEdit, CEdit)
	ON_MESSAGE(WM_SHOWINFO, OnShowInfo)
END_MESSAGE_MAP()


LRESULT CRectEdit::OnShowInfo(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	Info* info { reinterpret_cast<Info*>(lParam) };

	//Show(CPoint(wParam, lParam), {10,10,100,80});
	// 
	//Show(*info);

	delete info;

	return (LRESULT)0;
}

// CRectEdit 메시지 처리기


