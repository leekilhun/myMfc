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
  DECLARE_DYNCREATE(CUIThread) // ���� ������ �޸� ���� ���� ��

  int run();
  void job();

  virtual BOOL InitInstance();
};

