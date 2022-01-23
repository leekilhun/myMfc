#include "pch.h"
#include "jL_ioManager.h"

jL_ioManager::jL_ioManager(CommonAttribute commonData/*, unsigned short	usBoardNumber */)
	: ICommon(commonData)
{
	clearVariable();

	m_fasttechIO = new jL_fastechIO(commonData, L"COM2", 115200);

}

jL_ioManager::~jL_ioManager()
{
	if (m_fasttechIO != nullptr)
	{
		delete m_fasttechIO;
	}

	if (m_tableIO != nullptr)
	{
		delete m_tableIO;
	}
	clearVariable();
}

void jL_ioManager::clearVariable()
{
	m_fasttechIO = nullptr;
	m_tableIO = nullptr;
}

DWORD jL_ioManager::ThreadRun()
{
	m_fasttechIO->ThreadRun();
	return 0;
}

Table_IO* jL_ioManager::GetIOTable()
{
	/*
		CString in_title, out_title;
		int address = 0;

		// input map data
		length = m_IoTable->ioGetInputCount();
		for (int i = 0; i < length; i++){
			in_title = m_IoTable->ioGetInItem(i);
			address = IO_ADDR_BASE_INPUT + i;
		}

		// output map data
		length = m_IoTable->ioGetOutputCount();
		for (int i = 0; i < length; i++){
			out_title = m_IoTable->ioGetOutItem(i);
			address = IO_ADDR_BASE_OUTPUT + i;
		}
*/
	return m_tableIO;
}

int jL_ioManager::LoadIOMap(CString file_name /*= L"IO.Dat"*/)
{
	//TODO: 

	m_tableIO = new Table_IO(L"./io.dat"); //file_name
	
	// 콜백 함수 등록 후 map 데이터를 읽어 옴
	ioReadFile();		
	
	return 0;
}

int jL_ioManager::Initialize()
{
	return 0;
}

int jL_ioManager::Terminate()
{
	m_fasttechIO->Terminate();
	return 0;
}

bool jL_ioManager::IsOn(uint16_t addr)
{
	return false;
}

bool jL_ioManager::IsOff(uint16_t addr)
{
	return false;
}

int jL_ioManager::OutputOn(uint16_t addr)
{
	return 0;
}

int jL_ioManager::OutputOff(uint16_t addr)
{
	return 0;
}

int jL_ioManager::OutputToggle(uint16_t addr)
{
	return 0;
}

int jL_ioManager::ioReadFile()
{
	m_tableIO->ioSetCallback(this, &jL_ioManager::WarpFunc);
	m_tableIO->ioReadFileTrd();

	return 0;
}

int jL_ioManager::ioAssignTable()
{


	


	return 0;
}
