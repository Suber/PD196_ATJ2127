/******************************************************************************
 *                               US212A
 *                            Module: BROWSER
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan      2011-10-25 14:41:35          1.0              build this file
 ******************************************************************************/
/*!
 * \file     browser.h
 * \brief    browser模块头文件
 * \author   wuyufan
 * \version  1.0
 * \date     2011-10-25
 *******************************************************************************/
#ifndef _BROWSER_H_
#define _BROWSER_H_

#include "psp_includes.h"
#include "case_include.h"
#include "browser_res.h"
#include "browser_sty.h"

/*!定义browser应用的任务栈*/
#define    AP_BROWSER_STK_POS  AP_FRONT_LOW_STK_POS

/*!定义browser应用的主线程优先级*/
#define    AP_BROWSER_PRIO     AP_FRONT_LOW_PRIO

/*!应用自己需要的软定时器个数*/
#define APP_TIMER_COUNT		1

#define APP_TIMER_TOTAL		(COMMON_TIMER_COUNT + APP_TIMER_COUNT)

#define DISK_BASE           DISK_C

//#define AUTO_CHANGE_DISK  //是否支持自动切换盘符

/*!
 * \brief
 *  scene_browser_e browser场景枚举类型
 */
typedef enum
{
    /*! 退出场景 */
    SCENE_EXIT = 0,
    /*! 列表场景 */
    SCENE_FILELIST = 1,
    /*! 菜单场景 */
    SCENE_MENU = 2
} scene_browser_e;

/* 全局变量声明 */

/*! \brief browser应用VM变量*/
extern browser_var_t g_browser_var;

/*! \brief 通用VM变量*/
extern comval_t g_comval;

/*! \brief browser下一个场景 */
extern scene_browser_e g_browser_scene_next;

/*! \brief browser 挂载文件系统的mount ID*/
extern int32 browser_mount;

/*! \brief 应用进入模式*/
extern app_param_e g_browser_enter_mode;

/*! \brief 是否是从其他ap返回browser*/
extern bool g_return_browser;

/*! \brief browser文件名 */
extern uint8 browser_file_name[LIST_ITEM_BUFFER_LEN];


/* 函数声明 */

extern app_result_e browser_msg_callback(private_msg_t *msg);

extern app_result_e browser_scene_filelist(void) __FAR__;

extern app_result_e browser_scene_menu(void);

extern app_result_e browser_menu_option(file_path_info_t *path_info, uint16 active);

extern fsel_error_e browser_fsel_initial(uint8 disk_flag) __FAR__;

extern app_result_e browser_disk_initial(void) __FAR__;

extern void music_close_engine(void)__FAR__;

#endif
