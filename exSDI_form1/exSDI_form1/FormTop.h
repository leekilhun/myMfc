#pragma once



// FormTop 폼 보기

class FormTop : public CFormView
{
	DECLARE_DYNCREATE(FormTop)

protected:
	FormTop();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~FormTop();

public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TOP_FORM };
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


