/********************************************************************************
 *                            Module: radio_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-08 15:00     1.0             build this file
********************************************************************************/
#ifndef __FMENGINE_H__
#define __FMENGINE_H__

#include "psp_includes.h"
#include "fm_interface.h"


typedef enum
{
    STATUS_PLAY = 0,                   // 正常播放状态
    STATUS_PAUSE,                       //正常暂停状态(静音状态)
    STATUS_ERROR,                      //硬件出错
} FM_Playstatus_e;

typedef enum
{
    STERE = 0,               //[0]立体声
    MONO,                    //[1]单声道        
} FM_Audio_e;

typedef enum
{
    releaseMUTE = 0,        //解除静音
    SetMUTE,                   //静音        
} FM_MUTE_e;

typedef enum
{
    HARDSEEK = 0,            //采用硬件seek 模式
    SOFTSEEK,                  //采用软件seek 模式
} FM_SeekMode_e;

typedef enum
{
    DIR_UP = 0x10,             //向上搜台
    DIR_DOWN = 0x20,       //向下搜台
}FM_SeekDir_e;


/* 电台波段模式分为播放模式和菜单选择模式
** 如果在非用户电台播放模式下，播放的波段和
** 菜单中选择的波段一致。
** 用户电台播放时，播放模式的波段为Band_MAX 
** 菜单中的波段维持不变
** 此时如果选择清除预设或保存到预设，都是将
** 用户电台的频点保存到当前菜单波段指定的
** 预设列表或者从菜单波段指定的列表选择频点删除
** 播放用户电台时，如果在菜单中改变菜单波段
** 的选择值，会进行波段切换，并更改为预设播放模式*/
typedef enum                    
{
    Band_China_US = 0,              //china/USA band   87500---108000,  step 100kHz
    Band_Japan,                         //Japan band   76000---90000, step 100kHz
    Band_Europe,                       //Europe band  87500---108000, step 50kHz
    Band_MAX                           //播放用户电台时，播放模式可以为Band_MAX
} radio_band_e;


/*硬件seek 状态枚举*/
typedef enum
{
    HARDSEEK_NOT_START = 0,             //未启动硬件搜台或者此轮硬件搜台未搜到有效台
    HARDSEEK_DOING,                           //硬件搜台过程中
    HARDSEEK_COMPLETE,                     //此轮硬件搜台过程结束并搜到有效台
} hardseek_status_e;

typedef struct
{
    UINT16            FM_CurrentFreq;           //当前频率
    UINT16            FM_SignalStg;             //当前频点信号强度
    FM_Audio_e        FM_Stereo_Status;        //0，立体声；非0，单声道
    radio_band_e      FM_CurBand;                //当前波段 
    FM_Playstatus_e   FM_VolStatus;               //当前所处状态
    hardseek_status_e STC_flag;                      //硬件seek 是否完成标志，主要用于硬件seek 需要
} Engine_Status_t;


typedef enum
{
    PA_MODE = 0,                                      //非直驱
    PA_DDV_MODE,                               //直驱
} PA_DDV_Mode_e;

/* FM 引擎配置信息结构体*/
typedef struct
{
    uint16  magic;
    PA_DDV_Mode_e  paddv_mode;               //PA 直驱模式或非直驱模式          
    uint8  fm_threshold;                               //模组默认搜台门限值                          
} FMEngine_cfg_t;

/* FM 模组状态枚举*/
typedef enum
{
    MODULE_NO_INIT = 0,              //模组处于未初始化状态
    MODULE_WORK,                       //模组初始化完成，处于正常工作状态
    MODULE_STANDBY,                  //模组处于Standby 状态
} Module_Status_e;

#endif //__FMENGINE_H__
