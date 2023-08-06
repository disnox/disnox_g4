#include "foc_calculate.h"

/**
***********************************************************************
* @brief:      foc_calculate(foc_para_t *foc)
* @param[in]:  foc  FOC参数结构体指针
* @retval:     void
* @details:    FOC算法的主要函数，用于计算电机的电流和电压控制 
***********************************************************************
**/
void foc_calculate(foc_para_t *foc)
{
	sin_cos_val(foc);
	clarke_transform(foc);
	park_transform(foc);
	inverse_park(foc);
	svpwm_midpoint(foc);
//	svpwm_sector(foc);
}
/**
***********************************************************************
* @brief:      sin_cos_val(foc_para_t *foc)
* @param[in]:  foc  FOC参数结构体指针
* @retval:     void
* @details:    计算电角度theta对应的sin和cos值
***********************************************************************
**/
void sin_cos_val(foc_para_t *foc)
{
	foc->sin_val = sinf(foc->theta); 
	foc->cos_val = cosf(foc->theta); 
}
/**
***********************************************************************
* @brief:      clarke_transform(foc_para_t *foc)
* @param[in]:  foc  FOC参数结构体指针
* @retval:     void
* @details:    Clarke变换将三相电流转换为αβ坐标系下的电流 
***********************************************************************
**/
void clarke_transform(foc_para_t *foc)
{
	foc->i_alpha = foc->i_a;
	foc->i_beta = (foc->i_b - foc->i_c) * ONE_BY_SQRT3;
}
/**
***********************************************************************
* @brief:      void inverse_clarke(foc_para_t *foc)
* @param[in]:  foc  FOC参数结构体指针
* @retval:     void
* @details:    逆Clarke变换将αβ坐标系下的电压转换为三相电压 
***********************************************************************
**/
void inverse_clarke(foc_para_t *foc)
{
	foc->v_a = foc->v_alpha;
	foc->v_b = -0.5f * foc->v_alpha + SQRT3_BY_2 * foc->v_beta;
	foc->v_c = -0.5f * foc->v_alpha - SQRT3_BY_2 * foc->v_beta;
}
/**
***********************************************************************
* @brief:      park_transform(foc_para_t *foc)
* @param[in]:  foc  FOC参数结构体指针
* @retval:     void
* @details:    Park变换将αβ坐标系下的电流转换为dq坐标系下的电流 
***********************************************************************
**/
void park_transform(foc_para_t *foc)
{
	foc->i_d = foc->i_alpha * foc->cos_val + foc->i_beta * foc->sin_val;
	foc->i_q = foc->i_alpha * foc->sin_val + foc->i_beta * foc->cos_val;
}
/**
***********************************************************************
* @brief:      void inverse_park(foc_para_t *foc)
* @param[in]:  foc  FOC参数结构体指针
* @retval:     void
* @details:    逆Park变换将dq坐标系下的电压转换为αβ坐标系下的电压 
***********************************************************************
**/
void inverse_park(foc_para_t *foc)
{
	foc->v_alpha = (foc->v_d * foc->cos_val - foc->v_q * foc->sin_val);
	foc->v_beta  = (foc->v_d * foc->sin_val + foc->v_q * foc->cos_val);
}
/**
***********************************************************************
* @brief:      svpwm_midpoint(foc_para_t *foc)
* @param[in]:  foc  FOC参数结构体指针
* @retval:     void
* @details:    通过中点平移法生成SVPWM的电压向量 
***********************************************************************
**/
void svpwm_midpoint(foc_para_t *foc)
{ 
	foc->v_alpha = foc->v_alpha/foc->v_bus;
	foc->v_beta = foc->v_beta/foc->v_bus;
	
  float va = foc->v_alpha;  
  float vb = -0.5f * foc->v_alpha + SQRT3_BY_2 * foc->v_beta; 
  float vc = -0.5f * foc->v_alpha - SQRT3_BY_2 * foc->v_beta; 

	float vmax = max(max(va, vb), vc);
	float vmin = min(min(va, vb), vc);
  
  float vcom = (vmax + vmin) * 0.5f;
	
	foc->dtc_a = (va - vcom) + 0.5f;
	foc->dtc_b = (vb - vcom) + 0.5f;
	foc->dtc_c = (vc - vcom) + 0.5f;
}
/**
***********************************************************************
* @brief:      svpwm_sector(foc_para_t *foc)
* @param[in]:  foc  FOC参数结构体指针
* @retval:     void
* @details:    通过扇区切换法生成SVPWM的电压向量 
***********************************************************************
**/
void svpwm_sectr(foc_para_t *foc)
{
	float ta, tb, tc;
	int sextant;
	float tx, ty;
	
	foc->v_alpha = foc->v_alpha/foc->v_bus;
	foc->v_beta = foc->v_beta/foc->v_bus;
	
	float va = TWO_BY_SQRT3 * foc->v_beta;
	float vb = foc->v_alpha - ONE_BY_SQRT3 * foc->v_beta;
	float vc = -foc->v_alpha - ONE_BY_SQRT3 * foc->v_beta;

	if (foc->v_beta >= 0.0f) 
	{
		if (foc->v_alpha >= 0.0f) 
			sextant = (-vb > 0.0f) ? 2 : 1;
		else 
			sextant = (vc > 0.0f) ? 3 : 2;
	} 
	else 
	{
		if (foc->v_alpha >= 0.0f) 
			sextant = (vc > 0.0f) ? 5 : 6;
		else
			sextant = (-vb > 0.0f) ? 4 : 5;
	}

	switch (sextant) 
	{
		case 1: 
			tx = vb; ty = va;
		
			ta = (1.0f - tx - ty) * 0.5f;
			tb = ta + tx;
			tc = tb + ty;
		break;
		case 2: 
			tx = -vc; ty = -vb;
		
			tb = (1.0f - tx - ty) * 0.5f;
			ta = tb + ty;
			tc = ta + tx;
		break;
		case 3: 
			tx = va;  ty = vc;
		
			tb = (1.0f - tx - ty) * 0.5f;
			tc = tb + tx;
			ta = tc + ty;
		break;
		case 4:
			tx = -vb; ty = -va;
		
			tc = (1.0f - tx - ty) * 0.5f;
			tb = tc + ty;
			ta = tb + tx;
		break;
		case 5: 
			tx = vc;  ty = vb;
		
			tc = (1.0f - tx - ty) * 0.5f;
			ta = tc + tx;
			tb = ta + ty;
		break;
		case 6: 
			tx = -va; ty = -vc;
		
			ta = (1.0f - tx - ty) * 0.5f;
			tc = ta + ty;
			tb = tc + tx;
		break;
	}
	
	foc->dtc_a = ta;
	foc->dtc_b = tb;
	foc->dtc_c = tc;
}


#define     TS              	1.0f
#define 		udc 24.0f
#define     SQRT3_MULT_TS   (float)((float)TS * SQRT3 / udc)


void svpwm_sector(foc_para_t *foc)
{	
	float channel1, channel2, channel3;
	
	float va = foc->v_beta;
	float vb = (SQRT3 * foc->v_alpha - foc->v_beta) / 2.0f;
	float vc = (-SQRT3 * foc->v_alpha - foc->v_beta) / 2.0f;
	
	uint8_t a = (va > 0.0f) ? 1 : 0;
	uint8_t b = (vb > 0.0f) ? 1 : 0;
	uint8_t c = (vc > 0.0f) ? 1 : 0;
	
	uint8_t sextant = (c << 2) + (b << 1) + a;
	
	switch(sextant)
	{
		case 0:
		{
			channel1 = TS / 2;
			channel2 = TS / 2;
			channel3 = TS / 2;
		}break;

		case 3:
		{
			float t4 = SQRT3_MULT_TS * vb;
			float t6 = SQRT3_MULT_TS * va;
			float t0 = (TS - t4 - t6) / 2;
			
			channel1 = t4 + t6 + t0;
			channel2 = t6 + t0;
			channel3 = t0;
		}break;

		case 1:
		{
			float t2 = -SQRT3_MULT_TS * vb;
			float t6 = -SQRT3_MULT_TS * vc;
			float t0 = (TS - t2 - t6) / 2;
			
			channel1 = t6 + t0;
			channel2 = t2 + t6 + t0;
			channel3 = t0;
		}break;

		case 5:
		{
			float t2 = SQRT3_MULT_TS * va;
			float t3 = SQRT3_MULT_TS * vc;
			float t0 = (TS - t2 - t3) / 2;
			
			channel1 = t0;
			channel2 = t2 + t3 + t0;
			channel3 = t3 + t0;
		}break;

		case 4:
		{
			float t1 = -SQRT3_MULT_TS * va;
			float t3 = -SQRT3_MULT_TS * vb;
			float t0 = (TS - t1 - t3) / 2;
			
			channel1 = t0;
			channel2 = t3 + t0;
			channel3 = t1 + t3 + t0;
		}break;

		case 6:
		{
			float t1 = SQRT3_MULT_TS * vc;
			float t5 = SQRT3_MULT_TS * vb;
			float t0 = (TS - t1 - t5) / 2;
			
			channel1 = t5 + t0;
			channel2 = t0;
			channel3 = t1 + t5 + t0;
		}break;

		case 2:
		{
			float t4 = -SQRT3_MULT_TS * vc;
			float t5 = -SQRT3_MULT_TS * va;
			float t0 = (TS - t4 - t5) / 2;
			
			channel1 = t4 + t5 + t0;
			channel2 = t0;
			channel3 = t5 + t0;
		}break;

		default:
			break;
		}
    
    foc->dtc_a = channel1;
    foc->dtc_b = channel2;
    foc->dtc_c = channel3;
   
}


