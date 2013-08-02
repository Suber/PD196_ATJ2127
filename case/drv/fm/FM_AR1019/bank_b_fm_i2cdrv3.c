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

extern uint8 WriteBuffer[];
/* CHAN<9:0>*/
extern uint16 Freq_RF;
extern uint8 ReadBuffer[];
extern uint8 Set_Band;
extern uint8 Space50k;
extern uint16 signal_stg;
extern uint8 ST_info;

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
extern uint16 FreqToChan(uint16 freq, uint8 space)
__FAR__;
extern uint16 ChanToFreq(uint16 chan, uint8 space)
__FAR__;

/*
 ************************************************* *******************************
 * int  sFM_GetBand(void* band, void* null2, void* null3)
 *
 * Description : 获取当前波段信息
 *
 * Arguments : band      存放返回信息的地址
 *
 * Returns : 获取成功，返回1;  否则，返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetBand(void* band, void* null2, void* null3)
{
    radio_band_e* bandinfo;
    int result = 1;

    bandinfo = (radio_band_e*) band;

    //不用去取，直接从驱动层全局变量获取到
    if (Set_Band == 0x10)
    {
        *bandinfo = Band_Japan;
    }
    else
    {
        if (Space50k == 1)
        {
            *bandinfo = Band_Europe;
        }
        else
        {
            *bandinfo = Band_China_US;
        }
    }
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_GetFreq(void* freq, void* null2, void* null3)
 *
 * Description : 获取当前频率信息
 *
 * Arguments : freq      存放返回信息的地址
 *
 * Returns : 获取成功，返回1;  否则，返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetFreq(void* freq, void* null2, void* null3)
{
    int result;
    uint16* pfreq;
    uint8 tmp;

    pfreq = (uint16*) freq;
    //read status reg
    result = ReadRegister(0x13);
    if (result == 1)
    {
        Freq_RF = (uint16) ReadBuffer[0] * 4;
        tmp = (ReadBuffer[1] & (uint8) 0x80) >> 6;
        tmp += (ReadBuffer[1] & (uint8) 0x01);
        //CHAN<9:0>
        Freq_RF += tmp;
        *pfreq = ChanToFreq(Freq_RF, Space50k);
    }
    else
    {
        result = 0;
    }
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_GetIntsity(void* intensity, void* null2, void* null3)
 *
 * Description : 获取当前电台信号强度
 *
 * Arguments : intensity      存放返回信息的地址
 *
 * Returns : 获取成功，返回1;  否则，返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetIntsity(void* intensity, void* null2, void* null3)
{
    int result;
    uint16* p_intensity;

    p_intensity = (uint16*) intensity;
    result = ReadRegister(0x12);
    if (result == 1)
    {
        signal_stg = (uint16)((ReadBuffer[0] & (uint8) 0xfe) >> 1);
        *p_intensity = signal_stg;
    }
    else
    {
        result = 0;
    }
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_GetAnten(void* antenna, void* null2, void* null3)
 *
 * Description : 获取当前天线状态
 *
 * Arguments : antenna      存放返回信息的地址
 *
 * Returns : 获取成功，返回1;  否则，返回0
 *
 * Notes :  天线状态检测与硬件相关，后续添加
 *
 ********************************************************************************
 */
int sFM_GetAnten(void* antenna, void* null2, void* null3)
{
    int result;
    uint8* p_antenna;
    unsigned char adcdat;

    p_antenna = (uint8*) antenna;

    //Demo 用LRADC1 进行耳机检测
    adcdat = act_readb(LRADC1_DATA) & 0x3f;
    //天线连接上
    if ((adcdat >= 0x2B) && (adcdat < 0x34))
    {
        *p_antenna = 1;
    }
    else
    {
        *p_antenna = 0;
    }
    //获取状态成功
    result = 1;
    return result;
}

/*
 ************************************************* *******************************
 * int  sFM_GetStereo(void* stereo, void* null2, void* null3)
 *
 * Description : 获取当前电台立体声信息
 *
 * Arguments : stereo      存放返回信息的地址
 *
 * Returns : 获取成功，返回1;  否则，返回0
 *
 * Notes :
 *
 ********************************************************************************
 */
int sFM_GetStereo(void* stereo, void* null2, void* null3)
{
    int result;
    FM_Audio_e* p_stereo;
    uint8 tmp;

    p_stereo = (FM_Audio_e*) stereo;
    result = ReadRegister(0x13);
    if (result == 1)
    {
        tmp = ReadBuffer[1] & 0x08;
        if (tmp == 0)
        {
            //mono
            ST_info = 1;
        }
        else
        {
            //stereo
            ST_info = 0;
        }
        *p_stereo = (FM_Audio_e) ST_info;
    }
    else
    {
        result = 0;
    }
    return result;
}

