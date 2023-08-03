#include "mt6825.h"
#include "main.h"
#include "spi.h"
#include "math.h"

magnetic_encoder_para_t mt6825_encoder;

void encoder_init(void)
{
		// Init

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


void encoder_loop(magnetic_encoder_para_t *encoder)
{
	if(encoder->dir == +1)
		encoder->raw = encoder_read_raw();
	else
		encoder->raw = ENCODER_CPR - encoder_read_raw();
	
	/* 磁编码器线性化 */
	int off_1 = encoder->offset_lut[(encoder->raw) >> 11];                                        // lookup table lower entry
  int off_2 = encoder->offset_lut[((encoder->raw >> 11) + 1) % 128];                                // lookup table higher entry
  int off_interp = off_1 + ((off_2 - off_1) * (encoder->raw - ((encoder->raw >> 11) << 11)) >> 11);     // Interpolate between lookup table entries
  int count = encoder->raw - off_interp;
	
	/*  控制计数值在编码器的单周期内 */
  while (count > ENCODER_CPR) 
		count -= ENCODER_CPR;
  while (count < 0) 
		count += ENCODER_CPR;
  encoder->count = count;

	encoder->delta_count = encoder->count - encoder->count_prev;
	encoder->count_prev = encoder->count;
	while (encoder->delta_count > +ENCODER_CPR_DIV) 
		encoder->delta_count -= ENCODER_CPR;
	while (encoder->delta_count < -ENCODER_CPR_DIV) 
		encoder->delta_count += ENCODER_CPR;
	
	encoder->acc_count += encoder->delta_count;
}

// 归一化角度到 [0,2PI]
//float _normalizeAngle(float angle){
//  float a = fmod(angle, 2*PI);   //取余运算可以用于归一化，列出特殊值例子算便知
//  return a >= 0 ? a : (a + 2*PI);  
//}Encoder.pos_abs_ =  Encoder.raw * (360.0f/ 262144.0f) * (PI / 180.0f);

//int PP = 21,DIR = -1;
//float _electricalAngle(void){
//  return  _normalizeAngle((float)(DIR *  PP) * Encoder.pos_abs_);
//}

