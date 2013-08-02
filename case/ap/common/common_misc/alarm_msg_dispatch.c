/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-4-13 16:47     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _alarm_msg_dispatch.c
 * \brief    
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               分拣处理alarm中断消息
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-4-13
*******************************************************************************/

#include "common_ui.h"

app_result_e ap_fmautorecord_init(alarm_fm_t *aufm_temp);
app_result_e ap_alarm_init(alarm_alarm_t *alarm_temp);
uint8 CompareAndSet_timer_flag(alarm_fm_t *aufm_temp, alarm_alarm_t *g_alarm);

extern uint8 __get_weekofday(uint16 y, uint8 m, uint8 d);
extern uint8 compare_curtime(time_t *Cur_T, time_t *timer1, time_t *timer2, /*time_t *timer3,*/ uint8 num);

/*! \cond COMMON_API */

/******************************************************************************/
/*!
 * \par  Description:
 *    alarm消息到之后的处理，根据cycle来决定alarm on/off
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e
 * \retval           RESULT_AUTORECORD FM自动录音 alarm 消息
 * \retval           RESULT_ALARM 闹钟 alarm 消息
 * \retval           RESULT_NULL 无须处理 alarm 消息
 * \ingroup      misc_func
 * \note
*******************************************************************************/
app_result_e com_alarm_msg_dispatch(void)
{
    alarm_vars_t  temp_alarm_val;
    uint8 flag;
    int result = 0;
    
    //sys_vm_read(&g_alarm_temp, VM_ALARM, sizeof(alarm_alarm_t));
    //sys_vm_read(&g_aufm_temp, VM_AP_DSC, sizeof(g_aufm_temp));
    sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
    /*如果没有设置闹钟和自动FM录音功能，返回*/
    if(temp_alarm_val.magic != MAGIC_ALARM)
    {
        return RESULT_NULL;
    }
    libc_memcpy(&g_alarm_temp, &temp_alarm_val.g_alarm, sizeof(alarm_alarm_t));
    libc_memcpy(&g_aufm_temp, &temp_alarm_val.g_aufm, sizeof(alarm_fm_t));
    
    if((g_aufm_temp.enable == 0) && (g_alarm_temp.enable == 0))
    {
        return RESULT_NULL;
    }

    if (temp_alarm_val.timer_flag == 0)
    {
        result = ap_fmautorecord_init(&g_aufm_temp);
        if (result == RESULT_ALARMFM)
        {
            if (temp_alarm_val.g_aufm.fmrec_enable == 1)
            {
                result = RESULT_FMRECORD;
            }
            //snooze闹铃被漏报的情况
            if(g_alarm_temp.alarm_count != 0)//Snooze标志
            {
                time_t time_tmp;
                sys_get_time(&time_tmp);  //获取系统时间
                if((time_tmp.hour*60 + time_tmp.minute + 6) > 
                    (temp_alarm_val.snooze_time.hour*60 + temp_alarm_val.snooze_time.minute) )
                {
                    //将snooze时间设入
                    temp_alarm_val.timer_flag = 1;
                    sys_set_alarm_time(&temp_alarm_val.snooze_time);
                }
            }
        }
    }
    else if (temp_alarm_val.timer_flag == 1)
    {
        result = ap_alarm_init(&g_alarm_temp);
    }
    else
    {
        return RESULT_NULL;
    }
    //决定NEXT闹铃
    flag = (uint8)CompareAndSet_timer_flag(&g_aufm_temp, &g_alarm_temp);
    if (flag == 1)
    {
        temp_alarm_val.timer_flag = 0;
        sys_set_alarm_time(&g_aufm_temp.start_time);
    }
    else if (flag == 2)
    {
        temp_alarm_val.timer_flag = 1;
        sys_set_alarm_time(&g_alarm_temp.start_time);
    }
    else
    {
        return RESULT_NULL;
    }
    //将VM重新写回
    sys_vm_write(&temp_alarm_val, VM_ALARM);

    return result;
}

/*! \endcond */

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    根据cycle标志判定是否发出RESULT_ALARMFM消息
 * \param[in]    alarm_fm_t *aufm_temp
 * \param[out]   none
 * \return       app_result_e
 * \retval           RESULT_ALARMFM 自动FM消息
 * \retval           RESULT_NULL 空消息
 * \ingroup      misc_func
 * \note
*******************************************************************************/
app_result_e ap_fmautorecord_init(alarm_fm_t *aufm_temp)
{
    uint8 startCol = 0;
    date_t t_data;
    //snooze闹铃被误报的情况
    if(g_alarm_temp.alarm_count != 0)//Snooze标志
    {
        time_t time_tmp;
        sys_get_time(&time_tmp);  //获取当前系统时间
        if(g_aufm_temp.start_time.minute != time_tmp.minute)
        {
            return RESULT_ALARM;
        }
    }
    
    sys_get_date(&t_data);
    if (aufm_temp->cycle_flag == 0) //循环--单次
    {
        if ((aufm_temp->cycle.one_time.year == t_data.year) &&
            (aufm_temp->cycle.one_time.month == t_data.month) &&
            (aufm_temp->cycle.one_time.day == t_data.day))
        {
            return RESULT_ALARMFM;
        }
    }
    else if (aufm_temp->cycle_flag == 1) //循环--每天
    {
        return RESULT_ALARMFM;
    }
    else if (aufm_temp->cycle_flag == 2) //循环--按星期
    {
        startCol = __get_weekofday(t_data.year, t_data.month, t_data.day);
        if ((startCol > 0) && (startCol < 6)) //工作日
        {
            return RESULT_ALARMFM;
        }
    }
    else
    {
        //do nothing for QAC
    }
    
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    根据cycle标志判定是否发出RESULT_ALARM消息
 * \param[in]    alarm_fm_t *aufm_temp
 * \param[out]   none
 * \return       app_result_e
 * \retval           RESULT_ALARM alarm消息
 * \retval           RESULT_NULL 空消息
 * \ingroup      misc_func
 * \note
*******************************************************************************/
app_result_e ap_alarm_init(alarm_alarm_t *alarm_temp)
{
    uint8 startCol = 0;
    date_t t_data;
    sys_get_date(&t_data);
    if (alarm_temp->cycle_flag == 0) //循环--单次
    {
        if ((alarm_temp->cycle.one_time.year == t_data.year) &&
            (alarm_temp->cycle.one_time.month == t_data.month) &&
            (alarm_temp->cycle.one_time.day == t_data.day))
        {
            return RESULT_ALARM;
        }
    }
    else if (alarm_temp->cycle_flag == 1) //循环--每天
    {
        return RESULT_ALARM;
    }
    else if (alarm_temp->cycle_flag == 2) //循环--按星期
    {
        startCol = __get_weekofday(t_data.year, t_data.month, t_data.day);
        if ((startCol > 0) && (startCol < 6)) //工作日
        {
            return RESULT_ALARM;
        }
    }
    else
    {
        //do nothing for QAC
    }
    
    return RESULT_NULL;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    根据enable_flag、及几组alarm的时间先后,返回
 * \param[in]    1 -- aufm
 * \param[in]    2 -- alarm
 * \param[out]   none
 * \return       app_result_e
 * \retval           1 -- aufm
 * \retval           2 -- alarm
 * \retval           0 -- error
 * \retval           4 -- 时间相同
 * \ingroup      misc_func
 * \note
*******************************************************************************/
uint8 CompareAndSet_timer_flag(alarm_fm_t *aufm_temp, alarm_alarm_t *g_alarm)
{
    uint8 result;
    time_t time_1;
    sys_get_time(&time_1);  //获取当前系统时间
    time_1.second ++;  //仅为简化比较

    if((aufm_temp->enable == 1) && (g_alarm->enable == 1))
    {
        result = compare_curtime(&time_1, &aufm_temp->start_time, &g_alarm->start_time, /*NULL,*/ 3);
        return result;
    }
    if (aufm_temp->enable == 1)
    {
        return 1;
    }
    if (g_alarm->enable == 1)
    {
        return 2;
    }
    return 0;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    通过比较几组闹铃的时间先后、及优先级，设置alarm时间。
 * \param[in]    alarm_fm_t *aufm_temp
 * \param[in]    alarm_alarm_t *alarm_temp
 * \param[in]    uint8 *timer_flag
 * \param[out]   none
 * \return       uint8 *timer_flag
 * \retval           0 -- aufm
 * \retval           1 -- alarm
 * \retval           3 -- error
 * \ingroup      misc_func
 * \note         设置alarm时间时调用
*******************************************************************************/
void set_timer_msg(alarm_fm_t *aufm_temp, alarm_alarm_t *alarm_temp, uint8 *timer_flag)
{
    uint8 flag;
    flag = (uint8)CompareAndSet_timer_flag(aufm_temp, alarm_temp);
    switch(flag)
    {
    case 1:
        if(aufm_temp->enable == 1)
        {
            sys_set_alarm_time(&aufm_temp->start_time);
            *timer_flag = 0;
        }
        break;
        
    case 2:
        if(alarm_temp->enable == 1)
        {
            sys_set_alarm_time(&alarm_temp->start_time);
            *timer_flag = 1;
        }
        break;
        
    case 4:  //相同时据优先级
        if((aufm_temp->enable == 1) && (alarm_temp->enable == 1))
        {
            sys_set_alarm_time(&aufm_temp->start_time);
            *timer_flag = 0;
        }
        break;
        
    case 0:
    default:
        *timer_flag = 3;
        break;
    }//end of switch
}

/*! \endcond */
