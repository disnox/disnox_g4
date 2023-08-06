#include "calibration.h"
#include "foc_control.h"
#include "foc_driver.h"
#include "drivers.h"


 
mec_enc_cali_t mec_enc_cali;   

float start_count;
int32_t diff;
uint16_t wait_max = 2000;
void mec_encoder_cali(mec_enc_para_t *encoder)
{
	switch (mec_enc_cali.step)
  {
  	case cs_dir_pp_start:
			start_count = (float)encoder->acc_count;
			mec_enc_cali.step = cs_dir_pp_loop;
  		break;
		
  	case cs_dir_pp_loop:
			motor.foc.theta += motor.theta_acc;
			motor.foc.v_d = 0.5f;
			motor.foc.v_q = 0.0f;
			if (motor.foc.theta >= 4.0f * M_2PI) 
				mec_enc_cali.step = cs_dir_pp_end;
  		break;
		
		case cs_dir_pp_end:
			diff = encoder->acc_count - start_count;
      // Check direction
			if (diff > 0) 
					encoder->dir = +1;
			else
					encoder->dir = -1;
			// Motor pole pairs
			motor.pairs = round(ABS(ENCODER_CPR_F/(diff / 4.0f)));
			
			mec_enc_cali.step = cs_encoder_start;
  		break;
			
		case cs_encoder_start: 
			if(++mec_enc_cali.drag_cnt > wait_max)//等待
			{
				mec_enc_cali.offset = 0;//电角度偏置清零
				mec_enc_cali.zero = 0.0f;
				mec_enc_cali.zero_cw = 0.0f;
				mec_enc_cali.zero_ccw = 0.0f;
				mec_enc_cali.drag_cnt = 0;
				mec_enc_cali.step = cs_encoder_cw_loop;
			}
			break;
		
		case cs_encoder_cw_loop: 
			motor.foc.theta += motor.theta_acc;
			if(motor.foc.theta > M_2PI)
			{
				motor.foc.theta -= M_2PI;
				if(mec_enc_cali.zero_cw)
					mec_enc_cali.zero_cw += 0.2f * (encoder->count - mec_enc_cali.zero_cw);
				else
					mec_enc_cali.zero_cw = encoder->count;
				
				mec_enc_cali.drag_cnt++;
			}
			
			mec_enc_cali.cycle = motor.pairs;
			
			if(mec_enc_cali.drag_cnt > mec_enc_cali.cycle)
			{
				mec_enc_cali.drag_cnt = 0;
				mec_enc_cali.step = cs_encoder_wait;
			}
			break;
		
		case cs_encoder_wait: 
			if(++mec_enc_cali.drag_cnt > wait_max)//等待
			{
				mec_enc_cali.drag_cnt = 0;
				mec_enc_cali.step = cs_encoder_ccw_loop;
			}
			break;
		
		case cs_encoder_ccw_loop: 
			motor.foc.theta -= motor.theta_acc;
			if(motor.foc.theta < 0)
			{
				motor.foc.theta += M_2PI;
				if(mec_enc_cali.zero_ccw)
					mec_enc_cali.zero_ccw += 0.2f * (encoder->count - mec_enc_cali.zero_ccw);
				else
					mec_enc_cali.zero_ccw = encoder->count;
				
				mec_enc_cali.drag_cnt++;
			}
			if(mec_enc_cali.drag_cnt > mec_enc_cali.cycle)
			{
				mec_enc_cali.drag_cnt = 0;
				mec_enc_cali.step = cs_encoder_end;
			}
			break;
		
		case cs_encoder_end: 
			mec_enc_cali.offset = (mec_enc_cali.zero_cw + mec_enc_cali.zero_ccw) * 0.5f;
			encoder->offset = mec_enc_cali.offset;
			break;
		
  	default:
  		break;
  }


}

