/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-27 20:38     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_bank_broadcast.c
 * \brief    消息通信管理模块实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现一套基本消息通信管理接口，包括异步发送，同步发送，广播消息，及接收消息等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，applib 进程管理接口，等等。
 * \version  1.0
 * \date     2011-9-27
*******************************************************************************/

#include "applib.h"

#ifdef SUPPORT_FILTER_GRAB

static bool __app_running_set_app_info(uint8 app_id, const app_info_t *app_info);

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    以广播方式（也属于异步发送）发送应用私有消息。
 * \param[in]    msg 指向要发送的应用消息结构体
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 广播消息成功。
 * \retval           FALSE 广播消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：广播 MSG_POWER_OFF 关机消息
    msg_apps_t msg;
    
    msg.type = MSG_POWER_OFF;
    broadcast_msg(&msg);
 * \endcode
 * \note 
 * \li  广播应用消息最多只能附带4个字节的消息内容（建议采用双方约定code作为消息内容）。
*******************************************************************************/
bool broadcast_msg(msg_apps_t *msg)
{
    const app_info_t *grab_msg_owner = NULL;
    app_info_t *app;
    uint8 i, j;

    //查找抓取了该消息的应用
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        for(j = 0; j < MAX_GRAB_MSG; j++)
        {
            if((app->used == 1) && (app->msg_grabed[i] == msg->type))
            {
                grab_msg_owner = app;
                break;
            }
        }
    }

    //有应用抓取了该消息，仅发送给该应用即可
    if(grab_msg_owner != NULL)
    {
        send_async_msg(grab_msg_owner->app_id, msg);
    }
    //否则，广播该消息给所有应用
    else
    {
        for(i = 0; i < MAX_APP_COUNT; i++)
        {
            app = &g_app_info_vector[i];
            if(app->used == 1)
            {
                send_async_msg(app->app_id, msg);
            }
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    添加应用抓取消息，以在广播发送消息时独占该消息。
 * \param[in]    msg_type 应用要抓取的应用消息类型
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 添加抓取消息成功。
 * \retval           FALSE 添加抓取消息失败。
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool grab_msg(msg_apps_type_e msg_type)
{
    app_info_t app_info;
    uint8 i;

    //先对已经抓取此消息的应用释放掉该消息
    release_msg(msg_type);

    //寻找合适的位置添加抓取消息
    for(i = 0; i < MAX_GRAB_MSG; i++)
    {
        if(g_this_app_info->msg_grabed[i] == V_U16_INVALID)
        {
            libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
            app_info.msg_grabed[i] = msg_type;
            __app_running_set_app_info(app_info.app_id, &app_info);
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    删除已添加的应用抓取消息。
 * \param[in]    msg_type 应用要删除的抓取应用消息类型
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 删除抓取消息成功。
 * \retval           FALSE 删除抓取消息失败。
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool release_msg(msg_apps_type_e msg_type)
{
    app_info_t *app;
    app_info_t  app_info;
    uint8 i, j;

    //遍历应用信息结构体数组
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        for(j = 0; j < MAX_GRAB_MSG; j++)
        {
            if((app->used == 1) && (app->msg_grabed[j] == msg_type))
            {
                libc_memcpy(&app_info, app, sizeof(app_info_t));
                app_info.msg_grabed[i] = V_U16_INVALID;
                __app_running_set_app_info(app_info.app_id, &app_info);
            }
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    添加应用过滤消息，以广播消息时过滤掉该消息，不让其发送到应用私有消息队列。
 * \param[in]    msg_type 应用要添加的过滤应用消息类型
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 添加过滤消息成功。
 * \retval           FALSE 添加过滤消息失败。
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool filter_msg(msg_apps_type_e msg_type)
{
    app_info_t app_info;
    uint8 i;

    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(g_this_app_info->msg_filtered[i] == V_U16_INVALID)
        {
            libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
            app_info.msg_filtered[i] = msg_type;
            __app_running_set_app_info(app_info.app_id, &app_info);
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    删除过滤了的应用消息。
 * \param[in]    msg_type 应用要删除的过滤应用消息类型
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 删除过滤消息成功。
 * \retval           FALSE 删除过滤消息失败。
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool unfilter_msg(msg_apps_type_e msg_type)
{
    app_info_t app_info;
    uint8 i;

    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(g_this_app_info->msg_filtered[i] == msg_type)
        {
            libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
            app_info.msg_filtered[i] = V_U16_INVALID;
            __app_running_set_app_info(app_info.app_id, &app_info);
            return TRUE;
        }
    }

    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    过滤掉所有广播的应用消息。
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 过滤所有广播的应用消息成功。
 * \retval           FALSE 过滤所有广播失败。
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool filter_all_msg(void)
{
    app_info_t app_info;
    
    libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
    app_info.filter_all_msg  = TRUE;
    __app_running_set_app_info(app_info.app_id, &app_info);

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    解除过滤掉所有广播的应用消息，并清空所有通过filter_msg添加的过滤消息。
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 解除过滤所有广播的应用消息成功。
 * \retval           FALSE 解除过滤所有广播的应用消息失败。
 * \ingroup      applib_msg
 * \note 
*******************************************************************************/
bool unfilter_all_msg(void)
{
    app_info_t app_info;

    libc_memcpy(&app_info, g_this_app_info, sizeof(app_info_t));
    app_info.filter_all_msg  = FALSE;
    libc_memset(app_info.msg_filtered, V_U16_INVALID, sizeof(app_info.msg_filtered));
    __app_running_set_app_info(app_info.app_id, &app_info);
    
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

#else

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    以广播方式（也属于异步发送）发送应用私有消息。
 * \param[in]    msg 指向要发送的应用消息结构体
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 广播消息成功。
 * \retval           FALSE 广播消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：广播 MSG_POWER_OFF 关机消息
    msg_apps_t msg;
    
    msg.type = MSG_POWER_OFF;
    broadcast_msg(&msg);
 * \endcode
 * \note 
 * \li  广播应用消息最多只能附带4个字节的消息内容（建议采用双方约定code作为消息内容）。
*******************************************************************************/
bool broadcast_msg(msg_apps_t *msg)
{
    app_info_t *app;
    uint8 i;

    //广播该消息给所有应用
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if(app->used == 1)
        {
            send_async_msg(app->app_id, msg);
        }
    }

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    以广播方式（也属于异步发送）发送应用私有消息。
 * \param[in]    msg 指向要发送的应用消息结构体
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 广播消息成功。
 * \retval           FALSE 广播消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：广播系统消息，见 get_sys_msg 实现
 * \endcode
 * \note 
 * \li  广播应用消息最多只能附带4个字节的消息内容（建议采用双方约定code作为消息内容）。
 * \li  该接口不会把系统消息广播到 manager ap 私有消息队列中。
*******************************************************************************/
bool broadcast_msg_sys(msg_apps_t *msg)
{
    app_info_t *app;
    uint8 i;
    
    if(msg->type == MSG_RECLAIM)//nandflash reclaim，只发送给 manager AP
    {
        if(g_this_app_info->app_id != APP_ID_UDISK)//U 盘连接下不处理
        {
            send_async_msg(APP_ID_MANAGER, msg);
        }
        return TRUE;
    }

    //广播该消息给所有应用
    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if((app->used == 1) && (app->app_id != APP_ID_MANAGER))
        {
            send_async_msg(app->app_id, msg);
        }
    }

    return TRUE;
}

/*! \endcond */

#endif
