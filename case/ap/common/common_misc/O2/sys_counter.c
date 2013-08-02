/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-20 9:42     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _sys_counter.c
 * \brief    common 系统计时器功能接口实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现一些应用层计时相关公共功能接口。
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_ui.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    系统计时软定时器服务例程，处理屏幕背光，屏保，省电关机，睡眠关机，回到正在
 *    播放等。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
 * \li  系统定时功能，包括背光，屏保，省电关机，睡眠关机，app休眠，由前台AP进行计时。
 *      即在前台AP创建一个定时器，专用于上面五个功能计时器的计时，在 get_app_msg 中
 *      调用 handle_timers 去做检测，一旦定时已到，触发对应定时功能。
 * \li  屏幕背光完全由 common 模块控制，无须用户关心，用户可以通过 get_backlight_state 
 *      来获取背光状态。
 * \li  屏幕保护同样完全由 common 模块控制，无须用户关心。等需要进入屏幕保护时，先发送
 *      异步消息 MSG_ENTER_SCREEN_SAVE 给当前AP，在 com_message_box 中进入屏保。
 * \li  省电关机和睡眠（定时）关机则是广播 MSG_POWER_OFF 消息，manager ap 在收到该消息
 *      后，发送 MSG_APP_QUIT 让前台和后台AP自己退出。
 * \li  返回正在播放界面通过以"返回正在播放的模式"创建对应的前台AP或者发送异步消息 back_to_playing
 *      让前台AP自己回到正在播放场景。返回正在播放界面需要AP的正在播放场景协助，即在进入场景时 
 *      change_app_state(APP_STATE_PLAYING)，在退出场景时 change_app_state(APP_STATE_NO_PLAY)。
*******************************************************************************/
void sys_counter_handle(void)
{
    msg_apps_t msg;
    uint8 cur_light_timer = sys_comval->light_timer;
    uint8 cur_save_timer = sys_comval->screen_saver_timer;
    uint8 cur_poff_timer = sys_comval->poweroff_timer;
    uint8 cur_sleep_timer = sys_comval->sleep_timer;
    uint8 cur_playing_timer = sys_comval->playing_timer;
    uint8 cur_ss_mode = sys_comval->screen_saver_mode;
    uint8 cur_dark_lightoff_timer = sys_comval->dark_lightoff_timer;
    uint8 cur_screen_lightoff_timer = sys_comval->screen_lightoff_timer;
    
    /* 开启背光亮度计时 */
    //背光时间开启只能通过菜单选择，即有按键处理，会在 com_gui_msg_hook 中把计时器清0，
    //所以这里无需重复，屏保也是如此
    //if((cur_light_timer != 0)
    //&& (cur_light_timer != g_app_last_state.last_light_timer) )
    //{
    //    g_sys_counter.light_counter = 0;
    //}

    /* 背光变暗计时器 */
    if((cur_light_timer != 0)
    && (g_app_info_state.backlight_state == BACKLIGHT_STATE_NORMAL)//背光处于正常状态中
    && (g_app_info_state.app_state != APP_STATE_PLAYING_ALWAYS))//播放中，但无需常亮背光
    {
        g_sys_counter.light_counter++;
        if(g_sys_counter.light_counter >= cur_light_timer)
        {
            //背光调暗，并修改背光状态
            com_set_contrast(0);//调到最暗
            g_app_info_state.backlight_state = BACKLIGHT_STATE_DARK;
        }
    }
    
    /* 开启屏幕保护计时 */
    //if((cur_ss_mode != 0)
    //&& (cur_ss_mode != g_app_last_state.last_screen_save_mode) )
    //{
    //    g_sys_counter.screen_saver_counter = 0;
    //}
    
    /* 屏幕保护计时器 */
    if((cur_save_timer != 0) 
    && (cur_ss_mode != 0)
    && (g_app_info_state.screensave_state == FALSE)//没有处于屏保状态中
    && (g_app_info_state.app_state != APP_STATE_PLAYING_ALWAYS))//播放中，但无需常亮背光
    {
        g_sys_counter.screen_saver_counter++;
        if(g_sys_counter.screen_saver_counter >= cur_save_timer)
        {
            //给当前AP发送 enter_screen_save 异步消息，在 com_message_box 中进入屏保
            msg.type = MSG_APP_ENTER_SCREEN_SAVE;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
    }
    
    /* 背光关掉计时器，实现为进入关掉背光的屏保模式 */
    if((cur_dark_lightoff_timer != 0)//启动背光变暗后一段时间自动关掉背光功能
    && (g_app_info_state.backlight_state == BACKLIGHT_STATE_DARK)//背光变暗状态
    && (cur_ss_mode == SCREEN_NULL))//没有设置屏保模式
    {
        g_sys_counter.light_counter++;
        if(g_sys_counter.light_counter >= (cur_light_timer + cur_dark_lightoff_timer))
        {
            //给当前AP发送 enter_screen_save 异步消息，在 com_message_box 中进入屏保
            msg.type = MSG_APP_ENTER_SCREEN_SAVE2;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
    }
    if((cur_screen_lightoff_timer != 0)//启动屏保后一段时间自动关掉背光功能
    && (g_app_info_state.screensave_state == TRUE)//进入屏保状态
    && (g_ss_mode != SCRREN_SCREEN_OFF)//当前屏保模式不是关掉背光
    && (g_app_info_state.backlight_state != BACKLIGHT_STATE_OFF))//尚未关掉背光
    {
        g_sys_counter.screen_saver_counter++;
        if(g_sys_counter.screen_saver_counter >= (cur_save_timer + cur_screen_lightoff_timer*120))
        {
            //给当前AP发送 enter_screen_save 异步消息，在 com_message_box 中进入屏保
            msg.type = MSG_APP_ENTER_SCREEN_SAVE2;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
    }
    
    /* 开启省电关机计时 */
    if((  (g_app_last_state.last_state == ENGINE_STATE_PLAYING)
        ||(g_app_last_state.last_app_state == APP_STATE_PLAYING))
    && (  (g_app_info_state.state != ENGINE_STATE_PLAYING)
        &&(g_app_info_state.app_state == APP_STATE_NO_PLAY)) )
    {
        g_sys_counter.poweroff_counter = 0;
    }
    
    /* 省电关机计时器 */
    if((cur_poff_timer != 0)
    && (g_app_info_state.cable_state == CABLE_STATE_CABLE_NULL)//USB电缆没有插入
    && (g_app_info_state.state != ENGINE_STATE_PLAYING)//后台引擎不处于播放状态
    && (g_app_info_state.app_state == APP_STATE_NO_PLAY))//前台应用不处于播放 UI 状态
    {
        g_sys_counter.poweroff_counter++;
        if(g_sys_counter.poweroff_counter >= cur_poff_timer)
        {
            //广播关机消息
            msg.type = MSG_POWER_OFF;
            broadcast_msg(&msg);
        }
    }
    
    /* 开启睡眠关机计时 */
    if((cur_sleep_timer != 0)
    && (cur_sleep_timer != g_app_last_state.last_sleep_timer))
    {
        g_sys_counter.sleep_counter = 0;
    }
    
    /* 睡眠关机计时器 */
    if((cur_sleep_timer != 0) 
    && (g_app_info_state.cable_state == CABLE_STATE_CABLE_NULL))//USB电缆没有插入
    {
        g_sys_counter.sleep_counter++;
        if(g_sys_counter.sleep_counter >= (cur_sleep_timer * 120))
        {
            //广播关机消息
            msg.type = MSG_POWER_OFF;
            broadcast_msg(&msg);
        }
    }
    
    /* 开启返回正在播放计时 */
    if((  (g_app_last_state.last_state == ENGINE_STATE_PLAYING)
        ||(g_app_last_state.last_app_state == APP_STATE_PLAYING))
    && (  (g_app_info_state.state != ENGINE_STATE_PLAYING)
        &&(g_app_info_state.app_state == APP_STATE_NO_PLAY)) )
    {
        g_sys_counter.playing_counter = 0;
    }
    
    /* 返回正在播放计时器 */
    if((cur_playing_timer != 0) 
    && (g_app_info_state.screensave_state == FALSE)//系统不在屏保状态中
    && (g_app_info_state.state == ENGINE_STATE_PLAYING)//后台引擎不处于播放状态
    && (g_app_info_state.app_state == APP_STATE_NO_PLAY))//前台应用不处于播放 UI 状态
    {
        g_sys_counter.playing_counter++;
        if(g_sys_counter.playing_counter >= cur_playing_timer)
        {
            //给当前AP发送 back_to_playing 异步消息
            msg.type = MSG_APP_BACKTO_PLAYING;
            send_async_msg(g_this_app_info->app_id, &msg);
        }
    }
    
    /* 获取电池电量，并检测低电；周期为 10 秒 */
    g_sys_counter.battery_counter++;
    if(g_sys_counter.battery_counter >= 20)
    {
        uint8 tmp_battery_value;    //当前电池电量
        uint8 tmp_charge_state;    //当前充电状态，分无充电，正在充电，充电已满，无电池
        bool bat_now = FALSE;
        msg.type = MSG_NULL;
            
        if(g_sys_counter.battery_counter >= 100)//立即检测电池状态及电量标志
        {
            //连续采样7次
            g_bat_index = BAT_SAMPLE_TIME;
            bat_now = TRUE;
        }
        if(g_bat_index >= BAT_SAMPLE_TIME)
        {
            g_sys_counter.battery_counter = 0;
        	
            if(g_app_info_state.cable_state == CABLE_STATE_CABLE_NULL)//USB电缆没有插入
            {
                tmp_charge_state = CHARGE_NONE;
            }
            else
            {
                tmp_charge_state = (uint8)(int)key_chargeget(0);
            }
            tmp_battery_value = com_get_battery_grade(bat_now);
            
            if((g_app_info_state.charge_state != CHARGE_FULL) 
            && (tmp_charge_state == CHARGE_FULL))//第一次检测到充电已满才提示
            {
                //检测充电已满，广播充电已满消息
                msg.type = MSG_FULL_CHARGE;
            }
            else if((tmp_charge_state == CHARGE_NONE) 
            && (tmp_battery_value == 0))//电池供电，且检测到低电
            {
                if(g_app_info_state.cable_state == CABLE_STATE_CABLE_NULL)//USB电缆没有插入
                {
                    //检测到低电，广播低电消息
                    msg.type = MSG_LOW_POWER;
                }
            }
            else
            {
                //do nothing for QAC
            }
            
            g_app_info_state.charge_state = tmp_charge_state;
            g_app_info_state.bat_value = tmp_battery_value;
            if(msg.type != MSG_NULL)//先改全局电池状态及电量再发送消息
            {
                broadcast_msg_sys(&msg);//广播充满电或低电消息，不发送给 manager ap
            }
        }
        else
        {
            g_bat_sample[g_bat_index] = act_readb(BATADC_DATA);//0-6bit有效
            g_bat_index++;
        }
    }
    
    /* 更新最近状态 */
    g_app_last_state.last_state = g_app_info_state.state;
    g_app_last_state.last_app_state = g_app_info_state.app_state;
    //g_app_last_state.last_light_timer = cur_light_timer;
    //g_app_last_state.last_screen_save_mode = cur_ss_mode;
    g_app_last_state.last_sleep_timer = cur_sleep_timer;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    系统状态更新软定时器服务例程，包括顶部状态栏更新等。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      misc_func
 * \note
*******************************************************************************/
void sys_status_handle(void)
{
    /* 顶部状态栏更新 */
    g_battery_display++;//电池动态，放在这里递增是为了连续多次刷新 gui_headbar 电池电量显示不变
    if(g_headbar_update == HEADBAR_UPDATE_INIT)
    {
        gui_headbar(HEADBAR_UPDATE_INIT);
        g_headbar_update = HEADBAR_UPDATE_CHANGE;
    }
    else
    {
        gui_headbar_handle();
    }
}

/*! \endcond */

/*! \cond */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    演示模式屏幕保护软定时器服务例程，用来周期切换演示图片
 * \param[in]    none
 * \param[out]   none    
 * \return       none
 * \note 
*******************************************************************************/
void demo_mode_handle(void)
{
    picbox_private_t ss_pic;
    style_infor_t style_infor;
    
    ss_pic.pic_id = V_U16_INVALID;
    ss_pic.frame_id = g_ss_demo_mode_phase;
    style_infor.style_id = DEMO_MODE_PIC;
    style_infor.type = UI_COM;
    ui_show_picbox(&style_infor, &ss_pic);
    g_ss_demo_mode_phase++;
    if(g_ss_demo_mode_phase >= g_ss_demo_frame_cnt)
    {
        g_ss_demo_mode_phase = 0;
    }
}

/*! \endcond */
