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
 * \file     _rom_ui_image.c
 * \brief    ROM代码模块--图片显示部分（小彩屏）
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2012-1-12
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    根据图片res_id显示该图片
 * \param[in]    id：图片资源ID
 * \param[in]    x：图片的X坐标
 * \param[in]    y：图片的Y坐标
 * \param[out]   none
 * \return       图片是否显示成功，1或0
 * \ingroup      picture
 * \note
 *******************************************************************************/
bool romf_show_pic(uint16 id, uint16 x, uint16 y)
{
    res_open_infor_t *res_ptr;
    res_entry_t res_entry;
    region_t res_region;

    //读取图片资源信息
    res_ptr = res_ptr_select(id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));

    /*设窗参数设置*/
    res_region.x = x;
    res_region.y = y;
    res_region.width = res_entry.width;
    res_region.height = res_entry.height;

    /*读取图片数据并刷屏*/
    return put_sd_image(res_ptr->res_fp, res_entry.offset, &res_region);
}
/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示sd文件中指定位置的图片
 * \param[in]    res_fp：SD资源文件句柄
 * \param[in]    img_addr：当前图片在sd文件中的起始地址
 * \param[in]    img_region：图片显示区域
 * \param[out]   none
 * \return       none
 * \ingroup      ui driver
 * \note
 *******************************************************************************/
bool romf_put_sd_image(sd_handle *res_fp, uint32 img_addr, region_t *img_region)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint32 cnt;
    uint32 start_addr;
    uint16 count;
    bool result = TRUE;

    //检测显示区域是否超出
    if (((img_region->x + img_region->width) > (region_win.x + region_win.width))
     || ((img_region->y + img_region->height) > (region_win.y + region_win.height)))
    {
        return FALSE;//X轴或者Y轴越界
    }

    //设置图片显示模式及窗口
    set_lcd_param_hook(img_region, LCD_PARAM_IMAGE);

    //初始化数据大小及下次读取地址，以及缓冲区地址
    cnt = ((uint32) img_region->width) * ((uint32) img_region->height) * 2;
    start_addr = img_addr;

    //分多次读取显示，每次读取并显示显示暂存区的大小的图片数据
    while (cnt > 0)
    {
        //读取数据
        if (cnt >= (uint32) display_buf_len)
        {
            count = display_buf_len;
        }
        else
        {
            count = (uint16) cnt;
        }
        sys_sd_fseek(res_fp, SEEK_SET, start_addr);
        sys_sd_fread(res_fp, display_buffer, (uint32) count);

        //DMA到GRAM中
        if (dma_buffer_data_hook(count / 2, img_region->width) == FALSE)
        {
            result = FALSE;
            break;
        }

        //更新剩余数据大小及下次读取地址
        cnt = cnt - count;
        start_addr += count;
    }

    return result;
}

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
bool romf_fill_rect(region_t *fill_region, uint16 color)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint32 cnt, buf_cnt;
    uint32 count;
    uint16 i;
    bool result = TRUE;

    //检测显示区域是否超出
    if (((fill_region->x + fill_region->width) > (region_win.x + region_win.width)) 
     || ((fill_region->y + fill_region->height) > (region_win.y + region_win.height)))
    {
        return FALSE;//X轴或者Y轴越界
    }

    //设置显示模式及填充窗口
    set_lcd_param_hook(fill_region, LCD_PARAM_FILL);

    //初始化数据大小，缓冲区地址，缓冲区填充
    cnt = ((uint32) fill_region->width) * ((uint32) fill_region->height) * 2;
    //如果要填充的数据量较小，无需初始化那么多缓冲区
    buf_cnt = display_buf_len;
    if (cnt < buf_cnt)
    {
        buf_cnt = cnt;//仅需初始化刚好覆盖填充区域的缓冲区
    }
    for (i = 0; i < (buf_cnt / 2); i++)
    {
        display_buffer[i * 2] = (uint8)(color & 0xff);
        display_buffer[i * 2 + 1] = (uint8)((color >> 8) & 0xff);
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
