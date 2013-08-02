/******************************************************************************
 *                               USDK
 *                            Module: app
 *                 Copyright(c) 2002-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File: browser_cfg_menu.h
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyufan    2011-8-22 13:29:40      1.0             build this file
 ******************************************************************************/
 /*!
 * \file     browser_menu_cfg.h
 * \brief    browser菜单配置模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               菜单配置头文件
 * \par      EXTERNALIZED FUNCTIONS:
 *               该模块不会被应用调用，工具通过解析该文件生成的目标文件从而生成
 *               菜单数据文件
 * \version 1.0
 * \date    2011-8-22
 *******************************************************************************/
 #ifndef _BROWSER_MENU_CFG_H
 #define _BROWSER_MENU_CFG_H
 
 #include "psp_includes.h"
#include "case_include.h"	
 
 /*! 定义browser入口菜单编号 共5个入口菜单*/
#define BROWSER_MENU_ENTRY_NOPLAY    0	//删除
#define BROWSER_MENU_ENTRY_PLAYING   1	//正在播放/删除
#define BROWSER_MENU_ENTRY_LASTPLAY  2  //上一次播放/删除
#define BROWSER_MENU_ENTRY_CARD      3  //主盘目录/卡目录
#define BROWSER_MENU_ENTRY_DEL_ALL   4  //全部删除 为record/radio应用ui_directory嵌套菜单项

//叶子菜单 提供给菜单配置文件

extern app_result_e menu_func_main_disk(void *param);

extern app_result_e menu_func_card_disk(void *param);

extern app_result_e menu_func_del_all(void *param);

extern app_result_e menu_func_playing(void * param);

extern app_result_e menu_func_last_play(void * param);

extern app_result_e menu_func_del_file(void *param);

extern app_result_e menu_func_option_option(void);

#ifdef PC

extern menu_cb_func _get_menu_func(uint16 str_id);

extern menu_cb_leaf _get_callback_func(uint16 str_id);

extern menu_cb_option _get_menu_option(uint16 str_id);

#endif

#endif
