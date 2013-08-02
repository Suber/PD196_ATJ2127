/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-14 9:21     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_dialog.c
 * \brief    gui 控制流：对话框控件控制流实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了通知或询问用途的对话框控件。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，ui 驱动，applib，等等。
 * \version  1.0
 * \date     2011-9-14
*******************************************************************************/

#include "common_ui.h"

//不同类型按键列表，每个按键对应一个结果消息
const button_list_item_t button_list[] = 
{
    {2, {{S_BUTTON_OK, RESULT_DIALOG_OK}, {S_BUTTON_CANCEL, RESULT_REDRAW}, {0, 0}}},
    {3, {{S_BUTTON_ABORT,RESULT_DIALOG_ABORT},{S_BUTTON_RETRY,RESULT_DIALOG_RETRY},{S_BUTTON_IGNORE,RESULT_REDRAW}}},
    //{3, {{S_BUTTON_YES, RESULT_DIALOG_YES}, {S_BUTTON_NO, RESULT_DIALOG_NO}, {S_BUTTON_CANCEL, RESULT_REDRAW}}},
    {2, {{S_BUTTON_YES, RESULT_DIALOG_YES}, {S_BUTTON_NO, RESULT_DIALOG_NO}, {0, 0}}},
    {2, {{S_BUTTON_YES, RESULT_DIALOG_YES}, {S_BUTTON_NO, RESULT_REDRAW}, {0, 0}}},
    {2, {{S_BUTTON_RETRY, RESULT_DIALOG_RETRY}, {S_BUTTON_CANCEL, RESULT_REDRAW}, {0, 0}}},
    {2, {{0, 0}, {0, 0}, {0, 0}}}
};

//dialog gui app msg deal
app_result_e dialog_msg_deal(dialog_com_data_t *dialog_com, dialog_private_t *dialog_private, bool any_key_flag)
{
    input_gui_msg_t input_msg;
    msg_apps_type_e gui_event;
    private_msg_t private_msg;
    app_result_e result = RESULT_NULL;
    
    //获取到 gui消息
    if(get_gui_msg(&input_msg) == TRUE)
    {
        //按任意按键退出的对话框模式
        if(any_key_flag == TRUE)
        {
            if((INPUT_MSG_KEY == input_msg.type) || (INPUT_MSG_TOUCH == input_msg.type))
            {
                result = RESULT_REDRAW;
                //启动过滤后续按键
                com_filter_key_hold();
            }
        }
        
        //进行 gui消息映射
        if(com_key_mapping(&input_msg, &gui_event, dialog_key_map_list) == TRUE)
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
                break;
                
            case EVENT_DIALOG_CONFIRM:
                //确定选项返回
                result = button_list[(uint8)dialog_com->button_type].buttons[dialog_private->active].result;
                break;
                
            case EVENT_DIALOG_CANCEL:
                //取消返回
                result = RESULT_REDRAW;
                break;
                
            default:
                result = com_message_box(gui_event);
                if(result == RESULT_REDRAW)
                {
                    dialog_com->draw_mode = DIALOG_DRAW_ALL;
                    result = RESULT_NULL;//不退出控件
                }
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
        }
    }
    
    return result;
}

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    对话框控件控制流实现。
 * \param[in]    style_id ui editor工具设计的对话框控件模板
 * \param[in]    dialog_com 指向对话框控件初始化结构体变量，是上层对对话框的控制命令或参数
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_DIALOG_XXX 选择了有效按键后返回，详细参见 app_result_e 定义
 * \retval           RESULT_REDRAW 无效返回，AP需要重绘UI
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \ingroup      controls
 * \par          exmaple code
 * \code
    dialog_com_data_t dialog_com;
    app_result_e result;
    
    dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
    dialog_com.button_type = BUTTON_YESNO;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    dialog_com.active_default = 0;
    result = gui_dialog(DIALOG_ASK, &dialog_com);
    if(result == DIALOG_RESULT_YES)//选择 Yes 按钮，执行操作
    {
    
    }
    else if(result == RESULT_REDRAW)//取消或选择 No 按钮，返回重绘
    {
    
    }
    else//其他返回结果，需要做特殊处理
    {
    
    }
 * \endcode
 * \note 
*******************************************************************************/
app_result_e gui_dialog (uint16 style_id, dialog_com_data_t *dialog_com)
{
    dialog_private_t dialog_private;
    style_infor_t style_infor;
    app_result_e result = RESULT_NULL;
    bool any_key_flag = FALSE;
    uint8 tag_backup;
    uint8 mode_backup, icon_backup;
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_DIALOG);

    gui_get_headbar_mode(&mode_backup, &icon_backup);
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    
    dialog_private.icon_id = dialog_com->icon_id;
    dialog_private.string_desc = dialog_com->string_desc;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    dialog_com->draw_mode = DIALOG_DRAW_ALL;
    
    //没有按键的对话框处理
    if(dialog_com->dialog_type != DIALOG_BUTTON_INCLUDE)
    {
        //绘制dialogbox
        dialog_private.button_cnt = 0;
        ui_show_dialogbox(&style_infor, &dialog_private, (uint32)dialog_com->draw_mode);
        dialog_com->draw_mode = DIALOG_DRAW_NULL;
#ifdef PC
        UpdateMainWnd();
#endif
        //如果立即信息提示则马上退出
        if(dialog_com->dialog_type == DIALOG_INFOR_IMMEDIATE)
        {
            change_app_timer_tag(tag_backup);
            gui_set_headbar_mode(mode_backup, icon_backup);
            return RESULT_REDRAW;
        }
        //按任意按键退出
        else if(dialog_com->dialog_type == DIALOG_BUTTON_ANYKEY)
        {
            any_key_flag = TRUE;
        }
        //如果是需要等待的信息提示，则先等待2秒钟后退出
        else
        {
            com_app_sleep(4, NULL);
            change_app_timer_tag(tag_backup);
            gui_set_headbar_mode(mode_backup, icon_backup);
            return RESULT_REDRAW;
        }
    }
    //有按键的对话框处理，先初始化
    else
    {
        uint8 i;
        
        dialog_private.button_cnt = button_list[(uint8)dialog_com->button_type].button_cnt;
        for(i = 0; i < dialog_private.button_cnt; i++)
        {
            dialog_private.buttons[i] = button_list[(uint8)dialog_com->button_type].buttons[i].str_id;
        }
        
        dialog_private.old = dialog_private.active = dialog_com->active_default;
    }
  
    //进入对话框消息循环
    while (1)
    {
        //刷新dialogbox
        if(dialog_com->draw_mode != DIALOG_DRAW_NULL)
        {   
            ui_show_dialogbox(&style_infor, &dialog_private, (uint32)dialog_com->draw_mode);
            dialog_com->draw_mode = DIALOG_DRAW_NULL;
        }
        
        result = dialog_msg_deal(dialog_com, &dialog_private, any_key_flag);
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

/******************************************************************************/
/*!                    
 * \par  Description:
 *    消息提示型对话框控制流实现。
 * \param[in]    style_id ui editor工具设计的对话框控件模板
 * \param[in]    type 对话框类型，只能是 DIALOG_BUTTON_ANYKEY，DIALOG_INFOR_WAIT，
 *                     DIALOG_INFOR_IMMEDIATE 中的一种
 * \param[in]    str_id 多国语言字符串ID
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_DIALOG_XXX 选择了有效按键后返回，详细参见 app_result_e 定义
 * \retval           RESULT_REDRAW 无效返回，AP需要重绘UI
 * \retval           other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \ingroup      controls
 * \par          exmaple code
 * \code
    app_result_e result;
    
    result = gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING);//显示“请稍后…”
    if(result == RESULT_REDRAW)//显示正常结束返回，重绘UI
    {
    
    }
    else//其他返回结果，需要做特殊处理
    {
    
    }
 * \endcode
 * \note 
 * \li  对话框类型只能是 DIALOG_BUTTON_ANYKEY，DIALOG_INFOR_WAIT，DIALOG_INFOR_IMMEDIATE 
 *      中的一种，否则强制为 DIALOG_INFOR_WAIT。
*******************************************************************************/
app_result_e gui_dialog_msg (uint16 style_id, dialog_type_e type, uint16 str_id)
{
    dialog_com_data_t dialog_param;
    string_desc_t str_desc;
    
    str_desc.data.id = str_id;
    str_desc.language = UNICODEID;
    
    //对话框类型 提示
    if(type == DIALOG_BUTTON_INCLUDE)
    {
        type = DIALOG_INFOR_WAIT;
    }
    dialog_param.dialog_type = type;
    dialog_param.icon_id = -1;
    //字符串
    dialog_param.string_desc = &str_desc;
    
    //按键类型
    dialog_param.button_type = BUTTON_NO_BUTTON;
    
    //调用 gui_dialog 提示消息
    return gui_dialog(style_id, &dialog_param);
}

/*! \endcond */
