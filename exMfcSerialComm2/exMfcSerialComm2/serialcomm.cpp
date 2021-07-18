
#include "pch.h"
#include "framework.h"
#include "serialcomm.h"


CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}


int CSerialComm::connect(char* portNum)
{
	if (!serial.OpenPort((CString)portNum))
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_9600, 8, FALSE, NOPARITY, ONESTOPBIT);
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0);

	return RETURN_SUCCESS;
}


int CSerialComm::sendCommand(BYTE * pos, UINT size)
{
	if (serial.WriteByte(pos,size))
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}



int CSerialComm::readByte(BYTE * resp, UINT size)
{
	if (serial.ReadByte(resp, size)) {
		return RETURN_SUCCESS;
	}
	else {
		return RETURN_FAIL;
	}
}




void CSerialComm::disconnect()
{
	serial.ClosePort();
}



int CSerialComm::getHexData(char * String, BYTE * returnBytes)
{
	int i = 0;
	BYTE data[256];
	int j = 0;
	while (*(String + i) != '\0') {
		if (!strncmp(String + i, "0x",2)) {
			BYTE ten = *(String + i + 2);
			BYTE one = *(String + i + 3);

			if (ten >= 'a' && ten <= 'f') {
				ten = (((ten - 'a') + 10) * 16);
			}
			else if(ten >= '0' && ten <= '9'){
				ten = ((ten - '0') * 16);
			}

			if (one >= 'a' && one <= 'f') {
				one = ((one - 'a') + 10);
			}
			else if (one >= '0' && one <= '9') {
				one = (one - '0');
			}
			i = i + 4;
			data[j] = (BYTE)(ten + one);
			j++;
		}
		else {
			i++;
		}
	}
	memcpy(returnBytes, data, j);
	return j;
}