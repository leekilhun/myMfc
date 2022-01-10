#pragma once

#include "IIO.h"



class DTIO :  public IIO
{
 /****************************************************
	*	data
	****************************************************/
private:
  /* Input IO Table */
  UINT32 m_IoAddress;
  Table_IO*	m_IoTable;

  //CString* m_IoItem[IO_ADDR_TOTAL_MAX_CNT];
  //TCHAR* m_NameInputAddr[IO_ADDR_MAX_INPUT];
  //CString*  m_NameInputAddr[IO_ADDR_MAX_INPUT];
  //UINT32 m_CntInputName;
  //TCHAR* m_NameOutputAddr[IO_ADDR_MAX_OUTPUT];
  //UINT32 m_CntOutputName;

public:

 /****************************************************
	*	Constructor
	****************************************************/
public:
	DTIO();

	virtual ~DTIO();

 /****************************************************
  *	func
  ****************************************************/
public:
  Table_IO*	GetIOTable();
    
  int LoadIOMap();

  int Initialize();

  void RunIOThread();

  int Terminate();

  BOOL IsOn(unsigned short addr);

  BOOL IsOff(unsigned short addr);

  int OutputOn(unsigned short addr);

  int OutputOff(unsigned short addr);

  int OutputToggle(unsigned short addr);

public:
  CString& operator[](const int index) {
    //return *m_IoItem[index];
    int addr = 0;
    CString ret;
    if (index < IO_ADDR_BASE_OUTPUT)
    {//input
      if (index < IO_ADDR_BASE_INPUT)
        return CString();
      addr = index - IO_ADDR_BASE_INPUT;
      if (addr < IO_ADDR_MAX_INPUT)
        ret = vrTCHARToCString((TCHAR*)m_IoTable->IN_TITLE[addr]);
    }
    else
    {//output
      if (index < IO_ADDR_BASE_OUTPUT)
        return CString();
      addr = index - IO_ADDR_BASE_OUTPUT;
      if (addr < IO_ADDR_MAX_OUTPUT)
        ret = vrTCHARToCString((TCHAR*)m_IoTable->OUT_TITLE[addr]);
    }
    return  ret;
  }
  
  CString GetTitle(const int address) {
    int addr = 0;
    CString ret;
    if (address < IO_ADDR_BASE_OUTPUT)
    {//input
      if (address < IO_ADDR_BASE_INPUT)
        return CString();
      addr = address - IO_ADDR_BASE_INPUT;
      if (addr < IO_ADDR_MAX_INPUT)
        ret = vrTCHARToCString((TCHAR*)m_IoTable->IN_TITLE[addr]);
    }
    else
    {//output
      if (address < IO_ADDR_BASE_OUTPUT)
        return CString();
      addr = address - IO_ADDR_BASE_OUTPUT;
      if (addr < IO_ADDR_MAX_OUTPUT)
        ret = vrTCHARToCString((TCHAR*)m_IoTable->OUT_TITLE[addr]);
    }
    return  ret;
  }

#if 0
  CString GetInputItem(const int index) {
    return *m_NameInputAddr[index];
  }

  CString GetOutputItem(const int index) {
    return vrTCHARToCString(m_NameOutputAddr[index]);
  }
#endif
  void ReadIoFile();
  void Test();
  static int WarpFunc(void* obj) {
    DTIO* self = (DTIO*)(obj);    
    return self->LoadIOMap();
  }
};

