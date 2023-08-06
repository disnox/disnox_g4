#ifndef __CALIBRATION_H__
#define __CALIBRATION_H__

#include "main.h"
#include "mt6825.h"
#include "stdbool.h"

typedef enum
{
	cs_dir_pp_start,
	cs_dir_pp_loop,
	cs_dir_pp_end,

	cs_encoder_start,
	cs_encoder_cw_loop,
	cs_encoder_wait,
	cs_encoder_ccw_loop,
	cs_encoder_end,
	
	cs_null = 0,
	
}mec_enc_cali_e;


typedef struct
{
	mec_enc_cali_e step;
	float angle;
	float theta_acc;
	bool calib_flag;
	float zero;
	float zero_cw;
	float zero_ccw;
	
	uint32_t offset;
	
	uint8_t dragflag;
	uint16_t drag_cnt;
	uint8_t cycle;
}mec_enc_cali_t;









void mec_encoder_cali(mec_enc_para_t *encoder);

#endif /* __CALIBRATION_H__ */


