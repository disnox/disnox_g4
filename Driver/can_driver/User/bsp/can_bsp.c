#include "can_bsp.h"

void can_bsp_init(void)
{
	can_filter_init();
	HAL_FDCAN_Start(&hfdcan1);                               //����FDCAN
	HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
}

/**
************************************************************************
* @brief:      	can_filter_init(void)
* @param[in]:   void
* @retval:     	void
* @details:    	CAN�˲�����ʼ��
************************************************************************
**/
void can_filter_init(void)
{
	FDCAN_FilterTypeDef fdcan_filter;
	
	fdcan_filter.IdType = FDCAN_STANDARD_ID;                       //��׼ID
	fdcan_filter.FilterIndex = 0;                                  //�˲�������                   
	fdcan_filter.FilterType = FDCAN_FILTER_RANGE;                  //�˲�������
	fdcan_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;           //������0������FIFO0  
	fdcan_filter.FilterID1 = 0x0000;                               //32λID
	fdcan_filter.FilterID2 = 0x0000;                               //���FDCAN����Ϊ��ͳģʽ�Ļ���������32λ����
	if(HAL_FDCAN_ConfigFilter(&hfdcan1,&fdcan_filter)!=HAL_OK) 		 //�˲�����ʼ��
	{
		Error_Handler();
	}
}

uint8_t fdcanx_bsp_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len)
{	
	FDCAN_TxHeaderTypeDef TxHeader;
	
	len = len << 16;
	
  TxHeader.Identifier = id;
  TxHeader.IdType = FDCAN_STANDARD_ID;
  TxHeader.TxFrameType = FDCAN_DATA_FRAME;
  TxHeader.DataLength = len;
  TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
  TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
  TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
  TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
  TxHeader.MessageMarker = 0x52;                  
    
  if(HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, data)!=HAL_OK) 
		return 1;//����
	return 0;	
}

uint8_t fdcanx_bsp_receive(FDCAN_HandleTypeDef *hfdcan, uint8_t *buf)
{	
	FDCAN_RxHeaderTypeDef fdcan_RxHeader;
  if(HAL_FDCAN_GetRxMessage(hfdcan,FDCAN_RX_FIFO0, &fdcan_RxHeader, buf)!=HAL_OK)
		return 0;//��������
  return fdcan_RxHeader.DataLength>>16;	
}

void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
  if((RxFifo0ITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET)
  {
		if(hfdcan == &hfdcan1)
		{
			fdcan_rx_callback();
		}
	}
}

__weak void fdcan_rx_callback(void)
{

}
