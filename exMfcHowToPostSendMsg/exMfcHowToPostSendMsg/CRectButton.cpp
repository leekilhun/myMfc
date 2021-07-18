// CRectButton.cpp: 구현 파일
//

#include "pch.h"
#include "exMfcHowToPostSendMsg.h"
#include "CRectButton.h"


// CRectButton

IMPLEMENT_DYNAMIC(CRectButton, CButton)

CRectButton::CRectButton()
{

}

CRectButton::~CRectButton()
{
}

void CRectButton::Show(CPoint point, const CRect& rect)
{
	CString text;
	text.Format(_T("%d, %d"), point.x, point.y);

	SetWindowTextW(text);

	MoveWindow(rect);
	ShowWindow(SW_SHOW);

}


BEGIN_MESSAGE_MAP(CRectButton, CButton)
END_MESSAGE_MAP()



// CRectButton 메시지 처리기


