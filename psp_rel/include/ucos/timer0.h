/********************************************************************************
 *                              USDK(1100)
 *                            Module: TIMER
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 9:42     1.0             build this file
 ********************************************************************************/
/*!
 * \file     timer.h
 * \brief    系统定时器头文件
 * \author   open source
 * \version 1.0
 * \date  2011-9-10
 *******************************************************************************/
#ifndef __TIMER_H__
#define __TIMER_H__

#include <ucos/list.h>
#include <ucos/interrupt.h>

#ifndef _ASSEMBLER_

/*!
 * \brief
 *      定时器链表结构定义.
 */
struct timer_list {
	/*! 定义器双向链表*/
	struct list_head entry;//null
	/*! 激活时刻*/
	unsigned long expires;//null
	/*! 定时器回调函数*/
	void (*function)(void);//用户需填
	/*! period*/
	unsigned long count;//用户需填
};

extern void add_timer(struct timer_list * timer);
extern int del_timer(struct timer_list * timer);

extern void init_timers(void);
extern struct list_head timer_header;

#endif /*_ASSEMBLER_*/
#endif 	/*__TIMER_H__*/
