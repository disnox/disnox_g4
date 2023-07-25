#include "key_driver.h"
#include "key_bsp.h"

key_driver_t key_array[key_num];

/**
***********************************************************************
* @brief:      key_driver_init(void)
* @param:		   void
* @retval:     void
* @details:    ����������ʼ��
***********************************************************************
**/
void key_driver_init(void)
{
	key_bsp_init();
	key_array_init();
}
/**
***********************************************************************
* @brief:      key_check(void)
* @param:		   void
* @retval:     void
* @details:    �жϰ����Ƿ񱻰���
***********************************************************************
**/
void key_check(void)
{
	key_driver_t *key_p;
	key_p = &key_array[key_sw1];
	hold_filter(&key_p->bsp_hold_filter, key_1); // ��Ӳ�������İ���״̬�˲�
//	key_p = &key_array[key_sw2];
//	hold_filter(&key_p->bsp_hold_filter, key_2); // ��Ӳ�������İ���״̬�˲�
//	key_p = &key_array[key_sw3];
//	hold_filter(&key_p->bsp_hold_filter, key_3); // ��Ӳ�������İ���״̬�˲�
//	key_p = &key_array[key_sw4];
//	hold_filter(&key_p->bsp_hold_filter, key_4); // ��Ӳ�������İ���״̬�˲�
//	key_p = &key_array[key_sw5];
//	hold_filter(&key_p->bsp_hold_filter, key_5); // ��Ӳ�������İ���״̬�˲�
//	key_p = &key_array[key_sw6];
//	hold_filter(&key_p->bsp_hold_filter, key_6); // ��Ӳ�������İ���״̬�˲�
}
/**
***********************************************************************
* @brief:      key_array_init(void)
* @param:		   void
* @retval:     void
* @details:    �����ṹ���ֵ����
***********************************************************************
**/
void key_array_init(void)
{
	key_driver_t *key_p;
	key_p = &key_array[key_sw1];
	key_p->bsp_hold_filter.hold_time = 10;
	key_p->bsp_hold_filter.time_point = 0;
	key_p->bsp_hold_filter.trigger_variable = 1;	// �ߵ�ƽ����
	key_p->bsp_hold_filter.result = 0;
	key_p->shield = 0;
	key_p->press_time = 0;
	key_p->release_time = 0;
	key_p->state_machine = key_release;
	key_p->key_value = key_none;

//	key_p = &key_array[key_sw2];
//	key_p->bsp_hold_filter.hold_time = 10;
//	key_p->bsp_hold_filter.time_point = 0;
//	key_p->bsp_hold_filter.trigger_variable = 0;
//	key_p->bsp_hold_filter.result = 0;
//	key_p->shield = 0;
//	key_p->press_time = 0;
//	key_p->release_time = 0;
//	key_p->state_machine = key_release;
//	key_p->key_value = key_none;

//	key_p = &key_array[key_sw3];
//	key_p->bsp_hold_filter.hold_time = 10;
//	key_p->bsp_hold_filter.time_point = 0;
//	key_p->bsp_hold_filter.trigger_variable = 0;
//	key_p->bsp_hold_filter.result = 0;
//	key_p->shield = 0;
//	key_p->press_time = 0;
//	key_p->release_time = 0;
//	key_p->state_machine = key_release;
//	key_p->key_value = key_none;
//	
//	key_p = &key_array[key_sw4];
//	key_p->bsp_hold_filter.hold_time = 10;
//	key_p->bsp_hold_filter.time_point = 0;
//	key_p->bsp_hold_filter.trigger_variable = 0;
//	key_p->bsp_hold_filter.result = 0;
//	key_p->shield = 0;
//	key_p->press_time = 0;
//	key_p->release_time = 0;
//	key_p->state_machine = key_release;
//	key_p->key_value = key_none;
//	
//	key_p = &key_array[key_sw5];
//	key_p->bsp_hold_filter.hold_time = 10;
//	key_p->bsp_hold_filter.time_point = 0;
//	key_p->bsp_hold_filter.trigger_variable = 0;
//	key_p->bsp_hold_filter.result = 0;
//	key_p->shield = 0;
//	key_p->press_time = 0;
//	key_p->release_time = 0;
//	key_p->state_machine = key_release;
//	key_p->key_value = key_none;

//	key_p = &key_array[key_sw6];
//	key_p->bsp_hold_filter.hold_time = 10;
//	key_p->bsp_hold_filter.time_point = 0;
//	key_p->bsp_hold_filter.trigger_variable = 0;
//	key_p->bsp_hold_filter.result = 0;
//	key_p->shield = 0;
//	key_p->press_time = 0;
//	key_p->release_time = 0;
//	key_p->state_machine = key_release;
//	key_p->key_value = key_none;
}
/**
***********************************************************************
* @brief:      hold_filter(hold_filter_t *filter_p, uint32_t variable)
* @param:		   filter_p: ���������ṹ����
* @param:		   variable: ����״̬��־
* @retval:     void
* @details:    �����������˲�
***********************************************************************
**/
uint8_t hold_filter(hold_filter_t *filter_p, uint32_t variable)
{
	uint8_t variable_state = 0;
	uint32_t pass_time, system_time_temp;
	
	system_time_temp = get_key_bsptick();
	pass_time = system_time_temp - filter_p->time_point;

	if (filter_p->trigger_variable == variable)
	{
		variable_state = 1;
	}

	if (variable_state == filter_p->last_variable_state) 
	{
		if (filter_p->last_variable_state) 
		{
			if (pass_time > filter_p->hold_time && filter_p->result == 0) 
			{
				filter_p->result = 1;
			}
		} 
		else 
		{
			if (pass_time > filter_p->hold_time && filter_p->result == 1) 
			{
				filter_p->result = 0;
			}
		}
	} 
	else 
	{
		if (pass_time >= filter_p->hold_time) 
		{
			filter_p->time_point = system_time_temp;
		} 
		else 
		{
			filter_p->time_point = system_time_temp - (filter_p->hold_time - pass_time);
		}
	}
	filter_p->last_variable_state = variable_state;
	
	return filter_p->result;
}
/**
***********************************************************************
* @brief:      key_process(void)
* @param:		   void
* @retval:     void
* @details:    ����������(�������̰���������˫��)
***********************************************************************
**/
void key_process(void)
{
	uint8_t i;
	key_driver_t *key_p;

	key_check();
	
	for (i = 0; i < key_num; i++) 
	{
		key_p = &key_array[i];
		key_p->key_value = key_none;
		if (get_key_bsptick() > key_p->shield && key_p->shield != key_alway_shield) 
		{
			switch (key_p->state_machine) 
			{
				case key_release:
				if (key_p->bsp_hold_filter.result)  // ��������
				{
					key_p->press_time = get_key_bsptick();
					key_p->state_machine = key_short_pressing;
				}
					break;
				
				case key_short_pressing:
					if (key_p->bsp_hold_filter.result)  // ��Ȼ����
					{
						if (get_key_bsptick() - key_p->press_time > long_press_time)  // ��ס��ʱ�����������涨ʱ��
						{
							key_p->press_time = get_key_bsptick();
							key_p->key_value = key_long_press; // ������Ч
							key_p->state_machine = key_long_pressing;
						}
					} 
					else // ����
					{ 
						key_p->release_time = get_key_bsptick();
						key_p->state_machine = key_click_one_wait_for_double;
					}
					break;
					
				case key_click_one_wait_for_double:
					if (get_key_bsptick() - key_p->press_time < click_one_wait_for_double_time)  // ��˫���ȴ�ʱ����
					{
						if (key_p->bsp_hold_filter.result)  // �ٴΰ���
						{
							key_p->key_value = key_click_double; // ˫����Ч
							key_p->state_machine = key_double_pressing;
						}
					} 
					else  // ��ʱ
					{
						key_p->key_value = key_click_one; // ������Ч
						key_p->state_machine = key_release;
					}
					break;

				case key_double_pressing: // ˫����δ����
					if (!key_p->bsp_hold_filter.result) 
					{
						key_p->state_machine = key_release;
					}
					break;

				case key_long_pressing:
					if (key_p->bsp_hold_filter.result)  // ��Ȼ����
					{	
						if ((get_key_bsptick() - key_p->press_time) % long_press_effective_interval_time == 0) 
						{
							key_p->key_value = key_long_press; // ������Ч
						}
					} 
					else  // ���ֺ�λ
					{	
						key_p->state_machine = key_release;
					}
					break;

				default:
					break;
			}
		} 
		else 
		{
			if (!key_p->bsp_hold_filter.result) 
			{
				key_p->state_machine = key_release;
			}
		}
	}
}
