/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-textread
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 21:49     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_textread_sub.c
 * \brief    commonUI 文本浏览控件控制流实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种文本浏览 GUI。
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2011-9-19
*******************************************************************************/

#include "common_ui.h"

#define TEXT_ROW_MAX_COUNT 10

const uint16 row_pos_x = 0;
const uint16 row_pos_y[TEXT_ROW_MAX_COUNT] = {0, 16, 32, 48, 64, 80, 96, 112, 128, 144};


bool sd_fseek_sector (sd_handle file_handle, uint8 where, int32 sector_offset)
{
    return (bool)(sys_sd_fseek(file_handle, (uint32)where, sector_offset * 512) == 0);
}

bool sd_fread_sector (sd_handle file_handle, uint8 *text_buf, uint32 sector_len)
{
    return (bool)(sys_sd_fread(file_handle, text_buf, sector_len * 512) != -1);
}

//更新文本页
void show_text_line(string_desc_t *string_infor, uint8 line)
{
    region_t row_region;
    
    row_region.x = row_pos_x;
    row_region.y = row_pos_y[line];
    row_region.width = 128;
    row_region.height = 16;
    
    ui_set_pen_color(0xffff);
    ui_show_string(string_infor, &row_region, DISP_LONGSTR_NO_RECT);
}
