#ifndef __CAN_DRIVER_H__
#define __CAN_DRIVER_H__

#include "main.h"
#include "can_bsp.h"

#define CAN_SEND_ID 0X500
#define CAN_RECEIVE_ID 0X580
#define CAN_BUFFER_SIZE   (128)

typedef struct
{
    uint8_t *data;
    uint16_t read_index, write_index;
    uint16_t size;
} Can_fifo_buffer_t;

extern uint8_t Car_buff[CAN_BUFFER_SIZE];

void fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len);

uint16_t can_serial_available(void);
uint8_t can_serial_read_char(void);
uint16_t can_serial_write(uint8_t *buffer, uint16_t length); //∑¢ÀÕ
uint16_t can_serial_read(uint8_t *buffer, uint16_t length);  //Ω” ’

void CAN_Para_Init(void);
void can_transmit(void);



#endif /* __CAN_DRIVER_H__ */


