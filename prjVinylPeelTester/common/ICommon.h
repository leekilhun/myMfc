#pragma once
#include "pch.h"
#include "Log.h"

const	int	ERR_ICOMMON_SUCCESS = 0;

const int LOG_DEFAULT_KEEPING_DAYS = 60;

//
//struct Attribute_t
//{
//	cLog Log;
//	int		InstanceNo;
//	int		LogKeepingDays;
//	CString	ObjectName;
//};


class CommonAttribute
{
	/****************************************************
	 *	data
	 ****************************************************/
public:
	int	 m_objNo;
	int	 m_instanceNo;
	CString	m_objName;
	CString m_logFileName;
	int  m_logKeepingDays;



	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	CommonAttribute() { Init(); }
	CommonAttribute(const CommonAttribute& self)
	{
		m_objNo = self.m_objNo;
		m_instanceNo = self.m_instanceNo;
		m_objName = self.m_objName;
		m_logFileName = self.m_logFileName;
		m_logKeepingDays = self.m_logKeepingDays;
	}

	virtual  ~CommonAttribute() { Init(); }

	/****************************************************
	 *	func
	 ****************************************************/
	inline void Init() {
		m_objNo = 0;
		m_instanceNo = 0;
		m_objName.Empty();
		m_logFileName.Empty();
		m_logKeepingDays = LOG_DEFAULT_KEEPING_DAYS;
	};

	inline int SetData(CommonAttribute* data) {
		m_objNo = data->m_objNo;
		m_instanceNo = data->m_instanceNo;
		m_objName = data->m_objName;
		m_logFileName = data->m_logFileName;
		m_logKeepingDays = data->m_logKeepingDays;

		return 0;
	}

	inline void GetData(CommonAttribute* data) const {
		data->m_objNo = m_objNo;
		data->m_instanceNo = m_instanceNo;
		data->m_objName = m_objName;
		data->m_logFileName = m_logFileName;
		data->m_logKeepingDays = m_logKeepingDays;
	}

	inline CommonAttribute GetData() const {
		CommonAttribute data;
		data.m_objNo = m_objNo;
		data.m_instanceNo = m_instanceNo;
		data.m_objName = m_objName;
		data.m_logFileName = m_logFileName;
		data.m_logKeepingDays = m_logKeepingDays;
		return data;
	}


	inline CString GetObjectName() const {
		return m_objName;
	}

	inline int GetInstanceNo() const {
		return m_instanceNo;
	}

	inline int SetInstanceNo(int no) {
		m_instanceNo = no;
		return 0;
	}

	inline CString GetLogFileName() const {
		return m_logFileName;
	}


	inline int GetLogKeepingDays() const {
		return m_logKeepingDays;
	}

	inline int GetObjectID() const {
		return m_objNo;
	}

	inline int SetObjectID(int id) {
		m_objNo = id;
		return 0;
	}

	inline int SetObjectName(CString name) {
		m_objName = name;
		return 0;
	}

	inline int SetLogFileName(CString name) {
		m_logFileName = name;
		return 0;
	}
};



/** @interface */
class ICommon
{
	/****************************************************
	 *	data
	 ****************************************************/
private:

public:

protected:
	/** �Ӽ� ������*/
	CommonAttribute m_Attribute;

	/** Log */
	MLog* m_pLogMng;

	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	ICommon() :m_pLogMng(nullptr){
		m_pLogMng = new MLog;
	};
	ICommon(CommonAttribute commonData) {
		m_Attribute = commonData;
		m_pLogMng = new MLog;
	}

	/** @destructor */
	virtual ~ICommon() {	
		if (m_pLogMng != nullptr)
			delete m_pLogMng;
		m_pLogMng = nullptr;
	}

	/****************************************************
	 *	func
	 ****************************************************/
	 /** Set Common Attribute */
	inline void SetCommonAttribute(CommonAttribute commonData) {
		m_Attribute = commonData;
	}

	/** Get Common Attribute */
	void GetCommonAttribute(CommonAttribute* pCommonData) {
		if (pCommonData == NULL)
			return;
		pCommonData->SetData(&m_Attribute);
	}

	/** GetObject Name */
	inline CString GetObjectName() const {
		return m_Attribute.GetObjectName();
	}

	/// <summary>
	/// Object ID�� �����Ѵ�.
	/// </summary>
	/// <param name="ObjectID">������ Object ID</param>
	inline void SetObjectID(int ObjectID) {
		m_Attribute.SetObjectID(ObjectID);
	}

	/// <summary>
	/// Object ID�� �д´�
	/// </summary>
	/// <param name=""></param>
	/// <returns> Object ID</returns>
	inline int GetObjectID(void) const {
		return m_Attribute.GetObjectID();
	}

	/// <summary>
	/// Instance ��ȣ�� �����Ѵ�.
	/// </summary>
	/// <param name="InstanceNo">������ Instacne ��ȣ</param>
	inline void SetInstanceNo(int InstanceNo) {
		m_Attribute.SetInstanceNo(InstanceNo);
	}

	/// <summary>
	/// Instance ��ȣ�� �д´�.
	/// </summary>
	/// <param name=""></param>
	/// <returns>Instacne ��ȣ</returns>
	inline int GetInstanceNo(void) {
		return m_Attribute.GetInstanceNo();
	}


protected:

	/// <summary>
	/// Error Code �����ϱ�
	/// </summary>
	/// <param name="iErrorCode">�߻��� Error Code</param>
	/// <returns>Object ID (2bytes)�� Error Code + Error Base (2bytes)�� 4bytes�� ������ �ڵ�</returns>
	inline int	generateErrorCode(int err_code) {
#ifndef	NO_ERROR_ENCODING
		int	iError = 0;
		/** Error Code�� SUCCESS�� �ƴϸ� �ڵ� ���� */
		if (err_code != 0)
			iError = err_code * 1000 + m_Attribute.GetInstanceNo();
		/** Error Code�� SUCCESS�̸� SUCCESS return */
		else
			iError = 0;

		return iError;
#else
		return err_code;
#endif
	}

};