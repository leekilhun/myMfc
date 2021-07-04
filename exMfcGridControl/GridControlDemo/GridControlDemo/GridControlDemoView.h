
// GridControlDemoView.h : CGridControlDemoView Ŭ������ �������̽�
//

#pragma once

#include "resource.h"

#include "GridCtrl.h"


class CGridControlDemoView : public CFormView
{
protected: // serialization������ ��������ϴ�.
	CGridControlDemoView();
	DECLARE_DYNCREATE(CGridControlDemoView)

public:
	enum{ IDD = IDD_GRIDCONTROLDEMO_FORM };

// Ư���Դϴ�.
public:
	CGridControlDemoDoc* GetDocument() const;
	CGridCtrl	m_Grid;

	

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual void OnInitialUpdate(); // ���� �� ó�� ȣ��Ǿ����ϴ�.

// �����Դϴ�.
public:
	virtual ~CGridControlDemoView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // GridControlDemoView.cpp�� ����� ����
inline CGridControlDemoDoc* CGridControlDemoView::GetDocument() const
   { return reinterpret_cast<CGridControlDemoDoc*>(m_pDocument); }
#endif

