#include "pch.h"
#include "MultiViewSplitter.h"

CMultiViewSplitter::CMultiViewSplitter()
{
	m_nIDCounter = 1;

	// 분할창의 테두리 값, 이 값만큼 뷰가 안쪽에 위치한다.
	m_cxBorder = 0;
	m_cyBorder = 0;

	// 분할바를 사이에 둔 양쪽 View 사이의 공간
	m_cxSplitterGap = 1;
	m_cySplitterGap = 1;

	// 수직/수평 분할 바의 두께를 정의한다.
	m_cxSplitter = 0;
	m_cySplitter = 0;

	// 분할바의 양끝이 테두리와 겹치는 정도
	m_cxBorderShare = 0;
	m_cyBorderShare = 0;

	// Splitter Border와 Split Bar를 
	m_bSplitBorderOption = FALSE;
	m_iHeight_Title = 0;
	m_iHeight_Bottom = 0;

}

CMultiViewSplitter::~CMultiViewSplitter()
{
}

int CMultiViewSplitter::AddView(int nRow, int nCol, CRuntimeClass* pViewClass, CCreateContext* pContext)
{
	// hide the current view of the pane
	int PreviousID = HideCurrentView(nRow, nCol);

	// create the new view, if fail, set the previous view current 
	if (CreateView(nRow, nCol, pViewClass, CSize(1, 1), pContext) == 0)
	{
		if (PreviousID != -1)
			SetCurrentView(nRow, nCol, PreviousID);
		return -1;
	}

	// get and store the niew view
	int NewViewID = m_nIDCounter;
	CWnd* pNewWnd = GetPane(nRow, nCol);
	CPoint pane(nRow, nCol);
	long paneID = MAKELONG(pane.x, pane.y);
	m_mapViewPane.insert(map<int, long>::value_type(NewViewID, paneID));
	m_mapIDViews.insert(map<int, CWnd*>::value_type(NewViewID, pNewWnd));

	// set the new view current
	SetCurrentView(nRow, nCol, NewViewID);

	RedrawWindow();
	m_nIDCounter++;

	return NewViewID;
}

int CMultiViewSplitter::ReplaceView(int nRow, int nCol, CRuntimeClass* pViewClass)
{
	// hide the current view of the pane
	int PreviousID = 0;


	CCreateContext ccx;

	// Designate the class from which to build the view
	ccx.m_pNewViewClass = pViewClass;


	// create the new view, if fail, set the previous view current 
	if (CreateView(nRow, nCol, pViewClass, CSize(1, 1), &ccx) == 0)
	{
		if (PreviousID != -1)
			SetCurrentView(nRow, nCol, PreviousID);
		return -1;
	}

	// get and store the niew view
	int NewViewID = m_nIDCounter;
	CWnd* pNewWnd = GetPane(nRow, nCol);
	CPoint pane(nRow, nCol);
	long paneID = MAKELONG(pane.x, pane.y);
	m_mapViewPane.insert(map<int, long>::value_type(NewViewID, paneID));
	m_mapIDViews.insert(map<int, CWnd*>::value_type(NewViewID, pNewWnd));

	// set the new view current
	SetCurrentView(nRow, nCol, NewViewID);

	RedrawWindow();
	//	m_nIDCounter ++;
	return NewViewID;
}

void CMultiViewSplitter::ShowView(int nViewID)
{
	if (GetView(nViewID) == NULL)
		return;

	// find the pane containing the view 
	CPoint pane;
	GetPaneFromViewID(nViewID, &pane);

	// switch views
	HideCurrentView(pane.x, pane.y);
	SetCurrentView(pane.x, pane.y, nViewID);

	RecalcLayout();
}

CWnd* CMultiViewSplitter::GetView(int nViewID)
{
	map<int, CWnd*>::iterator itView;

	itView = m_mapIDViews.find(nViewID);
	if (itView == m_mapIDViews.end())
		return NULL;
	else
		return (*itView).second;
}

int CMultiViewSplitter::HitTest(CPoint pt) const
{
	UNUSED_ALWAYS(pt);

	ASSERT_VALID(this);
	// 마우스 위치에 대한 반응으로 HTNOWHERE=0 를 리턴하여
	// CSplitterWnd 의 Border 을 움직이지 않게 막는다....
	return HTNOWHERE;
	// return CSplitterWnd::HitTest(pt);
}

void CMultiViewSplitter::SetCurrentView(int nRow, int nCol, int nViewID)
{
	long paneID = MAKELONG(nRow, nCol);

	map<long, int>::iterator itCur;
	itCur = m_mapCurrentViews.find(paneID);
	if (itCur != m_mapCurrentViews.end())
		(*itCur).second = nViewID;
	else
		m_mapCurrentViews.insert(map<long, int>::value_type(paneID, nViewID));

	CWnd* pView = GetView(nViewID);
	pView->SetDlgCtrlID(IdFromRowCol(nRow, nCol));
	pView->ShowWindow(SW_SHOW);
}

CWnd* CMultiViewSplitter::GetCurrentView(int nRow, int nCol, int* nCurID)
{
	long paneID = MAKELONG(nRow, nCol);

	map<long, int>::iterator itCur;
	itCur = m_mapCurrentViews.find(paneID);
	if (itCur == m_mapCurrentViews.end())
		return NULL;
	else
	{
		int PreviousID = (*itCur).second;
		*nCurID = PreviousID;
		return GetView(PreviousID);
	}
}

int CMultiViewSplitter::HideCurrentView(int nRow, int nCol)
{
	int prevID;
	CWnd* pCurView = GetCurrentView(nRow, nCol, &prevID);
	if (pCurView == NULL)
		return -1;
	else
	{
		pCurView->SetDlgCtrlID(0);
		pCurView->ShowWindow(SW_HIDE);
		return prevID;
	}
}

void CMultiViewSplitter::GetPaneFromViewID(int nViewID, CPoint* pane)
{
	map<int, long>::iterator itPane;
	itPane = m_mapViewPane.find(nViewID);
	if (itPane == m_mapViewPane.end())
	{
		pane = NULL;
		return;
	}
	long paneID = (*itPane).second;
	CPoint p(paneID);
	pane->x = p.x;
	pane->y = p.y;
}

void CMultiViewSplitter::OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect)
{
	CClientDC dc(this);
	CPen* pBorderPen, * pOldPen;

	//pBorderPen = new CPen( PS_SOLID, 3, ::GetSysColor( COLOR_3DFACE ) );

	if (m_bSplitBorderOption == TRUE)
	{
		if (m_iHeight_Title > 0 && rect.Height() == m_iHeight_Title)		// Title Formview 일 때
			pBorderPen = new CPen(PS_SOLID, 2, RGB(230, 230, 230));
		else if (m_iHeight_Bottom > 0 && rect.Height() == m_iHeight_Bottom)
			pBorderPen = new CPen(PS_SOLID, 2, RGB(60, 90, 151));
		else
			pBorderPen = new CPen(PS_SOLID, 2, RGB(255, 255, 255));

		pOldPen = dc.SelectObject(pBorderPen);
		dc.SelectStockObject(HOLLOW_BRUSH);
		dc.Rectangle(rect);
		dc.SelectObject(pOldPen);
		if (pBorderPen)
			delete pBorderPen;
	}
	else
	{
		if (nType == splitBorder)
		{
			CClientDC dc(this);
			CPen* pBorderPen, * pOldPen;
			//pBorderPen = new CPen( PS_SOLID, 3, ::GetSysColor( COLOR_3DFACE ) );
			pBorderPen = new CPen(PS_SOLID, 1, RGB(255, 255, 255));
			pOldPen = dc.SelectObject(pBorderPen);
			dc.SelectStockObject(HOLLOW_BRUSH);
			dc.Rectangle(rect);
			dc.SelectObject(pOldPen);
			if (pBorderPen)
				delete pBorderPen;
		}
		if (nType == splitBar)
		{
			CSplitterWnd::OnDrawSplitter(pDC, nType, rect);
		}
	}

}

void CMultiViewSplitter::SetSplitBorderOption(BOOL bSet, int iHeight_Title, int iHeight_Bottom)
{
	m_bSplitBorderOption = bSet;
	m_iHeight_Title = iHeight_Title;
	m_iHeight_Bottom = iHeight_Bottom;
}
