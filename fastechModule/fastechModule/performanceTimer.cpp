
#include "pch.h"
#include "performanceTimer.h"

performanceTimer::performanceTimer() :
	m_bIsStart(FALSE), m_StartClock({}), m_RecordClock({})
{
	// �ð� ������ CPU Clock�� �����´�.
	QueryPerformanceFrequency(&m_RecordClock);

	m_bIsStart = FALSE;
	StartTimer();
}

performanceTimer::~performanceTimer()
{
}

int performanceTimer::StartTimer()
{
	QueryPerformanceCounter(&m_StartClock);

	m_bIsStart = TRUE;
	return DEF_PERFORMANCETIMER_SUCCESS;
}

int performanceTimer::StopTimer()
{
	if (!m_bIsStart) return ERR_PERFORMANCETIMER_NOT_STARTED;

	SetThreadAffinityMask(::GetCurrentThread(), m_oldmask);

	m_bIsStart = FALSE;
	return DEF_PERFORMANCETIMER_SUCCESS;
}

double performanceTimer::GetElaspTime()
{
	if (!m_bIsStart)	return 0.;

	LARGE_INTEGER	iStop;		// Stop���� ��
	double			dElasp;	// ��� �ð� 

	QueryPerformanceCounter(&iStop);
	dElasp = (double)(iStop.QuadPart - m_StartClock.QuadPart) / (double)m_RecordClock.QuadPart;

	return dElasp;
}

BOOL performanceTimer::LessThan(double dSec)
{
	if (GetElaspTime() < dSec) return TRUE;
	else return FALSE;
}

BOOL performanceTimer::MoreThan(double dSec)
{
	if (GetElaspTime() > dSec) return TRUE;
	else return FALSE;
}

BOOL performanceTimer::IsTimerStarted() const
{
	return m_bIsStart;
}
