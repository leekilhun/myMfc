#include "pch.h"
#include "jL_system.h"

jL_system::jL_system()
{
  initSystem();
}

jL_system::~jL_system()
{
  terminateSystem();
  initSystem();
}

void jL_system::initSystem()
{
  m_pObjinfo = nullptr;
  m_pIO = nullptr;
}

void jL_system::terminateSystem()
{
 
  if (m_pObjinfo != nullptr)
  {
    delete m_pObjinfo;
    m_pObjinfo = nullptr;
  }

  if (m_pIO != nullptr)
  {
    delete m_pIO;
    m_pIO = nullptr;
  }
}


int jL_system::_createBasicHWComponents()
{
  int error = 0;
  m_pIO = new jL_ioManager(m_pObjinfo->GetCommonData(HL_IO_BASE));

  error = m_pObjinfo->RegisterComponent(HL_IO_BASE, m_pIO);
  if (error)
    AfxMessageBox(L"IO ??ü ???? ????");

  m_pIO->LoadIOMap();

  return 0;
}

void jL_system::startThreads()
{
  m_pIO->ThreadRun();
}

int jL_system::Initialize()
{

  /** Object of System Information */
  m_pObjinfo = new jL_inforObj;

  _createBasicHWComponents();

  //startThreads();

  return 0;
}


IIO* jL_system::GetIOComponent()
{
  return m_pIO;
}

jL_inforObj* jL_system::GetSystemInfo()
{
  return m_pObjinfo;
}
