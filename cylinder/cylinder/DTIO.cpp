#include "pch.h"
#include "DTIO.h"

DTIO::DTIO():m_IoTable(nullptr)/*, m_CntInputName(0), m_CntOutputName(0)*/
{
	//memset(m_IoItem, 0x00, sizeof(m_IoItem));

	/*for (int i = 0; i < IO_ADDR_TOTAL_MAX_CNT; i++)
	{
		m_IoItem[i] = nullptr;
	}*/

	//memset(m_NameInputAddr, 0x00, sizeof(m_NameInputAddr));
	/*for (int i = 0; i < IO_ADDR_MAX_INPUT; i++)
	{
		m_NameInputAddr[i] = nullptr;
	}*/


	m_IoTable = new Table_IO(L"./io.dat");

	ReadIoFile();

}

DTIO::~DTIO()
{
	if (m_IoTable != nullptr)
	{
		delete m_IoTable;
	}
	m_IoTable = nullptr;
	
	/*for (int i = 0; i < IO_ADDR_MAX_INPUT; i++)
	{
		if (m_NameInputAddr[i] != nullptr)
		{
			delete m_NameInputAddr[i];
		}
		m_NameInputAddr[i] = nullptr;
	}*/


	/*for (int i = 0; i < (int)m_CntOutputName; i++)
	{
		if (m_NameOutputAddr[i] != nullptr)
		{
			delete m_NameOutputAddr[i];
		}
		m_NameOutputAddr[i] = nullptr;
	}*/

	/*for (int i = 0; i < IO_ADDR_TOTAL_MAX_CNT; i++)
	{
		if (m_IoItem[i] != nullptr)
		{
			delete m_IoItem[i];
		}
		m_IoItem[i] = nullptr;
	}*/


}

Table_IO* DTIO::GetIOTable()
{
	return m_IoTable;
}

int DTIO::LoadIOMap()
{
	int cnt = 0;
	int size = 0, length = 0 ;
	performanceTimer timer;
	timer.StartTimer();
	TRACE("\n\r");
	TRACE("LoadIOMap Time Start : 0 [%7.6f] \n\r", timer.GetElaspTime());
	timer.StartTimer();

	//m_IoTable->ioReadFile();

	TRACE("LoadIOMap Time ReadFile&Parsing : 0 [%7.3f] \n\r", timer.GetElaspTime());
	timer.StartTimer();
#if 0
	for (int i = 0; i < m_IoTable->ioGetInputCount(); i++)
	{
		m_NameInputAddr[i] = new CString(m_IoTable->ioGetInItem(IO_ADDR_BASE_INPUT + i));
		//*m_NameInputAddr[i] = m_IoTable->ioGetInItem(IO_ADDR_BASE_INPUT + i);
		m_CntInputName++;
	}

	TRACE("LoadIOMap Time SetInputText : 0 [%7.3f] \n\r", timer.GetElaspTime());
	timer.StartTimer();
	//m_IoTable->ioGetOutItem(IO_ADDR_BASE_OutPUT + i);
	for (int i = 0; i < m_IoTable->ioGetOutputCount(); i++)
	{
		m_NameOutputAddr[i] = new TCHAR[IO_TITLE_MAX_CHART * sizeof(TCHAR)];
		memset(m_NameOutputAddr[i], 0x00, IO_TITLE_MAX_CHART * sizeof(TCHAR));
		m_IoTable->ioGetOutItem(IO_ADDR_BASE_OUTPUT + i, m_NameOutputAddr[i] , IO_TITLE_MAX_CHART-1);	
		m_CntOutputName++;	
	}
#endif
	TRACE("LoadIOMap Time SetOutputText : 0 [%7.3f] \n\r", timer.GetElaspTime());
	timer.StartTimer();
	//CString temp;
	//for (int i = 0; i < m_IoTable->ioGetOutputCount(); i++)
	//{
	//	//CString temp = m_IoTable->ioGetOutItem(IO_ADDR_BASE_OUTPUT + i);
	//}



		//temp =  m_IoTable->ioGetInItem(IO_ADDR_BASE_INPUT+i);
		//size = temp.GetLength() * sizeof(TCHAR)+1;
		//length = temp.GetLength() + 1;
		//m_NameInputAddr[i] = nullptr;
		//m_NameInputAddr[i] = new TCHAR(50);
		//

		//m_IoTable->ioGetInItem(IO_ADDR_BASE_INPUT + i, m_NameInputAddr[i],50);




		//LPTSTR buf = temp.GetBuffer();
		
		//_tcsncpy_s(m_NameInputAddr[i], length, buf, length +10);
		//m_NameInputAddr[IO_ADDR_HEAD_INPUT][IO_ADDR_BASE_INPUT + i] = temp.GetBuffer();
		//temp.ReleaseBuffer();

		//temp.Empty();
		

		/*temp = m_IoTable->ioGetOutItem(i);
		m_NameOutputAddr[IO_ADDR_HEAD_OUTPUT][IO_ADDR_BASE_OUTPUT + i] = nullptr;
		m_NameOutputAddr[IO_ADDR_HEAD_OUTPUT][IO_ADDR_BASE_OUTPUT + i] = new TCHAR(temp.GetLength());
		m_CntOutputName++;*/

		// 
		//m_NameOutputAddr[IO_ADDR_HEAD_OUTPUT][IO_ADDR_BASE_OUTPUT + i] = m_IoTable->ioGetOutItem(i);
	//}
	
	return cnt;
#if 0

	char buffer[_MAX_PATH];
	CString strFileName;
	FILE* InFile;
	int numclosed;
	int ch;
	CString	FormattedMessage;
	CString	Type;
	CString	Name;
	CString	Address;
	static int	inputAddressCount = 0;
	static int	outputAddressCount = 0;

	memset(InputIOTable, 0x00, sizeof(InputIOTable));
	memset(OutputIOTable, 0x00, sizeof(OutputIOTable));

	/* Get the current working directory: */
	if (_getcwd(buffer, _MAX_PATH) == NULL)
		perror("_getcwd error");
	else
		printf("%s\n", buffer);

	// IO Map File Name
	strFileName.Format("%s\\Info\\%s", buffer, FileName);

	/* Open for read (will fail if file "data" does not exist) */
	if ((InFile = fopen(strFileName, "r")) == NULL)
	{
		printf("The file 'data' was not opened\n");
		return ERR_IO_INVALID_ADDRESS_NUMVER;
	}
	else {
		printf("The file 'data' was opened\n");
	}


	while (!feof(InFile))
	{
		//	fscanf(InFile,"%s\n",buffer);
		memset(buffer, 0x00, sizeof(buffer));

		//fread( buffer, sizeof( char ), 80, InFile );

		ch = fgetc(InFile);
		for (int i = 0; (ch != '\n') & (feof(InFile) == 0); i++)
		{
			buffer[i] = (char)ch;
			ch = fgetc(InFile);
		}

		FormattedMessage = buffer;
		FormattedMessage.TrimLeft();
		FormattedMessage.TrimRight();

		int n = FormattedMessage.Find(';', 0);
		if (n != -1)
			continue;
		else
		{
			int iType = FormattedMessage.Find(',', 0);
			Type = FormattedMessage.Left(iType);
			Type.TrimLeft();
			Type.TrimRight();

			int iName = FormattedMessage.Find(',', iType + 1);

			Name = FormattedMessage.Mid(iType + 1, iName - iType - 1);
			Name.TrimLeft();
			Name.TrimRight();


			Address = FormattedMessage.Mid(iName + 1);
			Address.TrimLeft();
			Address.TrimRight();


			if (!strcmp(Type, "INPUT"))
			{
				InputIOTable[inputAddressCount].Type = INPUT_ADDRESS;
				if (strlen(Name) > 80) {
					fclose(InFile);
					return FALSE;
				}
				strcpy(InputIOTable[inputAddressCount].Name, Name);
				InputIOTable[inputAddressCount].Address = atoi(Address);
				InputIOTable[inputAddressCount].Index = (atoi(Address) - INPUT_BASE_ADDRESS) % (CHAR_BIT * 2);

				inputAddressCount++;

			}
			else if (!strcmp(Type, "OUTPUT"))
			{
				OutputIOTable[outputAddressCount].Type = OUTPUT_ADDRESS;
				if (strlen(Name) > 80) {
					fclose(InFile);
					return FALSE;
				}
				strcpy(OutputIOTable[outputAddressCount].Name, Name);
				OutputIOTable[outputAddressCount].Address = atoi(Address);
				OutputIOTable[outputAddressCount].Index = (atoi(Address) - OUTPUT_BASE_ADDRESS) % (CHAR_BIT * 2);

				outputAddressCount++;
			}
		}

	}

	/* Close stream */
	if (fclose(InFile))
		printf("The file 'data' was not closed\n");

	/* All other files are closed: */
	numclosed = _fcloseall();
	//   printf( "Number of files closed by _fcloseall: %d\n", numclosed );
	return ERR_IO_SUCCESS;

#endif // 0


}

int DTIO::Initialize()
{
	return 0;
}

void DTIO::RunIOThread()
{
}

int DTIO::Terminate()
{
	return 0;
}

BOOL DTIO::IsOn(unsigned short addr)
{
	return 0;
}

BOOL DTIO::IsOff(unsigned short addr)
{
	return 0;
}

int DTIO::OutputOn(unsigned short addr)
{
	return 0;
}

int DTIO::OutputOff(unsigned short addr)
{
	return 0;
}

int DTIO::OutputToggle(unsigned short addr)
{
	return 0;
}

void DTIO::ReadIoFile()
{	
	m_IoTable->ioSetCallback(this, &DTIO::WarpFunc);
	m_IoTable->ioReadFileTrd();	
	
}

void DTIO::Test()
{
}
