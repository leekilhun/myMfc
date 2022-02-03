/*
 * planner.h
 *
 *  Created on: 2022. 1. 27.
 *      Author: gns2l
 */

#pragma once

#include "def.h"

class _Scheduler
{
public:
  typedef void (*scheduler_cb)(void* obj); // 콜백 타입 정의


  enum class job_period {
    _1ms,
    _5ms,
    _10ms,
    _50ms,
    _100ms,
    _500ms,
    _1000ms,
    _max
  };
#define JOB_PERIOD(x)          static_cast<job_period>(x)

  struct scheduler_tbl_t {
    job_period           jobPeriod{};
    bool                 enable{};
    uint32_t             pass_ms{};
    scheduler_cb         func_callback{};
  };

  /****************************************************
   *	data
   ****************************************************/
public:
  scheduler_tbl_t job_tbl[static_cast<int>(job_period::_max)];
private:
  void* CallbackObj;
  uint32_t test_time[2] ;
public:

  bool	m_bThreadLife;
  DWORD	m_ThreadId;
  HANDLE m_hThread;

  /****************************************************
   *	Constructor
   ****************************************************/
  explicit _Scheduler(const _Scheduler& ref) :
    job_tbl(), CallbackObj(nullptr), m_bThreadLife(false), m_ThreadId(0), m_hThread(), test_time( ) {
  }
  _Scheduler& operator=(const _Scheduler ref) { return *this; }
public:
  _Scheduler(void* obj) :job_tbl(), CallbackObj(nullptr)
   ,m_bThreadLife(false), m_ThreadId(0), m_hThread(), test_time()
  {
    CallbackObj = obj;
    for (int i = 0; i < static_cast<int>(job_period::_max); i++)
    {
      job_tbl[i].jobPeriod = JOB_PERIOD(i);
      job_tbl[i].enable = false;
      job_tbl[i].pass_ms = 0;
      job_tbl[i].func_callback = nullptr;
    }
  }

  virtual~_Scheduler() {
    if (m_bThreadLife)
    {
      m_bThreadLife = false;

      while (m_ThreadId != 0);
    }
  }
  /****************************************************
   *	func
   ****************************************************/
private:
  inline static UINT schedulerThread(LPVOID pParam) {
    _Scheduler* pThis = (_Scheduler*)pParam;
    uint32_t pre_us = util::micros();
    uint32_t pass_us = 0;
    TRACE("Scheduler ISR Thread start\n");
    while (pThis->m_bThreadLife)
    {
      /* make 1ms tick */
      pass_us = util::micros() - pre_us;
      if (pass_us > 999) {
        pre_us = util::micros();
        pThis->scheduler_ISR();
      }
      //Sleep(1);
    }
    TRACE("Scheduler ISR Thread Terminatet\n");
    pThis->m_ThreadId = 0;
    return 0;
  
  }

public:
  inline DWORD ThreadRun() {
    m_hThread = CreateThread(
      (LPSECURITY_ATTRIBUTES)NULL,
      0,
      (LPTHREAD_START_ROUTINE)schedulerThread,
      (LPVOID)this,
      0,
      &this->m_ThreadId
    );
    if (!m_hThread)
      TRACE("Fail Scheduler ISR Thread \n");
    else
      m_bThreadLife = true;

    return this->m_ThreadId;
  }

  int Terminate() {
    m_bThreadLife = false;
    while (m_ThreadId != 0);
    return 0;
  }

public:
  inline scheduler_tbl_t& operator[](job_period period) {
    return job_tbl[static_cast<int>(period)];
  }

  inline void Start(job_period period) {
    job_tbl[static_cast<int>(period)].pass_ms = 0;
    job_tbl[static_cast<int>(period)].enable = true;
  }
  inline void Stop(job_period period) {
    job_tbl[static_cast<int>(period)].pass_ms = 0;
    job_tbl[static_cast<int>(period)].enable = false;
  }
  inline void AttachCallback(job_period period, scheduler_cb func) {
    job_tbl[static_cast<int>(period)].func_callback = func;
  }

  inline void scheduler_ISR(void) {
    //test_time[0] = util::millis() - test_time[0];
    //test_time[0] = util::millis();   

    for (int i = 0; i < static_cast<int>(job_period::_max); i++)
    {
      if (job_tbl[i].enable == true)
      {
        job_tbl[i].pass_ms += 1;
      }
      if (job_tbl[i].func_callback != NULL)
      {
        switch (JOB_PERIOD(i))
        {
        case job_period::_1ms:
          (*job_tbl[i].func_callback)(CallbackObj);
          break;
        case job_period::_5ms:
          if (job_tbl[i].pass_ms < 5) {
            test_time[1] = test_time[1] + test_time[0];
            break;
          }

          (*job_tbl[i].func_callback)(CallbackObj);
          job_tbl[i].pass_ms = 0;
          test_time[1] = 0;
          break;
        case job_period::_10ms:
          if (job_tbl[i].pass_ms < 10)
            break;
          (*job_tbl[i].func_callback)(CallbackObj);
          job_tbl[i].pass_ms = 0;
          break;
        case job_period::_50ms:
          if (job_tbl[i].pass_ms < 50)
            break;
          (*job_tbl[i].func_callback)(CallbackObj);
          job_tbl[i].pass_ms = 0;
          break;
        case job_period::_100ms:
          if (job_tbl[i].pass_ms < 100)
            break;
          (*job_tbl[i].func_callback)(CallbackObj);
          job_tbl[i].pass_ms = 0;
          break;
        case job_period::_500ms:
          if (job_tbl[i].pass_ms < 500)
            break;
          (*job_tbl[i].func_callback)(CallbackObj);
          job_tbl[i].pass_ms = 0;
          break;
        case job_period::_1000ms:
          if (job_tbl[i].pass_ms < 1000)
            break;
          (*job_tbl[i].func_callback)(CallbackObj);
          job_tbl[i].pass_ms = 0;
          break;
        }
      }
    }

  }











};
