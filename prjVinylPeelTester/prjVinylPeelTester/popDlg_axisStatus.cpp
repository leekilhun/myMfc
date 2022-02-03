// popDlg_axisStatus.cpp: 구현 파일
//

#include "pch.h"
#include "prjVinylPeelTester.h"
#include "popDlg_axisStatus.h"
#include "afxdialogex.h"
#include "tabMotor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define UPDATE_TIMER__AXIS_STATE_DLG 3  // timer id

// popDlg_axisStatus 대화 상자

IMPLEMENT_DYNAMIC(popDlg_axisStatus, CDialogEx)

popDlg_axisStatus::popDlg_axisStatus(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_POPDLG_STATUS, pParent)
	,m_TimerID(0),m_pParent(nullptr)
{
	m_pParent = (tabMotor*)pParent;
}

popDlg_axisStatus::~popDlg_axisStatus()
{
	if (m_TimerID != 0)
	{
		KillTimer(m_TimerID);
		m_TimerID = 0;
	}
}

void popDlg_axisStatus::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	int idx = 0;
	DDX_Control(pDX, IDC_STATE1, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE2, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE3, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE4, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE5, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE6, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE7, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE8, m_state[idx++]);

	DDX_Control(pDX, IDC_STATE9, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE10, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE11, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE12, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE13, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE14, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE15, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE16, m_state[idx++]);

	DDX_Control(pDX, IDC_STATE17, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE18, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE19, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE20, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE21, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE22, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE23, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE24, m_state[idx++]);

	DDX_Control(pDX, IDC_STATE25, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE26, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE27, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE28, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE29, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE30, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE31, m_state[idx++]);
	DDX_Control(pDX, IDC_STATE32, m_state[idx++]);
}


BEGIN_MESSAGE_MAP(popDlg_axisStatus, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// popDlg_axisStatus 메시지 처리기


BOOL popDlg_axisStatus::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void popDlg_axisStatus::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (m_TimerID != 0)
	{
		KillTimer(m_TimerID);
		m_TimerID = 0;
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void popDlg_axisStatus::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (bShow == TRUE)
	{
		m_TimerID = SetTimer(UPDATE_TIMER__AXIS_STATE_DLG, 100, NULL);
	}
	else
	{
		if (m_TimerID != 0)
		{
			KillTimer(m_TimerID);
			m_TimerID = 0;
		}
	}
}


void popDlg_axisStatus::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	update();

	CDialogEx::OnTimer(nIDEvent);
}

void popDlg_axisStatus::update()
{
	int i = 0;
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_0][reg_::b0]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_0][reg_::b1]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_0][reg_::b2]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_0][reg_::b3]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_0][reg_::b4]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_0][reg_::b5]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_0][reg_::b6]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_0][reg_::b7]);

	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_1][reg_::b0]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_1][reg_::b1]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_1][reg_::b2]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_1][reg_::b3]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_1][reg_::b4]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_1][reg_::b5]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_1][reg_::b6]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_1][reg_::b7]);

	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_2][reg_::b0]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_2][reg_::b1]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_2][reg_::b2]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_2][reg_::b3]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_2][reg_::b4]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_2][reg_::b5]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_2][reg_::b6]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_2][reg_::b7]);

	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_3][reg_::b0]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_3][reg_::b1]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_3][reg_::b2]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_3][reg_::b3]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_3][reg_::b4]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_3][reg_::b5]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_3][reg_::b6]);
	m_state[i++].SetCheck(m_pParent->m_pMotor->m_motorState.axis_status[reg_bank::_3][reg_::b7]);
}
