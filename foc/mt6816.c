#include "mt6816.h"


#define     RD_REG_3        		0x8300//0xffff//
#define     RD_REG_4        		0x8400//0x0000//



#define     AA_LINE             	((float32_t)LINE_NUM / (float32_t)POLE_PAIRS)       //2340.57
#define     AA_ANGLE            	((float32_t)LINE_NUM / 360.0f)                      //45.51
#define     BB_ANGLE				((float32_t)AA_LINE / 360.0f)                       //6.50

#define     ANGLE_RATIO				(float32_t)(360.0f / 16384.0f)


#define     OFFSET					0x755		//小
#define     OFFSET1					0x70		//大


uint16_t sendBuf[2] = {RD_REG_3, RD_REG_4};

uint32_t timeout;

struct ENCODER_Type encoder = {
    .hspi = &hspi1,
    .CS_Port = GPIOA,
    .CS_Pin = GPIO_PIN_15,
    .sendBuf = {RD_REG_3, RD_REG_4},
//    .offset = OFFSET,
};

extern int32_t lut_arr[128];


uint8_t SPI_TransmitReceive(SPI_HandleTypeDef * hspi, uint16_t TxData, uint16_t *RxData)            
{
    uint32_t cnt = 0;
    
    while ((hspi->Instance->SR & SPI_SR_TXE) == 0);  
    hspi->Instance->DR = TxData;                           
    while (cnt < 30) 
    {
        if((hspi->Instance->SR & SPI_SR_RXNE))
        {
            *RxData = hspi->Instance->DR;
            return 0;
        }
        cnt++;
    }    
       
    return 1;                             
}

int16_t angleVal;

int sw = 1;

int off_1, off_2, off_interp;
int16_t MT_ReadAngle(struct ENCODER_Type *encoder)
{   
	
    /*读spi编码器*/
    HAL_GPIO_WritePin(encoder->CS_Port, encoder->CS_Pin, GPIO_PIN_RESET); 
    timeout += SPI_TransmitReceive(encoder->hspi, encoder->sendBuf[0], &encoder->recvBuf[0]);
    HAL_GPIO_WritePin(encoder->CS_Port, encoder->CS_Pin, GPIO_PIN_SET);
    for(int i = 0; i < 2; i ++);
    HAL_GPIO_WritePin(encoder->CS_Port, encoder->CS_Pin, GPIO_PIN_RESET);    
    timeout += SPI_TransmitReceive(encoder->hspi, encoder->sendBuf[1], &encoder->recvBuf[1]);     
    HAL_GPIO_WritePin(encoder->CS_Port, encoder->CS_Pin, GPIO_PIN_SET);

	
	
    /*处理原始数据并转换为机械角度*/
    encoder->angleVal = (((encoder->recvBuf[0]&0x00FF) << 6) + ((encoder->recvBuf[1]&0x00FF) >> 2));
	
	off_1 = lut_arr[(encoder->angleVal)>>7];				// lookup table lower entry
	off_2 = lut_arr[((encoder->angleVal>>7)+1)%128];		// lookup table higher entry
	off_interp = off_1 + ( (off_2 - off_1) * (encoder->angleVal - ((encoder->angleVal>>7)<<7)) >> 7 );     // Interpolate between lookup table entries
		
	
	if(sw == 1)
		angleVal = (int16_t)encoder->angleVal - off_interp;
	else if(sw == 2)
		angleVal = (int16_t)encoder->angleVal + off_interp;
	else
		angleVal = (int16_t)encoder->angleVal;// - off_interp;
	
    encoder->angle_360 = (float32_t)angleVal * ANGLE_RATIO;
    
    /*软件记圈*/
    if(encoder->lastAngle>9000&&encoder->angleVal<7000)
        encoder->cycleNum ++;
    else if(encoder->lastAngle<7000&&encoder->angleVal>9000)
        encoder->cycleNum --;
    
    /*记速*/
//    encoder->angleArr[encoder->cnt][0] = encoder->angleVal;
//    encoder->angleArr[encoder->cnt][1] = encoder->cycleNum;
    
//    int32_t temp1 = encoder->angleArr[encoder->cnt][0] - encoder->angleArr[(encoder->cnt +1) % 8][0];
//    int32_t temp2 = (encoder->angleArr[encoder->cnt][1] - encoder->angleArr[(encoder->cnt +1) % 8][1]) << 14;
//    encoder->speed = (float32_t)(temp1 + temp2) ;// / 32.0f;
    
//    int32_t temp1 = 0;
//    int32_t temp2 = 0;
//    int32_t sum = 0;
//    for(uint8_t i = 0; i < 8 - 1; i++)
//    {
//        temp1 = encoder->angleArr[i + 1][0] - encoder->angleArr[i][0];
//        temp2 = (encoder->angleArr[i + 1][1] - encoder->angleArr[i][1]) << 14;
//        sum += (temp1 + temp2) ;// / 32.0f;
//    }
//    encoder->speed = (float32_t)(sum) / 7.0f;// / 32.0f;
    
    
//    encoder->cnt ++;
//    encoder->cnt &= 0x07;
    
    
    encoder->lastAngle = encoder->angleVal;
    
    return angleVal;
}


float32_t GetSpeed(struct ENCODER_Type *encoder)
{
    static int32_t lastAngle, lastCycle;
    
    encoder->speed = (float32_t)(encoder->angleVal - lastAngle + ((encoder->cycleNum - lastCycle)<<14));
    lastAngle = encoder->angleVal; lastCycle = encoder->cycleNum;
    
    encoder->speedFilt = encoder->speed * encoder->filt + (1.0f - encoder->filt) * encoder->speedFilt;
    
    return encoder->speedFilt;
}


float eAng;
float lasteAng;
float alpha = 0.5;
uint8_t sw1;
float32_t MT_ReadAngle_360(struct ENCODER_Type *encoder)
{	
	encoder->eAngle_360 = ((float)POLE_PAIRS*(float)(MT_ReadAngle(encoder) - encoder->offset))/((float)16384);
	int int_angle = (int)encoder->eAngle_360;
	encoder->eAngle_360 = 360.0f*(encoder->eAngle_360 - (float)int_angle);
	//encoder->elec_angle = TWO_PI_F*fmodf((encoder->ppairs*(float)(encoder->count-E_ZERO))/((float)ENC_CPR), 1.0f);
	encoder->eAngle_360 = encoder->eAngle_360<0 ? encoder->eAngle_360 + 360.0f : encoder->eAngle_360;	// Add 2*pi to negative numbers
	
//    float32_t temp = (float32_t)MT_ReadAngle(encoder) - encoder->offset;
//    
//    while(temp > AA_LINE)
//    {
//        temp -= AA_LINE;
//    }
//    
//    encoder->eAngle_360 = temp / BB_ANGLE;
    
	
//	eAng = encoder->eAngle_360;
//	
//	if((eAng < 60.0f) && (lasteAng >300.0f))
//		lasteAng -= 360.0f;
//	else if((eAng > 300.0f) && (lasteAng < 60.0f))
//		lasteAng += 360.0f;
//	
//	eAng = alpha * eAng + (1.0f - alpha) * lasteAng;
//	lasteAng = eAng;
//	
//	if(sw1)
//		return eAng;
	
    return encoder->eAngle_360;
}

//void CS_HIGH(struct ENCODER_Typ encoder)
//{
//    
//}

