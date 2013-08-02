/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-5-10 9:28     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _sys_timer.c
 * \brief    common 系统计时器初始化和退出接口实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-5-10
*******************************************************************************/

#include "common_ui.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    创建系统计时软定时器和 headbar 周期更新软定时器，由前台应用进入应用时调用。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  除了 config.app，前台应用在进入时必须调用该接口，否则背光，屏保，省电关机，
 *      定时关机，状态栏更新，等等功能将无法使用。
*******************************************************************************/
void sys_timer_init(void)
{
    uint8 tag_backup;
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_SYS);
    sys_counter_timer_id = set_app_timer(APP_TIMER_ATTRB_CONTROL, 500, sys_counter_handle);
    change_app_timer_tag(tag_backup);
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIMER_TAG_SYS);
    sys_status_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 500, sys_status_handle);
    change_app_timer_tag(tag_backup);
    
    stop_app_timer(sys_status_timer_id);//默认不显示 status，通过 gui_set_headbar_mode 开关
    g_sys_counter.battery_counter = 100;//强制读取充电状态和电池电量
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    删除系统计时软定时器和 headbar 周期更新软定时器，由前台应用退出应用时调用。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  退出应用时，需要调用 sys_counter_exit，以删除 sys_counter_init 创建的软定时器。
*******************************************************************************/
void sys_timer_exit(void)
{
    kill_app_timer(sys_counter_timer_id);
    kill_app_timer(sys_status_timer_id);
}

/*! \endcond */
