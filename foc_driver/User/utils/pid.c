#include "pid.h"


float pi_ctrl(pid_para_t *pi, float ref_value, float fback_value)
{
	pi->ref_value = ref_value;
	pi->fback_value = fback_value;
	
	pi->error = pi->ref_value - pi->fback_value;
	
	// 计算比例项
	pi->p_term = pi->kp * pi->error;
	
	// 计算积分项
	pi->i_term += pi->ki * pi->error;

	// 限制积分项范围
	if(pi->i_term > pi->i_term_max)
		pi->i_term = pi->i_term_max;
	if(pi->i_term < pi->i_term_min)
		pi->i_term = pi->i_term_min;
	
	// 计算控制器输出（P、I项）
	pi->out_value = pi->p_term + pi->i_term;
	
	// 限制输出范围
	if(pi->out_value > pi->out_max)
		pi->out_value = pi->out_max;
	if(pi->out_value < pi->out_min)
		pi->out_value = pi->out_min;
	
	return pi->out_value;
}

float pid_ctrl(pid_para_t *pid, float ref_value, float fdback_value) 
{
	pid->ref_value = ref_value;
	pid->fback_value = fdback_value;
	
	pid->error = pid->ref_value - pid->fback_value;
	
	// 计算比例项
	pid->p_term = pid->kp * pid->error;
	
	// 计算积分项
	pid->i_term += pid->ki * pid->error;

	// 限制积分项范围
	if (pid->i_term > pid->i_term_max)
		pid->i_term = pid->i_term_max;
	else if (pid->i_term < pid->i_term_min)
		pid->i_term = pid->i_term_min;
	
	// 计算微分项
	pid->d_term = pid->kd * (pid->error - pid->pre_err);
	pid->pre_err = pid->error;
	
	// 计算控制器输出（P、I和D项）
	pid->out_value = pid->p_term + pid->i_term + pid->d_term;
	
	// 限制输出范围
	if (pid->out_value > pid->out_max)
		pid->out_value = pid->out_max;
	else if (pid->out_value < pid->out_min)
		pid->out_value = pid->out_min;
}

