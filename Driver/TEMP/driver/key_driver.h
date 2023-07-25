#ifndef __KEY_DRIVER_H__
#define __KEY_DRIVER_H__

#include "main.h"

/* 按键永久屏蔽值屏蔽 */
#define key_alway_shield 0xFFFFFFFF

/* 按键时长定义 */
#define click_one_wait_for_double_time 500       // 双击两下之间的最大间隔时间，单位ms
#define long_press_time 1600                     // 识别为长按的时间，单位ms
#define long_press_effective_interval_time 10000 // 长按时每次触发的间隔时间，单位ms

/* 按键个数 */
typedef enum {
	key_sw1,
	key_sw2,
	key_sw3,
	key_sw4,
	key_sw5,
	key_sw6,
	key_num,
} key_t;

/* 状态机 */
typedef enum 
{
	key_release,                       // 按键释放状态
	key_short_pressing,                // 按键短按状态
	key_double_pressing,               // 按键双击状态
	key_click_one_wait_for_double,     // 按键单击等待双击状态
	key_long_pressing,                 // 按键长按状态
	key_state_machine_num,             // 状态机数量，用于辅助计数
} key_state_machine_t;

/* 键值 */
typedef enum {
	key_none = key_state_machine_num,
	key_click_one,
	key_click_double,
	key_long_press,
} key_value_t;

/* 按键触发 */
typedef struct {
	uint32_t hold_time;             // 按键保持时间
	uint64_t time_point;            // 时间点
	uint32_t trigger_variable;      // 触发变量
	uint8_t last_variable_state;    // 上一次变量状态
	uint8_t result;                 // 结果
} hold_filter_t;

/* 按键驱动层 */
typedef struct {
	hold_filter_t bsp_hold_filter;    	// 硬件层的按键是否按下判断，滤波器
	uint32_t shield;                    // 屏蔽时间：0：完全屏蔽，1：按键使能，大于1：倒计时屏蔽
	uint32_t press_time;                // 按住开始时间
	uint32_t release_time;              // 释放开始时间
	key_state_machine_t state_machine;     // 按键当前持续状态（状态机）
	key_value_t key_value;             // 键值
} key_driver_t;

void key_driver_init(void);
void key_check(void);
void key_array_init(void);
void key_process(void);
uint8_t hold_filter(hold_filter_t *filter_p, uint32_t variable);

#endif /* __KEY_DRIVER_H__ */


