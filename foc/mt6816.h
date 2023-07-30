#ifndef __MT6816_H__
#define __MT6816_H__

#include "spi.h"
#include "objdef.h"
#include "conf.h"





extern struct ENCODER_Type encoder;

int16_t MT_ReadAngle(struct ENCODER_Type *encoder);
float32_t MT_ReadAngle_360(struct ENCODER_Type *encoder);

float32_t GetSpeed(struct ENCODER_Type *encoder);

#endif
