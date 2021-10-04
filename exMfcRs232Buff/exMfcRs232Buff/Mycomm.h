#pragma once

#define MAXBUF              4096*2
#define InBufSize           4096*2
#define OutBufSize          4096*2
#define ASCII_XON           0x11
#define ASCII_XOFF          0x13
#define WM_MYRECEIVE        (WM_USER +1)
#define WM_MYCLOSE          (WM_USER +2)
#define RS232_BAUD_LIST_MAX	12


class CMycomm:public CCmdTarget
{
public:
    struct sRs232_baudrate_tbl { DWORD baudrate_data; LPCTSTR label; }
    RS_232_BAUDRATE_DATA[RS232_BAUD_LIST_MAX] =
    {
        {CBR_110,		_T("110")},
        {CBR_300,		_T("300")},
        {CBR_600,		_T("600")},
        {CBR_1200,		_T("1200")},
        {CBR_2400,		_T("2400")},
        {CBR_4800,		_T("4800")},
        {CBR_9600,		_T("9600")},
        {CBR_14400,		_T("14400")},
        {CBR_19200,		_T("19200")},
        {CBR_38400,		_T("38400")},
        {CBR_56000,		_T("56000")},
        {CBR_115200,	_T("115200")},

    };

public:
    HANDLE  m_hComDev;
    HWND    m_hWnd;

    BOOL    m_bIsOpenned;
    CString m_sComPort;
    CString m_sBaudRate;
    CString m_sParity;
    CString m_sDataBit;
    CString m_sStopBit;

    DWORD   m_BaudRate;
    BYTE    m_Parity;
    BYTE   m_ByteSize;
    BYTE   m_StopBit;
    BOOL    m_bFlowChk;

    OVERLAPPED m_OLR, m_OLW;
    char    m_sInBuf[MAXBUF * 2];
    int     m_nLength;
    CEvent* m_pEvent;

public:
    CMycomm() { Init(); }
    CMycomm(HWND  hWnd) {
        Init(); 

        m_pEvent = new CEvent(FALSE, TRUE);
        m_hWnd = hWnd;
    }
    CMycomm(CString port, CString baudrate, CString parity, CString databit, CString stopbit);


    ~CMycomm() {
        if (m_bIsOpenned)
        {
            Close();
        }
        delete m_pEvent;
        m_pEvent = nullptr;

        Init();
        

       // _CrtDumpMemoryLeaks();
    }

    void Init() {
        m_hComDev = nullptr;
        m_hWnd = nullptr;
        m_pEvent = nullptr;
        m_sComPort.Empty();
        m_sBaudRate.Empty();
        m_sParity.Empty();
        m_sDataBit.Empty();
        m_sStopBit.Empty();
        m_bFlowChk = 1;
        m_bIsOpenned = FALSE;
        m_nLength = 0;
        m_BaudRate  = CBR_115200;
        m_Parity    = NOPARITY;
        m_ByteSize  = 8;
        m_StopBit   = ONESTOPBIT;
        m_OLR = {};
        m_OLW = {};
        memset(m_sInBuf, 0x00, MAXBUF * 2);
    }
   

public:
    void Clear();
    int Receive(LPSTR inbuf, int len);
    BOOL Send(LPCTSTR outbuf, int len);
    // Serial Port¸¦ ¿¬´Ù
    BOOL Open(CString port_name, DWORD baud, HWND hwnd = NULL);
    void HandleClose();
    void Close();
    void ResetSerial();
    void	AddPortList(CComboBox* combobox);
    void	AddBaudrateList(CComboBox* combobox);

  
};

UINT CommThread(LPVOID lpData);