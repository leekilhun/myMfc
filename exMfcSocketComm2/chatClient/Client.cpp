#include "pch.h"
#include "Client.h"

//#include <ws2tcpip.h>
//#include <chrono>
//#include <atlimage.h>
#include <fstream>
//#include <climits>



using namespace std;

unsigned int threadRecv(LPVOID p, SOCKET& sock);
unsigned int threadSend(LPVOID p, SOCKET& sock, const DATA_TYPE& type, const wstring& name = L"", const wstring& path = L"", const wstring& ext = L"BMP");

Client::Client(CWnd* pParent) :
	client_sock(INVALID_SOCKET),
	addr{},
	m_pSendThread(nullptr),
	m_pRecvThread(nullptr),
	m_pParent(pParent),
	bSend(true),
	bRecv(true)
{
}

Client::~Client()
{
	disconnectServer();
}

void Client::disconnectServer()
{
	bRecv = bSend = false;
	// close client socket
	if (client_sock != INVALID_SOCKET)
	{
		closesocket(client_sock);
		client_sock = INVALID_SOCKET;
	}

	// stop recv thread
	if (m_pRecvThread)
	{
		if (m_pRecvThread->joinable())
			m_pRecvThread->join();

		delete m_pRecvThread;
		m_pRecvThread = nullptr;
	}

	// stop send thread
	if (m_pSendThread)
	{
		if (m_pSendThread->joinable())
			m_pSendThread->join();

		delete m_pSendThread;
		m_pSendThread = nullptr;
	}
}


std::wstring Client::getMyip()
{
	char host[MAX_PATH] = {0,};
	int result = gethostname(host, sizeof(host));

	if (result == SOCKET_ERROR)
		return wstring();

	// ip V4 만 가능
	//PHOSTENT pHostInfo;
	//pHostInfo = gethostbyname(host);
	//if (pHostInfo == nullptr)
	//	return string();

	//if (pHostInfo->h_addrtype == AF_INET) //ip V4
	//{
	//	in_addr addr;
	//	memset(&addr, 0, sizeof(addr));
	//	int i = 0;

	//	while (pHostInfo->h_addr_list[i] != 0)
	//	{
	//		addr.s_addr = *(u_long *)pHostInfo->h_addr_list[i++];
	//		inet_ntoa(addr);			
	//	}

	//}

	//// ip V6 대응
	char port_str[16] = {0,};
	sprintf_s(port_str, "%d", 7000);
	addrinfo hint, * pResult;
	memset(&hint, 0, sizeof(hint));

	//result = getaddrinfo(host, port_str, &hint, &pResult);
	result = getaddrinfo(host, "7000", &hint, &pResult);

	if (result != 0)
		return wstring();

	addrinfo* ptr = nullptr;
	sockaddr_in* pIpv4;
	sockaddr_in6* pIpv6;

	wchar_t ip_str[46];
	DWORD ip_size = sizeof(ip_str);
	memset(&ip_str, 0, ip_size);

	wstring ip;

	for (ptr = pResult; ptr != NULL; ptr = ptr->ai_next)
	{
		switch (ptr->ai_family)
		{
		case AF_UNSPEC:
			break;
		case AF_INET:
			printf("AF_INET (IPv4)\n");
			pIpv4 = (struct sockaddr_in*)ptr->ai_addr;
			//inet_ntoa(pIpv4->sin_addr);
			InetNtop(AF_INET, &pIpv4->sin_addr, ip_str, ip_size %46);
			ip = ip_str;
			break;
		case AF_INET6:
			// the InetNtop function is available on Windows Vista and later
			pIpv6 = (struct sockaddr_in6*)ptr->ai_addr;
			InetNtop(AF_INET6, &pIpv6->sin6_addr, ip_str, ip_size%46);
			break;
		case AF_NETBIOS:
			break;
		default:
			break;
		}
	}

	return ip;
}



bool Client::connectServer(wstring ip, int port)
{
	if (client_sock == INVALID_SOCKET)
	{
		client_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (client_sock == INVALID_SOCKET)
			return false;

		addr.sin_family = AF_INET;
		//addr.sin_addr.s_addr = htonl(INADDR_ANY);
		InetPton(AF_INET, (PWSTR)ip.c_str(), &(addr.sin_addr));
		addr.sin_port = htons(port);

		int result = connect(client_sock, (SOCKADDR*)&addr, sizeof(addr));

		if (result != SOCKET_ERROR)
		{
			bRecv = true;
			if (m_pRecvThread == nullptr)
				m_pRecvThread = new thread(&threadRecv, this, std::ref(client_sock));
		}
		else
		{
			int err = WSAGetLastError();
			return false;
		}
	}
	return true;
}



bool Client::sendSocket(const char* buf, const size_t& size)
{
	if (client_sock != INVALID_SOCKET)
	{
		if (size <= INT_MAX)
		{
			int send_size = 0;
			do
			{
				send_size += send(client_sock, &buf[send_size], (int)size, 0);
			} while (send_size < size);
		}
		else
		{
			size_t total_size = size;
			size_t pos = 0;
			int send_size = 0;
			int sent_size = 0;
			do
			{
				if (total_size > INT_MAX)
					send_size = INT_MAX;
				else
					send_size = (int)total_size;

				sent_size = send(client_sock, &buf[pos], send_size, 0);

				if (send_size == SOCKET_ERROR)
				{
					int e = WSAGetLastError();
					break;
				}

				total_size -= sent_size;
				pos += sent_size;

			} while (total_size > 0);
		}
	}
	else
		return false;

	return true;
}

bool Client::sendText(const std::wstring& msg, const DATA_TYPE& type)
{
	wstring send_msg = L"[" + m_nick + L"] " + msg;

	// utf-8 -> char
	string str;
	trans::TcharToLPSTR((LPTSTR)send_msg.c_str(), (LPSTR)str.c_str(), (int)str.capacity());
	//str.assign(send_msg.begin(), send_msg.end());
	//= Client::UnicodeToMultibyte(CP_UTF8, send_msg);

	size_t txt_size = str.length();
	size_t packet_size = 1 + sizeof(size_t) + txt_size;

	char* buf = new char[packet_size];
	memset(buf, 0, packet_size);
	buf[0] = static_cast<byte>(type);
	memcpy(&buf[1], &txt_size, sizeof(size_t));
	memcpy(&buf[1 + sizeof(size_t)], &str[0], txt_size);

	bool result = sendSocket(buf, packet_size);

	delete[] buf;
	return result;
}

bool Client::sendNick(const std::wstring& msg, const DATA_TYPE& type)
{
	m_nick = msg;
	// utf-8 -> char
	string str;
	trans::TcharToLPSTR((LPTSTR)msg.c_str(), (LPSTR)str.c_str(), (int)str.capacity());
	//str.assign(msg.begin(), msg.end());
	//Client::UnicodeToMultibyte(CP_UTF8, msg);

	size_t txt_size = str.length();
	size_t packet_size = 1 + sizeof(size_t) + txt_size;

	char* buf = new char[packet_size];
	memset(buf, 0, packet_size);
	buf[0] = static_cast<byte>(type);
	memcpy(&buf[1], &txt_size, sizeof(size_t));
	memcpy(&buf[1 + sizeof(size_t)], &str[0], txt_size);

	bool result = sendSocket(buf, packet_size);
	delete[] buf;
	return result;
}

bool Client::sendFile(const std::wstring& file_name, const std::wstring& file_path, const std::wstring& file_ext, const DATA_TYPE& type)
{
	if (m_pSendThread == nullptr)
	{
		bSend = true;
		m_pSendThread = new thread(&threadSend, this, std::ref(client_sock), type, file_name, file_path, file_ext);

		if (m_pSendThread->joinable())
		{
			m_pSendThread->join();
			delete m_pSendThread;
			m_pSendThread = nullptr;
		}
	}
	else
		return false;

	return true;
}


void Client::recvFinished(const DATA_TYPE& type, const char* buf, const size_t& recv_size, const size_t& data_size)
{
	if (m_pParent)
	{
		switch (type)
		{
		case DATA_TYPE::_NICK:
			break;
		case DATA_TYPE::_TEXT:
			m_pParent->SendMessage(UM_RECV_TEXT, (WPARAM)&buf[1 + sizeof(size_t)], data_size);
			break;
		case DATA_TYPE::_IMAGE_NAME:
			m_pParent->SendMessage(UM_RECV_IMAGE_NAME, (WPARAM)&buf[1 + sizeof(size_t)], data_size);
			break;
		case DATA_TYPE::_IMAGE:
			m_pParent->SendMessage(UM_RECV_IMAGE, (WPARAM)&buf[1 + sizeof(size_t)], data_size);
			break;
		case DATA_TYPE::_FILE:
			m_pParent->SendMessage(UM_RECV_FILE, (WPARAM)&buf[1 + sizeof(size_t)], data_size);
			break;
		case DATA_TYPE::_FILE_NAME:
			m_pParent->SendMessage(UM_RECV_FILE_NAME, (WPARAM)&buf[1 + sizeof(size_t)], data_size);
			break;
		}
	}
}


char* Client::ImageToBytes(const std::wstring& file_path, const std::wstring& file_ext, size_t& size)
{
	CImage img;
	img.Load(file_path.c_str());

	if (img.IsNull())
	{
		size = 0;
		return nullptr;
	}

	IStream* pStream = nullptr;
	if (::CreateStreamOnHGlobal(nullptr, true, &pStream) == S_OK)
	{
		if (file_ext == L"BMP")
			img.Save(pStream, Gdiplus::ImageFormatBMP);
		else if (file_ext == L"JPG")
			img.Save(pStream, Gdiplus::ImageFormatJPEG);
		else if (file_ext == L"GIF")
			img.Save(pStream, Gdiplus::ImageFormatGIF);
		else if (file_ext == L"PNG")
			img.Save(pStream, Gdiplus::ImageFormatPNG);

		HGLOBAL hg = nullptr;
		HRESULT ret = ::GetHGlobalFromStream(pStream, &hg);
		char* pBuf = static_cast<char*>(::GlobalLock(hg));
		size_t img_size = 0;
		if (pBuf)
			img_size = ::GlobalSize(pBuf);
		size_t packet_size = img_size + 1 + sizeof(size_t);
		size = packet_size;

		char* pImg = new char[packet_size];
		memset(pImg, 0, packet_size);
		pImg[0] = static_cast<byte>(DATA_TYPE::_IMAGE);
		memcpy(&pImg[1], &img_size, sizeof(size_t));
		memcpy(&pImg[1 + sizeof(size_t)], pBuf, img_size);

		::GlobalUnlock(hg);
		pStream->Release();
		::GlobalFree(hg);

		return pImg;
	}
	else
	{
		size = 0;
		return nullptr;
	}

	return nullptr;


	/*unsigned int img_w = img.GetWidth();
	unsigned int img_h = img.GetHeight();
	unsigned int img_bpp = img.GetBPP();

	unsigned int img_wbyte = img_w * (img_bpp / 8);
	unsigned int img_size = img_h * img_wbyte;

	unsigned int packet_size = 1 + 4 + img_size;
	char* pImg = new char[packet_size];
	memset(pImg, 0, packet_size);

	pImg[0] = _IMAGE;
	memcpy(&pImg[1], &img_size, sizeof(unsigned int));

	for (unsigned int y = 0; y < img_h; y++)
	{
		char* p = reinterpret_cast<char*>(img.GetPixelAddress(0, y));
		memcpy(&pImg[1+4+y * img_wbyte], p, img_wbyte);
	}*/

	/*CImage img2;
	img2.Create(img_w, img_h, img_bpp, 0);
	int i = 0;
	for (unsigned int y = 0; y < img_h; y++)
	{
		for (unsigned int x = 0; x < img_w; x++)
		{
			unsigned char r = pImg[i++];
			unsigned char g = pImg[i++];
			unsigned char b = pImg[i++];
			img2.SetPixel(x, y, RGB(r, g, b));
		}
	}

	img2.Save(_T("test.png"), Gdiplus::ImageFormatPNG);*/

	//size = img_size+1+4;
	//img.Destroy();

	//return pImg;
}

char* Client::FileToBytes(const std::wstring& file_path, const std::wstring& file_ext, size_t& size)
{
	ifstream file(file_path, ios::binary | ios::in);

	if (file.is_open())
	{
		// get file size		
		file.seekg(0, ios::end);
		size_t file_size = file.tellg();

		file.clear();
		file.seekg(0, ios::beg);

		size_t packet_size = file_size + 1 + sizeof(size_t);
		size = packet_size;

		char* pBuf = new char[packet_size];
		memset(pBuf, 0, size);

		pBuf[0] = static_cast<byte>(DATA_TYPE::_FILE);
		memcpy(&pBuf[1], &file_size, sizeof(size_t));

		file.read(&pBuf[1 + sizeof(size_t)], size);
		file.close();
		return pBuf;
	}
	else
		return nullptr;

	return nullptr;
}



#if 0

const string Client::UnicodeToMultibyte(const unsigned int& code_page, const std::wstring& strWide)
{
	if (strWide.empty())
		return string().c_str();

	int size = WideCharToMultiByte(code_page, 0, strWide.c_str(), (int)strWide.size(), NULL, 0, NULL, NULL);
	string str(size, 0);
	size = WideCharToMultiByte(code_page, 0, strWide.c_str(), (int)strWide.size(), &str[0], size, NULL, NULL);
	return str;
}

const std::wstring Client::MultibyteToUnicode(const unsigned int& code_page, const char* buf, const int& size)
{
	int str_size = size;
	if (str_size <= 0)
		return wstring().c_str();

	str_size = MultiByteToWideChar(code_page, 0, buf, size, NULL, 0);
	wstring str(str_size, 0);
	str_size = MultiByteToWideChar(code_page, 0, buf, size, &str[0], size);
	return str.c_str();
}


#endif

unsigned int threadSend(LPVOID p, SOCKET& sock, const DATA_TYPE& type, const wstring& name, const wstring& path, const wstring& ext)
{
	Client* pC = reinterpret_cast<Client*>(p);
	if (pC == nullptr)
		return 0;

	if (sock != INVALID_SOCKET)
	{
		switch (type)
		{
		case DATA_TYPE::_NICK:
			break;
		case DATA_TYPE::_TEXT:
			break;
		case DATA_TYPE::_IMAGE:
		{
			pC->sendText(name, DATA_TYPE::_IMAGE_NAME);
			std::this_thread::sleep_for(100ms);
			size_t packet_size = 0;
			char* pImg = pC->ImageToBytes(path, ext, packet_size);
			pC->sendSocket(pImg, packet_size);
			delete[] pImg;
		}
		break;
		case DATA_TYPE::_FILE:
		{
			pC->sendText(name, DATA_TYPE::_FILE_NAME);
			std::this_thread::sleep_for(100ms);
			size_t packet_size = 0;
			char* pFile = pC->FileToBytes(path, ext, packet_size);
			pC->sendSocket(pFile, packet_size);
			delete[] pFile;
		}
		break;
		}
	}

	return 0;
}

unsigned int threadRecv(LPVOID p, SOCKET& sock)
{
	Client* pC = reinterpret_cast<Client*>(p);
	if (pC == nullptr)
		return 0;

	char buf[MAX_BUF];
	char* pRecvBuf = nullptr;


	while (pC->bRecv)
	{
		memset(buf, 0, sizeof(buf));
		size_t total_size = 0;
		int recv_size = 0;
		size_t data_size = 0;
		size_t pos = 0;
		DATA_TYPE type = DATA_TYPE::_UNKNOWN;
		bool bDisconnect = false;

		do
		{
			recv_size = recv(sock, buf, MAX_BUF, 0);

			if (recv_size < 0)
			{
				bDisconnect = true;
				break;
			}
			else
			{
				total_size += recv_size;
				if (type == DATA_TYPE::_UNKNOWN && recv_size > 1 + sizeof(size_t))
				{
					type = static_cast<DATA_TYPE>(buf[0]);
					memcpy(&data_size, &buf[1], sizeof(size_t));

					if (pRecvBuf == nullptr)
					{
						size_t packet_size = 1 + sizeof(size_t) + data_size;
						pRecvBuf = new char[packet_size];
						memset(pRecvBuf, 0, packet_size);
					}
				}
                if (pRecvBuf) {
                    if (&pRecvBuf[pos])
                        memcpy(&pRecvBuf[pos], buf, recv_size);
                    pos += recv_size;
                }
			}
		} while (total_size < data_size + 1 + sizeof(size_t));

		if (bDisconnect)
		{
			if (pC->m_pParent && pC->m_pParent->m_hWnd != nullptr)
				pC->m_pParent->PostMessage(UM_DISCONNECT_SERVER, 0, 0);
			break;
		}

		if (total_size == data_size + 1 + sizeof(size_t))
		{
			pC->recvFinished(type, pRecvBuf, recv_size, data_size);
			if (pRecvBuf)
			{
				delete[] pRecvBuf;
				pRecvBuf = nullptr;
			}
		}
	}
	if (pRecvBuf)
	{
		delete[] pRecvBuf;
		pRecvBuf = nullptr;
	}
	closesocket(sock);
	sock = INVALID_SOCKET;
	return 0;
}
