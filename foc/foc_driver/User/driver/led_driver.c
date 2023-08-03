#include "led_driver.h"
#include "led_bsp.h"

#define is_led(v)    (v & led_all)
/**
***********************************************************************
* @brief:      led_driver_init(void)
* @param:		   void
* @retval:     void
* @details:    led ������ʼ����Ĭ������led������
***********************************************************************
**/
void led_driver_init(void)
{
	led_bsp_init();
	led_off(led_all);
}
/**
***********************************************************************
* @brief:      led_get_color(led_color_t led)
* @param:		   led��led��ɫ��־
* @retval:     led��ɫ��־
* @details:    ��ȡled����ɫ��־���Ծ���������һ��
***********************************************************************
**/
led_bsp_t *led_get_color(led_color_t led)
{
	static led_bsp_t ret;
	
	switch(led)
	{
		case led_net_red:
				ret.port = GPIOC;
				ret.pin = GPIO_PIN_14;
				break;
		case led_net_blue:
				ret.port = GPIOC;
				ret.pin = GPIO_PIN_13;
				break;
		default:
				break;
	}
	return &ret;
}
/**
***********************************************************************
* @brief:      led_on(led_color_t leds)
* @param:		   leds��led��ɫ��־
* @retval:     void
* @details:    ������Ӧ��ɫ��led
***********************************************************************
**/
void led_on(led_color_t leds)
{
	led_bsp_t *led;

	for(uint16_t i=0; i<16; i++)
	{
		if(!is_led(leds & (1 << i))) continue;
		led = led_get_color((led_color_t)(1 << i));
		led_bsp_on(led->port, led->pin);
	}
}
/**
***********************************************************************
* @brief:      led_off(led_color_t leds)
* @param:		   leds��led��ɫ��־
* @retval:     void
* @details:    �رն�Ӧ��ɫ��led
***********************************************************************
**/
void led_off(led_color_t leds)
{
	led_bsp_t *led;

	for(uint16_t i=0; i<16; i++)
	{
		if(!is_led(leds & (1 << i))) continue;
		led = led_get_color((led_color_t)(1 << i));
		led_bsp_off(led->port, led->pin);
	}
}
/**
***********************************************************************
* @brief:      led_toggle(led_color_t leds)
* @param:		   leds��led��ɫ��־
* @retval:     void
* @details:    ��ת��Ӧ��ɫ��led
***********************************************************************
**/
void led_toggle(led_color_t leds)
{
	led_bsp_t *led;
	
	for(uint16_t i=0; i<16; i++)
	{
		if(!is_led(leds & (1 << i))) continue;
		led = led_get_color((led_color_t)(1 << i));
		led_bsp_toggle(led->port, led->pin);
	}
}

