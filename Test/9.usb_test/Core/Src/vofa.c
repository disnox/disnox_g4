#include "vofa.h"
#include "usbd_cdc_if.h"
#include "math.h"
/*
************************************************************************
* @brief      �������ݸ���λ��
* @param      NULL 
* @return     void
* describe:   NULL
************************************************************************
*/
void vofa_start(void)
{
	static uint16_t scnt = 0;
	scnt++;
	if(scnt >= 360)
		scnt = 0;
	
	float v1 = sin((double)scnt / 180 * 3.14159) * 180 + 180;
	
	vofa_send_data(0,v1);
	vofa_send_data(0,10);
	vofa_send_data(0,20);
	vofa_send_data(0,15);
	
	vofa_sendframetail();
}
/*
************************************************************************
* @brief      �������ݸ���λ��
* @param      NULL 
* @return     void
* describe:   NULL
************************************************************************
*/
void vofa_send_data(uint8_t num, float data)
{
  // ��������
  float Data[1];
  Data[0] = data;
//  HAL_UART_Transmit(&huart3,(uint8_t*)Data,sizeof(Data),0XFFFF);
	
	CDC_Transmit_FS((uint8_t *)Data,sizeof(Data));
}
/*
************************************************************************
* @brief      ��ÿ�����ݰ�����֡β
* @param      NULL 
* @return     void
* describe:   NULL
************************************************************************
*/
void vofa_sendframetail(void)
{
  // ����֡β
  char tail[4] = {0x00, 0x00, 0x80, 0x7f};
  CDC_Transmit_FS((uint8_t *)tail,sizeof(tail));
}





















