#include "mt6825.h"
#include "main.h"
#include "spi.h"
#include "math.h"

mec_enc_para_t mt6825_encoder;

void encoder_init(mec_enc_para_t *encoder)
{
	encoder->dir = -1; 
	encoder->raw = 0;
	encoder->count = 0;
	encoder->count_prev = 0;
	encoder->acc_count = 0;
	encoder->delta_count = 0;
	encoder->offset = 155807;
}

uint32_t encoder_read_raw(void)
{
	uint8_t data_t[2];
	uint8_t data_r[4];
	uint32_t count;
	
	data_t[0] = 0x83;
	data_t[1] = 0xff;

	cs_down;
	HAL_SPI_Transmit(&hspi1,&data_t[0],1,1000);
	HAL_SPI_TransmitReceive(&hspi1,&data_t[1],&data_r[0],1,1000);
	HAL_SPI_TransmitReceive(&hspi1,&data_t[1],&data_r[1],1,1000);
	HAL_SPI_TransmitReceive(&hspi1,&data_t[1],&data_r[2],1,1000);

	cs_up;

	data_r[1] >>= 2;
	data_r[2] >>= 4;
	count = data_r[0] << 10 | data_r[1] << 4 | data_r[2] ;

	return count;
}


void encoder_loop(mec_enc_para_t *encoder)
{
	int raw = encoder_read_raw();
	if(encoder->dir == +1)
		encoder->raw = raw;
	else
		encoder->raw = ENCODER_CPR - raw;
	
	/* 磁编码器线性化 */
	int off_1 = encoder->offset_lut[(encoder->raw) >> 11];                                        // lookup table lower entry
  int off_2 = encoder->offset_lut[((encoder->raw >> 11) + 1) % 128];                                // lookup table higher entry
  int off_interp = off_1 + ((off_2 - off_1) * (encoder->raw - ((encoder->raw >> 11) << 11)) >> 11);     // Interpolate between lookup table entries
  int count = encoder->raw - off_interp;
	
	/*  控制计数值在编码器的单周期内 */
  if (count > ENCODER_CPR) 
		count -= ENCODER_CPR;
  if (count < 0) 
		count += ENCODER_CPR;
  encoder->count = count;
	
	

	encoder->delta_count = encoder->count - encoder->count_prev;
	encoder->count_prev = encoder->count;
//	if (encoder->delta_count > +ENCODER_CPR_DIV) 
//		encoder->delta_count -= ENCODER_CPR;
//	if (encoder->delta_count < -ENCODER_CPR_DIV) 
//		encoder->delta_count += ENCODER_CPR;
	
	encoder->acc_count += encoder->delta_count;
}
int32_t cali_theta;
void encoder_read_theta(mec_enc_para_t *encoder, float pp)
{
	encoder_loop(encoder);
	/* 处理编码器数据获得机械角度 */
	cali_theta = encoder->count - encoder->offset;
	/*  控制计数值在编码器的单周期内 */
  if (cali_theta > ENCODER_CPR) 
		cali_theta -= ENCODER_CPR;
  if (cali_theta < 0) 
		cali_theta += ENCODER_CPR;
	
	encoder->mec_angle =  normalize_angle(cali_theta * (360.0f/ 262144.0f) * (PI / 180.0f));
	encoder->elec_angle = normalize_angle(pp * encoder->mec_angle);
}

// 归一化角度到 [0,2PI]
float normalize_angle(float angle)
{
  float a = fmod(angle, 2*PI);   //取余运算可以用于归一化
  return a >= 0 ? a : (a + 2*PI);  
}



