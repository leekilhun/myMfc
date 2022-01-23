#include "pch.h"
#include "jL_fastechMotor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

jL_fastechMotor::jL_fastechMotor():
	m_pComm(nullptr), m_motorId(0), m_motorState({})
{
	Initialize();
}

jL_fastechMotor::~jL_fastechMotor()
{

	PortClose();

	if (m_pComm != nullptr)
		delete m_pComm;
	m_pComm = nullptr;

}

int jL_fastechMotor::Initialize()
{

	m_pComm = new jL_fastechComm;
	return 0;
}

int jL_fastechMotor::PortOpen(CString portname, DWORD baudrate, WORD port_no, HWND hwnd)
{
	int retSuccess = 0;
	if (m_pComm->OpenPort(portname, baudrate, port_no, hwnd) == retSuccess)
	{
		m_pComm->ReadyPort();
	//	m_pComm->ThreadRun();
	}
	return retSuccess;
}

void jL_fastechMotor::PortClose()
{
	m_pComm->Terminate();
}

void jL_fastechMotor::SendCommand(uint32_t cmd_index, uint8_t id)
{
	if (m_pComm->m_IsConnected == false)
		return;
	m_pComm->SetMotorId(id);
	m_pComm->SendCmd(cmd_index);
}

int jL_fastechMotor::MotorOnOff(bool on_off)
{
	uint8_t data = (uint8_t)on_off;

	m_pComm->SetMotorId(m_motorState.id);
	return m_pComm->SendCmd(jL_fastechComm::fastech_cmdList::SERVO_ENABLE, &data, 1);
}

jL_fastechMotor::motor_state* jL_fastechMotor::GetMotorState()
{
	return &m_motorState;
}
