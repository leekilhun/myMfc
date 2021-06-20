// CBottomForm.cpp: 구현 파일
//

#include "pch.h"
#include "exSDI_form2.h"
#include "CBottomForm.h"


// CBottomForm

IMPLEMENT_DYNCREATE(CBottomForm, CFormView)

CBottomForm::CBottomForm()
	: CFormView(IDD_BOTTOM_FORM)
{

}

CBottomForm::~CBottomForm()
{
}

void CBottomForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBottomForm, CFormView)
END_MESSAGE_MAP()


// CBottomForm 진단

#ifdef _DEBUG
void CBottomForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CBottomForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBottomForm 메시지 처리기
