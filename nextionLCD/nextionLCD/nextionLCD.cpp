
// nextionLCD.cpp: 애플리케이션에 대한 클래스 동작을 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "nextionLCD.h"
#include "nextionLCDDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CnextionLCDApp

BEGIN_MESSAGE_MAP(CnextionLCDApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()



void cmdTest(uint8_t* reg_mcu, uint8_t len)
{
  ;
}

// CnextionLCDApp 생성

CnextionLCDApp::CnextionLCDApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
	m_nextionLcd = nullptr;
		
}


// 유일한 CnextionLCDApp 개체입니다.

CnextionLCDApp theApp;


// CnextionLCDApp 초기화

BOOL CnextionLCDApp::InitInstance()
{
	CWinApp::InitInstance();


	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}





  uint8_t tfemp[80] = { 0x63, 0x63,0x5f,0x72,0x63,0x76,0x5F,0x69,0x6E,0x3D,0, };





  enum class reg_bank {
    err_H,
    err_L,
    runReg,
    stateReg,
    _max
  };

  enum class reg_ {
    b0,
    b1,
    b2,
    b3,
    b4,
    b5,
    b6,
    b7,
    _max
  };


#if 0

  class _Regist {
    class _elem {
      bool* m_bit;

      explicit _elem(const _elem& ref) :m_bit(nullptr) {}
      _elem& operator=(const _elem& ref) { return *this; }
    public:
      explicit _elem() :m_bit(new bool[8]{ false }) {};
      virtual ~_elem() { delete[] m_bit; m_bit = nullptr; };

    public:
      /*inline bool& operator[](const uint8_t elem_no) {
        if (IsOutBound(elem_no))
          return this->m_bit[0];
        return m_bit[elem_no];
      } */
      inline bool& operator[](const reg_ elem_no) {
        return m_bit[static_cast<int>(elem_no)];
      }

      inline void operator = (const uint8_t in_data) {
        uint8_t data = 0;
        for (uint8_t i = 0; i < 8; i++) {
          data = in_data;
          data = data >> i;
          data = 0xfe | data;
          data = ~(data);
          m_bit[i] = !data; //!(~(0xfe | (data >> i)));
        }
      }

      inline uint8_t get() {
        uint8_t data;
        data = (int)m_bit[0] | ((int)m_bit[1] * 2) | ((int)m_bit[2] * 4) | ((int)m_bit[3] * 8)
          | ((int)m_bit[4] * 16) | ((int)m_bit[5] * 32) | ((int)m_bit[6] * 64 | ((int)m_bit[7] * 128));
        return data;
      }


};

    _elem* m_byte;
    uint8_t* m_data;

    explicit _Regist(const _Regist& ref) :m_byte(nullptr), m_data(nullptr) {}
    _Regist& operator=(const _Regist ref) { return *this; }
  public:
    _Regist() :m_byte(new _elem[static_cast<uint8_t>(reg_bank::_max)]), m_data(nullptr) { };
    virtual ~_Regist() {
      delete[] m_byte; m_byte = nullptr;
      if (m_data != nullptr) { delete[] m_data; } m_data = nullptr;
    };

  public:
    inline void ClearAll() {
      Clear(reg_bank::err_H);
      Clear(reg_bank::err_L);
      Clear(reg_bank::runReg);
      Clear(reg_bank::stateReg);
    }

    inline void Clear(reg_bank bank) {
      m_byte[static_cast<uint8_t>(bank)] = 0;
    }

    inline uint8_t SetOn(reg_bank bank, reg_ reg_no) {
      m_byte[static_cast<uint8_t>(bank)][reg_no] = true;
      return m_byte[static_cast<uint8_t>(bank)].get();
    }
    inline uint8_t SetOff(reg_bank bank, reg_ reg_no) {
      m_byte[static_cast<uint8_t>(bank)][reg_no] = false;
      return m_byte[static_cast<uint8_t>(bank)].get();
    }

    inline bool Get(reg_bank bank, reg_ bit) {
      return m_byte[static_cast<uint8_t>(bank)][bit];
    }

    inline uint8_t* Out() {
      if (m_data != nullptr) {
        m_data[0] = m_byte[0].get();
        m_data[1] = m_byte[1].get();
        m_data[2] = m_byte[2].get();
        m_data[3] = m_byte[3].get();
      }
      else {
        m_data = new uint8_t[static_cast<uint8_t>(reg_bank::_max)]{
          m_byte[0].get(),m_byte[1].get(),m_byte[2].get(),m_byte[3].get()
        };
      };

      return m_data;
    }

    inline _elem& operator[](reg_bank bank) {
      return m_byte[static_cast<int>(bank)];
    }

    inline uint8_t operator[](const int reg) {
      return m_byte[reg].get();
    }
  };


#endif // 0

  
  class _Regist {
    class _elem {
      bool m_bit[8];

      explicit _elem(const _elem& ref):m_bit(){}
      _elem& operator=(const _elem& ref) { return *this; }
    public:
      explicit _elem() :m_bit() {
      };
      virtual ~_elem() { };

    public:
      /*inline bool& operator[](const uint8_t elem_no) {
        if (IsOutBound(elem_no))
          return this->m_bit[0];
        return m_bit[elem_no];
      } */ 
      inline bool& operator[](const reg_ elem_no) {
        return m_bit[static_cast<int>(elem_no)];
      }

      inline void operator = (const uint8_t in_data) {
        uint8_t data = 0;        
        for (uint8_t i=0; i < 8; i++) {
          data = in_data;
          data = data >> i;
          data = 0xfe | data;
          data = ~(data);
          m_bit[i] = !data; //!(~(0xfe | (data >> i)));
        }
      }

      inline uint8_t get() {
        uint8_t data;
        data = (int)m_bit[0] | ((int)m_bit[1] * 2) | ((int)m_bit[2] * 4) | ((int)m_bit[3] * 8)
          | ((int)m_bit[4] * 16) | ((int)m_bit[5] * 32) | ((int)m_bit[6] * 64 | ((int)m_bit[7] * 128));
        return data;
      }


    };

    _elem m_byte[static_cast<uint8_t>(reg_bank::_max)];
    uint8_t m_data[static_cast<uint8_t>(reg_bank::_max)];

    explicit _Regist(const _Regist& ref):m_byte(), m_data() {}
    _Regist& operator=(const _Regist ref) { return *this; }
  public:
    _Regist() :m_byte(), m_data() { };
    virtual ~_Regist() { };

  public:
    inline void ClearAll() {
        Clear(reg_bank::err_H);
        Clear(reg_bank::err_L);
        Clear(reg_bank::runReg);
        Clear(reg_bank::stateReg);
    }

    inline void Clear(reg_bank bank) {
      m_byte[static_cast<uint8_t>(bank)] = 0;
    }

    inline uint8_t SetOn(reg_bank bank, reg_ reg_no) {
      m_byte[static_cast<uint8_t>(bank)][reg_no] = true;
      return m_byte[static_cast<uint8_t>(bank)].get();
    }
    inline uint8_t SetOff(reg_bank bank, reg_ reg_no) {
      m_byte[static_cast<uint8_t>(bank)][reg_no] = false;
      return m_byte[static_cast<uint8_t>(bank)].get();
    }

    inline bool Get(reg_bank bank, reg_ bit) {
      return m_byte[static_cast<uint8_t>(bank)][bit];
    }

    inline uint8_t* Out() {
      m_data[0] = m_byte[0].get();
      m_data[1] = m_byte[1].get();
      m_data[2] = m_byte[2].get();
      m_data[3] = m_byte[3].get();
      return m_data;
    }

    inline _elem& operator[](reg_bank bank) {
      return m_byte[static_cast<int>(bank)];
    }
    
    inline uint8_t operator[](const int reg) {
      return m_byte[reg].get();
    }
  };


  _Regist reg;

  reg[reg_bank::err_H] = 245;
  reg[reg_bank::err_H][reg_::b1] = false;
  uint8_t temp = reg[reg_bank::runReg].get();
  bool ret = reg.Get(reg_bank::err_L, reg_::b2); // reg[1][2];
  ret = reg[reg_bank::err_H][reg_::b4];
  ret = reg[reg_bank::err_H][reg_::b7];
  reg[reg_bank::err_H][reg_::b7] = true;
  reg[0];


  cmdTest(reg.Out(), 4);




	m_nextionLcd = new nextionCommLcd;



	CnextionLCDDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 여기에 [확인]을 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 여기에 [취소]를 클릭하여 대화 상자가 없어질 때 처리할
		//  코드를 배치합니다.
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "경고: 대화 상자를 만들지 못했으므로 애플리케이션이 예기치 않게 종료됩니다.\n");
		TRACE(traceAppMsg, 0, "경고: 대화 상자에서 MFC 컨트롤을 사용하는 경우 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS를 수행할 수 없습니다.\n");
	}


	// 대화 상자가 닫혔으므로 응용 프로그램의 메시지 펌프를 시작하지 않고 응용 프로그램을 끝낼 수 있도록 FALSE를
	// 반환합니다.
	return FALSE;
}



nextionCommLcd* CnextionLCDApp::Get_nextionObj()
{
	return m_nextionLcd;
}

int CnextionLCDApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (m_nextionLcd != nullptr)
		delete m_nextionLcd;
	m_nextionLcd = nullptr;
	return CWinApp::ExitInstance();
}
