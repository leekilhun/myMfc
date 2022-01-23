#pragma once
// object 정의 및 데이터 클래스

class ICommon;

/******************  Bits definition for IO_ register  ****************/
#define IO_TYPE_OT0_Pos								(0U)
#define IO_TYPE_OT0_Msk								(0x1UL << IO_TYPE_OT0_Pos)           /*!< 0x00000001 */
#define IO_TYPE_OT0										IO_TYPE_OT0_Msk
#define IO_TYPE_OT1_Pos								(1U)
#define IO_TYPE_OT1_Msk								(0x1UL << IO_TYPE_OT1_Pos)           /*!< 0x00000002 */
#define IO_TYPE_OT1										IO_TYPE_OT1_Msk
#define IO_TYPE_OT2_Pos								(2U)
#define IO_TYPE_OT2_Msk								(0x1UL << IO_TYPE_OT2_Pos)           /*!< 0x00000004 */
#define IO_TYPE_OT2										IO_TYPE_OT2_Msk
#define IO_TYPE_OT3_Pos								(3U)
#define IO_TYPE_OT3_Msk								(0x1UL << IO_TYPE_OT3_Pos)           /*!< 0x00000008 */
#define IO_TYPE_OT3										IO_TYPE_OT3_Msk
#define IO_TYPE_OT4_Pos								(4U)
#define IO_TYPE_OT4_Msk								(0x1UL << IO_TYPE_OT4_Pos)           /*!< 0x00000010 */
#define IO_TYPE_OT4										IO_TYPE_OT4_Msk
#define IO_TYPE_OT5_Pos								(5U)
#define IO_TYPE_OT5_Msk								(0x1UL << IO_TYPE_OT5_Pos)           /*!< 0x00000020 */
#define IO_TYPE_OT5										IO_TYPE_OT5_Msk
#define IO_TYPE_OT6_Pos								(6U)
#define IO_TYPE_OT6_Msk								(0x1UL << IO_TYPE_OT6_Pos)           /*!< 0x00000040 */
#define IO_TYPE_OT6										IO_TYPE_OT6_Msk
#define IO_TYPE_OT7_Pos								(7U)
#define IO_TYPE_OT7_Msk								(0x1UL << IO_TYPE_OT7_Pos)           /*!< 0x00000080 */
#define IO_TYPE_OT7										IO_TYPE_OT7_Msk
#define IO_TYPE_OT8_Pos								(8U)
#define IO_TYPE_OT8_Msk								(0x1UL << IO_TYPE_OT8_Pos)           /*!< 0x00000100 */
#define IO_TYPE_OT8										IO_TYPE_OT8_Msk
#define IO_TYPE_OT9_Pos								(9U)
#define IO_TYPE_OT9_Msk								(0x1UL << IO_TYPE_OT9_Pos)           /*!< 0x00000200 */
#define IO_TYPE_OT9										IO_TYPE_OT9_Msk
#define IO_TYPE_OT10_Pos							(10U)
#define IO_TYPE_OT10_Msk							(0x1UL << IO_TYPE_OT10_Pos)           /*!< 0x00000400 */
#define IO_TYPE_OT10									IO_TYPE_OT10_Msk
#define IO_TYPE_OT11_Pos							(11U)
#define IO_TYPE_OT11_Msk							(0x1UL << IO_TYPE_OT11_Pos)           /*!< 0x00000800 */
#define IO_TYPE_OT11									IO_TYPE_OT11_Msk
#define IO_TYPE_OT12_Pos							(12U)
#define IO_TYPE_OT12_Msk							(0x1UL << IO_TYPE_OT12_Pos)           /*!< 0x00001000 */
#define IO_TYPE_OT12									IO_TYPE_OT12_Msk
#define IO_TYPE_OT13_Pos							(13U)
#define IO_TYPE_OT13_Msk							(0x1UL << IO_TYPE_OT13_Pos)           /*!< 0x00002000 */
#define IO_TYPE_OT13									IO_TYPE_OT13_Msk
#define IO_TYPE_OT14_Pos							(14U)
#define IO_TYPE_OT14_Msk							(0x1UL << IO_TYPE_OT14_Pos)           /*!< 0x00004000 */
#define IO_TYPE_OT14									IO_TYPE_OT15_Msk
#define IO_TYPE_OT15_Pos							(15U)
#define IO_TYPE_OT15_Msk							(0x1UL << IO_TYPE_OT15_Pos)           /*!< 0x00008000 */
#define IO_TYPE_OT15									IO_TYPE_OT15_Msk


#define  SYSTEM_PERIPH_BASE						0x00000000UL // 증가 시 0x00100000UL~
/*!< peripherals> */
//Hardware Layer 
#define HL_IO_BASE										(SYSTEM_PERIPH_BASE + 0x00000100UL)
#define HL_SERIAL_BASE								(SYSTEM_PERIPH_BASE + 0x00000140UL)
#define HL_CYLINDER_BASE							(SYSTEM_PERIPH_BASE + 0x00000200UL)
#define HLVACUUM_BASE									(SYSTEM_PERIPH_BASE + 0x00000400UL)

//Mechanical Layer 
#define ML_OPPANEL_BASE								(SYSTEM_PERIPH_BASE + 0x00001000UL) // max 16 object 
//#define ML__BASE										(SYSTEM_PERIPH_BASE + 0x00001010UL)
//#define ML__BASE										(SYSTEM_PERIPH_BASE + 0x00001020UL)


//Control Layer 
#define CTR_MANAGE_OPPANEL_BASE				(SYSTEM_PERIPH_BASE + 0x00001100UL)
//#define CTR__BASE										(SYSTEM_PERIPH_BASE + 0x00001110UL)

//Process Layer 
#define TRS_AUTOMANAGER_BASE					(SYSTEM_PERIPH_BASE + 0x00001200UL)
#define TRS_TOWERLAMP_BASE						(SYSTEM_PERIPH_BASE + 0x00001210UL)


/**
 * Object Type Enum Define
 * @stereotype enum
 */
enum eObjType
{
	OBJ_NONE,

	// Hardware Layer
	OBJ_HL_IO = 100,
	OBJ_HL_CYLINDER,					//101
	OBJ_HL_VACUUM,						//102
	OBJ_HL_INDUCTION_MOTOR,		//103
	OBJ_HL_VISION,						//104
	OBJ_HL_MOTION,						//105
	OBJ_HL_MOTION_LIB,				//106
	OBJ_HL_MOTION_BOARD,			//107
	OBJ_HL_SERIAL,						//108
	OBJ_HL_ETHERNET,					//109

	// Mechanical Layer - Common
	OBJ_ML_OP_PANEL = 200,
	OBJ_ML_SAFETYPLC = 205,
	OBJ_ML_ROBOTUNIT = 240,
	OBJ_ML_ROBOTINTERFACE = 245,

	// Control Layer - Common
	OBJ_CL_MANAGE_OP_PANEL = 300,
	OBJ_CL_ROBOT = 310,

	// Process Layer - Common
	OBJ_PL_AUTO_MANAGER = 400,
	OBJ_PL_TOWER_LAMP = 405,
	OBJ_PL_ROBOT = 430,

	// Common Layer
	OBJ_ETC_RESOURCE_MANAGER = 600,

};


enum eLogCategory
{
	LOG_CATEGORY_TACT_TIME,
	LOG_CATEGORY_HARDWARE_LAYER,
	LOG_CATEGROY_MECHANICAL_LAYER,
	LOG_CATEGORY_CONTROL_LAYER,
	LOG_CATEGORY_PROCESS_LAYER
};



enum class ObjType
{
	NONE,

	// Hardware Layer
	HL_IO = 100,
	HL_CYLINDER,				//101
	HL_VACUUM,					//102
	HL_INDUCTION_MOTOR,	//103
	HL_VISION,					//104
	HL_MOTION,					//105
	HL_MOTION_LIB,			//106
	HL_MOTION_BOARD,		//107
	HL_SERIAL,					//108
	HL_ETHERNET,				//109

	// Mechanical Layer - Common
	ML_OP_PANEL = 200,

	// Control Layer - Common
	CL_MANAGE_OP_PANEL = 300,

	// Process Layer - Common
	PL_AUTO_MANAGER = 400,
	PL_TOWER_LAMP = 405,

	// Common Layer
	ETC_RESOURCE_MANAGER = 600,
};








class ObjDataDef
{
	/****************************************************
	 *	data
	 ****************************************************/
public:

	struct Info_t
	{
		// 생성 후 시스템에 포함되면 true
		BOOL		 is_reg;
		// 오브젝트 
		ObjType	 type;
		int			 baseAddr;
		int			 instanceNo;
		CString	 name;
		CString	 fileName;
		ICommon* pObj;
	};

private:
	BOOL			m_is_reg;
	ObjType		m_type;
	int				m_objID;
	int				m_baseAddr;
	int				m_instanceNo;
	ICommon* m_pObj;

	CString		m_name;
	CString	  m_fileName;

public:
	/****************************************************
	 *	Constructor
	 ****************************************************/
	ObjDataDef() :m_is_reg(FALSE)
		, m_type(ObjType::NONE), m_objID(0), m_baseAddr(0)
		, m_instanceNo(0), m_pObj(nullptr)
	{
		Init();
	}

	ObjDataDef(const ObjDataDef& self)
	{
		m_is_reg = self.m_is_reg;
		m_type = self.m_type;
		m_objID = self.m_objID;
		m_baseAddr = self.m_baseAddr;
		m_instanceNo = self.m_instanceNo;
		m_name = self.m_name;
		m_fileName = self.m_fileName;
		m_pObj = self.m_pObj;
	}
	virtual ~ObjDataDef() { Init(); }

	/****************************************************
	 *	func
	 ****************************************************/
public:
	inline void Init() {
		m_is_reg = FALSE;
		m_type = ObjType::NONE;
		m_objID = 0;
		m_baseAddr = 0;
		m_instanceNo = 0;
		m_name.Empty();
		m_fileName.Empty();
		m_pObj = nullptr;


	};



	inline int SetData(Info_t* data) {
		//m_is_reg = data->is_reg;
		m_type = data->type;

		m_baseAddr = data->baseAddr;
		m_instanceNo = data->instanceNo;
		m_objID = data->baseAddr + data->instanceNo;

		m_name = data->name;
		m_fileName = data->fileName;

		//m_pObj				= data->pObj;
		return 0;
	}

	inline void GetData(Info_t* data) const {
		data->type = m_type;
		data->baseAddr = m_baseAddr;
		data->instanceNo = m_instanceNo;
		data->name = m_name;
		data->fileName = m_fileName;
		data->pObj = m_pObj;
		data->is_reg = m_is_reg;
	}

	inline int GetObjectID() const {
		return m_objID;
	}

	inline ObjType GetObjectType() const {
		return m_type;
	}

	inline CString GetObjectName() const {
		return m_name;
	}

	inline CString GetLogFileName() const {
		return m_fileName;
	}

	inline int GetObjectInstanceNo() const {
		return m_instanceNo;
	}

	inline ICommon* GetSystemObject() const {
		return m_pObj;
	}

	inline void SetSystemObject(ICommon* pComp) {
		m_is_reg = true;
		m_pObj = pComp;
	}

	inline bool IsMatchingID(int ojb_id)	{
		if (ojb_id == m_objID)
			return true;
		return false;
	}
};




/** @add to group Peripheral_declaration
	* @{
	*/
//#define FASTECH_1616IO									((IO_DataDef *) HL_IO_BASE)