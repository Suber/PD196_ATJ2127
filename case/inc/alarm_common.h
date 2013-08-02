/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : setting_common.h
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
#ifndef _ALARM_COMMON_H
#define _ALARM_COMMON_H

#include "psp_includes.h"
#include "enhanced.h"
#include "applib.h"
#include "case_type.h"
#include "common_func.h"

#define  ALARM_FUN //宏定义alarm功能
#define  ALARMFM   //宏定义ALARMFM功能
/*秒表结构体*/
typedef struct
{    
    uint8 Ms;    
    uint8 Second;    
    uint8 Minute;    
    uint8 Hour;
} StopWatch_time_t;
/*闹钟循环日期*/
typedef struct
{
    uint16 year;
    uint8 month;
    uint8 day;
} cycle_one_time_s;

/*闹钟循环星期*/
typedef struct
{
    uint8 week_day[7];
} cycle_select_week_s;

/*闹钟循环方式*/
typedef struct
{
    cycle_one_time_s  one_time;
    cycle_select_week_s  select_week;//Reserved
} cycle_t;

/*ALARM结构体*/
typedef struct
{
    //uint16 magic;
    uint8  enable;
    time_t start_time;  //闹钟1的时间设置
    uint8  cycle_flag;  //闹钟日期的设置标识：0-单次，1-每天，2-按星期
    cycle_t cycle;      //闹钟日期的设置方式
    uint8  ring_flag;   //0-内置 1-flash 2-card
    file_path_info_t path;  //响应闹钟时所播放音乐的路径
    uint8  volume;          //闹钟音乐播放时，音量的大小
    uint8  alarm_count; //Snooze标志及次数
} alarm_alarm_t;

/*ALARMFM结构体*/
typedef struct
{
    //uint16 magic;
    uint8  enable; //使能
    time_t start_time;  //定时FM的时间设置
    time_t end_time;    //定时FM的时间设置
    uint8  cycle_flag;  //周期循环标识：0-单次，1-每天，2-按星期
    cycle_t cycle;      //循环结构
    uint16 frequency;   //FM频段
    uint8  volume;      //FM音量
    uint8  fmrec_enable;   //录音使能标志
    //uint8  timer_flag;
} alarm_fm_t;

/*Reserved*/
typedef struct
{
    //uint16 magic;
    uint8  enable;  //定时录音的开关标识
    time_t start_time; // 开始录音的时间
    time_t end_time;   // 结束录音的时间
    uint8  rcycle_flag;  //定时录音日期设置标识：0 -单次，1-每天，2-按星期
    cycle_t cycle; //定时录音日期的设置方式
    uint8  rec_type;    //
} alarm_record_t;


typedef struct
{
    uint16 magic;  //MAGIC标志
    uint8  timer_flag; //标志闹钟种类
    alarm_alarm_t g_alarm;
    alarm_fm_t   g_aufm;
    time_t  snooze_time; //保存snooze时间
    //alarm_record_t g_arecord; //Reserved
}alarm_vars_t;

#endif //_ALARM_COMMON_H

