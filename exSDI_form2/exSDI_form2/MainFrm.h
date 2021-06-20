
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

#define		DEF_WINDOW_SIZE_1280_X		1280
#define		DEF_WINDOW_SIZE_1280_Y		1024

// UI관련 클래스 Include
#include "MultiViewSplitter.h"

// UI관련 Header Include
#include "CTitleForm.h"
#include "CBottomForm.h"

#include "CAutoMainForm.h"


/** View 번호 */
enum ePaneView
{
	VIEW_AUTO = 0,
	VIEW_MANUAL,
	VIEW_DATA,
	//	VIEW_LOG,
	VIEW_MAX
};



class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame() noexcept;
	DECLARE_DYNCREATE(CMainFrame)

// 특성입니다.
public:

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;

	virtual void ActivateFrame(int nCmdShow = -1);
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
#endif

protected:  // 컨트롤 모음이 포함된 멤버입니다.
	CToolBar          m_wndToolBar;
	CStatusBar        m_wndStatusBar;

// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);




	/***************************************************/

private:
	/**
	* 멤버 변수를 초기화 한다.
	*/
	void clearVariable();
	/***************************************************/
public:

	//void SetWinSystem(MWinSystem* pSystem);

	/**
	* MWinSystem의 포인터를 반환한다.
	*
	* @return		MWinSystem*		: 생성된 MWinSystem의 포인터
	*/
	//MWinSystem* GetWinSystem();

	void InitializeAllView();

	/**
	* 화면을 전환한다.
	*
	* @param		iViewNumber		: 전환할 화면 번호
	*/
	void SwitchMainViews(int iViewNumber);

	/**
	*
	*/
	void OnManual();

	/**
	*
	*/
	void OnAutoRun();

	/**
	* 현재 View Page가 Auto Run Page 여부를 리턴한다.
	*
	* @return		BOOL			: Auto View 일 경우 TRUE, 그 외 FALSE
	*/
	BOOL GetIsShowAutoRun();

	/**
	* Model Data를 Reload한다.
	*/
	void ReloadModelData();

	void ShowStepDisp(int iStation);

	// 제어 관련 변수
private:
	//MWinSystem* m_plnkWinSystem;

	// UI관련 변수
private:
	/** 분할 View 및 화면 전환을 위한 클래스 */
	CMultiViewSplitter				m_wndSplitter;
	int								m_iLastNaviPage_ID;

	/** 생성된 View Number 저장 변수 */
	int								m_nView[VIEW_MAX];

	RECT							m_rcClient;

	BOOL							m_bInit;

	BOOL							m_bInitComplete;

	BOOL							m_bScreenSaver_Backup;

private:
	int								m_iHeight_Title;
	int								m_iHeight_Bottom;

	// View 클래스 변수
private:
	CTitleForm* m_pTitleForm;
	CBottomForm* m_pBottomForm;

	CAutoMainForm* m_pAutoMainForm;
	//CAutoMainDualForm* m_pAutoMainDualForm;
	//CManualMainForm* m_pManualMainForm;
	//CVisionMainForm* m_pVisionMainForm;
	//CVisionMainDualForm* m_pVisionMainDualForm;
	//CDataMainForm* m_pDataMainForm;
	//	CLogForm*						m_pLogMainForm;

private:
	//UScreenSaverControl				m_ScreenSaverControl;
	//CPlatformManager				m_PlatformManager;

public:
	afx_msg void OnClose();

	HWND							GetMainSafeHwnd();


};


