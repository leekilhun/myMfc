#pragma once

// MyUser 명령 대상

class MyUser : public CSocket
{
public:
	MyUser();
	virtual ~MyUser();
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
};


