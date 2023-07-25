#include "remoter.h"
#include "string.h"
#include "custom_lib.h"

#define SBUS_HEAD 0X0F
#define SBUS_END 0X00

remoter_t remoter;
int ljy2 = 0;
void sbus_frame_parse(remoter_t *remoter, uint8_t *buf)
{
	ljy2++;
    if ((buf[0] != SBUS_HEAD) || (buf[24] != SBUS_END))
        return;

    // 按协议，该byte的地4、5位为帧丢失位，但测试开关遥控，没有见到变化
    // 但第6、7位（CH17、CH18）在遥控开启时为0，关闭为1
    // 因此可以用这两位来判断遥控器是否在线
    if (buf[23] == 0x0c)
        remoter->online = 0;
    else
        remoter->online = 1;

    remoter->rc.ch[0] = ((buf[1] | buf[2] << 8) & 0x07FF);
    remoter->rc.ch[1] = ((buf[2] >> 3 | buf[3] << 5) & 0x07FF);
    remoter->rc.ch[2] = ((buf[3] >> 6 | buf[4] << 2 | buf[5] << 10) & 0x07FF);
    remoter->rc.ch[3] = ((buf[5] >> 1 | buf[6] << 7) & 0x07FF);
    remoter->rc.ch[4] = ((buf[6] >> 4 | buf[7] << 4) & 0x07FF);
    remoter->rc.ch[5] = ((buf[7] >> 7 | buf[8] << 1 | buf[9] << 9) & 0x07FF);
    remoter->rc.ch[6] = ((buf[9] >> 2 | buf[10] << 6) & 0x07FF);
    remoter->rc.ch[7] = ((buf[10] >> 5 | buf[11] << 3) & 0x07FF);
    remoter->rc.ch[8] = ((buf[12] | buf[13] << 8) & 0x07FF);
    remoter->rc.ch[9] = ((buf[13] >> 3 | buf[14] << 5) & 0x07FF);

    //遥控器未在线检测,防止忘开遥控器
    //没开遥控器重启开发板之后可能会进入该条件
    if (remoter->rc.ch[0] == 1000 && remoter->rc.ch[1] == 1000 && 
        remoter->rc.ch[2] == 0    && remoter->rc.ch[3] == 1000 && 
		    remoter->rc.ch[4] == 1000 && remoter->rc.ch[5] == 1000 &&	
				remoter->rc.ch[6] == 1000 && remoter->rc.ch[7] == 1000)
    {
        memset(remoter, 0, sizeof(&remoter));
    }
    else
    {
        remoter->joy.left_hori = int16_deadline(remoter->rc.ch[3] - RC_CH_VALUE_OFFSET, -REMOTER_DEADLINE, REMOTER_DEADLINE);
        remoter->joy.left_vert = 1686 - remoter->rc.ch[2];
        remoter->joy.right_hori = int16_deadline(remoter->rc.ch[0] - RC_CH_VALUE_OFFSET, -REMOTER_DEADLINE, REMOTER_DEADLINE);
        remoter->joy.right_vert = int16_deadline(remoter->rc.ch[1] - RC_CH_VALUE_OFFSET, -REMOTER_DEADLINE, REMOTER_DEADLINE);

        remoter->key.a = remoter_2stage_switch_parse(remoter->rc.ch[4], 1000);
        remoter->key.b = remoter_2stage_switch_parse(remoter->rc.ch[5], 1350);
        remoter->key.c = remoter_3stage_switch_parse(remoter->rc.ch[8], 500, 1500);
        remoter->key.d = remoter_2stage_switch_parse(remoter->rc.ch[9], 1000);

        //306 - 1694
        remoter->var.a = remoter_var_parse(remoter->rc.ch[6], 306, 1694);
        remoter->var.b = remoter_var_parse(remoter->rc.ch[7], 306, 1694);
    }
}

inline static int16_t remoter_2stage_switch_parse(int16_t ch, int16_t mid_val)
{
    if (ch < mid_val)
        ch = 0;
    else
        ch = 1;

    return ch;
}

inline static int16_t remoter_3stage_switch_parse(int16_t ch, int16_t mid_val0, int16_t mid_val1)
{
    if (ch > 0 && ch < mid_val0)
        ch = 0;
    else if (ch >= mid_val0 && ch < mid_val1)
        ch = 1;
    else
        ch = 2;
    return ch;
}

inline static float remoter_var_parse(int16_t ch, int16_t mini, int16_t max)
{
    if (ch < mini || ch > max)
        return 0.0f;

    return ((float) (ch - mini)) / (max - mini);
}
