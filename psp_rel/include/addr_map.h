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
 * \file     addr_map.h
 * \brief    µÿ÷∑–≈œ¢
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef __ADDR_MAP_H__
#define __ADDR_MAP_H__

#define SRAM_BANK_A_ADDR 0x24c00
#define SRAM_BANK_A_SIZE 0x400
#define SRAM_BANK_B_ADDR 0x25000
#define SRAM_BANK_B_SIZE 0x800
#define SRAM_BANK_C_ADDR 0x21A00
#define SRAM_BANK_C_SIZE 0x400

#define SRAM_SIZE_MASK 0x3ffff
#define RDATA_ADDR_BASE 0x9fc00000
#define RCODE_ADDR_BASE 0xbfc00000

#define KERNEL_BANK_A 0x1
#define KERNEL_BANK_B 0x2
#define KERNEL_BANK_C 0x3

#define AP_BANK_FRONT_CONTROL_1 0x40//0b01000000
#define AP_BANK_FRONT_CONTROL_1_PAGE_INDEX 8
#define AP_BANK_FRONT_CONTROL_1_BASE_OFFSET (2048)

#define AP_BANK_FRONT_UI_1      0x48//0b01001000
#define AP_BANK_FRONT_UI_1_PAGE_INDEX      9
#define AP_BANK_FRONT_UI_1_BASE_OFFSET      (2048*2)

#define AP_BANK_FRONT_BASAL     0x50//0b01010000
#define AP_BANK_FRONT_BASAL_PAGE_INDEX     10
#define AP_BANK_FRONT_BASAL_BASE_OFFSET      (2048)

#define AP_BANK_FRONT_CODEC     0x58//0b01011000
#define AP_BANK_FRONT_CODEC_PAGE_INDEX     11
#define AP_BANK_FRONT_CODEC_BASE_OFFSET      (2048)

#define AP_BANK_BACK_CONTROL_1  0x60//0b01100000
#define AP_BANK_BACK_CONTROL_1_PAGE_INDEX  12
#define AP_BANK_BACK_CONTROL_1_BASE_OFFSET      (2048)

#define AP_BANK_BACK_BASAL      0x68//0b01101000
#define AP_BANK_BACK_BASAL_PAGE_INDEX      13
#define AP_BANK_BACK_BASAL_BASE_OFFSET      (2048)

#define AP_BANK_BACK_CODEC      0x70//0b01110000
#define AP_BANK_BACK_CODEC_PAGE_INDEX      14
#define AP_BANK_BACK_CODEC_BASE_OFFSET      (2048)

#define AP_BANK_FRONT_ENHANCED_1      0x78//0b01111000
#define AP_BANK_FRONT_ENHANCED_1_PAGE_INDEX      15
#define AP_BANK_FRONT_ENHANCED_1_BASE_OFFSET      (2048*3)

#define AP_BANK_FRONT_ENHANCED_2      0x71//0b01110001
#define AP_BANK_FRONT_ENHANCED_2_PAGE_INDEX      16
#define AP_BANK_FRONT_ENHANCED_2_BASE_OFFSET      (2048*4)

#define AP_BANK_BACK_ENHANCED_1       0x7a//0b01111010
#define AP_BANK_BACK_ENHANCED_1_PAGE_INDEX       17
#define AP_BANK_BACK_ENHANCED_1_BASE_OFFSET      (2048*3)

#define AP_BANK_BACK_ENHANCED_2       0x7b//0b01111011
#define AP_BANK_BACK_ENHANCED_2_PAGE_INDEX       18
#define AP_BANK_BACK_ENHANCED_2_BASE_OFFSET      (2048*4)

#define AP_BANK_MANAGER       0x7f//0b01111111
#define AP_BANK_MANAGER_PAGE_INDEX       19
#define AP_BANK_MANAGER_BASE_OFFSET      (2048)

#endif
