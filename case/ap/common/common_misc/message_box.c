/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-20 9:44     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _message_box.c
 * \brief    common 默认应用消息处理
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现对默认应用消息的统一处理，方便统一开发和用户使用。
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2011-9-20
*******************************************************************************/

#include "common_ui.h"

extern void deal_antenna_detect(bool in_out) __FAR__;

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    gui 快捷事件 和 系统消息处理函数
 * \param[in]    msg_type 待处理的消息（事件）类型
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_NULL 返回空结果，AP可不进行任何处理
 * \retval           RESULT_REDRAW 返回重绘UI，AP一般需要重新 redraw 所有UI
 * \retval           other 直接返回消息（事件）处理的结果，具体参见 app_result_e 定义
 * \ingroup      misc_func
 * \par          exmaple code
 * \code
    例子1：在按键消息（事件）循环中调用，处理快捷按键事件
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
    
    例子2：在应用私有消息处理中，处理默认系统消息
    private_msg_t private_msg;
    msg_apps_type_e msg_type
    app_result_e result;
    
    if(get_app_msg(&private_msg) == TRUE)
    {
        msg_type = private_msg.msg.type;
        switch(msg_type)
        {
        case XXX://处理应用私有消息，或者一些应用需要特殊处理的系统消息
            break;
            
        default:
            result = com_message_box(msg_type);
            break;
        }
    }
 * \endcode
 * \note 
 * \li  com_message_box 是用默认的方式对 gui 快捷事件和系统消息 进行处理。
 * \li  如果用户要对某个 gui 快捷事件或系统消息进行特殊处理，那么应该在 gui 事件分发
 *      或应用私有消息分发的 case 分支中抢先处理掉。
*******************************************************************************/
app_result_e com_message_box(msg_apps_type_e msg_type)
{
    app_result_e result = RESULT_NULL;
    
    switch(msg_type)
    {
    /************************** 快捷键处理 ******************************/
    /* 长按 RETURN 键返回 mainmenu 应用事件 */
    case EVENT_RETURN_MAINMENU:
        {
            //在非 mainmenu AP响应
            if(g_this_app_info->app_id != APP_ID_MAINMENU)
            {
                //退出当前AP，回到 MainMenu AP
                result = RESULT_MAIN_APP;
            }
        }
        break;
        
    /* 按下 VOL+ 或 VOL- 进入音量调节界面事件 */
    case EVENT_ENTER_VOLUMEBAR:
        {
            result = gui_volumebar(&sys_comval->volume_current, &sys_comval->volume_limit, SET_VOLUME_VALUE);
        }
        break;
        
    /* 拨动 LOCK 键到常关，进入按键锁处理事件 */
    case EVENT_ENTER_KEYLOCK:
        {
            result = gui_keylock(TRUE);
        }
        break;
    
    /* 拨动 LOCK 键到常关，进入按键锁处理事件 */
    case EVENT_ENTER_KEYLOCK_UN:
        {
            result = gui_keylock(TRUE);
        }
        break;
    
    /*! 按键锁住状态下，按任意按键进入按键锁保持事件 */ 
    case EVENT_ENTER_KEYLOCK_HOLD:
        {
            result = gui_keylock(FALSE);
        }
        break;
        
    /* 长按 PLAY 键进入关机请求确认事件 */
    case EVENT_ENTER_SHUTOFF_ASK:
        {
            result = gui_shut_off();
        }
        break;
    
    /************************** 系统消息处理 ******************************/
    /* 进入屏幕保护事件 */
    case MSG_APP_ENTER_SCREEN_SAVE:
        {
            gui_screen_save_enter(sys_comval->screen_saver_mode);
        }
        break;
    
    /* 进入屏幕保护事件，关掉背光 */
    case MSG_APP_ENTER_SCREEN_SAVE2:
        {
            if(get_screensave_state() == TRUE)
            {
                //关掉背光灯，关掉背光前先清屏，效果更好
                ui_set_backgd_color(0x0000);//清为全黑屏
                ui_clear_screen(NULL);
                lcd_backlight_on_off(FALSE);
                lcd_standby_screen(FALSE);
                change_backlight_state(BACKLIGHT_STATE_OFF);
                
                //调低频率为 3M
                g_ss_clk_backup = (uint8)sys_adjust_clk(FREQ_3M, 0);
            }
            else
            {
                gui_screen_save_enter(SCRREN_SCREEN_OFF);
            }
        }
        break;
    
    /* 进入屏幕保护事件 */
    case MSG_APP_EXIT_SCREEN_SAVE:
        {
            result = gui_screen_save_exit();
            com_filter_key_hold();//过滤后续按键
        }
        break;
    
    /* 回到正在播放界面 */
    case MSG_APP_BACKTO_PLAYING:
        {
            msg_apps_t msg;
            
            if(g_ss_delay_flag == TRUE)//delay退出屏保
            {
                gui_screen_save_exit();
                g_ss_delay_flag = FALSE;
            }
            
            if (get_engine_type() == ENGINE_MUSIC)
            {
                if(g_this_app_info->app_id == APP_ID_MUSIC)
                {
                    break;//异常，在 music 应用中应该处理该消息
                }
                msg.content.data[0] = APP_ID_MUSIC;
            }
            else
            {
                if(g_this_app_info->app_id == APP_ID_RADIO)
                {
                    break;//异常，在 radio 应用中应该处理该消息
                }
                msg.content.data[0] = APP_ID_RADIO;
            }
            msg.content.data[1] = PARAM_BACKTO_PLAYING;
            msg.type = MSG_CREAT_APP;
            send_async_msg(APP_ID_MANAGER, &msg);
            
            result = RESULT_APP_QUIT;
        }
        break;
    
    /* 系统低电消息 */
    case MSG_LOW_POWER:
        {
            private_msg_t private_msg;
            msg_apps_t msg;
            style_infor_t style_infor;

            //显示低电图标
            style_infor.style_id = LOW_POWER_BG;
            style_infor.type = UI_COM;
            ui_show_picbox(&style_infor, NULL);
            
            //显示低电字符串
            style_infor.style_id = LOW_POWER_TEXT;
            style_infor.type = UI_COM;
            ui_show_textbox(&style_infor, NULL, TEXTBOX_DRAW_NORMAL);
            
            //等待2秒钟
            com_app_sleep(4, NULL);
            
            //广播关机消息
            msg.type = MSG_POWER_OFF;
            broadcast_msg(&msg);
            
            //等待AP退出消息 MSG_APP_QUIT ，再返回 RESULT_APP_QUIT，应用直接退出
            while(1)
            {
                //挂起10ms，多任务调度
                sys_os_time_dly(1); 
                if((get_app_msg(&private_msg) == TRUE) && (private_msg.msg.type == MSG_APP_QUIT))
                {
                    result = RESULT_APP_QUIT;
                    break;
                }
            }
        }
        break;
    
    /* 充电已满消息 */
    case MSG_FULL_CHARGE:
        {
            style_infor_t style_infor;

            //显示低电图标
            style_infor.style_id = FULL_CHARGE_BG;
            style_infor.type = UI_COM;
            ui_show_picbox(&style_infor, NULL);
            
            //显示低电字符串
            style_infor.style_id = FULL_CHARGE_TEXT;
            style_infor.type = UI_COM;
            ui_show_textbox(&style_infor, NULL, TEXTBOX_DRAW_NORMAL);
            
            //等待2秒钟
            com_app_sleep(4, NULL);
            result = RESULT_REDRAW;
        }
        break;

    /* 卡插入消息 */
    case MSG_SD_IN:
        {
            //不做处理，在使用时再加载相关驱动
        }
        break;
        
    /* 卡拔出消息*/
    case MSG_SD_OUT:
        {
            //如果正在使用卡，提示卡拔出
        }
        break;
    
    /* U盘插入*/
    case MSG_UH_IN:
        {
            //不做处理，在使用时再加载相关驱动
        }
        break;
        
    /* U盘拔出*/
    case MSG_UH_OUT:
        {
            //如果正在使用U 盘，提示U 盘拔出
        }
        break;
        
    /* USB B线插入消息 */
    case MSG_USB_STICK:
        {       
            //显示 USB 选项对话框
            result = gui_usbconnect();
            
            g_sys_counter.battery_counter = 100;//强制读取充电状态和电池电量
        }
        break;
        
    /* USB B线拔出消息 */
    case MSG_USB_UNSTICK:
        {
            //只在USB选项对话框中，或者非 udisk 应用充电播放中响应（在USB传输连接下由udisk 应用自己处理）
            //如果在USB选项对话框中
            if(usb_selecting_state == TRUE)
            {
                //返回 RESULT_USB_UNSTICK 即可（ui_usbconnect 将 RESULT_USB_UNSTICK 转为 RESULT_REDRAW 后退出对话框）
                result = RESULT_USB_UNSTICK;
            }
            
            g_sys_counter.battery_counter = 100;//强制读取充电状态和电池电量
        }
        break;
        
    /* 充电器 ADAPTOR 插入消息 */
    case MSG_ADAPTOR_IN:
        {
            //开启充电，但不进入充电UI
            key_chargeset(CHARGE_START, ChargeCurrent250mA, BATFULL_LEVEL1);
            
            g_sys_counter.battery_counter = 100;//强制读取充电状态和电池电量
        }
        break;
        
    /* 充电器 ADAPTOR 拔出消息 */
    case MSG_ADAPTOR_OUT:
        {
            g_sys_counter.battery_counter = 100;//强制读取充电状态和电池电量
        }
        break;
            
    /* RTC定时闹钟消息 */
    case MSG_RTCALARM:
        {
            //由tools 处理
            msg_apps_t msg;
    
            result = com_alarm_msg_dispatch();
            if(result == RESULT_ALARMFM)
            {
                if(g_this_app_info->app_id == APP_ID_RADIO)
                {
                    break;
                }
                msg.content.data[0] = APP_ID_RADIO;
            }
            else if(result == RESULT_FMRECORD)
            {
                if(g_this_app_info->app_id == APP_ID_RECORD)
                {
                    break;
                }
                msg.content.data[0] = APP_ID_RECORD;
            }
            else if(result == RESULT_ALARM)
            {
                msg.content.data[0] = APP_ID_ALARM;
            }
            else
            {
                break;
            }
            msg.type = MSG_CREAT_APP; //给process manager发送创建其它进程消息
            msg.content.data[1] = (uint8) PARAM_FROM_ALARM;
            send_async_msg(APP_ID_MANAGER, &msg);
            
            result = RESULT_APP_QUIT;
        }
        break;
        
    /* 耳机（天线）插入消息 */
    case MSG_EARPHONE_IN:
        {
            deal_antenna_detect(TRUE);
        }
        break;
        
    /* 耳机（天线）拔出消息 */
    case MSG_EARPHONE_OUT:
        {
            deal_antenna_detect(FALSE);
            if(g_this_app_info->app_id == APP_ID_RADIO)//只对 RADIO 进行提示
            {
                gui_dialog_msg (DIALOG_MSG, DIALOG_INFOR_WAIT, S_EARPHONE_OUT);
                result = RESULT_REDRAW;
            }
        }
        break;
        
    /************************** 特殊应用私有消息处理 ******************************/
    case MSG_APP_QUIT:
        {
            result = RESULT_APP_QUIT;
        }
        break;

    default:
        break;
    }
    
    return result;
}

/*! \endcond */
