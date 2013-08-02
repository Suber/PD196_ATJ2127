/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-14 16:58     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_msgmap.c
 * \brief    按键（包括触摸屏，g-sensor）消息映射接口实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现按键（包括触摸屏，g-sensor）映射和快捷键映射功能。
 * \par      EXTERNALIZED FUNCTIONS:
 *               无
 * \version  1.0
 * \date     2011-10-14
*******************************************************************************/

#include "common_func.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    按键消息映射，将 gui 消息映射为事件，以支持不同按键输入对应统一业务处理。
 * \param[in]    input_msg 要进行映射的 gui 消息
 * \param[in]    key_map_list 进行映射的按键映射表
 * \param[out]   gui_event 存储映射后事件
 * \return       bool
 * \retval           TRUE 映射得到有效 gui 事件。
 * \retval           FALSE 没有映射得到有效 gui 事件，按键消息被过滤掉了。
 * \ingroup      common_func
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
 * \li  按键映射接口中，会用两个映射表进行映射，一个是接口内默认的快捷键映射表，
 *      一个是用户自定义的按键映射表，后者优先级比较高。
 * \li  如果用户不想响应某个快捷键，或者想把某个快捷键另作别用，那么可以在自定义
 *      的按键映射表中添加该按键的映射项。
*******************************************************************************/
bool com_key_mapping(input_gui_msg_t *input_msg, msg_apps_type_e *gui_event, const key_map_t *key_map_list)
{
    const key_map_t *this_key_map;
    
    //按键（机械和触摸）
    if(INPUT_MSG_KEY == input_msg->type)
    {
        if(key_map_list != NULL)
        {
            this_key_map = key_map_list;
            for(;;this_key_map++)
            {
                /* 达到用户最后一个映射，结束按键映射 */
                if ( KEY_NULL == this_key_map->key_event.val )
                {
                    break;
                }
    
                /* 找到需要映射的按键, 进行映射 */
                if( (input_msg->data.kmsg.val == this_key_map->key_event.val)
                  &&(input_msg->data.kmsg.type & this_key_map->key_event.type) )
                {
                    *gui_event = this_key_map->event;
                    if(*gui_event == MSG_NULL)
                    {
                        return FALSE;
                    }
                    else
                    {
                        return TRUE;
                    }
                }
            }
        }
        
        //快捷键映射
        this_key_map = key_shortcut_list;
        for (;;this_key_map++)
        {
            /* 达到用户最后一个快捷键, 或者达到最大快捷键个数, 退出*/
            if ( 0 == this_key_map->key_event.val )
            {
                break;
            }

            /* 找到快捷键, 发送快捷键对应消息 */
            if ( (input_msg->data.kmsg.val == this_key_map->key_event.val) 
              && (input_msg->data.kmsg.type & this_key_map->key_event.type) )
            {
                *gui_event = this_key_map->event;
                return TRUE;
            }
        }
    }
    ////触摸屏
    //else if(INPUT_MSG_TOUCH == input_msg->type)
    //{}
    ////g-sensor
    //else
    //{}

    return FALSE;
}

/*! \endcond */
