/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-9 17:29     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_bank_init.c
 * \brief    消息通信管理模块实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现一套基本消息通信管理接口，包括异步发送，同步发送，广播消息，及接收消息等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，applib 进程管理接口，等等。
 * \version  1.0
 * \date     2011-11-9
*******************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    初始化 applib 消息通信管理模块。
 * \param[in]    msg_dispatch 应用消息分发函数，仅前台应用需向 common 模块注册该函数
 * \param[out]   none
 * \return       bool
 * \retval           TRUE 初始化消息通信管理模块成功。
 * \retval           FALSE 初始化消息通信管理模块失败
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：前台AP调用
    extern app_result_e app_msg_dispatch(private_msg_t *msg);
    
    applib_message_init(app_msg_dispatch);
    
    例子2：进程管理 及 后台AP调用
    applib_message_init(NULL);
 * \endcode
 * \note 
*******************************************************************************/
bool applib_message_init(app_msg_dispatch msg_dispatch)
{
    if(g_this_app_info->app_type == APP_TYPE_GUI)
    {
        //清空 gui 消息队列
        sys_mq_flush(MQ_ID_GUI);
        
        //注册前台应用消息分发函数，以供 common 模块调用
        g_this_app_msg_dispatch = msg_dispatch;
    }
    
    //清空 应用消息队列
    sys_mq_flush(g_this_app_info->mq_id);
    
    return TRUE;
}

/*! \endcond */
