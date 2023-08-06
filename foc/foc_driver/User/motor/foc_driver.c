#include "foc_driver.h"
#include "foc_control.h"

motor_ctrl_t motor;


void motor_foc_para_init(void)
{
	motor.state = motor_start;
	motor.foc_ctrl_mode = drag_mode;
	
	motor.theta_acc = 0.005f;
	motor.openloop_vd = 0.0f;
	motor.openloop_vq = 0.5f;
	
	motor.pairs = 10;
	
	motor.foc.v_bus = 24.0f;
	
	motor.foc.dtc_a = 0.0f;
	motor.foc.dtc_b = 0.0f;
	motor.foc.dtc_c = 0.0f;
	
	motor.foc.ia_last = 0;
	motor.foc.ib_last = 0;
	motor.foc.ic_last = 0;
	
	motor.foc.ia_offset = 0.0f;
	motor.foc.ib_offset = 0.0f;
	motor.foc.ic_offset = 0.0f;
}


void foc_pwm_start(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1); 
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);   
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);    
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}

void foc_pwm_stop(void)
{
	set_dtc_a(PWM_ARR());
	set_dtc_b(PWM_ARR());
	set_dtc_c(PWM_ARR());
}

void foc_pwm_run(foc_para_t *foc)
{
	set_dtc_a((uint16_t)(foc->dtc_a * PWM_ARR()));
	set_dtc_b((uint16_t)(foc->dtc_b * PWM_ARR()));
	set_dtc_c((uint16_t)(foc->dtc_c * PWM_ARR()));
}




