#include "foc_calculate.h"

float a, b, c;

void svm(float alpha, float beta, float* tA, float* tB, float* tC);

void foc_calculate(foc_para_t *foc)
{
	sin_cos_val(foc);
	clarke_transform(foc);
	park_transform(foc);
	inverse_park(foc);
//	svpwm_midpoint(foc);
	svpwm_sector(foc);
	svm(foc->v_alpha, foc->v_beta, &a, &b, &c);
}

void sin_cos_val(foc_para_t *foc)
{
	foc->sin_val = sinf(foc->theta); 
	foc->cos_val = cosf(foc->theta); 
}

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

void svpwm_sector(foc_para_t *foc)
{
	float ta, tb, tc;
	int sextant;
	
	float tx, ty;
	
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
