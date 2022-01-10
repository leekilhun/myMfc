#pragma once
#include "pch.h"

const	int	DEF_CYLINDER_SUCCESS = 0;		// Success
const	int DEF_MAX_CYLINDER_SOLENOID = 4;
const	int DEF_MAX_CYLINDER_SENSOR = 4;

const	int	ERR_CYLINDER_SUCCESS								= 0;
const	int	ERR_CYLINDER_TIMEOUT								= 1;
const	int	ERR_CYLINDER_INVALID_POS						= 2;
const	int	ERR_CYLINDER_FAIL_IN_LOADING_PARAM	= 3;
const	int	ERR_CYLINDER_FAIL_IN_SAVING_PARAM		= 4;


/** Cylinder Timer의 종류 정의
 * @stereotype enumeration */
enum class CylinderTime
{
	MOVING,
	NOSEN_MOVING,
	SETTLING_1,
	SETTLING_2
};

/*
 Cylinder 타입을 정의한다.
 */
enum class CylinderType
{
	UNKOWN,
	UP_DOWN,
	LEFT_RIGHT,
	FORWARD_BACKWARD,
	TURN_RETURN,
	LOCK_UNLOCK,
	UPSTREAM_DOWNSTREAM,
	CW_CCW,
	OPEN_CLOSE,
	UP_MID_DOWN,
	EXPAND_REDUCE,
	LEFT_MIDE_RIGHT,
	FRONT_MID_BACK,
	UPSTREAM_MID_DOWNSTREAM,
	UPSTREAM_DOWNSTREAM_VARIOUS_VELOCITY,
	UPSTREAM_MID_DOWNSTREAM_VARIOUS_VELOCITY,
};

/*
  Solenoid 타입을 정의한다.
 */
enum class SolenoidType
{
	UNKOWN,
	SINGLE,
	REVERSE_SINGLE,
	DOUBLE,
	DOUBLE_3WAY,
	DOUBLE_SOLENOID_VARIOUS_VELOCITY
};


class CylinderData
{
public:
	struct CylinderVal_t
	{
		/** 생성된 Cylinder id */
		int Cyl_id;
		/** 생성된 Cylinder 객체와 연관된 Solenoid 단동식일때는 하나 사용, 복동식일때는 2개 사용  */
		unsigned short solenoid[2];
		/** 생성된 Cylinder 객체와 연관된 가감속 Solenoid  +,- 방향 1개씩   */
		unsigned short acc_solenoid[2];
		/** Up Sensor  : 체크하고자 하는 갯수 만큼 지정 하고 나머지는 NULL로 한다. */
		unsigned short up_sensor[DEF_MAX_CYLINDER_SENSOR];
		/** Down Sensor : 체크하고자 하는 갯수 만큼 지정 하고 나머지는 NULL로 한다. */
		unsigned short down_sensor[DEF_MAX_CYLINDER_SENSOR];
		/** Middle Sensor : 등록된 Sensor들의 상태 체크  */
		unsigned short middle_sensor[DEF_MAX_CYLINDER_SENSOR];
		/** 가감속 센서 : +방향, - 방향 2개 밖에 지정할 수 없다. */
		unsigned short acc_sensor[2];
		/** Cylinder 타입 */
		CylinderType cyl_type;
		/** Solenoid 타입 */
		SolenoidType sol_type;
	};


	/****************************************************
	*	data
	****************************************************/
private:
	CylinderVal_t m_CylinderValue;
	
	TCHAR* m_CylName;
	TCHAR* m_FileName;
	
public:


	/****************************************************
	*	Constructor
	****************************************************/
public:
	CylinderData(LPCTSTR name, LPCTSTR file) : m_CylinderValue({0,}), m_CylName(nullptr), m_FileName(nullptr) {
		int size = (int)_tcslen(name) + 1;
		m_CylName = new TCHAR[size];
		memset(m_CylName, 0x00, size * sizeof(TCHAR));
		_tcsncat_s(m_CylName, size, name, 256);

		size = (int)_tcslen(file) + 1;
		m_FileName = new TCHAR[size];
		memset(m_FileName, 0x00, size * sizeof(TCHAR));
		_tcsncat_s(m_FileName, size, file, 256);
	}

	virtual ~CylinderData() {
		if (m_CylName != nullptr) {
			delete m_CylName;
		}
		if (m_FileName != nullptr) {
			delete m_FileName;
		}
		m_FileName = nullptr;
		m_CylName = nullptr;
	}

	/****************************************************
	 *	func
	 ****************************************************/





};



class ICylinder
{

	/****************************************************
	 *	파라미터 설정
	 ****************************************************/

  /// <summary>
  /// Cylinder의 Moving Time과, Settling Time, NO Sensor Moving Time을 읽어드린다.
  /// </summary>
  /// <param name="moveing_sec">Cylinder 이동시 걸리는 최대 시간 (초단위)</param>
  /// <param name="settling_sec_1">Cylinder가 1동작후  안정화 되는데 걸리는 시간 (초단위)</param>
  /// <param name="settling_sec_2">Cylinder가 2동작후  안정화 되는데 걸리는 시간 (초단위)</param>
  /// <param name="no_sensor_moving_sec">Cylinder 이동시 Sensor가 없을때의 Moving Time (초단위)</param>
  /// <returns> 0 = Success, 그 외 = Error</returns>
  virtual int GetCylinderTime(double& moveing_sec, double& settling_sec_1, double& settling_sec_2, double& no_sensor_moving_sec) = 0;

 

  /// <summary>
  /// Cylinder의 Moving Time과, Settling Time, NO Sensor Moving Time 중 하나를 읽어드린다.
  /// </summary>
  /// <param name="cylinder_time">MOVING_TIME, NOSEN_WAITING_TIME, SETTLING_TIME</param>
  /// <param name="cylinder_sec">eCylinderTime에서 결정된 Time의 Value (초단위)</param>
  /// <returns> 0 = Success, 그 외 = Error</returns>
  virtual int GetCylinderTime(CylinderTime cylinder_time, double& cylinder_sec) = 0;


  /****************************************************
   *	실린더 상태 확인
   ****************************************************/

   /**
    * Cylinder가 Up 되어 있는는지 확인한다.
    * @return : TRUE = UP , FALSE = UP 아님
    */
  virtual BOOL IsUp() = 0;

  /**
   * Cylinder가 Down 되어 있는지 확인한다.
   * @return : TRUE = DOWN , FALSE = DOWN 아님
   */
  virtual BOOL IsDown() = 0;

  /**
  * Cylinder가 Left 인지 확인한다.
  * @return : TRUE = LEFT , FALSE = LEFT 아님
  */
  virtual BOOL IsLeft() = 0;

  /**
  * Cylinder가 Right 인지 확인한다.
  * @return : TRUE = RIGHT , FALSE = RIGHT 아님
  */
  virtual BOOL IsRight() = 0;

  /**
  * Cylinder가 Front에 있는지 확인한다.
  * @return : TRUE = FRONT , FALSE = FRONT 아님
  */
  virtual BOOL IsForward() = 0;

  /**
  * Cylinder가 Front에 있는지 확인한다.
  * @return : TRUE = Front , FALSE = Front 아님
  */
  virtual BOOL IsBackward() = 0;

  /**
    * Cylinder가 Downstream에 있는지 확인한다.
    * @return : TRUE = Downstream , FALSE = Downstream 아님
    */
  virtual BOOL IsDownstr() = 0;

  /**
  * Cylinder가 Upstream에 있는지 확인한다.
  * @return : TRUE = Upstream , FALSE = Upstream 아님
  */
  virtual BOOL IsUpstr() = 0;

  /**
  * Cylinder가 CW에 있는지 확인한다.
  * @return : TRUE = CW , FALSE = CW 아님
  */
  virtual BOOL IsCW() = 0;

  /**
  * Cylinder가 CCW에 있는지 확인한다.
  * @return : TRUE = CCW , FALSE = CCW 아님
  */
  virtual BOOL IsCCW() = 0;

  /**
  * Cylinder가 Open 인지 확인한다.
  * @return : TRUE = Open , FALSE = Open 아님
  */
  virtual BOOL IsOpen() = 0;

  /**
  * Cylinder가 Close 인지 확인한다.
  * @return : TRUE = Close , FALSE = Close 아님
  */
  virtual BOOL IsClose() = 0;

  /**
   * Cylinder가 Middle에 있는지 확인한다.
   * @return : TRUE = Middle , FALSE = Middle에 아님
   */
  virtual BOOL IsMiddle() = 0;

  /**
  * Cylinder가 Expand 인지 확인한다.
  * @return : TRUE = Expand , FALSE = Expand 아님
  */
  virtual BOOL IsExpand() = 0;

  /**
  * Cylinder가 Shrink 인지 확인한다.
  * @return : TRUE = Shrink , FALSE = Shrink 아님
  */
  virtual BOOL IsShrink() = 0;


  /**
   * Cylinder가 Lock 인지 확인한다.
   * @return : TRUE = Lock , FALSE = Lock 아님
   */
  virtual BOOL IsLock() = 0;

  /**
  * Cylinder가 Unlock 인지 확인한다.
  * @return : TRUE = Unlock , FALSE = Unlock 아님
  */
  virtual BOOL IsUnlock() = 0;

  /**
   * Cylinder가 Turn 인지 확인한다.
   * @return : TRUE = Turn , FALSE = Turn 아님
   */
  virtual BOOL IsTurn() = 0;

  /**
  * Cylinder가 Return 인지 확인한다.
  * @return : TRUE = Return , FALSE = Return 아님
  */
  virtual BOOL IsReturn() = 0;

  /**
  * Cylinder가 출력이 나가고 있느지 확이한다.
  * @return : TRUE = Return , FALSE = Return 아님
  */
  virtual BOOL IsOutputOn() = 0;

  /**
  * Cylinder가 출력이 잘렸는지 확인한다.
  * @return : TRUE = Return , FALSE = Return 아님
  */
  virtual BOOL IsOutputOff() = 0;



  /****************************************************
   *	실린더 이동
   ****************************************************/

  /**
  * Cylinder를 Up으로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Up(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Down으로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Down(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Left로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Left(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Right로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Right(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Front로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Forward(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Back으로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Backward(int skip_sensor = FALSE) = 0;

  /**
    * Cylinder를 Downstr으로 이동시킨다.
    * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
    * @return 0 = Success, 그 외 = Error
    */
  virtual int Downstr(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Upstr으로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Upstr(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 CW로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int CW(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 CCW으로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int CCW(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Open으로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Open(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Close으로 이동시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Close(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 중간 위치에서 정지 시킨다.
* @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Middle(int skip_sensor = FALSE) = 0;//중간정지..3way sv만허용...

  /**
  * Cylinder를 Expand 시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Expand(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Shrink 시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Shrink(int skip_sensor = FALSE) = 0;

  /**
   * Cylinder를 Lock 시킨다.
   * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
   * @return 0 = Success, 그 외 = Error
   */
  virtual int Lock(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Unlock 시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Unlock(int skip_sensor = FALSE) = 0;


  /**
    * Cylinder를 Return 시킨다.
    * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
    * @return 0 = Success, 그 외 = Error
    */
  virtual int Return(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder를 Turn 시킨다.
  * @param skip_sensor: Unit의 동작을 명령한 후에 sensor로 완료 Check할 것인가를 결정하는 변수
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Turn(int skip_sensor = FALSE) = 0;



  /****************************************************
   *	실린더 이동 시작
   ****************************************************/

  /**
  * Cylinder를 Up으로 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartUp() = 0;

  /**
  * Cylinder를 Down으로 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartDown() = 0;

  /**
    * Cylinder를 Left로 이동을 시작한다.
    * @return 0 = Success, 그 외 = Error
    */
  virtual int StartLeft() = 0;

  /**
  * Cylinder를 Right로 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartRight() = 0;


  /**
  * Cylinder를 Forward 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartForward() = 0;

  /**
  * Cylinder를 Backward으로 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartBackward() = 0;

  /**
    * Cylinder를 Downstr으로 이동을 시작한다.
    * @return 0 = Success, 그 외 = Error
    */
  virtual int StartDownstr() = 0;

  /**
  * Cylinder를 Upstr으로 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartUpstr() = 0;

  /**
  * Cylinder를 CW로 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartCW() = 0;

  /**
  * Cylinder를 CCW으로 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartCCW() = 0;

  /**
  * Cylinder를 Open으로 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartOpen() = 0;

  /**
  * Cylinder를 Close으로 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartClose() = 0;

  /**
  * Cylinder를 중간 위치에서 정지 시킨다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartMiddle() = 0;//중간정지..3way sv만허용...

  /**
  * Cylinder Expand 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartExpand() = 0;

  /**
  * Cylinder Shrink 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartShrink() = 0;


  /**
     * Cylinder Lock 이동을 시작한다.
     * @return 0 = Success, 그 외 = Error
     */
  virtual int StartLock() = 0;

  /**
  * Cylinder Unlock 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartUnlock() = 0;

  /**
    * Cylinder Turn 이동을 시작한다.
    * @return 0 = Success, 그 외 = Error
    */
  virtual int StartTurn() = 0;

  /**
  * Cylinder Return 이동을 시작한다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int StartReturn() = 0;


  /****************************************************
   *	실린더 동작완료때까지 Sleep
   ****************************************************/

  /**
  * Cylinder가 Up 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4UpComplete() = 0;

  /**
  * Cylinder가 Down 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4DownComplete() = 0;

  /**
  * Cylinder가 Left 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4LeftComplete() = 0;

  /**
  * Cylinder가 Right 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4RightComplete() = 0;

  /**
  * Cylinder가 Front 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4ForwardComplete() = 0;

  /**
  * Cylinder가 Back 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4BackwardComplete() = 0;

  /**
  * Cylinder가 CW 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4CWComplete() = 0;

  /**
  * Cylinder가 CCW 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4CCWComplete() = 0;

  /**
  * Cylinder가 Open 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4OpenComplete() = 0;

  /**
  * Cylinder가 Close 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4CloseComplete() = 0;

  /**
  * Cylinder가 Downstr 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4DownstrComplete() = 0;

  /**
  * Cylinder가 Upstr 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4UpstrComplete() = 0;

  /**
  * Cylinder가 MiddlePoint 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4MiddleComplete(BOOL bDir) = 0;//중간정지..3way sv만허용...

  /**
  * Cylinder가 Expand 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4ExpandComplete() = 0;

  /**
  * Cylinder가 Shrink 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4ShrinkComplete() = 0;

  /**
    * Cylinder가 Lock 동작이 완료될때까지 기다린다.
    * @return 0 = Success, 그 외 = Error
    */
  virtual int Wait4LockComplete() = 0;

  /**
  * Cylinder가 Unlock 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4UnlockComplete() = 0;


  /**
    * Cylinder가 Turn 동작이 완료될때까지 기다린다.
    * @return 0 = Success, 그 외 = Error
    */
  virtual int Wait4TurnComplete() = 0;

  /**
  * Cylinder가 Return 동작이 완료될때까지 기다린다.
  * @return 0 = Success, 그 외 = Error
  */
  virtual int Wait4ReturnComplete() = 0;

  /**
   * 모든 Solenoid를 Off한다.
   */
  virtual void OffSolenoid() = 0;

  /**
  * Solenoid을 Off한다.
  *
  * @param   bDir : TRUE -> 1동작, FALSE -> 2동작
  */
  virtual void OffSolenoid(BOOL bDir) = 0;

  /**
   * 실린더 동작 완료를 확인한다.
   *
   * @param   bDir  : TRUE -> 1동작, FALSE -> 2동작
   * @return : TRUE = Complete, FALSE = NOT Complet
   */
  virtual BOOL IsMoveComplete(BOOL bDir) = 0;


  /****************************************************
   *	Common Interface
   ****************************************************/

  /**
  * Assign component linkage
  *
  * @param	pIO	: 참조할 IO Component Pointer
  */
  //virtual int AssignComponents(IIO* pIO) = 0;

  /**
 * Component Data 처리
 *
 * @param	datComponent : 기록할 데이타
 * @return	Error : 0 = Success, 그외=기타
 */
  virtual int SetData(CylinderData datComponent) = 0;

  /**
 * Component Data 읽어올 데이타 인터
 *
 * @param	datComponent : MCylinderData Class에 대한 포인터
 * @return	Error : 0 = Success, 그외=기타
 */
  virtual int GetData(CylinderData* pData) = 0;

  /**
   * Cylinder 관련 Data를 File에서 읽어온다.
   *
   * @param	pcylTimerData (읽어올 Data가 저장될 변수)
   * @return	int (0 = Success, Error Code = 그 외)
   */
  virtual int LoadParameter() = 0;

  /**
   * Cylinder 관련 Data를 File에 저장한다.
   *
   * @param	pcylTimerData (저장할 Data)
   * @return	int (0 = Success, Error Code = 그 외)
   */
  virtual int SaveParameter(CylinderData* pcylTimerData) = 0;

  /**
   * Cylinder 관련 Data를 얻어 온다.
   *
   * @param	pcylTimerData (읽어올 Data가 저장될 변수)
   * @return	int (0 = Success, Error Code = 그 외)
   */
  virtual void GetParameter(CylinderData* pcylTimerData) = 0;

  /**
   * Cylinder 이름을 Return 한다.
   * @return : Cylinder Name
   */
  virtual CString GetCylinderName() = 0;



};


