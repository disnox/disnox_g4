/**
  ****************************(C) COPYRIGHT 2020 HCRT****************************
  * @file       bsp_usart.c/h
  * @brief      空闲中断配置和中断处理函数
  * @note
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     2020.2.1    		 YLT             V1.0 done
	*  V1.0.1     2020.3.16    		 YLT               update
  @verbatim
  ==============================================================================
	* 2020.3.16(update):增加详细的注释 优化程序逻辑 将中断入口函数全部添加到
	* stm32f4xx_it.c 增加 空闲中断+DMA 接收数据的流程说明
	*
	* 空闲中断介绍 :
	*						接收中断													空闲中断
	*	处理函数 	USARTx_IRQHandler								USARTx_IRQHandler
	*	回调函数 	HAL_UART_RxCpltCallback 				HAL库没有提供
	*	USART 状态寄存器中的位 UART_FLAG_RXNE 		UART_FLAG_IDLE
	*	触发条件 完成一帧数据的接收之后触发一次		串口接收完一帧数据后又过了一个
  *																						字节的时间没有接收到任何数据
	*	串口空闲中断即每当串口接收完一帧数据后又过了一个字节的时间没有接收到任何数据则
	* 触发一次中断，中断处理函数同样为 USARTx_IRQHandler，可以通过 USART 状态寄存器
	* 中的 UART_FLAG_IDLE 判断是否发生了空闲中断。
	*
	*	对于DMA UART 发送来说 通道剩余数据个数 = 总共需要接收的个数 - 目前已发送出的数据的个数
	*	对于DMA UART 接收来说 通道剩余数据个数 = 总共需要接收的个数 - 目前已接收到的数据的个数
	*
	* 空闲中断的数据长度判断 为什么 USARTX_BUFLEN = USARTX_MAX_LEN - dma_stream->NDTR 
	*	在DMA开启时填充的期望接收到的数据的个数为 USARTX_MAX_LEN 即NDTR寄存器=USARTX_MAX_LEN
  * 而每当DMA搬运完一个数据 NDTR寄存器的值就会减一
	*	因为我们定义了最大接收数设置为数据长度的两倍 USARTX_MAX_LEN = USARTX_BUFLEN * 2
	* 因此当dma_stream->NDTR 的值由USARTX_MAX_LEN 变为 USARTX_BUFLEN时 就满足了判断条件
	*	为什么要将最大接收数设置为数据长度的两倍？
	* 1.留出接收余量 保证数据能能够被全部接收到 2.为了好判断 写判断条件的时候比较简洁
	*
	*	2020.2.14(update):
	* 写完了所有串口的空闲中断，按需使用
	* 需要在stm32f4xx_it.c 中添加入口函数 uart_receive_handler
  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2020 HCRT****************************
  */

#include "bsp_usart.h"
#include "main.h"
#include "string.h"
#include "remoter.h"

/******** 串口接收数组定义 ********/
uint8_t usart3_buf[USART3_BUFLEN];

/******** 数据结构体定义 ********/
imudata_t imudata;
rc_sbus_t rc_sbus_receive;

/******** 测试变量 ********/
int ljy1=0;
int ljy3=0;
/******** 串口空闲中断处理函数 ********/
void usart3_callback_handler(uint8_t *buff)
{
	sbus_frame_parse(&remoter, buff);
}
/**
  * @brief      返回当前DMA通道中剩余的数据个数
  * @param[in]  dma_stream: DMA通道
  * @retval     DMA通道中剩余的数据个数
  */
uint16_t dma_current_data_counter(DMA_Stream_TypeDef *dma_stream)
{
  return ((uint16_t)(dma_stream->NDTR));
}

/**
  * @brief	在接收到一帧数据之后空闲一帧数据时间之后无数据
	*					再来则进入此回调函数,此函数会清除空闲中断标志位
  * @param	huart: UART句柄指针
  * @retval
  */
static void uart_rx_idle_callback(UART_HandleTypeDef *huart)
{
  if (huart == &huart3)
  {
    //判断数据是否为期望的长度 如不是则不进入回调函数 直接开启下一次接收
    if ((USART3_MAX_LEN - dma_current_data_counter(huart->hdmarx->Instance)) == USART3_BUFLEN)
    {
      /* 进入空闲中断回调函数 */
      usart3_callback_handler(usart3_buf);
    }

    /* 设置DMA接收数据的长度 */
    __HAL_DMA_SET_COUNTER(huart->hdmarx, USART3_MAX_LEN);
  }

}

/**
  * @brief	当串口发生中断的时候进此函数
  * @param	huart: UART句柄指针
  * @retval	在stm32f4xx_it.c中添加
  */
int ljy6 = 0;
void uart_receive_handler(UART_HandleTypeDef *huart)
{
  /* __HAL_UART_GET_FLAG	检查指定的UART空闲标志位是否触发 */
  /* __HAL_UART_GET_IT_SOURCEG	检查指定的UART空闲中断是否触发 */
  if (__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) &&
      __HAL_UART_GET_IT_SOURCE(huart, UART_IT_IDLE))
  {
		ljy6++;
    /* 清除空闲中断标志位 */
    __HAL_UART_CLEAR_IDLEFLAG(huart);

    /* 关掉DMA */
    __HAL_DMA_DISABLE(huart->hdmarx);

    /* 进入空闲中断处理函数 */
    uart_rx_idle_callback(huart);

    /* 重启DMA传输 */
    __HAL_DMA_ENABLE(huart->hdmarx);
  }
}

/**
  * @brief      配置使能DMA接收(而不是中断接收)
  * @param[in]  huart: UART句柄指针
  * @param[in]  pData: receive buff
  * @param[in]  Size:  buff size
  * @retval     set success or fail
  */
static int uart_receive_dma_no_it(UART_HandleTypeDef *huart, uint8_t *pData, uint32_t Size)
{
  uint32_t tmp = 0;
  tmp = huart->RxState;

  /* 判断串口是否已经初始化完成 */
  if (tmp == HAL_UART_STATE_READY)
  {
    /* 检测用户输入的数据是否正确 */
    if ((pData == NULL) || (Size == 0))
      return HAL_ERROR;

    huart->pRxBuffPtr = pData;
    huart->RxXferSize = Size;
    huart->ErrorCode = HAL_UART_ERROR_NONE;

    /* 使能DMA通道 */
    HAL_DMA_Start(huart->hdmarx, (uint32_t)&huart->Instance->DR, (uint32_t)pData, Size);

    /* 开启DMA传输 将UART CR3 寄存器中的 DMAR位 置高 */
    SET_BIT(huart->Instance->CR3, USART_CR3_DMAR);

    return HAL_OK;
  }
  else
    return HAL_BUSY;
}

/**
  * @brief	空闲中断初始化函数
  * @param	huart:UART句柄指针
  * @retval	none
  */
void uart_receive_init(UART_HandleTypeDef *huart)
{
  if (huart == &huart3)
  {
    /* 清除空闲中断标志位 */
    __HAL_UART_CLEAR_IDLEFLAG(&huart3);
    /* 开启串口空闲中断 */
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
    /* 开启DMA接收 指定接收长度和数据地址 */
    uart_receive_dma_no_it(&huart3, usart3_buf, USART3_MAX_LEN);
  }
}

void usart_init(void)
{
	uart_receive_init(&huart3);
}


