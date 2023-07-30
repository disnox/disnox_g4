#include "pid.h"

/**
***********************************************************************
* @brief:      pid_para_init()
* @param[in]:	 pid_config  指向pid_para_t结构体的指针
* @param[in]:	 kp  比例增益
* @param[in]:	 ki  积分增益
* @param[in]:	 kd  微分增益
* @retval:     void
* @details:    初始化PID控制器参数 
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
* @param[in]:	 pid_config  指向pid_para_t结构体的指针
* @param[in]:	 i_term_max  积分项上限
* @param[in]:	 i_term_min  积分项下限
* @param[in]:	 out_max  输出上限
* @param[in]:	 out_min  输出下限
* @retval:     void
* @details:    设置PID控制器积分项和输出的限制范围 
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
* @param[in]:	 pid_clear  指向pid_para_t结构体的指针
* @retval:     void
* @details:    清除PID控制器的历史状态 
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
* @param[in]:	 pid_config  指向pid_para_t结构体的指针
* @param[in]:	 kp  比例增益
* @param[in]:	 ki  积分增益
* @param[in]:	 kd  微分增益
* @retval:     void
* @details:    pid的三个参数修改 
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
* @param[in]:	 pi  指向pid_para_t结构体的指针
* @param[in]:	 ref_value  目标值
* @param[in]:	 fback_value  实际值
* @retval:     float  PI控制器的输出值
* @details:    实现PI控制器的计算逻辑 
***********************************************************************
**/
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
/**
***********************************************************************
* @brief:      pid_ctrl
* @param[in]:	 pid  指向pid_para_t结构体的指针
* @param[in]:	 ref_value  目标值
* @param[in]:	 fback_value  实际值
* @retval:     float  PI控制器的输出值
* @details:    实现PI控制器的计算逻辑 
***********************************************************************
**/
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
	
	return pid->out_value;
}
/**
***********************************************************************
* @brief:      pid_ctrl_demo
* @param[in]:	 void
* @retval:     void
* @details:    PID控制器的模拟仿真测试函数 
***********************************************************************
**/
void pid_ctrl_demo(void)
{
	pid_para_t pid; // 定义PID控制器参数结构体
	float ref_value = 20.0f; // 目标值设定为50.0
	float fback_value = 0.0f; // 初始反馈值设定为0.0
	float target_value = 0.0f; // PID控制器的输出值
	
	static uint8_t i;
	// 初始化PID控制器参数
	pid_para_init(&pid, 10.0f, 5.0f, 0.0f);

	// 设置PID控制器积分项和输出的限制范围
	pid_limit_init(&pid, 200.0f, -200.0f, 200.0f, -200.0f);

	while(1)
	{
		// 模拟反馈值在当前值基础上加入随机噪声（假设随机噪声范围在-1到1之间）
		fback_value += ((float)rand() / RAND_MAX) * 2.0f - 1.0f;

		// 使用PI控制器计算输出值
		target_value = pi_ctrl(&pid, ref_value, fback_value);

		// 输出当前迭代的目标值和实际值
		printf("Iteration %d: Target = %.2f, Feedback = %.2f, Output = %.2f\n", i + 1, ref_value, fback_value, target_value);
	}
}
