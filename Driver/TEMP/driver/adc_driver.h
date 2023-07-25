#ifndef __ADC_DRIVER_H__
#define __ADC_DRIVER_H__

#include "main.h"
#include "adc_bsp.h"

/* adc通道名字定义，根据自己的使用来命名 */
typedef enum
{
	adc1_ch1 = 0,
	adc1_ch2 = 1,
	adc1_ch3 = 2,
	adc1_ch4 = 3
} adc1_num;

typedef enum
{
	adc2_ch1 = 0,
	adc2_ch2 = 1,
	adc2_ch3 = 2
} adc2_num;

void adc_driver_init(void);

uint16_t adc1_median_filter(uint8_t channel);
uint16_t adc1_avg_filter(uint8_t channel);

uint16_t adc2_median_filter(uint8_t channel);
uint16_t adc2_avg_filter(uint8_t channel);


#endif /* __ADC_DRIVER_H__ */


