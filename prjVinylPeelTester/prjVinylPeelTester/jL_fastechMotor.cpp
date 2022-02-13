#include "pch.h"
#include "jL_fastechMotor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define CALLBACK_GET_STATUS   _Scheduler::job_period::_100ms

jL_fastechMotor::jL_fastechMotor():
	m_pComm(nullptr), m_motorId(0), m_motorState(), m_jobScheduler(nullptr), m_timer(), m_responseTime(0)
	, m_useISR(true)
{
	Initialize();	
}

jL_fastechMotor::~jL_fastechMotor()
{
	this->ISR_OnOff(false);
	PortClose();

	if (m_pComm != nullptr)
		delete m_pComm;
	m_pComm = nullptr;

	if (m_jobScheduler != nullptr)
		delete m_jobScheduler;
	m_jobScheduler = nullptr;

}

int jL_fastechMotor::Initialize()
{
	m_pComm = new jL_fastechComm;

	m_jobScheduler = new _Scheduler(this);
	//m_jobScheduler->AttachCallback(_Scheduler::job_period::_100ms, Callback_GetAxisParam);
	//m_jobScheduler->Start(_Scheduler::job_period::_100ms);

	//_Scheduler::scheduler_tbl_t job_t = m_jobScheduler[_Scheduler::job_period::_10ms];
	m_jobScheduler[0][CALLBACK_GET_STATUS].func_callback = Callback_GetAxisStatus;
	m_jobScheduler[0][CALLBACK_GET_STATUS].enable = false;

	m_jobScheduler->ThreadRun();
	return 0;
}

int jL_fastechMotor::PortOpen(CString portname, DWORD baudrate, WORD port_no, HWND hwnd)
{
	int retSuccess = 0;
	if (m_pComm->OpenPort(portname, baudrate, port_no, hwnd) == retSuccess)
	{
		m_pComm->ReadyPort();
		m_pComm->ThreadRun();

		m_timer.Start();
		m_pComm->m_checkSync.is_checked = true;
		// Callbac function attach
		m_pComm->SetCallback(this, WarpFunc);


		ISR_OnOff(false);
		//Loading motor Parameter data	
		LoadingParamterData();
		//
		ISR_OnOff(true);
	}
	return retSuccess;
}

void jL_fastechMotor::PortClose()
{
	m_pComm->Terminate();
}

void jL_fastechMotor::SetMotorId(uint8_t id)
{
	m_motorState.id = id;
}

uint32_t jL_fastechMotor::SendCommand(uint32_t cmd_index, uint8_t* data, uint8_t length)
{
	if (m_pComm->m_IsConnected == false)
		return 0;

	mutex mtwait;
	ISR_OnOff(false);
	mtwait.lock();
	m_timer.Start();
	//m_pComm->m_checkSync.keep_id = cmd_index;
	m_pComm->m_checkSync.is_checked = false;
	uint32_t ret = m_pComm->SendCmd(static_cast<jL_fastechComm::fastech_cmdList>(cmd_index), data, length);
	//timer::_ms chTM;
	//chTM.Start();
	while (!m_pComm->m_checkSync.is_checked) // not
	{
		util::delay(10);
		uint64_t passtime = m_timer.Elaps();
		if (m_timer.MoreThan(100)/*passtime >100*/)
		{
			TRACE("sendCommand check -return flag- time over %lld\t\n", passtime);
			break;			
		}
	}
	mtwait.unlock();
	ISR_OnOff(true);
	//uint64_t resulttime = chTM.Elaps();
	return ret;
}

int jL_fastechMotor::MotorOnOff(bool on_off)
{
	uint8_t data = (uint8_t)on_off;

	m_pComm->SetMotorId(m_motorState.id);
	return SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::SERVO_ENABLE), &data, 1);
}

uint32_t jL_fastechMotor::GetAxisStatus(uint8_t id)
{	
	return SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::GET_AXIS_STATUS));
}

int jL_fastechMotor::GetAxisParameter(int param_no)
{
	uint8_t data = (uint8_t)param_no;
	return SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::GET_RAM_PARAM), &data, 1);
}

int jL_fastechMotor::SetAxisParameter(int param_no, int value)
{
	return 0;
}

void jL_fastechMotor::LoadingParamterData()
{
	for (uint8_t i = 0; i < static_cast<uint8_t>(fm_parameter::param::_max); i++)
	{

		m_pComm->m_checkSync.keep_id = i;
		timer::_ms checkTM;

		SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::GET_RAM_PARAM), &i, 1);
		checkTM.Start();
		uint8_t retry_cnt = 3;
		while (m_pComm->m_checkSync.is_checked == false)
		{
			util::delay(5);
			//timeout
			uint64_t passtime = checkTM.Elaps();
			if (checkTM.MoreThan(500))
			{
				TRACE("LoadingParamterData time over %lld\t\n", passtime);
				break;
			}
			//retry 
			if (checkTM.MoreThan(100) && retry_cnt > 0)
			{
				retry_cnt--;
				TRACE("LoadingParamterData retry (%lld)\t\n", passtime);
				SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::GET_RAM_PARAM), &i, 1);
			}
		}
		::PostMessage(m_pComm->m_hCommWnd, WM_FASTECH_MOTOR_CMD_MESSAGE, static_cast<uint8_t>(fm_parameter::param::_max), i);
	}		
}


void jL_fastechMotor::SetUseISR(bool on_off)
{
	m_useISR = on_off;
	ISR_OnOff(on_off);
}

bool jL_fastechMotor::ISR_OnOff(bool on_off)
{
	if (on_off && m_useISR)
	{
		m_jobScheduler->Start(CALLBACK_GET_STATUS);
	}
	else
	{
		m_jobScheduler->Stop(CALLBACK_GET_STATUS);
	}		

	return on_off;
}




void jL_fastechMotor::Callback_GetAxisParam(void* obj)
{
	jL_fastechMotor* pThis = (jL_fastechMotor*)obj;
	
	/*
	uint64_t passtime = pThis->m_timer.Elaps();
	pThis->m_timer.Start();
	TRACE("timer pass time %lld\t\n", passtime);
	*/

	if (pThis->m_pComm->m_IsConnected == false)
		return;	

	uint8_t data = 1;
	pThis->m_pComm->m_checkSync.keep_id = data;
	pThis->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::GET_RAM_PARAM), &data, 1);
}

void jL_fastechMotor::Callback_GetAxisStatus(void* obj)
{
	jL_fastechMotor* pThis = (jL_fastechMotor*)obj;
	if (pThis->m_pComm->m_IsConnected == false)
		return;			
	
	pThis->SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::GET_MOTION_STATUS));
}


jL_fastechMotor::motor_state* jL_fastechMotor::GetMotorState()
{
	return &m_motorState;
}

int jL_fastechMotor::UpdateAxisData(uint8_t* p_data, uint16_t length, uint8_t index)
{
	uint8_t ret = 0;
	jL_fastechComm::fastech_cmdList cmd_index = static_cast<jL_fastechComm::fastech_cmdList>(index);

	uint32_t get_time, avg_time = 0;
	m_pComm->m_checkSync.avr_cnt++;
	get_time = (uint32_t)m_timer.Elaps();
	m_responseTime = get_time;
	if (m_pComm->m_checkSync.avr_cnt < 10)
	{
		m_pComm->m_checkSync.resp_time += get_time;
	}
	else
	{
		avg_time = m_pComm->m_checkSync.resp_time / m_pComm->m_checkSync.avr_cnt;
		m_pComm->m_checkSync.avr_cnt = 0;
		m_pComm->m_checkSync.resp_time = 0;
		m_responseTime = avg_time;
	}

	m_pComm->m_checkSync.is_checked = true;

	switch (cmd_index)
	{
	case jL_fastechComm::fastech_cmdList::GET_SLAVE_INFO:
		break;
	case jL_fastechComm::fastech_cmdList::GET_MOTOR_INFO:
		break;
	case jL_fastechComm::fastech_cmdList::SAVE_ROM_ALL:
		break;
	case jL_fastechComm::fastech_cmdList::GET_ROM_PARAM:
	{

	}
	break;
	case jL_fastechComm::fastech_cmdList::SET_RAM_PARAM:
	{
		SendCommand(static_cast<uint32_t>(jL_fastechComm::fastech_cmdList::GET_RAM_PARAM), &m_pComm->m_checkSync.keep_id, 1);
	}
	break;
	case jL_fastechComm::fastech_cmdList::GET_RAM_PARAM:
	{		
		byte tmp[4] = { 0, };
		for (int i = 0; i < length; i++)
		{
			tmp[i] = p_data[i];
		}
		m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));		

#if 0
		fm_parameter::param id = static_cast<fm_parameter::param>(m_pComm->m_checkSync.keep_id);
		switch (id)
		{
		case fm_parameter::param::pulse_per_revolution:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id,	conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::axis_max_speed:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::axis_start_speed:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::axis_acc_time:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::axis_dec_time:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::speed_override:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::jog_speed:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::jog_start_speed:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::jog_acc_dec_time:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::alarm_logic:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::run_stop_logic:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::alarm_reset_logic:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::sw_limit_plus:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::sw_limit_minus:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::sw_limit_stop_method:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::hw_limit_stop_method:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::limit_sensor_logic:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::org_speed:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::org_search_speed:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::org_acc_dec_time:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::org_method:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::org_dir:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::org_offset:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::org_position_set:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::org_sensor_logic:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::stop_current:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::motion_dir:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::limit_sensor_dir:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::encoder_multiply_value:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::encoder_dir:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		case fm_parameter::param::pos_value_counting_method:
			m_motorParam.Set(m_pComm->m_checkSync.keep_id, conv::DwToInt(&tmp[0]));
			break;
		default:
			break;
		}
#endif
		ret = 1;
	}
	break;
	case jL_fastechComm::fastech_cmdList::SET_IO_OUT_REG:
		break;
	case jL_fastechComm::fastech_cmdList::SET_IO_IN_REG:
		break;
	case jL_fastechComm::fastech_cmdList::GET_IO_IN_REG:
		break;
	case jL_fastechComm::fastech_cmdList::GET_IO_OUT_REG:
		break;
	case jL_fastechComm::fastech_cmdList::SET_ASSN_IO_MAP:
		break;
	case jL_fastechComm::fastech_cmdList::GET_ASSN_IO_MAP:
		break;
	case jL_fastechComm::fastech_cmdList::READ_IO_MAP:
		break;
	case jL_fastechComm::fastech_cmdList::TRG_OUT_RUN_A:
		break;
	case jL_fastechComm::fastech_cmdList::TRG_OUT_STATUS:
		break;
	case jL_fastechComm::fastech_cmdList::SERVO_ENABLE:
		break;
	case jL_fastechComm::fastech_cmdList::ALARAM_RESET:
		break;
	case jL_fastechComm::fastech_cmdList::GET_ALARM_INFO:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_STOP:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_EMG_STOP:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_ORG_SINGLE_AXIS:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_ABS_SINGLE_AXIS:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_REL_SINGLE_AXIS:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_TO_LIMIT:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_VELOCITY:
	{
		ret = 1;
	}
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_ABS_OVERRIDE:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_REL_OVERRIDE:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_VELOCITY_OVERRIDE:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_ALL_STOP:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_ALL_EMG_STOP:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_ALL_ORG_STOP:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_ALL_ABS_POS:
		break;
	case jL_fastechComm::fastech_cmdList::MOVE_ALL_REL_POS:
		break;
	case jL_fastechComm::fastech_cmdList::GET_AXIS_STATUS:
	{
		for (int i = 0; i < length; i++)
		{
			m_motorState.axis_status[REG_BANK(i)] = p_data[i];
		}
		m_motorState.is_motor_on = m_motorState.axis_status[REG_BANK(FM_STATE_01)][REG_BIT(REG_SERVO_ON)];
		ret = 1;
	}
	break;
	case jL_fastechComm::fastech_cmdList::GET_AXIS_IO_STATUS:
		break;
	case jL_fastechComm::fastech_cmdList::GET_MOTION_STATUS:
	{
		int step = 0;
		byte tmp[4] = { 0, };
		int index = 0;
		for (uint8_t i = 0; i < length; i++)
		{
			switch ((uint8_t)(i / 4))
			{
			case 0:
				tmp[index] = p_data[i];
				index++;
				break;
			case 1:
				tmp[index] = p_data[i];
				index++;
				break;
			case 2:
				tmp[index] = p_data[i];
				index++;
				break;
			case 3:
				tmp[index] = p_data[i];
				index++;
				break;
			case 4:
				m_motorState.axis_status[REG_BANK(index)] = p_data[i];
				index++;
				break;
			default:
				break;
			}

			if ((i + 1) % 4 != 0)
				continue;

			switch (step)
			{
			case 0:
				step++;
				m_motorState.cmd_pos = conv::DwToInt(&tmp[0]);
				memset(&tmp[0], 0, 4);
				index = 0;
				break;
			case 1:
				step++;
				m_motorState.act_pos = conv::DwToInt(&tmp[0]);
				memset(&tmp[0], 0, 4);
				index = 0;
				break;
			case 2:
				step++;
				m_motorState.pos_err = conv::DwToInt(&tmp[0]);
				memset(&tmp[0], 0, 4);
				index = 0;
				break;
			case 3:
				step++;
				m_motorState.act_vel = conv::DwToInt(&tmp[0]);
				memset(&tmp[0], 0, 4);
				index = 0;
				break;
			case 4:
				step++;
				// cplt
				break;
			default:
				break;
			}
		}//for (int i = 0; i < length; i++)			
		m_motorState.is_motor_on = m_motorState.axis_status[REG_BANK(FM_STATE_01)][REG_BIT(REG_SERVO_ON)];
		ret = 1;
	}
	break;
	case jL_fastechComm::fastech_cmdList::_max:
		break;
	}

	::PostMessage(m_pComm->m_hCommWnd, WM_FASTECH_MOTOR_REG_UPDATE_MESSAGE, 0, index);
	return ret;
}


