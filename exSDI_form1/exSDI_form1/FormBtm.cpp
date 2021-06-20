// FormBtm.cpp: 구현 파일
//

#include "pch.h"
#include "exSDI_form1.h"
#include "FormBtm.h"


// FormBtm

IMPLEMENT_DYNCREATE(FormBtm, CFormView)

FormBtm::FormBtm()
	: CFormView(IDD_BTM_FORM)
{

}

FormBtm::~FormBtm()
{
}

void FormBtm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FormBtm, CFormView)
END_MESSAGE_MAP()


// FormBtm 진단

#ifdef _DEBUG
void FormBtm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormBtm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// FormBtm 메시지 처리기
