#pragma once
#include "util.h"

#define ETHERNET_IP_STR_MAX      26


class jL_ethernet
{
 /****************************************************
  *	data
  ****************************************************/
public:
    enum class mode_t {
        servo,
        client,
    };

    struct cfg_t
    {
        jL_ethernet::mode_t mode{};
        char ip_str[ETHERNET_IP_STR_MAX]{};
        uint32_t  port_no{};
        HWND hwnd = {};

        inline int SetPortName(const char* value) {
            return sprintf_s(ip_str, ETHERNET_IP_STR_MAX, value);
        }


        inline char* GetPortName() {
            return &ip_str[0];
        }

        inline void GetPortName(TCHAR* p_value, uint32_t len = ETHERNET_IP_STR_MAX) {
            TCHAR tmp[ETHERNET_IP_STR_MAX] = { 0, };
            trans::CharToLPTSTR(ip_str, tmp, ETHERNET_IP_STR_MAX);
            wsprintf(p_value, tmp);
        }

    };

private:
    cfg_t m_CfgData;

    enum class err_t
    {
        ERR_SUCCESS,
        ERR_NOT_SUPPORTED = 1,
        ERR_WSA_START_UP,
        ERR_CREATE_MUTEX,
        ERR_SOCKET,
        ERR_BIND,
        ERR_LISTEN,
        ERR_ACCEPT,
        ERR_WSAEVENT_SELECT,
        ERR_READ,
        ERR_CLOSE,
        ERR_CONNECT,
        ERR_NOT_CONNECTED,
        ERR_SENDED,
        ERR_CLIENT_NO_OVER,
    };

public:

    uint32_t m_SocketCnt;
    SOCKET m_Socket;
    WSAEVENT  m_Event;

    bool m_IsLiveThread;
    bool m_IsConnected;
    DWORD m_TrdId;

    // 포트를 감시할 함수, 스레드의 핸들을 보관
    HANDLE m_WatchComm; 
 /****************************************************
  *	Constructor
  ****************************************************/
public:
	jL_ethernet(jL_ethernet::cfg_t cfg);
	~jL_ethernet();

 /****************************************************
  *	func
  ****************************************************/
private:
    errno_t iniComm();
    /** Socket 초기화*/
    int initServerSocket();
    void CloseSockets();
    static UINT CommWatchTrd(LPVOID pParam);

public:
    void ThreadStart();
    void StopThread();
    void Open();
    
    /*int SendData(CString str, CString strMateIP = "");
    int SendCharacter(char* pChar, int nLength, CString strMateIP = "");
    int ReadData(CString& str, CString strMateIP = "");
    BOOL IsMateConnected(CString strMateIP = "");
    int AddClient(CString strClientIP);
    BOOL IsMateMsgReceived(CString strMateIP = "");*/

};
