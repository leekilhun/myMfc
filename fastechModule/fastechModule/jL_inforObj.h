#pragma once

#include "def_inforObj.h"
#include "ICommon.h"


//
///** Value Define */
//const	int		DEF_MAX_OBJECT_NO = 6000;
//const	int		DEF_MAX_OBJECT_INFO_NO = 500;
//const	int		DEF_MAX_OBJ_KINDS = 100;
//const	int		DEF_MAX_SYSTEM_INFO_LOG_ITEM = 200;
//
///** Error Define */
//const	int		ERR_SYSTEMINFO_SUCCESS = 0;
//const	int		ERR_SYSTEMINFO_INVALID_POINTER = 1;
//const	int		ERR_SYSTEMINFO_INVALID_OBJECTID = 2;
//const	int		ERR_SYSTEMINFO_NOT_REGISTED_OBJECTID = 3;
//const	int		ERR_SYSTEMINFO_CANNOT_FIND_LOG_OBJECT = 4;
//const	int		ERR_SYSTEMINFO_REGISTER_FAIL = 5;
//
//const	int		DEF_SYSTEM_INFO_MIN_LOG_KEEPING_DAYS = 5;
//const	int		DEF_SYSTEM_INFO_MAX_LOG_KEEPING_DAYS = 90;
//
//
///** Object Base 정의 */
////Hardware Layer (0~1999)
//const int	DEF_IO_OBJECT_BASE = 1;
//const int	DEF_MOTION_LIB_OBJECT_BASE = 2;
//const int	DEF_SERIAL_OBJECT_BASE = 10;
//const int	DEF_CYLINDER_OBJECT_BASE = 100;
//const int	DEF_VACUUM_OBJECT_BASE = 500;
//const int	DEF_INDMOTOR_OBJECT_BASE = 900;
//const int	DEF_MOTION_OBJECT_BASE = 1000;
//
//
////Mechanical Layer (2000~2999)
//const int	DEF_OPPANEL_OBJECT_BASE = 2000;
//
////Control Layer (3000 ~ 3999)
//const int	DEF_CTR_MANAGE_OPPANEL_OBJECT_BASE = 3000;
//
////Process Layer (4000 ~ 4999)
//const int	DEF_TRS_AUTOMANAGER_OBJECT_BASE = 4000;
//const int	DEF_TRS_TOWERLAMP_OBJECT_BASE = 4050;
//

//enum class LogCategory
//{
//	TactTime,
//	Hardware,
//	Mechanical,
//	Control,
//	Process,
//};

class jL_inforObj
{

	/****************************************************
	 *	data
	 ****************************************************/
private:
	int m_idx;

	/* Number of component objects */
	int	m_iNumObjects;

	/* Object 정보 */
	ObjDataDef* m_infoObject;

public:


	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	jL_inforObj();
	virtual ~jL_inforObj();

	/****************************************************
	 *	func
	 ****************************************************/
private:
	void	makeSystemInfo();
	//int		getInstanceNo(int obj_id) const;
	ObjDataDef* getObjectData(int obj_id) const;

public:
	CommonAttribute GetCommonData(int id);
	ObjType	GetObjectType(int id) const;
	CString GetObjectName(int id) const;
	CString GetLogFileName(int id) const;
	ICommon* GetSystemObject(int id);
	int		RegisterComponent(int id, ICommon* pComp);

};

