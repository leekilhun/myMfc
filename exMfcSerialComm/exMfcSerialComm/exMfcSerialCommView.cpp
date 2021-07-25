
// exMfcSerialCommView.cpp: CexMfcSerialCommView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "exMfcSerialComm.h"
#endif

#include "exMfcSerialCommDoc.h"
#include "exMfcSerialCommView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexMfcSerialCommView

IMPLEMENT_DYNCREATE(CexMfcSerialCommView, CView)

BEGIN_MESSAGE_MAP(CexMfcSerialCommView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CexMfcSerialCommView 생성/소멸

CexMfcSerialCommView::CexMfcSerialCommView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CexMfcSerialCommView::~CexMfcSerialCommView()
{
}

BOOL CexMfcSerialCommView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CexMfcSerialCommView 그리기

void CexMfcSerialCommView::OnDraw(CDC* pDC)
{
	CexMfcSerialCommDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	static HWND hEdit2;
	hEdit2 = CreateWindow(
		_T("edit"),
		_T(""),
		WS_CHILD | WS_VISIBLE | WS_BORDER,
		225,
		125,
		100,
		25,
		this->m_hWnd,
		(HMENU)IDC_EDIT2,
		0,
		NULL);

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}

void CexMfcSerialCommView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CexMfcSerialCommView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CexMfcSerialCommView 진단

#ifdef _DEBUG
void CexMfcSerialCommView::AssertValid() const
{
	CView::AssertValid();
}

void CexMfcSerialCommView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CexMfcSerialCommDoc* CexMfcSerialCommView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CexMfcSerialCommDoc)));
	return (CexMfcSerialCommDoc*)m_pDocument;
}
#endif //_DEBUG


// CexMfcSerialCommView 메시지 처리기
