/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_menu_cfg.h
 * \brief    picture的菜单配置项模块，由菜单配置工具
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#ifndef _PIC_MENU_CFG_H
#define _PIC_MENU_CFG_H
 
#include "psp_includes.h"
#include "case_include.h"	
 
/*!定义AP的编号*/
/*!定义option入口菜单的编号*/
#define    NOPLAY_NOPREVIEW_MENU         0
#define    PLAYING_NOPREVIEW_MENU        1
#define    LASTPLAY_NOPREVIEW_MENU       2
#define    NOPLAY_PREVIEW_MENU           3
#define    PLAYING_PREVIEW_MENU          4
#define    LASTPLAY_PREVIEW_MENU         5 
#define    PIC_CARD_MENU_ENTRY           6

//叶子菜单 提供给菜单配置文件

extern app_result_e menu_func_playing(void *param);

extern app_result_e menu_func_last_play(void *param);

extern app_result_e menu_func_bright(void *param);

extern app_result_e menu_func_backlight_timer(void *param);

extern app_result_e menu_func_slideshow_set(void *param);

extern app_result_e menu_func_delete_pic(void *param);

extern app_result_e menu_func_timer_10s(void *param);

extern app_result_e menu_func_timer_20s(void *param);

extern app_result_e menu_func_timer_30s(void *param);

extern app_result_e menu_func_timer_all_time(void *param);

extern app_result_e menu_func_time_per_slide(void *param);

extern app_result_e menu_func_time_repeat(void *param);

extern app_result_e menu_func_time_repeat_on(void *param);

extern app_result_e menu_func_time_repeat_off(void *param);

extern app_result_e menu_func_time_shuffle(void *param);

extern app_result_e menu_func_time_shuffle_on(void *param);

extern app_result_e menu_func_time_shuffle_off(void *param);

extern app_result_e menu_func_preview(void *param);

extern app_result_e menu_func_preview_on(void *param);

extern app_result_e menu_func_preview_off(void *param);

extern app_result_e menu_func_generate_pic_playlist(void *param);

extern app_result_e menu_func_view_main_disk(void *param);

extern app_result_e menu_func_view_card_disk(void *param);

extern app_result_e menu_func_option_option(void);

#ifdef PC

extern menu_cb_func _get_menu_func(uint16 str_id);

extern menu_cb_leaf _get_callback_func(uint16 str_id);

extern menu_cb_option _get_menu_option(uint16 str_id);

#endif

#endif
