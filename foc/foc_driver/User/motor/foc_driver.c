#include "foc_driver.h"
#include "foc_control.h"

void motor_foc_para_init(void)
{
	motor.theta_acc = 0.0008f;
	motor.openloop_vd = 0.0f;
	motor.openloop_vq = 0.1f;
}


void foc_pwm_start(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1); 
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);   
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);    
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim1,TIM_CHANNEL_4);
}

void foc_pwm_stop(void)
{
	set_dtc_a(PWM_ARR());
	set_dtc_b(PWM_ARR());
	set_dtc_c(PWM_ARR());
}

void foc_pwm_run(foc_para_t *foc)
{
	set_dtc_a((uint16_t)(PWM_ARR()*foc->dtc_a));
	set_dtc_b((uint16_t)(PWM_ARR()*foc->dtc_b));
	set_dtc_c((uint16_t)(PWM_ARR()*foc->dtc_c));
}



