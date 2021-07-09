
// exMfxHowToUsingSpinButtonDlg.h: 헤더 파일
//

#pragma once

const int LOWEST_SPIN_VALUE = 0;
const int HIGHEST_SPIN_VALUE = 10;
// this will be the bottom y-coordinate of the rectangle at the beginning:
const int RECT_START_BOTTOM_POS = 450;


// CexMfxHowToUsingSpinButtonDlg 대화 상자
class CexMfxHowToUsingSpinButtonDlg : public CDialogEx
{
// 생성입니다.
public:
	CexMfxHowToUsingSpinButtonDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXMFXHOWTOUSINGSPINBUTTON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
private:

public:
	afx_msg void OnDeltaposSpinNumbers(NMHDR* pNMHDR, LRESULT* pResult);
private:
	CSpinButtonCtrl m_spinNumbers;
	CString m_float;
	CSpinButtonCtrl m_spinRectangle;
	
	// this variable hold always the curreunt bottom y-coordinate
	// of the rectangle to be draw
	int m_rectBottomPos{RECT_START_BOTTOM_POS};
public:
	afx_msg void OnDeltaposSpinRectangle(NMHDR* pNMHDR, LRESULT* pResult);
};
