/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     video_menu_cfg.h
 * \brief    video的菜单配置项模块，由菜单配置工具
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#ifndef _VIDEO_MENU_CFG_H
#define _VIDEO_MENU_CFG_H

#include "psp_includes.h"
#include "case_include.h"	

/*!定义option入口菜单的编号*/
#define    VIDEO_NOPLAY_MENU_ENTRY         0
#define    VIDEO_PLAYING_MENU_ENTRY        1
#define    VIDEO_LASTPLAY_MENU_ENTRY       2
#define    VIDEO_CARD_MENU_ENTRY           3

//叶子菜单 提供给菜单配置文件

extern app_result_e menu_func_playing(void *param);

extern app_result_e menu_func_last_play(void *param);

extern app_result_e menu_func_bright(void *param);

extern app_result_e menu_func_repeat_mode(void *param);

extern app_result_e menu_func_repeat_normal(void *param);

extern app_result_e menu_func_repeat_one(void *param);

extern app_result_e menu_func_repeat_all(void *param);

extern app_result_e menu_func_delete_video(void *param);

extern app_result_e menu_func_generate_video_playlist(void *param);

extern app_result_e menu_func_view_main_disk(void *param);

extern app_result_e menu_func_view_card_disk(void *param);

extern app_result_e menu_func_option_option(void);

#ifdef PC

extern menu_cb_func _get_menu_func(uint16 str_id);

extern menu_cb_leaf _get_callback_func(uint16 str_id);

extern menu_cb_option _get_menu_option(uint16 str_id);

#endif
 
#endif
