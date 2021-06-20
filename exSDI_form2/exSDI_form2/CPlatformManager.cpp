#include "pch.h"
#include "CPlatformManager.h"

CPlatformManager::CPlatformManager()
{
	Init();
}

CPlatformManager::~CPlatformManager()
{
	Init();
}

void CPlatformManager::Init()
{
	m_ePGMType = ePGM_Type_None;
}

BOOL CPlatformManager::LoadData()
{
	return TRUE;
}

BOOL CPlatformManager::SaveData()
{
	
	return TRUE;
}

