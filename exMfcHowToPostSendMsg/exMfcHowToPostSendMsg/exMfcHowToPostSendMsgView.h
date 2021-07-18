
// exMfcHowToPostSendMsgView.h: CexMfcHowToPostSendMsgView 클래스의 인터페이스
//

#pragma once
#include <vector>
#include "CRectButton.h"
#include "RectEdit.h"

constexpr UINT IDC_BUTTON{101};
constexpr UINT IDC_EDIT{102};


class CexMfcHowToPostSendMsgView : public CView
{
private:
	CRectButton m_button;
	CRectEdit m_edit;
	std::vector<CRect> m_rectangles;

	void CreateRectangles();
	void DrawRectangle(CDC* pDC);
	void RectangleHit(const CPoint& point);

protected: // serialization에서만 만들어집니다.
	CexMfcHowToPostSendMsgView() noexcept;
	DECLARE_DYNCREATE(CexMfcHowToPostSendMsgView)

// 특성입니다.
public:
	CexMfcHowToPostSendMsgDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현입니다.
public:
	virtual ~CexMfcHowToPostSendMsgView();
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
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // exMfcHowToPostSendMsgView.cpp의 디버그 버전
inline CexMfcHowToPostSendMsgDoc* CexMfcHowToPostSendMsgView::GetDocument() const
   { return reinterpret_cast<CexMfcHowToPostSendMsgDoc*>(m_pDocument); }
#endif

