/*******************************************************************************
 *                              USDK1100
 *                            Module: IRQ_TIMER
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       wuyueqian     2011-9-10 9:42     1.0             build this file
*******************************************************************************/
/*!
 * \file     irq_timer.h
 * \brief    在中断服务程序中运行的定时器
 * \author   wuyueqian
 * \version 1.0
 * \date    2011-9-10 
*******************************************************************************/

#ifndef __TIMER1_H__
#define __TIMER1_H__

#include <ucos/init.h>
#ifndef _ASSEMBLER_

#define MAX_IRQ_TIMERS 6

/*!
 * \brief
 *      中断定时器数据结构
 */
typedef struct
{
    /* 处理函数 */
    void (*handler)(void);
    /* 时间间隔 */
    uint16 period;
    /* 当前计数值 */
    uint16 cur_value;
}irq_timer_t;

extern int set_irq_timer1(void (*handler)(void), uint16 period, void *null2) __FAR__;
extern int del_irq_timer1(unsigned int timer, void *null1, void *null2) __FAR__;

#endif /*_ASSEMBLER_*/
#endif /*__IRQ_TIMER_H__*/
