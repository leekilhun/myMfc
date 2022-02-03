
#pragma once
#include "stdint.h"
#include <chrono>

using namespace std;

namespace conv 
{
  inline int DwToInt(byte* bytes) {
    int a = bytes[0] & 0xFF;
    a |= ((bytes[1] << 8) & 0xFF00);
    a |= ((bytes[2] << 16) & 0xFF0000);
    a |= ((bytes[3] << 24) & 0xFF000000);

    return a;
  }




}
namespace timer
{
  class _us
  {
    chrono::system_clock::time_point start_time;
    bool is_start;
  public:
    _us() :start_time({}), is_start(false)
    {
    }

    ~_us()
    {
    }

  private:
  public:
    inline void Start(){
      start_time = chrono::system_clock::now();
      is_start = true;
    }

    inline chrono::system_clock::time_point End() {
      return chrono::system_clock::now();
    }

    inline void Stop() {
      is_start = false;
    }

    inline bool IsStarted() {
      return is_start;
    }

    inline uint64_t Elaps(bool is_reset = false) {
      if (!is_start) //not
        return 0; 
      chrono::microseconds micro = chrono::duration_cast<chrono::microseconds>(End() - start_time);
      if (is_reset)
        Start();
      return micro.count();
    }

    inline bool LessThan(uint32_t us) {
      if (Elaps() < us)
      {
        Start();
        return true;
      }
      else
        return false;
    }

    inline bool MoreThan(uint32_t us) {
      if (Elaps() > us)
      {
        Start();
        return true;
      }
      else
        return false;
    }
  };



  class _ms
  {
    chrono::system_clock::time_point start_time;
    bool is_start;
  public:
    _ms() :start_time({}), is_start(false)
    {
    }

    ~_ms()
    {
    }

  private:
  public:
    inline void Start() {
      start_time = chrono::system_clock::now();
      is_start = true;
    }

    inline chrono::system_clock::time_point End() {
      return chrono::system_clock::now();
    }

    inline void Stop() {
      is_start = false;
    }

    inline bool IsStarted() {
      return is_start;
    }

    inline uint64_t Elaps(bool is_reset = false) {
      if (!is_start) //not
        return 0;
      chrono::milliseconds mill = chrono::duration_cast<chrono::milliseconds>(End() - start_time);
      if (is_reset)
        Start();
      return mill.count();
    }

    inline bool LessThan(uint32_t ms) {
      if (Elaps() < ms)
      {
        Start();
        return true;
      }
      else
        return false;
    }

    inline bool MoreThan(uint32_t ms) {
      if (Elaps() > ms)
      {
        Start();
        return true;
      }
      else
        return false;
    }
  };



}



namespace util
{
  const unsigned short crc_table[] = 
  { 
    0x0000, 0xC0C1, 0xC181, 0x0140, 0xC301, 0x03C0, 0x0280, 0xC241,
    0xC601, 0x06C0, 0x0780, 0xC741, 0x0500, 0xC5C1, 0xC481, 0x0440,
    0xCC01, 0x0CC0, 0x0D80, 0xCD41, 0x0F00, 0xCFC1, 0xCE81, 0x0E40,
    0x0A00, 0xCAC1, 0xCB81, 0x0B40, 0xC901, 0x09C0, 0x0880, 0xC841,
    0xD801, 0x18C0, 0x1980, 0xD941, 0x1B00, 0xDBC1, 0xDA81, 0x1A40,
    0x1E00, 0xDEC1, 0xDF81, 0x1F40, 0xDD01, 0x1DC0, 0x1C80, 0xDC41,
    0x1400, 0xD4C1, 0xD581, 0x1540, 0xD701, 0x17C0, 0x1680, 0xD641,
    0xD201, 0x12C0, 0x1380, 0xD341, 0x1100, 0xD1C1, 0xD081, 0x1040,
    0xF001, 0x30C0, 0x3180, 0xF141, 0x3300, 0xF3C1, 0xF281, 0x3240,
    0x3600, 0xF6C1, 0xF781, 0x3740, 0xF501, 0x35C0, 0x3480, 0xF441,
    0x3C00, 0xFCC1, 0xFD81, 0x3D40, 0xFF01, 0x3FC0, 0x3E80, 0xFE41,
    0xFA01, 0x3AC0, 0x3B80, 0xFB41, 0x3900, 0xF9C1, 0xF881, 0x3840,
    0x2800, 0xE8C1, 0xE981, 0x2940, 0xEB01, 0x2BC0, 0x2A80, 0xEA41,
    0xEE01, 0x2EC0, 0x2F80, 0xEF41, 0x2D00, 0xEDC1, 0xEC81, 0x2C40,
    0xE401, 0x24C0, 0x2580, 0xE541, 0x2700, 0xE7C1, 0xE681, 0x2640,
    0x2200, 0xE2C1, 0xE381, 0x2340, 0xE101, 0x21C0, 0x2080, 0xE041,
    0xA001, 0x60C0, 0x6180, 0xA141, 0x6300, 0xA3C1, 0xA281, 0x6240,
    0x6600, 0xA6C1, 0xA781, 0x6740, 0xA501, 0x65C0, 0x6480, 0xA441,
    0x6C00, 0xACC1, 0xAD81, 0x6D40, 0xAF01, 0x6FC0, 0x6E80, 0xAE41,
    0xAA01, 0x6AC0, 0x6B80, 0xAB41, 0x6900, 0xA9C1, 0xA881, 0x6840,
    0x7800, 0xB8C1, 0xB981, 0x7940, 0xBB01, 0x7BC0, 0x7A80, 0xBA41,
    0xBE01, 0x7EC0, 0x7F80, 0xBF41, 0x7D00, 0xBDC1, 0xBC81, 0x7C40,
    0xB401, 0x74C0, 0x7580, 0xB541, 0x7700, 0xB7C1, 0xB681, 0x7640,
    0x7200, 0xB2C1, 0xB381, 0x7340, 0xB101, 0x71C0, 0x7080, 0xB041,
    0x5000, 0x90C1, 0x9181, 0x5140, 0x9301, 0x53C0, 0x5280, 0x9241,
    0x9601, 0x56C0, 0x5780, 0x9741, 0x5500, 0x95C1, 0x9481, 0x5440,
    0x9C01, 0x5CC0, 0x5D80, 0x9D41, 0x5F00, 0x9FC1, 0x9E81, 0x5E40,
    0x5A00, 0x9AC1, 0x9B81, 0x5B40, 0x9901, 0x59C0, 0x5880, 0x9841,
    0x8801, 0x48C0, 0x4980, 0x8941, 0x4B00, 0x8BC1, 0x8A81, 0x4A40,
    0x4E00, 0x8EC1, 0x8F81, 0x4F40, 0x8D01, 0x4DC0, 0x4C80, 0x8C41,
    0x4400, 0x84C1, 0x8581, 0x4540, 0x8701, 0x47C0, 0x4680, 0x8641,
    0x8201, 0x42C0, 0x4380, 0x8341, 0x4100, 0x81C1, 0x8081, 0x4040 
};

  inline void crc16_put(uint32_t* p_crc_cur, uint8_t data_in) {
    uint32_t crc;
    uint8_t i;
    
    crc = *p_crc_cur;
    i = crc ^ data_in;
    crc >>= 8;
    crc ^= crc_table[i];
    *p_crc_cur  = crc;    
  }



  uint32_t millis(void);

  /// <summary>
  /// 프로세스 지연
  /// </summary>
  /// <param name="ms">ms</param>
  inline void delay(uint32_t ms) {
    uint32_t pre_time;

    pre_time = millis();
    while (1) {
      if (millis() - pre_time >= ms)
        break;
    }
  }


  /// <summary>
  /// 프로세스 시간 측정 (ms)
  /// </summary>
  /// <param name=""></param>
  /// <returns></returns>
  inline uint32_t millis(void) {
    double ret;
    LARGE_INTEGER freq, counter;

    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&freq);

    ret = ((double)counter.QuadPart / (double)freq.QuadPart) * 1000.0;

    return (uint32_t)ret;
  }


  /// <summary>
  /// 프로세스 정밀 시간 측정
  /// </summary>
  /// <param name=""></param>
  /// <returns></returns>
  inline uint32_t micros(void) {
    LARGE_INTEGER freq, counter;
    double ret;

    QueryPerformanceCounter(&counter);
    QueryPerformanceFrequency(&freq);

    ret = ((double)counter.QuadPart / (double)freq.QuadPart) * 1000000.0;

    return (uint32_t)ret;
  }



}



namespace trans 
{


  //
  // Char → CString
  //
  inline CString CharToCString(char* str) {
    CString cStr;
#if defined(UNICODE) || defined(_UNICODE)
    int nLen = (int)strlen(str) + 1;
    TCHAR* tszTemp = nullptr;
    tszTemp = new TCHAR[nLen];
    memset(tszTemp, 0x00, nLen * sizeof(TCHAR));
    MultiByteToWideChar(
      CP_ACP,
      0,
      str,
      -1,
      tszTemp,
      nLen 
    );

    cStr.Format(_T("%s"), tszTemp);
    if (tszTemp)
    {
      delete[] tszTemp;
      tszTemp = NULL;
    }
#else
    cStr.Format("%s", str);
#endif
    return cStr;
  }



  /// <summary>
    /// Char 문자열을 Tchar문자열로 변환
    /// </summary>
    /// <param name="str">Char 문자열</param>
    /// <param name="ret_str">Tchar문자열로 반환주소</param>
    /// <returns>문자 수 </returns>
  inline int CharToLPTSTR(LPSTR str, LPTSTR ret_str, int length) {
    if (strlen(str) == 0)
      return 0;
    if (_tcslen(ret_str) != 0)
      memset(ret_str, 0x00, length);   /* _tcslen(ret_str) * sizeof(TCHAR));*/
    int ret = 0;

#if defined(UNICODE) || defined(_UNICODE)
    //int nLen = (int)_tcslen(ret_str) * (int)sizeof(TCHAR);
    ret = MultiByteToWideChar(
      CP_ACP,
      MB_PRECOMPOSED,
      str,
      (int)strlen(str),
      ret_str,
      length
    );
#else
    int nLen = strlen(str) + 1;
    _tcscpy(ret_str, str);
    ret = nLen;
#endif
    return ret;
  }


  //
  // TCHAR → CString
  //
  inline CString TCHARToCString(TCHAR* str) {
    CString cStr;
    cStr.Format(_T("%s"), str);
    return cStr;
  }


  /// <summary>
  /// TCHAR 문자열 → Char 문자열
  /// </summary>
  /// <param name="str"></param>
  /// <param name="ret_str"></param>
  /// <returns></returns>
  inline int TcharToLPSTR(LPTSTR str, LPSTR ret_str, int length) {
    int ret = 0;
    if (_tcslen(str) == 0)
      return 0;
    if (length != 0)
      memset(ret_str, 0x00, length * sizeof(CHAR));

#if defined(UNICODE) || defined(_UNICODE)
    ret = WideCharToMultiByte(
      CP_ACP,
      0,
      str,
      -1,
      ret_str,
      length + 1,
      NULL,
      NULL
    );
#else
    strcpy(ret_str, str);

#endif
    return ret;
  }


}