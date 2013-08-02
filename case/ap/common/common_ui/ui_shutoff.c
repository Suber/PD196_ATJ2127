/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-shutoff
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 10:55     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_shutoff.c
 * \brief    commonUI 关机确定控件实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种关机确定GUI。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的picbox完成控件显示流任务。
 * \version 1.0
 * \date  2011-9-19
*******************************************************************************/

#include "common_ui.h"

void show_shutoff_time(uint8 time)
{
    style_infor_t style_infor;
    picbox_private_t picbox_data;
    
    picbox_data.pic_id = V_U16_INVALID;
    picbox_data.frame_id = time;

    style_infor.style_id = SHUTOFF_TIME;
    style_infor.type = UI_COM;
    
    //显示计时界面
    ui_show_picbox(&style_infor, &picbox_data);
}

void timer_shutoff_handle(void)
{
    show_shutoff_time(g_so_time);
    g_so_time++;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    关机确定主函数，是关机确定的外部接口
 * \param[in]    none
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_NULL 长按3 秒，广播 POWER_OFF 消息后返回
 * \retval           RESULT_REDRAW PLAY 按键弹起返回，返回AP 重绘UI
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \note 
*******************************************************************************/
app_result_e gui_shut_off (void)
{
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    style_infor_t style_infor;
    int8 timer_id_shutoff;
    app_result_e result = RESULT_NULL;
    uint8 frame_cnt;
    bool ret;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup;
    gui_direct_e direct_bk;
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_SHUT);
    
    direct_bk = com_get_gui_direction();
    com_set_gui_direction(GUI_DIRECT_NORMAL);
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    
    //刷背景
    style_infor.style_id = SHUTOFF_BG;
    style_infor.type = UI_COM;
    ui_show_picbox(&style_infor, NULL);
    
    //获取计时多帧图片帧数
    style_infor.style_id = SHUTOFF_TIME;
    style_infor.type = UI_COM;
    ui_get_picbox_attrb(&style_infor, &frame_cnt, 1);

    //刷1s图片
    g_so_time = 0;
    timer_shutoff_handle();
    
    //创建软定时器
    timer_id_shutoff = set_app_timer(APP_TIMER_ATTRB_UI, 1000, timer_shutoff_handle);
    
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    
    //进入对话框消息循环
    while (1)
    {   
        //关机确定时间已到
        if(g_so_time >= frame_cnt)
        {
            msg_apps_t msg;

            //广播关机消息
            msg.type = MSG_POWER_OFF;
            broadcast_msg(&msg);
            
            //屏蔽后续 play 按键，以实现进入充电界面后按按键退出充电
            com_filter_key_hold();
            
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
            break;
        }
        
        //获取gui消息
        ret = get_gui_msg(&input_msg);
        if(ret == TRUE)//有gui消息
        {
            //进行按键映射
            if(com_key_mapping(&input_msg, &gui_event, shutoff_key_map_list) == TRUE)
            {
                //GUI按键消息处理
                if(gui_event == EVENT_SHUTOFF_QUIT)
                {
                    //关机键弹起，重绘原先UI
                    result = RESULT_REDRAW;
                    break;
                }
            }
        }
        else
        {
            //到此，gui消息队列已经处理完毕
            //获取ap私有消息和系统消息（会先转换为私有消息再返回），并处理
            ret = get_app_msg(&private_msg);
            if(ret == TRUE)
            {
                result = g_this_app_msg_dispatch(&private_msg);
                if(result != RESULT_NULL)
                {
                    break;
                }
            }
        }
        
        //挂起10ms，多任务调度
        sys_os_time_dly(1);
    }
    
    kill_app_timer(timer_id_shutoff);
    com_set_gui_direction(direct_bk);
    change_app_timer_tag(tag_backup);
    gui_set_headbar_mode(mode_backup, icon_backup);
    return result;
}
