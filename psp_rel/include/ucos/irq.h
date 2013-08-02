/********************************************************************************
 *                               USDK(1100)
 *                            Module: IRQ
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     irq.h
 * \brief    中断处理相关宏定义、数据结构和函数声明
 * \author   wuyueqian
 * \version 1.0
 * \date  2011-9-10
 *******************************************************************************/
#ifndef __IRQ_H__
#define __IRQ_H__

#include <typeext.h>
#include <asm-mips/mach-gl5110/irq.h>
#include <asm-mips/exception.h>
#include <os_cpu.h>
#include <attr.h>

#define SA_SHIRQ        0x04000000
#define IRQ_DISABLED    2   /* IRQ disabled - do not enter! */

#define IRQ_HANDLED     (1)
#define IRQ_NONE            (0)

#define OFFSET_OS_SYSCALL_API       (0x268)
#define OFFSET_OS_SYSCALL_STATUS    (0x26c)

#define OS_START 0xbfc00000
#define ABS_OS_SYSCALL_API          (OS_START|OFFSET_OS_SYSCALL_API)
#define ABS_OS_SYSCALL_STATUS       (OS_START|OFFSET_OS_SYSCALL_STATUS)

#ifndef _ASSEMBLER_

#include <asm-mips/interrupt.h>

typedef int irqreturn_t;

typedef void (*interrupt_handle_t)(void);

extern interrupt_handle_t interrupt_handle[NR_IRQS];

extern void init_irq(void);
extern int request_irq(unsigned int irq, interrupt_handle_t int_handle, void* null2) __FAR__;
extern void free_irq(unsigned int irq, void *null1, void* null2) __FAR__;

//extern uint32 local_irq_save(uint32 flags);
//extern void local_irq_restore(uint32 flags);

#endif /*_ASSEMBLER_*/

#endif /*__IRQ_H__*/
