/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-28 14:42     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _app_manager_bank.c
 * \brief    应用（进程）管理模块实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现应用（进程）管理接口，包括应用注册和初始化、应用注销等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，common_config，等等
 * \version  1.0
 * \date     2011-9-28
*******************************************************************************/

#include "applib.h"

static bool __app_running_set_app_info(uint8 app_id, const app_info_t *app_info);

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    应用在 applib 模块上的注册和初始化。
 * \param[in]    app_id 应用ID，如 APP_ID_MUSIC，APP_ID_MANAGER，APP_ID_MENGINE 等
 * \param[in]    type 应用类型，参见 app_type_e 定义
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 表示注册和初始化成功。
 * \retval           FALSE 表示注册和初始化失败。
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
    例子1：前台应用 music AP 进行 applib 模块注册和初始化
    applib_init(APP_ID_MUSIC, APP_TYPE_GUI);
    
    例子2：后台引擎应用 music engine AP 进行 applib 模块注册和初始化
    applib_init(APP_ID_MENGINE, APP_TYPE_CONSOLE);
    
    例子3：进程管理应用 manager AP 进行 applib 模块注册和初始化
    applib_init(APP_ID_MANAGER, APP_TYPE_PMNG);
 * \endcode
 * \note
 * \li  在应用 main 函数入口处调用，在此之前，不许调用其他 applib 接口。
*******************************************************************************/
bool applib_init(uint8 app_id, app_type_e type)
{
    app_info_t *cur_app_info;

    if(app_id >= APP_ID_MAX)
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
    cur_app_info->app_type = (uint8)type;
    cur_app_info->soft_watchdog_timeout_expires = 0;
    if(type == APP_TYPE_PMNG)
    {
        cur_app_info->mq_id = (uint8)MQ_ID_MNG;
    }
    else if(type == APP_TYPE_GUI)
    {
        cur_app_info->mq_id = (uint8)MQ_ID_DESK;
        g_app_info_state.app_state = APP_STATE_NO_PLAY;//前台AP注册，初始化为 NO_PLAY 状态
    }
    else
    {
        cur_app_info->mq_id = (uint8)MQ_ID_EGN;
        
        if(app_id == APP_ID_MENGINE)
        {
            g_app_info_state.type = ENGINE_MUSIC;
        }
        else if(app_id == APP_ID_FMENGINE)
        {
            g_app_info_state.type = ENGINE_RADIO;
        }
        else//不可能到达这里
        {
            g_app_info_state.type = ENGINE_NULL;
        }
    }
    
    //解锁，允许任务调度
    sys_os_sched_unlock();

    g_this_app_info = cur_app_info;

    //初始化线程互斥信号量
    libc_sem_init(&thread_mutex, 1);

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    应用在 applib 模块上的注销处理。
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 表示注销成功。
 * \retval           FALSE 表示注销失败。
 * \ingroup      app_manager
 * \par          exmaple code
 * \code
    例子1：应用进行 applib 模块注销
    applib_quit();
 * \endcode
 * \note
 * \li  在应用 main 函数退出处调用，在此之后，不许调用其他 applib 接口。
*******************************************************************************/
bool applib_quit(void)
{
    app_info_t cur_app_info;
    
    //销毁线程互斥信号量
    libc_sem_destroy(&thread_mutex);
    
    //记录上一次退出的前台AP，以便返回
    if(g_this_app_info->app_type == APP_TYPE_GUI)
    {
        g_app_last_state.last_app = g_this_app_info->app_id;
    }
    
    //修改后台引擎类型为 NONE
    if(g_this_app_info->app_type == APP_TYPE_CONSOLE)
    {
        g_app_info_state.type = ENGINE_NULL;
        g_app_info_state.state = ENGINE_STATE_NULL;
    }
    
    //修改 app_info 结构体为未被使用
    libc_memcpy(&cur_app_info, g_this_app_info, sizeof(app_info_t));
    cur_app_info.used = 0;
    cur_app_info.app_id = APP_ID_MAX;
    cur_app_info.soft_watchdog_timeout_expires = 0;
    __app_running_set_app_info(g_this_app_info->app_id, &cur_app_info);
    
    return TRUE;
}

/*! \endcond */

/*! \cond */

//根据应用ID设置应用信息
static bool __app_running_set_app_info(uint8 app_id, const app_info_t *app_info)
{
    app_info_t *app;
    uint8 i;
    
    if(app_id >= APP_ID_MAX)
    {
        return FALSE;
    }

    //搜索 app_id 对应 app_info_t 
    app = NULL;
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if((app->used == 1) && (app->app_id == app_id))
        {
            break;
        }
    }
    
    if(NULL == app)
    {
        return FALSE;
    }

    sys_os_sched_lock();
    libc_memcpy(app, app_info, sizeof(app_info_t));
    sys_os_sched_unlock();

    return TRUE;
}

/*! \endcond */
