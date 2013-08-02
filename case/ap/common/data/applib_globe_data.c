/*******************************************************************************
 *                              us212A
 *                            Module: applib-message(send)
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-10 11:26     1.0             build this file 
*******************************************************************************/
/*!
 * \file     applib_data.c
 * \brief    applib 模块全局变量定义
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               applib 模块全局变量定义
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2011-11-10
*******************************************************************************/

#include "applib.h"
#include "case_type.h"

/*!
 *  \brief
 *  g_app_info_vector 全局应用信息结构体数组
 */
app_info_t g_app_info_vector[MAX_APP_COUNT];

/*!
 *  \brief
 *  g_app_info_state 全局系统当前状态结构体
 */
app_info_state_t g_app_info_state;

/*!
 *  \brief
 *  g_app_last_state 全局系统最近状态结构体
 */
app_last_state_t g_app_last_state;

/*!
 *  \brief
 *  g_sys_counter 系统计时器结构体
 */
sys_counter_t g_sys_counter;

/*!
 *  \brief
 *  config_fp 配置脚本文件句柄
 */
sd_handle config_fp;

/*!
 *  \brief
 *  g_bat_sample 电池电量采样值，采样7次取平均值
 */
uint8 g_bat_sample[BAT_SAMPLE_TIME];

/*!
 *  \brief
 *  g_bat_index 电池电量采样序号
 */
uint8 g_bat_index;
