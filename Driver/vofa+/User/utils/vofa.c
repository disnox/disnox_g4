#include "vofa.h"
#include "usbd_cdc_if.h"
#include "math.h"
#include "usart.h"

#define SEND_BUF_LENGTH 1024  // 设置发送缓冲区的长度

uint8_t send_buf[SEND_BUF_LENGTH];
uint8_t send_buf_index = 0;

/*
************************************************************************
* @brief      发送数据给上位机
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
* @brief      发送数据给上位机
* @param      NULL 
* @return     void
* describe:   NULL
************************************************************************
*/
void vofa_send_data(uint8_t num, float data)
{
  // 发送数据
  float Data[1];
  Data[0] = data;
  HAL_UART_Transmit(&huart3,(uint8_t*)Data,sizeof(Data),0XFFFF);
	
//	CDC_Transmit_FS((uint8_t *)Data,sizeof(Data));
}
/*
************************************************************************
* @brief      给每个数据包发送帧尾
* @param      NULL 
* @return     void
* describe:   NULL
************************************************************************
*/
void vofa_sendframetail(void)
{
  // 发送帧尾
  char tail[4] = {0x00, 0x00, 0x80, 0x7f};
	HAL_UART_Transmit(&huart3,(uint8_t*)tail,sizeof(tail),0XFFFF);
//  CDC_Transmit_FS((uint8_t *)tail,sizeof(tail));
}
float v5 = 100.0f;
void vofa_demo(void)
{
	static uint16_t scnt = 0;
	uint16_t cnt = 0; 
	scnt++;
	if(scnt >= 360)
		scnt = 0;
	
	float v1 = scnt;
	float v2 = sin((double)scnt / 180 * 3.14159) * 180 + 180;
	float v3 = sin((double)(scnt+120) / 180 * 3.14159) * 180 + 180;
	float v4 = sin((double)(scnt+240) / 180 * 3.14159) * 180 + 180;
	
	send_buf[cnt++] = byte0(v1);
	send_buf[cnt++] = byte1(v1);
	send_buf[cnt++] = byte2(v1);
	send_buf[cnt++] = byte3(v1);
	
	send_buf[cnt++] = byte0(v2);
	send_buf[cnt++] = byte1(v2);
	send_buf[cnt++] = byte2(v2);
	send_buf[cnt++] = byte3(v2);
	
	send_buf[cnt++] = byte0(v3);
	send_buf[cnt++] = byte1(v3);
	send_buf[cnt++] = byte2(v3);
	send_buf[cnt++] = byte3(v3);
	
	send_buf[cnt++] = byte0(v4);
	send_buf[cnt++] = byte1(v4);
	send_buf[cnt++] = byte2(v4);
	send_buf[cnt++] = byte3(v4);
	
	send_buf[cnt++] = byte0(v5);
	send_buf[cnt++] = byte1(v5);
	send_buf[cnt++] = byte2(v5);
	send_buf[cnt++] = byte3(v5);
	
	send_buf[cnt++] = 0x00;
	send_buf[cnt++] = 0x00;
	send_buf[cnt++] = 0x80;
	send_buf[cnt++] = 0x7f;
	
	CDC_Transmit_FS((uint8_t *)send_buf, cnt);
}











