/*
 ******************************************************************************
 *                               US212A
 *                            Module: Radio
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: main module of radio
 * Module:  RDA5807E
 * History:
 *      <author>    <time>           <version >             <desc>
 *       Mikeyang    2010-11-25 14:35     1.0             build this file
 ******************************************************************************
 */
#include "fm_drv.h"

extern uint8 WriteBuffer[];
extern uint8 ReadBuffer[];
extern uint8 Set_Band;
extern uint16 Freq_RF;
extern const uint8 InitBuffer_SP[104];
extern const uint8 InitBuffer_HP[110];
extern const uint8 InitBuffer_NP[64];
extern uint16 signal_stg; //驱动层，电台信号强度信息
extern uint8 ST_info; //驱动层:   电台立体声信息  0~~立体声   1~~单声道
extern uint8 hardseek_flag;
extern uint8 WriteNum;
extern uint8 RDA_model; //模组型号:    1  RDA 5807SP      2   RDA 5807HP    3  RDA 5807P


int FM_Standby_Drv(uint8 StandbyFlag);

extern int WriteRegister(uint8 count)
__FAR__;//写寄存器
extern uint8 ReadRegister(uint8 count)
__FAR__; //读寄存器
extern uint16 FreqToChan(uint16 freq, uint8 mode)
__FAR__;

/*
 ***** ***************************************************************************
 * int sFM_Restore_Global_Data(uint8 band, uint8 level, uint16 freq)
 *
 * Description : 重新恢复FM驱动所需要的全局数组WriteBuffer的内容
 *
 * Arguments : 波段选择值,门限
 *                   band=0: 中国/美国电台，步进100KHz
 *                   band=1: 日本电台，步进100KHz
 *                   band=2: 欧洲电台，步进50KHz
 *
 * Returns : 若初始化成功,则返回1,否则,返回0
 *
 * Notes :
 *         由于FM驱动和录音codec所占用的空间是一致的，因此FM录音时FM驱动代码被覆盖，
 *         当退出FM驱动时，需要重新恢复WriteBuffer的内容，以便可以正确的设置相关模
 *         组的寄存器值
 *
 ********************************************************************************
 */
static int sFM_Restore_Global_Data(uint8 band, uint8 level, uint16 freq)
{
    uint16 ChipID;
    uint8 TH_Slevel;

    freq = FreqToChan(freq, Set_Band);

    ReadRegister(10);
    ChipID = ((uint16) ReadBuffer[8] << 8) + ReadBuffer[9];

    if (0x5804 == ChipID) //5807SP
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_SP, sizeof(InitBuffer_SP));
        WriteNum = sizeof(InitBuffer_NP);
        WriteBuffer[7] = 0x80; //8
        WriteBuffer[7] |= level; //搜台门限
        RDA_model = 1;
    }
    else if (0x5801 == ChipID) //5807HP
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_HP, sizeof(InitBuffer_HP));
        WriteNum = sizeof(InitBuffer_NP);
        TH_Slevel = level >> 1;
        WriteBuffer[67] = TH_Slevel & 0x3f;
        TH_Slevel = level << 7;
        WriteBuffer[68] = TH_Slevel + level; //搜台门限
        RDA_model = 2;
    }
    else //5807P
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_NP, sizeof(InitBuffer_NP));
        WriteNum = sizeof(InitBuffer_NP);
        WriteBuffer[7] = 0x80; //8
        WriteBuffer[7] |= level; //搜台门限
        RDA_model = 3;
    }

    WriteBuffer[4] &= (uint8) 0xf0;
    WriteBuffer[4] |= Set_Band; //设置频段

    WriteBuffer[1] &= (uint8) 0xfe; //禁止SEEK ,注意硬件SEEK方式,在设置频点\静音 时,必须disable seek!!!!
    WriteBuffer[2] |= (0x01 << 0); //Enable FM
    WriteBuffer[3] = (uint8)(freq >> 8); //设置要写入的频率
    WriteBuffer[4] &= 0x3f;
    WriteBuffer[4] |= (uint8)(freq % 256);
    WriteBuffer[4] |= (0x01 << 4); //enable tune

    return TRUE;
}
/*
 ***** ***************************************************************************
 * int  sFM_Init(uint8 band, uint8 level, void* null3)
 *
 * Description : FM初始化,设置搜索门限,波段,步进等
 *
 * Arguments : 波段选择值,门限
 *                   band=0: 中国/美国电台
 *                   band=1: 日本电台
 *                   band=2: 欧洲电台
 * Returns : 若初始化成功,则返回1,否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_Init(uint8 band, uint8 level, uint16 freq)
{
    int result;
    uint16 ChipID;
    uint8 TH_Slevel;

    if (band == 0)
    {
        //87~108, 100k
        Set_Band = 0;
    }
    else if (band == 1)
    {
        //76~91, 100k
        Set_Band = 0x04;
    }
    else
    {
        //87~108, 50k
        Set_Band = 0x02;
    }

    WriteBuffer[0] = 0x20; //写入的第一个字节为FM模组的I2C器件地址
    WriteBuffer[1] = 0x00;
    WriteBuffer[2] = 0x02;

    if (freq != 0)
    {
        return sFM_Restore_Global_Data(band, level, freq);
    }
    WriteRegister(2); //FM reset
    sys_mdelay(10);

    ReadRegister(10);
    ChipID = ((uint16) ReadBuffer[8] << 8) + ReadBuffer[9];

    if (0x5804 == ChipID) //5807SP
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_SP, sizeof(InitBuffer_SP));
        WriteNum = sizeof(InitBuffer_SP);
        WriteBuffer[7] = 0x80; //8
        WriteBuffer[7] |= level; //搜台门限
        RDA_model = 1;
    }
    else if (0x5801 == ChipID) //5807HP
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_HP, sizeof(InitBuffer_HP));
        WriteNum = sizeof(InitBuffer_HP);
        TH_Slevel = level >> 1;
        WriteBuffer[67] = TH_Slevel & 0x3f;
        TH_Slevel = level << 7;
        WriteBuffer[68] = TH_Slevel + level; //搜台门限
        RDA_model = 2;
    }
    else //5807P
    {
        libc_memcpy(&WriteBuffer[1], InitBuffer_NP, sizeof(InitBuffer_NP));
        WriteNum = sizeof(InitBuffer_NP);
        WriteBuffer[7] = 0x80; //8
        WriteBuffer[7] |= level; //搜台门限
        RDA_model = 3;
    }
    WriteBuffer[4] &= (uint8) 0xf0;
    WriteBuffer[4] |= Set_Band; //设置频段

    result = FM_Standby_Drv(1);

    sys_mdelay(500); //延时500MS，等待FM模组稳定
    return result;
}

/*
 ********************************************************************************
 *             int sFM_Standby(void* null1, void* null2, void* null3)
 *
 * Description : FM standby
 *
 * Arguments : NULL
 *
 * Returns : 若设置standby成功,则返回1,否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_Standby(void* null1, void* null2, void* null3)
{
    int result;
    result = FM_Standby_Drv(0);
    return result;
}

/*
 ************************************************* *******************************
 * int sFM_Mute(FM_MUTE_e mode, void* null2, void* null3)
 *
 * Description : FM静音设置
 *
 * Arguments : 是否静音,0为取消静音,1为静音
 *
 * Returns : 若设置静音控制成功,则返回1,否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_Mute(FM_MUTE_e mode, void* null2, void* null3)
{
    int result;
    WriteBuffer[1] &= (uint8) 0xfe; //disable seek ,注意硬件SEE K方式,在设置频点\静音时,必须disable seek!!!!
    if (mode == releaseMUTE)
    {
        WriteBuffer[1] |= (0x01 << 6);
    }
    else
    {
        WriteBuffer[1] &= ~(0x01 << 6);
    }
    result = WriteRegister(2);
    return result;
}

/*
 ********************************************************************************
 *            int FM_Standby_Drv(uint8 StandbyFlag)
 *
 * Description : FM standby或wake up
 *
 * Arguments : standby标志,1表示wake up,0表示standby
 *
 * Returns : 若 standby或wake up成功，返回1;否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int FM_Standby_Drv(uint8 StandbyFlag)
{
    int result;
    WriteBuffer[2] &= (uint8) 0xfe; //FM disable
    if (StandbyFlag != 0)
    {
        WriteBuffer[2] |= (0x01 << 0); //FM enable
    }
    result = WriteRegister(WriteNum);
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_SetBand(uint8 band, void* null2, void* null3)
 *
 * Description : FM  波段设置
 *
 * Arguments : band    0:  中国/ 美国波段 1: 日本波段  2: 欧洲波段
 *
 * Returns : 设置成功，返回1;  否则，返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_SetBand(radio_band_e band, void* null2, void* null3)
{
    int result;
    if (band == Band_China_US)
    {
        Set_Band = 0;
    }
    else if (band == Band_Japan)
    {
        Set_Band = 0x04;
    }
    else
    {
        Set_Band = 0x02;
    }

    WriteBuffer[4] &= (uint8) 0xf0;
    WriteBuffer[4] |= Set_Band; //设置频段, 同时会设置space
    result = WriteRegister(4);
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_SetThrod(uint8 level, void* null2, void* null3)
 *
 * Description : FM  搜台门限设置
 *
 * Arguments : level---搜台门限值
 *
 * Returns : 设置成功，返回1;  否则，返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_SetThrod(uint8 level, void* null2, void* null3)
{
    int result;
    uint8 TH_Slevel, num;

    if (RDA_model == 2)
    {
        //5807HP
        TH_Slevel = level >> 1;
        WriteBuffer[67] = TH_Slevel & 0x3f;
        TH_Slevel = level << 7;
        WriteBuffer[68] = TH_Slevel + level; //搜台门限
        num = 68;
    }
    else
    {
        //5807SP or 5807P
        WriteBuffer[7] = 0x80;
        WriteBuffer[7] |= level; //搜台门限
        num = 7;
    }
    result = WriteRegister(num);
    return result;
}

