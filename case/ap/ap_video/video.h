/*******************************************************************************
 *                              这里填写项目名
 *                            Module: 这里填写模块名
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
/*
 * \file     这里请真实填写文件名字
 * \brief    这里填写文件的概述
 * \author   这里填写文件的作者
 * \version 1.0
 * \date  2009/5/4
 *******************************************************************************/
#ifndef    _VIDEO_H
#define    _VIDEO_H

#include  "psp_includes.h"
#include  "case_include.h"
#include  "video_res.h"
#include  "video_sty.h"
#include  "mmm_vp.h"
#include  "video_display.h"
#include "audio_device.h"

/*!定义视频应用的任务栈和主线程优先级*/
#define    AP_VIDEO_STK_POS   AP_FRONT_LOW_STK_POS
#define    AP_VIDEO_PRIO     AP_FRONT_LOW_PRIO
//#define PC
/*!定义AP的编号*/


/*！应用自己需要的软定时器个数*/
#define APP_TIMER_COUNT		2
#define APP_TIMER_TOTAL     (COMMON_TIMER_COUNT + APP_TIMER_COUNT)

#define DISK_BASE           DISK_C

//#define AUTO_CHANGE_DISK  //是否支持自动切换盘符

//视频快进快退速度定义
#define VIDEO_FFB_LEVEL_0   1
#define VIDEO_FFB_LEVEL_1   5
#define VIDEO_FFB_LEVEL_2   9

//视频刷新定义
#define VIDEO_PAINT_NONE     0   //不刷新
#define VIDEO_PAINT_PARTIAL  1   //部分刷新
#define VIDEO_PAINT_ALL      2   //全刷新
//视频循环模式定义
#define VIDEO_REPEAT_NORMAL  FSEL_MODE_NORMAL
#define VIDEO_REPEAT_ONE     FSEL_MODE_LOOPONE
#define VIDEO_REPEAT_ALL     FSEL_MODE_LOOPALL

//视频播放界面标志位定义
#define    up_videoBackGDPic       0x00000001   //显示视频播放背景
#define    up_videoheaderbar       0x00000002   //headerbar
#define    up_videofileinfor       0x00000004   //文件名
#define    up_videoplaystatus      0x00000008   //播放状态
#define    up_videoprogress        0x00000010   //进度条
#define    up_videotimeinfor       0x00000020   //时间（当前时间+总时间）
#define    up_videovolume          0x00000040   //音量条
#define    up_videovoladd          0x00000080   //音量条增加
#define    up_videovolsub          0x00000100   //音量条减少
#define    up_videospeed           0x00000200   //刷新快进快退速度标识
#define    up_videofullscreen      0x00000400   //全屏标识
//---------------------------------------
#define    clr_videoBackGDPic      0xfffffffe
#define    clr_videoheaderbar      0xfffffffd
#define    clr_videofileinfor      0xfffffffb
#define    clr_videoplaystatus     0xfffffff7
#define    clr_videoprogress       0xffffffef
#define    clr_videotimeinfor      0xffffffdf
#define    clr_videovolume         0xffffffbf
#define    clr_videovoladd         0xffffff7f
#define    clr_videovolsub         0xfffffeff
#define    clr_videospeed          0xfffffdff
#define    clr_videofullscreen     0xfffffbff

/**********************设置全界面刷新标志位********************/
#define video_set_full_flag()          \
{                                      \
    g_display_bitmap=g_display_bitmap|   \
                    up_videoBackGDPic |   \
                    up_videoheaderbar |   \
                    up_videofileinfor |   \
                    up_videoplaystatus | \
                    up_videoprogress |   \
                    up_videotimeinfor; \
}
/**********************设置状态条界面刷新标志位*****************/
#define video_set_playbar_flag()             \
{                                        \
    g_display_bitmap=g_display_bitmap|   \
                    up_videoheaderbar |   \
                    up_videofileinfor |   \
                    up_videoplaystatus |   \
                    up_videoprogress| \
                    up_videotimeinfor; \
}
/**********************设置音量条界面刷新标志位*****************/
#define video_set_volumebar_flag()             \
{                                        \
    g_display_bitmap=g_display_bitmap| \
                    up_videovolume; \
}

//视频切换文件标志位
#define up_video_changenext   0x00
#define up_video_changeprev   0x01
#define up_video_changefile   0x02
#define up_video_playnext     (up_video_changefile | up_video_changenext)
#define up_video_playprev     (up_video_changefile | up_video_changeprev)
#define clr_video_changefile 0xfd

#define up_video_fullscreen_timer 0x01
#define up_video_filename_timer   0x02
#define up_video_progress_timer   0x04

#define clr_video_fullscreen_timer 0x08
#define clr_video_filename_timer   0x10
#define clr_video_progress_timer   0x20

/*定义主菜单应用的VM全局变量*/
/*!
 * \brief
 *  video的VM全局变量.
 */
typedef struct
{
    /*! VM变量的魔数值*/
    uint16 magic;
    /*! 上一次进入视频的应用*/
    uint8 prev_enter_mode;
    /*! 保留*/
    uint8 repeat_mode;
    /*! 视频扩展名的bit map*/
    uint32 file_ext_bitmap;
    /*! 视频文件的播放路径信息*/
    file_path_info_t path;
    /*! 视频播放断点信息*/
    videobpinfor_t break_point;
    /*! 视频播放断点位置信息*/
    uint32 bk_cluster_no;
    uint32 bk_dir_entry;
} g_video_var_t;

/*定义主菜单的场景*/
/*!
 *  \brief
 *  scene type
 *  \li SCENE_EXIT : 退出
 *  \li SCENE_FILELIST : video文件列表
 *  \li SCENE_OPTIONMENU : 菜单选项列表场景
 *  \li SCENE_PLAY : 播放场景
 */
typedef enum
{
    SCENE_EXIT = 0, SCENE_FILELIST, SCENE_OPTIONMENU, SCENE_PLAY, SCENE_MENU
} scene_video_e;

/*定义播放场景的ui消息事件*/
/*!
 *  \brief
 */
typedef enum
{
    VIDEO_VOLUMEBAR = 0, VIDEO_PROGRESSBAR, VIDEO_FULLSCREEN
} video_window_e;

typedef enum
{
    DECODE_NO_ERR = 0, DECODE_LOAD_ERR, DECODE_OPEN_ERR, DECODE_FILE_ERR
} decode_status_e;

/*定义视频播放模式*/
/*!
 *  \brief
 *  应用类型定义
 *  \li VIDEO_PLAY :  播放状态
 *  \li VIDEO_PAUSE : 暂停播放状态
 *  \li VIDEO_STOP :  停止播放状态
 *  \li VIDEO_STOP :  快进状态
 *  \li VIDEO_STOP :  快退状态
 */
typedef enum
{
    VIDEO_PLAY = 0, VIDEO_PAUSE, VIDEO_FF, VIDEO_FB, VIDEO_STOP
} video_play_status_e;

typedef struct
{
    uint16 avi_prog_y;
    uint16 amv_prog_y;
    uint16 avi_vol_y;
    uint16 amv_vol_y;
} adjust_region_t;

/*!外部变量定义*/

/*! \brief video应用VM变量*/
extern g_video_var_t g_video_var;

/*! \brief 通用VM变量*/
extern comval_t g_comval;

/*! \brief video 挂载文件系统的mount ID*/
extern int32 video_mount;

/*! \brief 视频文件总时间*/
extern uint32 g_total_time;

/*! \brief 用于刷新进度条的定时器ID，1s定时*/
extern int8 timer_flush_progress; 

/*! \brief 用于滚动显示文件名的定时器ID，80ms定时*/
extern int8 timer_flush_filename; 

/*! \brief 用于全屏态与窗口态切换的定时器ID, 5s定时*/
extern int8 timer_flush_screen; 

/*! \brief 用于控制定时器的开启与关闭 bit0: 全屏定时器 bit1:文件名定时器 bit2:进度条定时器*/
extern uint8 g_video_timer_flag;

/*! \brief 刷新位标志变量*/
extern uint32 g_display_bitmap;

/*! \brief video应用进入模式*/
extern app_param_e g_video_enter_mode;

/*! \brief 视频解码句柄*/
extern void* g_video_handle;

/*! \brief 视频播放状态*/
extern video_play_status_e g_video_play_status;

/*! \brief 视频播放状态备份*/
extern video_play_status_e g_video_play_status_backup;

/*! \brief 视频当前窗口状态*/
extern video_window_e g_video_cur_window;

/*! \brief 视频之前窗口状态*/
extern video_window_e g_video_prev_window;

/*! \brief 刷新标志，标识是否刷新*/
extern uint8 g_video_paint_flag;

/*! \brief 应用获取视频解码信息的变量*/
extern mmm_vp_userparam_t g_video_userinfor;

/*! \brief 应用控制屏窗口变量*/
extern ctlinfor_t g_video_ctlinfor;

/*! \brief 满屏窗参数*/
extern wregion_t g_region_fullscreen;

/*! \brief 时间进度条窗参数*/
extern wregion_t g_region_progressbar;

/*! \brief 音量条窗参数*/
extern wregion_t g_region_volumebar;

/*! \brief 进度条和音量条高度调整参数*/
extern adjust_region_t g_region_adjust;

/*! \brief 自动播放切换文件标志 bit0: 0:切下一曲 1:切上一曲 bit1:是否切换歌曲*/
extern uint8 g_video_change_file;

/*! \brief video刷新模式 */
extern uint8 g_video_draw_mode;

/*! \brief video快进快退速度控制 */
extern uint8 g_video_speed_control;

/*! \brief video时间计数 */
extern uint8 g_video_timer_counter;

/*! \brief 文件名缓冲区 */
extern uint8 g_video_filename[LIST_ITEM_BUFFER_LEN];

/*! \brief 文件名是否滚动显示标志 */
extern bool need_scroll_filename;

/*! \brief video要进入的场景*/
extern scene_video_e g_video_scene_next;

/*! \brief video上一次调度的场景*/
extern scene_video_e g_video_scene_prev;

/*! \brief video文件总数计数*/
extern uint16 g_file_total;

/*! \brief video错误文件计数*/
extern uint16 g_file_err;

/*! \brief video是否删除文件*/
extern uint8 g_del_file;

/*! \brief video快退到头标志*/
extern uint8 g_fast_back_end;

/*!外部函数定义*/

extern app_result_e video_menu(void);
extern app_result_e video_filelist(void);
extern app_result_e video_option_menulist(void);
extern app_result_e video_scene_playing(void);
extern app_result_e video_play(void)__FAR__;

extern app_result_e video_msg_callback(private_msg_t *msg)__FAR__;


extern void video_refresh_screen(uint8 mode)__FAR__;
extern bool video_get_file_information(void);
extern app_result_e video_playing_proc_gui_msg(input_gui_msg_t *ui_msg)__FAR__;

extern void filename_flag_update(void);
extern void play_flag_update(void);
extern void video_full_display(void);

extern void video_decoder_free(void) __FAR__;
extern bool video_change_file(void) __FAR__;
extern void video_clear_breakinfor(void);
extern decode_status_e video_start_decode(bool *need_free_mmm) __FAR__; 

extern void music_close_engine(void)
__FAR__;
extern void close_engine(void);
extern uint8 music_get_disk_type(void);
extern fsel_error_e video_fsel_initial(uint8 disk_flag);
extern app_result_e video_disk_initial(void);
extern app_result_e video_select_disk(void);

#endif

