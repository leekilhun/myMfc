// CAutoMainForm.cpp: 구현 파일
//

#include "pch.h"
#include "exSDI_form2.h"
#include "CAutoMainForm.h"


// CAutoMainForm

IMPLEMENT_DYNCREATE(CAutoMainForm, CFormView)

CAutoMainForm::CAutoMainForm()
	: CFormView(IDD_AUTO_MAIN_FORM)
{

}

CAutoMainForm::~CAutoMainForm()
{
}

void CAutoMainForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAutoMainForm, CFormView)
END_MESSAGE_MAP()


// CAutoMainForm 진단

#ifdef _DEBUG
void CAutoMainForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CAutoMainForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CAutoMainForm 메시지 처리기
