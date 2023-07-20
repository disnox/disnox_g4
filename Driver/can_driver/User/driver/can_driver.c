#include "can_driver.h"
#include "string.h"

uint8_t Car_buff[CAN_BUFFER_SIZE] = {0};
Can_fifo_buffer_t CANRx_Buffer;
uint8_t CAN_Rx_buffer[CAN_BUFFER_SIZE];

Can_fifo_buffer_t CANTx_Buffer;
uint8_t CAN_Tx_buffer[CAN_BUFFER_SIZE];


void can_driver_init(void)
{
	can_bsp_init();
}


/**
************************************************************************
* @brief:      	CAN_Para_Init(void)
* @param[in]:   void
* @retval:     	void
* @details:    	数据结构体初始化
************************************************************************
**/
void CAN_Para_Init(void)
{
	CANTx_Buffer.data = CAN_Tx_buffer;
	CANTx_Buffer.size = CAN_BUFFER_SIZE;
	CANTx_Buffer.read_index = 0;
	CANTx_Buffer.write_index = 0;
	
	CANRx_Buffer.data = CAN_Rx_buffer;
	CANRx_Buffer.size = CAN_BUFFER_SIZE;
	CANRx_Buffer.read_index = 0;
	CANRx_Buffer.write_index = 0;
}

void fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len)
{
	fdcanx_bsp_send_data(hfdcan, id, data, len);
}

void fdcan_rx_callback(void)
{
	uint8_t rx_data[8] = {0};
	fdcanx_bsp_receive(&hfdcan1, rx_data);
	for(int i = 0; i < 8; i++) {
		CANRx_Buffer.data[CANRx_Buffer.write_index] = rx_data[i];	
		CANRx_Buffer.write_index = (CANRx_Buffer.write_index + 1) % CANRx_Buffer.size;
	}
}

/**
************************************************************************
* @brief:      	can_serial_available(void)
* @param[in]:   void
* @retval:     	ch: 字节数据
* @details:    	从缓存区读取一个字节
************************************************************************
**/
uint8_t can_serial_read_char(void)
{
    uint8_t ch = 0;
    ch = CANRx_Buffer.data[CANRx_Buffer.read_index];
    CANRx_Buffer.read_index = (CANRx_Buffer.read_index + 1) % CANRx_Buffer.size;
    return ch;
}
/**
************************************************************************
* @brief:      	can_serial_read(uint8_t *buffer, uint16_t length)
* @param[in]:   buffer: 数据
* @param[in]:   length: 缓冲区数据长度
* @retval:     	i: 数据个数
* @details:    	从缓存区读取数据
************************************************************************
**/
uint16_t can_serial_read(uint8_t *buffer, uint16_t length)
{
	uint16_t i = 0;

	for (i = 0; i < length; i++) {
		buffer[i] = CANRx_Buffer.data[CANRx_Buffer.read_index];
		CANRx_Buffer.read_index = (CANRx_Buffer.read_index + 1) % CANRx_Buffer.size;
	}
	return i;
}
/**
************************************************************************
* @brief:      	can_serial_available(void)
* @param[in]:   void
* @retval:     	void
* @details:    	从缓存区读取数据长度
************************************************************************
**/
uint16_t can_serial_available(void)
{
	uint16_t len = 0;
	uint16_t CANRx_Buffer_read_index = CANRx_Buffer.read_index;
	uint16_t CANRx_Buffer_write_index = CANRx_Buffer.write_index;

	if (CANRx_Buffer_read_index > CANRx_Buffer_write_index) {
		len = CANRx_Buffer.size + CANRx_Buffer_write_index - CANRx_Buffer_read_index;
	}
	else if (CANRx_Buffer_read_index  < CANRx_Buffer_write_index) {
		len = CANRx_Buffer_write_index - CANRx_Buffer_read_index;
	}
	return len;
}
/**
************************************************************************
* @brief:      	can_tx_buf
* @param[in]:   buffer: 数据
* @param[in]:   length: 缓冲区数据长度
* @param[in]:   Tx_Buffer: 发送结构体句柄
* @retval:     	i: 数据个数
* @details:    	把待发送的数据写入缓冲区
************************************************************************
**/
uint16_t can_tx_buf(uint8_t *buffer, uint16_t length, Can_fifo_buffer_t* Tx_Buffer)
{
	uint16_t i = 0;

	for (i = 0; i < length; i++) {
		Tx_Buffer->data[Tx_Buffer->write_index] = buffer[i];
		Tx_Buffer->write_index = (Tx_Buffer->write_index + 1) % Tx_Buffer->size;
	}
	return i;
}
/**
************************************************************************
* @brief:      	can_serial_write(uint8_t *buffer, uint16_t length)
* @param[in]:   buffer: 数据
* @param[in]:   length: 缓冲区数据长度
* @retval:     	i: 数据个数
* @details:    	把待发送的数据写入缓冲区
************************************************************************
**/
uint16_t can_serial_write(uint8_t *buffer, uint16_t length)
{
	uint16_t i = 0;
	i = can_tx_buf(buffer, length, &CANTx_Buffer);
	return i;
}
/**
************************************************************************
* @brief:      	can_tx_available(Can_fifo_buffer_t * Tx_Buffer)
* @param[in]:   Tx_Buffer: 发送结构体句柄
* @retval:     	len: 缓冲区数据长度
* @details:    	判断缓冲区数据长度
************************************************************************
**/
uint16_t can_tx_available(Can_fifo_buffer_t * Tx_Buffer)
{
	uint16_t len = 0;
	if (Tx_Buffer->read_index > Tx_Buffer->write_index) {
		len = Tx_Buffer->size + Tx_Buffer->write_index - Tx_Buffer->read_index;
	}
	else if (Tx_Buffer->read_index  < Tx_Buffer->write_index) {
		len = Tx_Buffer->write_index - Tx_Buffer->read_index;
	}
	return len;
}
/**
************************************************************************
* @brief:      	can_transmit(void)
* @param[in]:   void
* @retval:     	void
* @details:    	can总线数据发出
************************************************************************
**/
uint16_t tx_len1 = 0;
uint8_t TxMsg[8];
void can_transmit(void)
{  
	tx_len1 =  can_tx_available(&CANTx_Buffer);
	if(tx_len1 > 0) {
		uint16_t temp;
		if(CAN_SEND_ID != 0) {
			if(tx_len1 > 8) {
				temp = CANTx_Buffer.size - CANTx_Buffer.read_index;
				if(temp < 8) {
					memcpy(TxMsg,&(CANTx_Buffer.data[CANTx_Buffer.read_index]), temp);
					memcpy(&TxMsg[temp],&(CANTx_Buffer.data[0]), 8-temp);
				}
				else {
					memcpy(TxMsg, &(CANTx_Buffer.data[CANTx_Buffer.read_index]), 8);
				}
				fdcanx_send_data(&hfdcan1, CAN_SEND_ID,TxMsg, 8);
				CANTx_Buffer.read_index = (CANTx_Buffer.read_index + 8)%CANTx_Buffer.size;
			}
			else {
				temp = CANTx_Buffer.size - CANTx_Buffer.read_index;
				if(temp < tx_len1) {
					memcpy(TxMsg,&(CANTx_Buffer.data[CANTx_Buffer.read_index]), temp);
					memcpy(&TxMsg[temp],&(CANTx_Buffer.data[0]), tx_len1-temp);
				}
				else {
					memcpy(TxMsg,&(CANTx_Buffer.data[CANTx_Buffer.read_index]), tx_len1);
				}
				fdcanx_send_data(&hfdcan1, CAN_SEND_ID, TxMsg, tx_len1);
				CANTx_Buffer.read_index = (CANTx_Buffer.read_index + tx_len1)%CANTx_Buffer.size;
			}
		}
	}
}

