/*
 * jL_scheduler.c
 *
 *  Created on: 2022. 1. 27.
 *      Author: gns2l
 */
#include "pch.h"
#include "jL_scheduler.h"


#if 0
typedef struct
{
  job_period           jobPeriod;
  bool                 state;
  uint32_t            pass_ms;
  void (*func_callback)(void);
}scheduler_tbl_t;

static scheduler_tbl_t job_tbl[JOB_PERIOD_max] =
    {
      { JOB_PERIOD_1ms,     false,   0,  NULL},
      { JOB_PERIOD_5ms,     false,   0,  NULL},
      { JOB_PERIOD_10ms,    false,   0,  NULL},
      { JOB_PERIOD_50ms,    false,   0,  NULL},
      { JOB_PERIOD_100ms,   false,   0,  NULL},
      { JOB_PERIOD_500ms,   false,   0,  NULL},
      { JOB_PERIOD_1000ms,  false,   0,  NULL},
    };

void scheduler_Start(job_period period)
{
  job_tbl[period].pass_ms = 0;
  job_tbl[period].state = true;
}

void scheduler_Stop(job_period period)
{
  job_tbl[period].pass_ms = 0;
  job_tbl[period].state = false;
}


void scheduler_AttachCallback(job_period period, void (*func)())
{
  job_tbl[period].func_callback = func;
}

void scheduler_ISR(void)
{
  for (int i = 0; i < JOB_PERIOD_max; i++)
  {
    if (job_tbl[i].state == true)
    {
      job_tbl[i].pass_ms += 1 ;
    }
    if (job_tbl[i].func_callback != NULL)
    {
      switch(i)
      {
        case JOB_PERIOD_1ms:
          (*job_tbl[i].func_callback)();
          break;
        case JOB_PERIOD_5ms:
          if (job_tbl[i].pass_ms <6)
            break;
          (*job_tbl[i].func_callback)();
          job_tbl[i].pass_ms = 0;
          break;
        case JOB_PERIOD_10ms:
          if (job_tbl[i].pass_ms <11)
            break;
          (*job_tbl[i].func_callback)();
          job_tbl[i].pass_ms = 0;
          break;
        case JOB_PERIOD_50ms:
          if (job_tbl[i].pass_ms <51)
            break;
          (*job_tbl[i].func_callback)();
          job_tbl[i].pass_ms = 0;
          break;
        case JOB_PERIOD_100ms:
          if (job_tbl[i].pass_ms <101)
            break;
          (*job_tbl[i].func_callback)();
          job_tbl[i].pass_ms = 0;
          break;
        case JOB_PERIOD_500ms:
          if (job_tbl[i].pass_ms <501)
            break;
          (*job_tbl[i].func_callback)();
          job_tbl[i].pass_ms = 0;
          break;
        case JOB_PERIOD_1000ms:
          if (job_tbl[i].pass_ms <1001)
            break;
          (*job_tbl[i].func_callback)();
          job_tbl[i].pass_ms = 0;
          break;
      }
    }
  }
}
;

#endif