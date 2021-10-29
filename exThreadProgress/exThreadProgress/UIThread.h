#pragma once
//#include <afxwin.h>

class CexThreadProgressDlg;

class CUIThread :
  public CWinThread
{
private:
  CexThreadProgressDlg* m_mainDlg;
  int m_i;
  int m_per;
public:
  DECLARE_DYNCREATE(CUIThread) // 선언 없으면 메모리 부족 에러 뜸

  int run();
  void job();

  virtual BOOL InitInstance();
};

