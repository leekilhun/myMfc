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


/** Cylinder Timer�� ���� ����
 * @stereotype enumeration */
enum class CylinderTime
{
	MOVING,
	NOSEN_MOVING,
	SETTLING_1,
	SETTLING_2
};

/*
 Cylinder Ÿ���� �����Ѵ�.
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
  Solenoid Ÿ���� �����Ѵ�.
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
		/** ������ Cylinder id */
		int Cyl_id;
		/** ������ Cylinder ��ü�� ������ Solenoid �ܵ����϶��� �ϳ� ���, �������϶��� 2�� ���  */
		unsigned short solenoid[2];
		/** ������ Cylinder ��ü�� ������ ������ Solenoid  +,- ���� 1����   */
		unsigned short acc_solenoid[2];
		/** Up Sensor  : üũ�ϰ��� �ϴ� ���� ��ŭ ���� �ϰ� �������� NULL�� �Ѵ�. */
		unsigned short up_sensor[DEF_MAX_CYLINDER_SENSOR];
		/** Down Sensor : üũ�ϰ��� �ϴ� ���� ��ŭ ���� �ϰ� �������� NULL�� �Ѵ�. */
		unsigned short down_sensor[DEF_MAX_CYLINDER_SENSOR];
		/** Middle Sensor : ��ϵ� Sensor���� ���� üũ  */
		unsigned short middle_sensor[DEF_MAX_CYLINDER_SENSOR];
		/** ������ ���� : +����, - ���� 2�� �ۿ� ������ �� ����. */
		unsigned short acc_sensor[2];
		/** Cylinder Ÿ�� */
		CylinderType cyl_type;
		/** Solenoid Ÿ�� */
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
	 *	�Ķ���� ����
	 ****************************************************/

  /// <summary>
  /// Cylinder�� Moving Time��, Settling Time, NO Sensor Moving Time�� �о�帰��.
  /// </summary>
  /// <param name="moveing_sec">Cylinder �̵��� �ɸ��� �ִ� �ð� (�ʴ���)</param>
  /// <param name="settling_sec_1">Cylinder�� 1������  ����ȭ �Ǵµ� �ɸ��� �ð� (�ʴ���)</param>
  /// <param name="settling_sec_2">Cylinder�� 2������  ����ȭ �Ǵµ� �ɸ��� �ð� (�ʴ���)</param>
  /// <param name="no_sensor_moving_sec">Cylinder �̵��� Sensor�� �������� Moving Time (�ʴ���)</param>
  /// <returns> 0 = Success, �� �� = Error</returns>
  virtual int GetCylinderTime(double& moveing_sec, double& settling_sec_1, double& settling_sec_2, double& no_sensor_moving_sec) = 0;

 

  /// <summary>
  /// Cylinder�� Moving Time��, Settling Time, NO Sensor Moving Time �� �ϳ��� �о�帰��.
  /// </summary>
  /// <param name="cylinder_time">MOVING_TIME, NOSEN_WAITING_TIME, SETTLING_TIME</param>
  /// <param name="cylinder_sec">eCylinderTime���� ������ Time�� Value (�ʴ���)</param>
  /// <returns> 0 = Success, �� �� = Error</returns>
  virtual int GetCylinderTime(CylinderTime cylinder_time, double& cylinder_sec) = 0;


  /****************************************************
   *	�Ǹ��� ���� Ȯ��
   ****************************************************/

   /**
    * Cylinder�� Up �Ǿ� �ִ´��� Ȯ���Ѵ�.
    * @return : TRUE = UP , FALSE = UP �ƴ�
    */
  virtual BOOL IsUp() = 0;

  /**
   * Cylinder�� Down �Ǿ� �ִ��� Ȯ���Ѵ�.
   * @return : TRUE = DOWN , FALSE = DOWN �ƴ�
   */
  virtual BOOL IsDown() = 0;

  /**
  * Cylinder�� Left ���� Ȯ���Ѵ�.
  * @return : TRUE = LEFT , FALSE = LEFT �ƴ�
  */
  virtual BOOL IsLeft() = 0;

  /**
  * Cylinder�� Right ���� Ȯ���Ѵ�.
  * @return : TRUE = RIGHT , FALSE = RIGHT �ƴ�
  */
  virtual BOOL IsRight() = 0;

  /**
  * Cylinder�� Front�� �ִ��� Ȯ���Ѵ�.
  * @return : TRUE = FRONT , FALSE = FRONT �ƴ�
  */
  virtual BOOL IsForward() = 0;

  /**
  * Cylinder�� Front�� �ִ��� Ȯ���Ѵ�.
  * @return : TRUE = Front , FALSE = Front �ƴ�
  */
  virtual BOOL IsBackward() = 0;

  /**
    * Cylinder�� Downstream�� �ִ��� Ȯ���Ѵ�.
    * @return : TRUE = Downstream , FALSE = Downstream �ƴ�
    */
  virtual BOOL IsDownstr() = 0;

  /**
  * Cylinder�� Upstream�� �ִ��� Ȯ���Ѵ�.
  * @return : TRUE = Upstream , FALSE = Upstream �ƴ�
  */
  virtual BOOL IsUpstr() = 0;

  /**
  * Cylinder�� CW�� �ִ��� Ȯ���Ѵ�.
  * @return : TRUE = CW , FALSE = CW �ƴ�
  */
  virtual BOOL IsCW() = 0;

  /**
  * Cylinder�� CCW�� �ִ��� Ȯ���Ѵ�.
  * @return : TRUE = CCW , FALSE = CCW �ƴ�
  */
  virtual BOOL IsCCW() = 0;

  /**
  * Cylinder�� Open ���� Ȯ���Ѵ�.
  * @return : TRUE = Open , FALSE = Open �ƴ�
  */
  virtual BOOL IsOpen() = 0;

  /**
  * Cylinder�� Close ���� Ȯ���Ѵ�.
  * @return : TRUE = Close , FALSE = Close �ƴ�
  */
  virtual BOOL IsClose() = 0;

  /**
   * Cylinder�� Middle�� �ִ��� Ȯ���Ѵ�.
   * @return : TRUE = Middle , FALSE = Middle�� �ƴ�
   */
  virtual BOOL IsMiddle() = 0;

  /**
  * Cylinder�� Expand ���� Ȯ���Ѵ�.
  * @return : TRUE = Expand , FALSE = Expand �ƴ�
  */
  virtual BOOL IsExpand() = 0;

  /**
  * Cylinder�� Shrink ���� Ȯ���Ѵ�.
  * @return : TRUE = Shrink , FALSE = Shrink �ƴ�
  */
  virtual BOOL IsShrink() = 0;


  /**
   * Cylinder�� Lock ���� Ȯ���Ѵ�.
   * @return : TRUE = Lock , FALSE = Lock �ƴ�
   */
  virtual BOOL IsLock() = 0;

  /**
  * Cylinder�� Unlock ���� Ȯ���Ѵ�.
  * @return : TRUE = Unlock , FALSE = Unlock �ƴ�
  */
  virtual BOOL IsUnlock() = 0;

  /**
   * Cylinder�� Turn ���� Ȯ���Ѵ�.
   * @return : TRUE = Turn , FALSE = Turn �ƴ�
   */
  virtual BOOL IsTurn() = 0;

  /**
  * Cylinder�� Return ���� Ȯ���Ѵ�.
  * @return : TRUE = Return , FALSE = Return �ƴ�
  */
  virtual BOOL IsReturn() = 0;

  /**
  * Cylinder�� ����� ������ �ִ��� Ȯ���Ѵ�.
  * @return : TRUE = Return , FALSE = Return �ƴ�
  */
  virtual BOOL IsOutputOn() = 0;

  /**
  * Cylinder�� ����� �߷ȴ��� Ȯ���Ѵ�.
  * @return : TRUE = Return , FALSE = Return �ƴ�
  */
  virtual BOOL IsOutputOff() = 0;



  /****************************************************
   *	�Ǹ��� �̵�
   ****************************************************/

  /**
  * Cylinder�� Up���� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Up(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Down���� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Down(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Left�� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Left(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Right�� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Right(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Front�� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Forward(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Back���� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Backward(int skip_sensor = FALSE) = 0;

  /**
    * Cylinder�� Downstr���� �̵���Ų��.
    * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
    * @return 0 = Success, �� �� = Error
    */
  virtual int Downstr(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Upstr���� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Upstr(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� CW�� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int CW(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� CCW���� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int CCW(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Open���� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Open(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Close���� �̵���Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Close(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� �߰� ��ġ���� ���� ��Ų��.
* @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Middle(int skip_sensor = FALSE) = 0;//�߰�����..3way sv�����...

  /**
  * Cylinder�� Expand ��Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Expand(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Shrink ��Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Shrink(int skip_sensor = FALSE) = 0;

  /**
   * Cylinder�� Lock ��Ų��.
   * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
   * @return 0 = Success, �� �� = Error
   */
  virtual int Lock(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Unlock ��Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Unlock(int skip_sensor = FALSE) = 0;


  /**
    * Cylinder�� Return ��Ų��.
    * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
    * @return 0 = Success, �� �� = Error
    */
  virtual int Return(int skip_sensor = FALSE) = 0;

  /**
  * Cylinder�� Turn ��Ų��.
  * @param skip_sensor: Unit�� ������ ����� �Ŀ� sensor�� �Ϸ� Check�� ���ΰ��� �����ϴ� ����
  * @return 0 = Success, �� �� = Error
  */
  virtual int Turn(int skip_sensor = FALSE) = 0;



  /****************************************************
   *	�Ǹ��� �̵� ����
   ****************************************************/

  /**
  * Cylinder�� Up���� �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartUp() = 0;

  /**
  * Cylinder�� Down���� �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartDown() = 0;

  /**
    * Cylinder�� Left�� �̵��� �����Ѵ�.
    * @return 0 = Success, �� �� = Error
    */
  virtual int StartLeft() = 0;

  /**
  * Cylinder�� Right�� �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartRight() = 0;


  /**
  * Cylinder�� Forward �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartForward() = 0;

  /**
  * Cylinder�� Backward���� �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartBackward() = 0;

  /**
    * Cylinder�� Downstr���� �̵��� �����Ѵ�.
    * @return 0 = Success, �� �� = Error
    */
  virtual int StartDownstr() = 0;

  /**
  * Cylinder�� Upstr���� �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartUpstr() = 0;

  /**
  * Cylinder�� CW�� �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartCW() = 0;

  /**
  * Cylinder�� CCW���� �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartCCW() = 0;

  /**
  * Cylinder�� Open���� �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartOpen() = 0;

  /**
  * Cylinder�� Close���� �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartClose() = 0;

  /**
  * Cylinder�� �߰� ��ġ���� ���� ��Ų��.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartMiddle() = 0;//�߰�����..3way sv�����...

  /**
  * Cylinder Expand �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartExpand() = 0;

  /**
  * Cylinder Shrink �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartShrink() = 0;


  /**
     * Cylinder Lock �̵��� �����Ѵ�.
     * @return 0 = Success, �� �� = Error
     */
  virtual int StartLock() = 0;

  /**
  * Cylinder Unlock �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartUnlock() = 0;

  /**
    * Cylinder Turn �̵��� �����Ѵ�.
    * @return 0 = Success, �� �� = Error
    */
  virtual int StartTurn() = 0;

  /**
  * Cylinder Return �̵��� �����Ѵ�.
  * @return 0 = Success, �� �� = Error
  */
  virtual int StartReturn() = 0;


  /****************************************************
   *	�Ǹ��� ���ۿϷᶧ���� Sleep
   ****************************************************/

  /**
  * Cylinder�� Up ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4UpComplete() = 0;

  /**
  * Cylinder�� Down ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4DownComplete() = 0;

  /**
  * Cylinder�� Left ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4LeftComplete() = 0;

  /**
  * Cylinder�� Right ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4RightComplete() = 0;

  /**
  * Cylinder�� Front ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4ForwardComplete() = 0;

  /**
  * Cylinder�� Back ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4BackwardComplete() = 0;

  /**
  * Cylinder�� CW ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4CWComplete() = 0;

  /**
  * Cylinder�� CCW ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4CCWComplete() = 0;

  /**
  * Cylinder�� Open ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4OpenComplete() = 0;

  /**
  * Cylinder�� Close ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4CloseComplete() = 0;

  /**
  * Cylinder�� Downstr ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4DownstrComplete() = 0;

  /**
  * Cylinder�� Upstr ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4UpstrComplete() = 0;

  /**
  * Cylinder�� MiddlePoint ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4MiddleComplete(BOOL bDir) = 0;//�߰�����..3way sv�����...

  /**
  * Cylinder�� Expand ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4ExpandComplete() = 0;

  /**
  * Cylinder�� Shrink ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4ShrinkComplete() = 0;

  /**
    * Cylinder�� Lock ������ �Ϸ�ɶ����� ��ٸ���.
    * @return 0 = Success, �� �� = Error
    */
  virtual int Wait4LockComplete() = 0;

  /**
  * Cylinder�� Unlock ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4UnlockComplete() = 0;


  /**
    * Cylinder�� Turn ������ �Ϸ�ɶ����� ��ٸ���.
    * @return 0 = Success, �� �� = Error
    */
  virtual int Wait4TurnComplete() = 0;

  /**
  * Cylinder�� Return ������ �Ϸ�ɶ����� ��ٸ���.
  * @return 0 = Success, �� �� = Error
  */
  virtual int Wait4ReturnComplete() = 0;

  /**
   * ��� Solenoid�� Off�Ѵ�.
   */
  virtual void OffSolenoid() = 0;

  /**
  * Solenoid�� Off�Ѵ�.
  *
  * @param   bDir : TRUE -> 1����, FALSE -> 2����
  */
  virtual void OffSolenoid(BOOL bDir) = 0;

  /**
   * �Ǹ��� ���� �ϷḦ Ȯ���Ѵ�.
   *
   * @param   bDir  : TRUE -> 1����, FALSE -> 2����
   * @return : TRUE = Complete, FALSE = NOT Complet
   */
  virtual BOOL IsMoveComplete(BOOL bDir) = 0;


  /****************************************************
   *	Common Interface
   ****************************************************/

  /**
  * Assign component linkage
  *
  * @param	pIO	: ������ IO Component Pointer
  */
  //virtual int AssignComponents(IIO* pIO) = 0;

  /**
 * Component Data ó��
 *
 * @param	datComponent : ����� ����Ÿ
 * @return	Error : 0 = Success, �׿�=��Ÿ
 */
  virtual int SetData(CylinderData datComponent) = 0;

  /**
 * Component Data �о�� ����Ÿ ����
 *
 * @param	datComponent : MCylinderData Class�� ���� ������
 * @return	Error : 0 = Success, �׿�=��Ÿ
 */
  virtual int GetData(CylinderData* pData) = 0;

  /**
   * Cylinder ���� Data�� File���� �о�´�.
   *
   * @param	pcylTimerData (�о�� Data�� ����� ����)
   * @return	int (0 = Success, Error Code = �� ��)
   */
  virtual int LoadParameter() = 0;

  /**
   * Cylinder ���� Data�� File�� �����Ѵ�.
   *
   * @param	pcylTimerData (������ Data)
   * @return	int (0 = Success, Error Code = �� ��)
   */
  virtual int SaveParameter(CylinderData* pcylTimerData) = 0;

  /**
   * Cylinder ���� Data�� ��� �´�.
   *
   * @param	pcylTimerData (�о�� Data�� ����� ����)
   * @return	int (0 = Success, Error Code = �� ��)
   */
  virtual void GetParameter(CylinderData* pcylTimerData) = 0;

  /**
   * Cylinder �̸��� Return �Ѵ�.
   * @return : Cylinder Name
   */
  virtual CString GetCylinderName() = 0;



};


