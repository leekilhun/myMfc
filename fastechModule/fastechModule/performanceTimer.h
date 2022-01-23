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
 *  CPU시간(하드웨어)을 리턴하는 QueryPerformanceCounter() 함수를 이용하여 경과 시간을 계산한다.
 */
class performanceTimer
{
	/****************************************************
	 *	data
	 ****************************************************/

	BOOL m_bIsStart;/* Timer가 시작 되었는가 */
	LARGE_INTEGER	m_StartClock;/* Timer Start시 Clock Value */
	LARGE_INTEGER	m_RecordClock;// CPU Clock 정보를 저장할 변수
	DWORD_PTR m_oldmask;



	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	/* 생성자 */
	performanceTimer();

	/* 소멸자 */
	virtual ~performanceTimer();


	/****************************************************
	 *	func
	 ****************************************************/
public:

	/**
	 * Timer를 시작 한다.
	 *
	 * @return 0 : 성공, others : 에러
	 */
	int	StartTimer();

	/**
	 * Timer를 종료 한다.
	 *
	 * @return 0 : 성공, others : 에러
	 */
	int	StopTimer();

	/**
	 * 경과 시간을 리턴한다.
	 *
	 *@ return double : 경과 시간 ()
	 */
	double GetElaspTime();

	/**
	 * 지정 시간을 초과 하지 않았는지 체크 한다.
	 *
	 * @param  dSec : 지정시간 (초단위)
	 * @return TRUE : 경과함
				 FALSE : 경과하지 않음
	 */
	BOOL LessThan(double dSec);

	/**
	 * 지정 시간을 초과 했는지 체크 한다.
	 *
	 * @param  dSec : 지정시간 (초단위)
	 * @return TRUE : 경과하지 않음
				 FALSE : 경과함
	 */
	BOOL MoreThan(double dSec);

	/**
	 * Timer 시작 여부를 알려준다.
	 */
	BOOL IsTimerStarted() const;
};