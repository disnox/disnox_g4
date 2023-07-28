#ifndef __PID_H__
#define __PID_H__


typedef struct
{
	volatile float kp;						// 比例
	volatile float ki;						// 积分
	volatile float kd;						// 微分
	
	volatile float p_term;				// 比例项
	volatile float i_term;				// 积分项
	volatile float d_term;				// 微分项
	
	volatile float i_term_max;		// 积分累加限幅
	volatile float i_term_min;		// 积分累加限幅
	
	volatile float ref_value;			// 目标值
	volatile float fback_value;		// 实际值
	
	volatile float error;					// 误差
	volatile float pre_err;				// 上一次误差
	
	volatile float out_min;				// 输出限幅
	volatile float out_max;				// 输出限幅
	
	volatile float out_value;

}pid_para_t;


#endif /* __PID_H__ */















