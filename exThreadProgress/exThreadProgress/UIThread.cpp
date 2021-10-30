#include "pch.h"
#include "UIThread.h"
#include "exThreadProgress.h"
#include "exThreadProgressDlg.h"

IMPLEMENT_DYNCREATE(CUIThread, CWinThread)

int CUIThread::run()
{
  TRACE(_T("threadUI Run\n"));
  // Thread Loop
  while (m_mainDlg->m_lifeThreadUI)    
  { 
    job();
    Sleep(20);
  }    

  m_mainDlg->GetDlgItem(IDC_BTN_START_UI)->EnableWindow(TRUE);
  TRACE("\r Exit UI Thread \n");
	return 0;
}

void CUIThread::job()
{
  CString str;
  m_mainDlg->m_progressUI.SetPos(m_i % 1000);
  if (m_i % 10 == 0)
  {
    m_per++;
    str.Format(L"%d%%", m_per % 100);
    m_mainDlg->m_strUI.SetWindowText(str);
    //dlg->SetDlgItemTextW(IDC_STATIC_UI, str);
  }
  m_i++;
}


BOOL CUIThread::InitInstance()
{
  // TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
  m_mainDlg = nullptr;
  m_i = 0;
  m_per = 0;

  CexThreadProgressApp* pApp = (CexThreadProgressApp*)AfxGetApp();// ->m_pMainWnd;
  m_mainDlg = (CexThreadProgressDlg*)pApp->m_pMainWnd;
  return run();
}
