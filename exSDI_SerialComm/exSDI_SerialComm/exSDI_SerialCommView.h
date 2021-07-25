﻿
// exSDI_SerialCommView.h: CexSDISerialCommView 클래스의 인터페이스
//

#pragma once


class CexSDISerialCommView : public CFormView
{
protected: // serialization에서만 만들어집니다.
	CexSDISerialCommView() noexcept;
	DECLARE_DYNCREATE(CexSDISerialCommView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_EXSDI_SERIALCOMM_FORM };
#endif

// 특성입니다.
public:
	CexSDISerialCommDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void OnInitialUpdate(); // 생성 후 처음 호출되었습니다.

// 구현입니다.
public:
	virtual ~CexSDISerialCommView();
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
};

#ifndef _DEBUG  // exSDI_SerialCommView.cpp의 디버그 버전
inline CexSDISerialCommDoc* CexSDISerialCommView::GetDocument() const
   { return reinterpret_cast<CexSDISerialCommDoc*>(m_pDocument); }
#endif

