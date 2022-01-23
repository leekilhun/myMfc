#include "pch.h"
#include "jL_inforObj.h"

static ObjDataDef::Info_t systemObj[] =
{
  { FALSE,ObjType::NONE					,NULL							,0	,_T("None")						,_T("")												,NULL},
  { FALSE,ObjType::HL_IO				,HL_IO_BASE				,0	,_T("IO")							,_T("./Log/IO.log")						,NULL},
  { FALSE,ObjType::HL_SERIAL		,HL_SERIAL_BASE		,1	,_T("Serial")					,_T("./Log/Serial.log")				,NULL},
  { FALSE,ObjType::ML_OP_PANEL	,ML_OPPANEL_BASE	,0	,_T("Op Panel")				,_T("./Log/OpPanel.log")			,NULL},
  { FALSE,ObjType::PL_AUTO_MANAGER	,TRS_AUTOMANAGER_BASE	,0	,_T("Auto Manager")				,_T("./Log/AutoManager.log")			,NULL},
};


jL_inforObj::jL_inforObj() :m_iNumObjects(0), m_idx(0)
{
  makeSystemInfo();
}

jL_inforObj::~jL_inforObj()
{
  if (m_infoObject != nullptr)
  {
    delete[] m_infoObject;
  }

}

void jL_inforObj::makeSystemInfo()
{
  ObjDataDef::Info_t* pSys = &systemObj[0];
  int len = sizeof(systemObj) / 40;
  m_infoObject = new ObjDataDef[len];
  int i = 0;
  while (i < len)
  {
    if (pSys->type == ObjType::NONE)
      goto _exit;

    m_infoObject[i].SetData(pSys);

    m_iNumObjects++;
  _exit:
    i++;
    pSys++;
  }

}


ObjDataDef* jL_inforObj::getObjectData(int obj_id) const
{
  ObjDataDef* p_data = &m_infoObject[0];
  int max = m_iNumObjects;
  while ((p_data->IsMatchingID(obj_id)) == false)
  {
    p_data++;
    if (0 == max--)
    {
      return nullptr;
    }
  }

  return p_data;
}


CommonAttribute jL_inforObj::GetCommonData(int id)
{
  CommonAttribute commonData;
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return commonData;

  commonData.SetObjectID(id);
  commonData.SetInstanceNo(p_data->GetObjectInstanceNo());
  commonData.SetObjectName(p_data->GetObjectName());
  commonData.SetLogFileName(p_data->GetLogFileName());
  return commonData;
}

ObjType jL_inforObj::GetObjectType(int id) const
{
  return ObjType();
}

CString jL_inforObj::GetObjectName(int id) const
{
  return CString();
}

CString jL_inforObj::GetLogFileName(int id) const
{
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return CString();

  return p_data->GetLogFileName();}

ICommon* jL_inforObj::GetSystemObject(int id)
{
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return nullptr;
  return p_data->GetSystemObject();
}

int jL_inforObj::RegisterComponent(int id, ICommon* pComp)
{
  ObjDataDef* p_data = getObjectData(id);
  if (p_data == nullptr)
    return -1;

  p_data->SetSystemObject(pComp);
  return 0;
}
