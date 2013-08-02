/********************************************************************************
*                               USDK(1100)
*                            Module: CTYPE
*                 Copyright(c) 2001-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*      wuyueqian     2011-9-10 15:00     1.0             build this file
********************************************************************************/
/*!
* \file     delay.h
* \brief    延时函数(无任务切换)声明
* \author   wuyueqian
* \version 1.0
* \date  2011-9-10
*******************************************************************************/
#ifndef __DELAY_H__
#define __DELAY_H__

#ifndef _ASSEMBLER_

#include <ucos/init.h>

extern void mdelay_fix(int num, void *null1, void* null2) __FAR__;
extern void udelay_fix(int num, void *null1, void* null2) __FAR__;
extern void mdelay(int num, void *null1, void* null2) __FAR__;
extern void udelay(int num, void *null1, void* null2) __FAR__;
extern void sleep(unsigned seconds, void *null1, void* null2);
extern void usleep(unsigned useconds, void *null1, void* null2);

#endif /*_ASSEMBLER_*/
#endif /*__DELAY_H__*/
