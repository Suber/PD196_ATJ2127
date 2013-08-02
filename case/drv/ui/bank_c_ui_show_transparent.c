/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-09 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_show_transparent.c
 * \brief    显示透明图标
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *               实现了透明图标的显示，比如数字，这样数字就不再需要限制于背景颜色必须是纯色的。
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-7-30
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

void turn_pixel(uint8 *pixel_buffer, region_t *turn_region);
void pic_overlay(uint8 *pixel_result, uint8 *pic_source, region_t *b_region, region_t *p_region, \
transparent_pic_t *p_trs_pic);
bool put_image(region_t *img_region);

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    显示透明图标
 * \param[in]    p_trs_pic 透明图标显示控制结构体
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
 * \endcode
 * \note
 *******************************************************************************/
void show_pic_transparent(transparent_pic_t *p_trs_pic)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint8 *display_buffer_tmp = display_buffer + 512;
    res_open_infor_t *back_ptr, *res_ptr;
    res_entry_t back_entry, res_entry;
    region_t back_region, pic_region;
    uint32 back_data_col, back_data_len, pic_data_len;

    //背景图片
    back_ptr = res_ptr_select(p_trs_pic->back_id);
    sys_sd_fseek(back_ptr->res_fp, SEEK_SET, \
    back_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(p_trs_pic->back_id) * RESHEADITEM));
    sys_sd_fread(back_ptr->res_fp, &back_entry, sizeof(res_entry_t));
    
    //计算按列优先存储图片数据偏移位置
    if(p_trs_pic->direct == 0)
    {
        back_data_col = back_entry.offset + (back_entry.width * back_entry.height * 2);
        //4字节对齐
        if((back_data_col % 4) != 0)
        {
            back_data_col = (back_data_col & 0xfffffffc) + 4;
        }
        back_data_col += (p_trs_pic->pic_x - p_trs_pic->back_x)*back_entry.height*2;
    }
    else
    {
        back_data_col = back_entry.offset;
        back_data_col += (p_trs_pic->pic_y - p_trs_pic->back_y)*back_entry.width*2;
    }
    
    //数字图片
    res_ptr = res_ptr_select(p_trs_pic->pic_id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, \
    res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(p_trs_pic->pic_id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));
    //前景数字图片的大小
    pic_data_len = res_entry.width * res_entry.height * 2;
    pic_region.x = p_trs_pic->pic_x;
    pic_region.y = p_trs_pic->pic_y;
    pic_region.width = res_entry.width;
    pic_region.height = res_entry.height;

    /*设窗参数设置*/
    if(p_trs_pic->direct == 0)
    {
        back_region.x = p_trs_pic->pic_x;
        back_region.y = p_trs_pic->back_y;
        back_region.width = res_entry.width;
        back_region.height = back_entry.height;//允许高度不一致
    }
    else
    {
        back_region.x = p_trs_pic->back_x;
        back_region.y = p_trs_pic->pic_y;
        back_region.width = back_entry.width;//允许宽度不一致
        back_region.height = res_entry.height;
    }
    
    //背景图片读取数据大小为区域大小
    back_data_len = back_region.width * back_region.height * 2;
    
    //读取背景图片数据
    sys_sd_fseek(back_ptr->res_fp, SEEK_SET, back_data_col);
    sys_sd_fread(back_ptr->res_fp, display_buffer, back_data_len);
    
    //背景图片数据转换
    if(p_trs_pic->direct == 0)//横向才需要矩阵变换
    {
        turn_pixel(display_buffer, &back_region);
    }
    
    //读取数字图片数据
    if(p_trs_pic->clear == FALSE)//透明图标，读回图标后透明叠加
    {
        sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_entry.offset);
        sys_sd_fread(res_ptr->res_fp, display_buffer_tmp, pic_data_len);
        
        //数字图片叠加到背景图片
        pic_overlay(display_buffer, display_buffer_tmp, &back_region, &pic_region, p_trs_pic);
    }

    //显示叠加ok数字图片
    put_image(&back_region);
}

//背景图标按列优先置换为按行优先
void turn_pixel(uint8 *pixel_buffer, region_t *turn_region)
{
    uint16 *pixel_16 = (uint16 *)pixel_buffer;
    uint16 *pixel_tmp = (uint16 *)(pixel_buffer + 512);//大小限定在512字节，即最大图标支持为16*16，32*8等
    uint16 x, y;
    
    for(x = 0; x < turn_region->width; x++)
    {
        for(y = 0; y < turn_region->height; y++)
        {
            pixel_tmp[y*turn_region->width + x] = pixel_16[x*turn_region->height + y];
        }
    }
    
    libc_memcpy(pixel_16, pixel_tmp, 512);
}

//把透明图标叠加到背景区域上
void pic_overlay(uint8 *pixel_result, uint8 *pic_source, region_t *b_region, region_t *p_region, \
transparent_pic_t *p_trs_pic)
{
    uint16 x, y;
    uint16 *pixel_result_16 = (uint16 *)pixel_result;
    uint16 *pic_source_16 = (uint16 *)pic_source;
    
    if(p_trs_pic->direct == 0)//行可能会偏移
    {
        uint16 rel_y = p_region->y - b_region->y;
        
        for(y = 0; y < p_region->height; y++)
        {
            for(x = 0; x < p_region->width; x++)
            {
                if(pic_source_16[y * p_region->width + x] != p_trs_pic->back_color)
                {
                    pixel_result_16[(rel_y + y) *b_region->width + x] = pic_source_16[y * p_region->width + x];
                }
            }
        }
    }
    else//列可能会偏移
    {
        uint16 rel_x = p_region->x - b_region->x;
        
        for(y = 0; y < p_region->height; y++)
        {
            for(x = 0; x < p_region->width; x++)
            {
                if(pic_source_16[y * p_region->width + x] != p_trs_pic->back_color)
                {
                    pixel_result_16[y *b_region->width + (rel_x + x)] = pic_source_16[y * p_region->width + x];
                }
            }
        }
    }
}

//输出图片数据到 lcd 屏
bool put_image(region_t *img_region)
{
    uint32 count;
    bool result = TRUE;

    //设置图片显示模式及窗口
    set_lcd_param_hook(img_region, LCD_PARAM_IMAGE);

    count = ((uint32) img_region->width) * ((uint32) img_region->height) * 2;
    //DMA到GRAM中
    if (dma_buffer_data_hook(count / 2, img_region->width) == FALSE)
    {
        result = FALSE;
    }

    return result;
}

/*! \endcond */
