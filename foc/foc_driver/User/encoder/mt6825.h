#ifndef __MT6825_H__
#define __MT6825_H__
#include "main.h"

#define PI 3.1415926f 

#define cs_down HAL_GPIO_WritePin(SPI1_CSN_GPIO_Port,SPI1_CSN_Pin,GPIO_PIN_RESET);
#define cs_up HAL_GPIO_WritePin(SPI1_CSN_GPIO_Port,SPI1_CSN_Pin,GPIO_PIN_SET);

#define ENCODER_CPR			 262144u
#define ENCODER_CPR_F		(262144.0f)
#define ENCODER_CPR_DIV	(ENCODER_CPR>>1)


typedef struct 
{
	int raw;                          // 磁编码器的原始计数值
	int dir;                          // 磁编码器的旋转方向，+1 表示顺时针，-1 表示逆时针
	int32_t count;                    // 磁编码器的当前计数值（在一个完整编码器周期内的计数值）
	int32_t count_prev;               // 上一个编码器周期的计数值
	int32_t acc_count;                // 累计计数值（所有编码器周期内的累积计数值）
	int32_t delta_count;              // 编码器周期内的增量计数值（两个编码器周期之间的差值）

	int32_t offset_lut[128];          // 用于磁编码器线性化的查找表
	uint32_t offset;
	
	float mec_angle;                  // 机械角度，即编码器计数值经过线性化处理后得到的机械角度值
	float elec_angle;                 // 电角度，对应于机械角度的电机转子位置（根据电机的极对数计算得到）

} mec_enc_para_t;


extern mec_enc_para_t mt6825_encoder;


void encoder_init(mec_enc_para_t *encoder);
uint32_t encoder_read_raw(void);
void encoder_loop(mec_enc_para_t *encoder);
void encoder_read_theta(mec_enc_para_t *encoder, float pp);
float normalize_angle(float angle);

#endif /* __MT6825_H__ */
