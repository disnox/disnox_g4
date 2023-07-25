#include "foc_driver.h"


/**
***********************************************************************
* @brief:    			clarke变换
* @param[in]:    	theta: 电角度 
* @param[out]:    Ialpha、Ibeta：三相电流的两相坐标分量
* @retval:     		void
* @details:   		将三相对称电流矢量Ia、Ib、Ic分别在α、β轴投影
***********************************************************************
**/
void sin_cos_val(foc_para_t *foc, float *sin_val, float *cos_val)
{
	*sin_val = sinf(foc->theta); 
	*cos_val = cosf(foc->theta); 
}
/**
***********************************************************************
* @brief:    			clarke变换
* @param[in]:    	Ia、Ib、Ic：三相电流 
* @param[out]:    Ialpha、Ibeta：三相电流的两相坐标分量
* @retval:     		void
* @details:   		将三相对称电流矢量Ia、Ib、Ic分别在α、β轴投影
***********************************************************************
**/
void clarke_transform(foc_para_t *foc)
{
	foc->i_alpha = foc->i_a;
	foc->i_beta = (foc->i_b - foc->i_c) * ONE_BY_SQRT3;
}

void inverse_clarke(foc_para_t *foc)
{
	foc->v_a = foc->v_alpha;
	foc->v_b = -0.5f * foc->v_alpha + SQRT3_BY_2 * foc->v_beta;
	foc->v_c = -0.5f * foc->v_alpha - SQRT3_BY_2 * foc->v_beta;
}
/*
************************************************************************
* @brief    	park变换
* @param    	Ialpha、Ibeta：三相电流的两相坐标分量
* @param    	Theta：电角度
* @param    	Id、Iq：dq坐标分量 
* @return     void
* describe:   将α、β 的两相静止坐标系变换为随磁场旋转的d、q坐标系
************************************************************************
*/
void park_transform(foc_para_t *foc, float sin_val, float cos_val)
{
	foc->i_d = foc->i_alpha * cos_val + foc->i_beta * sin_val;
	foc->i_q = foc->i_alpha * sin_val + foc->i_beta * cos_val;
}
/**
**********************************************************************
* @brief    	park反变换
* @param    	mod_d、mod_q：dq坐标电压
* @param    	Theta：电角度
* @param    	mod_alpha、mod_beta：dq坐标分量 
* @return     void
* @describe:   将dq坐标电压在α、β轴投影
************************************************************************
*/
void inverse_park(foc_para_t *foc, float sin_val, float cos_val)
{
	foc->v_alpha = (foc->v_d * cos_val - foc->v_q * sin_val);
	foc->v_beta  = (foc->v_d * sin_val + foc->v_q * cos_val);
}
/*
************************************************************************
* @brief    	svpwm空间矢量脉宽调制
* @param    	alpha、beta电压 
* @return     void
* describe:   中点平移算法，不判断扇区，根据alpha_beta电压计算三相占空比
************************************************************************
*/
float value_tmp[3];
void svm(foc_para_t *foc)
{
  float Vmax = 0.0f, Vmin = 0.0f, Vcom;
  float Vectora,Vectorb,Vectorc;
	float T1,T2,T3;
	float Va_tmp ,Vb_tmp;	
	
	
  Va_tmp = foc->v_alpha * 0.5f;
  Vb_tmp = SQRT3_BY_2 * foc->v_beta;
  
  Vectora = foc->v_alpha;  //alpha
  Vectorb = -Va_tmp + Vb_tmp; //-0.5*alpha + sqrt(3)/2 * beta;
  Vectorc = -Va_tmp - Vb_tmp; //-0.5*alpha - sqrt(3)/2 * beta;

  if (Vectora > Vectorb) {
    Vmax = Vectora;
    Vmin = Vectorb;
  }
  else {
    Vmax = Vectorb;
    Vmin = Vectora;
  }

  if (Vectorc > Vmax) {
    Vmax = Vectorc;
  }
  else if (Vectorc < Vmin) {
    Vmin = Vectorc;
  }
  
  Vcom = (Vmax + Vmin) * 0.5f;

	T1 = (Vcom - Vectora);
  T2 = (Vcom - Vectorb);
  T3 = (Vcom - Vectorc);
	
	value_tmp[0] = T1 + 0.5f;
	value_tmp[1] = T2 + 0.5f;
	value_tmp[2] = T3 + 0.5f;
	TIM1->CCR1 = (value_tmp[0]*PWM_ARR);
	TIM1->CCR2 = (value_tmp[1]*PWM_ARR);
	TIM1->CCR3 = (value_tmp[2]*PWM_ARR);
}

void foc_calculate(foc_para_t *foc)
{
	float sin_val, cos_val;
	
	sin_cos_val(foc, &sin_val, &cos_val);
	clarke_transform(foc);
	park_transform(foc, sin_val, cos_val);
	inverse_park(foc, sin_val, cos_val);
	svm(foc);
}

