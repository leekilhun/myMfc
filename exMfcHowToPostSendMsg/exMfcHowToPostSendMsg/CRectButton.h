#pragma once


// CRectButton

class CRectButton : public CButton
{
	DECLARE_DYNAMIC(CRectButton)

public:
	CRectButton();
	virtual ~CRectButton();

	void Show(CPoint point,const CRect &rect);

	inline
	void Hide() { ShowWindow(SW_HIDE); };

protected:
	DECLARE_MESSAGE_MAP()
};


