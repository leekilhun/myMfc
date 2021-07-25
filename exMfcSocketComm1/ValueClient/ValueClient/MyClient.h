#pragma once

// MyClient 명령 대상

class MyClient : public CSocket
{
public:
	MyClient();
	virtual ~MyClient();
	virtual void OnReceive(int nErrorCode);
};


