#ifndef __FOC_DRIVER_H__
#define __FOC_DRIVER_H__

#include "tim.h"
#include "foc_calculate.h"

#define PWM_ARR()	__HAL_TIM_GET_AUTORELOAD(&htim1) 

#define set_dtc_a(value) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_1,value)
#define set_dtc_b(value) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_2,value)
#define set_dtc_c(value) __HAL_TIM_SET_COMPARE(&htim1,TIM_CHANNEL_3,value)

#define rse     0.005f
#define iop 		20u
#define iratio	((3.3f / 4095.0f) / rse / iop)

#define vratio  (11.0f * 3.3f / 4095.0f)     				

typedef enum
{
	motor_stop,			//���ֹͣ
	motor_start,		//�����ʼ��
	motor_wait,			//����ȴ�
	motor_run,			//�������
	motor_precharge,//������
}motor_state_e;

typedef enum
{
	drag_mode         = 0,	//ǿ��ģʽ
	volt_mode					= 1,  //��ѹģʽ
	torque_mode				= 2,  //����ģʽ
	speed_mode  			= 3,	//ת��ģʽ
	angel_mode				= 4,  //�Ƕ�ģʽ
	mec_enc_cali_mode	= 5,	//�ű�����У׼ģʽ
	hall_cali_mode		= 6,	//����У׼ģʽ
}foc_ctrl_mode_t;


typedef struct
{
	motor_state_e state;
	foc_ctrl_mode_t foc_ctrl_mode;
	foc_para_t foc;
	
	uint8_t start_time;
	uint8_t stop_time;
	
	uint8_t pairs;
	
	float theta_acc;
	
	float openloop_vd;
	float openloop_vq;
	
	float inductance;
	float resistance;
	
	float i_kp;
	float i_ki;
	
}motor_ctrl_t; 

extern motor_ctrl_t motor;

void motor_foc_para_init(void);
void foc_pwm_start(void);
void foc_pwm_stop(void);
void foc_pwm_run(foc_para_t *foc);

#endif /* __FOC_DRIVER_H__ */
