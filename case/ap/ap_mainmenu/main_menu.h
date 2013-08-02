/*******************************************************************************
 *                              us212a
 *                            Module: Mainmenu
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
/*
 * \file     main_menu.h
 * \brief    mainmneu应用的头文件
 * \author   zhangxs
 * \version 1.0
 * \date  2011/09/04
 *******************************************************************************/
#ifndef _MAIN_MENU_H
#define _MAIN_MENU_H

#include  "mainmenu_res.h"
#include  "mainmenu_sty.h"
#include  "psp_includes.h"
#include  "case_include.h"

/*!定义主菜单应用的任务栈和主线程优先级*/
#define    AP_MAINMENU_STK_POS  0
#define    AP_MAINMENU_PRIO     6
/*!定义option入口菜单的编号*/
#define    NOPLAY_MENU         0
#define    PLAYING_MENU        1
#define    LASTPLAY_MENU       2

/*!定义MAINMENU 魔数值*/
#define MAGIC_MAINMENU 0x55aa
#define    MAX_APP_NUM    14
#define  FRAME_NUM  1
//id3 buffer长度
#define Id3BufSize 52
#define    MAX_INFO_LEN    50
//#define  MAGIC_COMVAL   0x55aa
/*定义主菜单应用的VM全局变量*/
/*!
 * \brief
 *  main_menu的VM全局变量.
 */

typedef struct
{
    uint16 active_item;//活动AP菜单项编号
    uint16 total_item; //AP菜单总数
    uint16 ap_id[MAX_APP_NUM]; //支持应用编号的数组
} cfg_mainmenu_var_t;

typedef struct
{
    uint16 magic; //VM 变量魔数
    cfg_mainmenu_var_t cfg_var;
} g_mainmenu_var_t;

/*定义主菜单的场景*/
/*!
 *  \brief
 *  scene type
 *  \li SCENE_EXIT : 退出
 *  \li SCENE_MAINMENU : 主菜单应用列表场景
 *  \li SCENE_OPTIONMENU : 菜单选项列表场景
 */
typedef enum
{
    /*!退出*/
    SCENE_EXIT = 0,
    /*!主菜单应用列表场景*/
    SCENE_MAINMENU,
    /*!菜单选项列表场景*/
    SCENE_OPTIONMENU
} scene_mainmenu_e;

/*定义应用类型*/
/*!
 *  \brief
 *  应用类型定义
 *  \li MUSIC_TYPE : 音乐类
 *  \li VIDEO_TYPE : 视频类
 *  \li PIC_TYPE : 图片类
 *  \li TXT_TYPE : 文本类
 *  \li FM_TYPE : 收音机类
 */
typedef enum
{
    /*!音乐类*/
    MUSIC_TYPE = 0,
    /*!视频类*/
    VIDEO_TYPE,
    /*!图片类*/
    PIC_TYPE,
    /*!文本类*/
    TXT_TYPE,
    /*!收音机类*/
    FM_TYPE
} ap_type_e;

typedef enum
{
    /*!正常消息*/
    NORMAL = 0,
    /*!下一项消息*/
    NEXTITEM,
    /*!上一项消息*/
    PREVITEM,
    /*!更新底部*/
    BOTTOM
} mainmenu_ui_result_e;

typedef enum
{
    FILE_NO_CHANGE = 0, FILE_CHANGE, FORMAT_ERR
} file_status_e;

/*! 外部变量定义*/
extern g_mainmenu_var_t g_mainmenu_var;
extern setting_comval_t g_setting_comval;
extern uint16 g_active_index;
extern uint16 g_item_amount;
extern uint16 ap_pic_box[];
extern uint16 ap_pic_box_bg[];

//extern uint16 ap_pic_bottom_box[];
extern uint16 ap_text_box[][MAX_APP_NUM + 1];
extern uint16 ap_info_box[][MAX_APP_NUM + 1];
extern uint16 ap_string_box[];
extern uint16 ap_string_info_box[];
extern uint16 ap_file_amount[];
extern uint8 ap_string_infor[];
extern uint8 g_title_buf[];
extern int32 g_music_mount_id;
extern file_path_info_t g_file_path_info;
extern id3_info_t g_id3_info;
extern mengine_status_t g_music_status;
extern bool need_check_status;
extern bool need_fsel_init;
extern uint8 g_frame_num;
extern mainmenu_ui_result_e g_direction;
extern int8 scroll_timer;
extern int8 status_timer;
extern uint32 g_fm_feq;
extern uint16 g_ap_file_num[];
extern uint8 headbar_update;
extern uint8 g_check_mengine_status;
/*!外部函数定义*/
extern app_result_e mainmenu_desktop(void) __FAR__;
extern app_result_e option_menulist(void) __FAR__;
extern app_result_e mainmenu_gui_msg_handle(msg_apps_type_e gui_event) __FAR__;
/*!菜单选项函数定义*/
extern app_result_e menu_func_playing(void *param) __FAR__;
extern app_result_e menu_func_last_play(void *param) __FAR__;
extern app_result_e menu_func_theme(void *param) __FAR__;
extern app_result_e menu_func_theme_blue(void *param) __FAR__;
extern app_result_e menu_func_theme_green(void *param) __FAR__;
extern app_result_e menu_func_theme_gray(void *param) __FAR__;
extern app_result_e menu_func_theme_pink(void *param) __FAR__;
extern app_result_e menu_func_theme_red(void *param) __FAR__;
extern app_result_e menu_func_return(void) __FAR__;
extern app_result_e mainmenu_app_msg_callback(private_msg_t *pri_msg) __FAR__;
extern void _get_music_info(void) __FAR__;
extern void _init_ap_display_infor(void) __FAR__;
extern void _paint_desktop(mainmenu_ui_result_e ui_result) __FAR__;
extern bool music_get_filepath(file_path_info_t *filepath) __FAR__;
extern bool music_close_engine(void) __FAR__;
extern void item_scroll_handle(void) __FAR__;
#endif

