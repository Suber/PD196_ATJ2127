/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-ext in rom
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-12 11:04     1.0             build this file 
*******************************************************************************/
/*!
 * \file     common_func_data.c
 * \brief    common func 全局数据
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2011-10-12
*******************************************************************************/

#include "common_func.h"

/*!
 *  \brief
 *  confmenu_fp 菜单配置文件句柄
 */
sd_handle confmenu_fp;

/*!
 *  \brief
 *  confmenu_offset 菜单配置入口菜单列表起始地址
 */
uint32 confmenu_offset;

/*!
 *  \brief
 *  this_filter_key_value 过滤掉按键长按动作，要过滤的按键
 */
uint8 this_filter_key_value;

/*!
 *  \brief
 *  g_keytone_backup 进入录音时按键音使能与否备份
 */
bool g_keytone_backup;

/*!
 *  \brief
 *  dir_history_valid 文件浏览路径记忆有效与否
 */
bool dir_history_valid;

/*!
 *  \brief
 *  menu_history_valid 菜单列表径记忆有效与否
 */
bool menu_history_valid;

/*!
 *  \brief
 *  dir_history_addr 文件浏览径记忆缓冲区
 */
uint8 *dir_history_addr;

/*!
 *  \brief
 *  menu_history_addr 菜单列表径记忆缓冲区
 */
uint8 *menu_history_addr;
