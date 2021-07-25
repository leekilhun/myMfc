// MyServer.cpp: 구현 파일
//

#include "pch.h"
#include "exMfcSocketComm1.h"
#include "MyServer.h"


// MyServer

MyServer::MyServer()
{
}

MyServer::~MyServer()
{
}


// MyServer 멤버 함수
void MyServer::OnAccept(int nErrorCode)
{
	Accept(m_user);
	CSocket::OnAccept(nErrorCode);
}
