/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-menulist for usb menu
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.  
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-16 9:07     1.0             build this file 
*******************************************************************************/
/*! 
 * \file     _ui_usbconnect.c
 * \brief    commonUI 专用于usb选项对话框控件控制流实现 
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种专用于usb选项对话框GUI。 
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0 
 * \date  2011-9-16 
*******************************************************************************/

#include "common_ui.h"

#define USB_OPTION_COUNT    2
const uint16 usb_buttons[USB_OPTION_COUNT] = {S_LINK_MODE_TRANS, S_LINK_MODE_PLAY}; 
const uint16 usb_result[USB_OPTION_COUNT] = {RESULT_USB_TRANS, RESULT_CHARGE_PLAY}; 

const uint16 style_usb_time[5] = {USB_TIME_ONE, USB_TIME_TWO, USB_TIME_THREE, USB_TIME_FOUR, USB_TIME_FIVE};

void timer_usb_handle(void) 
{
    style_infor_t style_infor;
    
    style_infor.style_id = style_usb_time[g_usb_auto_select];
    style_infor.type = UI_COM;
    //显示计时界面
    ui_show_picbox(&style_infor, NULL); 
    g_usb_auto_select++;
}

void clear_usb_time(void)
{
    style_infor_t style_infor;
    
    style_infor.style_id = USB_TIME_CLEAR;
    style_infor.type = UI_COM;
    //显示计时界面
    ui_show_picbox(&style_infor, NULL); 
}

//usbconnect gui app msg deal
app_result_e usbconnect_msg_deal (dialog_com_data_t *dialog_com, dialog_private_t *dialog_private)
{
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    app_result_e result = RESULT_NULL;
    
    //获取到 gui消息
    if(get_gui_msg(&input_msg) == TRUE) 
    {               
        //进行 gui消息映射
        if(com_key_mapping(&input_msg, &gui_event, usbconnect_key_map_list) == TRUE)
        {
            switch(gui_event)
            {
            case EVENT_DIALOG_NEXT_BUTTON:
                //选择下一个按钮选项
                dialog_private->old = dialog_private->active; 
                if(dialog_private->active < (dialog_private->button_cnt - 1)) 
                {
                    dialog_private->active++;
                }
                else
                {
                    dialog_private->active = 0;
                }
                dialog_com->draw_mode = DIALOG_DRAW_BUTTON; 
                
                //修改软定时器时间
                clear_usb_time();
                g_usb_auto_select = 0;
                restart_app_timer(g_usb_timer_id);
                break;
                
            case EVENT_DIALOG_PREV_BUTTON:
                //选择上一个按钮选项
                dialog_private->old = dialog_private->active; 
                if(dialog_private->active > 0)
                {
                    dialog_private->active--;
                }
                else
                {
                    dialog_private->active = dialog_private->button_cnt - 1;
                }
                dialog_com->draw_mode = DIALOG_DRAW_BUTTON; 
                
                //修改软定时器时间
                clear_usb_time();
                g_usb_auto_select = 0;
                restart_app_timer(g_usb_timer_id);
                break;
                
            case EVENT_DIALOG_CONFIRM:
                //确定选项返回
                result = usb_result[dialog_private->active]; 
                break;
                
            case EVENT_DIALOG_CANCEL:
                //取消返回，以默认方式连接--充电和播放
                result = RESULT_CHARGE_PLAY; 
                break;
                
            default:
                result = com_message_box(gui_event);
                break;
            }
        }
    }
    //到此，gui消息队列已经处理完毕，获取ap私有消息和系统消息（会先转换为私有消息再返回）并处理 
    else
    {
        if(get_app_msg(&private_msg) == TRUE)
        {
            result = g_this_app_msg_dispatch(&private_msg); 
            if(result == RESULT_REDRAW)
            {
                dialog_com->draw_mode = DIALOG_DRAW_ALL;
                result = RESULT_NULL;//不退出控件
            }
            else if(result != RESULT_NULL)
            {
                if(result == RESULT_USB_UNSTICK)
                {
                    result = RESULT_REDRAW; 
                }
            }
            else
            {
                //do nothing for QAC
            }
        }
    }
    
    return result;
}

/******************************************************************************/
/*!                     
 * \par  Description:
 *    对话框控件控制流主函数，是对话框控件的外部接口
 * \param[in]    style_id ui editor工具设计的对话框控件模板 
 * \param[in]    dialog_com 指向对话框控件初始化结构体变量，是上层对对话框的控制命令或参数
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_USB_TRANS 选择USB数据传输模式
 * \retval           RESULT_USB_PLAY 选择USB充电播放模式或者取消返回默认连接方式
 * \retval           RESULT_REDRAW 无效返回，AP需要重绘UI
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \note 
*******************************************************************************/
app_result_e ui_dialog_usbconnect (uint16 style_id, dialog_com_data_t *dialog_com)
{
    dialog_private_t dialog_private;
    style_infor_t style_infor;
    app_result_e result;
    uint8 i;
    
    //bank data 初始化
    g_usb_auto_select = 0;
    
    usb_selecting_state = TRUE; 
    dialog_private.icon_id = dialog_com->icon_id;
    dialog_private.string_desc = dialog_com->string_desc;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    dialog_com->draw_mode = DIALOG_DRAW_ALL;
        
    //替换按钮字符串
    dialog_private.button_cnt = USB_OPTION_COUNT;
    for(i = 0; i < dialog_private.button_cnt; i++)
    {
        dialog_private.buttons[i] = usb_buttons[i]; 
    }
    dialog_private.old = dialog_private.active = dialog_com->active_default;
    
    //进入对话框消息循环
    while (1)
    {
        //刷新dialogbox 
        if(dialog_com->draw_mode != DIALOG_DRAW_NULL)
        {
            ui_show_dialogbox(&style_infor, &dialog_private, (uint32)dialog_com->draw_mode);            
            //初始时绘制 USB时间 背景图片
            if(dialog_com->draw_mode == DIALOG_DRAW_ALL)
            {
                clear_usb_time();
            }
            
            dialog_com->draw_mode = DIALOG_DRAW_NULL;
        }
        
        //已经5秒，选择当前激活项
        if(g_usb_auto_select == 5)
        {
            //确定选项返回
            result = usb_result[dialog_private.active]; 
            break;  
        }
        
        result = usbconnect_msg_deal (dialog_com, &dialog_private);
        if(result != RESULT_NULL)
        {
            break;
        }

        //挂起10ms，多任务调度
        sys_os_time_dly(1); 
    }
    
    usb_selecting_state = FALSE;
    return result;
}

/******************************************************************************/
/*!                     
 * \par  Description:
 *    usb选项对话框 
 * \param[in]    none
 * \param[out]   none    
 * \return       app_result_e（同 ui_dialog_usbconnect 一致）
 * \retval           RESULT_APP_QUIT 选择USB数据传输模式，退出当前前台AP
 * \retval           RESULT_USB_TRANS 在config ap中，选择USB数据传输模式，只适用于 config ap
 * \retval           RESULT_REDRAW 选择USB充电播放模式 或 无效返回，AP需要重绘UI
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \note 
*******************************************************************************/
app_result_e gui_usbconnect(void)
{
    dialog_com_data_t dialog_com;
    string_desc_t string_desc;
    private_msg_t private_msg;
    app_result_e result;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup; 
    gui_direct_e direct_bk; 
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_USBCN);
    
    direct_bk = com_get_gui_direction();
    com_set_gui_direction(GUI_DIRECT_NORMAL);
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    
    //初始化控制变量，并创建软定时器
    g_usb_auto_select = 0;
    g_usb_timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, timer_usb_handle); 
    
    //显示usb选项对话框，选择连接模式
    string_desc.data.id = S_CHOOSE_LINK_MODE;
    string_desc.language = UNICODEID;
    dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE; 
    dialog_com.button_type = BUTTON_YESNO;
    dialog_com.icon_id = V_U16_INVALID; 
    dialog_com.string_desc = &string_desc;
    dialog_com.active_default = 0;//默认选择 “充电与传输” 
    result = ui_dialog_usbconnect (DIALOG_USB, &dialog_com);
    if(result == RESULT_USB_TRANS)//选择USB数据传输模式 
    {
        if(g_this_app_info->app_id != APP_ID_CONFIG)//不在开机应用中
        {
            //usb选择数据传输模式，通知 manager ap 进入Udisk 数据传输
            msg_apps_t msg; 
    
            //给 manager AP 发送 MSG_USB_TRANS 异步消息，由 manager ap控制进入 Udisk ap 
            msg.type = MSG_USB_TRANS;
            send_async_msg(APP_ID_MANAGER, &msg);
            
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
    }
    else if(result == RESULT_CHARGE_PLAY)//选择USB充电播放模式
    {
        //usb选择充电播放模式，开启充电，返回原先界面
        key_chargeset(CHARGE_START, ChargeCurrent250mA, BATFULL_LEVEL1);
        result = RESULT_REDRAW; 
    }
    else
    {
        //do nothing for QAC
    }
    
    //销毁软定时器
    kill_app_timer(g_usb_timer_id); 
    
    com_set_gui_direction(direct_bk);
    change_app_timer_tag(tag_backup);
    gui_set_headbar_mode(mode_backup, icon_backup); 
    
    return result;
}
