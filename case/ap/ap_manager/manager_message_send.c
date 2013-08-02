/*******************************************************************************
 *                              us212A
 *                            Module: Manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-27 20:38     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_message_send.c
 * \brief    applib 消息通信机制 发送类（bank） 接口实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现一套基本消息通信机制，主要包括消息发送机制。
 * \par      EXTERNALIZED FUNCTIONS:
 *               调用 libc 和 psp 的接口，以及applib 进程管理模块接口实现消息机制。
 * \version 1.0
 * \date  2011-9-27
 *******************************************************************************/

#include "applib.h"
/******************************************************************************/
/*!
 * \par  Description:
 *    以异步方式发送应用私有消息
 * \param[in]    app_id：同步发送应用私有消息的目标应用ID
 * \param[in]    msg：指向要发送的应用消息结构体
 * \param[out]   none
 * \return       bool
 * \retval           返回发送异步消息是否成功，true表示成功，false表示失败
 * \par          exmaple code
 * \code
 *               msg.type = MSG_KILL_APP;
 *               msg.content.data[0] = APP_ID_MUSIC;
 *               result = send_async_msg(APP_ID_MANAGER, &msg);
 *               if(result == TRUE)
 *               {}
 * \endcode
 * \note
 *******************************************************************************/
bool send_async_msg(uint8 app_id, msg_apps_t *msg)
{
    private_msg_t cur_send_pmsg;
    mq_id_e mq_id;

    cur_send_pmsg.sem = NULL;
    cur_send_pmsg.reply = NULL;

    //查找目标应用
    if ((app_id == APP_ID_MENGINE) || (app_id == APP_ID_FMENGINE))
    {
        mq_id = MQ_ID_EGN;
    }
    else
    {
        mq_id = MQ_ID_DESK;
    }

    //填充私有消息结构体
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));

    //发送消息到队列中
    if ((int) sys_mq_send((uint32) (mq_id), (void *) &cur_send_pmsg) == -1)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    以广播方式（也属于异步发送）发送应用私有消息
 * \param[in]    msg 指向要发送的应用消息结构体
 * \param[out]   none
 * \return       bool
 * \retval           返回广播消息是否成功，true表示成功，false表示失败
 * \par          exmaple code
 * \code
 *               msg.type = MSG_APP_QUIT;
 *               result = broadcast_msg(&msg);
 *               if(result == TRUE)
 *               {}
 * \endcode
 * \note
 *******************************************************************************/
bool broadcast_msg(msg_apps_t *msg)
{
    app_info_t *app;
    bool ret = FALSE;
    uint8 i;

    //广播该消息给所有前台应用，事实上只有1个
    for (i = 0; i < MAX_APP_COUNT; i++)
    {
        app = &g_app_info_vector[i];
        if ((app->used == 1) && (app->app_type == APP_TYPE_GUI))
        {
            ret = send_async_msg(app->app_id, msg);
            if(ret == FALSE)
            {
                return FALSE;
            }
        }
    }

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    应答同步消息，仅需要释放信号量即可
 * \param[in]    private_msg：指向接收到的应用私有消息（以同步方式发送）
 * \param[out]   none
 * \return       bool
 * \retval           返回应答同步消息是否成功，true表示成功，false表示失败
 * \note
 *******************************************************************************/
bool reply_sync_msg(private_msg_t *private_msg)
{
    libc_sem_post(private_msg->sem);

    return TRUE;
}

