#pragma once


// CAutoMainForm 폼 보기

class CAutoMainForm : public CFormView
{
	DECLARE_DYNCREATE(CAutoMainForm)

protected:
	CAutoMainForm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CAutoMainForm();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_AUTO_MAIN_FORM };
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};


