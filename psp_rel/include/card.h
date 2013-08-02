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
 * \file     storage_device.h
 * \brief    块设备驱动接口
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef _CARD_DEVICE_H
#define _CARD_DEVICE_H

#ifndef _ASSEMBLER_

#include <storage_device.h>
#include <typeext.h>

/* 
 * 通过全局变量g_card_para（详见下边地址）设置卡的属性，应用需要在装载卡驱动之前设置这个变量。如果不设置则会使用前一次设置的值。
 */
/* 
    bit0: 0--1_line, 1--4_line; 
    bit1(sdvccout power): 0--direct, 1--GPIO; 
    bit2(DAT3 pull high): 0--hardware pull, 1--GPIO pull;
    bit3(write_fast):0--not continuous, 1--continuous write;
*/
#define     g_card_para     (*(uint8 *)(RDATA_ADDR_BASE + 0x19e7d))

extern int device_card_read(unsigned int addr,unsigned char* buf,unsigned int sec_len) __FAR__;
extern int device_card_write(unsigned int addr,unsigned char* buf,unsigned int sec_len) __FAR__;
extern int device_card_update(void* null0, void* null1, void* null2);
extern int device_card_is_write_protect(void* null0, void* null1, void* null2);
extern uint32 device_card_get_cap(void* null0, void* null1, void* null2);
extern uint32 device_card_get_sector_size(void* null0, void* null1, void* null2);

#endif

#endif

