#include "pwm_bsp.h"
#include "tim.h"


/**
***********************************************************************
* @brief:      tim_bsp_init(void)
* @param:		   void
* @retval:     void
* @details:    定时器硬件使能
***********************************************************************
**/
void tim_bsp_init(void)
{
	
}

void tim1_pwm_start(void)
{
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);   
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);   
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);    
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);
	
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1); 
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_3);
}

void tim1_pwm_ccr_set()
{
	set_dtc_a

}

