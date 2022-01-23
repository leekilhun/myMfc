#pragma once
#include "IIO.h"
#include "jL_CommSerial.h"

class jL_fastechIO :   public IIO, public jL_CommSerial
{
	class _Register
	{
	public:
		class _io
		{
			inline uint32_t  power2(int exponent) {
				uint32_t ret = 1;
				for (int i = 0; i < exponent; i++)
				{
					ret *= 2;
				}
				return ret;
			}
		private:
			bool* status;
			uint8_t value;
		public:
			//uint32_t maxReg;
			_io(/*uint32_t max = 32*/) : status(nullptr), value(0) {
				Clear();
				status = new bool[8]{};
				//memset(status, 0x00, sizeof(status));
			}

			virtual ~_io() {
				if (status != nullptr)
					delete[] status;

				Clear();
			};

			// 초기화.
			inline void Clear() {
				status = nullptr;
				value = 0;
			}

			inline int SetRegister(uint8_t reg_value) {
				status[0] = (reg_value & 1) >> 0;
				status[1] = (reg_value & 2) >> 1;
				status[2] = (reg_value & 4) >> 2;
				status[3] = (reg_value & 8) >> 3;
				status[4] = (reg_value & 16) >> 4;
				status[5] = (reg_value & 32) >> 5;
				status[6] = (reg_value & 64) >> 6;
				status[7] = (reg_value & 128) >> 7;
				value = reg_value;
				/*for (int i = 0; i < 32; i++)
				{
					status[i] = (reg_value & power2(i)) >> i;
				}*/
				return 0;
			}

			inline uint8_t GetValue() {
				value = 0;
				for (int i = 0; i < 8; i++) {
					value |= status[i] << i;
				}
				return value;
			}

			inline bool& operator[](const uint8_t index) {
				return status[index % 8];
			}
		};




		_io* inState;
		_io* outState;
		uint32_t maxBankIn;  // 1 bank에 32비트 register
		uint32_t maxBankOut;
		_Register(uint32_t max_in_count, uint32_t max_out_count)
			: maxBankIn(max_in_count), maxBankOut(max_out_count) {
			Clear();

			inState = new _io[max_in_count];
			outState = new _io[max_out_count];
		}
		virtual ~_Register() {
			if (inState != nullptr)
				delete[] inState;
			if (outState != nullptr)
				delete[] outState;
			Clear();
		};

		// 초기화.
		inline void Clear() {
			inState = outState = nullptr;
		}

		inline int SetRegisterIn(uint8_t value, uint32_t bank_addr) {
			inState[bank_addr % maxBankIn].SetRegister(value);
			return 0;
		}

		inline int SetRegisterOut(uint8_t value, uint32_t bank_addr) {
			outState[bank_addr % maxBankOut].SetRegister(value);
			return 0;
		}

		//inline uint32_t SetRegisterOut(uint32_t reg_value) {
		//	if (reg_value > maxRegOut)
		//		reg_value = maxRegOut;
		//	m_output = reg_value;
		//	return m_output;
		//}
		//



	};
	/****************************************************
	 *	data
	 ****************************************************/
	enum class cmd_state
	{
		wait_stx,
		wait_id,
		wait_type,
		wait_data,
		wait_checksum,
		wait_etx,
	};

	cmd_state m_CmdState;
	uint16_t  m_Checksum;

	enum class cmd_type
	{
		io_bd,
	};

#define FASTECH_CMD_MAX_DATA_LENGTH  248
	typedef struct
	{
		uint8_t	  id;
		uint8_t   type;
		uint16_t  length;
		uint32_t  check_sum;
		uint16_t  check_sum_recv;
		uint8_t   buffer[FASTECH_CMD_MAX_DATA_LENGTH + 8];
		uint8_t* data;
	} fastech_packet_t;

	typedef struct
	{
		uint8_t   ch;
		bool      is_init;
		uint32_t  baud;
		uint8_t   state;
		uint32_t  pre_time;
		uint16_t  packet_size;
		uint32_t  index;
		uint8_t   error;
		fastech_packet_t  rx_packet;
	} fastech_t;



	fastech_t m_FastechComm;

public:
	bool m_IsWiatResponse;
	bool	m_bThreadLife;
	DWORD	m_ThreadId;
	HANDLE m_hThread;
	_Register* m_pRegisterIO;

	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	jL_fastechIO(CommonAttribute commonData,CString port_name, DWORD baudrate);
	virtual ~jL_fastechIO();

	/****************************************************
	 *	func
	 ****************************************************/
private:
	static UINT ioThread(LPVOID pParam);


public:
	int ParsingPacket();
	void UpdateReg();

	// io Manager에서 요청
	// 통신포트 수신 감시를 시작
	DWORD ThreadRun();
	int Terminate();


	////// 아래 함수는 상위 ioManager에서 구현
	Table_IO* GetIOTable();
	int LoadIOMap(CString file_name = L"IO.Dat");
	int Initialize();
	bool IsOn(uint16_t addr);
	bool IsOff(uint16_t addr);
	int OutputOn(uint16_t addr);
	int OutputOff(uint16_t addr);
	int OutputToggle(uint16_t addr);
	//////  <--



		/// <summary>
	/// 수신 콜백 함수 등록, 데이터가 수신되면 ParsingPacket이 실행
	/// </summary>
	/// <param name="obj"></param>
	/// <returns></returns>
	static int WarpFunc(void* obj) {
		jL_fastechIO* self = (jL_fastechIO*)(obj);
		return self->ParsingPacket();
	}


};

