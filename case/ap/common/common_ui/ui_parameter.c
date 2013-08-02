/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.  
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-14 20:29     1.0             build this file  
*******************************************************************************/
/*! 
 * \file     _ui_parameter.c
 * \brief    gui 控制流：参数设置控件控制流实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种（多）参数设置控件。 
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，ui 驱动，applib，等等。
 * \version  1.0
 * \date     2011-9-14
*******************************************************************************/

#include "common_ui.h"
#include "display.h"

/*! \cond COMMON_API */ 

//parambox value inc
void parambox_value_inc(param_com_data_t *param_com)
{
    parambox_private_t *private_data;
    parambox_one_t *cur_one;
    
    private_data = param_com->private_data; 
    cur_one = &(private_data->items[private_data->active]);
    
    if(cur_one->value < cur_one->max)
    {
        cur_one->value += cur_one->step;
        //即时回调函数
        if(cur_one->callback != NULL)   
        {
            cur_one->callback(cur_one->value);
        }
        param_com->draw_mode = PARAMBOX_DRAW_VALUE;
    }
    else if(cur_one->cycle == TRUE)//允许循环调节参数
    {
        cur_one->value = cur_one->min;
        //即时回调函数
        if(cur_one->callback != NULL)   
        {
            cur_one->callback(cur_one->value);
        }
        param_com->draw_mode = PARAMBOX_DRAW_VALUE;
    }
    else
    {
        //do nothing for QAC
    }
    
    if(param_com->draw_mode == PARAMBOX_DRAW_VALUE)
    {
        //参数值检查和纠正
        if(cur_one->adjust_func != NULL)
        {
            adjust_result_e adjust_result = ADJUST_RESULT_NULL;
            
            adjust_result = cur_one->adjust_func(private_data->items, private_data->active, TRUE);
            if(adjust_result == ADJUST_RESULT_ALL)
            {
                param_com->draw_mode = PARAMBOX_DRAW_ALL;
            }
        }
    }
}

//parambox value dec
void parambox_value_dec(param_com_data_t *param_com)
{
    parambox_private_t *private_data;
    parambox_one_t *cur_one;
    
    private_data = param_com->private_data; 
    cur_one = &(private_data->items[private_data->active]);
    
    if(cur_one->value > cur_one->min)
    {
        cur_one->value -= cur_one->step;
        //即时回调函数
        if(cur_one->callback != NULL)   
        {
            cur_one->callback(cur_one->value);
        }
        param_com->draw_mode = PARAMBOX_DRAW_VALUE;
    }
    else if(cur_one->cycle == TRUE)//允许循环调节参数
    {
        cur_one->value = cur_one->max;
        //即时回调函数
        if(cur_one->callback != NULL)   
        {
            cur_one->callback(cur_one->value);
        }
        param_com->draw_mode = PARAMBOX_DRAW_VALUE;
    }
    else
    {
        //do nothing for QAC
    }
    
    if(param_com->draw_mode == PARAMBOX_DRAW_VALUE)
    {
        //参数值检查和纠正
        if(cur_one->adjust_func != NULL)
        {
            adjust_result_e adjust_result = ADJUST_RESULT_NULL;
            
            adjust_result = cur_one->adjust_func(private_data->items, private_data->active, FALSE); 
            if(adjust_result == ADJUST_RESULT_ALL)
            {
                param_com->draw_mode = PARAMBOX_DRAW_ALL;
            }
        }
    }
}

//parambox gui app msg deal
app_result_e parambox_msg_deal(param_com_data_t *param_com)
{
    parambox_private_t *private_data;
    parambox_one_t *cur_one;
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    app_result_e result = RESULT_NULL;
    
    private_data = param_com->private_data; 
    cur_one = &(private_data->items[private_data->active]); 
    
    //获取gui消息
    if(get_gui_msg(&input_msg) == TRUE)//有gui消息
    {
        //进行按键映射
        if(com_key_mapping(&input_msg, &gui_event, parameter_key_map_list) == TRUE) 
        {
            //GUI按键消息处理
            switch(gui_event)
            {
            case EVENT_PARAMETER_VALUE_INC: 
                //参数值递增
                parambox_value_inc(param_com);
                break;
            
            case EVENT_PARAMETER_VALUE_DEC: 
                //参数值递减
                parambox_value_dec(param_com);
                break;
                
                //切换到下一个参数
            case EVENT_PARAMETER_SELECT_NEXT:
                if(private_data->param_cnt > 1) 
                {
                    private_data->old = private_data->active;
                    if(private_data->active == (private_data->param_cnt - 1))
                    {
                        private_data->active = 0;
                    }
                    else
                    {
                        private_data->active++; 
                    }
                    
                    param_com->draw_mode = PARAMBOX_DRAW_PARAM;
                }
                break;
                
                //切换到上一个参数
            case EVENT_PARAMETER_SELECT_PREV:
                if(private_data->param_cnt > 1) 
                {
                    private_data->old = private_data->active;
                    if(private_data->active == 0)
                    {
                        private_data->active = private_data->param_cnt - 1; 
                    }
                    else
                    {
                        private_data->active--; 
                    }
                     
                    param_com->draw_mode = PARAMBOX_DRAW_PARAM;
                }
                break;
                
            case EVENT_PARAMETER_CONFIRM:
                //确定选项，返回
                result = RESULT_CONFIRM;
                break;
                 
            case EVENT_PARAMETER_CANCEL:
                //取消退出
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
 *    参数设置控件控制流实现。
 * \param[in]    style_id ui editor工具设计的参数设置控件模板
 * \param[in]    param_com 指向参数设置控件初始化结构体变量，是上层对参数设置控件的控制命令或参数
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_CONFIRM 确定参数设置返回
 * \retval           RESULT_REDRAW 参数设置无效返回AP，重绘UI
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \ingroup      controls
 * \par          exmaple code
 * \code
    param_com_data_t param_com; 
    parambox_private_t private_data;
    parambox_one_t param_item;
    app_result_e result;
    
    param_item.unit_id = V_U16_INVALID; 
    param_item.min = 0; 
    param_item.max = 40;
    param_item.step = 1;
    param_item.value = 24;
    param_item.cycle = FALSE;
    param_item.max_number = 2;
    param_item.value_str = NULL;
    param_item.adjust_func = NULL;
    param_item.callback = volumn_cb;
    
    private_data.back_id = V_U16_INVALID;
    private_data.icon_id = P_SPEAKER;
    private_data.title_id = V_U16_INVALID;
    private_data.param_cnt = 1; 
    private_data.items = &param_item;
    private_data.active = 0;
    private_data.sign_flag = 0; 
    private_data.scale = 0;//非slider模板，无须理会 
    
    param_com.private_data = &private_data; 
    result = gui_set_parameter(PARAMETER_SIMPLE, &param_com);//普通单参数设置
    if(result == RESULT_CONFIRM)//确定设置参数返回
    {
    
    }
    else if(result == RESULT_REDRAW)//取消设置参数返回，重绘UI
    {
    
    }
    else//其他返回结果，需要做特殊处理
    {
    
    }
 * \endcode 
 * \note 
 * \li  通常情况下 adjust_func 不需要用到，但如果参数设置时需要加上特殊条件约束 
 *      参数值，或者需要显示为字符串时，必须用该回调函数进行处理。
*******************************************************************************/
app_result_e gui_set_parameter (uint16 style_id, param_com_data_t *param_com)
{
    parambox_private_t *private_data;
    style_infor_t style_infor;
    app_result_e result = RESULT_NULL;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup; 
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_PARAM);
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    if(style_id == SLIDER_SIMPLE_V) 
    {
        gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    }
    else
    {
        gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);
    }
    
    param_com->draw_mode = PARAMBOX_DRAW_ALL;
    private_data = param_com->private_data; 
    //进入后，获取默认参数
    if(private_data->active >= private_data->param_cnt) 
    {
        private_data->active = 0;
    }
    private_data->old = private_data->active;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    //进入设置参数消息循环
    while (1)
    {
        if(param_com->draw_mode != PARAMBOX_DRAW_NULL) 
        {
            //前4bit表示控件类型，0x07表示 parambox，0x09表示 sliderbar 
            if((style_id & 0xf000) == 0x7000)
            {
                ui_show_parambox(&style_infor, private_data, (uint32)param_com->draw_mode);
            }
            else
            {
                ui_show_sliderbar(&style_infor, private_data, (uint32)param_com->draw_mode);
            }
            param_com->draw_mode = PARAMBOX_DRAW_NULL; 
        }
        
        result = parambox_msg_deal(param_com);
        if(result != RESULT_NULL)
        {
            break;
        }
        
        //挂起10ms，多任务调度
        sys_os_time_dly(1); 
    }
    
    change_app_timer_tag(tag_backup);
    gui_set_headbar_mode(mode_backup, icon_backup); 
    return result;
}

/*! \endcond */ 
