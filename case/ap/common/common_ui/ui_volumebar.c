/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-volumebar
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.  
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-15 9:51     1.0             build this file 
*******************************************************************************/
/*! 
 * \file     _ui_volumebar.c
 * \brief    commonUI 音量设置控件控制流实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种音量调节GUI。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的parambox完成控件显示流任务。
 * \version 1.0 
 * \date  2011-9-15 
*******************************************************************************/

#include "common_ui.h"

extern void show_volumebar_ui(style_infor_t *style_infor, parambox_private_t *private_data, \
int16 value_last, uint8 mode, parambox_draw_mode_e draw_mode) __FAR__;

void timeup_func_proc(void) 
{
    g_vol_time_up_flag = TRUE;
}

//volumebar gui app msg deal
app_result_e volumebar_msg_deal(param_com_data_t *param_com, int8 timeup_id, uint8 mode)
{
    parambox_private_t *private_data;
    parambox_one_t *cur_one;
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    app_result_e result = RESULT_NULL;
    const key_map_t *this_key_map_list; 
    
    private_data = param_com->private_data; 
    cur_one = &(private_data->items[private_data->active]);
    
    //获取gui消息
    if(get_gui_msg(&input_msg) == TRUE)//有gui消息
    {
        //有按键，重新计算超时时间
        if(mode == SET_VOLUME_VALUE)
        {
            restart_app_timer(timeup_id);
        }
        
        if(mode == SET_VOLUME_VALUE)
        {
            this_key_map_list = volume_key_map_list;
        }
        else
        {
            this_key_map_list = limit_key_map_list; 
        }
        
        //进行 gui消息映射
        if(com_key_mapping(&input_msg, &gui_event, this_key_map_list) == TRUE)
        {
            switch(gui_event)
            {
            case EVENT_VOLUME_INC:
                //参数值递增
                if(mode == SET_VOLUME_LIMIT)
                {
                    if(cur_one->value < cur_one->max)
                    {
                        cur_one->value += cur_one->step;
                        param_com->draw_mode = PARAMBOX_DRAW_VALUE;
                        
                        com_set_sound_volume((uint8)(cur_one->value));   
                    }
                    
                    break;//for QAC，至此case分支结束
                }
                
                //至此，说明mode不等于SET_VOLUME_LIMIT ，for QAC ，减少1层控制嵌套
                if(cur_one->value < sys_comval->volume_limit)
                {
                    cur_one->value += cur_one->step;
                    param_com->draw_mode = PARAMBOX_DRAW_VALUE;
                    
                    if(mode == SET_VOLUME_VALUE)
                    {
                        com_set_sound_volume((uint8)(cur_one->value));
                    }
                }
                break;
                
            case EVENT_VOLUME_DEC:
                //参数值递减
                if(cur_one->value > cur_one->min)
                {
                    cur_one->value -= cur_one->step;
                    param_com->draw_mode = PARAMBOX_DRAW_VALUE;
                    
                    if((mode == SET_VOLUME_LIMIT) || (mode == SET_VOLUME_VALUE))
                    {
                        com_set_sound_volume((uint8)(cur_one->value));   
                    }
                }
                break;
                
            case EVENT_VOLUME_CONFIRM:
                //确定，退出音量设置
                result = RESULT_CONFIRM;
                break;
            
            case EVENT_VOLUME_CANCEL:
                //取消，退出音量设置
                result = RESULT_REDRAW; 
                break;
                
            default:
                result = com_message_box(gui_event);
                if(result == RESULT_REDRAW)
                {
                    param_com->draw_mode = PARAMBOX_DRAW_ALL;
                    result = RESULT_NULL;//不退出控件
                }
                break;
            }
        }
    }
    else
    {
        //到此，gui消息队列已经处理完毕 
        //处理ap私有消息和系统消息
        if(get_app_msg(&private_msg) == TRUE)
        {
            result = g_this_app_msg_dispatch(&private_msg); 
            if(result == RESULT_REDRAW)
            {
                param_com->draw_mode = PARAMBOX_DRAW_ALL;
                result = RESULT_NULL;//不退出控件
            }
        }
    }
    
    return result;
}

/******************************************************************************/
/*!                     
 * \par  Description:
 *    参数设置控件控制流主函数，是参数设置控件的外部接口
 * \param[in]    style_id ui editor工具设计的参数设置控件模板
 * \param[in]    param_com 指向参数设置控件初始化结构体变量，是上层对参数设置控件的控制命令或参数
 * \param[in]    mode 设置模式， SET_VOLUME_VALUE 表示设置实际音量， SET_VOLUME_LIMIT 
 *               表示设置音量限制值，SET_VOLUME_ALARM 专用于 alarm 应用设置 alarm 音量
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_CONFIRM 确定音量设置返回
 * \retval           RESULT_REDRAW 取消音量设置退出，对于音量限制将不保存结果（调节音量时 4 秒钟没
 *                                  操作会自动返回 RESULT_REDRAW）
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \note 
*******************************************************************************/
app_result_e ui_set_parameter_volume (uint16 style_id, param_com_data_t *param_com, uint8 mode) 
{
    parambox_private_t *private_data;
    style_infor_t style_infor;
    app_result_e result;
    int8 timeup_id;//设置音量超时返回定时器 
    
    private_data = param_com->private_data; 
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    //进入后，获取默认参数
    private_data->old = private_data->active = 0;
    volume_value_last = private_data->items[private_data->active].value;//递增递减前音量值为 last 值 
    
    if(mode == SET_VOLUME_VALUE)
    {
        //过滤掉当前按键后续动作，保证只响应一次音量调节
        com_filter_key_hold();
        
        //设置 4 秒定时器
        g_vol_time_up_flag = FALSE; 
        timeup_id = set_app_timer(APP_TIMER_ATTRB_UI, 4000, timeup_func_proc);
    }
    
    //初始化
    param_com->draw_mode = PARAMBOX_DRAW_ALL;
    volume_enter_forbidden = 0xff;
    
    //进入设置参数消息循环
    while (1)
    {
        //设置音量超时返回 
        if((mode == SET_VOLUME_VALUE) && (g_vol_time_up_flag == TRUE))
        {
            result = RESULT_REDRAW; 
            break;
        }
        
        if(param_com->draw_mode != PARAMBOX_DRAW_NULL) 
        {
            //显示音量条UI
            show_volumebar_ui(&style_infor, private_data, volume_value_last, mode, param_com->draw_mode);
            //更新last值
            volume_value_last = private_data->items[private_data->active].value; 
            param_com->draw_mode = PARAMBOX_DRAW_NULL; 
        }
        
        result = volumebar_msg_deal(param_com, timeup_id, mode);
        if(result != RESULT_NULL)
        {
            break;
        }
        
        //挂起10ms，多任务调度
        sys_os_time_dly(1); 
    }
    
    if(mode == SET_VOLUME_VALUE)
    {
        kill_app_timer(timeup_id);
    }
    return result;
}

/******************************************************************************/
/*!                     
 * \par  Description:
 *    音量设置控件，设置音量值或音量限制值。
 * \param[in]    volume 当前音量值指针
 * \param[in]    limit 音量限制值指针
 * \param[in]    mode 设置模式， SET_VOLUME_VALUE 表示设置实际音量， SET_VOLUME_LIMIT 
 *               表示设置音量限制值，SET_VOLUME_ALARM 专用于 alarm 应用设置 alarm 音量
 * \param[out]   volume 返回设置好的音量值   
 * \return       app_result_e（同 ui_set_parameter_volume 一致）
 * \retval           RESULT_CONFIRM 确定音量（限制）设置返回
 * \retval           RESULT_REDRAW 取消音量设置退出，对于音量限制将不保存结果
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \note 
*******************************************************************************/
app_result_e gui_volumebar (uint8 *volume, uint8 *limit, uint8 mode)
{
    param_com_data_t param_com; 
    parambox_private_t param_private;
    parambox_one_t param_volume;
    app_result_e result;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup; 
    gui_direct_e direct_bk; 
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_VOLUME); 
    
    direct_bk = com_get_gui_direction();
    com_set_gui_direction(GUI_DIRECT_NORMAL);
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    if(mode == SET_VOLUME_VALUE)
    {
        gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    }
    else//音量限制设置，同普通参数设置一样
    {
        gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);
    }
    
    param_volume.unit_id = V_U16_INVALID;
    param_volume.min = 0;
    param_volume.max = VOLUME_VALUE_MAX;
    param_volume.step = 1;
    param_volume.value = (int16)(*volume);
    param_volume.value_str = NULL;
    param_volume.cycle = FALSE; 
    param_volume.max_number = 2;
    param_volume.adjust_func = NULL;
    
    param_private.back_id = -1; 
    param_private.icon_id = V_U16_INVALID;
    param_private.title_id = V_U16_INVALID; 
    param_private.param_cnt = 1;
    param_private.active = 0;
    param_private.old = 0;
    param_private.items = &param_volume;
    param_private.sign_flag = 0;
    
    param_com.private_data = &param_private;

    result = ui_set_parameter_volume(SLIDER_VOLUMEBAR, &param_com, mode);
    if(mode == SET_VOLUME_VALUE)
    {
        *volume = (uint8)param_volume.value;
    }
    else if(mode == SET_VOLUME_ALARM)
    {
        if(result == RESULT_CONFIRM)
        {
            *volume = (uint8)param_volume.value;
        }
    }
    else//音量限制
    {
        if(result == RESULT_CONFIRM)
        {
            *limit = (uint8)param_volume.value; 
            if(*volume > (uint8)param_volume.value) 
            {
                *volume = (uint8)param_volume.value;
            }
            else
            {
                com_set_sound_volume(*volume);
            }
        }
        else
        {
            com_set_sound_volume(*volume);
        }
    }
    
    com_set_gui_direction(direct_bk);
    change_app_timer_tag(tag_backup);
    gui_set_headbar_mode(mode_backup, icon_backup); 
    return result;
}
