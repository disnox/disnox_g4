#ifndef __PID_H__
#define __PID_H__

#include "main.h"

typedef struct
{
	volatile float kp;						// ����
	volatile float ki;						// ����
	volatile float kd;						// ΢��
	
	volatile float p_term;				// ������
	volatile float i_term;				// ������
	volatile float d_term;				// ΢����
	
	volatile float i_term_max;		// �����ۼ��޷�
	volatile float i_term_min;		// �����ۼ��޷�
	
	volatile float ctrl_period;		//��������
	
	volatile float ref_value;			// Ŀ��ֵ
	volatile float fback_value;		// ʵ��ֵ
	
	volatile float error;					// ���
	volatile float pre_err;				// ��һ�����
	
	volatile float out_min;				// ����޷�
	volatile float out_max;				// ����޷�
	
	volatile float out_value;
	
}pid_para_t;

void pid_para_init(pid_para_t *pid_config, float kp, float ki, float kd);
void pid_limit_init(pid_para_t *pid_config, float i_term_max, float i_term_min,float out_max, float out_min);
void pid_clear(pid_para_t *pid_clear);
void pid_reset(pid_para_t *pid_config, float kp, float ki, float kd);
float pi_ctrl(pid_para_t *pi, float ref_value, float fback_value);
float pid_ctrl(pid_para_t *pid, float ref_value, float fdback_value);
void pid_ctrl_demo(void);

#endif /* __PID_H__ */















