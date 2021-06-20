// CTitleForm.cpp: 구현 파일
//

#include "pch.h"
#include "exSDI_form2.h"
#include "CTitleForm.h"


// CTitleForm

IMPLEMENT_DYNCREATE(CTitleForm, CFormView)

CTitleForm::CTitleForm()
	: CFormView(IDD_TITLE_FORM)
{

}

CTitleForm::~CTitleForm()
{
}

void CTitleForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTitleForm, CFormView)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTitleForm 진단

#ifdef _DEBUG
void CTitleForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTitleForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTitleForm 메시지 처리기


void CTitleForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// 스크롤 없애기
	SetScrollSizes(1, CSize(0, 0));

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}


void CTitleForm::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFormView::OnTimer(nIDEvent);
}


void CTitleForm::OnDestroy()
{
	CFormView::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
