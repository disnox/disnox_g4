#ifndef __FOC_CONTROL_H__
#define __FOC_CONTROL_H__

#include "foc_calculate.h"

typedef struct
{
	float theta_acc;
	
	float openloop_vd;
	float openloop_vq;
	
	foc_para_t foc;
	
}motor_ctrl_t; 

extern motor_ctrl_t motor;




#endif /* __FOC_CONTROL_H__ */



