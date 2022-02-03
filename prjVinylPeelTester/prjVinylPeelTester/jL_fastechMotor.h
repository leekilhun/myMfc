#pragma once
#include "jL_fastechComm.h"
#include "IAxis.h"
#include "jL_Registor.h"
#include "jL_scheduler.h"



/* fastech motor axis status*/

#define REG_ERR_ALL               0
#define REG_ERR_HW_PLUS_LIMIT     1
#define REG_ERR_HW_MINUS_LIMIT    2
#define REG_ERR_SW_PLUS_LIMIT     3
#define REG_ERR_SW_MINUS_LIMIT    4
#define REG_ERR_RESERVED          5
#define REG_ERR_POS_CNT_OVER      6
#define REG_ERR_SERVO_ALARM       7

#define REG_ERR_OVER_CURRENT      0
#define REG_ERR_OVER_SPEED        1
#define REG_ERR_STEP_OUT          2
#define REG_ERR_OVER_LOAD         3
#define REG_ERR_OVER_HEAT         4
#define REG_ERR_BACK_EMF          5
#define REG_ERR_MOTOR_POWER       6
#define REG_ERR_INPOSITION        7

#define REG_EMG_STOP              0
#define REG_SLOW_STOP             1
#define REG_ORG_RETURNING         2
#define REG_INPOSIOTION           3
#define REG_SERVO_ON              4
#define REG_ALARM_RESET           5
#define REG_PT_STOPPED            6
#define REG_ORIGIN_SENSOR         7

#define REG_Z_PLUSE               0
#define REG_ORG_RETURN_OK         1
#define REG_MOTION_DIR            2
#define REG_MOTIONING             3
#define REG_MOTION_PAUSE          4
#define REG_MOTION_ACCEL          5
#define REG_MOTION_DECEL          6
#define REG_MOTION_CONSTANT       7

enum class err_type {
  error_all,
  hw_plus_limit,
  hw_minus_limit,
  sw_plus_limit,
  sw_minus_limit,
  reserved_05,
  pos_cnt_over,
  servo_alarm,

  over_current,
  over_speed,
  step_out,
  over_load,
  over_heat,
  back_EMF,
  motor_power,
  inposition,
  _max
};
#define ERR_CODE(x)          static_cast<uint8_t>(err_type::x)



#define FM_ERR_01    0
#define FM_ERR_02    1
#define FM_STATE_01  2
#define FM_STATE_02  3

/*
번호  이름                 단위              하한              상한            출하치
0 Pulse per Revolution                        0                 9                 9
1 Axis Max Speed          [pps]               1           2500000            500000
2 Axis Start Speed        [pps]               1           2500000                 1
3 Axis Acc Time           [msec]              1              9999               100
4 Axis Dec Time           [msec]              1              9999               100
5 Speed Override          [%]                 1               500               100
6 Jog Speed               [pps]               1           2500000              5000
7 Jog Start Speed         [pps]               1           2500000                 1
8 Jog Acc Dec Time        [msec]              1              9999               100
9 Alarm Logic                                 0                 1                 0
10 Servo On Logic                             0                 1                 0
11 Alarm Reset Logic                          0                 1                 0
12 SW Limit Plus Value    [pulse]    -134217727         134217727        +134217727
13 SW Limit Minus Value   [pulse]    -134217727         134217727        -134217727
14 SW Limit Stop Method                       0                 1                 1
15 HW Limit Stop Method                       0                 1                 1
16 Limit Sensor Logic                         0                 1                 0
17 Org Speed              [pps]               1           2500000              5000
18 Org Search Speed       [pps]               1           2500000              1000
19 Org Acc Dec Time       [msec]              1              9999                50
20 Org Method                                 0                 4                 0
21 Org Dir                                    0                 1                 0
22 Org Offset             [pulse]    -134217727        +134217727                 0
23 Org Position Set       [pulse]    -134217727        +134217727                 0
24 Org Sensor Logic                           0                 1                 0
25 Position loop gain                         0                15                 0
26 Inpos value                                0                15                 0
27 Pos tracking limit                         0         134217727                 0
28 Motion Dir                                 0                 1                 0
29 Limit Sensor Dir                           0                 1                 0
30 Org torque ratio       [%]                20                90                50
31 Motion Lead                                1            100000                 1  
32 Gear Ratio                                 1            100000                 1
33 Pos.err overflow limit [pulse]             1         134217727              5000
34 Brake delay time       [msec]              1              1000               200
35 Run current            [*10%]              5                15                10     
36 Stop current           [*10%]              2                10                10
37 Alarm for time         [sec]               1                10                 5
*/

class fm_parameter
{
public:
  enum class param
  {
    pulse_per_revolution = 0,
    axis_max_speed,
    axis_start_speed,
    axis_acc_time,
    axis_dec_time,
    speed_override,
    jog_speed,
    jog_start_speed,
    jog_acc_dec_time,
    alarm_logic,

    servo_on_logic,
    alarm_reset_logic,
    sw_limit_plus,
    sw_limit_minus,
    sw_limit_stop_method,
    hw_limit_stop_method,
    limit_sensor_logic,
    org_speed,
    org_search_speed,
    org_acc_dec_time,

    org_method,
    org_dir,
    org_offset,
    org_position_set,
    org_sensor_logic,
    pos_loop_gain,
    inpos_value,
    pos_tracking_limit,
    motion_dir,
    limit_sensor_dir,

    org_torqueratio,
    motion_lead,
    gear_ratio,
    pos_err_overflow_limit,
    brake_delay_time,
    run_current,
    stop_current,
    alarm_for_time,
    _max
  };

private:
  uint8_t _pulse_per_revolution;
  uint16_t _axis_max_speed;
  uint8_t _axis_start_speed;
  uint16_t _axis_acc_time;
  uint16_t _axis_dec_time;
  uint16_t _speed_override;
  uint16_t _jog_speed;
  uint16_t _jog_start_speed;
  uint16_t _jog_acc_dec_time;
  uint8_t _alarm_logic;

  uint8_t _servo_on_logic;
  uint8_t _alarm_reset_logic;
  int _sw_limit_plus;
  int _sw_limit_minus;
  uint8_t _sw_limit_stop_method;
  uint8_t _hw_limit_stop_method;
  uint8_t _limit_sensor_logic;
  uint16_t _org_speed;
  uint16_t _org_search_speed;
  uint16_t _org_acc_dec_time;

  uint16_t _org_method;
  uint8_t _org_dir;
  int16_t _org_offset;
  int16_t _org_position_set;
  uint8_t _org_sensor_logic;
  uint8_t _pos_loop_gain;
  uint8_t _inpos_value;
  uint32_t _pos_tracking_limit;
  uint8_t _motion_dir;
  uint8_t _limit_sensor_dir;

  uint8_t _org_torqueratio;
  uint32_t _motion_lead;
  uint32_t _gear_ratio;
  uint32_t _pos_err_overflow_limit;
  uint32_t _brake_delay_time;
  uint8_t  _run_current;
  uint8_t  _stop_current;
  uint8_t  _alarm_for_time;
public:
  fm_parameter() {
    init();
  }

  ~fm_parameter() {
    init();
  }

private:
  inline bool in_bound(int value, int min, int max) {
    return value < min ? 0 : (value > max ? 0 : 1);
  }

  inline bool out_bound(int value, int min, int max) {
    return !(value < min ? 0 : (value > max ? 0 : 1));
  }

  inline void init() {
    _pulse_per_revolution = 0;
    _axis_max_speed = 0;
    _axis_start_speed = 0;
    _axis_acc_time = 0;
    _axis_dec_time = 0;
    _speed_override = 0;
    _jog_speed = 0;
    _jog_start_speed = 0;
    _jog_acc_dec_time = 0;
    _alarm_logic = 0;

    _servo_on_logic = 0;
    _alarm_logic = 0;
    _sw_limit_plus = 0;
    _sw_limit_minus = 0;
    _sw_limit_stop_method = 0;
    _hw_limit_stop_method = 0;
    _limit_sensor_logic = 0;
    _org_speed = 0;
    _org_search_speed = 0;
    _org_acc_dec_time = 0;

    _org_method = 0;
    _org_dir = 0;
    _org_offset = 0;
    _org_position_set = 0;
    _org_sensor_logic = 0;
    _pos_loop_gain = 0;
    _inpos_value = 0;
    _pos_tracking_limit = 0;
    _motion_dir = 0;
    _limit_sensor_dir = 0;

    _org_torqueratio = 0;
    _motion_lead = 0;
    _gear_ratio = 0;
    _pos_err_overflow_limit = 0;
    _brake_delay_time = 0;
    _run_current = 0;
    _stop_current = 0;
    _alarm_for_time = 0;
  }
public:

  inline int Get(uint16_t param) {
    int ret = 0;
    if (param > (static_cast<int>(fm_parameter::param::_max) - 1))
      return 0;

    fm_parameter::param  a = static_cast<fm_parameter::param>(param);
    switch (a)
    {
    case fm_parameter::param::pulse_per_revolution:
      ret = _pulse_per_revolution;
      break;
    case fm_parameter::param::axis_max_speed:
      ret = _axis_max_speed;
      break;
    case fm_parameter::param::axis_start_speed:
      ret = _axis_start_speed;
      break;
    case fm_parameter::param::axis_acc_time:
      ret = _axis_acc_time;
      break;
    case fm_parameter::param::axis_dec_time:
      ret = _axis_dec_time;
      break;
    case fm_parameter::param::speed_override:
      ret = _speed_override;
      break;
    case fm_parameter::param::jog_speed:
      ret = _jog_speed;
      break;
    case fm_parameter::param::jog_start_speed:
      ret = _jog_start_speed;
      break;
    case fm_parameter::param::jog_acc_dec_time:
      ret = _jog_acc_dec_time;
      break;
    case fm_parameter::param::alarm_logic:
      ret = _alarm_logic;
      break;
    case fm_parameter::param::servo_on_logic:
      ret = _servo_on_logic;
      break;
    case fm_parameter::param::alarm_reset_logic:
      ret = _alarm_reset_logic;
      break;
    case fm_parameter::param::sw_limit_plus:
      ret = _sw_limit_plus;
      break;
    case fm_parameter::param::sw_limit_minus:
      ret = _sw_limit_minus;
      break;
    case fm_parameter::param::sw_limit_stop_method:
      ret = _sw_limit_stop_method;
      break;
    case fm_parameter::param::hw_limit_stop_method:
      ret = _hw_limit_stop_method;
      break;
    case fm_parameter::param::limit_sensor_logic:
      ret = _limit_sensor_logic;
      break;
    case fm_parameter::param::org_speed:
      ret = _org_speed;
      break;
    case fm_parameter::param::org_search_speed:
      ret = _org_search_speed;
      break;
    case fm_parameter::param::org_acc_dec_time:
      ret = _org_acc_dec_time;
      break;
    case fm_parameter::param::org_method:
      ret = _org_method;
      break;
    case fm_parameter::param::org_dir:
      ret = _org_dir;
      break;
    case fm_parameter::param::org_offset:
      ret = _org_offset;
      break;
    case fm_parameter::param::org_position_set:
      ret = _org_position_set;
      break;
    case fm_parameter::param::org_sensor_logic:
      ret = _org_sensor_logic;
      break;
    case fm_parameter::param::pos_loop_gain:
      ret = _pos_loop_gain;
      break;
    case fm_parameter::param::inpos_value:
      ret = _inpos_value;
      break;
    case fm_parameter::param::pos_tracking_limit:
      ret = _pos_tracking_limit;
      break;
    case fm_parameter::param::motion_dir:
      ret = _motion_dir;
      break;
    case fm_parameter::param::limit_sensor_dir:
      ret = _limit_sensor_dir;
      break;
    case fm_parameter::param::org_torqueratio:
      ret = _org_torqueratio;
      break;
    case fm_parameter::param::motion_lead:
      ret = _motion_lead;
      break;
    case fm_parameter::param::gear_ratio:
      ret = _gear_ratio;
      break;
    case fm_parameter::param::pos_err_overflow_limit:
      ret = _pos_err_overflow_limit;
      break;
    case fm_parameter::param::brake_delay_time:
      ret = _brake_delay_time;
      break;
    case fm_parameter::param::run_current:
      ret = _run_current;
      break;
    case fm_parameter::param::stop_current:
      ret = _stop_current;
      break;
    case fm_parameter::param::alarm_for_time:
      ret = _alarm_for_time;
      break;
    default:
      break;
    }

    return ret;

  }

  inline int Set(uint16_t param, int value) {
    int ret = 0;
    if (param > (static_cast<int>(fm_parameter::param::_max) - 1))
      return 0;

    fm_parameter::param  a = static_cast<fm_parameter::param>(param);
    switch (a)
    {
    case fm_parameter::param::pulse_per_revolution:
      if (out_bound(value, 0, 9))
        goto return_out;
      _pulse_per_revolution = value;
      ret = 1;
      break;
    case fm_parameter::param::axis_max_speed:
      if (out_bound(value, 1, 2500000))
        goto return_out;
      _axis_max_speed = value;
      ret = 1;
      break;
    case fm_parameter::param::axis_start_speed:
      if (out_bound(value, 1, 250000))
        goto return_out;
      _axis_start_speed = value;
      ret = 1;
      break;
    case fm_parameter::param::axis_acc_time:
      if (out_bound(value, 1, 9999))
        goto return_out;
      _axis_acc_time = value;
      ret = 1;
      break;
    case fm_parameter::param::axis_dec_time:
      if (out_bound(value, 1, 9999))
        goto return_out;
      _axis_dec_time = value;
      ret = 1;
      break;
    case fm_parameter::param::speed_override:
      if (out_bound(value, 1, 500))
        goto return_out;
      _speed_override = value;
      ret = 1;
      break;
    case fm_parameter::param::jog_speed:
      if (out_bound(value, 1, 2500000))
        goto return_out;
      _jog_speed = value;
      ret = 1;
      break;
    case fm_parameter::param::jog_start_speed:
      if (out_bound(value, 1, 2500000))
        goto return_out;
      _jog_start_speed = value;
      ret = 1;
      break;
    case fm_parameter::param::jog_acc_dec_time:
      if (out_bound(value, 1, 9999))
        goto return_out;
      _jog_acc_dec_time = value;
      ret = 1;
      break;
    case fm_parameter::param::alarm_logic:
      if (out_bound(value, 0, 1))
        goto return_out;
      _alarm_logic = value;
      ret = 1;
      break;
    case fm_parameter::param::servo_on_logic:
      if (out_bound(value, 0, 1))
        goto return_out;
      _servo_on_logic = value;
      ret = 1;
      break;
    case fm_parameter::param::alarm_reset_logic:
      if (out_bound(value, 0, 1))
        goto return_out;
      _alarm_reset_logic = value;
      ret = 1;
      break;
    case fm_parameter::param::sw_limit_plus:
      if (out_bound(value, -134217727, 134217727))
        goto return_out;
      _sw_limit_plus = value;
      ret = 1;
      break;
    case fm_parameter::param::sw_limit_minus:
      if (out_bound(value, -134217727, 134217727))
        goto return_out;
      _sw_limit_minus = value;
      ret = 1;
      break;
    case fm_parameter::param::sw_limit_stop_method:
      if (out_bound(value, 0, 1))
        goto return_out;
      _sw_limit_stop_method = value;
      ret = 1;
      break;
    case fm_parameter::param::hw_limit_stop_method:
      if (out_bound(value, 0, 1))
        goto return_out;
      _hw_limit_stop_method = value;
      ret = 1;
      break;
    case fm_parameter::param::limit_sensor_logic:
      if (out_bound(value, 0, 1))
        goto return_out;
      _limit_sensor_logic = value;
      ret = 1;
      break;
    case fm_parameter::param::org_speed:
      if (out_bound(value, 1, 2500000))
        goto return_out;
      _org_speed = value;
      ret = 1;
      break;
    case fm_parameter::param::org_search_speed:
      if (out_bound(value, 1, 2500000))
        goto return_out;
      _org_search_speed = value;
      ret = 1;
      break;
    case fm_parameter::param::org_acc_dec_time:
      if (out_bound(value, 1, 9999))
        goto return_out;
      _org_acc_dec_time = value;
      ret = 1;
      break;
    case fm_parameter::param::org_method:
      if (out_bound(value, 0, 4))
        goto return_out;
      _org_method = value;
      ret = 1;
      break;
    case fm_parameter::param::org_dir:
      if (out_bound(value, 0, 1))
        goto return_out;
      _org_dir = value;
      ret = 1;
      break;
    case fm_parameter::param::org_offset:
      if (out_bound(value, -134217727, 134217727))
        goto return_out;
      _org_offset = value;
      ret = 1;
      break;
    case fm_parameter::param::org_position_set:
      if (out_bound(value, -134217727, 134217727))
        goto return_out;
      _org_position_set = value;
      ret = _org_position_set;
      break;
    case fm_parameter::param::org_sensor_logic:
      if (out_bound(value, 0, 1))
        goto return_out;
      _org_sensor_logic = value;
      ret = 1;
      break;
    case fm_parameter::param::pos_loop_gain:
      if (out_bound(value, 0, 1))
        goto return_out;
      _pos_loop_gain = value;
      ret = 1;
      break;
    case fm_parameter::param::inpos_value:
      if (out_bound(value, 0, 1))
        goto return_out;
      _inpos_value = value;
      ret = 1;
      break;
    case fm_parameter::param::pos_tracking_limit:
      if (out_bound(value, 1, 134217727))
        goto return_out;
      _pos_tracking_limit = value;
      ret = 1;
      break;
    case fm_parameter::param::motion_dir:
      if (out_bound(value, 0, 1))
        goto return_out;
      _motion_dir = value;
      ret = 1;
      break;
    case fm_parameter::param::limit_sensor_dir:
      if (out_bound(value, 0, 1))
        goto return_out;
      _limit_sensor_dir = value;
      ret = 1;
      break;
    case fm_parameter::param::org_torqueratio:
      if (out_bound(value, 20, 90))
        goto return_out;
      _org_torqueratio = value;
      ret = 1;
      break;
    case fm_parameter::param::motion_lead:
      if (out_bound(value, 1, 100000))
        goto return_out;
      _motion_lead = value;
      ret = 1;
      break;
    case fm_parameter::param::gear_ratio:
      if (out_bound(value, 1, 100000))
        goto return_out;
      _gear_ratio = value;
      ret = 1;
      break;
    case fm_parameter::param::pos_err_overflow_limit:
      if (out_bound(value, 1, 134217727))
        goto return_out;
      _pos_err_overflow_limit = value;
      ret = 1;
      break;
    case fm_parameter::param::brake_delay_time:
      if (out_bound(value, 1, 1000))
        goto return_out;
      _brake_delay_time = value;
      ret = 1;
      break;
    case fm_parameter::param::run_current:
      if (out_bound(value, 5, 15))
        goto return_out;
      _run_current = value;
      ret = 1;
      break;
    case fm_parameter::param::stop_current:
      if (out_bound(value, 2, 10))
        goto return_out;
      _stop_current = value;
      ret = 1;
      break;
    case fm_parameter::param::alarm_for_time:
      if (out_bound(value, 1, 10))
        goto return_out;
      _alarm_for_time = value;
      ret = 1;
      break;
    }

  return_out:
    return ret;
  }


};


class jL_fastechMotor :public IAxis
{
  /****************************************************
   *	data
   ****************************************************/
public:
  struct motor_state
  {
    uint8_t	  id{};
    uint8_t   sync_no{};
    uint8_t   is_motor_on{};
    uint16_t  alarm{};
    uint16_t  encoder{};
    _Regist   axis_status;
    int       act_pos{};
    int       cmd_pos{};
    uint32_t  act_vel{};
    int       pos_err{};
  };

public:
  jL_fastechComm* m_pComm;
  uint8_t m_motorId;
  motor_state m_motorState;
  fm_parameter m_motorParam;
  _Scheduler* m_jobScheduler;
  timer::_ms m_timer;
  uint32_t m_responseTime;
  bool m_useISR;
  /****************************************************
   *	Constructor
   ****************************************************/
public:
  jL_fastechMotor();
  virtual ~jL_fastechMotor();

  /****************************************************
   *	func
   ****************************************************/
public:
  int Initialize();
  int PortOpen(CString portname, DWORD baudrate = CBR_9600, WORD port_no = NULL, HWND hwnd = NULL);
  void PortClose();
  void SetMotorId(uint8_t id = 0);
  uint32_t SendCommand(/*jL_fastechComm::fastech_cmdList*/uint32_t cmd_index, uint8_t* data = nullptr, uint8_t length = 0);
  int MotorOnOff(bool on_off);
  uint32_t GetAxisStatus(uint8_t id = 0);
  int GetAxisParameter(int param_no);
  int SetAxisParameter(int param_no, int value);
  void LoadingParamterData();

  void SetUseISR(bool on_off);
  bool ISR_OnOff(bool on_off);
  static void Callback_GetAxisParam(void* obj);
  static void Callback_GetAxisStatus(void* obj); 

  motor_state* GetMotorState();
  int UpdateAxisData(uint8_t* p_data, uint16_t length, uint8_t index);

  inline static int WarpFunc(void* obj, uint8_t* p_data, uint16_t len, uint8_t index) {
    jL_fastechMotor* self = (jL_fastechMotor*)(obj);
    return self->UpdateAxisData(p_data, len, index);
  }

};

