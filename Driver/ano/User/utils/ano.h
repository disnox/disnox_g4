#ifndef __ANO_H__
#define __ANO_H__

#include "main.h"


#define byte0(dw_temp)     (*(char*)(&dw_temp))
#define byte1(dw_temp)     (*((char*)(&dw_temp) + 1))
#define byte2(dw_temp)     (*((char*)(&dw_temp) + 2))
#define byte3(dw_temp)     (*((char*)(&dw_temp) + 3))

#define ANOPTV8_MYDEVID		(0x01)
#define ANOPTV8_SWJID		(0xFE)
#define ANOPTV8_HWVER		(123)
#define ANOPTV8_SWVER		(234)
#define ANOPTV8_BLVER		(0)
#define ANOPTV8_PTVER		(800)

#define ANOPTV8_FRAME_HEAD			0xAB
#define ANOPTV8_FRAME_HEADLEN 		6
#define ANOPTV8_FRAME_MAXDATALEN	100
#define ANOPTV8_FRAME_MAXFRAMELEN	ANOPTV8_FRAME_MAXDATALEN+ANOPTV8_FRAME_HEADLEN+2

#define SENDBUFLEN 			10

typedef struct {
    uint8_t head;
    uint8_t sdevid;
    uint8_t ddevid;
    uint8_t frameid;
    uint16_t datalen;
    uint8_t data[ANOPTV8_FRAME_MAXDATALEN];
    uint8_t sumcheck;
    uint8_t addcheck;
} __attribute__ ((__packed__)) ano_frame_t;

typedef union {
    ano_frame_t frame;
    uint8_t raw_bytes[sizeof(ano_frame_t)];
} ano_frame_u;

typedef struct
{
    uint8_t used;
    uint8_t ready_to_send;
    ano_frame_u data_buf;
} ano_send_t;

void ano_user_data(void);

void ano_start(void);
void ano_start_transmit(void);
void ano_transmit(uint8_t *buf, uint16_t len);

void ano_send_u8(uint8_t data);
void ano_send_u16(uint16_t data);
void ano_send_f(float data);

int ano_get_free_txbuf_index(void);
void ano_check(ano_frame_u *p);
void ano_send_buf(uint8_t daddr, uint8_t fid, uint8_t *buf, uint16_t len);

void ano_demo(void);

#endif /* __ANO_H__ */

