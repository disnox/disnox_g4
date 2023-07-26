#include "foc_calculate.h"

void SVPWM_Generate(foc_para_t *foc);

void foc_calculate(foc_para_t *foc)
{
	sin_cos_val(foc);
	clarke_transform(foc);
	park_transform(foc);
	inverse_park(foc);
//	svpwm_midpoint(foc);
	svpwm_sector(foc);
//	SVPWM_Generate(foc);
}

void sin_cos_val(foc_para_t *foc)
{
	foc->sin_val = sinf(foc->theta); 
	foc->cos_val = cosf(foc->theta); 
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

void park_transform(foc_para_t *foc)
{
	foc->i_d = foc->i_alpha * foc->cos_val + foc->i_beta * foc->sin_val;
	foc->i_q = foc->i_alpha * foc->sin_val + foc->i_beta * foc->cos_val;
}

void inverse_park(foc_para_t *foc)
{
	foc->v_alpha = (foc->v_d * foc->cos_val - foc->v_q * foc->sin_val);
	foc->v_beta  = (foc->v_d * foc->sin_val + foc->v_q * foc->cos_val);
}

void svpwm_midpoint(foc_para_t *foc)
{ 
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
#define PWM 8500
void svpwm_sector(foc_para_t *foc) 
{
  int sector = 0;
	float Vref1,Vref2,Vref3;
	float Ta,Tb,Tc;
	float X,Y,Z;
  float T1;
  float T2;

  float Tcmp1 = 0.0f;
  float Tcmp2 = 0.0f ;
  float Tcmp3 = 0.0f;
	
	/* Parameters statement */
	Vref1 = foc->v_beta;
	Vref2 = (1.73205078f * foc->v_alpha - foc->v_beta) / 2.0f;
	Vref3 = (-1.73205078f * foc->v_alpha - foc->v_beta) / 2.0f;
	/* 计算扇区 */
  if (Vref1 > 0.0f) sector = 1;
  if (Vref2 > 0.0f) sector += 2;
  if (Vref3 > 0.0f) sector += 4;
	/* 计算X Y Z */
	X = 1.73205078f * foc->v_beta;
	Y = (1.5f * foc->v_alpha + 0.866025388f * foc->v_beta);
	Z = (-1.5f * foc->v_alpha + 0.866025388f * foc->v_beta);
	/* 占空比计算 */
  switch(sector) 
	{
   case 1:T1 =  Z;  T2 =  Y;  break;
	 case 2:T1 =  Y;  T2 = -X;  break;
   case 3:T1 = -Z;  T2 =  X;  break;
	 case 4:T1 = -X;  T2 =  Z;  break;
   case 5:T1 =  X;  T2 = -Y;  break;
   case 6:T1 = -Y;  T2 = -Z;  break; 
  }

//  if(T1 + T2 > PWM) 
//	{
//    T1 = T1/(T1 + T2);
//    T2 = T2/(T1 + T2);
//  }

  Ta = (1.0f - (T1 + T2)) / 4.0f;
  Tb = T1 / 2.0f +Ta;
  Tc = T2 / 2.0f +Tb;
	
  switch (sector) 
	{
   case 1:Tcmp1 = Tb; Tcmp2 = Ta; Tcmp3 = Tc;break;
   case 2:Tcmp1 = Ta; Tcmp2 = Tc; Tcmp3 = Tb;break;
   case 3:Tcmp1 = Ta; Tcmp2 = Tb; Tcmp3 = Tc;break;
   case 4:Tcmp1 = Tc; Tcmp2 = Tb; Tcmp3 = Ta;break;
   case 5:Tcmp1 = Tc; Tcmp2 = Ta; Tcmp3 = Tb;break;
   case 6:Tcmp1 = Tb; Tcmp2 = Tc; Tcmp3 = Ta;break;
  }
	
	foc->dtc_a = Tcmp1;
	foc->dtc_b = Tcmp2;
	foc->dtc_c = Tcmp3;
}

#define     TS              	8500

#define     LIMIT           	(float)(MAX_DUTY_CYCLE / SQRT3)

#define     SQRT3_MULT_TS   (float)((float)TS * SQRT3)
	static const uint8_t sectionMap[7] = {0, 2, 6, 1, 4, 3, 5};
void SVPWM_Generate(foc_para_t *foc)
{	
    float U1, U2, U3;
	float udc = 24.0f;
    uint8_t a, b, c, n = 0;
    uint16_t channel1 = 0, channel2 = 0, channel3 = 0;
    
    U1 = foc->v_beta;
    U2 = (SQRT3 * foc->v_alpha - foc->v_beta) / 2.0f;
    U3 = (-SQRT3 * foc->v_alpha - foc->v_beta) / 2.0f;
 
    if(U1 > 0.0f)
        a = 1;
    else 
        a = 0;
    if(U2 > 0.0f)
        b = 1;
    else 
        b = 0;
    if(U3 > 0.0f)
        c = 1;
    else 
        c = 0;
    
    n = (c << 2) + (b << 1) + a;
    
    switch(sectionMap[n])
    {
	case 0:
	{
		channel1 = TS / 2;
		channel2 = TS / 2;
		channel3 = TS / 2;
	}break;
	
	case 1:
	{
		int16_t t4 = SQRT3_MULT_TS * U2 / udc;
		int16_t t6 = SQRT3_MULT_TS * U1 / udc;
		int16_t t0 = (TS - t4 - t6) / 2;
		
		channel1 = t4 + t6 + t0;
		channel2 = t6 + t0;
		channel3 = t0;
	}break;
	
	case 2:
	{
		int16_t t2 = -SQRT3_MULT_TS * U2 / udc;
		int16_t t6 = -SQRT3_MULT_TS * U3 / udc;
		int16_t t0 = (TS - t2 - t6) / 2;
		
		channel1 = t6 + t0;
		channel2 = t2 + t6 + t0;
		channel3 = t0;
	}break;
	
	case 3:
	{
		int16_t t2 = SQRT3_MULT_TS * U1 / udc;
		int16_t t3 = SQRT3_MULT_TS * U3 / udc;
		int16_t t0 = (TS - t2 - t3) / 2;
		
		channel1 = t0;
		channel2 = t2 + t3 + t0;
		channel3 = t3 + t0;
	}break;
	
	case 4:
	{
		int16_t t1 = -SQRT3_MULT_TS * U1 / udc;
		int16_t t3 = -SQRT3_MULT_TS * U2 / udc;
		int16_t t0 = (TS - t1 - t3) / 2;
		
		channel1 = t0;
		channel2 = t3 + t0;
		channel3 = t1 + t3 + t0;
	}break;
	
	case 5:
	{
		int16_t t1 = SQRT3_MULT_TS * U3 / udc;
		int16_t t5 = SQRT3_MULT_TS * U2 / udc;
		int16_t t0 = (TS - t1 - t5) / 2;
		
		channel1 = t5 + t0;
		channel2 = t0;
		channel3 = t1 + t5 + t0;
	}break;
	
	case 6:
	{
		int16_t t4 = -SQRT3_MULT_TS * U3 / udc;
		int16_t t5 = -SQRT3_MULT_TS * U1 / udc;
		int16_t t0 = (TS - t4 - t5) / 2;
		
		channel1 = t4 + t5 + t0;
		channel2 = t0;
		channel3 = t5 + t0;
	}break;
	
	default:
		break;
    }

    if(channel1 > TS)
        channel1 = TS;
    if(channel2 > TS)
        channel2 = TS;
    if(channel3 > TS)
        channel3 = TS;
	
    foc->dtc_a = channel1;
    foc->dtc_b = channel2;
    foc->dtc_c = channel3;	
}

