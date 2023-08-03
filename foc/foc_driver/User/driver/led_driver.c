#include "led_driver.h"
#include "led_bsp.h"

#define is_led(v)    (v & led_all)
/**
***********************************************************************
* @brief:      led_driver_init(void)
* @param:		   void
* @retval:     void
* @details:    led 驱动初始化，默认所有led不点亮
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
* @param:		   led：led颜色标志
* @retval:     led颜色标志
* @details:    获取led的颜色标志，以决定点亮哪一个
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
* @param:		   leds：led颜色标志
* @retval:     void
* @details:    点亮对应颜色的led
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
* @param:		   leds：led颜色标志
* @retval:     void
* @details:    关闭对应颜色的led
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
* @param:		   leds：led颜色标志
* @retval:     void
* @details:    翻转对应颜色的led
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

