/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-20 9:42     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_key.c
 * \brief    按键（包括触摸屏，g-sensor）发生公共特殊处理实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               包括按键消息过滤，背光处理，按键锁，按键音，等等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               applib
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_func.h"
#include "applib.h"

extern comval_t *sys_comval;
extern void com_set_contrast(uint8 lightness) __FAR__;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    按键发生特殊处理，作为 gui 消息处理的回调函数。
 * \param[in]    input_msg 获取到的 gui消息指针
 * \param[out]   input_msg 可能会对input_msg进行修改，比如将val修改为KEY_HOLD虚拟按键
 * \return       bool
 * \retval           TRUE 获取到的按键有效，没有被过滤掉。
 * \retval           FALSE 获取到的按键无效，被过滤掉了。
 * \ingroup      common_func
 * \note         
*******************************************************************************/
bool com_gui_msg_hook(input_gui_msg_t *input_msg)
{
    bool ret;
    
    //0.长按按键后过滤掉后续按键
    if(get_filter_key() != KEY_NULL)
    {
        //直到检测到按键抬起（或者其他按键按下）才算过滤完成
        if(input_msg->data.kmsg.val == get_filter_key())
        {
            //过滤LONG和HOLD按键
            if(input_msg->data.kmsg.type != KEY_TYPE_SHORT_UP)
            {
                return FALSE;
            }
            else//过滤完成，但还是不响应SHORT_UP按键
            {
                change_filter_key(KEY_NULL);
                return FALSE;
            }
        }
        else//其他按键，过滤完成，继续处理
        {
            change_filter_key(KEY_NULL);
        }
    }
    
    //更新当前按键值
    this_filter_key_value = input_msg->data.kmsg.val;
    
    //1.1若背光处于调暗状态，恢复背光亮度，并修改背光状态
    if(get_backlight_state() == BACKLIGHT_STATE_DARK)
    {
        com_set_contrast(sys_comval->lightness);
        change_backlight_state(BACKLIGHT_STATE_NORMAL);
    }
    
    //1.2若处于屏幕保护模式，退出屏幕保护模式
    if(get_screensave_state() == TRUE)
    {
        msg_apps_t msg;
        
        //给当前AP发送 exit_screen_save 异步消息，在 com_message_box 中退出屏保
        msg.type = MSG_APP_EXIT_SCREEN_SAVE;
        send_async_msg(g_this_app_info->app_id, &msg);
        
        ret = FALSE;
    }
    //1.3正常状态下，处理按键音，按键锁
    else
    {
        //2.按键音处理，仅在按键按下时发出按键音
        if(input_msg->data.kmsg.type == KEY_TYPE_DOWN)
        {
            soundout_state_e state = (soundout_state_e)get_soundout_state();
            if(((state == SOUND_OUT_STOP) || (state == SOUND_OUT_PAUSE))
            && (get_keytone_enable() == TRUE))
            {
                //没音频输出且按键音使能，发出按键音
                start_key_tone(0);
            }
        }
        
        //3.按键锁处理，只处理按键锁住状态下按下普通按键
        //例外：config, playlist, udisk, 等应用不检测按键锁
        if((g_this_app_info->app_id != APP_ID_CONFIG)
        && (g_this_app_info->app_id != APP_ID_PLAYLIST)
        && (g_this_app_info->app_id != APP_ID_UDISK))
        {
            if( (get_keylock_state() == TRUE )
              &&(input_msg->data.kmsg.val != KEY_UNLOCK) )
            {
                //按键锁住状态下，按键普通按键后，转换为该按键，以供应用层选择性处理
                input_msg->data.kmsg.val = KEY_LOCKHOLD;
                //过滤掉当前按键后续动作，保证只响应一次“按键锁住”提示
                com_filter_key_hold();
            }
        }
        
        ret = TRUE;
    }
    
    //4.背光关闭计时，屏幕保护计时，省电关机计时，返回正在播放计时清0
    g_sys_counter.light_counter = 0;
    g_sys_counter.screen_saver_counter = 0;
    g_sys_counter.poweroff_counter = 0;
    g_sys_counter.playing_counter = 0;
    
    return ret;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    过滤掉当前按键后续动作，即相同值按键消息。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      common_func
 * \note
 * \li  该接口用于过滤当前按键值的后续动作，如需过滤掉任意指定按键，需直接调用
 *      change_filter_key 设置。
 * \li  在检测到当前按键抬起消息或者其他按键消息后，由 com_gui_msg_hook 自动调用解除。
 * \li  如果此时没有按键按下，该接口调用不会产生真正的按键过滤效果。
*******************************************************************************/
void com_filter_key_hold(void)
{
    change_filter_key(this_filter_key_value);
}

/*! \endcond */
