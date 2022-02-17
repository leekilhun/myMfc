#include "pch.h"

#include "jL_ethernet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_ETHERNET_BUFFER_SIZE		4096
#define MAX_CLIENT			5

#define END_CHARACTER		'^'
#define DELIMITER_CHARACTER	'^'


jL_ethernet::jL_ethernet(jL_ethernet::cfg_t cfg) :m_CfgData(cfg)
, m_Socket(NULL), m_Event(NULL), m_SocketCnt(0)
{
    m_IsLiveThread = false;
    m_IsConnected = false;
    m_TrdId = NULL;

    // 포트를 감시할 함수, 스레드의 핸들을 보관
    m_WatchComm = NULL;
}

jL_ethernet::~jL_ethernet()
{
    CloseSockets();
}

errno_t jL_ethernet::iniComm()
{
    return errno_t();
}

int jL_ethernet::initServerSocket()
{

    u_short port_no = 23;

    WSADATA wSaData;
    SOCKET hServSock;
    SOCKADDR_IN servAddr;

    err_t  ret = jL_ethernet::err_t::ERR_SUCCESS;

    if (WSAStartup(MAKEWORD(2, 2), &wSaData) != 0) {
        ret = jL_ethernet::err_t::ERR_WSA_START_UP;
        goto ret_out;
    }

    hServSock = socket(PF_INET, SOCK_STREAM, 0);
    if (hServSock == INVALID_SOCKET) {
        ret = jL_ethernet::err_t::ERR_SOCKET;
        goto ret_out;
    }

    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(u_short(port_no));

    if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR) {
        ret = jL_ethernet::err_t::ERR_BIND;
        goto ret_out;
    }

    WSAEVENT wsaEvent;
    wsaEvent = WSACreateEvent();
    if (WSAEventSelect(hServSock, wsaEvent, FD_ACCEPT) == SOCKET_ERROR) {
        ret = jL_ethernet::err_t::ERR_WSAEVENT_SELECT;
        goto ret_out;
    }

    if (listen(hServSock, MAX_CLIENT) == SOCKET_ERROR) {
        ret = jL_ethernet::err_t::ERR_LISTEN;
        goto ret_out;
    }

    //m_iSockNumber = 0;
    m_Socket = hServSock;
    m_Event = wsaEvent;
    //m_iSockNumber++;

    ThreadStart();
ret_out:
    return static_cast<int>(ret);
}

void jL_ethernet::CloseSockets()
{

    if (m_Socket) {
        closesocket(m_Socket);
        m_Socket = NULL;
    }

    WSACleanup();
    m_IsConnected = false;

}


void jL_ethernet::ThreadStart()
{
    // 포트 감시 스레드 생성
    m_WatchComm = CreateThread(
        (LPSECURITY_ATTRIBUTES)NULL,
        0,
        (LPTHREAD_START_ROUTINE)CommWatchTrd,
        (LPVOID)this,
        0,
        &m_TrdId
    );

    if (!m_WatchComm)
    {
        // 실패하면 포트를 닫는다.
        CloseSockets();
        TRACE(L"Error create thread for watch comm! ");
    }
    m_IsLiveThread = true;
}

void jL_ethernet::StopThread()
{
}

void jL_ethernet::Open()
{
    initServerSocket();
}


/****************************************************
 *	thread
 ****************************************************/
UINT jL_ethernet::CommWatchTrd(LPVOID pParam)
{
    jL_ethernet* pThis = (jL_ethernet*)pParam;

    SOCKADDR_IN clntAddr;
    int clntAddrSize;

    WSAEVENT wsaEvent, dummyEvent;
    WSANETWORKEVENTS netEvents;
    SOCKET hClntSock;

    int index, nResult, i;
    char message[MAX_ETHERNET_BUFFER_SIZE];
    CString strBuf;
    BOOL	bReceiveAll = FALSE;
    err_t  ret = jL_ethernet::err_t::ERR_SUCCESS;

    unsigned long flags = 0;
    WSAOVERLAPPED overlapped;

    dummyEvent = WSACreateEvent();

    while (pThis->m_IsLiveThread)
    {
        nResult = WSAWaitForMultipleEvents(0, &pThis->m_Event, FALSE, 900, FALSE);
        if (nResult == WSA_WAIT_TIMEOUT) continue;
        if (nResult == WSA_WAIT_FAILED) break;

        index = nResult - WSA_WAIT_EVENT_0;


        nResult = WSAWaitForMultipleEvents(1, &(pThis->m_Event), TRUE, 0, FALSE);
        if (nResult == WSA_WAIT_TIMEOUT || nResult == WSA_WAIT_FAILED) continue;

        WSAEnumNetworkEvents(pThis->m_Socket, pThis->m_Event, &netEvents);

        if (netEvents.lNetworkEvents & FD_ACCEPT) // 연결 요청의 경우
        {
            if (netEvents.iErrorCode[FD_ACCEPT_BIT] != 0)
            {
                ret = jL_ethernet::err_t::ERR_ACCEPT;
                break;
            }

            clntAddrSize = sizeof(clntAddr);
            hClntSock = accept(pThis->m_Socket, (SOCKADDR*)&clntAddr, &clntAddrSize);
            if (hClntSock == INVALID_SOCKET)
            {
                ret = jL_ethernet::err_t::ERR_ACCEPT;
                break;
            }
            wsaEvent = WSACreateEvent();
            WSAEventSelect(hClntSock, wsaEvent, FD_READ | FD_CLOSE);

            pThis->m_Socket = hClntSock;
            pThis->m_Event = wsaEvent;
            pThis->m_IsConnected= true;

            TRACE("소켓 연결 :%d \n", hClntSock);

            /*
            sprintf_s(message, sizeof(message), "%s", pThis->m_strMateIP[i + 1].GetBuffer());

            if (pThis->m_hNotifyHandle != NULL)
                //::PostMessage(pMain->m_hNotifyHandle, pMain->m_uiConnectMessage, i+1, (LPARAM)(message));
                ::PostMessage(pThis->m_hNotifyHandle, pMain->m_uiConnectMessage, i + 1, (LPARAM)(pMain));

            if (pThis->m_iSockNumber > pMain->m_iMaxClient) // Client갯수를 초과하면 Accept를 허용하지 않는다.
            {
                WSAEventSelect(pMain->m_SockArray[0], pMain->m_EventArray[0], 0);
            }
            */

        }

        if (netEvents.lNetworkEvents & FD_READ) // 데이터 전송의 경우
        {
            if (netEvents.iErrorCode[FD_READ_BIT] != 0)
            {
                ret = jL_ethernet::err_t::ERR_READ;
                break;
            }

            bReceiveAll = FALSE;
            flags = 0;
            memset(message, 0, sizeof(message));
            memset(&overlapped, 0, sizeof(overlapped));
            pThis->m_dataBuf.len = MAX_ETHERNET_BUFFER_SIZE - 1;
            pThis->m_dataBuf.buf = message;

            if (WSARecv(pThis->m_Socket, &pMain->m_dataBuf, 1, &g_irecvBytes, &flags, &overlapped, CompletionRoutine) == SOCKET_ERROR)
            {
                ret = jL_ethernet::err_t::ERR_READ;
                break;
            }

            strcpy(pMain->test, pMain->m_dataBuf.buf);

            if (pMain->m_hNotifyHandle != NULL)
            {
                TRACE(_T("%s\n"), pMain->test);

                //::PostMessage(pMain->m_hNotifyHandle, pMain->m_uiReceiveMessage, i+1, (LPARAM)(message));
                ::SendMessage(pMain->m_hNotifyHandle, pMain->m_uiReceiveMessage, i, (LPARAM)(pMain));  // client Port, LPARAM
            }

            break;
        }

        if (netEvents.lNetworkEvents & FD_CLOSE)
        {
            //				if(netEvents.iErrorCode[FD_CLOSE_BIT] != 0)
            //				{
            //					nResult = ERR_CLOSE;
            //					//break;
            //				}

            WSACloseEvent(pThis->m_Event);
            closesocket(pThis->m_Socket);
            TRACE("Socket 종료 :%d \n", pThis->m_Socket);

            pThis->m_IsConnected = FALSE;
            /*
            //pMain->m_iSockNumber--;
            for (int j = i; j < pMain->m_iSockNumber; j++)
            {
                pMain->m_SockArray[j] = pMain->m_SockArray[j + 1];
                pMain->m_EventArray[j] = pMain->m_EventArray[j + 1];
                pMain->m_bConnected[j] = pMain->m_bConnected[j + 1];
                pMain->m_bReceived[i] = pMain->m_bReceived[j + 1];

                pMain->m_strQue[j].Reset();
                for (;;)
                {
                    if (pMain->m_strQue[j + 1].IsEmpty()) break;
                    strBuf = pMain->m_strQue[j + 1].Pop();
                    pMain->m_strQue[j].Push(strBuf);
                }
            }

            sprintf_s(message, sizeof(message), "%s", pMain->m_strMateIP[i].GetBuffer());
            if (pMain->m_hNotifyHandle != NULL)
                ::PostMessage(pMain->m_hNotifyHandle, pMain->m_uiCloseMessage, i, (LPARAM)(pMain));

            if (pMain->m_iSockNumber == pMain->m_iMaxClient) // Client갯수를 넘지 않으면 Accept를 허용한다.
            {
                WSAEventSelect(pMain->m_SockArray[0], pMain->m_EventArray[0], FD_ACCEPT);
            }
            */
        }

        Sleep(1);
    }

    pThis->m_IsLiveThread = FALSE;
    pThis->CloseSockets();

    return static_cast<int>(ret);
}
