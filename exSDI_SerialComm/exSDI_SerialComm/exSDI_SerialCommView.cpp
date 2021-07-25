
// exSDI_SerialCommView.cpp: CexSDISerialCommView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "exSDI_SerialComm.h"
#endif

#include "exSDI_SerialCommDoc.h"
#include "exSDI_SerialCommView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexSDISerialCommView

IMPLEMENT_DYNCREATE(CexSDISerialCommView, CFormView)

BEGIN_MESSAGE_MAP(CexSDISerialCommView, CFormView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CexSDISerialCommView 생성/소멸

CexSDISerialCommView::CexSDISerialCommView() noexcept
	: CFormView(IDD_EXSDI_SERIALCOMM_FORM)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CexSDISerialCommView::~CexSDISerialCommView()
{
}

void CexSDISerialCommView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BOOL CexSDISerialCommView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CexSDISerialCommView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

}

void CexSDISerialCommView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CexSDISerialCommView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CexSDISerialCommView 진단

#ifdef _DEBUG
void CexSDISerialCommView::AssertValid() const
{
	CFormView::AssertValid();
}

void CexSDISerialCommView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CexSDISerialCommDoc* CexSDISerialCommView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CexSDISerialCommDoc)));
	return (CexSDISerialCommDoc*)m_pDocument;
}
#endif //_DEBUG


// CexSDISerialCommView 메시지 처리기
