/*******************************************************************************
 *                              us212a
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
/*
 * \file     ap_config.h
 * \brief    config应用的头文件
 * \author   zhangxs
 * \version 1.0
 * \date  2011/09/04
 *******************************************************************************/
#ifndef _AP_CONFIG_H
#define _AP_CONFIG_H

#include  "config_res.h"
#include  "config_sty.h"
#include  "psp_includes.h"
#include  "case_include.h"

#define SWITCH_ON           0
#define SWITCH_OFF          1
#define NANDFLASH_RECLAIM   2

#define     VM_AP_RECORD_WAV	    VM_AP_RECORD+0x200
//前台应用总数
#define    MAX_APP_NUM    14
//创建应用函数的传参定义
#define    PARAM_ENTER_NORMAL   0
#define    PARAM_ENTER_ALARM    1
//创建应用函数的传参定义
#define    GUI_OUTOF_STANDBY      0
#define    ALARM_OUTOF_STANDBY    1

typedef enum
{
    BACKLIGHT_OFF, BACKLIGHT_ON
} backlight_type_e;

typedef struct
{
    uint16 magic;       //VM 变量魔数
    uint8 ap_id;        //关机的应用
    uint8 engine_state; //引擎状态
    uint8 keyID[16];    //按键定义数组
} g_config_var_t;

typedef struct
{
    uint16 active;     //此参数没用到，只是保持与config.bin中的数据结构一致
    uint16 total;      //按键总数
    uint16 keyID[16];  //按键定义数组
} cfg_key_var_t;

/*wav录音掉电回写文件头结构体*/
typedef struct
{
    /*长名*/
    uint8 rec_filename[28];
    /*文件偏移信息*/
    pfile_offset_t ptr_file_offset;
    /*已录文件大小*/
    uint32 file_bytes;
    /*录音比特率*/
    uint32 bitrate;
    /*目录信息,73byte,需要做对齐*/
    pdir_layer_t ptr_layer;
    /*盘符信息*/
    uint8 disk_no;
    /*录音格式，0-wav, 1-mp3*/
    uint8 rec_format;
    /*需要重写标志*/
    bool rewrite_need;
} record_writehead_t;

extern record_writehead_t g_writehead;
extern uint8 charge_frame_id;
extern uint8 backlight_cnt;
extern backlight_type_e backlight_flag;
extern int8 timer_id;
extern bool charge_full_flag;

extern bool write_head(void);
extern void _config_charging(void);
extern void _creat_application(uint8 param);
extern void draw_logo(uint8 param);
extern void switch_off(void);
extern void config_reclaim(int argc);
extern bool _config_format_disk(void);
#endif

