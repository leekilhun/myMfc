#pragma once
#include "nextionCommSerial.h"
#include "qBuffer.h"

#include "jL_logManager.h"

#define CMD_MAX_DATA_LENGTH			24
#define DEF_NEX_PAGE_INIT			  0
#define DEF_NEX_PAGE_MAIN				1
#define DEF_NEX_PAGE_MANUAL			2
#define DEF_NEX_PAGE_TEACH			3
#define DEF_NEX_PAGE_IO					4
#define DEF_NEX_PAGE_DATA				5
#define DEF_NEX_PAGE_LOG				6
class nextionCommLcd
{
	enum class cmd_state
	{
		wait_stx,
		wait_page_no,
		wait_type,
		wait_obj_id,
		wiat_length_l,
		wait_length_h,
		wait_data,
		wait_checksum,
		wait_etx,
	};


	cmd_state m_CmdState;
	uint16_t m_Checksum;


	typedef enum
	{
		NEX_PAGE_INIT,
		NEX_PAGE_MAIN,
		NEX_PAGE_MANUAL,
		NEX_PAGE_TEACH,
		NEX_PAGE_IO,
		NEX_PAGE_DATA,
		NEX_PAGE_LOG,
	}nextionpage_t;

	typedef struct
	{
		nextionpage_t   page_no;
		uint8_t         type;
		uint8_t         obj_id;
		uint16_t        length;
		uint32_t        check_sum;
		uint16_t        check_sum_recv;
		uint8_t         buffer[CMD_MAX_DATA_LENGTH + 8];
		uint8_t* data;
	} nextion_packet_t;

	typedef struct
	{
		uint8_t   ch;
		bool      is_init;
		uint32_t  baud;
		uint8_t   state;
		uint32_t  pre_time;
		uint32_t  index;
		uint8_t   error;
		nextion_packet_t  rx_packet;
	} nextion_t;


public:

	struct arm_state {
		uint16_t input_reg;
		uint16_t output_reg;
	};

	jL_logManager* m_log;


	arm_state m_armStatus;

	nextion_t m_NextionComm;

	nextionCommSerial* m_ComPort;
	bool m_IsPortOpen;

	TQueue m_Qbuf; //  Buffer

	// 스레드의 핸들을 보관
	HANDLE m_hThread;

	DWORD m_idThread;

	bool m_lifeThread;

	string m_lastLog[2];


	// Main gui 윈도우 핸들
	HWND m_hCommWnd;

	uint16_t m_logCursor;

public:
	nextionCommLcd();

	virtual ~nextionCommLcd();

private:
	uint16_t getOutReg();
	uint16_t getInReg();
	static DWORD threadFunction(LPVOID pParam);
	void threadJob();
	void sendIoReg();
	void updateOutReg();
	void sendLog();

	uint32_t crc_update(uint32_t crc_in, int incr);
	void crc_update(uint32_t* crc_in, uint8_t data);
	uint16_t crc16(uint8_t data);

public:
	int ReceiveData();
	int OpenComPort(CString portname, DWORD baudrate = CBR_9600, WORD port_no = 0);
	int CloseComPort();

	void ThreadRun();
	void ThreadStop();

		int GetLCDPageNo();
	void SetOutputReg(uint16_t value);
	void ChangeLCDPage(int page_no);

	static int WrapFunc(void* obj) {
		nextionCommLcd* self = (nextionCommLcd*)(obj);
		return self->ReceiveData();
	}
};



