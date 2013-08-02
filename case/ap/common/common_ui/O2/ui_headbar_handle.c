/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-5-17 11:13     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_headbar_handle.c
 * \brief    状态栏状态监测实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-5-17
*******************************************************************************/

#include "common_ui.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    状态栏状态监测，如果有更新，调用 gui_headbar 进行更新。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      controls
 * \note
 * \li  该接口可以看出是 gui_headbar 的简化接口，只实现其检测部分功能。
*******************************************************************************/
void gui_headbar_handle(void)
{
    time_t temp_sys_time;
    uint8 temp_cable_state;
    uint8 temp_card_state;
    uint8 temp_icon_id;
    bool need_update_headbar = FALSE;
    
    do//执行1次循环，为了前面有一处地方满足条件后用break退出循环
    {
        if(this_headbar_style.battery_sty != V_U16_INVALID)
        {
            if((g_app_info_state.charge_state == CHARGE_CHARGING)//正在充电
            || (g_app_info_state.charge_state == CHARGE_FULL)//充电已满
            || (g_app_info_state.bat_value != g_battery_value))//无充电，电池供电，且电量变化
            {
                need_update_headbar = TRUE;
                break;
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
            
            if((temp_icon_id != HEADBAR_ICON_NONE) && (temp_icon_id != g_icon_id))
            {
                need_update_headbar = TRUE;
                break;
            }
        }
    
        if(this_headbar_style.time_sty != V_U16_INVALID)
        {
            //获取当前时间
            sys_get_time(&temp_sys_time);
            
            if(temp_sys_time.minute != g_sys_minute)
            {
                need_update_headbar = TRUE;
                break;
            }
        }
    
        if(this_headbar_style.cable_sty != V_U16_INVALID)
        {
            temp_cable_state = get_cable_state();
            
            if(temp_cable_state != g_app_last_state.last_cable_state)
            {
                need_update_headbar = TRUE;
                break;
            }
        }
    
        if(this_headbar_style.card_sty != V_U16_INVALID)
        {
            temp_card_state = get_card_state();
            
            if(temp_card_state != g_app_last_state.last_card_state)
            {
                need_update_headbar = TRUE;
                break;
            }
        }
    }while(0);
    
    //需要更新 headbar
    if(need_update_headbar == TRUE)
    {
        gui_headbar(HEADBAR_UPDATE_CHANGE);
    }
}

/*! \endcond */
