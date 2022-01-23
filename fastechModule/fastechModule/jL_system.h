#pragma once

//#include "jL_fastechComm.h"
#include "jL_inforObj.h"

/** Hardware Layer */
#include "IIO.h"
#include "jL_ioManager.h"


class jL_system
{
	/****************************************************
	 *	data
	 ****************************************************/
private:
	//jL_fastechComm* m_fastech;
	jL_inforObj* m_pObjinfo;
	IIO* m_pIO;
public:


	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	jL_system();
	virtual ~jL_system();

	/****************************************************
	 *	func
	 ****************************************************/
private:
	void initSystem();
	void terminateSystem();

	// Hardware Layer
	int createFastechCompoent();
	int _createBasicHWComponents();
	void startThreads();

public:

	int	Initialize();
	IIO* GetIOComponent();
	jL_inforObj* GetSystemInfo();

};
	

