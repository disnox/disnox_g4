/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-03     yuleitao       the first version
 */


#include <custom_lib.h>

//浮点死区
float float_deadline(float Value, float minValue, float maxValue)
{
    if (Value < maxValue && Value > minValue)
    {
        Value = 0.0f;
    }
    return Value;
}

//int死区
int16_t int16_deadline(int16_t Value, int16_t minValue, int16_t maxValue)
{
    if (Value < maxValue && Value > minValue)
    {
        Value = 0;
    }
    return Value;
}

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int floatEqual_0(float num)
{ //用于处理判断float变量是否为0
    if (num < (float) flaot0)
    {
        return 1;
    }
    return 0;
}

void float2byte(float *target, unsigned char *buf, unsigned char beg)
{
    unsigned char *point;
    point = (unsigned char *) target; //得到float的地址
    buf[beg] = point[0];
    buf[beg + 1] = point[1];
    buf[beg + 2] = point[2];
    buf[beg + 3] = point[3];
}
