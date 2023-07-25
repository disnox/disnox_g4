#include "foc_control.h"
#include "foc_driver.h"
#include "drivers.h"
#include "vofa.h"

motor_ctrl_t motor;

void adc1_injected_callback(void)
{
	vofa_start();
	
	motor.foc.theta += motor.theta_acc;
	if(motor.foc.theta > M_2PI)
	{
		motor.foc.theta = 0.0f;	
	}
	else if(motor.foc.theta < 0)
	{
		motor.foc.theta = M_2PI;	
	}
	motor.foc.v_d = motor.openloop_vd;
	motor.foc.v_q = motor.openloop_vq;
	
	foc_calculate(&motor.foc);
}


