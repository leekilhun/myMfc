#pragma once

//#include "jL_fastechComm.h"
#include "jL_inforObj.h"

/** Hardware Layer */
#include "jL_fastechMotor.h"

class jL_system
{
	/****************************************************
	 *	data
	 ****************************************************/
private:
	//jL_fastechComm* m_fastech;
	jL_inforObj* m_pObjinfo;
	//IIO* m_pIO;
	jL_fastechMotor* m_pMotor;  //IAxis*

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
	int createFastechMotorCompoent();
	int createNextionNcdCompoent();
	//int _createBasicHWComponents();
	void startThreads();

public:

	int	Initialize();
	//IIO* GetIOComponent();
	IAxis* GetMotorComponent();
	jL_inforObj* GetSystemInfo();

};


