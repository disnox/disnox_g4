#include "adc_driver.h"
#include "adc_bsp.h"

void adc_driver_init(void)
{
	adc_bsp_init();
}

/* 中值滤波 */
uint16_t adc1_median_filter(uint8_t channel)
{
	 uint16_t i, j, temp;
   for (j=0; j<adc1_samples-1; j++)
   {
      for (i=1; i<adc1_samples-j; i++)
      {
         if ( adc1_dma_value[i][channel]>adc1_dma_value[i-1][channel] )
         {
            temp = adc1_dma_value[i][channel];
            adc1_dma_value[i][channel] = adc1_dma_value[i-1][channel];
            adc1_dma_value[i-1][channel] = temp;
         }
      }
   }
	 return adc1_dma_value[(adc1_samples-1)/2][channel];
}

/* 均值滤波 */
uint16_t adc1_avg_filter(uint8_t channel)
{
	 uint16_t i, temp;
   for (i=0; i<adc1_samples; i++)
   {
			temp += adc1_dma_value[i][channel];
   }
	 return temp/adc1_samples;
}

/* 中值滤波 */
uint16_t adc2_median_filter(uint8_t channel)
{
	 uint16_t i,j,temp;
   for (j=0; j<adc2_samples-1; j++)
   {
      for (i=1; i<adc2_samples-j; i++)
      {
         if ( adc2_dma_value[i][channel]>adc2_dma_value[i-1][channel] )
         {
            temp = adc2_dma_value[i][channel];
            adc2_dma_value[i][channel] = adc2_dma_value[i-1][channel];
            adc2_dma_value[i-1][channel] = temp;
         }
      }
   }
	 return adc2_dma_value[(adc2_samples-1)/2][channel];
}

/* 均值滤波 */
uint16_t adc2_avg_filter(uint8_t channel)
{
	 uint16_t i, temp;
   for (i=0; i<adc2_samples; i++)
   {
			temp += adc2_dma_value[i][channel];
   }
	 return temp/adc2_samples;
}


