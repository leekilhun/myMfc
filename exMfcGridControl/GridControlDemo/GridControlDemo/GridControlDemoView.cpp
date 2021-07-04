
// GridControlDemoView.cpp : CGridControlDemoView 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "GridControlDemo.h"
#endif

#include "GridControlDemoDoc.h"
#include "GridControlDemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGridControlDemoView

IMPLEMENT_DYNCREATE(CGridControlDemoView, CFormView)

BEGIN_MESSAGE_MAP(CGridControlDemoView, CFormView)
END_MESSAGE_MAP()

// CGridControlDemoView 생성/소멸

CGridControlDemoView::CGridControlDemoView()
	: CFormView(CGridControlDemoView::IDD)
{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CGridControlDemoView::~CGridControlDemoView()
{
}

void CGridControlDemoView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_GRID, m_Grid);
}

BOOL CGridControlDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CGridControlDemoView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	GetDlgItem(IDC_GRID)->MoveWindow(1, 1, 600, 800);

	int m_nFixRows = 1;
	int m_nFixCols = 1;

	m_Grid.SetEditable(TRUE);
	m_Grid.SetListMode(TRUE);
	m_Grid.EnableDragAndDrop(TRUE);
	m_Grid.SetTextBkColor(RGB(0xFF, 0xFF, 0xE0));
	m_Grid.EnableScrollBar(ESB_ENABLE_BOTH, TRUE);

	m_Grid.SetRowCount(7);
	m_Grid.SetColumnCount(7);
	m_Grid.SetFixedRowCount(m_nFixRows);
	m_Grid.SetFixedColumnCount(1);

	DWORD dwTextStyle = DT_CENTER|DT_VCENTER|DT_SINGLELINE;

	// fill rows/cols with text
	for (int row = 0; row < m_Grid.GetRowCount(); row++) {
		for (int col = 0; col < m_Grid.GetColumnCount(); col++) { 
			GV_ITEM Item;
			Item.mask = GVIF_TEXT|GVIF_FORMAT;
			Item.row = row;
			Item.col = col;

			if (row < m_nFixRows) {
				Item.nFormat = DT_LEFT|DT_WORDBREAK;
				Item.strText.Format(_T("Column %d"),col);

			} else if (col < m_nFixCols) {
				Item.nFormat = dwTextStyle;
				Item.strText.Format(_T("Row %d"),row);

			} else {
				Item.nFormat = dwTextStyle;
				Item.strText.Format(_T("%d"),row*col);
			}
			m_Grid.SetItem(&Item);  

			if (rand() % 10 == 1) {
				COLORREF clr = RGB(rand() % 128+128, 
					rand() % 128+128, 
					rand() % 128+128);
				m_Grid.SetItemBkColour(row, col, clr);
				m_Grid.SetItemFgColour(row, col, RGB(255,0,0));
			}
		}
	}
	// Make cell 1,1 read-only
	m_Grid.SetItemState(1,1, m_Grid.GetItemState(1,1) | GVIS_READONLY);

	m_Grid.AutoSize();
	m_Grid.SetRowHeight(0, 3*m_Grid.GetRowHeight(0)/2);




}


// CGridControlDemoView 진단

#ifdef _DEBUG
void CGridControlDemoView::AssertValid() const
{
	CFormView::AssertValid();
}

void CGridControlDemoView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CGridControlDemoDoc* CGridControlDemoView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGridControlDemoDoc)));
	return (CGridControlDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CGridControlDemoView 메시지 처리기
