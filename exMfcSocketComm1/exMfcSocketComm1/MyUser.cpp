// MyUser.cpp: 구현 파일
//

#include "pch.h"
#include "exMfcSocketComm1.h"
#include "MyUser.h"


// MyUser

MyUser::MyUser()
{
}

MyUser::~MyUser()
{
}


// MyUser 멤버 함수


void MyUser::OnReceive(int nErrorCode)
{
	int value;
	Receive(&value, sizeof(int));

	AfxGetMainWnd()->SetDlgItemInt(IDC_VALUE_EDIT, value);

	value = 1;
	Send(&value, sizeof(int));
	CSocket::OnReceive(nErrorCode);
}


void MyUser::OnClose(int nErrorCode)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	ShutDown(2);
	Close();

	CSocket::OnClose(nErrorCode);
}
