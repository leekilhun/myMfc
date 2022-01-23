#pragma once

#include "def_inforObj.h"
#include "ICommon.h"


class jL_inforObj
{

	/****************************************************
	 *	data
	 ****************************************************/
private:
	int m_idx;

	/* Number of component objects */
	int	m_iNumObjects;

	/* Object Á¤º¸ */
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

