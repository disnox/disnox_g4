#ifndef __VOFA_H
#define __VOFA_H


#include "main.h"


#define byte0(dw_temp)     (*(char*)(&dw_temp))
#define byte1(dw_temp)     (*((char*)(&dw_temp) + 1))
#define byte2(dw_temp)     (*((char*)(&dw_temp) + 2))
#define byte3(dw_temp)     (*((char*)(&dw_temp) + 3))


void vofa_start(void);
void vofa_send_data(uint8_t num, float data);
void vofa_sendframetail(void);
void vofa_demo(void);

#endif














