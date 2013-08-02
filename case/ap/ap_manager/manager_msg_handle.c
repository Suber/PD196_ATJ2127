/*******************************************************************************
 *                              US212A
 *                            Module: Manager
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     manager_msg_handle.c
 * \brief    消息处理模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "manager.h"

extern void _get_app_name(char *namebuf, uint8 ap_id);
extern void manager_msg_callback_sub(private_msg_t *pri_msg);

/******************************************************************************/
/*!
 * \par  Description:
 *	  消息处理
 * \param[in]    私有消息指针
 * \param[out]   none
 * \return       none
 * \retval       
 * \retval       
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
void manager_msg_callback(private_msg_t *pri_msg)
{
    uint8 ap_name[12];
    int32 waitpid_ret;
    msg_apps_t async_msg;
    uint8 ap_param;
    uint8 ap_ret;

    switch (pri_msg->msg.type)
    {
        //创建前台应用
        case MSG_CREAT_APP:
        //不能创建当前的前台ap
        if (g_app_info_vector[1].app_id != pri_msg->msg.content.data[0])
        {
            waitpid_ret = libc_waitpid(&ap_ret, 0);
            if ((waitpid_ret >= 0) && (waitpid_ret != WAITPID_NO_PROCESS_EXIT))
            {
                _get_app_name(ap_name, pri_msg->msg.content.data[0]);
                ap_param = pri_msg->msg.content.data[1];
                sys_free_ap(FALSE);
                sys_exece_ap(ap_name, 0, (int32) ap_param);
            }
        }
        break;
        
        //创建后台应用
        case MSG_CREAT_APP_SYNC:
        //应用约定如果另一个后台存在，需要先把当前后台杀死，然后再创建需要的后台
        if (g_app_info_vector[2].used == 0)
        {
            _get_app_name(ap_name, pri_msg->msg.content.data[0]);
            ap_param = pri_msg->msg.content.data[1];
            sys_free_ap(TRUE);
            if (sys_exece_ap(ap_name, 1, (int32) ap_param) >= 0)
            {
                pri_msg->reply->type = MSG_REPLY_SUCCESS;
            }
            else
            {
                pri_msg->reply->type = MSG_REPLY_FAILED;
            }
            reply_sync_msg(pri_msg);
        }
        break;

        case MSG_KILL_APP_SYNC:
        //如果后台存在才处理
        if (g_app_info_vector[2].used != 0)
        {
            async_msg.type = MSG_APP_QUIT;
            send_async_msg(pri_msg->msg.content.data[0], &async_msg);
            while (1)
            {
                waitpid_ret = libc_waitpid(&ap_ret, 0);
                if ((waitpid_ret >= 0) && (waitpid_ret != WAITPID_NO_PROCESS_EXIT))
                {
                    pri_msg->reply->type = MSG_REPLY_SUCCESS;
                    reply_sync_msg(pri_msg);
                    break;
                }
            }
        }
        else//后台已经被杀死，直接返回成功，避免前台收不到响应而死等
        {
            pri_msg->reply->type = MSG_REPLY_SUCCESS;
            reply_sync_msg(pri_msg);
        }
        break;
        
        //停止watchdog 软看门狗模块暂时删除
        case MSG_STOP_HARD_WATCHDG_SYNC:
        break;
        
        //重新启动watchdog 软看门狗模块暂时删除
        case MSG_RESTART_HARD_WATCHDG_SYNC:
        break;

        default:
        manager_msg_callback_sub(pri_msg);
        break;
    }

}
