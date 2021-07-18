
// exMfcHowToPostSendMsgView.cpp: CexMfcHowToPostSendMsgView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "exMfcHowToPostSendMsg.h"
#endif

#include "exMfcHowToPostSendMsgDoc.h"
#include "exMfcHowToPostSendMsgView.h"
#include <optional>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CexMfcHowToPostSendMsgView

IMPLEMENT_DYNCREATE(CexMfcHowToPostSendMsgView, CView)

BEGIN_MESSAGE_MAP(CexMfcHowToPostSendMsgView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

// CexMfcHowToPostSendMsgView 생성/소멸

void CexMfcHowToPostSendMsgView::CreateRectangles()
{
	const CPoint groupTopLeft{50,10};
	const int VertSpace{5};
	const CSize sizeRectangle{200,50};

	for (int i{0}; i < 5; ++i)
	{
		CRect rect;
		rect.left = groupTopLeft.x;
		rect.top = groupTopLeft.y + i * sizeRectangle.cy + i * VertSpace;
		rect.right = sizeRectangle.cx;
		rect.bottom = rect.top + sizeRectangle.cy;

		m_rectangles.push_back(rect);
	}
}

void CexMfcHowToPostSendMsgView::DrawRectangle(CDC* pDC)
{
	CPen penBlue;
	penBlue.CreatePen(PS_SOLID, 3, RGB(0, 0, 200));
	CPen* pOldPen = pDC->SelectObject(&penBlue);

	const auto size{m_rectangles.size()};
	for (int i{0}; i < (int)size; ++i)
	{
		//CPoint pt(10, 10);
		//CSize sz(100, 50);
		//CRect myRect(pt, sz);
		pDC->Rectangle(&m_rectangles[i]);
	}
	pDC->SelectObject(pOldPen);

}

void CexMfcHowToPostSendMsgView::RectangleHit(const CPoint& point)
{
	std::optional<int> hitRectangleIndex;

	const auto size{m_rectangles.size()};
	for (int i{0}; i < (int)size; ++i)
	{
		if (m_rectangles[i].PtInRect(point))
		{
			hitRectangleIndex = i;
			break;
		}
	}
	if (hitRectangleIndex)
	{
		const CRect& hitRect{m_rectangles[*hitRectangleIndex]};

		CRect rectButton;
		rectButton.left = hitRect.right + 10;
		rectButton.top = hitRect.top;
		rectButton.right = rectButton.left + 80;
		rectButton.bottom = hitRect.top + 40;
				
		Info* info = new Info;
		info->mousePoint = point;
		info->text = _T("Pt");
		info->rectWindow = rectButton;

		//m_edit.Show(*info);

		m_edit.PostMessageW(WM_SHOWINFO, reinterpret_cast<LPARAM>(info));
		//m_edit.PostMessageW(WM_MOVE, 0, 0);
		//m_edit.SendMessageW(WM_MOVE, 0, 0);
		//m_button.Show(point, rectButton);
	}
	else
	{
		m_edit.Hide();
		//m_button.Hide();
	}
}

CexMfcHowToPostSendMsgView::CexMfcHowToPostSendMsgView() noexcept
{
	// TODO: 여기에 생성 코드를 추가합니다.
	CreateRectangles();

}

CexMfcHowToPostSendMsgView::~CexMfcHowToPostSendMsgView()
{
}

BOOL CexMfcHowToPostSendMsgView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CexMfcHowToPostSendMsgView 그리기

void CexMfcHowToPostSendMsgView::OnDraw(CDC* pDC)
{
	CexMfcHowToPostSendMsgDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
	/*CPen penBlack;
	penBlack.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	CPen* pOldPen = pDC->SelectObject(&penBlack);*/
	DrawRectangle(pDC);

}

void CexMfcHowToPostSendMsgView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CexMfcHowToPostSendMsgView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CexMfcHowToPostSendMsgView 진단

#ifdef _DEBUG
void CexMfcHowToPostSendMsgView::AssertValid() const
{
	CView::AssertValid();
}

void CexMfcHowToPostSendMsgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CexMfcHowToPostSendMsgDoc* CexMfcHowToPostSendMsgView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CexMfcHowToPostSendMsgDoc)));
	return (CexMfcHowToPostSendMsgDoc*)m_pDocument;
}
#endif //_DEBUG


// CexMfcHowToPostSendMsgView 메시지 처리기


int CexMfcHowToPostSendMsgView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  여기에 특수화된 작성 코드를 추가합니다.
	m_button.Create(_T("Hi"), BS_PUSHBUTTON,
		CRect(300, 100, 380, 150), this, IDC_BUTTON);


	m_edit.Create(WS_CHILD | WS_TABSTOP | WS_BORDER, // WS_VISIBLE |
		CRect(10, 10, 100, 100), this, IDC_EDIT);

	//m_button.ShowWindow(SW_SHOW);


	return 0;
}


void CexMfcHowToPostSendMsgView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	RectangleHit(point);
	/*
	const auto size{m_rectangles.size()};
	for (int i{0}; i < size; ++i)
	{
		if (m_rectangles[i].PtInRect(point))
		{

		}
	}
	*/
	CView::OnLButtonDown(nFlags, point);
}
