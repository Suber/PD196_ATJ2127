/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MUSIC_H__
#define __APP_MUSIC_H__

#include "psp_includes.h"
#include "case_include.h"	//display.h包含在common_ui.h中
#include "music_sty.h"
#include "music_res.h"
#include "music_common.h"

#include "setting_common.h"
#include "enhanced.h"

/*!定义音乐应用的任务栈和主线程优先级*/
#define    AP_MUSIC_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_MUSIC_PRIO     AP_FRONT_LOW_PRIO

//定义music批量添加收藏夹时的定时器场景ID
#define    APP_TIMER_TAG_FAVOR          0x01
#define    APP_TIMER_TAG_BOOKMARK       0x02

//定义music列表子场景所包含的控件状态机
#define    MUSIC_LISTSCENE_LIST         0x0
#define    MUSIC_LISTSCENE_MENU         0x1
#define    MUSIC_LISTSCENE_ALBUMLIST    0x2
#define    MUSIC_LISTSCENE_EXIT         0x3

//定义浏览模式
#define    MUSIC_BROWSER_ROOT_LAYER     0x0
#define    MUSIC_BROWSER_DEST_LAYER     0x1

//#define PRINT_BANK_INFO

//id3文件名 buffer长度
#define Id3BufSIZE 92

//id3专辑/歌手buffer长度
#define ArtBufSIZE 52

//最大后缀名个数
#define MusicExtNum 11

/*!
 * \brief
 *  从其他ap进入msuic的模式
 */
typedef enum
{
    /*! 从主界面选择music 图标 */
    ENTER_FROM_DESKTOP,
    /*! 从browser选择了music文件 */
    ENTER_FROM_BROWSER,
    /*! 从主界面选择了正在播放图标 */
    ENTER_FROM_PLAYNOW,
    /*! 从其他ap的option菜单选择了正在播放 */
    ENTER_FROM_UI_MENU
} music_enter_mode_e;

/*!
 * \brief
 *  进入msuic播放场景模式
 */
typedef enum
{
    /*! 设置文件开始播放 */
    PLAY_START,
    /*! audible断点播放 */
    PLAY_AUDIBLE_RESUME,
    /*! 保持播放/暂停状态 */
    PLAY_KEEP
} music_play_set_e;

/*!
 * \brief
 *  music_ui场景定义
 */
typedef enum
{
    /*! 播放场景 */
    SCENE_PLAYING,
    /*! 菜单/列表场景 */
    SCENE_LSTMENU,
    /*! 设置场景 */
    SCENE_SETMENU,
    /*! 退出场景 */
    SCENE_EXIT
} music_scene_e;

/*!
 * \brief
 *  控制ID3滚屏变量定义
 */
typedef struct
{
    /*! 滚屏字符串索引 */
    uint8 index;
    /*! 是否为初始化滚屏 */
    uint8 init;
    /*! 是否刷新显示，仅在滚屏初始化时有效 */
    uint8 update;
    /*! 滚屏方向 */
    uint8 direct;
} scroll_vars_t;



/*!
 * \brief
 *  music ui配置参数
 */
typedef struct
{
    /*! 魔数 */
    uint16 magic;
    /*! id3获取功能 */
    bool id3_support;
    /*! 歌词显示功能 */
    bool lrc_support;
    /*! album art显示功能 */
    bool album_art_support;
    /*! 是否存在audible 断点 */
    uint8 audible_bk_flag;
    /*! 播放速度*/
    uint16 playspeed;
    /*! ab复读次数*/
    uint16 ab_count;
    /*! ab复读间隔*/
    uint16 ab_gap;
    /*! audible的章节模式 */
    audible_switch_mode_e section_mode;
    /*! 从browser选择歌曲 */
    uint8 browser_sel;
    /*! 进入场景 */
    music_scene_e enter_scene;
    /*! 启动引擎设置 */
    music_play_set_e play_set;
    /*! 从专辑图片列表进入 */
    uint8 albumlist_sel;

    uint8 reserved[3];
} music_config_t;

/*!
 * \brief
 *  记录当前文件选择器的初始化类型
 */
typedef struct
{
    uint8 disk_type;
    uint8 list_type;
} fsel_module_t;


//for globle variables

//globle variable
//存放当前场景变量
extern music_scene_e g_music_scene;

//进入music的方式
extern app_param_e g_enter_mode;

//进入play的操作
extern music_play_set_e g_play_set;

//显示标识
extern bool g_paint_flag;

//album art 解码标识
extern uint8 g_album_art;

//获取文件信息标识 bit0:获取文件ID3信息 bit1:获取文件路径信息
extern uint8 g_getfile_info;

//歌词存在标识
extern bool g_lyric_flag;

//播放控制定时器id
extern int8 time_frush_id;

//记录滚屏定时器状态,参见id3_deal_status_e定义
extern uint8 g_scroll_status;

//ID3滚屏定时器
extern int8 scroll_frush_id;

//标记是否临时shuffle
extern uint8 g_shuffle_tmporary;

//标记当前是否是audible
extern uint8 g_audible_now;

//标志ab复读替换
extern bool g_ab_switch;

//当前播放的文件总数
//extern uint16 g_file_total;

//当前播放的文件序号
//extern uint16 g_file_num;

extern musfile_cnt_t g_file;

//连续错误歌曲数
extern uint16 g_error_num;

//向前切
extern uint16 g_switch_prev;

//存放id3信息
extern id3_info_t g_id3_info;

//标题缓冲区
extern uint8 g_title_buf[Id3BufSIZE];

//歌手缓冲区
extern uint8 g_artist_buf[ArtBufSIZE];

//专辑缓冲区
extern uint8 g_album_buf[ArtBufSIZE];

//滚屏内容
extern scroll_content_t g_scroll_content[3];

//滚屏控制参数变量
extern scroll_vars_t g_scroll_vars;

//当前播放歌曲的文件名 歌词与album art使用
extern uint8 g_song_filename[64];

//专辑图片显示信息
extern album_picinfo_t album_art_info;

//刷新标志位变量
extern uint32 g_display_bitmap;

//文件路径来源信息保存，播放时候用
extern file_path_info_t g_file_path_info;

//浏览的路径信息，浏览时用
extern file_path_info_t g_browser_filepath;

//存放文件系统id
extern int32 g_music_mount_id;

//当前播放状态信息
extern mengine_status_t g_music_status;

//当前播放时间和比特率信息
extern mengine_playinfo_t g_play_info;

//audible的断点信息
extern mmm_mp_bp_info_t g_audible_bkinfo;

//music ui的配置项信息
extern music_config_t g_music_config;

//系统全局变量
extern setting_comval_t g_setting_comval;

//查询引擎状态标志位
extern uint8 g_check_mengine_status;

//定时器计数
extern uint8 g_timer_counter;

//当前歌曲总时间
extern uint32 g_total_time;

//当前播放时间
extern uint32 g_cur_time;

//前一次记录播放时间
extern uint32 g_prev_time;

//对于nandflash，表示在VM区地址，对于其它介质，为句柄地址
extern uint32 g_bookmark_fp;

//快进退速率计数
extern uint8 g_speed_counter;

//歌词显示多屏显示标志位
extern bool g_lyric_multi_screen;

//恢复默认设置eq标志
extern uint8 g_eq_restore;

//是否需要开PA标志
extern bool g_need_open_pa;

//滚屏状态机是否调度标志
extern bool g_scroll_deal;

//临时缓冲区地址指针，位于lcd的1k buffer
extern uint8 *g_temp_buffer_p;

//文件选择器模块类型
extern fsel_module_t g_fsel_module;

#ifdef PRINT_BANK_INFO
extern int print_label;
#endif

//for function scene deal

extern app_result_e music_scene_playing(app_param_e enter_mode, file_path_info_t* path_locat,
music_play_set_e play_set);

extern app_result_e music_scene_listmenu(bool menu_list, file_path_info_t* browser_path);

extern app_result_e music_scene_setmenu(bool music_audible);

//for function favorite

extern uint8 deal_file_name(uint8 *source_str, uint8 *dest_str);

extern void get_music_id3_title(uint8 *buffer, uint8 buffer_len, uint8 *ext_name);

extern app_result_e music_message_deal(private_msg_t *msg)
__FAR__;

extern bool music_file_init(file_path_info_t* init_path_locat);

extern bool music_open_engine(mengine_enter_mode_e start_mode);

extern bool music_close_engine(void);

//for function rcode
extern bool music_get_filepath(file_path_info_t *filepath)
__FAR__;

extern bool music_get_fileinfo(mmm_mp_file_info_t* fileinfo)
__FAR__;

extern bool music_get_status(mengine_status_t* play_status)
__FAR__;

extern bool music_get_playinfo(mengine_playinfo_t* play_info)
__FAR__;

extern bool music_get_bkinfo(mmm_mp_bp_info_t* curbk_info)
__FAR__;

extern bool music_get_playmode(uint8* playmode)
__FAR__;

extern bool music_play(void)
__FAR__;

extern bool music_send_msg(void *input_param, void *output_param, uint32 msg_type)
__FAR__;

extern bool music_set_eq(mmm_mp_eq_info_t *eq_val);

extern bool music_set_volume(uint8 volume);

//extern bool music_set_section_mode(uint8 section_mod) __FAR__;

extern bool music_set_playmode(uint8 playmode);

extern bool music_set_ab_count(uint16 count);

extern bool music_set_ab_gap(uint16 gaptime);

extern bool music_start_ab(void);

extern bool music_set_filepath(file_path_info_t *filepath)
__FAR__;

extern bool music_play_next(void)
__FAR__;

extern bool music_play_prev(void)
__FAR__;

extern bool music_play_bookmark(mmm_mp_bp_info_t *bookmark)
__FAR__;

extern bool music_delete_file(void)
__FAR__;

extern bool music_change_filepath(file_path_info_t *filepath)
__FAR__;

extern bool music_clear_error(void)
__FAR__;

extern app_result_e _scene_play_sysmsg(void)
__FAR__;

//extern app_result_e _scene_deal_guimsg(input_gui_msg_t *gui_msg);

extern void play_flag_update(void)
__FAR__;

extern bool check_bookmark_exist(void);

extern void music_load_sysfile(void);

extern app_result_e music_deal_sd_out(void)__FAR__;

extern void _scene_result_deal(app_result_e result_val);

extern bool music_get_section_info(musfile_cnt_t *file_info);

extern void _scene_play_seteq(uint8 eq_type);

extern bool music_standby_play(void);

#endif //__APP_MUSIC_H__

