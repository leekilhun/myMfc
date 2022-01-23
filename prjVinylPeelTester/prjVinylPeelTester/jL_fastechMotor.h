#pragma once
#include "jL_fastechComm.h"
#include "IAxis.h"


class jL_fastechMotor:public IAxis
{
	 /****************************************************
	  *	data
	  ****************************************************/
public:
	struct motor_state
	{
		uint8_t	  id;
		uint8_t   sync_no;
		uint8_t   is_motor_on;
		uint16_t  alarm;
		uint16_t  encoder;
		uint32_t  axis_status;
		uint32_t  act_pos;
		uint32_t  cmd_pos;
		uint32_t  act_vel;
	};

public:
	jL_fastechComm* m_pComm;
	uint8_t m_motorId;
	motor_state m_motorState;
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
	void SendCommand(uint32_t cmd_index = 0, uint8_t id = 0);
	int MotorOnOff(bool on_off);
	motor_state* GetMotorState();

};

