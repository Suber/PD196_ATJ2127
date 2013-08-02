/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_time_date_setting.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_setting.h"
#include "menu_callback_time_setting.h"

const uint8 am_const[3] =
{ 'A', 'M', 0 };
const uint8 pm_const[3] =
{ 'P', 'M', 0 };

//一级菜单项-日期和时间
//日期和时间|-时间设置|-设置时间格式|-12小时制
//          |         |             |-24小时制
//          |         |-设置时间
//          |-日期设置|-设置日期格式|-MM-DD-YYYY
//                    |             |-DD-MM-YYYY
//                    |             |-YYYY-MM-DD
//                    |-设置日期
////////////////////
/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得当前时间格式选择
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回时间格式选项string ID
 * \retval       ：进入下一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_time_format_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_time;
    menu_title_time = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.time_format == VALUE_12HOURS_FORMAT)
    {
        active_id = S_TIME_FORMAT_12H;
    }
    else
    {
        active_id = S_TIME_FORMAT_24H;
    }
    menu_title_time->menu_entry = -1; //不更新入口
    menu_title_time->str_id = active_id; //指定active项
    menu_title_time->ret_str = NULL;
    menu_title_time->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于时间格式设置12小时制
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_time_format_12h_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.time_format = VALUE_12HOURS_FORMAT;
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于时间格式设置24小时制
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_time_format_24h_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.time_format = VALUE_24HOURS_FORMAT;
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于调整am/pm 显示字符串
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
adjust_result_e ampm_adjust_func(parambox_one_t *params, uint8 active, bool inc)
{

    uint8 *am;
    uint8 *pm;
    libc_memcpy(string_buffer, am_const, 3);
    libc_memcpy(string_buffer + 6, pm_const, 3);
    am = string_buffer;
    pm = string_buffer + 6;

    if (params[POSITION_AMPM].value == 0)
    {
        params[POSITION_AMPM].value_str = am;
    }
    else
    {
        params[POSITION_AMPM].value_str = pm;
    }

    return ADJUST_RESULT_NULL;
}
/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置时间
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_time_callback(void *menu_title_void)
{
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item[3];
    uint8 amflag;
    app_result_e result;
    alarm_vars_t  g_alarm_vars; //ALARM VM变量
    uint8 *am;
    uint8 *pm;
    libc_memcpy(string_buffer, am_const, 3);
    libc_memcpy(string_buffer + 6, pm_const, 3);
    am = string_buffer;
    pm = string_buffer + 6;

    sys_get_time(&time);
    if (time.hour < 12)
    {
        amflag = 1;
    }
    else
    {
        amflag = 0;
    }
    if (g_setting_vars.g_comval.time_format == VALUE_12HOURS_FORMAT)
    {
        s_param_item[0].min = VALUE_TIME_HH_12H_MIN;
    }
    else
    {
        s_param_item[0].min = VALUE_TIME_HH_24H_MIN;
    }
    if (g_setting_vars.g_comval.time_format == VALUE_12HOURS_FORMAT)
    {
        s_param_item[0].max = VALUE_TIME_HH_12H_MAX;
    }
    else
    {
        s_param_item[0].max = VALUE_TIME_HH_24H_MAX;
    }
    s_param_item[0].step = VALUE_TIME_HH_STEP;
    if (g_setting_vars.g_comval.time_format == VALUE_12HOURS_FORMAT)
    {
        time.hour = time.hour % 12;
        if (time.hour == 0)
        {
            time.hour = 12;
        }
    }
    else
    {
        time.hour = time.hour % 24;
    }
    s_param_item[0].value = (int16) time.hour;
    s_param_item[0].cycle = TRUE;
    s_param_item[0].adjust_func = NULL;
    s_param_item[0].callback = NULL;
    s_param_item[0].unit_id = S_TIME_HH;
    s_param_item[0].max_number = 2;
    s_param_item[0].value_str = NULL;

    s_param_item[1].min = VALUE_TIME_MM_MIN;
    s_param_item[1].max = VALUE_TIME_MM_MAX;
    s_param_item[1].step = VALUE_TIME_MM_STEP;
    s_param_item[1].value = (int16) time.minute;
    s_param_item[1].cycle = TRUE;
    s_param_item[1].adjust_func = NULL;
    s_param_item[1].callback = NULL;
    s_param_item[1].unit_id = S_TIME_MM;
    s_param_item[1].max_number = 2;
    s_param_item[1].value_str = NULL;

    s_param_item[POSITION_AMPM].min = VALUE_TIME_AM;
    s_param_item[POSITION_AMPM].max = VALUE_TIME_PM;
    s_param_item[POSITION_AMPM].step = VALUE_TIME_AMPM_STEP;
    if (amflag == 1)
    {
        s_param_item[POSITION_AMPM].value = VALUE_TIME_AM;
    }
    else
    {
        s_param_item[POSITION_AMPM].value = VALUE_TIME_PM;
    }
    s_param_item[POSITION_AMPM].cycle = TRUE;
    s_param_item[POSITION_AMPM].adjust_func = ampm_adjust_func;
    s_param_item[POSITION_AMPM].callback = NULL;
    s_param_item[POSITION_AMPM].unit_id = S_TIME_AMPM;
    s_param_item[POSITION_AMPM].max_number = 5;
    if (amflag == 1)
    {
        s_param_item[POSITION_AMPM].value_str = am;
    }
    else
    {
        s_param_item[POSITION_AMPM].value_str = pm;
    }

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    if (g_setting_vars.g_comval.time_format == VALUE_24HOURS_FORMAT)
    {
        s_param_private.param_cnt = 2;
    }
    else
    {
        s_param_private.param_cnt = 3;
    }
    s_param_private.items = (parambox_one_t *) s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;
    if (g_setting_vars.g_comval.time_format == VALUE_24HOURS_FORMAT)
    {
        result = gui_set_parameter(PARAM_TIME_24, &s_param_com);
    }
    else
    {
        result = gui_set_parameter(PARAM_TIME_12, &s_param_com);
    }
    if(result == RESULT_CONFIRM)//确定设置参数返回
    {
        time.minute = (uint8) s_param_com.private_data->items[1].value;
        if (g_setting_vars.g_comval.time_format == VALUE_12HOURS_FORMAT)
        {
            if ((s_param_com.private_data->items[POSITION_AMPM].value == VALUE_TIME_PM)
                    && (s_param_com.private_data->items[0].value == 12)) //PM 12点 “不动”
            {
                time.hour = 12;
            }
            else if ((s_param_com.private_data->items[POSITION_AMPM].value == VALUE_TIME_AM)
                    && (s_param_com.private_data->items[0].value == 12))
            {
                time.hour = 0;
        
            }
            else if ((s_param_com.private_data->items[POSITION_AMPM].value == VALUE_TIME_PM)
                    && (s_param_com.private_data->items[0].value != 12))
            {
                time.hour = 12 + (uint8) s_param_com.private_data->items[0].value;
            }
            else
            {
                time.hour = (uint8) s_param_com.private_data->items[0].value;
        
            }
        }
        else
        {
            time.hour = (uint8) s_param_com.private_data->items[0].value;
        }
        time.second = 0;
#ifndef PC
        sys_set_time(&time);
#endif
//重设时钟后，重新审视闹钟的设置，当新设置时间超过当前设置的闹钟时，更新闹钟设置
        sys_vm_read(&g_alarm_vars, VM_ALARM, sizeof(g_alarm_vars));
        set_timer_msg(&g_alarm_vars.g_aufm, &g_alarm_vars.g_alarm, &g_alarm_vars.timer_flag);
        sys_vm_write(&g_alarm_vars, VM_ALARM);
        return (app_result_e) RESULT_REDRAW; //RESULT_MENU_CUR; //返回上一级

    }
    else
    {
        return result;
    }
}

