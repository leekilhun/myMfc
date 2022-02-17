#pragma once

//#pragma comment(lib,"Ws2_32.lib")
//#include <WinSock2.h>


#include <thread>
#include <string>

class Client :
    public CWnd
{
public:
	Client(CWnd* pParent);
	~Client();

public:
	CWnd* m_pParent;
	bool bRecv, bSend;

private:
	SOCKET client_sock;
	SOCKADDR_IN addr;
	std::thread* m_pRecvThread, * m_pSendThread;
	std::wstring m_nick;

public:
	void disconnectServer();
	static std::wstring getMyip();
	bool connectServer(std::wstring ip, int port);

	bool sendText(const std::wstring& msg, const DATA_TYPE& type = DATA_TYPE::_TEXT);
	bool sendNick(const std::wstring& msg, const DATA_TYPE& type = DATA_TYPE::_NICK);
	bool sendFile(const std::wstring& file_name, const std::wstring& file_path, const std::wstring& file_ext, const DATA_TYPE& type);

	bool sendSocket(const char* buf, const size_t& size);

	void recvFinished(const DATA_TYPE& type, const char* buf, const size_t& recv_size, const size_t& data_size);



	char* ImageToBytes(const std::wstring& file_path, const std::wstring& file_ext, size_t& size);
	char* FileToBytes(const std::wstring& file_path, const std::wstring& file_ext, size_t& size);

	std::wstring getNick() { return m_nick; }



#if 0
	
	

	static const std::string UnicodeToMultibyte(const unsigned int& code_page, const std::wstring& strWide);
	static const std::wstring MultibyteToUnicode(const unsigned int& code_page, const char* buf, const int& size);

	#endif
	
};

