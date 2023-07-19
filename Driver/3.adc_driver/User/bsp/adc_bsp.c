#include "adc_bsp.h"
#include "adc.h"

uint16_t adc1_dma_value[adc1_samples][adc1_channel];
uint16_t adc2_dma_value[adc2_samples][adc2_channel];

void adc_bsp_init(void)
{
//	HAL_ADC_Start_DMA(&hadc1, (uint32_t *)adc1_dma_value, adc1_length);
//	HAL_ADC_Start_DMA(&hadc2, (uint32_t *)adc2_dma_value, adc2_length);
	
//	HAL_ADCEx_InjectedStart_IT(&hadc1);
//	HAL_ADCEx_InjectedStart_IT(&hadc2);
}

void HAL_ADCEx_InjectedConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc == &hadc1)
	{
		adc1_injected();
	}
	
	if(hadc == &hadc2)
	{
		adc2_injected();
	}
}

__weak void adc1_injected(void)
{

}

__weak void adc2_injected(void)
{

}


