#pragma once

class IAxis 
{

public:


	virtual int Initialize() = 0;

#if 0
	/**  ����Ÿ�� ���� �Ѵ�. */
	virtual int	SetData(SSMotionData* pData) = 0;

	/** ���� ����Ÿ�� ��ȸ �Ѵ�. */
	virtual int	GetData(SSMotionData* pData) = 0;


	virtual int LoadParameter() = 0;

	/**  ���Ͽ� ����Ÿ�� �����Ѵ�.  */
	virtual int SaveParameter(MAxisData* pData) = 0;

	/** �� Parameter (DataManager)�� ��´�. */
	virtual void GetParameter(MAxisData* pData) = 0;

	/**
	 * ���� �̸��� Return �Ѵ�.
	 * @return CString			: Axis Name
	 */
	virtual CString GetAxisName() = 0;

	/**
	 * Motion Component�� �ʱ�ȭ�Ѵ�.
	 *
	 * 1. Motion Board ������ �´� Motion Library ��ü�� �����Ѵ�.
	 * 2. �� ���� ������ �����Ѵ�.
	 * 3. �� ������ �����Ѵ�.
	 *
	 * @param	commonData		: ����
	 * @param	iBoardType		: Motion Board Type (1=MMC Board, 2=MEI board, 3=PPC Board, ...)
	 * @param	saxAxis			: 1�� ���� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXES NUMBER (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = INVALID MOTION BOARD TYPE (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Initialize(SCommonAttribute commonData, int iBoardType, SAxis1 saxAxis) = 0;

	/**
	 * Motion ������ ��� ������ Interlock ���ǿ� �ش�Ǵ� IO Address�� ���ذ��� �����Ѵ�.
	 *
	 * @param	usiIOAddress : ������ Interlock IO Address
	 * @param	bJudgeValue	: Interlock �Ǵ��� ��
	 * @return	Error Code : 0 = SUCCESS, �׿� = Error
	 */
	virtual void SetInterlockIOAddr(unsigned short usiIOAddress, BOOL bJudgeValue) = 0;
	virtual void SetInterlockIOAddr2(unsigned short usiIOAddress, BOOL bJudgeValue) = 0;
	virtual void SetInterlockIOAddr3(unsigned short usiIOAddress, BOOL bJudgeValue) = 0;
	virtual void SetInterlockIOAddr4(unsigned short usiIOAddress, BOOL bJudgeValue) = 0;

	virtual void SetSensorIOAddr(unsigned short usiIOAddress) = 0;

	/**
	 * �� 1���� ���� Data�� �����Ѵ�. (����ü)
	 *
	 *	(1) �� ���� ������ �־�� �Ѵ�.
	 *	(2) ���� �̹� ���ǰ� ������ �ȵȴ�.
	 *
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = USED AXIS ID (MULTIAXES)
	 *							  xx = NO EXIST AXIS PARAMETER AREA (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAxisData() = 0;


	/**
	* �� 1���� ���� Data�� �д´�. (����ü)
	*
	* @param   *ax1Data        : ������ ���� ���� Data
	* @return	Error Code		: 0 = SUCCESS
	*							  xx = INVALID AXIS ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  �� �� = Ÿ �Լ� Return Error
	*/
	virtual int GetAxisData(SAxis1* ax1Data) = 0;


	/**
	 * Board�� ���� �ڵ� ��, ���� ��뿩�θ� �����Ѵ�.
	 *
	 * @param   iBoardNo        : MMC Board ��ȣ 0 ~ 7, -1 = All Board
	 * @param   *pbAutoSet      : �ڵ� ��,���� ��������, TRUE : ����, FALSE : �ڵ�, iBoardNo=-1�̸� �迭�� ����
	 * @return	Error Code		: 0 = SUCCESS
	 *							  xx = INVALID MOTION BOARD ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAutoCP(int iBoardNo, BOOL* pbAutoSet) = 0;


	/**
	* Board�� ���� �ڵ� ��, ���� ��뿩�θ� �д´�.
	*
	* @param   iBoardNo        : MMC Board ��ȣ 0 ~ 7, -1 = All Board
	* @param   *pbAutoSet      : �ڵ� ��,���� ��������, TRUE : ����, FALSE : �ڵ�, iBoardNo=-1�̸� �迭�� ����
	* @return	Error Code		: 0 = SUCCESS
	*							  xx = INVALID MOTION BOARD ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  �� �� = Ÿ �Լ� Return Error
	*/
	virtual int GetAutoCP(int iBoardNo, BOOL* pbAutoSet) = 0;

	/**
	 * �� 1���� ���� Axis ID�� �д´�.
	 *
	 * @return	Error Code		: Axis ID
	 */
	virtual int GetAxisID() = 0;

	/**
	 * �� 1���� ���� Home ��ġ(����������ġ)�� �д´�.
	 *
	 * @return  double		 : ���� dHomePosition
	 */
	virtual double GetHomeOffset() = 0;

	/**
	 * �� 1���� ���� Negative Limit ��ġ�� �д´�.
	 *
	 * @return   double     : ���� dNegativeLimit Position
	 */
	virtual double GetNegativeSWLimit() = 0;

	/**
	 * �� 1���� ���� Positive Limit ��ġ�� �д´�.
	 *
	 * @return   double     : ���� dPositiveLimit Position
	 */
	virtual double GetPositiveSWLimit() = 0;

	/**
	 * �� 1���� ���� Moving�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return  double  : ���� dMovingVelocity
	 */
	virtual double GetMovingVelocity(BOOL bNormal = TRUE) = 0;
	virtual double GetMovingVelocityNormal() = 0;
	virtual double GetMovingVelocitySlow() = 0;

	/**
	 * �� 1���� ���� Moving�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return double  : ���� iMovingAccelerate
	 */
	virtual int GetMovingAccelerate() = 0;

	/**
	 * �� 1���� ���� Coarse�ӵ�, ���ӵ��� �д´�.
	 *
	 * return   double      : ���� dCoarseVelocity
	 */
	virtual double GetCoarseVelocity() = 0;
	/**
	 * �� 1���� ���� Coarse�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return   int    : ���� iCoarseAccelerate
	 */
	virtual int GetCoarseAccelerate() = 0;

	/**
	 * �� 1���� ���� Fine�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return  double     : ���� dFineVelocity
	 */
	virtual double GetFineVelocity() = 0;

	/**
	 * �� 1���� ���� Fine�ӵ�, ���ӵ��� �д´�.
	 *
	 * @return   int    : ���� iFineAccelerate
	 */
	virtual int GetFineAccelerate() = 0;

	/**
	 * �� 1���� ���� Jog Move�� Slow, �ӵ��� �д´�.
	 *
	 * @return   double         : ���� dJogSlow
	 */
	virtual double GetJogSlow() = 0;

	/**
	 * �� 1���� ���� Jog Move�� Fast, �ӵ��� �д´�.
	 *
	 * @return   double     : ���� dJogFast
	 */
	virtual double GetJogFast() = 0;

	/**
	 * �� 1���� ���� Sign�� �д´�.
	 *
	 * @return   BOOL          : ���� bSign
	 */
	virtual BOOL GetSign() = 0;

	/**
	 * �� 1���� ���� Abs Encoder ��� ���θ� �д´�.
	 *
	 * @return   BOOL          : ���� bAbsData
	 */
	virtual BOOL IsAbsEncoder() = 0;

	/**
	 * �� 1���� ���� �������� ��������� �д´�.
	 *   Limit Sensor ������ ���� �������� �ʱ� ��������� ���� �� �ְ� �Ѵ�.
	 *
	 * @return   BOOL           : ������ bOriginDir (TRUE : +����, FALSE : -����)
	 */
	virtual BOOL GetOriginDir() = 0;

	/**
	 * �� 1���� ���� �������� ����(Fine����)������ �д´�.
	 *   Fine �ӵ� �������� �ʱ� ��������� ���� �� �ְ� �Ѵ�.
	 *
	 * @return   BOOL           : ������ bOriginDir (TRUE : +����, FALSE : -����)
	 */
	virtual BOOL GetOriginFineDir() = 0;

	/**
	 * �� 1���� ���� C�� ��뿩�θ� �д´�.
	 *
	 * @param   BOOL           : ���� bC`Use
	 */
	virtual BOOL GetCPhaseUse() = 0;

	/**
	 * �� 1���� ���� Scale�� �д´�.
	 *
	 * @return   double         : ���� dScale
	 */
	virtual double GetScale() = 0;

	/**
	 * �� �̵� �� ���� �ð��� �д´�.
	 *
	 * @return   double          : ������ �̵� ���� �ð� (�ʴ���)
	 */
	virtual double GetMoveTime() = 0;

	/**
	 * �� �̵� �� ����ȭ �ð��� �д´�.
	 *
	 * @return  double          : ������ �̵� �� ����ȭ �ð� (�ʴ���)
	 */
	virtual double GetMoveAfterTime() = 0;

	/**
	 * �� ��ġ ��� ������ �д´�.
	 *
	 * @return   double     : ������ ��ġ ��� ���� (mm����)
	 */
	virtual double GetTolerance() = 0;

	/**
	 * �� ��ġ ��� ������ �д´�.
	 *
	 * @return   double    : ������ Offset (mm����)
	 */
	virtual double GetPassOffset() = 0;

	/**
	 * �� �������� �Ϸ� ��� �ð�(��)�� �д´�.
	 *
	 * @return   double          : ������ �������� �Ϸ� ��� �ð� (�� ����)
	 */
	virtual double GetOriginWaitTime() = 0;


	/**
	 * ���� �������� �ƴ��� Ȯ���Ѵ�. (�Ѱ��� �࿡ ���� ����)
	 * Ȯ���ϰ��� �ϴ� �࿡ ���� ���� Code�� �о���� �Ѵ�.
	 *
	 * @return	: TRUE = �������� �Ϸ�
	 *            FALSE = �������� �̿Ϸ�
	 */
	virtual BOOL IsOriginReturn() = 0;

	virtual BOOL IsServoOn() = 0;

	/**
	 * �� �������� �����ϱ� (�Ѱ��� �࿡ ���� ����)
	 *
	 * @param   *pbReturn        : (OPTION = NULL) �������� ��� �б�
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ResetOrigin(BOOL* pbReturn = NULL) = 0;

	/**
	 * �� �������� ���� �����ϱ� (������ ��� �࿡ ���� ���� ���� ��� ����)
	 *
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int StopReturnOrigin(void) = 0;


	/**
	 * ���� ������ǥ�� �д´�.
	 *
	 * @return   double   : ���� ��ǥ��
	 * @param   bCommandType         : ���� ��ġ ����, FALSE=������ġ(Encode), TRUE=��ǥ��ġ(Command)
	 */
	virtual double GetCurrentPosition(BOOL bCommandType) = 0;


	/**
	 * ���� ������ǥ�� Ư����ǥ���� ��ġ�� ���� ��ǥ���̸� ���Ѵ�.
	 *
	 * @param	bPosOpt              : ���� ��ġ ����, FALSE=������ġ, TRUE=Motion�� ��ǥ��ġ
	 * @param   dTargetPosition      : ���� ��ǥ��
	 * @param   *pdPermission        : (OPTION = NULL) ����� ����, NULL�̸� ���� Tolerance������ ���Ѵ�.
	 * @param   *pdDeviation         : (OPTION = NULL) �� ���̰�
	 * @return  BOOL	             : �񱳰��
	 */
	virtual BOOL ComparePosition(BOOL bPosOpt, double dTargetPosition,
		double* pdPermission = NULL, double* pdDeviation = NULL) = 0;
	/**
	 * �� �̵� (�Ѱ��� �࿡ ���� �̵�) - �̵� �Ϸ�� �� return
	 *
	 * @param   dPosition        : �̵��� ��ġ
	 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	 * @param	iMoveType        : (OPTION=1) �̵� Type,
	 *											0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *											1=S-Curve �ӵ� Profile, ������ǥ �̵�
	 *											4=���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *											5=���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Move(double dPosition, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
		int iMoveType = DEF_MOVE_POSITION) = 0;

	virtual int Move_LINEAR(double lPosition_X, double lPosition_Y, DWORD dwVelocity_X = 0.0, DWORD dwVelocity_Y = 0.0, int iAccelerate = 0,
		int iDecelerate = 0, int iMoveType = DEF_SMOVE_POSITION, int eBoxNum = 0) = 0;

	/**
	 * �� �̵� (�Ѱ��� �࿡ ���� �̵�) - �̵� ��� �� �ٷ� return
	 *
	 * @param   dPosition        : �̵��� ��ġ
	 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	 * @param	iMoveType        : (OPTION=1) �̵� Type,
	 *											0=��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *											1=S-Curve �ӵ� Profile, ������ǥ �̵�
	 *											4=���Ī ��ٸ��� �ӵ� Profile, ������ǥ �̵�
	 *											5=���Ī S-Curve �ӵ� Profile, ������ǥ �̵�
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int StartMove(double dPosition, double dVelocity = 0.0, int iAccelerate = 0,
		int iDecelerate = 0, int iMoveType = DEF_MOVE_POSITION) = 0;

	virtual int StartMove_LINEAR(double dPosition_X, double dPosition_Y, double dVelocity_X = 0.0, double dVelocity_Y = 0.0, int iAccelerate = 0,
		int iDecelerate = 0, int iMoveType = DEF_SMOVE_POSITION) = 0;

	virtual int StartMove_LINEAR_Ex(double dPosition_X, double dPosition_Y, SAxis1 sAxisX, SAxis1 sAxisY, double dVelocity_X = 0.0, double dVelocity_Y = 0.0, int iAccelerate = 0,
		int iDecelerate = 0, int iMoveType = DEF_MOVE_POSITION) = 0;


	/**
	 * �� �̵� (�Ѱ��� �࿡ ���� �����ġ �̵�) - �̵� �Ϸ�� �� return
	 *
	 * @param   dDistance        : �̵��� �Ÿ�
	 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	 * @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	 * @param	iMoveType        : (OPTION=3) �̵� Type,
	 *											2=��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	 *											3=S-Curve �ӵ� Profile, ���Ÿ� �̵�
	 *											6=���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	 *											7=���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�
	 * @param	bClearOpt        : (OPTION=FALSE) �̵� ���� �Ŀ� Encoder ���� Clear�ϴ� ���� ��� ���� (TRUE:���, FALSE:�̻��)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int RMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
		int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE) = 0;


	/**
	* �� �̵� (�Ѱ��� �࿡ ���� �����ġ �̵�) - �̵� ��� �� �ٷ� return
	*
	* @param   dDistance        : �̵��� �Ÿ�
	* @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	* @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	* @param   iDecelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	* @param	iMoveType        : (OPTION=2) �̵� Type,
	*											2=��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	*											3=S-Curve �ӵ� Profile, ���Ÿ� �̵�
	*											6=���Ī ��ٸ��� �ӵ� Profile, ���Ÿ� �̵�
	*											7=���Ī S-Curve �ӵ� Profile, ���Ÿ� �̵�
	* @param	bClearOpt        : (OPTION=FALSE) �̵� ���� �Ŀ� Encoder ���� Clear�ϴ� ���� ��� ���� (TRUE:���, FALSE:�̻��)
		* @return	Error Code		 : 0 = SUCCESS
	*							  xx = INVALID AXIS ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  �� �� = Ÿ �Լ� Return Error
	*/
	virtual int StartRMove(double dDistance, double dVelocity = 0.0, int iAccelerate = 0, int iDecelerate = 0,
		int iMoveType = DEF_SMOVE_DISTANCE, BOOL bClearOpt = TRUE) = 0;


	/**
	 * �� �̵� (�Ѱ��� �࿡ ���� ��� �̵�, ��� ��ġ���� ���� �̵���)
	 *
	 * @param   dVelocity        : (OPTION=0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @param   iAccelerate      : (OPTION=0) �̵��� ���ӵ�, 0 = ������ ���ӵ� ���
	 * @param   bDir             : (OPTION=TRUE) �̵��� ����, TRUE:(+), FALSE:(-), �����ϸ� (+)�������� �̵�
	 *                             dVelocity�� ���� �־��ָ� bDir�� �����ؼ� ����ϸ� �ȴ�.
	 *                             �� ���� dVelocity�� ��ȣ�� ���� �̵� ������ �����ȴ�.
	 *                             dVelocity�� 0.0�� �־� ������ �ӵ��� ����ϴ� ����
	 *                             bDir�� (+/-) ������ ������ �� �ִ�.
	 *                             ����, dVelocity�� ���� ���� ��� bDir�� ������ �ϰ� �Ǹ�
	 *                             ������ dVelocuty, dAccelerate�� bDir�� �ݿ��Ǿ� �̵��� �ϰ� �ȴ�.
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int VMove(double dVelocity = 0.0, int iAccelerate = 0, BOOL bDir = TRUE) = 0;


	/**
	 * �� �̵� �� �ϷḦ Ȯ���Ѵ�. (�Ѱ��� �࿡ ���� �Ϸ� Ȯ��)
	 *
	 * @param   bSkipMode        : (OPTION=FALSE) ��ġ Ȯ�� ���, �̵� �� ����ȭ �ð� ���� ��� ����
	 * @param	bUseInterlock    : (OPTION=FALSE) Interlock�� ���� ���� ���� ��� ��� ���� (FALSE:������, TRUE:�����)
		 * @param	bRMove		     : (OPTION=FALSE) RMove�� ��� ��� ���� (FALSE:������, TRUE:�����)
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = TIMEOUT MOVE-TIME (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Wait4Done(BOOL bSkipMode = FALSE, BOOL bUseInterlock = FALSE, BOOL bRMove = FALSE) = 0;

	/**
	* �� �̵� �� �ϷḦ Ȯ���Ѵ�. (�Ѱ��� �࿡ ���� �Ϸ� Ȯ��)
	*
	* @param   bSkipMode        : (OPTION=FALSE) ��ġ Ȯ�� ���, �̵� �� ����ȭ �ð� ���� ��� ����
	* @param	bUseInterlock    : (OPTION=FALSE) Interlock�� ���� ���� ���� ��� ��� ���� (FALSE:������, TRUE:�����)
	* @param	bRMove		     : (OPTION=FALSE) RMove�� ��� ��� ���� (FALSE:������, TRUE:�����)
	* @return	Error Code		 : 0 = SUCCESS
	*							  xx = INVALID AXIS ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  xx = TIMEOUT MOVE-TIME (MULTIAXES)
	*							  �� �� = Ÿ �Լ� Return Error
	*/
	virtual int Wait4Done_Sensor(BOOL bSkipMode = FALSE, BOOL bUseInterlock = FALSE, BOOL bJudge = TRUE, BOOL bRMove = FALSE) = 0;

	/**
	 * ���� ���� �Ϸ� ���θ� Return �Ѵ�. (����� : 2005.01.11)
	 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ�� �Ǵ� ���Ҿ� In-Position ���α��� Ȯ���Ѵ�.
	 *
	 * MMC Library : axis_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
	 */
	virtual BOOL IsAxisDone() = 0;


	/**
	 * ���� ���� �Ϸ� ���θ� Return �Ѵ�. (����� : 2005.01.11)
	 * �̵� �� �ӵ� �̵� �Ϸ� Ȯ���Ѵ�.  In-Position Ȯ�ξ���
	 *
	 * MMC Library : motion_done()
	 *
	 * @return	: TRUE = AXIS_DONE
	 *			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
	 */
	virtual BOOL IsMotionDone() = 0;

	/**
	* ���� ���� ���� �Ϸ� ���θ� Return �Ѵ�.
	* �̵� �� �ӵ� �̵� �Ϸ� Ȯ���Ѵ�.  In-Position Ȯ�ξ���
	*
	* MMC Library : motion_done()
	*
	* @return	: TRUE = AXIS_DONE
	*			  FALSE = AXIS IS IN COMMAND (MOTIONLIB)
	*/
	virtual BOOL IsContiMotionDone() = 0;

	/**
	 * �������� �̵��Ѵ�.
	 *
	 * @param   iMaxPoint        : �������� �̵����� ����
	 * @param   *pdPosition      : �������� �̵����� ����, iMaxPoint��ŭ ����
	 * @param   *pdVelocity      : �̵� �� �ӵ�, iMaxPoint��ŭ ���� (Scale�� ������� �ʴ´�.)
	 * @param   *piAccelerate    : �̵� �� ���ӵ�, iMaxPoint��ŭ ����
	 * @param	bAutoFlag        : Auto CP ����, FALSE=AutoCP ������, TRUE=AutoCP �����
	 * @param	bWaitOpt         : (OPTION=TRUE)�� �̵� �Ϸ� ��� ����, FALSE=��� ����, TRUE=�����
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  xx = NO EXIST AXIS ID TO MOVE (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int MoveSplineLine(int iMaxPoint, double* pdPosition, double* pdVelocity, int* piAccelerate,
		BOOL bAutoFlag, BOOL bWaitOpt = TRUE) = 0;

	/**
	* ���� �������� �̵��Ѵ�.
	*
	* @param	iMaxPoint		: �������� �̵����� ����
	* @param	*pdPosition_X	: �������� �̵�����(X��ǥ) ����, iMaxPoint��ŭ ����
	* @param	*pdPosition_Y	: �������� �̵�����(Y��ǥ) ����, iMaxPoint��ŭ ����
	* @param	*pdPosition_Z	: �������� �̵�����(Z��ǥ) ����, iMaxPoint��ŭ ����
	* @param	*pdVelocity     : �̵� �� �ӵ�, iMaxPoint��ŭ ���� (Scale�� ������� �ʴ´�.)
	* @param	*piAccelerate   : �̵� �� ���ӵ�, iMaxPoint��ŭ ����
	* @param	bAutoFlag		: Auto CP ����, FALSE=AutoCP ������, TRUE=AutoCP �����
	* @param	bWaitOpt        : (OPTION=TRUE)�� �̵� �Ϸ� ��� ����, FALSE=��� ����, TRUE=�����
	* @return	Error Code		: 0 = SUCCESS
	*							  xx = INVALID AXIS ID (MULTIAXES)
	*							  xx = INVALID POINTER (MULTIAXES)
	*							  xx = NO EXIST AXIS ID TO MOVE (MULTIAXES)
	*							  �� �� = Ÿ �Լ� Return Error
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
	 * Jog Slow�� ���� �̵��Ѵ�.
	 *
	 * @param   bDir             : �̵��� ����, TRUE:(+), FALSE:(-)
	 * @param   dSlow           : (OPTION = 0.0) �̵��� �Ÿ�, 0.0 = ������ Slow�Ÿ� ���
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int JogMoveSlow(BOOL bDir, double dSlow = 0.0) = 0;


	/**
	 * Jog Fast�� ���� �̵��Ѵ�.
	 *
	 * @param   bDir             : �̵��� ����, TRUE:(+), FALSE:(-)
	 * @param   dFast        : (OPTION = 0.0) �̵��� �ӵ�, 0.0 = ������ �ӵ� ���
	 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int JogMoveFast(BOOL bDir, double dFast = 0.0) = 0;


	/**
	 * ���� �����Ѵ�. (�Ѱ��� �࿡ ���� ����)
	 *
	 * @param   *pbStatus      : (OPTION = NULL) ���� Stop ����
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int Stop(BOOL* pbStatus = NULL) = 0;


	/**
	 * ���� ����̵��� ���� �����Ѵ�. (�Ѱ� ���� ����̵��� ���� ����)
	 *
	 * @param   *pbState         : (OPTION = NULL) �� ���� VStop ����
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int VStop(BOOL* pbStatus = NULL) = 0;


	/**
	 * ���� ��������Ѵ�. (�Ѱ��� �࿡ ���� �������)
	 *
	 * @param   *pbStatus        : (OPTION = NULL) �� ���� EStop ����
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int EStop(BOOL* pbStatus = NULL) = 0;


	/**
	 * ���� Servo�� On �Ѵ�. (�Ѱ��� �࿡ ���� Servo On ����)
	 *
	 * @param   *pbStatus        : (OPTION = NULL) �� ���� Servo ON ����
	 * @param   *bLinearAxis     : (OPTION = FALSE) ���� Linear Type�̸� TRUE, �׿� FALSE
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ServoOn(BOOL* pbStatus = NULL, BOOL bLinearAxis = FALSE) = 0;
	//virtual int Q_ServoOn (BOOL *pbStatus = NULL) = 0;


	/**
	 * ���� Servo�� Off �Ѵ�. (�Ѱ��� �࿡ ���� Servo Off ����)
	 *
	 * @param   *pbStatus        : (OPTION = NULL) �� ���� Servo OFF ����
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ServoOff(BOOL* pbStatus = NULL) = 0;



	/**
	 * ���� Home Sensor�� ���� Event�� �����Ѵ�. (�Ѱ��� �࿡ ���� ���� ����)
	 *
	 * @param	iLimit           : ������ Event (0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetHomeSensorEvent(int iLimit) = 0;


	/**
	 * ���� Home Sensor�� ���� Level�� �����Ѵ�. (�Ѱ��� �࿡ ���� ���� ����)
	 *
	 * @param	bLevel         : ��ȣ Level (TRUE=HIGH, FLASE=LOW)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetHomeSensorLevel(BOOL bLevel) = 0;


	/**
	 * ���� Positive Sensor�� ���� Event�� �����Ѵ�. (�Ѱ��� �࿡ ���� ���� ����)
	 *
	 * @param	iLimit           : ������ Event (0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetPositiveSensorEvent(int iLimit) = 0;


	/**
	 * ���� Positive Sensor�� ���� Limit�� �����Ѵ�. (�Ѱ��� �࿡ ���� ���� ����)
	 *
	 * @param	bLevel           : ��ȣ Level (TRUE=HIGH, FLASE=LOW)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetPositiveSensorLevel(BOOL bLevel) = 0;


	/**
	 * ���� Negative Sensor�� ���� Event �����Ѵ�. (�Ѱ��� �࿡ ���� ���� ����)
	 *
	 * @param	iLimit           : ������ Event (0=NO EVENT, 1=STOP EVENT, 2=ESTOP EVENT, 3=ABORT EVENT)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetNegativeSensorEvent(int iLimit) = 0;


	/**
	 * ���� Negative Sensor�� ���� Level�� �����Ѵ�. (�Ѱ��� �࿡ ���� ���� ����)
	 *
	 * @param	bLevel           : ��ȣ Level (TRUE=HIGH, FLASE=LOW)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetNegativeSensorLevel(BOOL bLevel) = 0;


	/**
	 * ���� ����(Source)�� �д´�.
	 *
	 * @return   int       : �� �ϳ��� ���� ���� (Source)
	 */
	virtual int GetAxisSource() = 0;


	/**
	 * ���� ����(State)�� �д´�.
	 *
	 * @retunr  int         : �� �ϳ��� ���� ���� (State)
	 */
	virtual int GetAxisState() = 0;


	/**
	 * ���� AMP Enable ���¸� �д´�.
	 *
	 * @param   BOOL        : �࿡ ���� AMP���� (AMP Enable : TRUE, �׿� : FALSE)
	 */
	virtual BOOL GetAmpEnable() = 0;

	/**
	 * ���� AMP Enable/Disable ���·� �����. (����� : 2005.01.11)
	 *
	 * @param   bEnable          : AMP Enable ���·� ���� : TRUE
	 *							   AMP Disable ���·� ���� : FALSE
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetAmpEnable(BOOL bEnable, BOOL bLinearAxis = FALSE) = 0;


	/**
	 * ���� AMP Fault ���¸� �д´�.
	 *
	 * @return   BOOL         : �࿡ ���� AMP Fault���� (AMP Fault:TRUE, Fault�ƴϸ�:FALSE)
	 */
	virtual BOOL GetAmpFault() = 0;

	/**
	 * ���� AMP Fault ���¸� Reset �Ѵ�. (����� : 2005.01.11)
	 *
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID POINTER (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ResetAmpFault() = 0;



	/**
	 * ���� ���¸� �ʱ�ȭ �Ѵ�. (�Ѱ��� �࿡ ���� �ʱ�ȭ)
	 *  Clear Status & Clear Frames
	 *
	 * @param   *pbStatus        : (OPTION = NULL) �� ���� �ʱ�ȭ ����
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ClearAxis(BOOL* pbStatus = NULL) = 0;

	/**
	 * �������� �켱 ������ Return �Ѵ�.
	 *
	 * @return	�켱 ����		: 0~63
	 */
	virtual int GetOriginPriority() = 0;

	/**
	 * �������� ���� ���� �Լ� (�ѹ� ȣ�⿡ �� Step�� ����)
	 * ���� ���� Step�� �Ϸ� Step�� �� �� ���� ��� ȣ�� �ؾ���
	 *
	 * @param   bMove			: FALSE -> Motor�� ���� ���� ���� ��ġ���� ���� ���� �Ϸ��
																TRUE -> Motor�� �������� ���� ������ ã�� ���� ������
		 * @return	Origin Step		: 1000 = ORIGIN RETURN SUCCESS
	 *							  990 = ERROR
	 *							  xx = ���� ���� ��...
	 */
	virtual int OriginReturn(BOOL bMove) = 0;

	/**
	 * �������� Step�� �����Ѵ�. (�Ѱ��� ��)
	 *
	 * @param   iStep            : ������ (0:����, 999:����, 1000:�Ϸ�, �׿�:������)
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  xx = INVALID ORIGIN STEP (<0) (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int SetOriginStep(int iStep) = 0;


	/**
	 * �������� Step�� �д´�. (�Ѱ��� ��)
	 *
	 * @return	Origin Setp
	 */
	virtual int GetOriginStep() = 0;


	/**
	 * �������� Error�� �ʱ�ȭ�Ѵ�. (�Ѱ��� ��)
	 *
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int ClearOriginError() = 0;


	/**
	 * �������� Error�� �д´�. (�Ѱ��� ��)
	 *
	 * @param   *piError         : �߻��� ���� Code
		 * @return	Error Code		 : 0 = SUCCESS
	 *							  xx = INVALID AXIS ID (MULTIAXES)
	 *							  �� �� = Ÿ �Լ� Return Error
	 */
	virtual int GetOriginError(int* piError) = 0;

	/**
	 * Home Sensor�� ������ ��ȯ �Ѵ�.
	 *
		 * @return	ERoleOfHome		HOME_N_NONE : Home Sensor ����
								HOME_N_POSITIVE_LIMIT : Home, Positive Sensor ����
								HOME_N_NEGATIVE_LIMIT : Home, Negative Sensor ����
	 */
	virtual ERoleOfHome GetRoleOfHomeSensor() = 0;

	virtual BOOL IsEStopEvent() = 0;

#endif

};

