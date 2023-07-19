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
  State curState;//��ǰ״̬
  EventID eventId;//�¼�ID
  State nextState;//�¸�״̬
  CallBack action;//�ص��������¼������󣬵��ö�Ӧ�Ļص�����
}StateTransform ; 


void action_callback(void *arg);
void event_happen(unsigned int event);
void fsm_loop(void);

#endif
