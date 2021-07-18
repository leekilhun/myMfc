#pragma once

#include "definitions.h"

//#define WM_MYMESSAGE (WM_USER+100)
constexpr UINT WM_SHOWINFO{WM_USER + 1};


// CRectEdit





class CRectEdit : public CEdit
{
	DECLARE_DYNAMIC(CRectEdit)

public:
	CRectEdit();
	virtual ~CRectEdit();

	void Show(/*const Info & info*/CPoint point, const CRect& rect);
	

	inline
		void Hide() { ShowWindow(SW_HIDE); };
private:
	afx_msg LRESULT OnShowInfo(WPARAM wParam, LPARAM lParam);

protected:
	DECLARE_MESSAGE_MAP()
};


