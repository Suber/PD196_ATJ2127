/********************************************************************************
 *                            Module: music_engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-05 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MENGINE_H__
#define __APP_MENGINE_H__

#include "psp_includes.h"
#include "case_include.h"
#include "enhanced.h"

#include "music_common.h"
#include "setting_common.h"

/*!定义音乐引擎的任务栈和主线程优先级*/
#define    AP_MENGINE_STK_POS  AP_BACK_LOW_STK_POS
#define    AP_MENGINE_PRIO     AP_BACK_LOW_PRIO

#define INTRO_INTERVAL 11000

//音乐播放模式
typedef enum
{
    //正常播放
    PLAY_NORMAL,
    //续播
    PLAY_RESUME,
    //继续快进
    PLAY_FFON,
    //快退播放
    PLAY_FBON,
    //不需要播放
    PLAY_NO_PLAY
} play_mode_e;

//音乐停止模式
typedef enum
{
    //正常停止
    STOP_NORMAL,
    //暂停
    STOP_PAUSE
} stop_mode_e;

typedef enum
{
    NORMAL_SWITCH_NEXT = 0x0,
    NORMAL_SWITCH_PREV = 0x1,
    FORCE_SWITCH_NEXT  = 0x2,
    FORCE_SWITCH_PREV  = 0x3,
    END_SWITCH         = 0x4
} switch_mode_e;

typedef enum
{
    SWITCH_NO_ERR,
    SWITCH_ERR_STOP,
    SWITCH_ERR_SWITCH,
    SWITCH_ERR_PLAY
} switch_result_e;

//for audible

typedef struct
{
    // 小机序列号
    unsigned char DeviceId_[20];
    //激活文件,sys文件内容
    unsigned char ActivationRecord_[560];
    //pos文件内容
    unsigned char playbackPosFilebuffer[16];
    //总的章节数目
    unsigned int m_cChapters_;
    // 章节时间
    unsigned int SongChapters_[200];

    //总的图片数目
    unsigned int m_cImages_;
    //图片时间
    unsigned int imageTime_[200];
    //图片位置
    unsigned int imagePos_[200];
    // 图片大小
    unsigned int imageSize_[200];

    //章节文本位置
    unsigned int textPos_[200];

} audible_para_t;


/*!
 * \brief
 *  引擎信息结构体变量定义
 */ 
typedef struct
{
    /******************************************************************
     *          引擎状态信息
     *******************************************************************/
    mengine_status_t eg_status;

    /******************************************************************
     *          播放文件的时间和bit率信息
     *******************************************************************/
    mengine_playinfo_t eg_playinfo;

    /******************************************************************
     *          中间件播放文件信息
     *******************************************************************/
    mmm_mp_file_info_t eg_file_info;

    /******************************************************************

     *          设置信息(保存配置)
     *******************************************************************/
    mengine_config_t eg_config;

} mengine_info_t;

//globle variable
//保存中间件句柄
extern void *mp_handle;

//存放文件系统id
extern int32 file_sys_id;

//进入模式
extern mengine_enter_mode_e mengine_enter_mode;

//引擎信息
extern mengine_info_t mengine_info;

//浏览播放起始时间
extern uint32 g_intro_start;

//存放music文件后缀名的bitmap配置
extern uint32 file_type_cfg;//

//删除当前文件标识
extern uint32 g_del_curfile;

//改变文件路径信息标识
extern uint32 g_change_path_flag;

//标识audbile文件
extern uint16 g_is_audible;

//audible 章节切换定时器
extern int8 g_chapter_timer;

//audible 图片检测定时器
extern int8 g_image_timer;

//pos file buffer
extern uint8 g_audible_pos[16];

//文件名buffer
extern uint8 g_file_name[64];

//audbile info ptr
extern audible_para_t *g_audible_info_p;

//sent to decoder audbile info ptr
extern audible_para_t *g_send_audible_p;

//sd 文件名
//extern uint8 g_sd_file_name[12];

//文件路径来源信息保存,改变路径信息时使用
extern fsel_type_e g_path_type;

//系统全局变量
//extern setting_comval_t g_setting_comval;
//extern comval_t g_comval;

//中间件状态检测定时器
extern int8 g_status_timer;

//中间件状态检测标志位
extern uint8 g_check_status_flag;

//music engine延时参数
extern uint8 g_delay_time;

//audible专辑图片数目
extern uint16 g_image_num;

//控件返回值
extern app_result_e g_mengine_result;

//引擎配置变量结构体指针
extern mengine_config_t *g_eg_cfg_p;

//引擎状态变量结构体指针
extern mengine_status_t *g_eg_status_p;

//引擎播放信息结构体指针
extern mengine_playinfo_t *g_eg_playinfo_p;

//私有消息全局变量
extern private_msg_t g_cur_msg;


//functions

extern app_result_e mengine_control_block(void)
__FAR__;

extern bool change_locat_deal(void)
__FAR__;

extern app_result_e mengine_message_done(private_msg_t* cur_msg)
__FAR__;

extern bool mengine_file_init(void)
__FAR__;

//extern void _end_switch(play_mode_e last_mode)
//__FAR__;

extern bool _set_file(void)
__FAR__;

extern bool _play(play_mode_e play_mode)
__FAR__;

extern bool _stop(stop_mode_e stop_mode)
__FAR__;

//extern bool _next(bool force_mod)
//__FAR__;

//extern bool _prev(bool force_mod)
//__FAR__;

extern eg_err_e _convert_error_no(mmm_mp_err_t err_no)
__FAR__;

extern void _error_timeout_handle(void)
__FAR__;

//for audible

extern void _deal_audible_posfile(uint8 mode)
__FAR__;

extern void _deal_audible_sysfile(void)
__FAR__;

extern void _clr_chapter_timer(void)
__FAR__;

extern void _clr_imag_timer(void)
__FAR__;

extern void _set_chapter_timer(void)
__FAR__;

extern void _set_imag_timer(void)
__FAR__;

extern void _check_audible_chapter(void)
__FAR__;

extern void _check_audible_image(void)
__FAR__;

extern void _adjust_big_little_endian(uint8 *data_address, uint8 data_length);

extern void _check_pos_time(void)
__FAR__;

extern void _chapter_switch_force(bool next_prev)
__FAR__;

extern app_result_e mengine_audible_getimage(void* msg_ptr)
__FAR__;

extern uint32 _volume_deal(uint8 vol_in, bool set_mode)
__FAR__;

extern void mengine_save_errno_no(void)
__FAR__;
//for event
//extern app_result_e mengine_get_config(void* msg_ptr) __FAR__;

extern app_result_e mengine_get_playmode(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_filepath(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_fileinfo(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_status(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_playinfo(void* msg_ptr)
__FAR__;

extern app_result_e mengine_get_bkinfo(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_eq(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_volume(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_section_mode(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_playmode(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_ab_count(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_ab_gap(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_ab_gap(void* msg_ptr)
__FAR__;

extern app_result_e mengine_start_ab(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_apoint(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_bpoint(void* msg_ptr)
__FAR__;

extern app_result_e mengine_clear_ab(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play(void* msg_ptr)
__FAR__;

extern app_result_e mengine_stop(void* msg_ptr)
__FAR__;

extern app_result_e mengine_pause(void* msg_ptr)
__FAR__;

extern app_result_e mengine_resume(void* msg_ptr)
__FAR__;

extern app_result_e mengine_fast_forward(void* msg_ptr)
__FAR__;

extern app_result_e mengine_fast_backward(void* msg_ptr)
__FAR__;

extern app_result_e mengine_fastplay_step(void *msg_ptr);

extern app_result_e mengine_cancel_ffb(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play_next(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play_prev(void* msg_ptr)
__FAR__;

extern app_result_e mengine_play_bookmark(void* msg_ptr)
__FAR__;

extern app_result_e mengine_musui_delete_file(void* msg_ptr)
__FAR__;

extern app_result_e mengine_other_delete_file(void* msg_ptr)
__FAR__;

extern app_result_e mengine_filepath_deal(void* msg_ptr)
__FAR__;

extern app_result_e mengine_set_filepath(void* msg_ptr)
__FAR__;

extern app_result_e mengine_change_filepath(void* msg_ptr)
__FAR__;

extern app_result_e mengine_del_favorite_deal(void* msg_ptr)
__FAR__;

extern app_result_e mengine_add_favorite_deal(void* msg_ptr)
__FAR__;

extern app_result_e mengine_clear_error(void* msg_ptr)
__FAR__;

extern app_result_e mengine_chapter_next(void* msg_ptr)
__FAR__;

extern app_result_e mengine_chapter_prev(void* msg_ptr)
__FAR__;

extern app_result_e mengine_audible_bookmark(void* msg_ptr)
__FAR__;

extern app_result_e mengine_message_done_bank(private_msg_t* cur_msg)
__FAR__;

extern void mengine_set_real_eq(uint8 mode)
__FAR__;

extern app_result_e mengine_get_engine_info(void* msg_ptr)
__FAR__;

extern void mengine_reply_msg(void* msg_ptr, bool ret_vals)
__FAR__;

extern app_result_e mengine_audible_get_section_info(void* msg_ptr);

extern app_result_e mengine_standby_play(void* msg_ptr);

extern switch_result_e mengine_file_switch(stop_mode_e stop_mode, switch_mode_e force_mode, play_mode_e play_mode)
__FAR__;

extern void mengine_save_audible_breakinfo(void) __FAR__;

extern bool _file_switch_info(void);

#endif //__APP_MENGINE_H__
