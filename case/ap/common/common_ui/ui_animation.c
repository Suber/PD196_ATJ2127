/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 9:23     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_animation.c
 * \brief    gui 控制流：多帧图片动画实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现动画播放，即多帧图片连续播放功能。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，ui 驱动，applib，等等。
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

static void __anm_handle(void);

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    动画播放，即多帧图片连续播放。
 * \param[in]    style_infor ui editor工具设计的多帧图片控件信息结构体指针
 * \param[in]    anm_com 指向动画显示初始化结构体变量，是上层对动画显示的控制命令或参数
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_NULL 动画显示正常结束后返回AP。
 * \retval           RESULT_REDRAW 应用消息处理返回 RESULT_REDRAW，包含在other2 中。
 * \retval           other1 动画显示终止返回（因用户gui输入而终止，依据key_map_list映射）。
 * \retval           other2 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义。
 * \ingroup      controls
 * \par          exmaple code
 * \code
    animation_com_data_t anm_com;
    style_infor_t style_infor;
    app_result_e result;
    
    style_infor.style_id = MUSIC_IN_ANIMATION;
    style_infor.type = UI_AP;
    anm_com.interval = 250;//250ms间隔
    anm_com.direction = DIRECTION_NORMAL;
    anm_com.forbid = TRUE;//允许终止动画显示
    anm_com.key_map_list  = mainmenu_key_map_list;
    anm_com.callback = callback;
    result = gui_animation(&style_infor, &anm_com);
    if((result == RESULT_NULL) || (result == RESULT_REDRAW))
    {
        //重绘UI
    }
    else//其他返回结果，需要做特殊处理
    {
        
    }
 * \endcode
 * \note
 * \li  动画显示过程中可以通过按键输入终止，这时 forbid 需为 TRUE，并且提供一个
 *      可以终止动画显示的按键映射表。
*******************************************************************************/
app_result_e gui_animation (style_infor_t *style_infor, animation_com_data_t *anm_com)
{
    input_gui_msg_t input_msg;          //输入型消息，即gui消息
    msg_apps_type_e gui_event;          //gui 事件
    private_msg_t private_msg;          //应用消息
    app_result_e result = RESULT_NULL;
    int8 timer_id;
    uint8 frame_cnt;
    uint8 tag_backup;
    
    tag_backup = get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_ANIM);

    g_anm_callback = anm_com->callback;
    g_anm_sty_infor = style_infor;
    //创建软定时器
    timer_id = set_app_timer(APP_TIMER_ATTRB_UI, anm_com->interval, __anm_handle);
    if(timer_id == -1)
    {
        change_app_timer_tag(tag_backup);
        return RESULT_NULL;
    }
    
    //获取picbox帧数
    ui_get_picbox_attrb(g_anm_sty_infor, &frame_cnt, 1);
    
    g_anm_direction = anm_com->direction;
    if(g_anm_direction == DIRECTION_NORMAL)
    {
        g_anm_index = 0;
    }
    else
    {
        g_anm_index = frame_cnt - 1;
    }
    
    //进入动画消息循环
    while (1)
    {   
        //动画已经显示完毕
        if(g_anm_direction == DIRECTION_NORMAL)
        {
            if(g_anm_index >= frame_cnt)
            {
                result = RESULT_NULL;
                break;
            }
        }
        else
        {
            if((int8)g_anm_index < 0)
            {
                result = RESULT_NULL;
                break;
            }
        }
        
        //允许终止动画显示，才需要获取 gui 消息
        if((anm_com->forbid == TRUE) && (get_gui_msg(&input_msg) == TRUE))//有gui消息
        {
            //进行按键映射
            if(com_key_mapping(&input_msg, &gui_event, anm_com->key_map_list) == TRUE)
            {
                result = gui_event;
                break; 
            }
        } 
        else
        {
            //到此，gui消息队列已经处理完毕
            //处理ap私有消息和系统消息
            if(get_app_msg(&private_msg) == TRUE)
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
    
    kill_app_timer(timer_id);
    change_app_timer_tag(tag_backup);
    return result;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    开机logo 和 关机logo，不接收消息。
 * \param[in]    style_infor ui editor工具设计的多帧图片控件信息结构体指针
 * \param[in]    anm_com 指向动画显示初始化结构体变量，是上层对动画显示的控制命令或参数
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_NULL 动画显示正常结束后返回AP。
 * \ingroup      controls
 * \par          exmaple code
 * \code
    animation_com_data_t anm_com;
    style_infor_t style_infor;
    
    style_infor.style_id = POWERON_LOGO;
    style_infor.type = UI_AP;
    anm_com.interval = 20;//20ms间隔
    anm_com.direction = DIRECTION_NORMAL;
    gui_logo(&style_infor, &anm_com);
 * \endcode
 * \note
 * \li  仅用于开机 logo 和 关机 logo，期间不支持任何其他处理。
*******************************************************************************/
app_result_e gui_logo (style_infor_t *style_infor, animation_com_data_t *anm_com)
{
    picbox_private_t animation_pic;
    app_result_e result = RESULT_NULL;
    uint16 time_index;
    uint8 frame_cnt, frame_index;
   
    //获取picbox帧数
    ui_get_picbox_attrb(style_infor, &frame_cnt, 1);
    
    if(anm_com->direction == DIRECTION_NORMAL)
    {
        frame_index = 0;
    }
    else
    {
        frame_index = frame_cnt - 1;
    }
    
    //进入动画消息循环
    while (1)
    {   
        //动画已经显示完毕
        if(anm_com->direction == DIRECTION_NORMAL)
        {
            if(frame_index >= frame_cnt)
            {
                result = RESULT_NULL;
                break;
            }
        }
        else
        {
            if((int8)frame_index < 0)
            {
                result = RESULT_NULL;
                break;
            }
        }
        
        //显示动画帧
        animation_pic.pic_id = V_U16_INVALID;
        animation_pic.frame_id = frame_index;
        
        ui_show_picbox(style_infor, &animation_pic);
        if(g_anm_direction == DIRECTION_NORMAL)
        {
            frame_index++;
        }
        else
        {
            frame_index--;//0再减为0xff，(int8)0xff < 0
        }
        
        //挂起，等待 interval 时间
        for(time_index = 0; time_index < (anm_com->interval / 10); time_index++)
        {
            sys_os_time_dly(1);
        }
    }
    
    return result;
}

/*! \endcond */

/*! \cond COMMON_API */

static void __anm_handle(void)
{
    picbox_private_t animation_pic;
    
    //显示计时界面
    animation_pic.pic_id = V_U16_INVALID;
    animation_pic.frame_id = g_anm_index;
    
    ui_show_picbox(g_anm_sty_infor, &animation_pic);
    if(g_anm_direction == DIRECTION_NORMAL)
    {
        g_anm_index++;
    }
    else
    {
        g_anm_index--;//0再减为0xff，(int8)0xff < 0
    }
    
    //每帧动画中执行回调函数
    if(g_anm_callback != NULL)
    {
        g_anm_callback();
    }
}

/*! \endcond */
