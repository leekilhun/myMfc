
// exMfcHowToGraphicsGdiView.h: CexMfcHowToGraphicsGdiView 클래스의 인터페이스
//

#pragma once


class CexMfcHowToGraphicsGdiView : public CView
{
	int m_move{0};
protected: // serialization에서만 만들어집니다.
	CexMfcHowToGraphicsGdiView() noexcept;
	DECLARE_DYNCREATE(CexMfcHowToGraphicsGdiView)

// 특성입니다.
public:
	CexMfcHowToGraphicsGdiDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CexMfcHowToGraphicsGdiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

#ifndef _DEBUG  // exMfcHowToGraphicsGdiView.cpp의 디버그 버전
inline CexMfcHowToGraphicsGdiDoc* CexMfcHowToGraphicsGdiView::GetDocument() const
   { return reinterpret_cast<CexMfcHowToGraphicsGdiDoc*>(m_pDocument); }
#endif

