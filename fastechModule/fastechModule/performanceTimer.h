#pragma once

const int DEF_PERFORMANCETIMER_SUCCESS = 0;
const int ERR_PERFORMANCETIMER_NOT_STARTED = 1;
const int ERR_PERFORMANCETIMER_ALREADY_STARTED = 2;

#ifndef TRUE
#define TRUE	1
#endif

#ifndef FALSE
#define FALSE	0
#endif

/**
 *  CPU�ð�(�ϵ����)�� �����ϴ� QueryPerformanceCounter() �Լ��� �̿��Ͽ� ��� �ð��� ����Ѵ�.
 */
class performanceTimer
{
	/****************************************************
	 *	data
	 ****************************************************/

	BOOL m_bIsStart;/* Timer�� ���� �Ǿ��°� */
	LARGE_INTEGER	m_StartClock;/* Timer Start�� Clock Value */
	LARGE_INTEGER	m_RecordClock;// CPU Clock ������ ������ ����
	DWORD_PTR m_oldmask;



	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	/* ������ */
	performanceTimer();

	/* �Ҹ��� */
	virtual ~performanceTimer();


	/****************************************************
	 *	func
	 ****************************************************/
public:

	/**
	 * Timer�� ���� �Ѵ�.
	 *
	 * @return 0 : ����, others : ����
	 */
	int	StartTimer();

	/**
	 * Timer�� ���� �Ѵ�.
	 *
	 * @return 0 : ����, others : ����
	 */
	int	StopTimer();

	/**
	 * ��� �ð��� �����Ѵ�.
	 *
	 *@ return double : ��� �ð� ()
	 */
	double GetElaspTime();

	/**
	 * ���� �ð��� �ʰ� ���� �ʾҴ��� üũ �Ѵ�.
	 *
	 * @param  dSec : �����ð� (�ʴ���)
	 * @return TRUE : �����
				 FALSE : ������� ����
	 */
	BOOL LessThan(double dSec);

	/**
	 * ���� �ð��� �ʰ� �ߴ��� üũ �Ѵ�.
	 *
	 * @param  dSec : �����ð� (�ʴ���)
	 * @return TRUE : ������� ����
				 FALSE : �����
	 */
	BOOL MoreThan(double dSec);

	/**
	 * Timer ���� ���θ� �˷��ش�.
	 */
	BOOL IsTimerStarted() const;
};