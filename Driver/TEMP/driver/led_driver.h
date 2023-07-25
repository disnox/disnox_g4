#ifndef __LED_DRIVER_H__
#define __LED_DRIVER_H__


#include "main.h"

/* led 颜色定义 */
typedef enum
{
	led_pow_red     = 0x0001, // 红色电源LED
	led_pow_blue    = 0x0002, // 蓝色电源LED
	led_pow_yellow  = 0x0003, // 黄色电源LED
	led_gps_red     = 0x0010, // 红色GPS LED
	led_gps_blue    = 0x0020, // 蓝色GPS LED
	led_gps_yellow  = 0x0030, // 黄色GPS LED
	led_rf_red      = 0x0100, // 红色射频 LED
	led_rf_blue     = 0x0200, // 蓝色射频 LED
	led_rf_yellow   = 0x0300, // 黄色射频 LED
	led_net_red     = 0x1000, // 红色网络 LED
	led_net_blue    = 0x2000, // 蓝色网络 LED
	led_net_yellow  = 0x3000, // 黄色网络 LED
	led_all_red     = 0x1111, // 所有红色 LED
	led_all_blue    = 0x2222, // 所有蓝色 LED
	led_all         = 0x3333  // 所有 LED
} led_color_t;

void led_driver_init(void);
void led_on(led_color_t leds);
void led_off(led_color_t leds);
void led_toggle(led_color_t leds);

#endif /* __LED_DRIVER_H__ */


