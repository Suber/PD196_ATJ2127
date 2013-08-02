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

/*!
 *  \brief
 *  thread_mutex 线程互斥信号量指针
 */
os_event_t *thread_mutex;

/*!
 *  \brief
 *  g_this_app_info 当前应用信息结构体指针
 */
const app_info_t *g_this_app_info;

/*!
 *  \brief
 *  g_app_timer_vector 应用软定时器数组指针，指向应用空间的软定时器数组
 */
app_timer_t *g_app_timer_vector;

/*!
 *  \brief
 *  g_app_timer_count 应用软定时器数目，为 0 表示应用不需要使用定时器
 */
uint8 g_app_timer_count = 0;

/*!
 *  \brief
 *  g_this_app_timer_tag 软定时器场景标志，AP场景为0
 */
uint8 g_this_app_timer_tag;

/*!
 *  \brief
 *  g_this_app_msg_dispatch 应用私有消息分发函数
 */
app_msg_dispatch g_this_app_msg_dispatch;

