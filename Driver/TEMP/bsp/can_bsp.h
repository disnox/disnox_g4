#ifndef __CAN_BSP_H__
#define __CAN_BSP_H__
#include "main.h"
#include "fdcan.h"

void can_bsp_init(void);
void can_filter_init(void);
uint8_t fdcanx_bsp_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len);
uint8_t fdcanx_bsp_receive(FDCAN_HandleTypeDef *hfdcan, uint8_t *buf);
void fdcan_rx_callback(void);

#endif /* __CAN_BSP_H_ */

