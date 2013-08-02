/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-21 16:06     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_rcode_for_engine.c
 * \brief    消息通信管理模块实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现一套基本消息通信管理接口，包括异步发送，同步发送，广播消息，及接收消息等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，applib 进程管理接口，等等。
 * \version  1.0
 * \date     2011-11-21
*******************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    接收应用私有消息（有些来自于接收系统消息时转发的），专给后台引擎使用的精简版本。
 * \param[in]    none
 * \param[out]   private_msg：返回接收到的应用私有消息
 * \return       bool
 * \retval           TRUE 接收应用私有消息成功。
 * \retval           FALSE 接收应用私有消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：后台引擎应用私有消息处理
    private_msg_t private_msg;
    app_result_e result;
    bool ret;
    
    ret = get_app_msg(&private_msg);
    if(ret == TRUE)
    {
        result = msg_callback(&private_msg);
        if(result != RESULT_NULL)
        {
            非空返回值处理
        }
    }
 * \endcode
 * \note 
*******************************************************************************/
bool get_app_msg_for_engine(private_msg_t *private_msg)
{
    //软定时器扫描
    if(g_app_timer_count > 0)
    {
        handle_timers();
    }

    //接收 应用私有消息
    if(sys_mq_receive((uint32)(g_this_app_info->mq_id), (void *)private_msg) == 0)
    {
        return TRUE;
    }
    
    //消息队列已空，返回false
    return FALSE;
}
#if 0
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
bool reply_sync_msg(private_msg_t *private_msg)//强链接，LD链接时使用此符号语义
{
    libc_sem_post(private_msg->sem);

    return TRUE;
}
#endif
/*! \endcond */
