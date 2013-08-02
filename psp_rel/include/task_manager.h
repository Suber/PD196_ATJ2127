
/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     TASK_MANAGER.h
 * \brief    APπ‹¿Ì
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef __TASK_MANAGER_H__
#define __TASK_MANAGER_H__

#define SRAM_STACK_ADDR 0X9fc25800

#define AP_PROCESS_MANAGER_STK_POS (SRAM_STACK_ADDR + 0x300) /*25b00*/
#define AP_PROCESS_MANAGER_PRIO 0xC

#define AP_BACK_HIGH_PRIO 0x3
#define AP_BACK_HIGH_STK_POS (AP_PROCESS_MANAGER_STK_POS + 0x500) /*26000*/
#define AP_BACK_LOW_PRIO 0x4				/*codec*/
#define AP_BACK_LOW_STK_POS (AP_BACK_HIGH_STK_POS + 0x5c0)/*265c0*/

#define AP_FRONT_HIGH_PRIO 0x6
#define AP_FRONT_HIGH_STK_POS (AP_BACK_LOW_STK_POS + 0x500)/*26ac0*/
#define AP_FRONT_LOW_PRIO 0x7
#define AP_FRONT_LOW_STK_POS (AP_FRONT_HIGH_STK_POS + 0x3f0)/*26eb0*/

/*0x300 to be distributed*/
#endif

