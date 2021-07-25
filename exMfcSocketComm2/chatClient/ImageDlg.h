#pragma once


// CImageDlg 대화 상자

class CImageDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CImageDlg)

public:
	CImageDlg(const CString& file_name, CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CImageDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_IMG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CString m_fileName;
	CImage m_img;
	CStatic m_pic;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
};
