#pragma once

class IAxis 
{

public:


	virtual int Initialize() = 0;

#if 0
	/**  데이타를 설정 한다. */
	virtual int	SetData(SSMotionData* pData) = 0;

	/** 설정 데이타를 조회 한다. */
	virtual int	GetData(SSMotionData* pData) = 0;


	virtual int LoadParameter() = 0;

	/**  파일에 데이타를 저장한다.  */
	virtual int SaveParameter(MAxisData* pData) = 0;

	/** 축 Parameter (DataManager)를 얻는다. */
	virtual void GetParameter(MAxisData* pData) = 0;

	/**
	 * 축의 이름을 Return 한다.
	 * @return CString			: Axis Name
	 */
	virtual CString GetAxisName() = 0;

	/**
	 * Motion Component를 초기화한다.
	 *
	 * 1. Motion Board 종류에 맞는 Motion Library 개체를 생성한다.
	 * 2. 축 구성 개수를 설정한다.
	 * 3. 축 정보를 설정한다.
	 *
	 * @param	commonData		: 설명
	 * @param	iBoardType		: Motion Board Type (1=MMC Board, 2=MEI board, 3=PPC Board, ...)
	 * @param	saxAxis			: 1축 구성 정보
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXES NUMBER (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = INVALID MOTION BOARD TYPE (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis) = 0;

	/**
	 * Motion 동작을 즉시 정지할 Interlock 조건에 해당되는 IO Address와 기준값을 설정한다.
	 *
	 * @param	usiIOAddress : 설정할 Interlock IO Address
	 * @param	bJudgeValue	: Interlock 판단할 값
	 * @return	Error Code : 0 = SUCCESS, 그외 = Error
	 */
	virtual void SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue) = 0;
	virtual void SetInterlockIOAddr2(unsigned short usiIOAddress, BOOL bJudgeValue) = 0;
	virtual void SetInterlockIOAddr3(unsigned short usiIOAddress, BOOL bJudgeValue) = 0;
	virtual void SetInterlockIOAddr4(unsigned short usiIOAddress, BOOL bJudgeValue) = 0;

	virtual void SetSensorIOAddr(unsigned short usiIOAddress) = 0;

	/**
	 * 축 1개에 대한 Data를 설정한다. (구조체)
	 *
	 *	(1) 축 정보 영역이 있어야 한다.
	 *	(2) 축이 이미 사용되고 있으면 안된다.
	 *
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = USED AXIS ID (MULTIAXES)
	 *							  xx = NO EXIST AXIS PARAMETER AREA (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetAxisData() = 0;


	/**
	* 축 1개에 대한 Data를 읽는다. (구조체)
	*
	* @param   *ax1Data        : 설정된 축의 설정 Data
	* @return	Error Code		: 0 = SUCCESS
	*							  xx = INVALID AXIS ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  그 외 = 타 함수 Return Error
	*/
	virtual int GetAxisData(SAxis1* ax1Data) = 0;


	/**
	 * Board에 대한 자동 가, 감속 사용여부를 설정한다.
	 *
	 * @param   iBoardNo        : MMC Board 번호 0 ~ 7, -1 = All Board
	 * @param   *pbAutoSet      : 자동 가,감속 설정여부, TRUE : 수동, FALSE : 자동, iBoardNo=-1이면 배열로 구성
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetAutoCP(int iBoardNo, BOOL* pbAutoSet) = 0;


	/**
	* Board에 대한 자동 가, 감속 사용여부를 읽는다.
	*
	* @param   iBoardNo        : MMC Board 번호 0 ~ 7, -1 = All Board
	* @param   *pbAutoSet      : 자동 가,감속 설정여부, TRUE : 수동, FALSE : 자동, iBoardNo=-1이면 배열로 구성
	* @return	Error Code		: 0 = SUCCESS
	*							  xx = INVALID MOTION BOARD ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  그 외 = 타 함수 Return Error
	*/
	virtual int GetAutoCP(int iBoardNo, BOOL* pbAutoSet) = 0;

	/**
	 * 축 1개에 대한 Axis ID를 읽는다.
	 *
	 * @return	Error Code		: Axis ID
	 */
	virtual int GetAxisID() = 0;

	/**
	 * 축 1개에 대한 Home 위치(원점복귀위치)를 읽는다.
	 *
	 * @return  double		 : 읽은 dHomePosition
	 */
	virtual double GetHomeOffset() = 0;

	/**
	 * 축 1개에 대한 Negative Limit 위치를 읽는다.
	 *
	 * @return   double     : 읽은 dNegativeLimit Position
	 */
	virtual double GetNegativeSWLimit() = 0;

	/**
	 * 축 1개에 대한 Positive Limit 위치를 읽는다.
	 *
	 * @return   double     : 읽은 dPositiveLimit Position
	 */
	virtual double GetPositiveSWLimit() = 0;

	/**
	 * 축 1개에 대한 Moving속도, 가속도를 읽는다.
	 *
	 * @return  double  : 읽은 dMovingVelocity
	 */
	virtual double GetMovingVelocity(BOOL bNormal = TRUE) = 0;
	virtual double GetMovingVelocityNormal() = 0;
	virtual double GetMovingVelocitySlow() = 0;

	/**
	 * 축 1개에 대한 Moving속도, 가속도를 읽는다.
	 *
	 * @return double  : 읽은 iMovingAccelerate
	 */
	virtual int GetMovingAccelerate() = 0;

	/**
	 * 축 1개에 대한 Coarse속도, 가속도를 읽는다.
	 *
	 * return   double      : 읽은 dCoarseVelocity
	 */
	virtual double GetCoarseVelocity() = 0;
	/**
	 * 축 1개에 대한 Coarse속도, 가속도를 읽는다.
	 *
	 * @return   int    : 읽은 iCoarseAccelerate
	 */
	virtual int GetCoarseAccelerate() = 0;

	/**
	 * 축 1개에 대한 Fine속도, 가속도를 읽는다.
	 *
	 * @return  double     : 읽은 dFineVelocity
	 */
	virtual double GetFineVelocity() = 0;

	/**
	 * 축 1개에 대한 Fine속도, 가속도를 읽는다.
	 *
	 * @return   int    : 읽은 iFineAccelerate
	 */
	virtual int GetFineAccelerate() = 0;

	/**
	 * 축 1개에 대한 Jog Move의 Slow, 속도를 읽는다.
	 *
	 * @return   double         : 읽은 dJogSlow
	 */
	virtual double GetJogSlow() = 0;

	/**
	 * 축 1개에 대한 Jog Move의 Fast, 속도를 읽는다.
	 *
	 * @return   double     : 읽은 dJogFast
	 */
	virtual double GetJogFast() = 0;

	/**
	 * 축 1개에 대한 Sign을 읽는다.
	 *
	 * @return   BOOL          : 읽은 bSign
	 */
	virtual BOOL GetSign() = 0;

	/**
	 * 축 1개에 대한 Abs Encoder 사용 여부를 읽는다.
	 *
	 * @return   BOOL          : 읽은 bAbsData
	 */
	virtual BOOL IsAbsEncoder() = 0;

	/**
	 * 축 1개에 대한 원점복귀 진행방향을 읽는다.
	 *   Limit Sensor 구성에 따른 원점복귀 초기 진행방향을 읽을 수 있게 한다.
	 *
	 * @return   BOOL           : 설정할 bOriginDir (TRUE : +방향, FALSE : -방향)
	 */
	virtual BOOL GetOriginDir() = 0;

	/**
	 * 축 1개에 대한 원점복귀 진행(Fine구간)방향을 읽는다.
	 *   Fine 속도 구간에서 초기 진행방향을 읽을 수 있게 한다.
	 *
	 * @return   BOOL           : 설정할 bOriginDir (TRUE : +방향, FALSE : -방향)
	 */
	virtual BOOL GetOriginFineDir() = 0;

	/**
	 * 축 1개에 대한 C상 사용여부를 읽는다.
	 *
	 * @param   BOOL           : 읽은 bC`Use
	 */
	virtual BOOL GetCPhaseUse() = 0;

	/**
	 * 축 1개에 대한 Scale을 읽는다.
	 *
	 * @return   double         : 읽은 dScale
	 */
	virtual double GetScale() = 0;

	/**
	 * 축 이동 시 지연 시간을 읽는다.
	 *
	 * @return   double          : 설정된 이동 지연 시간 (초단위)
	 */
	virtual double GetMoveTime() = 0;

	/**
	 * 축 이동 후 안정화 시간을 읽는다.
	 *
	 * @return  double          : 설정된 이동 후 안정화 시간 (초단위)
	 */
	virtual double GetMoveAfterTime() = 0;

	/**
	 * 축 위치 허용 오차를 읽는다.
	 *
	 * @return   double     : 설정된 위치 허용 오차 (mm단위)
	 */
	virtual double GetTolerance() = 0;

	/**
	 * 축 위치 허용 오차를 읽는다.
	 *
	 * @return   double    : 설정된 Offset (mm단위)
	 */
	virtual double GetPassOffset() = 0;

	/**
	 * 축 원점복귀 완료 대기 시간(초)을 읽는다.
	 *
	 * @return   double          : 설정된 원점복귀 완료 대기 시간 (초 단위)
	 */
	virtual double GetOriginWaitTime() = 0;


	/**
	 * 축이 원점복귀 됐는지 확인한다. (한개의 축에 대해 가능)
	 * 확인하고자 하는 축에 대해 오류 Code를 읽어봐야 한다.
	 *
	 * @return	: TRUE = 원점복귀 완료
	 *            FALSE = 원점복귀 미완료
	 */
	virtual BOOL IsOriginReturn() = 0;

	virtual BOOL IsServoOn() = 0;

	/**
	 * 축 원점복귀 해제하기 (한개의 축에 대해 가능)
	 *
	 * @param   *pbReturn        : (OPTION = NULL) 원점복귀 결과 읽기
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ResetOrigin(BOOL* pbReturn = NULL) = 0;

	/**
	 * 축 원점복귀 강제 종료하기 (구성된 모든 축에 대해 동작 정지 명령 수행)
	 *
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int StopReturnOrigin(void) = 0;


	/**
	 * 축의 현재좌표를 읽는다.
	 *
	 * @return   double   : 현재 좌표값
	 * @param   bCommandType         : 읽을 위치 종류, FALSE=실제위치(Encode), TRUE=목표위치(Command)
	 */
	virtual double GetCurrentPosition(BOOL bCommandType) = 0;


	/**
	 * 축의 현재좌표와 특정좌표간의 수치에 의한 좌표차이를 비교한다.
	 *
	 * @param	bPosOpt              : 비교할 위치 종류, FALSE=현재위치, TRUE=Motion의 목표위치
	 * @param   dTargetPosition      : 비교할 좌표값
	 * @param   *pdPermission        : (OPTION = NULL) 비교허용 오차, NULL이면 내부 Tolerance값으로 비교한다.
	 * @param   *pdDeviation         : (OPTION = NULL) 비교 차이값
	 * @return  BOOL	             : 비교결과
	 */
	virtual BOOL ComparePosition(BOOL bPosOpt, double dTargetPosition,
		double* pdPermission = NULL, double* pdDeviation = NULL) = 0;
	/**
	 * 축 이동 (한개의 축에 대한 이동) - 이동 완료된 후 return
	 *
	 * @param   dPosition        : 이동할 위치
	 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용
	 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용
	 * @param	iMoveType        : (OPTION=1) 이동 Type,
	 *											0=사다리꼴 속도 Profile, 절대좌표 이동
	 *											1=S-Curve 속도 Profile, 절대좌표 이동
	 *											4=비대칭 사다리꼴 속도 Profile, 절대좌표 이동
	 *											5=비대칭 S-Curve 속도 Profile, 절대좌표 이동
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Move(double dPosition, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
		int iMoveType = DEF_MOVE_POSITION) = 0;

	virtual int Move_LINEAR(double lPosition_X, double lPosition_Y, DWORD dwVelocity_X = 0.0, DWORD dwVelocity_Y = 0.0, int iAccelerate = 0,
		int iDecelerate = 0, int iMoveType = DEF_SMOVE_POSITION, int eBoxNum = 0) = 0;

	/**
	 * 축 이동 (한개의 축에 대한 이동) - 이동 명령 후 바로 return
	 *
	 * @param   dPosition        : 이동할 위치
	 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용
	 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용
	 * @param	iMoveType        : (OPTION=1) 이동 Type,
	 *											0=사다리꼴 속도 Profile, 절대좌표 이동
	 *											1=S-Curve 속도 Profile, 절대좌표 이동
	 *											4=비대칭 사다리꼴 속도 Profile, 절대좌표 이동
	 *											5=비대칭 S-Curve 속도 Profile, 절대좌표 이동
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int StartMove(double dPosition, double dVelocity = 0.0, int iAccelerate = 0,
		int iDecelerate = 0, int iMoveType = DEF_MOVE_POSITION) = 0;

	virtual int StartMove_LINEAR(double dPosition_X, double dPosition_Y, double dVelocity_X = 0.0, double dVelocity_Y = 0.0, int iAccelerate = 0,
		int iDecelerate = 0, int iMoveType = DEF_SMOVE_POSITION) = 0;

	virtual int StartMove_LINEAR_Ex(double dPosition_X, double dPosition_Y, SAxis1 sAxisX, SAxis1 sAxisY, double dVelocity_X = 0.0, double dVelocity_Y = 0.0, int iAccelerate = 0,
		int iDecelerate = 0, int iMoveType = DEF_MOVE_POSITION) = 0;


	/**
	 * 축 이동 (한개의 축에 대한 상대위치 이동) - 이동 완료된 후 return
	 *
	 * @param   dDistance        : 이동할 거리
	 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용
	 * @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용
	 * @param	iMoveType        : (OPTION=3) 이동 Type,
	 *											2=사다리꼴 속도 Profile, 상대거리 이동
	 *											3=S-Curve 속도 Profile, 상대거리 이동
	 *											6=비대칭 사다리꼴 속도 Profile, 상대거리 이동
	 *											7=비대칭 S-Curve 속도 Profile, 상대거리 이동
	 * @param	bClearOpt        : (OPTION=FALSE) 이동 전과 후에 Encoder 값을 Clear하는 동작 사용 여부 (TRUE:사용, FALSE:미사용)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int RMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
		int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE) = 0;


	/**
	* 축 이동 (한개의 축에 대한 상대위치 이동) - 이동 명령 후 바로 return
	*
	* @param   dDistance        : 이동할 거리
	* @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	* @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용
	* @param   iDecelerate      : (OPTION=0) 이동할 감속도, 0 = 지정된 감속도 사용
	* @param	iMoveType        : (OPTION=2) 이동 Type,
	*											2=사다리꼴 속도 Profile, 상대거리 이동
	*											3=S-Curve 속도 Profile, 상대거리 이동
	*											6=비대칭 사다리꼴 속도 Profile, 상대거리 이동
	*											7=비대칭 S-Curve 속도 Profile, 상대거리 이동
	* @param	bClearOpt        : (OPTION=FALSE) 이동 전과 후에 Encoder 값을 Clear하는 동작 사용 여부 (TRUE:사용, FALSE:미사용)
		* @return	Error Code		 : 0 = SUCCESS
	*							  xx = INVALID AXIS ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  그 외 = 타 함수 Return Error
	*/
	virtual int StartRMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
		int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE) = 0;


	/**
	 * 축 이동 (한개의 축에 대한 등속 이동, 등속 위치까진 가속 이동함)
	 *
	 * @param   dVelocity        : (OPTION=0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @param   iAccelerate      : (OPTION=0) 이동할 가속도, 0 = 지정된 가속도 사용
	 * @param   bDir             : (OPTION=TRUE) 이동할 방향, TRUE:(+), FALSE:(-), 생략하면 (+)방향으로 이동
	 *                             dVelocity에 값을 넣어주면 bDir은 생략해서 사용하면 된다.
	 *                             이 경우는 dVelocity의 부호에 의해 이동 방향이 결정된다.
	 *                             dVelocity에 0.0을 넣어 지정된 속도를 사용하는 경우는
	 *                             bDir로 (+/-) 방향을 설정할 수 있다.
	 *                             만약, dVelocity에 값을 넣은 경우 bDir을 설정을 하게 되면
	 *                             지정된 dVelocuty, dAccelerate에 bDir이 반영되어 이동을 하게 된다.
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int VMove(double dVelocity = 0.0, int iAccelerate = 0, BOOL bDir = TRUE) = 0;


	/**
	 * 축 이동 후 완료를 확인한다. (한개의 축에 대한 완료 확인)
	 *
	 * @param   bSkipMode        : (OPTION=FALSE) 위치 확인 대기, 이동 후 안정화 시간 지연 사용 여부
	 * @param	bUseInterlock    : (OPTION=FALSE) Interlock에 의한 동작 정지 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
		 * @param	bRMove		     : (OPTION=FALSE) RMove시 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = TIMEOUT MOVE-TIME (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Wait4Done(BOOL bSkipMode = FALSE, BOOL bUseInterlock = FALSE, BOOL bRMove = FALSE) = 0;

	/**
	* 축 이동 후 완료를 확인한다. (한개의 축에 대한 완료 확인)
	*
	* @param   bSkipMode        : (OPTION=FALSE) 위치 확인 대기, 이동 후 안정화 시간 지연 사용 여부
	* @param	bUseInterlock    : (OPTION=FALSE) Interlock에 의한 동작 정지 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
	* @param	bRMove		     : (OPTION=FALSE) RMove시 기능 사용 여부 (FALSE:사용안함, TRUE:사용함)
	* @return	Error Code		 : 0 = SUCCESS
	*							  xx = INVALID AXIS ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  xx = TIMEOUT MOVE-TIME (MULTIAXES)
	*							  그 외 = 타 함수 Return Error
	*/
	virtual int Wait4Done_Sensor(BOOL bSkipMode = FALSE, BOOL bUseInterlock = FALSE, BOOL bJudge = TRUE, BOOL bRMove = FALSE) = 0;

	/**
	 * 축의 동작 완료 여부를 Return 한다. (정헌식 : 2005.01.11)
	 * 이동 및 속도 이동 완료 확인 또는 더불어 In-Position 여부까지 확인한다.
	 *
	 * MMC Library : axis_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
	 */
	virtual BOOL IsAxisDone() = 0;


	/**
	 * 축의 동작 완료 여부를 Return 한다. (정헌식 : 2005.01.11)
	 * 이동 및 속도 이동 완료 확인한다.  In-Position 확인안함
	 *
	 * MMC Library : motion_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
	 */
	virtual BOOL IsMotionDone() = 0;

	/**
	* 축의 연속 동작 완료 여부를 Return 한다.
	* 이동 및 속도 이동 완료 확인한다.  In-Position 확인안함
	*
	* MMC Library : motion_done()
	*
	* @return	: TRUE = AXIS_DONE
	*			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
	*/
	virtual BOOL IsContiMotionDone() = 0;

	/**
	 * 직선보간 이동한다.
	 *
	 * @param   iMaxPoint        : 직선보간 이동구간 개수
	 * @param   *pdPosition      : 직선보간 이동구간 지정, iMaxPoint만큼 설정
	 * @param   *pdVelocity      : 이동 시 속도, iMaxPoint만큼 설정 (Scale은 적용되지 않는다.)
	 * @param   *piAccelerate    : 이동 시 가속도, iMaxPoint만큼 설정
	 * @param	bAutoFlag        : Auto CP 여부, FALSE=AutoCP 사용안함, TRUE=AutoCP 사용함
	 * @param	bWaitOpt         : (OPTION=TRUE)축 이동 완료 대기 여부, FALSE=대기 안함, TRUE=대기함
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = NO EXIST AXIS ID TO MOVE (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int MoveSplineLine(int iMaxPoint, double* pdPosition, double* pdVelocity, int* piAccelerate,
		BOOL bAutoFlag, BOOL bWaitOpt = TRUE) = 0;

	/**
	* 연속 직선보간 이동한다.
	*
	* @param	iMaxPoint		: 직선보간 이동구간 개수
	* @param	*pdPosition_X	: 직선보간 이동구간(X좌표) 지정, iMaxPoint만큼 설정
	* @param	*pdPosition_Y	: 직선보간 이동구간(Y좌표) 지정, iMaxPoint만큼 설정
	* @param	*pdPosition_Z	: 직선보간 이동구간(Z좌표) 지정, iMaxPoint만큼 설정
	* @param	*pdVelocity     : 이동 시 속도, iMaxPoint만큼 설정 (Scale은 적용되지 않는다.)
	* @param	*piAccelerate   : 이동 시 가속도, iMaxPoint만큼 설정
	* @param	bAutoFlag		: Auto CP 여부, FALSE=AutoCP 사용안함, TRUE=AutoCP 사용함
	* @param	bWaitOpt        : (OPTION=TRUE)축 이동 완료 대기 여부, FALSE=대기 안함, TRUE=대기함
	* @return	Error Code		: 0 = SUCCESS
	*							  xx = INVALID AXIS ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  xx = NO EXIST AXIS ID TO MOVE (MULTIAXES)
	*							  그 외 = 타 함수 Return Error
	*/
	virtual int MoveContiLine_XYZ(int iMaxPoint,
		double* pdPosition_X,
		double* pdPosition_Y,
		double* pdPosition_Z,
		double* pdVelocity,
		int* piAccelerate,
		SAxis1 sAxis_X,
		SAxis1 sAxis_Y,
		SAxis1 sAxis_Z,
		MAxisData AxisData_X,
		MAxisData AxisData_Y,
		MAxisData AxisData_Z,
		BOOL bAutoFlag,
		BOOL bWaitOpt = TRUE) = 0;


	/**
	 * Jog Slow에 의한 이동한다.
	 *
	 * @param   bDir             : 이동할 방향, TRUE:(+), FALSE:(-)
	 * @param   dSlow           : (OPTION = 0.0) 이동할 거리, 0.0 = 지정된 Slow거리 사용
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int JogMoveSlow(BOOL bDir, double dSlow = 0.0) = 0;


	/**
	 * Jog Fast에 의한 이동한다.
	 *
	 * @param   bDir             : 이동할 방향, TRUE:(+), FALSE:(-)
	 * @param   dFast        : (OPTION = 0.0) 이동할 속도, 0.0 = 지정된 속도 사용
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int JogMoveFast(BOOL bDir, double dFast = 0.0) = 0;


	/**
	 * 축을 정지한다. (한개의 축에 대한 정지)
	 *
	 * @param   *pbStatus      : (OPTION = NULL) 축의 Stop 상태
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int Stop(BOOL* pbStatus = NULL) = 0;


	/**
	 * 축을 등속이동에 대해 정지한다. (한개 축의 등속이동에 대한 정지)
	 *
	 * @param   *pbState         : (OPTION = NULL) 각 축의 VStop 상태
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int VStop(BOOL* pbStatus = NULL) = 0;


	/**
	 * 축을 비상정지한다. (한개의 축에 대한 비상정지)
	 *
	 * @param   *pbStatus        : (OPTION = NULL) 각 축의 EStop 상태
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int EStop(BOOL* pbStatus = NULL) = 0;


	/**
	 * 축의 Servo를 On 한다. (한개의 축에 대한 Servo On 수행)
	 *
	 * @param   *pbStatus        : (OPTION = NULL) 각 축의 Servo ON 상태
	 * @param   *bLinearAxis     : (OPTION = FALSE) 축이 Linear Type이면 TRUE, 그외 FALSE
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ServoOn(BOOL* pbStatus = NULL, BOOL bLinearAxis = FALSE) = 0;
	//virtual int Q_ServoOn (BOOL *pbStatus = NULL) = 0;


	/**
	 * 축의 Servo를 Off 한다. (한개의 축에 대한 Servo Off 수행)
	 *
	 * @param   *pbStatus        : (OPTION = NULL) 각 축의 Servo OFF 상태
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ServoOff(BOOL* pbStatus = NULL) = 0;



	/**
	 * 축의 Home Sensor에 대한 Event를 설정한다. (한개의 축에 대한 상태 설정)
	 *
	 * @param	iLimit           : 동작할 Event (0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetHomeSensorEvent(int iLimit) = 0;


	/**
	 * 축의 Home Sensor에 대한 Level을 설정한다. (한개의 축에 대한 상태 설정)
	 *
	 * @param	bLevel         : 신호 Level (TRUE=HIGH, FLASE=LOW)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetHomeSensorLevel(BOOL bLevel) = 0;


	/**
	 * 축의 Positive Sensor에 대한 Event를 설정한다. (한개의 축에 대한 상태 설정)
	 *
	 * @param	iLimit           : 동작할 Event (0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetPositiveSensorEvent(int iLimit) = 0;


	/**
	 * 축의 Positive Sensor에 대한 Limit를 설정한다. (한개의 축에 대한 상태 설정)
	 *
	 * @param	bLevel           : 신호 Level (TRUE=HIGH, FLASE=LOW)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetPositiveSensorLevel(BOOL bLevel) = 0;


	/**
	 * 축의 Negative Sensor에 대한 Event 설정한다. (한개의 축에 대한 상태 설정)
	 *
	 * @param	iLimit           : 동작할 Event (0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetNegativeSensorEvent(int iLimit) = 0;


	/**
	 * 축의 Negative Sensor에 대한 Level를 설정한다. (한개의 축에 대한 상태 설정)
	 *
	 * @param	bLevel           : 신호 Level (TRUE=HIGH, FLASE=LOW)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetNegativeSensorLevel(BOOL bLevel) = 0;


	/**
	 * 축의 상태(Source)를 읽는다.
	 *
	 * @return   int       : 축 하나에 대한 상태 (Source)
	 */
	virtual int GetAxisSource() = 0;


	/**
	 * 축의 상태(State)를 읽는다.
	 *
	 * @retunr  int         : 축 하나에 대한 상태 (State)
	 */
	virtual int GetAxisState() = 0;


	/**
	 * 축의 AMP Enable 상태를 읽는다.
	 *
	 * @param   BOOL        : 축에 대한 AMP상태 (AMP Enable : TRUE, 그외 : FALSE)
	 */
	virtual BOOL GetAmpEnable() = 0;

	/**
	 * 축의 AMP Enable/Disable 상태로 만든다. (정헌식 : 2005.01.11)
	 *
	 * @param   bEnable          : AMP Enable 상태로 변경 : TRUE
	 *							   AMP Disable 상태로 변경 : FALSE
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetAmpEnable(BOOL bEnable, BOOL bLinearAxis = FALSE) = 0;


	/**
	 * 축의 AMP Fault 상태를 읽는다.
	 *
	 * @return   BOOL         : 축에 대한 AMP Fault상태 (AMP Fault:TRUE, Fault아니면:FALSE)
	 */
	virtual BOOL GetAmpFault() = 0;

	/**
	 * 축의 AMP Fault 상태를 Reset 한다. (정헌식 : 2005.01.11)
	 *
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ResetAmpFault() = 0;



	/**
	 * 축의 상태를 초기화 한다. (한개의 축에 대해 초기화)
	 *  Clear Status & Clear Frames
	 *
	 * @param   *pbStatus        : (OPTION = NULL) 각 축의 초기화 상태
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ClearAxis(BOOL* pbStatus = NULL) = 0;

	/**
	 * 원점복귀 우선 순위를 Return 한다.
	 *
	 * @return	우선 순위		: 0~63
	 */
	virtual int GetOriginPriority() = 0;

	/**
	 * 원점복귀 동작 수행 함수 (한번 호출에 한 Step씩 수행)
	 * 원점 복귀 Step이 완료 Step이 될 때 까지 계속 호출 해야함
	 *
	 * @param   bMove			: FALSE -> Motor의 구동 없이 현재 위치에서 원점 복귀 완료됨
																TRUE -> Motor의 구동으로 원점 센서를 찾아 원점 복귀함
		 * @return	Origin Step		: 1000 = ORIGIN RETURN SUCCESS
	 *							  990 = ERROR
	 *							  xx = 원점 복귀 중...
	 */
	virtual int OriginReturn(BOOL bMove) = 0;

	/**
	 * 원점복귀 Step을 설정한다. (한개의 축)
	 *
	 * @param   iStep            : 설정값 (0:시작, 999:오류, 1000:완료, 그외:동작중)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID ORIGIN STEP (<0) (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int SetOriginStep(int iStep) = 0;


	/**
	 * 원점복귀 Step을 읽는다. (한개의 축)
	 *
	 * @return	Origin Setp
	 */
	virtual int GetOriginStep() = 0;


	/**
	 * 원점복귀 Error를 초기화한다. (한개의 축)
	 *
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int ClearOriginError() = 0;


	/**
	 * 원점복귀 Error를 읽는다. (한개의 축)
	 *
	 * @param   *piError         : 발생한 오류 Code
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  그 외 = 타 함수 Return Error
	 */
	virtual int GetOriginError(int* piError) = 0;

	/**
	 * Home Sensor의 역할을 반환 한다.
	 *
		 * @return	ERoleOfHome		HOME_N_NONE : Home Sensor 역할
								HOME_N_POSITIVE_LIMIT : Home, Positive Sensor 역할
								HOME_N_NEGATIVE_LIMIT : Home, Negative Sensor 역할
	 */
	virtual ERoleOfHome GetRoleOfHomeSensor() = 0;

	virtual BOOL IsEStopEvent() = 0;

#endif

};

