#pragma once

#include "DefSystem.h"


class CThreadComm;

class MWinSystem
{
public:
	MWinSystem();
	virtual ~MWinSystem();

	HWND m_mainHwnd;


private:
	CThreadComm* m_HwSerialComm;

private:
	/**
	*
	*/
	void	clearVariable();
	void	destrory();

public:
	int		Initialize();
	void	DeleteOldLogFiles(LPCTSTR path);


private:
	void	linkThread();
	void	startThread();

public:
	CThreadComm* GetHwSerialComm() const {return m_HwSerialComm;}

private:
	int		_createBasicHWComponents();


private:


};

