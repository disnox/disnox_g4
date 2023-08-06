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
	int raw;                          // �ű�������ԭʼ����ֵ
	int dir;                          // �ű���������ת����+1 ��ʾ˳ʱ�룬-1 ��ʾ��ʱ��
	int32_t count;                    // �ű������ĵ�ǰ����ֵ����һ�����������������ڵļ���ֵ��
	int32_t count_prev;               // ��һ�����������ڵļ���ֵ
	int32_t acc_count;                // �ۼƼ���ֵ�����б����������ڵ��ۻ�����ֵ��
	int32_t delta_count;              // �����������ڵ���������ֵ����������������֮��Ĳ�ֵ��

	int32_t offset_lut[128];          // ���ڴű��������Ի��Ĳ��ұ�
	uint32_t offset;
	
	float mec_angle;                  // ��е�Ƕȣ�������������ֵ�������Ի������õ��Ļ�е�Ƕ�ֵ
	float elec_angle;                 // ��Ƕȣ���Ӧ�ڻ�е�Ƕȵĵ��ת��λ�ã����ݵ���ļ���������õ���

} mec_enc_para_t;


extern mec_enc_para_t mt6825_encoder;


void encoder_init(mec_enc_para_t *encoder);
uint32_t encoder_read_raw(void);
void encoder_loop(mec_enc_para_t *encoder);
void encoder_read_theta(mec_enc_para_t *encoder, float pp);
float normalize_angle(float angle);

#endif /* __MT6825_H__ */
