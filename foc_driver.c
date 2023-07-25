#include "foc_driver.h"


/**
***********************************************************************
* @brief:    			clarke�任
* @param[in]:    	theta: ��Ƕ� 
* @param[out]:    Ialpha��Ibeta����������������������
* @retval:     		void
* @details:   		������ԳƵ���ʸ��Ia��Ib��Ic�ֱ��ڦ�������ͶӰ
***********************************************************************
**/
void sin_cos_val(foc_para_t *foc, float *sin_val, float *cos_val)
{
	*sin_val = sinf(foc->theta); 
	*cos_val = cosf(foc->theta); 
}
/**
***********************************************************************
* @brief:    			clarke�任
* @param[in]:    	Ia��Ib��Ic��������� 
* @param[out]:    Ialpha��Ibeta����������������������
* @retval:     		void
* @details:   		������ԳƵ���ʸ��Ia��Ib��Ic�ֱ��ڦ�������ͶӰ
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
* @brief    	park�任
* @param    	Ialpha��Ibeta����������������������
* @param    	Theta����Ƕ�
* @param    	Id��Iq��dq������� 
* @return     void
* describe:   �������� �����ྲֹ����ϵ�任Ϊ��ų���ת��d��q����ϵ
************************************************************************
*/
void park_transform(foc_para_t *foc, float sin_val, float cos_val)
{
	foc->i_d = foc->i_alpha * cos_val + foc->i_beta * sin_val;
	foc->i_q = foc->i_alpha * sin_val + foc->i_beta * cos_val;
}
/**
**********************************************************************
* @brief    	park���任
* @param    	mod_d��mod_q��dq�����ѹ
* @param    	Theta����Ƕ�
* @param    	mod_alpha��mod_beta��dq������� 
* @return     void
* @describe:   ��dq�����ѹ�ڦ�������ͶӰ
************************************************************************
*/
void inverse_park(foc_para_t *foc, float sin_val, float cos_val)
{
	foc->v_alpha = (foc->v_d * cos_val - foc->v_q * sin_val);
	foc->v_beta  = (foc->v_d * sin_val + foc->v_q * cos_val);
}
/*
************************************************************************
* @brief    	svpwm�ռ�ʸ���������
* @param    	alpha��beta��ѹ 
* @return     void
* describe:   �е�ƽ���㷨�����ж�����������alpha_beta��ѹ��������ռ�ձ�
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

