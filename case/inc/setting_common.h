/*******************************************************************************
 *                              us212A
 *                            Module: SETTING AP
 *                 Copyright(c) 2003-20012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       johnsen     2011-9-2 9:18     1.0             build this file 
*******************************************************************************/
/*!
 * \file     setting_common.h
 * \brief    setting 应用公共数据结构
 * \author   johnsen
 * \version  1.0
 * \date     2011-9-9
*******************************************************************************/

#ifndef _SETTING_COMMON_H
#define _SETTING_COMMON_H

#include "psp_includes.h"
#include "enhanced.h"
#include "applib.h"
#include "case_type.h"
#include "common_func.h"

#define VALUE_MUSIC_REPEAT_OFF 0x00 //此值由music AP决定
#define VALUE_MUSIC_REPEAT_ONE 0x01
#define VALUE_MUSIC_REPEAT_ALL 0x02
#define VALUE_MUSIC_REPEAT_INTRO 0x03

#define VALUE_MUSIC_SHUFFLE_OFF 0x00  //此值由music AP决定
#define VALUE_MUSIC_SHUFFLE_ON 0x10

#define VALUE_FULLSOUND_OFF    0x00    //此值由music AP决定
#define VALUE_FULLSOUND_ON     0x01

#define VALUE_EQ_OFF     0x00    //此值由music AP决定
#define VALUE_EQ_ROCK    0x01
#define VALUE_EQ_FUNK    0x02
#define VALUE_EQ_HIP_HOP 0x03
#define VALUE_EQ_JAZZ    0x04
#define VALUE_EQ_CLASSICAL    0x05
#define VALUE_EQ_TECHNO       0x06
#define VALUE_EQ_CUSTOM       0x07

#define VALUE_SRS_OFF 0x0
#define VALUE_WOWHD_ON 0x01
#define VALUE_USER_SRS_ON 0x2

#define COMMAND_EQ_NORMAL 0
#define COMMAND_EQ  0x07
#define COMMAND_FULLSOUND 0x07
#define COMMAND_SRS 0x4f
#define COMMAND_VPS 0x77

#define VALUE_FROM_DEFAULT    0x04      //此值由music AP决定
#define VALUE_FROM_ALL_SONGS  0x00  
#define VALUE_FROM_ALBUM      0x01
#define VALUE_FROM_ARTIST     0x02
#define VALUE_FROM_GENRE      0x03

#define VALUE_SHUT_DOWN_TIMER_OFF 0x00
#define VALUE_SHUT_DOWN_TIMER_ON 0x01

#define VALUE_SHUT_DOWN_TIME_MIN 1
#define VALUE_SHUT_DOWN_TIME_MAX 99
#define VALUE_SHUT_DOWN_TIME_STEP 1

#define VALUE_BUTTON_YESNO_YES  0
#define VALUE_BUTTON_YESNO_NO   1

#define VALUE_LIGHTNESS_MAX  11
#define VALUE_LIGHTNESS_MIN  0
#define VALUE_LIGHTNESS_STEP 1
#define VALUE_LIGHTNESS_MAX_IN_DRV 15   //此值需与显示驱动一致，由驱动决定

#define VALUE_KEY_TONE_ON  1
#define VALUE_KEY_TONE_OFF 0
/*!
 * \brief
 *  comval_t 系统公共变量组合结构体
 */
typedef struct
{
    /*! magic，值为 0x55AA 时有效 */
    uint16 magic;
    
    //显示相关项
    /*! 背光亮度等级 */
    uint8 lightness;
    /*! 主题 0：蓝色 1：绿色 2：灰色 3：粉红色 4：红色 */
    uint8 theme;
    //+4bytes
    /*! 屏幕保护 0: 无 1: 数字时钟 2：相册图画 3：关闭屏幕 4：演示模式 */
    uint8 screen_saver_mode;
    /*! 背光时间 以0.5秒为单位，0表示常亮 */
    uint8 light_timer;
    /*! 屏幕保护时间 以0.5秒为单位，0表示无屏幕保护 */
    uint8 screen_saver_timer;
    /*! 省电关机时间 以0.5秒为单位，0表示无省电关机 */
    uint8 poweroff_timer;
    //+8bytes
    /*! 定时返回正在播放界面 以0.5秒为单位，0表示无须返回 */
    uint8 playing_timer;
    /*! 定时关机9睡眠）时间 以分钟为单位，0表示不定时关机 */
    uint8 sleep_timer;
    
    //日期和时间
    /*! 时间格式 0：12小时制 1：24小时制 */
    uint8 time_format;
    /*! 日期格式 0：DD_MM_YYYY 1：MM_DD_YYYY 2：YYYY_MM_DD */
    uint8 date_format;
    //+12bytes
    /*! 界面语言, 此值由显示驱动决定 */
    uint8 language_id;
    /*! 电脑连接首选项 0：MSC 1：MTP */
    uint8 online_device;
    /*! CD安装程序 autorun支持选择 0 ：autorun，1： 支持autorun */
    uint8 autorun_set;
    /*! 支持卡选择  0:不支持  1:支持 */
    uint8 support_card;
    //+16bytes
    /*! 音量限制 */
    uint8 volume_limit;
    /*! 当前音量值 */
    uint8 volume_current;
    //图片设置
    /*! 幻灯片间隔时间 */
    uint8 slide_time;
    /*! 幻灯片重复模式 */
    uint8 slide_repeat_mode;
    //+20bytes 
    /*! 幻灯片 shuffle 功能 */
    uint8 slide_shuffle;
    /*! 屏幕变暗到关掉屏幕时间，以0.5秒为单位，0表示无此功能 */
    uint8 dark_lightoff_timer;
    /*! 屏保到关掉屏幕时间，以分钟为单位，0表示无此功能 */
    uint8 screen_lightoff_timer;
    /*!按键音开关*/
    uint8 key_tone;
} comval_t;

/*!
 * \brief
 *  music_comval_t 音乐设置变量组合结构体
 */
typedef struct
{
    /*! 音乐来源 */
    uint8 music_from;
    /*! 音乐循环方式 */
    uint8 music_repeat_mode;
    /*! 音乐随机不重复开关 */
    uint8 music_shuffle;
    /*! fullsound 音效开关 */
    uint8 fullsound;
    /*! srs 音效开关 */
    uint8 srs;
    /*! eq模式设定 */
    uint8 eq_setting;
    /*! 变速播放 */
    int8  variable_playback;
    uint8 reserved;
    /*! eq用户参数表 */
    int8 eq_parameter[12];
    /*! srs用户参数表 */
    int8 srs_parameter[12];
} music_comval_t;

/*!
 * \brief
 *  setting_comval_t 设置应用全局参数结构体
 */
typedef struct
{
    /*! 系统公共变量组合结构体 */
    comval_t g_comval;
    /*! 音乐设置变量组合结构体 */
    music_comval_t music_comval;
} setting_comval_t;

/*!
 * \brief
 *  browser的VM全局变量
 */
typedef struct browser_var_t
{
    //browser的路径信息
    file_path_info_t path;
    uint8 prev_enter_mode;
    //browser魔数
    uint16 magic;
    //uint16 reserved;
} browser_var_t;


#endif //_SETTING_COMMON_H
