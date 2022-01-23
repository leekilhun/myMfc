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


	// Table_IO ��ü�� ��ȯ 
	Table_IO* GetIOTable();

	/// <summary>
	/// Table_IO ��ü�� �����ϰ� Map ������ �о� ������ ���� 
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



	

	// ���� �ε� ������ ���� �ϱ� ���� �ݹ���
	// 1.Table_IO ���� �� ���ε� �Լ��� ��� 
	// 2.io file���� map �����͸� �о� table_io ��ü�� ����
	int ioReadFile();

	/// <summary>
	/// Table_IO ��ü���� 
	/// </summary>
	/// <returns></returns>
	int ioAssignTable();

	/// <summary>
	/// �ݹ�� Ŭ���� �Լ� �����͸� ���
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	static int WarpFunc(void* obj) {
		jL_ioManager* self = (jL_ioManager*)(obj);
		return self->ioAssignTable();
	}

	
};

