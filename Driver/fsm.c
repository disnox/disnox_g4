#include "fsm.h"

uint8_t globalState;

const char* statename[] = {
  "sta_origin",
  "sta_running",
  "sta_owencpu",
  "sta_sleep_int",
  "sta_sleep_unint"
};

/*origin*/
StateTransform stateTran_0[]={
	{sta_origin,evt_fork,        sta_running,action_callback},
	{sta_origin,evt_sched,       sta_origin,NULL},
	{sta_origin,evt_wait,        sta_origin,NULL},
	{sta_origin,evt_wait_unint,  sta_origin,NULL},
	{sta_origin,evt_wake_up,     sta_origin,NULL},
	{sta_origin,evt_wake,        sta_origin,NULL},
}; 
 
/*running*/
StateTransform stateTran_1[]={
	{sta_running,evt_fork,        sta_running,NULL},
	{sta_running,evt_sched,       sta_owencpu,action_callback},
	{sta_running,evt_wait,        sta_running,NULL},
	{sta_running,evt_wait_unint,  sta_running,NULL},
	{sta_running,evt_wake_up,     sta_running,NULL},
	{sta_running,evt_wake,        sta_running,NULL},
}; 
/*owencpu*/
StateTransform stateTran_2[]={
	{sta_owencpu,evt_fork,        sta_owencpu,NULL},
	{sta_owencpu,evt_sched,       sta_owencpu,NULL},
	{sta_owencpu,evt_wait,        sta_sleep_int,action_callback},
	{sta_owencpu,evt_wait_unint,  sta_sleep_unint,action_callback},
	{sta_owencpu,evt_wake_up,     sta_owencpu,NULL},
	{sta_owencpu,evt_wake,        sta_owencpu,NULL},
}; 
 
/*sleep_int*/
StateTransform stateTran_3[]={
	{sta_sleep_int,evt_fork,        sta_sleep_int,NULL},
	{sta_sleep_int,evt_sched,       sta_sleep_int,NULL},
	{sta_sleep_int,evt_wait,        sta_sleep_int,NULL},
	{sta_sleep_int,evt_wait_unint,  sta_sleep_int,NULL},
	{sta_sleep_int,evt_wake_up,     sta_sleep_int,NULL},
	{sta_sleep_int,evt_wake,        sta_running,action_callback},
}; 
/*sleep_unint*/
StateTransform stateTran_4[]={
	{sta_sleep_unint,evt_fork,        sta_sleep_unint,NULL},
	{sta_sleep_unint,evt_sched,       sta_sleep_unint,NULL},
	{sta_sleep_unint,evt_wait,        sta_sleep_unint,NULL},
	{sta_sleep_unint,evt_wait_unint,  sta_sleep_unint,NULL},
	{sta_sleep_unint,evt_wake_up,     sta_running,action_callback},
	{sta_sleep_unint,evt_wake,        sta_sleep_unint,NULL},
}; 

void action_callback(void *arg)
{
	StateTransform *statTran = (StateTransform *)arg;

	if(statename[statTran->curState] == statename[statTran->nextState])
	{
		printf("invalid event,state not change\n");
	}else{
		printf("call back state from %s --> %s\n",
		statename[statTran->curState],
		statename[statTran->nextState]);
	}
}

#define STATETRANS(n)  (stateTran_##n)
/*change state & call callback()*/
void do_action(StateTransform *statTran)
{
	if(NULL == statTran)
	{
		perror("statTran is NULL\n");
		return;
	}
	//状态迁移
	globalState = statTran->nextState;
	if(statTran->action != NULL)
	{//调用回调函数
		statTran->action((void*)statTran);
	}else{
		printf("invalid event,state not change\n");
	}
}

void event_happen(unsigned int event)
{
	switch(globalState)
	{
		case sta_origin:
			do_action(&STATETRANS(0)[event]);
		break;
		case sta_running:
			do_action(&STATETRANS(1)[event]);
		break;
		case sta_owencpu:
			do_action(&STATETRANS(2)[event]); 
		break;
		case sta_sleep_int:
			do_action(&STATETRANS(3)[event]); 
		break;
		case sta_sleep_unint:
			do_action(&STATETRANS(4)[event]); 
		break;
		default:
		printf("state is invalid\n");
		break;
	}
}

void fsm_loop(void)
{
	 HAL_Delay(1000);
	 event_happen(evt_fork);
	 HAL_Delay(1000);
	 event_happen(evt_sched);
	 HAL_Delay(1000);
	 event_happen(evt_sched);
	 HAL_Delay(1000);
	 event_happen(evt_wait);
	 HAL_Delay(1000);
	 event_happen(evt_wake);
}

