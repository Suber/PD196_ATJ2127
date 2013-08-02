/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-27 22:18     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_rcode.c
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

extern void com_app_msg_hook(private_msg_t *private_msg) __FAR__;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    接收 gui 消息，即输入型消息。
 * \param[in]    none
 * \param[out]   input_msg：返回接收到的 gui 消息
 * \return       bool
 * \retval           TRUE 接收 gui 消息成功。
 * \retval           FALSE 接收 gui 消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：gui 消息（事件）循环
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    bool ret;
    
    ret = get_gui_msg(&input_msg);
    if(ret == TRUE)
    {
        if(com_key_mapping(&input_msg, &gui_event, key_map_list) == TRUE)
        {
            switch(gui_event)
            {
            case EVENT_DO_SOMETHING:
                break;
               
            default:
                result = com_message_box(gui_event);
                break;
            }
        }
    }
    else
    {
        gui 消息（事件）处理完毕，接下来可以处理应用私有消息了
    }
 * \endcode
 * \note 
*******************************************************************************/
bool get_gui_msg(input_gui_msg_t *input_msg)
{
    uint32 key_data;
    
    if(sys_mq_receive(MQ_ID_GUI, (void *)&key_data) == 0)
    {
        //消息解压缩
        input_msg->type = (input_msg_type_e)(uint8)key_data;//byte 0
        //if(input_msg->type == INPUT_MSG_KEY)
        //{
        input_msg->data.kmsg.val = (uint8)(key_data >> 8);//byte 1
        input_msg->data.kmsg.type = (key_type_e)(key_data >> 16);//byte 2-3
        //}
        //else if(input_msg->type == INPUT_MSG_TOUCH)
        //{
        //    input_msg->data.tmsg.x = (uint16)(key_data >> 8);//bit[8-17]
        //    input_msg->data.tmsg.y = (uint16)(key_data >> 18);//bit[18-27]
        //    input_msg->data.tmsg.type = (tp_type_e)(key_data >> 28);//bit[28-31]
        //}
        //else//INPUT_MSG_GSENSOR
        //{
        //    
        //}
        
        //收到gui消息，表示有用户操作，进行一些系统处理，比如：
        //0.长按按键后过滤掉后续按键
        //1.背光关闭计时，屏幕保护计时，省电关机计时，返回正在播放计时 清0
        //2.按键音处理
        //3.按键锁处理
        return com_gui_msg_hook(input_msg);
    }
    
    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    接收系统消息，并广播给所有应用。
 * \param[in]    none
 * \param[out]   sys_msg：返回接收到的系统消息
 * \return       bool
 * \retval           TRUE 接收系统消息成功。
 * \retval           FALSE 接收系统消息失败。
 * \ingroup      applib_msg
 * \note 
 * \li  获取系统消息后，会广播给所有应用。
 * \li  该接口只在前台AP的 get_app_msg 中调用，也就是说，系统消息从发出到被处理，
 *      可能会被 delay 一些时间，直到有前台AP进入消息循环。
*******************************************************************************/
bool get_sys_msg(sys_msg_t *sys_msg)
{
    msg_apps_t msg;
    
    //须把type清0，因为系统消息长度为2个字节，sys_mq_receive 并不会覆盖到type（4字节）高2字节，会导致type错误
    sys_msg->type = MSG_NULL;
    if(sys_mq_receive(MQ_ID_SYS, (void *)sys_msg) == 0)
    {
        msg.type = sys_msg->type;
        return broadcast_msg_sys(&msg);
    }
    
    //消息队列已空，返回false
    return FALSE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    接收应用私有消息（有些来自于接收系统消息时转发的）。
 * \param[in]    none
 * \param[out]   private_msg：返回接收到的应用私有消息
 * \return       bool
 * \retval           TRUE 接收应用私有消息成功。
 * \retval           FALSE 接收应用私有消息失败。
 * \ingroup      applib_msg
 * \par          exmaple code
 * \code
    例子1：应用私有消息处理
    private_msg_t private_msg;
    app_result_e result;
    bool ret;
    
    ret = get_app_msg(&private_msg);
    if(ret == TRUE)
    {
        result = msg_callback(&private_msg);
        if(result == RESULT_REDRAW)
        {
            返回重绘
        }
        else
        {
            其他返回值处理
        }
    }
 * \endcode
 * \note 
*******************************************************************************/
bool get_app_msg(private_msg_t *private_msg)
{
    //软定时器扫描
    handle_timers();
    
#ifdef PC
    UpdateMainWnd();
#endif
    
    if(g_this_app_info->app_type == APP_TYPE_GUI)
    {
        //前台应用
        sys_msg_t sys_msg;
        
        //获取系统消息并广播
        get_sys_msg(&sys_msg);
    }
    //接收 应用私有消息
    if(sys_mq_receive((uint32)(g_this_app_info->mq_id), (void *)private_msg) == 0)
    {
        com_app_msg_hook(private_msg);
        return TRUE;
    }
    
    //消息队列已空，返回false
    return FALSE;
}

/*! \endcond */
