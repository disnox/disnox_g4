#ifndef __FSM_H__
#define __FSM_H__

#include "main.h"
#include "stdio.h"

typedef enum {
  sta_origin=0,
  sta_running,
  sta_owencpu,
  sta_sleep_int,
  sta_sleep_unint
}State;

typedef enum{
  evt_fork=0,
  evt_sched,
  evt_wait,
  evt_wait_unint,
  evt_wake_up,
  evt_wake, 
}EventID;

typedef void (*CallBack)(void*);

typedef struct {
  State curState;//当前状态
  EventID eventId;//事件ID
  State nextState;//下个状态
  CallBack action;//回调函数，事件发生后，调用对应的回调函数
}StateTransform ; 


void action_callback(void *arg);
void event_happen(unsigned int event);
void fsm_loop(void);

#endif
