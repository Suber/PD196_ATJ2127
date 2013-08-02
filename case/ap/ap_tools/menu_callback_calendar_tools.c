/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_calendar_tools.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "case_include.h"
#include "ap_tools.h"

/*12个月的天数*/
const uint8 day_table[12] =
{ 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

uint8 g_cal_select_key; //切换激活年、月、日
bool g_cal_needdraw;
bool g_cal_needdraw_bg;

extern uint16 chineseYear;
extern int16 chineseMonth;
extern uint16 chineseDate;
extern void paintCalendarTable(date_t date, uint8 col);
extern void getLunarString(const date_t *scolar, date_t *lunar, uint8 languageId);

const key_map_t cal_key_map_list[] =
{
    /*!短按play切换年月日事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_TOOLS_CAL_SEL_YMD },
    /*! 按next向前切换事件 */
    {{ KEY_NEXT, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_TOOLS_CAL_YMD_NEXT },
    /*! 按prev向后切换事件 */
    {{ KEY_PREV, 0, KEY_TYPE_DOWN | KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_TOOLS_CAL_YMD_PREV },
    /*! 短按MODE退出事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};


/******************************************************************************/
/*!
 * \par  Description:
 * \  "年月日"的最大/最小值矫正
 * \param[in]    none 
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      menu_callback_calendar_tools.C
 * \note
 */
/*******************************************************************************/
void date_yr_adjust_func(void)
{
    if (date.year > MAX_YEAR)
    {
        date.year = MIN_YEAR;
    }
    else if (date.year < MIN_YEAR)
    {
        date.year = MAX_YEAR;
    }
    else
    {
    }
}
void date_mn_adjust_func(void)
{
    if (date.month < 1)
    {
        date.month = 12;
        date.year--;
    }
    else if (date.month > 12)
    {
        date.month = 1;
        date.year++;
    }
    else
    {
    }
    date_yr_adjust_func();
}

void date_dy_adjust_func(void)
{
    g_cal_needdraw_bg = TRUE;
    if (date.day < 1)
    {
        date.month--;
        date_mn_adjust_func();
        date.day = daysInMonth_Internal(date.year, date.month);
    }
    else if (date.day > daysInMonth_Internal(date.year, date.month))
    {
        date.month++;
        date.day = 1;
        date_mn_adjust_func();
    }
    else
    {
        g_cal_needdraw_bg = FALSE;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \  调节年月日
 * \param[in]    bool  flag_dir
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      menu_callback_calendar_tools.C
 * \note
 */
/*******************************************************************************/
void calenar_set_ymd(bool flag_dir)
{
    if (g_cal_select_key == 0)
    {
        if (!flag_dir)
        {
            date.day--;
        }
        else
        {
            date.day++;
        }
        date_dy_adjust_func();
        g_cal_needdraw = TRUE;
        //g_cal_needdraw_bg = TRUE;
    }
    else if (g_cal_select_key == 1)
    {
        if (!flag_dir)
        {
            date.month--;
        }
        else
        {
            date.month++;
        }
        date_mn_adjust_func();

        if (date.day > daysInMonth_Internal(date.year, date.month))
        {
            date.day = daysInMonth_Internal(date.year, date.month);
        }

        g_cal_needdraw = TRUE;
        g_cal_needdraw_bg = TRUE;
    }
    else if (g_cal_select_key == 2)
    {
        if (!flag_dir)
        {
            date.year--;
        }
        else
        {
            date.year++;
        }
        date_yr_adjust_func();

        if (date.day > daysInMonth_Internal(date.year, date.month))
        {
            date.day = daysInMonth_Internal(date.year, date.month);
        }

        g_cal_needdraw = TRUE;
        g_cal_needdraw_bg = TRUE;
    }
    else
    {
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  处理gui消息
 * \param[in]    gui_msg  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      menu_callback_calendar_tools.C
 * \note
 */
/*******************************************************************************/
app_result_e _calendar_guimsg(msg_apps_type_e cur_event)
{
    //按键消息处理返回值
    app_result_e key_result = RESULT_NULL;
    switch (cur_event)
    {
        case EVENT_TOOLS_CAL_SEL_YMD:
        g_cal_select_key++;
        if (g_cal_select_key > 2)
        {
            g_cal_select_key = 0;
        }
        g_cal_needdraw_bg = TRUE;
        g_cal_needdraw = TRUE;
        break;

        case EVENT_TOOLS_CAL_YMD_NEXT: //in setting clock
        calenar_set_ymd(TRUE);
        break;

        case EVENT_TOOLS_CAL_YMD_PREV: //in setting clock
        calenar_set_ymd(FALSE);
        break;

        case EVENT_PLAYING_RETURN:
        key_result = RESULT_TOOLS_MENULIST;
        break;

        default:
        key_result = com_message_box(cur_event);
        break;
    }
    return key_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  显示年月日等信息
 * \param[in]    non
 * \param[out]   none
 * \return       
 * \retval       
 * \ingroup     menu_callback_calendar_tools.C
 * \note        
 *******************************************************************************/
void show_Calendar_YMD(void)
{
    uint16 str_data;
    uint8 i, str_num;
    //	uint8 days;
    uint8 zero_posx = 80, zero_posy = 2;
    uint8 monChar[3];
    uint8 startCol = 0;
    //uint8 startRow = 0;
    //uint8 cur_weekday=0;
    region_t day_region;
    string_desc_t str_info;

    day_region.width = 16;
    day_region.height = 16;

    for (i = 0; i < 3; i++)
    {
        //		ui_set_textout_mode(TEXT_OUT_NORMAL);
        if (i == g_cal_select_key)
        {
            ui_set_pen_color(Color_RED_def);
        }
        else
        {
            ui_set_pen_color(Color_WHITE_def);
        }

        day_region.x = zero_posx - startCol * (16 + 2 * i);
        day_region.y = zero_posy;//+startRow*(16+1);
        startCol++;

        if (i == 0)
        {
            str_data = date.day;
            str_num = 2;
        }
        else if (i == 1)
        {
            str_data = date.month;
            str_num = 2;
        }
        else //if (i == 2)
        {
            day_region.x = 30;
            day_region.width = 32;
            str_data = date.year;
            str_num = 4;
        }

        libc_itoa(str_data, monChar, str_num);
        str_info.data.str = monChar;
        str_info.language = (int8)g_comval.language_id;
        str_info.length = str_num;

        ui_show_string(&str_info, &day_region, DISP_ALIGN_MEDIACY);
    }
    
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  显示当前日期的节日，阴历等
 * \param[in]    date_t sdate
 * \param[in]    uint8 cur_weekday
 * \param[out]   none
 * \return       
 * \retval       
 * \ingroup     menu_callback_calendar_tools.C
 * \note        语言为简体或繁体中文时，显示阴历，季节，节日等信息。
                其他语言只显示公历和节日。
 *******************************************************************************/
void show_cal_lunar(date_t sdate, uint8 cur_weekday)
{
    date_t scolar, lunar;

    scolar.year = sdate.year;
    scolar.month = sdate.month;
    scolar.day = sdate.day;
    
    if (g_comval.language_id == CHINESE_SIMPLIFIED)
    {
        ui_set_pen_color(Color_WHITE_def);
        getLunarString(&scolar, &lunar, g_comval.language_id);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  calendar的main函数
 * \param[in]    flag  0——万年历；1——FM定时录音日期的设置；2——闹钟日期的设置
 * \param[out]   app_result_e result
 * \return       
 * \retval       
 * \ingroup     menu_callback_calendar_tools.C
 * \note        
 *******************************************************************************/
app_result_e calendar_main(uint8 flag)
{
    uint16 temp_i;
    uint8 days; 
    uint8 cur_weekday = 0;
    app_result_e result = RESULT_NULL;
    bool gui_result = FALSE;
    //for message 
    private_msg_t private_msg;
    input_gui_msg_t gui_msg;
    msg_apps_type_e gui_event;
    //for gui
    style_infor_t style_infor;
    textbox_private_t week_textbox_param;
    
    //init variable
    g_cal_select_key = 0;
    g_cal_needdraw = TRUE;
    g_cal_needdraw_bg = TRUE;
    
    if (flag == FROM_MENU)
    {
        sys_get_date(&date);
        if ((date.year > MAX_YEAR) || (date.year < MIN_YEAR))
        {
            date.year = 2012;
        }
        if ((date.month > 12) || (date.month < 1))
        {
            date.month = 1;
        }
        if ((date.day > 31) || (date.day < 1))
        {
            date.day = 1;
        }
#ifndef PC
        sys_set_date(&date);
#endif
    }
    else if (flag == FROM_ALARM)
    {
        date.year = g_alarm_vars.g_alarm.cycle.one_time.year;
        date.month = g_alarm_vars.g_alarm.cycle.one_time.month;
        date.day = g_alarm_vars.g_alarm.cycle.one_time.day;
    }
    else if (flag == FROM_ALARMFM)
    {
        date.year = g_alarm_vars.g_aufm.cycle.one_time.year;
        date.month = g_alarm_vars.g_aufm.cycle.one_time.month;
        date.day = g_alarm_vars.g_aufm.cycle.one_time.day;
    }
    else
    {
    }

    while (1)
    {
        //显示背景
        if (g_cal_needdraw_bg == TRUE)
        {
            style_infor.style_id = CALENDAR_BG;
            style_infor.type = UI_AP;
            ui_show_picbox(&style_infor, NULL);

            show_Calendar_YMD();

            for (temp_i = 0; temp_i < 7; temp_i++)
            {
                style_infor.style_id = CAL_WEEK7 + temp_i;
                week_textbox_param.lang_id = (int8) g_comval.language_id;
                week_textbox_param.str_id = -1;
                week_textbox_param.str_value = NULL;
                ui_show_textbox(&style_infor, &week_textbox_param, TEXTBOX_DRAW_NORMAL);
            }

            show_cal_lunar(date, cur_weekday);
            cur_weekday = (uint8)get_weekofday(date.year, date.month, 1);

            g_cal_needdraw_bg = FALSE;
        }

        //显示时钟
        if (g_cal_needdraw == TRUE)
        {
            paintCalendarTable(date, cur_weekday);
            ui_show_pic(P_CALENDAR_BAR1, 0, 0);
            show_Calendar_YMD();
            ui_show_pic(P_CALENDAR_BAR2, 0, 18);
            show_cal_lunar(date, cur_weekday);

            g_cal_needdraw = FALSE;
        }

        //获取和处理GUI消息、私有消息
        gui_result = get_gui_msg(&gui_msg);
        if ((gui_result == TRUE))
        {
            if (com_key_mapping(&gui_msg, &gui_event, cal_key_map_list) == TRUE)
            {
                result = _calendar_guimsg(gui_event);
                if(result == RESULT_REDRAW)
                {
                    g_cal_needdraw_bg = TRUE;
                    g_cal_needdraw = TRUE;
                    result = RESULT_NULL;
                }
                else if(result != RESULT_NULL)
                {
                    break;
                }
                else
                {
                    
                }
            }
//            if (result != RESULT_NULL)
//            {
//                break;//return ret;
//            }
        }

        //处理系统消息
        if (get_app_msg(&private_msg) == TRUE)
        {
            result = com_message_box(private_msg.msg.type);
            if(result == RESULT_REDRAW)
            {
                g_cal_needdraw_bg = TRUE;
                g_cal_needdraw = TRUE;
            }
            else if(result != RESULT_NULL)
            {
                break;
            }
            else
            {
                
            }
        }
        //挂起10ms，多任务调度
        sys_os_time_dly(1);

    }//while(1)

    //re-check the date/time format
    if ((date.month < 1) || (date.month > 12)) //QAM
    {
        return RESULT_TOOLS_MENULIST;
    }
    days = day_table[date.month - 1];
    if (((date.year % 4) == 0) && (date.month == 2))
    {
        days++; //润年
    }
    if (date.day > days)
    {
        date.day = days;
    }

    if (flag == FROM_ALARM) //保存闹钟日期的设置
    {
        g_alarm_vars.g_alarm.cycle.one_time.year = date.year;
        g_alarm_vars.g_alarm.cycle.one_time.month = date.month;
        g_alarm_vars.g_alarm.cycle.one_time.day = date.day;
        if(result == RESULT_TOOLS_MENULIST)
        {
            result = RESULT_REDRAW;
        }
    }
    else if (flag == FROM_ALARMFM) //保存闹钟日期的设置
    {
        g_alarm_vars.g_aufm.cycle.one_time.year = date.year;
        g_alarm_vars.g_aufm.cycle.one_time.month = date.month;
        g_alarm_vars.g_aufm.cycle.one_time.day = date.day;
        if(result == RESULT_TOOLS_MENULIST)
        {
            result = RESULT_REDRAW;
        }
    }
    else
    {
    }

    return result;
}

