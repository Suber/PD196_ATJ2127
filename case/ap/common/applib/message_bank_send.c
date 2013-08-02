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
 * \file     _message_bank_send.c
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

/*! \cond COMMON_API */

static app_info_t *__app_running_get_app_info(uint8 app_id);

/******************************************************************************/
/*!                    
 * \par  Description:
 *    以同步方式发送应用私有消息。
 * \param[in]    app_id 同步发送应用私有消息的目标应用ID
 * \param[in]    msg 指向要发送的应用消息结构体
 * \param[in]    reply 指向同步消息应答结构体
 * \param[in]    timeout 超时发送时间，0 表示一直等待不超时返回
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 发送同步消息成功。
 * \retval           FALSE 发送同步消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：阻塞式发送同步消息，直到对方调用 reply_sync_msg 应答
    msg_apps_t msg;
    msg_reply_t reply;
    bool ret;
    
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    reply.content = (void *)file_path;
    ret = send_sync_msg(APP_ID_MANAGER, &msg, &reply, 0);
    if(ret == TRUE)
    {
        if(reply.type == MSG_REPLY_SUCCESS)
        {
            正确返回，获取并处理文件路径
        }
        else
        {
            错误返回
        }
    }
    else
    {
        其他错误
    }
 * \endcode
 * \note 
 * \li  返回错误原因包括：找不到接收应用，发送同步消息给自己，同步消息被过滤掉，消息队列已满，等。
 * \li  注意消息内容缓冲区 msg.content.addr 和 应答缓冲区 reply.content 的空间都是开在消息发送方的。
 *      由于发送同步消息后会等待返回，如果调用 send_sync_msg 的代码是在 control bank段，那么上面的
 *      缓冲区可以开在bank data中。
*******************************************************************************/
bool send_sync_msg(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    private_msg_t cur_send_pmsg;
    const app_info_t *target_app_info;
    msg_reply_t temp_reply;
    uint8 i;
    os_event_t *sem;//同步消息同步信号量指针
    bool retvalue = TRUE;

    //无限时等待信号量，同时只能有1个线程发送同步消息
    libc_sem_wait(thread_mutex, 0);

    //查找目标应用
    target_app_info = __app_running_get_app_info(app_id);
    if(NULL == target_app_info)
    {
        libc_sem_post(thread_mutex);
        return FALSE;
    }
    
    //不能发送同步消息给应用自己
    if(target_app_info == g_this_app_info)
    {
        libc_sem_post(thread_mutex);
        return FALSE;
    }

#ifdef SUPPORT_FILTER_GRAB  
    //检测是否被过滤掉
    if(target_app_info->filter_all_msg == TRUE)
    {
        libc_sem_post(thread_mutex);
        return FALSE;
    }
    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(target_app_info->msg_filtered[i] == msg->type)
        {
            libc_sem_post(thread_mutex);
            return FALSE;
        }
    }
#endif
    
    //填充私有消息结构体
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));
    libc_sem_init(&sem, 0);
    cur_send_pmsg.sem = sem;
    if(reply == NULL)
    {
        cur_send_pmsg.reply = &temp_reply;
    }
    else
    {
        cur_send_pmsg.reply = reply;
    }

    //发送消息到队列中
    if(sys_mq_send((uint32)(target_app_info->mq_id), (void *)&cur_send_pmsg) == -1)
    {
        libc_sem_destroy(&sem);
        libc_sem_post(thread_mutex);
        return FALSE;
    }
    
    //等待同步消息回应
    if(libc_sem_wait(sem, timeout) < 0)
    {
        retvalue = FALSE;
    }

    libc_sem_destroy(&sem);
    libc_sem_post(thread_mutex);
    return retvalue;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    以异步方式发送应用私有消息。
 * \param[in]    app_id 同步发送应用私有消息的目标应用ID
 * \param[in]    msg 指向要发送的应用消息结构体
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 发送异步消息成功。
 * \retval           FALSE 发送异步消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：异步发送消息，无须等待立即返回
    msg_apps_t msg;
    bool ret;
    
    msg.type = MSG_KILL_APP;
    msg.content.data[0] = APP_ID_MUSIC;
    ret = send_async_msg(APP_ID_MANAGER, &msg);
    if(ret == FALSE)
    {
        发生错误
    }
 * \endcode
 * \note 
 * \li  返回错误原因包括：找不到接收应用，异步消息被过滤掉，消息队列已满，等。
 * \li  私有消息异步发送方式，最多只能附带4个字节的消息内容（建议采用双方约定code作为消息内容）。
*******************************************************************************/
bool send_async_msg(uint8 app_id, msg_apps_t *msg)
{
    const app_info_t *target_app_info;
    private_msg_t cur_send_pmsg;
    uint8 i;

    //查找目标应用
    target_app_info = __app_running_get_app_info(app_id);
    if(target_app_info == NULL)
    {
        return FALSE;
    }

#ifdef SUPPORT_FILTER_GRAB
    //检测是否被过滤掉
    if(target_app_info->filter_all_msg == TRUE)
    {
        return FALSE;
    }
    for(i = 0; i < MAX_FILTER_MSG; i++)
    {
        if(target_app_info->msg_filtered[i] == msg->type)
        {
            return FALSE;
        }
    }
#endif
    
    //填充私有消息结构体
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));

    //发送消息到队列中
    if(sys_mq_send((uint32)(target_app_info->mq_id), (void *)&cur_send_pmsg) == -1)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    应答同步消息，仅需要释放信号量即可
 * \param[in]    private_msg 指向接收到的应用私有消息（以同步方式发送）
 * \param[out]   none
 * \return       bool
 * \retval           返回应答同步消息是否成功，true表示成功，false表示失败
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：应答同步消息 MSG_MENGINE_GET_FILEPATH_SYNC
    libc_memcpy((file_path_info_t *)private_msg.reply.content.addr, &tmp_file_path, sizeof(file_path_info_t));
    private_msg.reply.type = MSG_REPLY_SUCCESS;
    reply_sync_msg(&private_msg);
 * \endcode
 * \note 
*******************************************************************************/

bool reply_sync_msg(private_msg_t *private_msg)
{
    libc_sem_post(private_msg->sem);

    return TRUE;
}


/*! \endcond */

/*! \cond */

//根据应用ID获取应用信息
static app_info_t *__app_running_get_app_info(uint8 app_id)
{
    app_info_t *app;
    uint8 i;
    
    if(app_id >= APP_ID_MAX)
    {
        return NULL;
    }

    for(i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if((app->used == 1) && (app->app_id == app_id))
        {
            return app;
        }
    }

    return NULL;
}

/*! \endcond */
