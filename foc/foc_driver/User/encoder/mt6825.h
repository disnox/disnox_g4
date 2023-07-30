#ifndef __MT6825_H__
#define __MT6825_H__
#include "main.h"

#define M_PI 3.1415926f 

#define cs_down HAL_GPIO_WritePin(SPI1_CSN_GPIO_Port,SPI1_CSN_Pin,GPIO_PIN_RESET);
#define cs_up HAL_GPIO_WritePin(SPI1_CSN_GPIO_Port,SPI1_CSN_Pin,GPIO_PIN_SET);

#define ENCODER_CPR			 262144
#define ENCODER_CPR_DIV_2	(ENCODER_CPR>>1)


typedef struct sEncoder {
	int raw;
	int cnt;
	float pos_abs_;
	int count_in_cpr_;
	int shadow_count_;
	float pos_estimate_counts_;
	float vel_estimate_counts_;
	float pos_cpr_counts_;
	
	float pos_estimate_;
    float vel_estimate_;
	float pos_cpr_;
	
	float phase_;
	float interpolation_;

	float pll_kp_;
	float pll_ki_;
} tEncoder;

extern tEncoder Encoder;


extern void RINE_MT6816_SPI_Get_AngleData(void);
extern void Encoder_init(void);
extern uint32_t ENCODER_read_raw(void);
extern void ENCODER_sample(void);
extern float _electricalAngle(void);
#endif /* __MT6825_H__ */
