#ifndef __FOC_DRIVER_H__
#define __FOC_DRIVER_H__

#include "tim.h"
#include "foc_calculate.h"

#define PWM_ARR()	__HAL_TIM_GET_AUTORELOAD(&htim1) 

#define set_dtc_a(value) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,value)
#define set_dtc_b(value) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,value)
#define set_dtc_c(value) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,value)


void motor_foc_para_init(void);
void foc_pwm_start(void);
void foc_pwm_stop(void);
void foc_pwm_run(foc_para_t *foc);

#endif /* __FOC_DRIVER_H__ */
