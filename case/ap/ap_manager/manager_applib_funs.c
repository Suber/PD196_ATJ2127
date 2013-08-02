/*******************************************************************************
 *                              us212A
 *                            Module: manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-28 14:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_applib_funs.c
 * \brief    applib 进程管理模块（bank部分）实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现一套基本 进程管理 接口。
 * \par      EXTERNALIZED FUNCTIONS:
 *               调用 libc 和 psp 的接口实现。
 * \version 1.0
 * \date  2011-9-28
 *******************************************************************************/

#include "applib.h"
extern bool open_config_file(void);
/******************************************************************************/
/*!
 * \par  Description:
 *    应用在 applib 模块上的初始化处理，在应用的 main 函数入口处调用
 * \param[in]    app_id：应用ID
 * \param[in]    type：应用类型，参见app_type_e 定义
 * \param[out]   none
 * \return       bool
 * \retval           返回是否初始化成功，true表示成功，false表示失败
 * \note
 *******************************************************************************/
bool applib_init(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    if (app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    //获取 app_info 结构体指针
    cur_app_info = &g_app_info_vector[type];

    //上锁，禁止任务调度
    sys_os_sched_lock();

    //初始化应用信息
    libc_memset(cur_app_info, 0x0, sizeof(app_info_t));
    cur_app_info->used = 1;//标志已被分配使用
    cur_app_info->app_id = app_id;
    cur_app_info->app_type = (uint8) type;
    if (type == APP_TYPE_PMNG)
    {
        cur_app_info->mq_id = (uint8) MQ_ID_MNG;
    }

    //解锁，允许任务调度
    sys_os_sched_unlock();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    全局数据初始化，在进程管理器开头调用（在调用 applib_init 前）
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           返回是否初始化成功，true表示成功，false表示失败
 * \note
 *******************************************************************************/
bool globe_data_init(void)
{
    uint8 i;

    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        libc_memset(&(g_app_info_vector[i]), 0x00, sizeof(app_info_t));
        g_app_info_vector[i].app_id = APP_ID_MAX;
    }

    g_app_info_state.type = ENGINE_NULL;
    g_app_info_state.state = ENGINE_STATE_NULL;
    g_app_info_state.app_state = APP_STATE_NO_PLAY;
    g_app_info_state.filter_key = KEY_NULL;
    g_app_info_state.keylock_state = FALSE;
    g_app_info_state.sound_state = SOUND_OUT_STOP;
    g_app_info_state.volume = 0;
    g_app_info_state.backlight_state = BACKLIGHT_STATE_NORMAL;
    g_app_info_state.screensave_state = FALSE;
    g_app_info_state.card_state = CARD_STATE_CARD_NULL;
#ifndef PC
    g_app_info_state.cable_state = CABLE_STATE_CABLE_NULL;
#else
    g_app_info_state.cable_state = CABLE_STATE_CABLE_IN;
#endif
    g_app_info_state.antenna_state = FALSE;
    g_app_info_state.charge_state = CHARGE_NONE;
    g_app_info_state.bat_value = 5;
    g_app_info_state.card_plist = FALSE;
    g_app_info_state.video_state = 0;
    g_app_info_state.card_update = FALSE;

    g_app_last_state.last_app = APP_ID_MAX;
    g_app_last_state.last_type = ENGINE_NULL;
    g_app_last_state.last_state = ENGINE_STATE_NULL;
    g_app_last_state.last_app_state = APP_STATE_NO_PLAY;
    g_app_last_state.last_card_state = CARD_STATE_CARD_NULL;
#ifndef PC
    g_app_last_state.last_cable_state = CABLE_STATE_CABLE_NULL;
#else
    g_app_last_state.last_cable_state = CABLE_STATE_CABLE_IN;
#endif
    g_app_last_state.last_light_timer = 0;
    g_app_last_state.last_screen_save_mode = 0;
    g_app_last_state.last_sleep_timer = 0;

    libc_memset(&g_sys_counter, 0x00, sizeof(sys_counter_t));
    g_bat_index = 0;

    return open_config_file();
}
