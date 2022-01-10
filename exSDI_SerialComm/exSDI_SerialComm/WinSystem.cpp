#include "pch.h"
#include "WinSystem.h"


#include "ThreadComm.h"


static const int  ERR_SYSTEM_SUCCESS = 0;


MWinSystem::MWinSystem()
{
	clearVariable();
}

MWinSystem::~MWinSystem()
{
	destrory();
}

void MWinSystem::clearVariable()
{
	m_HwSerialComm = nullptr;
	m_mainHwnd = nullptr;
}

void MWinSystem::destrory()
{
	// 1. Thread stoop

	// 2. delete object
	if (m_HwSerialComm != nullptr) delete m_HwSerialComm;

}

int MWinSystem::Initialize()
{
	_createBasicHWComponents();

	return ERR_SYSTEM_SUCCESS;
}

void MWinSystem::DeleteOldLogFiles(LPCTSTR path)
{
}

void MWinSystem::linkThread()
{
}

void MWinSystem::startThread()
{
}


int MWinSystem::_createBasicHWComponents()
{
	m_HwSerialComm = new CThreadComm();
	CString port_name = _T("COM10");
	WORD port_id = 10;
	if (!m_HwSerialComm->OpenPort(port_name, 115200, port_id))
		AfxMessageBox(L"connect faliled");
	else
		AfxMessageBox(L"connect successed\n");


	return 0;
}
