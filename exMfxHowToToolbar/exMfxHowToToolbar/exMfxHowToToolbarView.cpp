
// exMfxHowToToolbarView.cpp: CexMfxHowToToolbarView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "exMfxHowToToolbar.h"
#endif

#include "exMfxHowToToolbarDoc.h"
#include "exMfxHowToToolbarView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexMfxHowToToolbarView

IMPLEMENT_DYNCREATE(CexMfxHowToToolbarView, CView)

BEGIN_MESSAGE_MAP(CexMfxHowToToolbarView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_BUTTON_CIRCLE, &CexMfxHowToToolbarView::OnButtonCircle)
	ON_COMMAND(ID_BUTTON_EMPTY, &CexMfxHowToToolbarView::OnButtonEmpty)
	ON_COMMAND(ID_BUTTON_LINE, &CexMfxHowToToolbarView::OnButtonLine)
	ON_COMMAND(ID_BUTTON_RECTANGLE, &CexMfxHowToToolbarView::OnButtonRectangle)
	ON_COMMAND(ID_BUTTON_ZOOM_IN, &CexMfxHowToToolbarView::OnButtonZoomIn)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_CIRCLE, &CexMfxHowToToolbarView::OnUpdateButtonCircle)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_EMPTY, &CexMfxHowToToolbarView::OnUpdateButtonEmpty)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_LINE, &CexMfxHowToToolbarView::OnUpdateButtonLine)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RECTANGLE, &CexMfxHowToToolbarView::OnUpdateButtonRectangle)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_ZOOM_IN, &CexMfxHowToToolbarView::OnUpdateButtonZoomIn)
END_MESSAGE_MAP()

// CexMfxHowToToolbarView 생성/소멸

CexMfxHowToToolbarView::CexMfxHowToToolbarView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CexMfxHowToToolbarView::~CexMfxHowToToolbarView()
{
}

BOOL CexMfxHowToToolbarView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CexMfxHowToToolbarView 그리기
// lets do the drawing code: whatever shape is selected
// we draw that. Is none selected or cleared , then draw empty screen.
void CexMfxHowToToolbarView::OnDraw(CDC* pDC)
{
	CexMfxHowToToolbarDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	// create the pen for drawing
	CPen pen;
	pen.CreatePen(PS_SOLID, 10, RGB(222, 150, 0));
	// choose @pen to be the pen for drawing
	auto oldpen = pDC->SelectObject(pen);
	// if magnify toolbar button is pressed down draw bigger shapes
	const int topLeftXY = m_zoomIn ? 50 : 100;
	const int bottomRightXY = m_zoomIn ? 600 : 200;
	// so bigger shape bottom right is: 600, smaller has 200
	// the same way bigger shape top left is 50, and smaller has 100

	// draw the shape which is currently selected in the toolbar
	switch (m_shapeToDraw)
	{
	case DrawShape::Circle:
		pDC->Ellipse(CRect{topLeftXY,topLeftXY,bottomRightXY,bottomRightXY});
		break;
	case DrawShape::Rect:
		pDC->Rectangle(CRect{topLeftXY,topLeftXY,bottomRightXY,bottomRightXY});
		break;
	case DrawShape::Line:
	{
		pDC->MoveTo({topLeftXY,topLeftXY});
		pDC->LineTo({bottomRightXY,bottomRightXY});
	}
	break;
	}


	//choose back the old pen
	pDC->SelectObject(oldpen);
	pen.DeleteObject();



}

void CexMfxHowToToolbarView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CexMfxHowToToolbarView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CexMfxHowToToolbarView 진단

#ifdef _DEBUG
void CexMfxHowToToolbarView::AssertValid() const
{
	CView::AssertValid();
}

void CexMfxHowToToolbarView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CexMfxHowToToolbarDoc* CexMfxHowToToolbarView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CexMfxHowToToolbarDoc)));
	return (CexMfxHowToToolbarDoc*)m_pDocument;
}
#endif //_DEBUG


// CexMfxHowToToolbarView 메시지 처리기


void CexMfxHowToToolbarView::OnButtonCircle()
{
	//AfxMessageBox(_T("Circle !!"));

	//lets save the state: we are now drawing next a circle:
	m_shapeToDraw = DrawShape::Circle;
	// re draw the window:
	Invalidate();
	UpdateWindow();
}


void CexMfxHowToToolbarView::OnButtonEmpty()
{
	// special state: no shape should be draw and the window should be empty
	// next : lets clear the screen next:
	m_shapeToDraw = DrawShape::None;
	// re draw the window:
	Invalidate();
	UpdateWindow();
}


void CexMfxHowToToolbarView::OnButtonLine()
{
	m_shapeToDraw = DrawShape::Line;
	// re draw the window:
	Invalidate();
	UpdateWindow();
}


void CexMfxHowToToolbarView::OnButtonRectangle()
{
	m_shapeToDraw = DrawShape::Rect;
	// re draw the window:
	Invalidate();
	UpdateWindow();
}


void CexMfxHowToToolbarView::OnButtonZoomIn()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	m_zoomIn = !m_zoomIn;
	Invalidate();
	UpdateWindow();
}


void CexMfxHowToToolbarView::OnUpdateButtonCircle(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	// All the shape toolbar buttons and the clear toolbar button
	// are exclusive: Only one of them can be selected at a time
	// and if a button is selected it can not be pressed again util
	// another button is pressed - that is why it is greyed out/disabled.

	// so if shape is NOT a circle "m_shapeToDraw != DrawShape::Circle"
	// becomes TRUE and thus this circle button is enabled. Otherwise diableed.
	pCmdUI->Enable(m_shapeToDraw != DrawShape::Circle);
}


void CexMfxHowToToolbarView::OnUpdateButtonEmpty(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.

	pCmdUI->Enable(m_shapeToDraw != DrawShape::None);
}


void CexMfxHowToToolbarView::OnUpdateButtonLine(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_shapeToDraw != DrawShape::Line);
}


void CexMfxHowToToolbarView::OnUpdateButtonRectangle(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->Enable(m_shapeToDraw != DrawShape::Rect);
}


void CexMfxHowToToolbarView::OnUpdateButtonZoomIn(CCmdUI* pCmdUI)
{
	// TODO: 여기에 명령 업데이트 UI 처리기 코드를 추가합니다.
	pCmdUI->SetCheck(m_zoomIn);
}
