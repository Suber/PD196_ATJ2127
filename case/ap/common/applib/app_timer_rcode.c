/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-28 21:37     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _app_timer_rcode.c
 * \brief    应用级定时器管理模块实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现一套基本的应用级定时器管理接口，包括创建，修改，删除，handle处理等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，等等。
 * \version  1.0
 * \date     2011-9-28
*******************************************************************************/

#include "applib.h"

/*! \cond COMMON_API */

/******************************************************************************/
/*!                    
 * \par  Description:
 *    应用级定时器 handle 处理，在应用消息循环中循环调用，循环扫描所有定时器，执行
 *    定时溢出的定时器的服务例程 proc，并根据定时器类型进行设置。
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      app_timer
 * \note 
*******************************************************************************/
void handle_timers(void)
{
    app_timer_t *tm;
    uint32 cur_time_ms;
    int8 i;

    for(i = 0; i < g_app_timer_count; i++) 
    {
        tm = &g_app_timer_vector[i];
        if(((tm->attrb==APP_TIMER_ATTRB_CONTROL)||(g_app_info_state.backlight_state!=BACKLIGHT_STATE_OFF))//类型校验
        && ((tm->tag == g_this_app_timer_tag) || (tm->tag == APP_TIMER_TAG_SYS)))//场景校验
        {
            cur_time_ms = sys_get_ab_timer();
            if((cur_time_ms >= tm->timeout_expires)//定时时间已到
            && (tm->state == TIMER_STATE_RUNNING)//定时器正在运行才需要执行handle
            && (tm->func_proc != NULL))//定时器handle有效
            {
                //重设软定时器超时时间
                if(tm->timeout > (cur_time_ms - tm->timeout_expires))
                {
                    tm->timeout_expires += tm->timeout;//直接在expires上加定时周期，以确保多次定时时间准确
                }
                else
                {
                    tm->timeout_expires = sys_get_ab_timer() + tm->timeout;
                }
                
                //单发软定时器一次执行完后 kill 掉
                if(tm->type == TIMER_TYPE_SINGLE_SHOT)
                {
                    tm->state = TIMER_STATE_NOUSED;
                }
                
                //调用软定时器服务例程
                tm->func_proc();
            }
    
            if( cur_time_ms <= (tm->timeout_expires - tm->timeout ))
            {
                //如果系统时间因某种原因落在超时时间点后面，那么必须修复该时间点
                tm->timeout_expires = sys_get_ab_timer() + tm->timeout;
            }
        }
    }
}

/*! \endcond */
