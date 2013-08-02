/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 21:49     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _app_sleep.c
 * \brief    common 应用睡眠
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               应用睡眠接口实现
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    应用层休眠，可以接收 gui 消息。
 * \param[in]    sleep_timer 睡眠时间，以0.5秒为单位
 * \param[in]    key_map_list 终止睡眠的按键映射列表，为NULL表示任意按键都会终止睡眠
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_NULL 睡眠时间到时返回
 * \retval           gui_event 系统消息返回处理，避免嵌套
 * \ingroup      misc_func
 * \par          exmaple code
 * \code
    例子1：等待2秒钟，按任意按键退出
    app_result_e result;
    
    result = com_app_sleep(2, NULL);
    if(result != RESULT_NULL)
    {
        因按键按下退出
    }
    
    例子2：等待2秒钟，捕获 key_map_list 中的按键事件
    app_result_e result;
    
    result = com_app_sleep(2, key_map_list);
    if(result != RESULT_NULL)
    {
        因按键中断退出，result 表示该按键对应的事件，应进行分发处理
    }
    
 * \endcode
 * \note 
 * \li  在睡眠中一般不对按键消息处理，所以一般 key_map_list 为NULL。
 * \li  如果用户希望可以通过某个按键消息终止睡眠，那么就将该按键消息添加到 key_map_list 
 *      并传递进入即可。
 * \li  如果用户不想接收到 gui 快捷事件时退出睡眠，可以把该快捷按键添加到 key_map_list 中，
 *      但映射结果必须是 MSG_NULL。
*******************************************************************************/
app_result_e com_app_sleep(uint32 sleep_timer, const key_map_t *key_map_list)
{
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    uint32 sleep_500ms_timer;
    
    //计算 10ms 计时器
    sleep_500ms_timer = sleep_timer;
    if(sleep_500ms_timer == 0)
    {
        return RESULT_NULL;
    }
    while (1)
    {
        //获取gui消息
        while(get_gui_msg(&input_msg) == TRUE)//有gui消息
        {
            if(key_map_list != NULL)//有按键映射表，就按键映射表进行映射处理
            {
                //进行按键映射
                if(com_key_mapping(&input_msg, &gui_event, key_map_list) == TRUE)
                {
                    return gui_event;
                }
            }
            else//按任意按键返回
            {
                //启动过滤后续按键
                com_filter_key_hold();
                return RESULT_NULL;
            }
        }
               
        //挂起10ms，多任务调度
        sys_os_time_dly(50);
        
        //睡眠时间计时器递减
        sleep_500ms_timer--;
        //睡眠时间已到，退出
        if(sleep_500ms_timer == 0)
        {
            return RESULT_NULL;
        }
    }
}

/*! \endcond */
