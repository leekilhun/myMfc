
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "pch.h"
#include "framework.h"
#include "exSDI_form2.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_SYSCOMMAND()
	ON_WM_GETMINMAXINFO()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{

	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	clearVariable();

	m_iHeight_Title = 70;
	m_iHeight_Bottom = 88;

	EnableActiveAccessibility();
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	clearVariable();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);
	*/

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	//cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
	//	 | WS_SYSMENU;

	cs.style = WS_VISIBLE | WS_POPUP | WS_BORDER;
	//	cs.dwExStyle |= WS_EX_TOPMOST;		// 최상위 윈도우로 설정한다.

	int nX = 0;
	int nY = 0;
		int iFullScreen_Y = GetSystemMetrics(SM_CYSCREEN);
		if (iFullScreen_Y > 1280)	nY = DEF_WINDOW_SIZE_1280_Y;
		else						nY = iFullScreen_Y;

	cs.cx = nX;
	cs.cy = nY;

	/**********************************
	**	Menu Bar를 지운다.
	**********************************/
	CMenu* pMenu = new CMenu;
	pMenu->Attach(cs.hMenu);
	pMenu->DestroyMenu();
	cs.hMenu = NULL;
	delete pMenu;
	/**********************************/

	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 메시지 처리기
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	UNUSED_ALWAYS(lpcs);

	// Row : 3, Column : 1
	if (!m_wndSplitter.CreateStatic(this, 3, 1))
	{
		TRACE(_T("Failed to Create splitter bar"));
		return FALSE;
	}

	// Multi Splitter Customize Option Setting
	m_wndSplitter.SetSplitBorderOption(TRUE, m_iHeight_Title, m_iHeight_Bottom);

	// 각각의 분할뷰에 대한 View Class를 지정하고, 사이즈를 정해준다.(각 View Class에 대한 헤더파일 필요^^)
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CTitleForm), CSize(1, 1), pContext))
	{
		TRACE0("Fail to create view...");
		return FALSE;
	}

	m_pTitleForm = (CTitleForm*)m_wndSplitter.GetPane(0, 0);

	m_nView[VIEW_AUTO] = m_wndSplitter.AddView(1, 0, RUNTIME_CLASS(CAutoMainForm), pContext);
	m_pAutoMainForm = (CAutoMainForm*)m_wndSplitter.GetPane(1, 0);

	if (!m_wndSplitter.CreateView(2, 0, RUNTIME_CLASS(CBottomForm), CSize(100, 100), pContext))
	{
		TRACE0("Fail to create view...");
		return FALSE;
	}
	m_pBottomForm = (CBottomForm*)m_wndSplitter.GetPane(2, 0);

	// Client 영역의 크기를 구한다.
	GetClientRect(&m_rcClient);

	int iBottomHeight = 862;

	int iFullScreen_Y = GetSystemMetrics(SM_CYSCREEN);
	iBottomHeight = iFullScreen_Y - (m_iHeight_Title + m_iHeight_Bottom);

	// 디버그용
	//iBottomHeight = 750;

	m_wndSplitter.SetRowInfo(0, m_iHeight_Title, 0);
	m_wndSplitter.SetRowInfo(1, iBottomHeight, 0); // m_rcClient.bottom - (34 + 62 + 4), 0);
	m_wndSplitter.SetRowInfo(2, m_iHeight_Bottom, 0);

	SwitchMainViews(VIEW_AUTO);

	//	this->ModifyStyle(0, WS_SYSMENU, 0);

		//return CFrameWnd::OnCreateClient(lpcs, pContext);

		//m_TrayIcon.AddTrayIcon(m_hWnd, IDR_MAINFRAME, AfxGetApp()->LoadIcon(IDR_MAINFRAME), _T("test"));


	return TRUE;
}


void CMainFrame::ActivateFrame(int nCmdShow)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	CFrameWnd::ActivateFrame(nCmdShow);
}


void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd::OnTimer(nIDEvent);
}


void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd::OnSysCommand(nID, lParam);
}


void CMainFrame::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CFrameWnd::OnGetMinMaxInfo(lpMMI);
}

void CMainFrame::clearVariable()
{
}

void CMainFrame::InitializeAllView()
{
	//if (m_pTitleForm != NULL)				m_pTitleForm->InitView();

	//if (m_pAutoMainForm != NULL)			m_pAutoMainForm->InitView();

	//if (m_pBottomForm != NULL)				m_pBottomForm->InitView();

	m_bInit = TRUE;
}

void CMainFrame::SwitchMainViews(int iViewNumber)
{
	// 이전에 Click한 View와 동일할 경우 리턴한다.
	if (m_iLastNaviPage_ID != -1 && m_iLastNaviPage_ID == m_nView[iViewNumber])
		return;

	m_iLastNaviPage_ID = m_nView[iViewNumber];

	if (iViewNumber != VIEW_AUTO)
	{
		//if (m_pAutoMainForm != NULL)
		//	m_pAutoMainForm->HideStepDisplay();
	}

	m_wndSplitter.ShowView(m_nView[iViewNumber]);
}

void CMainFrame::OnManual()
{
}

void CMainFrame::OnAutoRun()
{
}

BOOL CMainFrame::GetIsShowAutoRun()
{
	return 0;
}

void CMainFrame::ReloadModelData()
{
}

void CMainFrame::ShowStepDisp(int iStation)
{
	
}

void CMainFrame::OnClose()
{
	//CFrameWndEx::OnClose();
}

HWND CMainFrame::GetMainSafeHwnd()
{
	return this->GetSafeHwnd();
}



