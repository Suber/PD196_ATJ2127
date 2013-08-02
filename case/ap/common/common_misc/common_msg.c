/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-5-11 15:07     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _miscellaneous.c
 * \brief    common 应用私有消息勾函数
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               应用私有消息勾函数
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-5-11
*******************************************************************************/

#include "common_ui.h"

#define  PLUGINSTATUS    (1<<6)

/******************************************************************************/
/*!                    
 * \par  Description:
 *    前台应用私有消息勾函数。
 * \param[in]    private_msg 获取到的私有消息指针
 * \param[out]   none 
 * \return       none
 * \ingroup      misc_func
 * \note 
*******************************************************************************/
void com_app_msg_hook(private_msg_t *private_msg)
{
    msg_apps_type_e msg_type = private_msg->msg.type;
    
    switch(msg_type)
    {
    case MSG_SD_IN:
        if(get_card_state() == CARD_STATE_CARD_NULL)//只有在CARD_NULL状态下才需要设置
        {
            change_card_state(CARD_STATE_CARD_IN);
        }
        change_card_plist(FALSE);
        change_card_update(FALSE);
        g_ss_vip_event = TRUE;
        break;
    case MSG_SD_OUT:
        change_card_state(CARD_STATE_CARD_NULL);
        change_card_plist(FALSE);
        change_card_update(FALSE);
        g_ss_vip_event = TRUE;
        break;
        
    case MSG_UH_IN:
    case MSG_UH_OUT:
        g_ss_vip_event = TRUE;
        break;
        
    case MSG_USB_STICK:
        /* wait 50ms for USB-connect stably. */
        sys_os_time_dly(20);
        
        if((act_readb(DPDMCTRL)&PLUGINSTATUS) == 0)//ADAPTOR适配线插入
        {
            private_msg->msg.type = MSG_ADAPTOR_IN;//改为MSG_ADAPTOR_IN消息
            change_cable_state(CABLE_STATE_ADAPTOR_IN);
        }
        else
        {
            change_cable_state(CABLE_STATE_CABLE_IN);
        }
    
        g_ss_vip_event = TRUE;
        break;
    case MSG_USB_UNSTICK:
        if(get_cable_state() == CABLE_STATE_ADAPTOR_IN)//ADAPTOR适配线拔出
        {
            private_msg->msg.type = MSG_ADAPTOR_OUT;//改为MSG_ADAPTOR_OUT消息
        }
        change_cable_state(CABLE_STATE_CABLE_NULL);
        g_ss_vip_event = TRUE;
        break;
        
    case MSG_POWER_OFF:
    case MSG_LOW_POWER:
    case MSG_RTCALARM:
        g_ss_vip_event = TRUE;
        break;
    
    default:
        break;
    }
    
    if(g_ss_vip_event == TRUE)
    {
        //若背光处于调暗状态，恢复背光亮度，并修改背光状态
        if(get_backlight_state() == BACKLIGHT_STATE_DARK)
        {
            com_set_contrast(sys_comval->lightness);
            change_backlight_state(BACKLIGHT_STATE_NORMAL);
        }
        
        //若处于屏幕保护模式，退出屏幕保护模式
        if(get_screensave_state() == TRUE)
        {
            msg_apps_t msg;
            
            //先响应退出屏幕保护模式，再响应当前消息，即偷梁换柱为退出屏幕保护模式消息，然后重发一次当前消息
            //1.返回 exit_screen_save 异步消息，在 com_message_box 中退出屏保
            private_msg->msg.type = MSG_APP_EXIT_SCREEN_SAVE;
            
            //2.重发一次当前消息
            msg.type = msg_type;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
        else
        {
            g_ss_vip_event = FALSE;
        }
        
        //背光关闭计时，屏幕保护计时，省电关机计时，返回正在播放计时 清0
        g_sys_counter.light_counter = 0;
        g_sys_counter.screen_saver_counter = 0;
        g_sys_counter.poweroff_counter = 0;
        g_sys_counter.playing_counter = 0;
    }
}

/*! \endcond */
