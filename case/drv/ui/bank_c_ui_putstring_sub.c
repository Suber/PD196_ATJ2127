/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       cailizhen     2012-10-16 9:27     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_putstring_sub.c
 * \brief    字符串处理阶段 bank c 接口
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *           
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-10-16
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 *    显示单个字符，支持不完整字符显示
 * \param[in]    psoure_buf：字符字模缓冲区，33 个字节（16*16，buf[32]表示宽度）
 * \param[in]    mode：字符填充模式
 * \param[in]    char_disp：字符（字模）显示模式
 * \param[in]    start_col：字符（字模）显示开始位置
 * \param[in]    mode：ROW_PRIORITY_DEF 表示行优先，COL_PRIORITY_DEF 表示列优先
 * \param[out]   none
 * \return       none
 * \note         1）start_col 不能大于等于字符宽度。
 *               2）必须在外面把 char_region 拷贝到 全局 region 中。
 *******************************************************************************/
void put_char_sd(uint8 *psoure_buf, region_t *char_region, char_disp_e char_disp, uint8 start_col, uint8 mode)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    region_t buf_region;
    uint8 start_row, end_row;
    
    if(mode == ROW_PRIORITY_DEF)
    {
        start_row = (uint8)(char_region->y - text_bg_desc.back_y);
        end_row = (uint8)(start_row + char_region->height);
        //计算缓冲区预设区域
        buf_region.x = char_region->x - region.x;
        buf_region.y = fix_font_height_actual - end_row;
        buf_region.height = char_region->height;
        buf_region.width = char_region->width;
        
        black_char_to_color_sd(psoure_buf, &buf_region, char_disp, start_col, start_row, end_row, mode);
    }
    else
    {
        //计算缓冲区预设区域
        buf_region.x = char_region->x - text_bg_desc.back_x;
        buf_region.y = char_region->y - text_bg_desc.back_y;
        buf_region.height = text_bg_desc.height;
        buf_region.width = char_region->width;
        
        region.x = char_region->x;
        region.y = text_bg_desc.back_y;
        region.width = char_region->width;
        region.height = text_bg_desc.height;
        
        //读取字符串背景图片，并设窗
        read_str_bgpic(display_buffer, &buf_region, CHAR_FILL_SDIMAGE);
        buf_region.x = 0;//指向首列，开始叠加字符串
        
        black_char_to_color_sd(psoure_buf, &buf_region, char_disp, start_col, 0, fix_font_height_actual, mode);
        
        //设置字符串显示参数
        set_lcd_param_hook(&region, LCD_PARAM_SCROLL);
        //传送缓冲区数据到DMA中
        if (dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width) == FALSE)
        {
            return FALSE;//dma传输失败
        }
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 *    字符从黑白转换为彩色，保存到缓冲区中
 * \param[in]    psoure_buf：字符字模缓冲区，33 个字节（16*16，buf[32]表示宽度）
 * \param[in]    buf_region：显示缓冲区预设区域（高度小于height_max）
 *               （缓冲区被划分为宽度为width，高度为height_max + buffer_len / width的区域）
 * \param[in]    char_disp：字符（字模）显示模式
 * \param[in]    start_col：字符（字模）显示开始位置
 * \param[in]    start_row：字符起始行，用于垂直滚屏
 * \param[in]    end_row：字符结束行，用于垂直滚屏
 * \param[in]    mode：ROW_PRIORITY_DEF 表示行优先，COL_PRIORITY_DEF 表示列优先
 * \param[out]   none
 * \return       none
 * \note         start_col 不能大于等于字符宽度
 *******************************************************************************/
void black_char_to_color_sd(uint8 *psoure_buf, region_t *buf_region, char_disp_e char_disp, \
        uint8 start_col, uint8 start_row, uint8 end_row, uint8 mode)
{
#ifndef PC
    uint16 *display_buffer_base = (uint16 *) display_buf_addr;
#else
    uint16 *display_buffer_base = GET_REAL_ADDR(display_buf_addr);
#endif
    uint16 *display_buffer;//缓冲区为16bit，符合565小彩屏
    uint16 pen_color = (uint16) (dc.pen_color & 0xffff);
    uint8 i, j, k;
    uint8 src_data;//0表示空白点，1表示字点
    uint8 temp_width;//1表示16*8字模，2表示16×16字模
    uint8 font_width;
    uint8 real_start, real_end;
    uint8 shift_mask;
    uint8 index;

    //初始基础参数
    font_width = psoure_buf[fix_font_size - 1];
    temp_width = ((font_width + 7) / 8);
    
    //确定处理区间
    if (char_disp == CHAR_DISP_NORMAL)
    {
        real_start = 0;
        real_end = font_width - 1;
    }
    else if (char_disp == CHAR_DISP_LEFT2RIGHT)
    {
        real_start = start_col;
        real_end = start_col + (uint8)(region.width - 1);
    }
    else//CHAR_DISP_RIGHT2LEFT
    {
        real_start = start_col - (uint8)(region.width - 1);
        real_end = start_col;
    }
    
    if(mode == ROW_PRIORITY_DEF)
    {
        for (i = start_row, k = (uint8)buf_region->y; i < end_row; i++, k++)//loop for height
        {
            display_buffer = display_buffer_base + (k * buf_region->width + buf_region->x);
            index = ((i + fix_font_height_up_margin) * temp_width) + (real_start/8);
            shift_mask = (uint8)(0x80 >> (real_start % 8));
            for (j = real_start; j <= real_end; j++)//loop for char width
            {
                if ((j % 8) == 0)
                {
                    index = ((i + fix_font_height_up_margin) * temp_width) + (j/8);
                    shift_mask = 0x80;
                }

                src_data = (uint8)(psoure_buf[index] & (shift_mask));
                shift_mask >>= 1;
    
                //空白点处理
                if (src_data == 0)
                {
                    //跳过无需转换
                    display_buffer++;
                }
                //字点处理
                else
                {
                    //将画笔颜色填入
                    *display_buffer = pen_color;
                    display_buffer++;
                }
            }
        }
    }
    else
    {
        for (j = real_start, k = (uint8)buf_region->x; j <= real_end; j++, k++)//loop for char width
        {
            display_buffer = display_buffer_base + (k * buf_region->height + buf_region->y);
            shift_mask = (uint8)(0x80 >> (j%8));	
            index = ((start_row + fix_font_height_up_margin) * temp_width) + (j/8);
            for (i = start_row; i < end_row; i++)//loop for height
            {
                src_data = (uint8)(psoure_buf[index] & shift_mask);
                index += temp_width;
    
                //空白点处理
                if (src_data == 0)
                {
                    //跳过无需转换
                    display_buffer++;
                }
                //字点处理
                else
                {
                    //将画笔颜色填入
                    *display_buffer = pen_color;
                    display_buffer++;
                }
            }
        }
    }
}

#if (USE_MULTI_FONTLIB != 0)
/******************************************************************************/
/*!
 * \par  Description:
 *    获取指定Unicode 编码字符字模
 * \param[in]    uni_code：Unicode 字符编码
 * \param[in]    pbuffer：字符字模缓冲区
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void fixf_get_next_char_point(uint16 uni_code, uint8 *pbuffer)
{
    uint8 cache_index;

    //获取字符点阵
    //有字模缓冲区，可用缓存机制减少读取字模次数
    //固化代码 for QAC：if(font_cache_flag) 等价于 if(font_cache_flag != FALSE) 而不等价于 if(font_cache_flag == TRUE)
    if (font_cache_flag != FALSE)
    {
        //找到缓存字模，直接读取
        if (search_cache(uni_code, &cache_index) == TRUE)
        {
            libc_memcpy(pbuffer, fix_font_cache_char_addr(cache_index), fix_font_size);
        }
        //没找到缓存字模，从字库文件读取并填充到字模缓存中
        else
        {
            fixf_get_font_point(uni_code, pbuffer);
            libc_memcpy(fix_font_cache_char_addr(cache_index), pbuffer, fix_font_size);
        }
    }
    //没有字模缓冲区，每次都需要直接读取字模
    else
    {
        fixf_get_font_point(uni_code, pbuffer);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    从字库文件中读取字符的点阵
 * \param[in]    code_rename：字符码值
 * \param[out]   picbuf：保存字符点阵的buffer地址
 * \return       none
 * \note
 *******************************************************************************/
void fixf_get_font_point(uint16 code_rename, uint8 *picbuf)
{
    sys_sd_fseek(FontLib_fp, SEEK_SET, (uint32) (code_rename * fix_font_size));
    sys_sd_fread(FontLib_fp, picbuf, fix_font_size);
}
#endif

/*! \endcond */
