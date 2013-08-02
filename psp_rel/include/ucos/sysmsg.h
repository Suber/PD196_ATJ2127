/*******************************************************************************
 *                              USDK213F
 *                            Module: SYSMSG
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     wuyueqian     2011-9-10 9:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     sysmsg.h
 * \brief    系统消息头文件
 * \author   wuyueqian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date   2011-9-10
 *******************************************************************************/
#ifndef __SYSMSG_H__
#define __SYSMSG_H__

#define msg_LowBat		0x06
#define msg_LowBatRec	0x07
#define msg_StubEnable 	0x08
#define msg_StubDisable	0x09
#define msg_StubSuspend	0x0a
#define msg_StubResume	0x0b

#ifndef _ASSEMBLER_

extern int put_sysmsg(int msg);
extern int get_sysmsg(void);
extern void flush_sysmsg(void);

#endif /*_ASSEMBLER_*/
#endif /*__SYSMSG_H__*/
