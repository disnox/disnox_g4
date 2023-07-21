#include "ano.h"
#include "math.h"
#include "usbd_cdc_if.h"
//#include "usart.h"

#define SEND_BUF_LENGTH 128  // 设置发送缓冲区的长度

uint8_t send_buf[SEND_BUF_LENGTH];
uint8_t send_buf_index = 0;

ano_send_t ano_send[SENDBUFLEN];


void ano_user_data(void)
{
//	ano_send_u16(data1);
//  ano_send_u16(data2);
//  ano_send_f(_f);
}

void ano_start(void)
{
	// 清空发送缓冲区
	memset(send_buf, 0, sizeof(send_buf));
	send_buf_index = 0;
	
	ano_user_data();
	ano_demo();
    
	ano_send_buf(ANOPTV8_SWJID, 0xF1, send_buf, send_buf_index);
	ano_start_transmit();
}

void ano_transmit(uint8_t *buf, uint16_t len)
{
	CDC_Transmit_FS((uint8_t *)buf, len);
//	HAL_UART_Transmit_DMA(&huart1, (uint8_t *)buf, len);
}

void store_data(const uint8_t* data, uint8_t len)
{
	for (uint8_t i = 0; i < len; i++)
	{
		send_buf[send_buf_index++] = data[i];
	}
}

void ano_send_u8(uint8_t data)
{
    uint8_t data_buf[2];
    data_buf[0] = byte0(data);
    store_data(data_buf, 1);
}

void ano_send_u16(uint16_t data)
{
    uint8_t data_buf[2];
    data_buf[0] = byte0(data);
    data_buf[1] = byte1(data);
    store_data(data_buf, 2);
}

void ano_send_f(float data)
{
    uint8_t data_buf[4];
    data_buf[0] = byte0(data);
    data_buf[1] = byte1(data);
    data_buf[2] = byte2(data);
    data_buf[3] = byte3(data);
    store_data(data_buf, 4);
}

void ano_start_transmit(void)
{
	static uint16_t last_index = 0;
	
	for(int i=0; i<SENDBUFLEN; i++)
	{
		if(ano_send[last_index].used == 1 && ano_send[last_index].ready_to_send == 1)
		{
				//
			ano_transmit(ano_send[last_index].data_buf.raw_bytes, ano_send[last_index].data_buf.frame.datalen+ANOPTV8_FRAME_HEADLEN+2);
			ano_send[last_index].used = 0;
			ano_send[last_index].ready_to_send = 0;
	
			return ;
		}
		else
		{
			last_index++;
			while(last_index >= SENDBUFLEN)
				last_index -= SENDBUFLEN;
		}
	}
}


int ano_get_free_txbuf_index(void)
{
    static uint16_t last_index = 0;
    int ret = -1;

    for(int i=0; i<SENDBUFLEN; i++)
    {
        if(ano_send[last_index].used == 0)
        {
            ano_send[last_index].used = 1;
            ret = last_index;
            return ret;
        }
				else
				{
					last_index++;
					while(last_index >= SENDBUFLEN)
						last_index -= SENDBUFLEN;
				}
		
    }
    return -1;
}

void ano_check(ano_frame_u *p)
{
    uint8_t sumcheck = 0, addcheck = 0;
    for (uint16_t i = 0; i < (ANOPTV8_FRAME_HEADLEN + p->frame.datalen); i++)
    {
        sumcheck += p->raw_bytes[i];
        addcheck += sumcheck;
    }
    p->raw_bytes[ANOPTV8_FRAME_HEADLEN + p->frame.datalen] = sumcheck;
    p->raw_bytes[ANOPTV8_FRAME_HEADLEN + p->frame.datalen + 1] = addcheck;
    p->frame.sumcheck = sumcheck;
    p->frame.addcheck = addcheck;
}


void ano_send_buf(uint8_t daddr, uint8_t fid, uint8_t *buf, uint16_t len)
{
	uint8_t _cnt = 0;

	int bufindex = ano_get_free_txbuf_index();
	
	if (bufindex >= 0)
	{
		ano_send[bufindex].used = 1;
		
		ano_send[bufindex].data_buf.frame.head = ANOPTV8_FRAME_HEAD;
		ano_send[bufindex].data_buf.frame.sdevid = ANOPTV8_MYDEVID;
		ano_send[bufindex].data_buf.frame.ddevid = daddr;
		ano_send[bufindex].data_buf.frame.frameid = fid;
		/***********************************************************************/
		for(uint16_t i=0; i<len; i++)
			ano_send[bufindex].data_buf.frame.data[_cnt++] = *(buf+i);
		/***********************************************************************/
		ano_send[bufindex].data_buf.frame.datalen = _cnt;
		ano_check(&ano_send[bufindex].data_buf);
		ano_send[bufindex].ready_to_send = 1;
	}
}


void ano_demo(void)
{
	static uint16_t scnt = 0;
	scnt++;
	if(scnt >= 360)
		scnt = 0;
	
	uint16_t v1 = scnt;
	float v2 = sin((double)scnt / 180 * 3.14159) * 180 + 180;
	float v3 = sin((double)(scnt+120) / 180 * 3.14159) * 180 + 180;
	float v4 = sin((double)(scnt+240) / 180 * 3.14159) * 180 + 180;
	
	ano_send_u16(v1);
	ano_send_f(v2);
	ano_send_f(v3);
	ano_send_f(v4);
}



