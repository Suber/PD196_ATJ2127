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
extern uint16 signal_stg; //驱动层，电台信号强度信息
extern uint8 ST_info; //驱动层:   电台立体声信息  0~~立体声   1~~单声道
extern uint8 hardseek_flag;

extern uint16 FreqToChan(uint16 freq, uint8 mode)
__FAR__;
extern uint16 ChanToFreq(uint16 Chan, uint8 mode)
__FAR__;
extern void DisAssemble_WriteBuffer(void)
__FAR__;
extern int WriteRegister(uint8 count)
__FAR__;//写寄存器
extern uint8 ReadRegister(uint8 count)
__FAR__; //读寄存器

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
    //中国/ 美国电台
    if (Set_Band == 0)
    {
        *bandinfo = Band_China_US;
    }
    //日本电台
    else if (Set_Band == 0x04)
    {
        *bandinfo = Band_Japan;
    }
    //欧洲电台
    else if (Set_Band == 0x02)
    {
        *bandinfo = Band_Europe;
    }
    else
    {
        result = 0;
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

    pfreq = (uint16*) freq;
    result = ReadRegister(10);
    if (result == 1)
    {
        //获取成功，进行转换
        DisAssemble_WriteBuffer();
        *pfreq = ChanToFreq(Freq_RF, Set_Band);
    }
    else
    {
        //获取失败
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
    result = ReadRegister(4);
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
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
    uint32 value;

    p_antenna = (uint8*) antenna;
    //天线连接上，EVB 用GPIO_A1 检测耳机插入
    act_writel((act_readl(GPIO_AOUTEN) & (0xfffffffd)), GPIO_AOUTEN);
    act_writel((act_readl(GPIO_AINEN) | (0x00000002)), GPIO_AINEN);
    sys_mdelay(20);
    value = act_readl(GPIO_ADAT) & (0x00000002);
    if (value == 0)
    {
        *p_antenna = 1;
    }
    else
    {
        *p_antenna = 0;
    }
    //获取成功
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

    p_stereo = (FM_Audio_e*) stereo;
    result = ReadRegister(4);
    if (result == 1)
    {
        DisAssemble_WriteBuffer();
        *p_stereo = (FM_Audio_e) ST_info;
    }
    else
    {
        result = 0;
    }
    return result;
}

