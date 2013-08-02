/*******************************************************************************
 *                              us212A
 *                            Module: ui驱动
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-9 16:23     1.0             build this file
 *******************************************************************************/
/*!
 * \file     lcd_driver.h
 * \brief    lcd物理驱动模块相关数据结构，宏定义，接口声明等
 * \author   liutinging
 * \version 1.0
 * \date  2011-9-9
 *******************************************************************************/

#ifndef _LCD_INNER_H
#define _LCD_INNER_H

#include "psp_includes.h"
#include "lcd_driver.h"

#ifdef _FPGA_

/*! LCD模组实际的分辨率的实际长度 */
#define LCD_WIDTH 132
/*! LCD模组分辨率的实际宽度 */
#define LCD_HEIGHT 162

/*! 132X162 的LCD居中显示时，相对于LCD模组长度的起始偏移 */
#define LCD_WIDTH_OFFSET  2
/*! 32X162 的LCD居中显示时，相对于LCD模组宽度的起始偏移 */
#define LCD_HEIGHT_OFFSET 1

#else
/*! LCD模组实际的分辨率的实际长度 */
#define LCD_WIDTH 128
/*! LCD模组分辨率的实际宽度 */
#define LCD_HEIGHT 64//160 //modify--vzhan

/*! 132X162 的LCD居中显示时，相对于LCD模组长度的起始偏移 */
#define LCD_WIDTH_OFFSET  0
/*! 32X162 的LCD居中显示时，相对于LCD模组宽度的起始偏移 */
#define LCD_HEIGHT_OFFSET 0

#endif

#define LCD_CMD_STANDBY         0x10
#define LCD_CMD_EXIT_SLEEP      0x11
#define LCD_CMD_DISPLAY_OFF     0x28
#define LCD_CMD_DISPLAY_ON      0x29
#define LCD_CMD_SET_X           0x2a
#define LCD_CMD_SET_Y           0x2b
#define LCD_CMD_WRITE           0x2c
#define LCD_CMD_READ            0x2e
#define LCD_CMD_SETMODE         0x36
//display mode

//内部函数
void store_ce(void)__FAR__;
void restore_ce(void)__FAR__;
void store_ce_gio(void)__FAR__;
void restore_ce_gio(void)__FAR__;
void write_data(uint8 *data_buf, uint16 pix_cnt)__FAR__;
void write_command(uint8 cmd)__FAR__;
void set_mfp_to_emif(void)__FAR__;
void set_mfp_to_lcd(void)__FAR__;
void dma_set_RAM_clk(uint8 dma_ram)__FAR__;
void lcd_hardware_init(void)__FAR__;

#ifdef __WELCOME__
void welcome_delay_us(uint8 dly_us);
void welcome_delay_ms(uint8 dly_ms);
#endif//__WELCOME__

extern region_t region_setwin;
#endif //_LCD_INNER_H
