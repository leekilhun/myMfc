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
	 * ��� ������ �ʱ�ȭ �Ѵ�.
	 */
	void Init();

	/**
	* ������Ʈ������ �ɼ� �����͸� �о�´�.
	*/
	BOOL LoadData();

	/**
	* ������Ʈ���� �ɼ� �����͸� �����Ѵ�.
	*/
	BOOL SaveData();

public:
	/** Get Data */

	/** Set Data */
	void SetPGMType(EPGM_Type eType) { m_ePGMType = eType; };

private:
	/** Platform ���� ���� Ÿ�� */
	EPGM_Type			m_ePGMType;
};

