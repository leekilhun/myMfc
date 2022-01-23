#pragma once
#include "jL_CommSerial.h"
#include "jL_logManager.h"


#define FASTECH_CMD_MAX_DATA_LENGTH  248


#define FASTECH_AXIS_STATUS_ERR_ALL_Pos							(0U)
#define FASTECH_AXIS_STATUS_ERR_ALL_Msk							(0x1UL << FASTECH_AXIS_STATUS_ERR_ALL_Pos)           /*!< 0x0000 0001 */
#define FASTECH_AXIS_STATUS_ERR_ALL									FASTECH_AXIS_STATUS_ERR_ALL_Msk
#define FASTECH_AXIS_STATUS_HW_PLUS_LIMIT_Pos				(1U)
#define FASTECH_AXIS_STATUS_HW_PLUS_LIMIT_Msk				(0x1UL << FASTECH_AXIS_STATUS_HW_PLUS_LIMIT_Pos)     /*!< 0x0000 0010 */
#define FASTECH_AXIS_STATUS_HW_PLUS_LIMIT						FASTECH_AXIS_STATUS_HW_PLUS_LIMIT_Msk
#define FASTECH_AXIS_STATUS_HW_MINUS_LIMIT_Pos			(2U)
#define FASTECH_AXIS_STATUS_HW_MINUS_LIMIT_Msk			(0x1UL << FASTECH_AXIS_STATUS_HW_MINUS_LIMIT_Pos)    /*!< 0x0000 0100 */
#define FASTECH_AXIS_STATUS_HW_MINUS_LIMIT					FASTECH_AXIS_STATUS_HW_MINUS_LIMIT_Msk
#define FASTECH_AXIS_STATUS_SW_PLUS_LIMIT_Pos				(3U)
#define FASTECH_AXIS_STATUS_SW_PLUS_LIMIT_Msk				(0x1UL << FASTECH_AXIS_STATUS_SW_PLUS_LIMIT_Pos)     /*!< 0x0000 1000 */
#define FASTECH_AXIS_STATUS_SW_PLUS_LIMIT						FASTECH_AXIS_STATUS_SW_PLUS_LIMIT_Msk
#define FASTECH_AXIS_STATUS_SW_MINUS_LIMIT_Pos			(4U)
#define FASTECH_AXIS_STATUS_SW_MINUS_LIMIT_Msk			(0x1UL << FASTECH_AXIS_STATUS_SW_MINUS_LIMIT_Pos)    /*!< 0x0001 0000 */
#define FASTECH_AXIS_STATUS_SW_MINUS_LIMIT					FASTECH_AXIS_STATUS_SW_MINUS_LIMIT_Msk
#define FASTECH_AXIS_STATUS_RESERVED_Pos						(5U)
#define FASTECH_AXIS_STATUS_RESERVED_Msk						(0x1UL << FASTECH_AXIS_STATUS_RESERVED_Pos)				   /*!< 0x0010 0000 */
#define FASTECH_AXIS_STATUS_RESERVED								FASTECH_AXIS_STATUS_RESERVED_Ms k
#define FASTECH_AXIS_STATUS_POS_CNT_OVER_Pos				(6U)
#define FASTECH_AXIS_STATUS_POS_CNT_OVER_Msk				(0x1UL << FASTECH_AXIS_STATUS_POS_CNT_OVER_Pos)		   /*!< 0x0100 0000 */
#define FASTECH_AXIS_STATUS_POS_CNT_OVER						FASTECH_AXIS_STATUS_POS_CNT_OVER_Msk
#define FASTECH_AXIS_STATUS_ERR_SERVO_ALARM_Pos			(7U)
#define FASTECH_AXIS_STATUS_ERR_SERVO_ALARM_Msk			(0x1UL << FASTECH_AXIS_STATUS_ERR_SERVO_ALARM_Pos)	 /*!< 0x1000 0000 */
#define FASTECH_AXIS_STATUS_ERR_SERVO_ALARM					FASTECH_AXIS_STATUS_ERR_SERVO_ALARM_Msk

#define FASTECH_AXIS_STATUS_ERR_OVER_CURRENT_Pos		(0U)
#define FASTECH_AXIS_STATUS_ERR_OVER_CURRENT_Msk		(0x100UL << FASTECH_AXIS_STATUS_ERR_OVER_CURRENT_Pos) /*!< 0x0000 0001 */
#define FASTECH_AXIS_STATUS_ERR_OVER_CURRENT				FASTECH_AXIS_STATUS_ERR_OVER_CURRENT_Msk
#define FASTECH_AXIS_STATUS_ERR_OVER_SPEED_Pos	    (1U)
#define FASTECH_AXIS_STATUS_ERR_OVER_SPEED_Msk	    (0x100UL << FASTECH_AXIS_STATUS_ERR_OVER_SPEED_Pos)   /*!< 0x0000 0010 */
#define FASTECH_AXIS_STATUS_ERR_OVER_SPEED				  FASTECH_AXIS_STATUS_ERR_OVER_SPEED_Msk
#define FASTECH_AXIS_STATUS_ERR_STEP_OUT_Pos	  		(2U)
#define FASTECH_AXIS_STATUS_ERR_STEP_OUT_Msk	  		(0x100UL << FASTECH_AXIS_STATUS_ERR_STEP_OUT_Pos)     /*!< 0x0000 0100 */
#define FASTECH_AXIS_STATUS_ERR_STEP_OUT						FASTECH_AXIS_STATUS_ERR_STEP_OUT_Msk
#define FASTECH_AXIS_STATUS_ERR_OVER_LOAD_Pos	  		(3U)
#define FASTECH_AXIS_STATUS_ERR_OVER_LOAD_Msk	  		(0x100UL << FASTECH_AXIS_STATUS_ERR_OVER_LOAD_Pos)    /*!< 0x0000 1000 */
#define FASTECH_AXIS_STATUS_ERR_OVER_LOAD						FASTECH_AXIS_STATUS_ERR_OVER_LOAD_Msk
#define FASTECH_AXIS_STATUS_ERR_OVER_HEAT_Pos	  		(4U)
#define FASTECH_AXIS_STATUS_ERR_OVER_HEAT_Msk	  		(0x100UL << FASTECH_AXIS_STATUS_ERR_OVER_HEAT_Pos)    /*!< 0x0001 0000 */
#define FASTECH_AXIS_STATUS_ERR_OVER_HEAT						FASTECH_AXIS_STATUS_ERR_OVER_HEAT_Msk
#define FASTECH_AXIS_STATUS_ERR_BACK_EMF_Pos	  		(5U)
#define FASTECH_AXIS_STATUS_ERR_BACK_EMF_Msk	  		(0x100UL << FASTECH_AXIS_STATUS_ERR_BACK_EMF_Pos)     /*!< 0x0010 0000 */
#define FASTECH_AXIS_STATUS_ERR_BACK_EMF						FASTECH_AXIS_STATUS_ERR_BACK_EMF_Msk
#define FASTECH_AXIS_STATUS_ERR_MOT_PWR_Pos	  			(6U)
#define FASTECH_AXIS_STATUS_ERR_MOT_PWR_Msk	  			(0x100UL << FASTECH_AXIS_STATUS_ERR_MOT_PWR_Pos)      /*!< 0x0100 0000 */
#define FASTECH_AXIS_STATUS_ERR_MOT_PWR							FASTECH_AXIS_STATUS_ERR_MOT_PWR_Msk
#define FASTECH_AXIS_STATUS_ERR_INPOSITION_Pos			(7U)
#define FASTECH_AXIS_STATUS_ERR_INPOSITION_Msk			(0x100UL << FASTECH_AXIS_STATUS_ERR_INPOSITION_Pos)   /*!< 0x1000 0000 */
#define FASTECH_AXIS_STATUS_ERR_INPOSITION					FASTECH_AXIS_STATUS_ERR_INPOSITION_Msk


#define FASTECH_AXIS_STATUS_EMG_STOP_Pos	  				(0U)
#define FASTECH_AXIS_STATUS_EMG_STOP_Msk	  				(0x10000UL << FASTECH_AXIS_STATUS_EMG_STOP_Pos)				/*!< 0x0000 0001 */
#define FASTECH_AXIS_STATUS_EMG_STOP								FASTECH_AXIS_STATUS_EMG_STOP_Msk
#define FASTECH_AXIS_STATUS_SLOW_STOP_Pos	  				(1U)
#define FASTECH_AXIS_STATUS_SLOW_STOP_Msk	  				(0x10000UL << FASTECH_AXIS_STATUS_SLOW_STOP_Pos)			/*!< 0x0000 0010 */
#define FASTECH_AXIS_STATUS_SLOW_STOP								FASTECH_AXIS_STATUS_SLOW_STOP_Msk
#define FASTECH_AXIS_STATUS_ORG_RETURNING_Pos	  		(2U)
#define FASTECH_AXIS_STATUS_ORG_RETURNING_Msk	  		(0x10000UL << FASTECH_AXIS_STATUS_ORG_RETURNING_Pos)  /*!< 0x0000 0100 */
#define FASTECH_AXIS_STATUS_ORG_RETURNING						FASTECH_AXIS_STATUS_ORG_RETURNING_Msk
#define FASTECH_AXIS_STATUS_INPOSITION_Pos	  			(3U)
#define FASTECH_AXIS_STATUS_INPOSITION_Msk	  			(0x10000UL << FASTECH_AXIS_STATUS_INPOSITION_Pos)     /*!< 0x0000 1000 */
#define FASTECH_AXIS_STATUS_INPOSITION							FASTECH_AXIS_STATUS_INPOSITION_Msk
#define FASTECH_AXIS_STATUS_SERVO_ON_Pos			  		(4U)
#define FASTECH_AXIS_STATUS_SERVO_ON_Msk			  		(0x10000UL << FASTECH_AXIS_STATUS_SERVO_ON_Pos)				/*!< 0x0001 0000 */
#define FASTECH_AXIS_STATUS_SERVO_ON								FASTECH_AXIS_STATUS_SERVO_ON_Msk
#define FASTECH_AXIS_STATUS_ALARM_RESET_Pos	  			(5U)
#define FASTECH_AXIS_STATUS_ALARM_RESET_Msk	  			(0x10000UL << FASTECH_AXIS_STATUS_ALARM_RESET_Pos)    /*!< 0x0010 0000 */
#define FASTECH_AXIS_STATUS_ALARM_RESET							FASTECH_AXIS_STATUS_ALARM_RESET_Msk
#define FASTECH_AXIS_STATUS_PT_STOPPED_Pos	  			(6U)
#define FASTECH_AXIS_STATUS_PT_STOPPED_Msk	  			(0x10000UL << FASTECH_AXIS_STATUS_PT_STOPPED_Pos)     /*!< 0x0100 0000 */
#define FASTECH_AXIS_STATUS_PT_STOPPED							FASTECH_AXIS_STATUS_PT_STOPPED_Msk
#define FASTECH_AXIS_STATUS_ORG_SENSOR_Pos					(7U)
#define FASTECH_AXIS_STATUS_ORG_SENSOR_Msk					(0x10000UL << FASTECH_AXIS_STATUS_ORG_SENSOR_Pos)			/*!< 0x1000 0000 */
#define FASTECH_AXIS_STATUS_ORG_SENSOR							FASTECH_AXIS_STATUS_ORG_SENSOR_Msk

#define FASTECH_AXIS_STATUS_Z_PLUS_Pos	  					(0U)
#define FASTECH_AXIS_STATUS_Z_PLUS_Msk	  					(0x1000000UL << FASTECH_AXIS_STATUS_Z_PLUS_Pos)				/*!< 0x0000 0001 */
#define FASTECH_AXIS_STATUS_Z_PLUS									FASTECH_AXIS_STATUS_Z_PLUS_Msk
#define FASTECH_AXIS_STATUS_ORG_RET_OK_Pos	  			(1U)
#define FASTECH_AXIS_STATUS_ORG_RET_OK_Msk	  			(0x1000000UL << FASTECH_AXIS_STATUS_ORG_RET_OK_Pos)			/*!< 0x0000 0010 */
#define FASTECH_AXIS_STATUS_ORG_RET_OK							FASTECH_AXIS_STATUS_ORG_RET_OK_Msk
#define FASTECH_AXIS_STATUS_MOTION_DIR_Pos	  			(2U)
#define FASTECH_AXIS_STATUS_MOTION_DIR_Msk	  			(0x1000000UL << FASTECH_AXIS_STATUS_MOTION_DIR_Pos)  /*!< 0x0000 0100 */
#define FASTECH_AXIS_STATUS_MOTION_DIR							FASTECH_AXIS_STATUS_MOTION_DIR_Msk
#define FASTECH_AXIS_STATUS_MOTIONING_Pos	  				(3U)
#define FASTECH_AXIS_STATUS_MOTIONING_Msk	  				(0x1000000UL << FASTECH_AXIS_STATUS_MOTIONING_Pos)     /*!< 0x0000 1000 */
#define FASTECH_AXIS_STATUS_MOTIONING								FASTECH_AXIS_STATUS_MOTIONING_Msk
#define FASTECH_AXIS_STATUS_MOTION_PAUSE_Pos				(4U)
#define FASTECH_AXIS_STATUS_MOTION_PAUSE_Msk				(0x1000000UL << FASTECH_AXIS_STATUS_MOTION_PAUSE_Pos)				/*!< 0x0001 0000 */
#define FASTECH_AXIS_STATUS_MOTION_PAUSE						FASTECH_AXIS_STATUS_MOTION_PAUSE_Msk
#define FASTECH_AXIS_STATUS_MOTION_ACCEL_Pos	  		(5U)
#define FASTECH_AXIS_STATUS_MOTION_ACCEL_Msk	  		(0x1000000UL << FASTECH_AXIS_STATUS_MOTION_ACCEL_Pos)    /*!< 0x0010 0000 */
#define FASTECH_AXIS_STATUS_MOTION_ACCEL						FASTECH_AXIS_STATUS_MOTION_ACCEL_Msk
#define FASTECH_AXIS_STATUS_MOTION_DECEL_Pos	  		(6U)
#define FASTECH_AXIS_STATUS_MOTION_DECEL_Msk	  		(0x1000000UL << FASTECH_AXIS_STATUS_MOTION_DECEL_Pos)     /*!< 0x0100 0000 */
#define FASTECH_AXIS_STATUS_MOTION_DECEL						FASTECH_AXIS_STATUS_MOTION_DECEL_Msk
#define FASTECH_AXIS_STATUS_MOTION_CONSTANT_Pos			(7U)
#define FASTECH_AXIS_STATUS_MOTION_CONSTANT_Msk			(0x1000000UL << FASTECH_AXIS_STATUS_MOTION_CONSTANT_Pos) /*!< 0x1000 0000 */	
#define FASTECH_AXIS_STATUS_MOTION_CONSTANT					FASTECH_AXIS_STATUS_MOTION_CONSTANT_Msk


class jL_fastechComm : public jL_CommSerial
{
public:
	typedef int (*CallbackDataUpdate)(void* obj, uint8_t* data, uint16_t length); // 콜백 타입 정의
	CallbackDataUpdate m_callbackUpdate;
	void*  m_fastechMotorObject;
private:
	class _Register
	{
	public:
		class _io
		{
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

	enum class cmd_state
	{
		wait_stx,
		wait_id,
		wait_sync,
		wait_type,
		wiat_response,
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

	typedef struct
	{
		uint8_t	  id;
		uint8_t   sync_no;
		uint8_t   type;
		uint8_t   response;
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
		bool			wait_next;
		uint32_t  baud;
		uint8_t   state;
		uint32_t  pre_time;
		uint16_t  packet_size;
		uint32_t  index;
		uint8_t   error;
		fastech_packet_t  rx_packet;
	} fastech_t;

public:
	enum class fastech_cmdList
	{
		GET_SLAVE_INFO,
		GET_MOTOR_INFO,
		SAVE_ROM_ALL,
		GET_ROM_PARAM,
		SET_RAM_PARAM,
		GET_RAM_PARAM,
		SET_IO_OUT_REG,
		SET_IO_IN_REG,
		GET_IO_IN_REG,
		GET_IO_OUT_REG,
		SET_ASSN_IO_MAP,
		GET_ASSN_IO_MAP,
		READ_IO_MAP,
		TRG_OUT_RUN_A,
		TRG_OUT_STATUS,
		SERVO_ENABLE,
		ALARAM_RESET,
		GET_ALARM_INFO,
		MOVE_STOP,
		MOVE_EMG_STOP,
		MOVE_ORG_SINGLE_AXIS,
		MOVE_ABS_SINGLE_AXIS,
		MOVE_REL_SINGLE_AXIS,
		MOVE_TO_LIMIT,
		MOVE_VELOCITY,
		MOVE_ABS_OVERRIDE,
		MOVE_REL_OVERRIDE,
		MOVE_VELOCITY_OVERRIDE,
		MOVE_ALL_STOP,
		MOVE_ALL_EMG_STOP,
		MOVE_ALL_ORG_STOP,
		MOVE_ALL_ABS_POS,
		MOVE_ALL_REL_POS,
		GET_AXIS_STATUS,
		_max,
	};

private:
	struct fastech_cmd
	{
		byte   cmd;
		LPCTSTR   name;
	};

	class _Cmd
	{
		fastech_cmd* pCmd;
	public:
		uint32_t  count;
		uint32_t  max_cnt;

	public:
		_Cmd() :pCmd(nullptr), count(0)
			, max_cnt(static_cast<int>(fastech_cmdList::_max)) {
			pCmd = new fastech_cmd[static_cast<int>(fastech_cmdList::_max)];
		}

		virtual ~_Cmd() {
			if (pCmd != nullptr)
				delete[] pCmd;
			pCmd = nullptr;
		}

	public:
		inline int InsertCmd(fastech_cmd data) {
			pCmd[count].cmd = data.cmd;
			pCmd[count].name = data.name;
			count = count + 1;
			return count > max_cnt ? -1 : 0;
		}

		inline fastech_cmd GetCmd(fastech_cmdList index) {
			return pCmd[static_cast<int>(index)];
		}

		inline fastech_cmdList GetIndex(uint8_t cmd) {
			uint32_t index = 0;
			while (index < count)
			{
				if (pCmd[index].cmd == cmd)
					return static_cast<fastech_cmdList>(index);
				index++;
			}
			return fastech_cmdList::_max;
		}

	};


	/****************************************************
	 *	data
	 ****************************************************/
private:
	fastech_t m_FastechComm;
	_Cmd* m_CmdList;

public:
	bool m_IsWiatResponse;
	bool	m_bThreadLife;
	DWORD	m_ThreadId;
	HANDLE m_hThread;
	//_Register* m_pRegisterIO;

	jL_logManager* m_log;
	string m_lastLog[2];


	/****************************************************
	 *	Constructor
	 ****************************************************/
public:
	jL_fastechComm();
	virtual ~jL_fastechComm();

	/****************************************************
	 *	func
	 ****************************************************/
private:
	static UINT commThread(LPVOID pParam);
	void doTypeComm(uint8_t type);

public:
	void ResetData();
	int ParsingPacket();
	void ReadyPort();
	void UpdateReg();
	int SetMotorId(uint8_t id);
	int SendCmd(uint32_t cmd_index);
	uint32_t SendCmd(fastech_cmdList cmdenum, uint8_t* data = nullptr, uint8_t length = 0);
	DWORD ThreadRun();
	int Terminate();
	void AddStringCmdList(CComboBox* combobox);

	inline static int WarpFunc(void* obj) {
		jL_fastechComm* self = (jL_fastechComm*)(obj);
		return self->ParsingPacket();
	} 

	inline void SetCallback(void* obj, CallbackDataUpdate cb)
	{
		m_callbackUpdate = cb;
		m_fastechMotorObject = obj;
	}


};

