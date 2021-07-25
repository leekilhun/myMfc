#pragma once


class MCommSerial
{

#define BUFF_SIZE 256

	
	/*
	struct sRs232_baudrate_tbl { DWORD baudrate_data; LPCTSTR label; } 
	RS_232_BAUDRATE_DATA[12] =
	{
		{CBR_110,		_T("110")},
		{CBR_300,		_T("300")},
		{CBR_600,		_T("600")},
		{CBR_1200,		_T("1200")},
		{CBR_2400,		_T("2400")},
		{CBR_4800,		_T("4800")},
		{CBR_9600,		_T("9600")},
		{CBR_14400,		_T("14400")},
		{CBR_19200,		_T("19200")},
		{CBR_38400,		_T("38400")},
		{CBR_56000,		_T("56000")},
		{CBR_115200,	_T("115200")},

	} ;
	*/

public:
	MCommSerial();
	virtual ~MCommSerial();


private:
	/**
	*
	*/
	void	clearVariable();
	void	destrory();

public:
	void	AddStringPortList(CComboBox *combobox);


};

