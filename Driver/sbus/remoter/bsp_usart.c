/**
  ****************************(C) COPYRIGHT 2020 HCRT****************************
  * @file       bsp_usart.c/h
  * @brief      �����ж����ú��жϴ�����
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2020.2.1    		 YLT             V1.0 done
	*  V1.0.1     2020.3.16    		 YLT               update
  @verbatim
  ==============================================================================
	* 2020.3.16(update):������ϸ��ע�� �Ż������߼� ���ж���ں���ȫ����ӵ�
	* stm32f4xx_it.c ���� �����ж�+DMA �������ݵ�����˵��
	*
	* �����жϽ��� :
	*						�����ж�													�����ж�
	*	������ 	USARTx_IRQHandler								USARTx_IRQHandler
	*	�ص����� 	HAL_UART_RxCpltCallback 				HAL��û���ṩ
	*	USART ״̬�Ĵ����е�λ UART_FLAG_RXNE 		UART_FLAG_IDLE
	*	�������� ���һ֡���ݵĽ���֮�󴥷�һ��		���ڽ�����һ֡���ݺ��ֹ���һ��
  *																						�ֽڵ�ʱ��û�н��յ��κ�����
	*	���ڿ����жϼ�ÿ�����ڽ�����һ֡���ݺ��ֹ���һ���ֽڵ�ʱ��û�н��յ��κ�������
	* ����һ���жϣ��жϴ�����ͬ��Ϊ USARTx_IRQHandler������ͨ�� USART ״̬�Ĵ���
	* �е� UART_FLAG_IDLE �ж��Ƿ����˿����жϡ�
	*
	*	����DMA UART ������˵ ͨ��ʣ�����ݸ��� = �ܹ���Ҫ���յĸ��� - Ŀǰ�ѷ��ͳ������ݵĸ���
	*	����DMA UART ������˵ ͨ��ʣ�����ݸ��� = �ܹ���Ҫ���յĸ��� - Ŀǰ�ѽ��յ������ݵĸ���
	*
	* �����жϵ����ݳ����ж� Ϊʲô USARTX_BUFLEN = USARTX_MAX_LEN - dma_stream->NDTR 
	*	��DMA����ʱ�����������յ������ݵĸ���Ϊ USARTX_MAX_LEN ��NDTR�Ĵ���=USARTX_MAX_LEN
  * ��ÿ��DMA������һ������ NDTR�Ĵ�����ֵ�ͻ��һ
	*	��Ϊ���Ƕ�����������������Ϊ���ݳ��ȵ����� USARTX_MAX_LEN = USARTX_BUFLEN * 2
	* ��˵�dma_stream->NDTR ��ֵ��USARTX_MAX_LEN ��Ϊ USARTX_BUFLENʱ ���������ж�����
	*	ΪʲôҪ��������������Ϊ���ݳ��ȵ�������
	* 1.������������ ��֤�������ܹ���ȫ�����յ� 2.Ϊ�˺��ж� д�ж�������ʱ��Ƚϼ��
	*
	*	2020.2.14(update):
	* д�������д��ڵĿ����жϣ�����ʹ��
	* ��Ҫ��stm32f4xx_it.c �������ں��� uart_receive_handler
  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 HCRT****************************
  */

#include "bsp_usart.h"
#include "main.h"
#include "string.h"
#include "remoter.h"

/******** ���ڽ������鶨�� ********/
uint8_t usart3_buf[USART3_BUFLEN];

/******** ���ݽṹ�嶨�� ********/
imudata_t imudata;
rc_sbus_t rc_sbus_receive;

/******** ���Ա��� ********/
int ljy1=0;
int ljy3=0;
/******** ���ڿ����жϴ����� ********/
void usart3_callback_handler(uint8_t *buff)
{
	sbus_frame_parse(&remoter, buff);
}
/**
  * @brief      ���ص�ǰDMAͨ����ʣ������ݸ���
  * @param[in]  dma_stream: DMAͨ��
  * @retval     DMAͨ����ʣ������ݸ���
  */
uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
  return ((uint16_t)(dma_stream->NDTR));
}

/**
  * @brief	�ڽ��յ�һ֡����֮�����һ֡����ʱ��֮��������
	*					���������˻ص�����,�˺�������������жϱ�־λ
  * @param	huart: UART���ָ��
  * @retval
  */
static void uart_rx_idle_callback(UART_HandleTypeDef *huart)
{
  if (huart == &huart3)
  {
    //�ж������Ƿ�Ϊ�����ĳ��� �粻���򲻽���ص����� ֱ�ӿ�����һ�ν���
    if ((USART3_MAX_LEN - dma_current_data_counter(huart->hdmarx->Instance)) == USART3_BUFLEN)
    {
      /* ��������жϻص����� */
      usart3_callback_handler(usart3_buf);
    }

    /* ����DMA�������ݵĳ��� */
    __HAL_DMA_SET_COUNTER(huart->hdmarx, USART3_MAX_LEN);
  }

}

/**
  * @brief	�����ڷ����жϵ�ʱ����˺���
  * @param	huart: UART���ָ��
  * @retval	��stm32f4xx_it.c�����
  */
int ljy6 = 0;
void uart_receive_handler(UART_HandleTypeDef *huart)
{
  /* __HAL_UART_GET_FLAG	���ָ����UART���б�־λ�Ƿ񴥷� */
  /* __HAL_UART_GET_IT_SOURCEG	���ָ����UART�����ж��Ƿ񴥷� */
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
      __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
  {
		ljy6++;
    /* ��������жϱ�־λ */
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    /* �ص�DMA */
    __HAL_DMA_DISABLE(huart->hdmarx);

    /* ��������жϴ����� */
    uart_rx_idle_callback(huart);

    /* ����DMA���� */
    __HAL_DMA_ENABLE(huart->hdmarx);
  }
}

/**
  * @brief      ����ʹ��DMA����(�������жϽ���)
  * @param[in]  huart: UART���ָ��
  * @param[in]  pData: receive buff
  * @param[in]  Size:  buff size
  * @retval     set success or fail
  */
static int uart_receive_dma_no_it(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
{
  uint32_t tmp = 0;
  tmp = huart->RxState;

  /* �жϴ����Ƿ��Ѿ���ʼ����� */
  if (tmp == HAL_UART_STATE_READY)
  {
    /* ����û�����������Ƿ���ȷ */
    if ((pData == NULL) || (Size == 0))
      return HAL_ERROR;

    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;
    huart->ErrorCode = HAL_UART_ERROR_NONE;

    /* ʹ��DMAͨ�� */
    HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)pData, Size);

    /* ����DMA���� ��UART CR3 �Ĵ����е� DMARλ �ø� */
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    return HAL_OK;
  }
  else
    return HAL_BUSY;
}

/**
  * @brief	�����жϳ�ʼ������
  * @param	huart:UART���ָ��
  * @retval	none
  */
void uart_receive_init(UART_HandleTypeDef *huart)
{
  if (huart == &huart3)
  {
    /* ��������жϱ�־λ */
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);
    /* �������ڿ����ж� */
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    /* ����DMA���� ָ�����ճ��Ⱥ����ݵ�ַ */
    uart_receive_dma_no_it(&huart3, usart3_buf, USART3_MAX_LEN);
  }
}

void usart_init(void)
{
	uart_receive_init(&huart3);
}


