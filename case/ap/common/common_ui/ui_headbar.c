/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 21:28     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_headbar.c
 * \brief    gui 控制流：状态栏控件控制流实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种（顶部）状态栏控件。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，ui驱动，applib，等等。
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

static void __show_battery(uint8 display);

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    设置状态栏模式等。
 * \param[in]    mode 状态栏模式，见 headbar_mode_e  枚举类型定义。
 * \param[in]    icon_id 应用图标ID，参考 headbar_icon_e 枚举类型定义。
 * \param[out]   none    
 * \return       none
 * \ingroup      controls
 * \note
 * \li  一般情况下，应用只需在初始化时中调用该接口设置模式。
*******************************************************************************/
void gui_set_headbar_mode(headbar_mode_e mode, headbar_icon_e icon_id)
{
    this_headbar_mode = mode;
    this_headbar_icon_id = icon_id;
    
    switch(mode)
    {
    case HEADBAR_MODE_NORMAL://默认模式，显示应用图标和电池图标
        //this_headbar_style.icon_sty = HEADBAR_ICON;
        this_headbar_style.icon_sty = V_U16_INVALID;
        this_headbar_style.battery_sty = HEADBAR_BATTERY;
        this_headbar_style.bg_sty = V_U16_INVALID;
        this_headbar_style.time_sty = V_U16_INVALID;
        this_headbar_style.cable_sty = V_U16_INVALID;
        this_headbar_style.card_sty = HEADBAR_CARD;
        
        g_headbar_update = HEADBAR_UPDATE_INIT;
        restart_app_timer(sys_status_timer_id);
        break;
    
    default:
        stop_app_timer(sys_status_timer_id);
        break;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    获取状态栏模式等。
 * \param[in]    none  
 * \param[out]   mode 返回状态栏模式，见 headbar_mode_e  枚举类型定义
 * \param[out]   icon_id 返回应用图标ID，参考 headbar_icon_e 枚举类型定义 
 * \return       none
 * \ingroup      controls
 * \note 
*******************************************************************************/
void gui_get_headbar_mode(headbar_mode_e *mode, headbar_icon_e *icon_id)
{
    *mode = this_headbar_mode;
    *icon_id = this_headbar_icon_id;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    状态栏更新绘制执行接口。
 * \param[in]    update 状态栏更新模式，见 headbar_update_e 定义
 * \param[out]   none    
 * \return       app_result_e
 * \retval           RESULT_NULL 没任何意义
 * \ingroup      controls
 * \note 
 * \li  该接口由 common 模块自动调用，用户无须干预。
 * \li  如果用户自己需要控制什么时候用什么方式刷新，也可以自己主动调用。
*******************************************************************************/
app_result_e gui_headbar(headbar_update_e update)
{
    picbox_private_t headbar_pic;
    style_infor_t style_infor;
    
    //临时变量
    time_t temp_sys_time;
    uint8 temp_cable_state;
    uint8 temp_card_state;
    uint8 temp_icon_id;
    
    if(this_headbar_style.bg_sty != V_U16_INVALID)
    {
        if((update == HEADBAR_UPDATE_INIT)
        || (update == HEADBAR_UPDATE_ALL))
        {
            //刷新背景图片
            style_infor.style_id = this_headbar_style.bg_sty;
            style_infor.type = UI_COM;
            ui_show_picbox(&style_infor, NULL);
        }
    }
    
    if(this_headbar_style.battery_sty != V_U16_INVALID)
    {
        if(update == HEADBAR_UPDATE_INIT)
        {
            g_app_info_state.charge_state = (uint8)(int)key_chargeget(0);
            g_app_info_state.bat_value = com_get_battery_grade(TRUE);
        
            g_battery_display = 1;
        }
        
        //正在充电
        if(g_app_info_state.charge_state == CHARGE_CHARGING)
        {
            if(g_battery_display > BATTERY_GRADE_MAX)
            {
                #if 1
                //充电时电池值显示范围为：空格电 ~ 满格电
                g_battery_display = 1;
                #else
                //充电时电池值显示范围为：当前电池电量 ~ 满格电
                g_battery_display = g_app_info_state.bat_value;
                #endif
            }
            
            __show_battery(g_battery_display);
            g_battery_value = -1;//等于无效值，一旦切换到电池供电，立即绘制图标
        }
        //充电已满
        else if(g_app_info_state.charge_state == CHARGE_FULL)
        {
            __show_battery(BATTERY_GRADE_MAX);
            g_battery_value = -1;//等于无效值，一旦切换到电池供电，立即绘制图标
        }
        //无充电，电池供电
        else
        {
            if((update == HEADBAR_UPDATE_INIT)
            || (update == HEADBAR_UPDATE_ALL)
            || (g_app_info_state.bat_value != g_battery_value))
            {
                __show_battery(g_app_info_state.bat_value);
                g_battery_value = g_app_info_state.bat_value;
            }
        }
    }
    
    if(this_headbar_style.icon_sty != V_U16_INVALID)
    {
        if(g_app_info_state.keylock_state == TRUE)
        {
            temp_icon_id = HEADBAR_ICON_KEYLOCK;
        }
        else
        {
            temp_icon_id = this_headbar_icon_id;
        }
        
        if((update == HEADBAR_UPDATE_INIT)
        || (update == HEADBAR_UPDATE_ALL)
        || ((temp_icon_id != HEADBAR_ICON_NONE) && (temp_icon_id != g_icon_id)) )
        {
            //刷新图标
            style_infor.style_id = this_headbar_style.icon_sty;
            style_infor.type = UI_COM;
            headbar_pic.pic_id = V_U16_INVALID;
            headbar_pic.frame_id = (uint8)temp_icon_id;
            ui_show_picbox(&style_infor, &headbar_pic);
            
            g_icon_id = temp_icon_id;
        }
    }

    if(this_headbar_style.time_sty != V_U16_INVALID)
    {
        timebox_draw_mode_e draw_mode;
        bool need_draw = FALSE;

        //获取当前时间
        sys_get_time(&temp_sys_time);
        
        if((update == HEADBAR_UPDATE_INIT)
        || (update == HEADBAR_UPDATE_ALL))
        {
            draw_mode = TIMEBOX_DRAW_ALL;//时间初始化
            need_draw = TRUE;
        }
        else if(temp_sys_time.minute != g_sys_minute)
        {
            draw_mode = TIMEBOX_DRAW_TIME;//更新时间
            need_draw = TRUE;
        }
        else
        {
            //do nothing for QAC
        }
        
        //显示系统时间
        if(need_draw == TRUE)
        {
            timebox_private_t headbar_time;
            time_timebox_t *tmp_time;
        
            tmp_time = &(headbar_time.times[0].union_time.time);
            tmp_time->hour = temp_sys_time.hour;
            tmp_time->minute = temp_sys_time.minute;
            tmp_time->second = temp_sys_time.second;
            headbar_time.times[0].time_type = 'T';
            headbar_time.times[0].mode = TIME_DISP_HHMM;
                
            style_infor.style_id = this_headbar_style.time_sty;
            style_infor.type = UI_COM;
            ui_show_timebox(&style_infor, &headbar_time, (uint32)draw_mode);
            g_sys_minute = temp_sys_time.minute;
        }
    }

    if(this_headbar_style.cable_sty != V_U16_INVALID)
    {
        temp_cable_state = get_cable_state();
        
        if((update == HEADBAR_UPDATE_INIT)
        || (update == HEADBAR_UPDATE_ALL)
        || (temp_cable_state != g_app_last_state.last_cable_state))
        {
            //刷新电缆状态图标
            style_infor.style_id = this_headbar_style.cable_sty;
            style_infor.type = UI_COM;
            headbar_pic.pic_id = V_U16_INVALID;
            headbar_pic.frame_id = temp_cable_state;
            ui_show_picbox(&style_infor, &headbar_pic);
            
            g_app_last_state.last_cable_state = temp_cable_state;
        }
    }

    if(this_headbar_style.card_sty != V_U16_INVALID)
    {
        temp_card_state = get_card_state();
        
        if((update == HEADBAR_UPDATE_INIT)
        || (update == HEADBAR_UPDATE_ALL)
        || (temp_card_state != g_app_last_state.last_card_state))
        {
            //刷新卡状态图标
            style_infor.style_id = this_headbar_style.card_sty;
            style_infor.type = UI_COM;
            headbar_pic.pic_id = V_U16_INVALID;
            headbar_pic.frame_id = temp_card_state;
            ui_show_picbox(&style_infor, &headbar_pic);
            
            g_app_last_state.last_card_state = temp_card_state;
        }
    }

    return RESULT_NULL;
}

/*! \endcond */

/*! \cond */

//更新电池图标
static void __show_battery(uint8 display)
{
    picbox_private_t headbar_pic;
    style_infor_t style_infor;
    
    style_infor.style_id = this_headbar_style.battery_sty;
    style_infor.type = UI_COM;
    headbar_pic.pic_id = V_U16_INVALID;
    headbar_pic.frame_id = display;
    ui_show_picbox(&style_infor, &headbar_pic);
}

/*! \endcond */
