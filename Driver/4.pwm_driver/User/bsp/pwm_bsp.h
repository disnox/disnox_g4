#ifndef __PWM_BSP_H__
#define __PWM_BSP_H__

#include "main.h"

#define TIM1_ARR()	__HAL_TIM_GET_AUTORELOAD(&htim1) 

#define set_dtc_a(value) __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, value)
#define set_dtc_b(value) __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, value)
#define set_dtc_c(value) __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, value)


void pwm_bsp_init(void);


#endif /* __PWM_BSP_H__ */

