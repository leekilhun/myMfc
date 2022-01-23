#pragma once
#include "IIO.h"
#include "jL_fastechIO.h"

class jL_ioManager :  public IIO
{
	/****************************************************
	 *	data
	 ****************************************************/
	jL_fastechIO* m_fasttechIO;
	Table_IO* m_tableIO;

 /****************************************************
 	*	Constructor
 	****************************************************/
public:
	jL_ioManager(CommonAttribute commonData/*, unsigned short	usBoardNumber = 0 */);

	virtual ~jL_ioManager();

 /****************************************************
  *	func
	****************************************************/
private:
	void clearVariable();

public:
	DWORD ThreadRun();


	// Table_IO 객체를 반환 
	Table_IO* GetIOTable();

	/// <summary>
	/// Table_IO 객체를 생성하고 Map 파일을 읽어 데이터 저장 
	/// </summary>
	/// <param name="file_name"></param>
	/// <returns></returns>
	int LoadIOMap(CString file_name = L"IO.Dat");

	int Initialize();

	int Terminate();

	bool IsOn(uint16_t addr);

	bool IsOff(uint16_t addr);

	int OutputOn(uint16_t addr);

	int OutputOff(uint16_t addr);

	int OutputToggle(uint16_t addr);



	

	// 파일 로딩 지연을 없게 하기 위해 콜백사용
	// 1.Table_IO 생성 후 랩핑된 함수를 등록 
	// 2.io file에서 map 데이터를 읽어 table_io 객체에 저장
	int ioReadFile();

	/// <summary>
	/// Table_IO 객체에서 
	/// </summary>
	/// <returns></returns>
	int ioAssignTable();

	/// <summary>
	/// 콜백될 클래스 함수 포인터를 등록
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	static int WarpFunc(void* obj) {
		jL_ioManager* self = (jL_ioManager*)(obj);
		return self->ioAssignTable();
	}

	
};

