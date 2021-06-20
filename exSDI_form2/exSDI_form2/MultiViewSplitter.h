#pragma once

#include <map>
using namespace std;


class CMultiViewSplitter : public CSplitterWnd
{

public:
	CMultiViewSplitter();

	virtual ~CMultiViewSplitter();

	int AddView(int nRow, int nCol, CRuntimeClass* pViewClass,
				CCreateContext* pContext);

	int ReplaceView(int nRow, int nCol, CRuntimeClass* pViewClass);

	void ShowView(int nViewID);
	CWnd* GetView(int nViewID);
	int	HitTest(CPoint pt) const;
	void SetCurrentView(int nRow, int nCol, int nViewID);
	CWnd* GetCurrentView(int nRow, int nCol, int* nCurID);

protected:
	map<int, long> m_mapViewPane;
	map<long, int> m_mapCurrentViews;
	map<int, CWnd*> m_mapIDViews;

	int m_nIDCounter;

public:
	int HideCurrentView(int nRow, int nCol);
	void GetPaneFromViewID(int nViewID, CPoint* pane);
	virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);

	// PYTHIA Platform에서만 사용하는 기능 정의
	void SetSplitBorderOption(BOOL bSet = FALSE, int iHeight_Title = 0, int iHeight_Bottom = 0);
	BOOL m_bSplitBorderOption;
	int m_iHeight_Title;
	int m_iHeight_Bottom;

};

