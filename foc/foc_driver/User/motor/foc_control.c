#include "foc_control.h"
#include "foc_driver.h"
#include "calibration.h"
#include "drivers.h"
#include "vofa.h"
#include "mt6825.h"

uint16_t adc1_offset;
void adc1_injected_callback(void)
{
	vofa_start(); 
	
	motor.foc.ia_last = ADC1->JDR1;
	motor.foc.ib_last = ADC1->JDR2;
	motor.foc.ic_last = ADC1->JDR3;
	
	if(adc1_offset < 2000)
	{
		adc1_offset++;
		motor.foc.ia_offset = motor.foc.ia_offset * 0.1f + motor.foc.ia_last * 0.9f;
		motor.foc.ib_offset = motor.foc.ib_offset * 0.1f + motor.foc.ib_last * 0.9f;
		motor.foc.ic_offset = motor.foc.ic_offset * 0.1f + motor.foc.ic_last * 0.9f;
	}
	else
	{
		motor.foc.i_a = (motor.foc.ia_last - motor.foc.ia_offset) * iratio;
		motor.foc.i_b = (motor.foc.ib_last - motor.foc.ib_offset) * iratio;
		motor.foc.i_c = (motor.foc.ic_last - motor.foc.ic_offset) * iratio;

		encoder_read_theta(&mt6825_encoder, motor.pairs);
		motor_ctrl();
	}
}


void motor_ctrl(void)
{
	switch (motor.state)
  {
  	case motor_start:
			foc_pwm_start();
			motor.start_time = 0;
			motor.state = motor_precharge;
  		break;
		
  	case motor_precharge:
			if(++motor.start_time > 10)
			{
				motor.start_time = 0;
				motor.state = motor_run;
			}
  		break;
		
		case motor_stop:
			if(++motor.stop_time > 5)
			{
				foc_pwm_stop();
				motor.stop_time = 0;
				motor.state = motor_wait;
			}
  		break;
		
		case motor_wait:
  		break;
			
		case motor_run:
			foc_ctrl_mode();
			foc_calculate(&motor.foc);
			foc_pwm_run(&motor.foc);
  		break;
		
  	default:
  		break;
  }
}

void foc_ctrl_mode(void)
{
	switch (motor.foc_ctrl_mode)
  {
  	case drag_mode:
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
  		break;
		
  	case volt_mode:
			motor.foc.theta = mt6825_encoder.elec_angle;
			motor.foc.v_d = motor.openloop_vd;
			motor.foc.v_q = motor.openloop_vq;
  		break;
		
		case torque_mode:

  		break;
		
		case speed_mode:
  		break;
			
		case angel_mode:

  		break;
		
		case mec_enc_cali_mode:
			mec_encoder_cali(&mt6825_encoder);
  		break;
		
  	default:
  		break;
  }
}









