#include "pch.h"
#include "CommSerial.h"



MCommSerial::MCommSerial()
{
	clearVariable();
}

MCommSerial::~MCommSerial()
{
	destrory();
}

void MCommSerial::clearVariable()
{
}

void MCommSerial::destrory()
{
}

void MCommSerial::AddStringPortList(CComboBox* combobox)
{
	if (combobox == NULL) return;

	//HKEY_LOCAL_MACHINE\HARDWARE\DEVICEMAP\SERIALCOMM ��  �ø�����Ʈ ��ȣ���� ��ϵǾ� ����.  
	//�������� �о���� ��. �ڵ�� �Ʒ� ��ũ���� ������  
	HKEY hKey;

	//������ �������� Ű�� ���� �⺻Ű �̸�  
	//������ �������� ����Ű �̸�  
	//��������Ű�� ���� �ڵ�  
	RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

	TCHAR szData[20], szName[100];
	DWORD index = 0, dwSize = 100, dwSize2 = 20, dwType = REG_SZ;
	combobox->ResetContent();
	memset(szData, 0x00, sizeof(szData));
	memset(szName, 0x00, sizeof(szName));



	//LPCTSTR sData, sName;

	//hKey - ��������Ű �ڵ�  
	//index - ���� ������ �ε���.. �ټ��� ���� ���� ��� �ʿ�  
	//szName - �׸��� ����� �迭  
	//dwSize - �迭�� ũ��  
	while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL))
	{
		index++;

		//szName-�����ͽ��� �׸��� �̸�  
		//dwType-�׸��� Ÿ��, ���⿡���� �η� ������ ���ڿ�  
		//szData-�׸��� ����� �迭  
		//dwSize2-�迭�� ũ��  
		RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
		combobox->AddString(CString(szData));

		memset(szData, 0x00, sizeof(szData));
		memset(szName, 0x00, sizeof(szName));
		dwSize = 100;
		dwSize2 = 20;
	}

	RegCloseKey(hKey);
}
