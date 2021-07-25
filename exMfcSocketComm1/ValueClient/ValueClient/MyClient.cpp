// MyClient.cpp: 구현 파일
//

#include "pch.h"
#include "ValueClient.h"
#include "MyClient.h"


// MyClient

MyClient::MyClient()
{
}

MyClient::~MyClient()
{
}


// MyClient 멤버 함수


void MyClient::OnReceive(int nErrorCode)
{
	int value;
	Receive(&value, sizeof(int));

	AfxGetMainWnd()->SetDlgItemInt(IDC_VALUE_EDIT, value);

	CSocket::OnReceive(nErrorCode);
}
