#pragma once

enum EPGM_Type
{
	ePGM_Type_None = -1,
	ePGM_Type_Normal = 0,

	ePGM_Type_Max
};

const CString DEF_NAME_PGM_TYPE[ePGM_Type_Max] = {
	_T("Normal")
};

class CPlatformManager
{
public:
	CPlatformManager();
	~CPlatformManager();

public:
	/**
	 * 멤버 변수를 초기화 한다.
	 */
	void Init();

	/**
	* 레지스트리에서 옵션 데이터를 읽어온다.
	*/
	BOOL LoadData();

	/**
	* 레지스트리에 옵션 데이터를 저장한다.
	*/
	BOOL SaveData();

public:
	/** Get Data */

	/** Set Data */
	void SetPGMType(EPGM_Type eType) { m_ePGMType = eType; };

private:
	/** Platform 동작 설비 타입 */
	EPGM_Type			m_ePGMType;
};

