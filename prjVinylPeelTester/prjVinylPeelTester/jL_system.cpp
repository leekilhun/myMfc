#include "pch.h"
#include "jL_system.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
  m_pMotor = nullptr;
  //m_pIO = nullptr;
}

void jL_system::terminateSystem()
{

  if (m_pObjinfo != nullptr)
  {
    delete m_pObjinfo;
    m_pObjinfo = nullptr;
  }

  if (m_pMotor != nullptr)
  {
    delete m_pMotor;
    m_pMotor = nullptr;
  }

  /*if (m_pIO != nullptr)
  {
    delete m_pIO;
    m_pIO = nullptr;
  }*/
}

int jL_system::createFastechMotorCompoent()
{
  m_pMotor = new jL_fastechMotor;

  return 0;
}

int jL_system::createNextionNcdCompoent()
{
  return 0;
}


//int jL_system::_createBasicHWComponents()
//{
//  int error = 0;
//  m_pIO = new jL_ioManager(m_pObjinfo->GetCommonData(HL_IO_BASE));
//
//  error = m_pObjinfo->RegisterComponent(HL_IO_BASE, m_pIO);
//  if (error)
//    AfxMessageBox(L"IO 객체 등록 실패");
//
//  m_pIO->LoadIOMap();
//
//  return 0;
//}

void jL_system::startThreads()
{
  //m_pMotor->();
}

int jL_system::Initialize()
{

  /** Object of System Information */
  m_pObjinfo = new jL_inforObj;

  //_createBasicHWComponents();
  createFastechMotorCompoent();
  createNextionNcdCompoent();
  //startThreads();

  return 0;
}

//
//IIO* jL_system::GetIOComponent()
//{
//  return m_pIO;
//}

IAxis* jL_system::GetMotorComponent()
{
  return m_pMotor;
}

jL_inforObj* jL_system::GetSystemInfo()
{
  return m_pObjinfo;
}
