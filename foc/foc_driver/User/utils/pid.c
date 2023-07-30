#include "pid.h"

/**
***********************************************************************
* @brief:      pid_para_init()
* @param[in]:	 pid_config  ָ��pid_para_t�ṹ���ָ��
* @param[in]:	 kp  ��������
* @param[in]:	 ki  ��������
* @param[in]:	 kd  ΢������
* @retval:     void
* @details:    ��ʼ��PID���������� 
***********************************************************************
**/
void pid_para_init(pid_para_t *pid_config, float kp, float ki, float kd)
{
	pid_config->kp = kp;
	pid_config->ki = ki;
	pid_config->kd = kd;

	pid_config->p_term = 0.0f;
	pid_config->i_term = 0.0f;
	pid_config->d_term = 0.0f;
	pid_config->ref_value = 0.0f;
	pid_config->fback_value = 0.0f;
	pid_config->error = 0.0f;
	pid_config->pre_err = 0.0f;
	pid_config->out_value = 0.0f;	
}
/**
***********************************************************************
* @brief:      pid_limit_init
* @param[in]:	 pid_config  ָ��pid_para_t�ṹ���ָ��
* @param[in]:	 i_term_max  ����������
* @param[in]:	 i_term_min  ����������
* @param[in]:	 out_max  �������
* @param[in]:	 out_min  �������
* @retval:     void
* @details:    ����PID���������������������Ʒ�Χ 
***********************************************************************
**/
void pid_limit_init(pid_para_t *pid_config, float i_term_max, float i_term_min,float out_max, float out_min)
{
	pid_config->i_term_max = i_term_max;
	pid_config->i_term_min = i_term_min;
	pid_config->out_max = out_max;
	pid_config->out_min = out_min;
}
/**
***********************************************************************
* @brief:      pid_clear
* @param[in]:	 pid_clear  ָ��pid_para_t�ṹ���ָ��
* @retval:     void
* @details:    ���PID����������ʷ״̬ 
***********************************************************************
**/
void pid_clear(pid_para_t *pid_clear)
{
	pid_clear->p_term = 0.0f;
	pid_clear->i_term = 0.0f;
	pid_clear->d_term = 0.0f;
	pid_clear->error = 0.0f;
	pid_clear->pre_err = 0.0f;
	pid_clear->out_value = 0.0f;
}
/**
***********************************************************************
* @brief:      pid_reset()
* @param[in]:	 pid_config  ָ��pid_para_t�ṹ���ָ��
* @param[in]:	 kp  ��������
* @param[in]:	 ki  ��������
* @param[in]:	 kd  ΢������
* @retval:     void
* @details:    pid�����������޸� 
***********************************************************************
**/
void pid_reset(pid_para_t *pid_config, float kp, float ki, float kd)
{
	pid_config->kp = kp;
	pid_config->ki = ki;
	pid_config->kd = kd;
}
/**
***********************************************************************
* @brief:      pi_ctrl
* @param[in]:	 pi  ָ��pid_para_t�ṹ���ָ��
* @param[in]:	 ref_value  Ŀ��ֵ
* @param[in]:	 fback_value  ʵ��ֵ
* @retval:     float  PI�����������ֵ
* @details:    ʵ��PI�������ļ����߼� 
***********************************************************************
**/
float pi_ctrl(pid_para_t *pi, float ref_value, float fback_value)
{
	pi->ref_value = ref_value;
	pi->fback_value = fback_value;
	
	pi->error = pi->ref_value - pi->fback_value;
	
	// ���������
	pi->p_term = pi->kp * pi->error;
	
	// ���������
	pi->i_term += pi->ki * pi->error;

	// ���ƻ����Χ
	if(pi->i_term > pi->i_term_max)
		pi->i_term = pi->i_term_max;
	if(pi->i_term < pi->i_term_min)
		pi->i_term = pi->i_term_min;
	
	// ��������������P��I�
	pi->out_value = pi->p_term + pi->i_term;
	
	// ���������Χ
	if(pi->out_value > pi->out_max)
		pi->out_value = pi->out_max;
	if(pi->out_value < pi->out_min)
		pi->out_value = pi->out_min;
	
	return pi->out_value;
}
/**
***********************************************************************
* @brief:      pid_ctrl
* @param[in]:	 pid  ָ��pid_para_t�ṹ���ָ��
* @param[in]:	 ref_value  Ŀ��ֵ
* @param[in]:	 fback_value  ʵ��ֵ
* @retval:     float  PI�����������ֵ
* @details:    ʵ��PI�������ļ����߼� 
***********************************************************************
**/
float pid_ctrl(pid_para_t *pid, float ref_value, float fdback_value) 
{
	pid->ref_value = ref_value;
	pid->fback_value = fdback_value;
	
	pid->error = pid->ref_value - pid->fback_value;
	
	// ���������
	pid->p_term = pid->kp * pid->error;
	
	// ���������
	pid->i_term += pid->ki * pid->error;

	// ���ƻ����Χ
	if (pid->i_term > pid->i_term_max)
		pid->i_term = pid->i_term_max;
	else if (pid->i_term < pid->i_term_min)
		pid->i_term = pid->i_term_min;
	
	// ����΢����
	pid->d_term = pid->kd * (pid->error - pid->pre_err);
	pid->pre_err = pid->error;
	
	// ��������������P��I��D�
	pid->out_value = pid->p_term + pid->i_term + pid->d_term;
	
	// ���������Χ
	if (pid->out_value > pid->out_max)
		pid->out_value = pid->out_max;
	else if (pid->out_value < pid->out_min)
		pid->out_value = pid->out_min;
	
	return pid->out_value;
}
/**
***********************************************************************
* @brief:      pid_ctrl_demo
* @param[in]:	 void
* @retval:     void
* @details:    PID��������ģ�������Ժ��� 
***********************************************************************
**/
void pid_ctrl_demo(void)
{
	pid_para_t pid; // ����PID�����������ṹ��
	float ref_value = 20.0f; // Ŀ��ֵ�趨Ϊ50.0
	float fback_value = 0.0f; // ��ʼ����ֵ�趨Ϊ0.0
	float target_value = 0.0f; // PID�����������ֵ
	
	static uint8_t i;
	// ��ʼ��PID����������
	pid_para_init(&pid, 10.0f, 5.0f, 0.0f);

	// ����PID���������������������Ʒ�Χ
	pid_limit_init(&pid, 200.0f, -200.0f, 200.0f, -200.0f);

	while(1)
	{
		// ģ�ⷴ��ֵ�ڵ�ǰֵ�����ϼ�������������������������Χ��-1��1֮�䣩
		fback_value += ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

		// ʹ��PI�������������ֵ
		target_value = pi_ctrl(&pid, ref_value, fback_value);

		// �����ǰ������Ŀ��ֵ��ʵ��ֵ
		printf("Iteration %d: Target = %.2f, Feedback = %.2f, Output = %.2f\n", i + 1, ref_value, fback_value, target_value);
	}
}
