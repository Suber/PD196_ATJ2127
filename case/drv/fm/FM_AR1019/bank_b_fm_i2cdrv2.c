/*
 ******************************************************************************
 *                               US212A
 *                            Module: Radio
 *                 Copyright(c) 2002-2010 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * Descripton: main module of radio
 * Module:  AR1019/AR1010
 * History:
 *      <author>    <time>           <version >             <desc>
 *       Mikeyang    2010-11-25 14:35     1.0             build this file
 ******************************************************************************
 */
#include "fm_drv.h"

extern const uint8 InitBuffer[36];
extern uint8 WriteBuffer[];
/* CHAN<9:0>*/
extern uint16 Freq_RF;
extern uint8 ReadBuffer[];
extern uint8 Set_Band;
extern uint8 Space50k;

extern uint8 WriteOneFrame(uint8 Addr, uint8 Count)
__FAR__;
extern uint8 ReadOneFrame(uint8 Addr)
__FAR__;
extern uint8 WaitSTC(void)
__FAR__;
extern uint8 WriteRegister(uint8 Addr, uint8 Count)
__FAR__;
extern uint8 ReadRegister(uint8 RegAddr)
__FAR__;

#if 1
extern uint8 CheckStation(void)
__FAR__;
#endif

uint16 FreqToChan(uint16 freq, uint8 space);
uint16 ChanToFreq(uint16 chan, uint8 space);

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
    uint8 chan_bit0;
    uint16 freq_tmp;

    band = band;
    level = level;

    freq = FreqToChan(freq, Space50k);

    chan_bit0 = (uint8)(freq % 2);
    freq_tmp = (freq >> 1); // CHAN<9:1>

    WriteBuffer[1] |= (0x01 << 0); //FM power up, Enable

    WriteBuffer[4] &= (uint8) 0xfd; //clear tune
    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //clear SEEK

    //set band/space/chan,enable tune
    WriteBuffer[4] &= (uint8) 0xf6;
    WriteBuffer[4] |= (chan_bit0 << 3);
    WriteBuffer[4] |= (uint8)(freq_tmp >> 8);
    WriteBuffer[5] = (uint8)(freq_tmp % 256);

    return TRUE;

}

/*
 ***** ***************************************************************************
 * int  sFM_Init(uint8 band, uint8 level, void* null3)
 *
 * Description : FM初始化,设置搜索门限,波段,步进等
 *
 * Arguments : 波段选择值,门限
 *                   band=0: 中国/美国电台，步进100KHz
 *                   band=1: 日本电台，步进100KHz
 *                   band=2: 欧洲电台，步进50KHz
 *
 * Returns : 若初始化成功,则返回1,否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_Init(uint8 band, uint8 level, uint16 freq)
{
    int result;
    libc_memcpy(WriteBuffer, InitBuffer, sizeof(InitBuffer));
    Space50k = 0;
    if (band == 1)
    {
        /* 76MHz~90MHz*/
        Set_Band = 0x10;
    }
    else
    {
        /*87.5MHz~108MHz*/
        Set_Band = 0;
        if (band == 2)
        {
            /* 欧洲电台，步进50k */
            Space50k = 0x01;
        }
    }

    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //初始化,disable SEEK

    WriteBuffer[6] &= (uint8) 0xe7;
    WriteBuffer[6] |= Set_Band; //设置频段
    WriteBuffer[30] &= (uint8) 0xfe;
    WriteBuffer[30] |= Space50k; //设置步进

    WriteBuffer[7] &= (uint8) 0x80;
    WriteBuffer[7] |= level; //搜台门限<6:0>

    WriteBuffer[1] &= ~(0x01 << 0); //Enable bit = 0

    if (freq != 0)
    {
        return sFM_Restore_Global_Data(band, level, freq);
    }

    result = WriteRegister(0x00, 0x02); //FM standby  ， 写2 个字节寄存器内容，只设置了FM_Enable
    if (result != 0)
    {
        result = WriteRegister(0x01, 34); //init, from R1 to R17
        if (result != 0)
        {
            WriteBuffer[1] |= (0x01 << 0); //FM power up, Enable
            result = WriteRegister(0x00, 2);
        }
    }
    if (result != 0)
    {
        result = WaitSTC(); //wait STC
    }
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
    WriteBuffer[1] &= ~(0x01 << 0); //FM Enable bit = 0
    result = WriteRegister(0x00, 0x02);
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_Mute(FM_MUTE_e mode, void* null2, void* null3)
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
    if (mode == releaseMUTE)
    {
        WriteBuffer[3] &= ~(0x01 << 1); //release mute
    }
    else
    {
        WriteBuffer[3] |= (0x01 << 1); //enable mute
    }
    result = WriteRegister(0x01, 0x02);
    return result;
}

/*
 ****************************************************************************************
 *           uint16 FreqToChan(uint16 freq, uint8 space)
 *
 * Description : 将频点转换成写寄存器的值
 *
 * Arguments :  freq:  需进行转换的频率( 实际频率khz 为单位的低2 字节)
 *			   space:  步进   0: 100K    1: 50K
 * Returns :	   CHAN<9:0>
 *
 * Notes :  各个频率值( 以1khz 为单位)  的最高bit  恒为1
 *
 * space 50K    CHAN<9:0> = FREQ ( 以50K 为单位)  -  69*20
 * else             CHAN<9:1> = FREQ ( 以100K 为单位) - 69*10
 *****************************************************************************************
 */
uint16 FreqToChan(uint16 freq, uint8 space)
{
    uint16 Chan, tmp_freq;
    uint32 temp;

    //实际频率值，以khz 为单位
    temp = (uint32) freq + 0x10000;

    //步进100K
    if (space == 0)
    {
        tmp_freq = (uint16)(temp / 100);
        //CHAN<9:1>
        Chan = tmp_freq - 69* 10 ;
        //CHAN<9:0>
        Chan = Chan*2;
    }
    //步进50K

    else
    {
        tmp_freq = (uint16)(temp/50);
        //CHAN<9:0>
        Chan = tmp_freq - 69*20;
    }

    return Chan;
}

/*
 ********************************************************************************
 *           uint16 ChanToFreq(uint16 chan, uint8 space)
 *
 * Description : 将读出的寄存器频率值转换成上层需要的频率
 *
 * Arguments :  chan:读出的频率值 CHAN<9:0>
 *                    space :  步进
 *
 * Returns :	   转换出来的实际频点值(以1KHz 为单位的低2   字节)
 *
 * Notes :
 *
 ********************************************************************************
 */
uint16 ChanToFreq(uint16 chan, uint8 space)
{
    uint16 Freq;
    uint32 temp;

    //步进100K  ，FREQ ( 以100K 为单位) = CHAN<9:1> +  69*10
    if (space == 0)
    {
        Freq = chan / 2 + 69* 10 ;
        //转换为以1khz  为单位
        temp = (uint32)Freq * 100;
    }
    //步进50K ， FREQ ( 以50K 为单位) = CHAN<9:0> +  69*20

    else
    {
        Freq = chan + 69*20;
        //转换为以1khz  为单位
        temp = (uint32)Freq * 50;
    }

    //最高位恒为1
    Freq = (uint16)(temp & 0x0ffff);
    return Freq;
}

/*
 ************************************************* *******************************
 * int  sFM_SetBand(radio_band_e band, void* null2, void* null3)
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
    uint8 space_bak;

    space_bak = Space50k;
    Space50k = 0;

    if (band == Band_Japan)
    {
        Set_Band = 0x10;
    }
    else
    {
        Set_Band = 0;
        if (band == Band_Europe)
        {
            Space50k = 1;
        }
    }

    WriteBuffer[6] &= (uint8) 0xe7;
    WriteBuffer[6] |= Set_Band; //设置频段
    WriteBuffer[30] &= (uint8) 0xfe;
    WriteBuffer[30] |= Space50k; //设置步进

    result = WriteRegister(0x03, 2);
    result = WriteRegister(0x0f, 2);
    if (result == 0)
    {
        //设置频段失败，恢复space
        Space50k = space_bak;
    }
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

    WriteBuffer[7] &= (uint8) 0x80;
    WriteBuffer[7] |= level; //搜台门限
    result = WriteRegister(0x03, 2);
    return result;
}


#if 0
/*
 ********************************************************************************
 *            int  sFM_Search(uint16 freq,  uint8 direct, void* null3)
 *
 * Description : FM  软件搜台，tune 单频点，判断是否有效电台
 *
 * Arguments :  软件搜台方式,需设置的频率值及搜索方向
 *
 * Returns : 电台为真台,则返回1;否则,返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
//改版前，软件搜台代码暂放bank
int sFM_Search(uint16 freq, uint8 direct, void* null3)
{
    int result;
    uint16 TempFreq;
    uint16 freq_tmp;
    uint8 chan_bit0;
    uint8 ret;

    //切换到single_seek 模式
    WriteBuffer[28] |= (uint8) 0x02;
    ret = WriteRegister(0x0E, 0x02);

    sFM_Mute(SetMUTE, 0, 0);

    if (direct == 0)
    {
        //seek down
        if (Space50k == 0x01)
        {
            freq_tmp = freq + 50;
        }
        else
        {
            freq_tmp = freq + 100;
        }
    }
    else
    {
        //seek up
        if (Space50k == 0x01)
        {
            freq_tmp = freq - 50;
        }
        else
        {
            freq_tmp = freq - 100;
        }
    }
    TempFreq = FreqToChan(freq_tmp, Space50k); //将电台频率转换成寄存器需写入的值
    chan_bit0 = (uint8)(TempFreq % 2);
    freq_tmp = (TempFreq >> 1); // CHAN<9:1>

    WriteBuffer[4] &= (uint8) 0xf4; //disable tune
    WriteBuffer[4] |= (chan_bit0 << 3);
    WriteBuffer[4] |= (uint8)(freq_tmp >> 8);
    WriteBuffer[5] = (uint8)(freq_tmp % 256); //update chan
    ret = WriteRegister(0x02, 0x02);

    WriteBuffer[6] &= (uint8) ~(0x01 << 6); //disable seek
    ret = WriteRegister(0x03, 0x02);

    WriteBuffer[6] &= (uint8) 0x7f; //设置SEEK direct
    WriteBuffer[6] |= (uint8)(direct << 7); //设置SEEK direct
    WriteBuffer[6] |= (uint8)(0x01 << 6); //enable SEEK
    ret = WriteRegister(0x03, 0x02);

    //已经启动single_seek
    if (ret != 0)
    {
        //软件搜台，判断所设置频点是否有效电台
        if (CheckStation() == 1) // check station
        {
            //0x17700~~96MHz, 过滤96MHz 频点
            if (freq != 0x7700)
            {
                result = 1;
            }
            else
            {
                result = 0;
            }
        }
        else
        {
            result = 0;
        }

    }
    else
    {
        result = 0;
    }

    //切换回正常模式
    WriteBuffer[28] &= (uint8) 0xfd;
    ret = WriteRegister(0x0E, 0x02);
    return result;
}
#endif

