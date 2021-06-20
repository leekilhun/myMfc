// FormTop.cpp: 구현 파일
//

#include "pch.h"
#include "exSDI_form1.h"
#include "FormTop.h"


// FormTop

IMPLEMENT_DYNCREATE(FormTop, CFormView)

FormTop::FormTop()
	: CFormView(IDD_TOP_FORM)
{

}

FormTop::~FormTop()
{
}

void FormTop::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(FormTop, CFormView)
END_MESSAGE_MAP()


// FormTop 진단

#ifdef _DEBUG
void FormTop::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void FormTop::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// FormTop 메시지 처리기
