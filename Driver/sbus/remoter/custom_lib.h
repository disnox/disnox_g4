/*
 * Copyright (c) 2006-2020, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-03-20     yuleitao       the first version
 */
#ifndef APPLICATIONS_COMMON_H_
#define APPLICATIONS_COMMON_H_
#include "main.h"

#define max(x,y)({          \
    typeof(x) _x = (x);     \
    typeof(y) _y = (y);     \
    (void) (&_x == &_y);    \
    _x > _y ? _x : _y;     })

//转换到0-2PI
#define constrain_0_2pi(amt) (((amt) > 2 * PI) ? ((amt) -= 2 * PI) : (((amt) < 0) ? ((amt) += 2 * PI) : ((amt) = (amt))))
//转换到-pi-pi
#define constrain_npi_pi(amt) (((amt) > PI) ? ((amt) -= 2 * PI) : (((amt) < -PI) ? ((amt) += 2 * PI) : ((amt) = (amt))))

//转换到0-360
#define constrain_0_360(amt) (((amt) > 360) ? ((amt) -= 360) : (((amt) < 0) ? ((amt) += 360) : ((amt) = (amt))))
//转换到-180-180
#define constrain_n180_180(amt) (((amt) > 180) ? ((amt) -= 360) : (((amt) < -180) ? ((amt) += 360) : ((amt) = (amt))))

//下限限幅
#define constrain_mini(amt, low) ((amt) < (low) ? (amt = low) : (amt = amt))
//上限限幅
#define constrain_max(amt, high) ((amt) > (high) ? (amt = high) : (amt = amt))
//限幅
#define constrain(amt, low, high) ((amt) < (low) ? (amt = low) : ((amt) > (high) ? (amt = high) : (amt = amt)))
//循环限幅
#define loop_constrain(amt, low, high) ((amt) < (low) ? (amt = high) : ((amt) > (high) ? (amt = low) : (amt = amt)))

#define DEG2RAD(x) (x * 0.01745329252f) // *PI/180
#define RAD2DEG(x) (x * 57.2957795131f) // *180/PI

float float_deadline(float Value, float minValue, float maxValue);
int16_t int16_deadline(int16_t Value, int16_t minValue, int16_t maxValue);

#define HIGH    0x1
#define LOW     0x0
#define flaot0  1e-6  //当变量的绝对值小于此时，变量float为0
//#define PI 3.1415926535897932384626433832795f
#define HALF_PI 1.5707963267948966192313216916398f
#define TWO_PI 6.283185307179586476925286766559f
#define DEG_TO_RAD 0.017453292519943295769236907684886f
#define RAD_TO_DEG 57.295779513082320876798154814105f
#define EULER 2.718281828459045235360287471352f

#ifdef abs
#undef abs
#endif

#define VAL_LIMIT(val, min, max) \
do {\
if((val) <= (min))\
{\
  (val) = (min);\
}\
else if((val) >= (max))\
{\
  (val) = (max);\
}\
} while(0)\

#define getBit(value, pos) ((value >> pos) & 1)
#define setBit(value, pos) value|(1 << pos)
#define clrBit(value, pos) value&(~(1 << pos))
#define toggleBit(value, pos) value^(1 << pos)

long map(long, long, long, long, long);
int floatEqual_0(float num);

#define  IndexOutofBounds(index,length) (index<0||index>length-1)

typedef union //uint8 与16进制转换联合体
{
    uint8_t value;
    unsigned char data[1];
} hex_uint8_u;

typedef union //UINT16 与16进制转换联合体
{
    uint16_t value;
    unsigned char data[2];
} hex_uint16_u;

typedef union //INT16 与16进制转换联合体
{
    int16_t value;
    unsigned char data[2];
} hex_int16_u;

typedef union //浮点数 与16进制转换联合体
{
    float value;
    unsigned char data[4];
} hex_float_u;

#endif /* APPLICATIONS_ECP_COMMON_H_ */
