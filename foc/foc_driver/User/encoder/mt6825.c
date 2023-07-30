#include "mt6825.h"
#include "main.h"
#include "spi.h"
#include "math.h"

tEncoder Encoder;

void Encoder_init(void)
{
		// Init
	Encoder.count_in_cpr_ = 0;
	Encoder.shadow_count_ = 0;
	Encoder.pos_estimate_counts_ = 0.0f;
	Encoder.vel_estimate_counts_ = 0.0f;
	Encoder.pos_cpr_counts_ = 0.0f;
	
	Encoder.pos_estimate_ = 0.0f;
	Encoder.vel_estimate_ = 0.0f;
	Encoder.pos_cpr_ = 0.0f;
	
	Encoder.phase_ = 0.0f;
	Encoder.interpolation_ = 0.0f;
	
	int encoder_pll_bandwidth = 2000;
	Encoder.pll_kp_ = 2.0f * encoder_pll_bandwidth;  				// basic conversion to discrete time
  Encoder.pll_ki_ = 0.25f * (Encoder.pll_kp_ * Encoder.pll_kp_); 	// Critically damped
}

uint32_t ENCODER_read_raw(void)
{

	uint8_t data_t[2];
	uint8_t data_r[4];
	uint32_t count;
	
	data_t[0] = 0x83;
	data_t[1] = 0xff;

	cs_down;
	HAL_SPI_Transmit(&hspi1,&data_t[0],1,1000);
	//HAL_SPI_TransmitReceive(&hspi3,&data_t[1],&data_r[3],1,1000);
	HAL_SPI_TransmitReceive(&hspi1,&data_t[1],&data_r[0],1,1000);
	HAL_SPI_TransmitReceive(&hspi1,&data_t[1],&data_r[1],1,1000);
	HAL_SPI_TransmitReceive(&hspi1,&data_t[1],&data_r[2],1,1000);

	cs_up;

	data_r[1] >>= 2;
	data_r[2] >>= 4;
	count = data_r[0] << 10 | data_r[1] << 4 | data_r[2] ;

	return count;
}


void ENCODER_sample(void)
{
    Encoder.raw = ENCODER_read_raw();
		Encoder.pos_abs_ =  Encoder.raw * (360.0f/ 262144.0f) * (M_PI / 180.0f);
}

// 归一化角度到 [0,2PI]
float _normalizeAngle(float angle){
  float a = fmod(angle, 2*M_PI);   //取余运算可以用于归一化，列出特殊值例子算便知
  return a >= 0 ? a : (a + 2*M_PI);  
}

int PP = 21,DIR = -1;
float _electricalAngle(void){
  return  _normalizeAngle((float)(DIR *  PP) * Encoder.pos_abs_);
}

