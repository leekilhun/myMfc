#pragma once




class MWinSystem
{
public:
	MWinSystem();
	virtual ~MWinSystem();

	HWND m_mainHwnd;

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
	//MSystemData* GetSystemData();

private:
	int		_createBasicHWComponents();


private:


};

