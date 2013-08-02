/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       cailizhen     2012-1-12 9:42     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_fill_rect_dump.c
 * \brief    用指定图标重复填充区域
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2012-2-17
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    将指定区域填充成指定颜色，填充过程类似显示SD BMP图片
 * \param[in]    fill_region：指向要填充的区域
 * \param[in]    color：填充颜色
 * \param[out]   none
 * \return       返回填充是否成功
 * \note         在一些图形化方案（比如游戏）中，填充是非常基本的接口，所以也固化进去。
 *******************************************************************************/
bool fill_rect_dump(region_t *fill_region, uint16 pic_id, fill_dump_direction_e direction)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint32 cnt, buf_cnt;
    uint32 count;
    uint32 pic_len;
    uint16 i;
    bool result = TRUE;

    //读回图片属性，并设置相关属性
    res_open_infor_t *res_ptr;
    res_entry_t res_entry;
    display_buffer_deal();
    //读取图片资源信息
    res_ptr = res_ptr_select(pic_id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(pic_id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));

    pic_len = res_entry.width * res_entry.height * 2;

    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_entry.offset);
    sys_sd_fread(res_ptr->res_fp, display_buffer, pic_len);

    //重新初始化DMA为RGB模式
    lcd_controller_init(RGB_MODE);

    if (direction == DUMP_LEFT2RIGHT)//图片
    {
        fill_region->height = res_entry.height;
#ifndef PC
        if (rotate_flag == FALSE)
        {//如果垂直方向上渐变，只适应单个像素点宽度，多像素点宽度显示会出错
            lcd_set_draw_mode(DRAW_MODE_H_SCROLL_DEF); //设置竖屏模式，对内容进行列拷贝，按列优先模式进行刷新
        }
        else
        {
            lcd_set_draw_mode(DRAW_MODE_V_DEF); //设置横屏模式
        }
#else
        lcd_set_draw_mode(DRAW_MODE_H_DEF); //设置竖屏模式
#endif
    }
    else if (direction == DUMP_DOWN2UP)
    {
        fill_region->width = res_entry.width;
#ifndef PC
        if (rotate_flag == FALSE)
        {
            lcd_set_draw_mode(DRAW_MODE_H_DEF); //设置竖屏模式
        }
        else
        {
            lcd_set_draw_mode(DRAW_MODE_V_DEF); //设置横屏模式
        }
#else
        lcd_set_draw_mode(DRAW_MODE_H_DEF); //设置竖屏模式
#endif
    }
    else
    {
        //do nothing for QAC
    }

    lcd_set_window(fill_region);

    //初始填充区域数据

    //初始化数据大小，缓冲区地址，缓冲区填充
    cnt = ((uint32) fill_region->width) * ((uint32) fill_region->height) * 2;
    //如果要填充的数据量较小，无需初始化那么多缓冲区
    buf_cnt = display_buf_len;
    if (cnt < buf_cnt)
    {
        buf_cnt = cnt;//仅需初始化刚好覆盖填充区域的缓冲区
    }
    if (pic_len != 0)
    {
        buf_cnt = buf_cnt - (buf_cnt % pic_len);
        for (i = 1; i < (buf_cnt / pic_len); i++)
        {
            libc_memcpy(display_buffer + pic_len * i, display_buffer, pic_len);
        }
    }
    //分多次填充
    while (cnt > 0)
    {
        //读取数据
        if (cnt >= buf_cnt)
        {
            count = buf_cnt;
        }
        else
        {
            count = cnt;
        }

        //DMA到GRAM中
        if (dma_buffer_data_hook(count / 2, fill_region->width) == FALSE)
        {
            result = FALSE;
            break;
        }

        //更新剩余数据大小及下次读取地址
        cnt = cnt - count;
    }

    return result;
}
/*! \endcond */
