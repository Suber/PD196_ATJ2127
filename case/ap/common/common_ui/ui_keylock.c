/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-keylock
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 16:52     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_keylock.c
 * \brief    commonUI 按键锁功能实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种按键锁GUI。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的picbox完成控件显示流任务。
 * \version 1.0
 * \date  2011-9-19
*******************************************************************************/

#include "common_ui.h"

//gui app msg deal
app_result_e keylock_msg_deal(key_lock_mode_e *this_lock_mode, key_lock_mode_e *last_lock_mode)
{
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    app_result_e result = RESULT_NULL;
    
    //获取gui消息
    if(get_gui_msg(&input_msg) == TRUE)//有gui消息
    {           
        //进行按键映射
        if(com_key_mapping(&input_msg, &gui_event, keylock_list) == TRUE)
        {
            switch(gui_event)
            {
            case EVENT_ENTER_KEYLOCK:
                switch(*last_lock_mode)
                {
                case KEY_LOCK_MODE_HOLD:
                    if(get_keylock_state() == TRUE)
                    {
                        *this_lock_mode = KEY_LOCK_MODE_UNLOCKING;
                    }
                    else
                    {
                        *this_lock_mode = KEY_LOCK_MODE_LOCKING;
                    }
                    break;
                
                case KEY_LOCK_MODE_LOCKING:
                    *this_lock_mode = KEY_LOCK_MODE_UNLOCKING;
                    break;
                
                default:
                    *this_lock_mode = KEY_LOCK_MODE_LOCKING;
                    break;
                }
                
                *last_lock_mode = *this_lock_mode;
                result = RESULT_REDRAW;//强迫退出第2层while循环
                break;
                
            default:
                result = com_message_box(gui_event);
                break;
            }
        }
    }
    else
    {
        if(get_app_msg(&private_msg) == TRUE)
        {
            result = g_this_app_msg_dispatch(&private_msg);
        }
    }
    
    return result;
}

//keylock_handle
void keylock_handle(void)
{
    g_kl_time_is_up = TRUE;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    按键锁主函数，是按键锁模块的外部接口
 * \param[in]    lockkey 是否按下“锁键”进入，TRUE表示是，FALSE表示否
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_REDRAW 正常返回AP，重绘UI
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \note 
*******************************************************************************/
app_result_e gui_keylock (bool lockkey)
{
    app_result_e result = RESULT_NULL;
    animation_com_data_t anm_com;
    style_infor_t style_infor;
    key_lock_mode_e this_lock_mode, last_lock_mode;
    int8 wait_timer;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup;
    gui_direct_e direct_bk;
    
    //初始化进入锁键 UI 状态
    if(lockkey == TRUE)
    {
        if(get_keylock_state() == TRUE)
        {
            this_lock_mode = KEY_LOCK_MODE_UNLOCKING;
        }
        else
        {
            this_lock_mode = KEY_LOCK_MODE_LOCKING;
        }
    }
    else
    {
        this_lock_mode = KEY_LOCK_MODE_HOLD;
    }
    last_lock_mode = this_lock_mode;
    
    //config, playlist, udisk, 等应用仅修改按键锁状态后立即退出
    if((g_this_app_info->app_id == APP_ID_CONFIG)
    || (g_this_app_info->app_id == APP_ID_PLAYLIST)
    || (g_this_app_info->app_id == APP_ID_UDISK))
    {
        //设置锁键状态
        if(last_lock_mode == KEY_LOCK_MODE_LOCKING)
        {
            change_keylock_state(TRUE);
        }
        else if(last_lock_mode == KEY_LOCK_MODE_UNLOCKING)
        {
            change_keylock_state(FALSE);
        }
        else
        {
            //do nothing for QAC
        }
        
        return RESULT_NULL;
    }

    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_KEYLK);
    
    direct_bk = com_get_gui_direction();
    com_set_gui_direction(GUI_DIRECT_NORMAL);
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    
    //临时解锁，以响应按键
    change_keylock_state(FALSE);
    
    //刷背景图
    style_infor.style_id = LOCK_BG;
    style_infor.type = UI_COM;
    ui_show_picbox(&style_infor, NULL);

    while(1)
    {
        //绘制按键锁UI
        if((this_lock_mode == KEY_LOCK_MODE_LOCKING) || (this_lock_mode == KEY_LOCK_MODE_UNLOCKING))
        {
            anm_com.interval = 250;
            
            if(this_lock_mode == KEY_LOCK_MODE_LOCKING)
            {
                anm_com.direction = DIRECTION_NORMAL;
            }
            else
            {
                anm_com.direction = DIRECTION_REVERSE;
            }
            
            anm_com.forbid = TRUE;//允许中止
            anm_com.key_map_list = keylock_list;
            anm_com.callback = NULL;
            
            style_infor.style_id = LOCK_ANIMATION;
            style_infor.type = UI_COM;
            result = gui_animation(&style_infor, &anm_com);
            if((result != RESULT_NULL) && (result != RESULT_REDRAW))
            {
                //动画显示终止返回（因用户gui输入而终止）
                if(result == EVENT_ENTER_KEYLOCK)
                {
                    if(this_lock_mode == KEY_LOCK_MODE_UNLOCKING)
                    {
                        this_lock_mode = KEY_LOCK_MODE_LOCKING;
                    }
                    else
                    {
                        this_lock_mode = KEY_LOCK_MODE_UNLOCKING;
                    }
                    last_lock_mode = this_lock_mode;
                    continue;
                }
                else//其他原因返回
                {
                    break;//退出控件
                }
            }
        }
        else
        {           
            style_infor.style_id = LOCK_HOLD;
            style_infor.type = UI_COM;
            ui_show_picbox(&style_infor, NULL);
        }
        
        //等待2秒钟后退出，在此期间如果又按下按键锁，将又一次触发按键锁处理
        wait_timer = set_app_timer(APP_TIMER_ATTRB_UI, 2000, keylock_handle);
        this_lock_mode = KEY_LOCK_MODE_NULL;
        g_kl_time_is_up = FALSE;
        while(1)
        {
            if(g_kl_time_is_up == TRUE)
            {
                //正常返回
                result = RESULT_REDRAW;
                break;
            }
            
            result = keylock_msg_deal(&this_lock_mode, &last_lock_mode);
            if(result != RESULT_NULL)
            {
                break;
            }
            
            //挂起10ms，多任务调度
            sys_os_time_dly(1);
        }
        kill_app_timer(wait_timer);
        if(this_lock_mode == KEY_LOCK_MODE_NULL)
        {
            break;
        }
    }
    
    //设置锁键状态
    if((last_lock_mode == KEY_LOCK_MODE_LOCKING) || (last_lock_mode == KEY_LOCK_MODE_HOLD))
    {
        change_keylock_state(TRUE);
    }
    else//KEY_LOCK_MODE_UNLOCKING
    {
        change_keylock_state(FALSE);
    }
    
    com_set_gui_direction(direct_bk);
    change_app_timer_tag(tag_backup);
    gui_set_headbar_mode(mode_backup, icon_backup);
    return result;
}
