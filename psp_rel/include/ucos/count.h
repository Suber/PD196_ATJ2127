/********************************************************************************
 *                              USDK130
 *                            Module: COUNT/COMPARE
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wuyueqian     2011-9-10 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     cout.h
 * \brief    绝对时间函数声明和绝对tick数函数声明
 * \author   wuyueqian
 * \version 1.0
 * \date  2011-9-10
 *******************************************************************************/
#ifndef __COUNT_H__
#define __COUNT_H__

#ifndef _ASSEMBLER_

#include <time.h>
#include <pm.h>
#include <ucos/init.h>

#define TIME_DIFF_S     (2)
#define TIME_DIFF_MS    (TIME_DIFF_S*1000)

extern void get_time_of_day(struct timeval *tv, void* null1, void* null2) __FAR__;
extern unsigned int get_ab_timer(void *null0, void* null1, void* null2) __FAR__;

extern void count_pm_cb(const frequency_param_t *curclk) __FAR__;
extern uint32 random(void *null1, void* null2, void* null3) __FAR__;


#endif /*_ASSEMBLER_*/
#endif/*__COUNT_H__*/
