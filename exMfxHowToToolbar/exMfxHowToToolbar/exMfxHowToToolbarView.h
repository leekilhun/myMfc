
// exMfxHowToToolbarView.h: CexMfxHowToToolbarView 클래스의 인터페이스
//

#pragma once

enum class DrawShape {None, Circle, Rect, Line};

class CexMfxHowToToolbarView : public CView
{
protected: // serialization에서만 만들어집니다.
	CexMfxHowToToolbarView() noexcept;
	DECLARE_DYNCREATE(CexMfxHowToToolbarView)

// 특성입니다.
public:
	CexMfxHowToToolbarDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CexMfxHowToToolbarView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:
	//State of the program : which shape selected currently
	DrawShape m_shapeToDraw{DrawShape::None};
	// are we zooming in the shapes
	bool m_zoomIn{false};


// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonCircle();
	afx_msg void OnButtonEmpty();
	afx_msg void OnButtonLine();
	afx_msg void OnButtonRectangle();
	afx_msg void OnButtonZoomIn();
	afx_msg void OnUpdateButtonCircle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonEmpty(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonRectangle(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonZoomIn(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // exMfxHowToToolbarView.cpp의 디버그 버전
inline CexMfxHowToToolbarDoc* CexMfxHowToToolbarView::GetDocument() const
   { return reinterpret_cast<CexMfxHowToToolbarDoc*>(m_pDocument); }
#endif

