
// exSDI_form2View.cpp: CexSDIform2View 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "exSDI_form2.h"
#endif

#include "exSDI_form2Doc.h"
#include "exSDI_form2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexSDIform2View

IMPLEMENT_DYNCREATE(CexSDIform2View, CView)

BEGIN_MESSAGE_MAP(CexSDIform2View, CView)
END_MESSAGE_MAP()

// CexSDIform2View 생성/소멸

CexSDIform2View::CexSDIform2View() noexcept
{
	EnableActiveAccessibility();
	// TODO: 여기에 생성 코드를 추가합니다.

}

CexSDIform2View::~CexSDIform2View()
{
}

BOOL CexSDIform2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CexSDIform2View 그리기

void CexSDIform2View::OnDraw(CDC* /*pDC*/)
{
	CexSDIform2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CexSDIform2View 진단

#ifdef _DEBUG
void CexSDIform2View::AssertValid() const
{
	CView::AssertValid();
}

void CexSDIform2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CexSDIform2Doc* CexSDIform2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CexSDIform2Doc)));
	return (CexSDIform2Doc*)m_pDocument;
}
#endif //_DEBUG


// CexSDIform2View 메시지 처리기
