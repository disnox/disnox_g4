#ifndef __LED_DRIVER_H__
#define __LED_DRIVER_H__


#include "main.h"

/* led ��ɫ���� */
typedef enum
{
	led_pow_red     = 0x0001, // ��ɫ��ԴLED
	led_pow_blue    = 0x0002, // ��ɫ��ԴLED
	led_pow_yellow  = 0x0003, // ��ɫ��ԴLED
	led_gps_red     = 0x0010, // ��ɫGPS LED
	led_gps_blue    = 0x0020, // ��ɫGPS LED
	led_gps_yellow  = 0x0030, // ��ɫGPS LED
	led_rf_red      = 0x0100, // ��ɫ��Ƶ LED
	led_rf_blue     = 0x0200, // ��ɫ��Ƶ LED
	led_rf_yellow   = 0x0300, // ��ɫ��Ƶ LED
	led_net_red     = 0x1000, // ��ɫ���� LED
	led_net_blue    = 0x2000, // ��ɫ���� LED
	led_net_yellow  = 0x3000, // ��ɫ���� LED
	led_all_red     = 0x1111, // ���к�ɫ LED
	led_all_blue    = 0x2222, // ������ɫ LED
	led_all         = 0x3333  // ���� LED
} led_color_t;

void led_driver_init(void);
void led_on(led_color_t leds);
void led_off(led_color_t leds);
void led_toggle(led_color_t leds);

#endif /* __LED_DRIVER_H__ */


