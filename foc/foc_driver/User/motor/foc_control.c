#include "foc_control.h"
#include "foc_driver.h"
#include "drivers.h"
#include "vofa.h"
#include "mt6825.h"

motor_ctrl_t motor;

void adc1_injected_callback(void)
{
	vofa_start(); 
	
	motor.foc.ia_last = ADC1->JDR1;
	motor.foc.ib_last = ADC1->JDR2;
	motor.foc.ic_last = ADC1->JDR3;
	
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
	foc_pwm_run(&motor.foc);
}


