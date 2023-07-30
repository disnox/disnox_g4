#ifndef __FOC_CALCULATE_H__
#define __FOC_CALCULATE_H__

#include "math.h"
#include "tim.h"

// 定义FOC算法所需的常数
#define M_PI					(3.14159265358f)
#define M_2PI					(6.28318530716f)
#define SQRT3 					(1.73205080757f)
#define SQRT3_BY_2				(0.86602540378f)
#define ONE_BY_SQRT3			(0.57735026919f)
#define TWO_BY_SQRT3			(1.15470053838f)

#define SQ(x) 					((x) * (x))
#define ABS(x) 					((x)>0?(x):-(x)) 

#define min(x,y) 				(((x)<(y))?(x):(y))
#define max(x,y) 				(((x)>(y))?(x):(y))

// 定义FOC算法所需的数据结构
typedef struct
{
	float theta;         // 电机转子角度
	float sin_val, cos_val;  // 电机转子角度的sin和cos值
	
	// 三相电流和dq坐标系电流
	float i_a, i_b, i_c;
	float i_d, i_q;
	float i_alpha, i_beta;
	
	// 三相电压和dq坐标系电压
	float v_a, v_b, v_c;
	float v_d, v_q;
	float v_alpha, v_beta;
	
	// 用于SVPWM控制的PWM占空比
	float dtc_a, dtc_b, dtc_c;
	
	// 上一次电流采样值（用于电流offset计算）
	uint16_t ia_last, ib_last, ic_last;
	float ia_offset, ib_offset, ic_offset;
}foc_para_t;	


void sin_cos_val(foc_para_t *foc);
void clarke_transform(foc_para_t *foc);
void inverse_clarke(foc_para_t *foc);
void park_transform(foc_para_t *foc);
void inverse_park(foc_para_t *foc);
void svpwm_midpoint(foc_para_t *foc);
void svpwm_sector(foc_para_t *foc);
void foc_calculate(foc_para_t *foc);

#endif /* __FOC_CALCULATE_H__ */
