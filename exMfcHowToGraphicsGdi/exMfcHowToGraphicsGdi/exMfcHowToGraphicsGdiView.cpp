
// exMfcHowToGraphicsGdiView.cpp: CexMfcHowToGraphicsGdiView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "exMfcHowToGraphicsGdi.h"
#endif

#include "exMfcHowToGraphicsGdiDoc.h"
#include "exMfcHowToGraphicsGdiView.h"
#include "FlickerFreeDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace Gdiplus;
// CexMfcHowToGraphicsGdiView

IMPLEMENT_DYNCREATE(CexMfcHowToGraphicsGdiView, CView)

BEGIN_MESSAGE_MAP(CexMfcHowToGraphicsGdiView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CexMfcHowToGraphicsGdiView 생성/소멸

CexMfcHowToGraphicsGdiView::CexMfcHowToGraphicsGdiView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CexMfcHowToGraphicsGdiView::~CexMfcHowToGraphicsGdiView()
{
}

BOOL CexMfcHowToGraphicsGdiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CexMfcHowToGraphicsGdiView 그리기

void CexMfcHowToGraphicsGdiView::OnDraw(CDC* pDC)
{
	CexMfcHowToGraphicsGdiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	FFDC::CMemDC dc{pDC};
	{
		Graphics graphics(dc->GetSafeHdc());

		Pen pen(Color(255, 0, 255, 128), 5.0);

		graphics.DrawEllipse(&pen, 200, 50, 100, 100);
		graphics.DrawRectangle(&pen, 200 + m_move, 200, 50, 70);
	}

	dc.LineTo(500, 300);
	
	/*
	dc.FillSolidRect(CRect(0, 0, 300, 300) +
		CPoint(m_move, 0), RGB(255, 0, 0));

	*/

}

void CexMfcHowToGraphicsGdiView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CexMfcHowToGraphicsGdiView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CexMfcHowToGraphicsGdiView 진단

#ifdef _DEBUG
void CexMfcHowToGraphicsGdiView::AssertValid() const
{
	CView::AssertValid();
}

void CexMfcHowToGraphicsGdiView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CexMfcHowToGraphicsGdiDoc* CexMfcHowToGraphicsGdiView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CexMfcHowToGraphicsGdiDoc)));
	return (CexMfcHowToGraphicsGdiDoc*)m_pDocument;
}
#endif //_DEBUG


// CexMfcHowToGraphicsGdiView 메시지 처리기


void CexMfcHowToGraphicsGdiView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	++m_move;
	Invalidate();
	UpdateWindow();
	CView::OnMouseMove(nFlags, point);
}


BOOL CexMfcHowToGraphicsGdiView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return FALSE;

	//return CView::OnEraseBkgnd(pDC);
}
