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
 * \file     ui_string_sdimage.c
 * \brief    实现字符串显示
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2012-02-12
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示单行字符串
 * \param[in]    desc：字符串描述符
 * \param[in]    str_region：单行字符串显示区域
 * \param[in]    mode：ROW_PRIORITY_DEF 表示行优先，COL_PRIORITY_DEF 表示列优先
 * \param[out]   none
 * \return       返回字符串显示结束指针
 * \note         1）要求传进来的字符串编码必须是最终用于显示的编码。
 *               2）显示区域是最终用于显示完整字符部分字符串的区域。
 *               3）str_region->height必须为UNICODE_HEIGHT_DEF。
 *               4）desc->result 返回单行字符串显示终止原因。
 *******************************************************************************/
uint8 *put_string_line_sdimage(string_desc_t *desc, region_t *str_region, uint8 mode)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    uint8 *str;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    region_t buf_region;
    uint16 this_width;//本次实际缓冲区字符串宽度
    uint16 total_width;//字符串已显示区域长度
    char_type_e char_type;
    uint8 fontwidth;
    bool draw_over = FALSE;
    bool remain_region_flag = TRUE;
    bool scan_over = FALSE;
    uint8 start_row, end_row;
    uint16 buf_region1_width, buf_region1_height;

    //基本参数初始化
    total_width = 0;
    str = desc->data.str;
    length = desc->length;
    language = desc->language;
    if (language == UNICODELANGUAGE)
    {
        get_next_char_type = get_next_char_type_unicode;
    }
    else
    {
        get_next_char_type = get_next_char_type_ansi;
    }

    //根据缓冲区大小和显示模式，字符串可能会分多次显示
    //每次显示都先缓冲到显示buffer，再一次性DMA到GRAM中，提升显示效率
    while (1)
    {
        if(mode == ROW_PRIORITY_DEF)//行优先，一次缓冲处理即可完成
        {
            start_row = (uint8)(str_region->y - text_bg_desc.back_y);
            end_row = (uint8)(start_row + str_region->height);
            //读字符串背景到缓冲区中
            //计算缓冲区预设区域
            buf_region.x = str_region->x - text_bg_desc.back_x;
            buf_region.y = fix_font_height_actual - end_row;
            buf_region.height = str_region->height;
            buf_region.width = str_region->width;
            remain_region_flag = FALSE;
            
            //设置当前窗口区域
            libc_memcpy(&region, str_region, sizeof(region_t));
        }
        else//列优先
        {
            buf_region1_height = text_bg_desc.height;
            if(buf_region1_height > 0)//for QAC
            {
                if(display_buf_len > (5*1024))
                {
                    display_buf_len = 5*1024;//buffer 最大为5k
                }
                buf_region1_width = ((display_buf_len / 2) / buf_region1_height);
            }
            else
            {
                return NULL;
            }
            if (buf_region1_width > (str_region->width - total_width))//缓冲区最大刚好完整覆盖（剩余）region
            {
                buf_region1_width = (str_region->width - total_width);
                remain_region_flag = FALSE;
            }
    
            //读字符串背景到缓冲区中
            //计算缓冲区预设区域
            buf_region.x = str_region->x - text_bg_desc.back_x;
            buf_region.y = str_region->y - text_bg_desc.back_y;
            buf_region.height = text_bg_desc.height;
            buf_region.width = buf_region1_width;
    
            //读取字符串背景图片，并设窗
            region.x = str_region->x + total_width;
            region.y = text_bg_desc.back_y;
            region.width = buf_region1_width;
            region.height = buf_region1_height;
            read_str_bgpic(display_buffer, &buf_region, CHAR_FILL_SDIMAGE);
            buf_region.x = 0;//指向首列，开始叠加字符串
        }

        //处理一次部分字符串
        this_width = 0;//累计每段实际缓冲区宽度
        while (1)
        {
            scan_string_other:
            //字符串数据已经完毕
            if (length == 0)
            {
                draw_over = TRUE;
                desc->result = STRING_TAIL_LEN_ZERO;
                break;
            }
            //获取下一个字符，分为结束符，换行符和普通字符
            char_type = get_next_char_type(str, length, language, &char_bytes);
            switch(char_type)
            {
            case CHAR_EOF:
            case CHAR_NOPEND://结束符或者不够数据显示
                draw_over = TRUE;
                desc->result = STRING_TAIL_EOF_NOPEND;
                scan_over = TRUE;
                break;

            case CHAR_UNI_FLAG://Unicode 标志符 小端Unicode
                str += 2;
                length -= 2;
                goto scan_string_other;

            case CHAR_LR://换行符
                str += char_bytes;
                draw_over = TRUE;
                desc->result = STRING_TAIL_CHAR_LR;
                scan_over = TRUE;
                break;

            default:
                //do nothing for QAC
                break;
            }
            
            if(scan_over == TRUE)
            {
                break;//退出字符串扫描
            }

            //考虑nor 方案裁减字库或内码字库
            char_bytes = get_next_char_point_hook(str, language, fix_p_char_point);
            fontwidth = fix_p_char_point[fix_font_size - 1];

            //检查显示区域（空间）是否足够显示
            if ((region.x + this_width + fontwidth) > (region_win.x + region_win.width))//超出物理X轴
            {
                draw_over = TRUE;
                desc->result = STRING_TAIL_REGION_ERR;
                break;
            }

            //再检查缓冲区是否够存放（同时也检测了当前 region 窗口是否够存放）
            if ((this_width + fontwidth) > buf_region.width)
            {
                //如果没有剩余 region，即可结束显示处理
                if (remain_region_flag == FALSE)
                {
                    draw_over = TRUE;
                    desc->result = STRING_TAIL_REGION_ERR;
                }
                break;
            }

            //先把字符缓冲到显示缓冲区中
            if(mode == ROW_PRIORITY_DEF)//行优先
            {
                black_char_to_color_sd(fix_p_char_point, &buf_region, CHAR_DISP_NORMAL, 0, start_row, end_row, 0);
            }
            else//列优先
            {
                black_char_to_color_sd(fix_p_char_point, &buf_region, CHAR_DISP_NORMAL,0,0,fix_font_height_actual,1);
            }
            //字符宽度累计
            this_width += fontwidth;
            //坐标移到下一个字的左上角
            buf_region.x += fontwidth;

            //指针递增
            str += char_bytes;
            length -= char_bytes;
        }

        
        if (this_width > 0)
        {
            //只有在有像素点显示时才需要调用下面程序显示
            if (mode == COL_PRIORITY_DEF)//列优先需要显示，行优先在外面显示
            {
                //显示部分字符串
                //设置字符串显示参数
                set_lcd_param_hook(&region, LCD_PARAM_SCROLL);
    
                //传送缓冲区数据到DMA中
                if (dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width) == FALSE)
                {
                    desc->result = STRING_TAIL_OTHER_ERR;
                    break;//dma传输失败
                }
            }

            //累加 total_width
            total_width += this_width;
        }

        //字符串显示结束
        if (draw_over == TRUE)
        {
            if(mode == ROW_PRIORITY_DEF)
            {
                str_region->x += total_width;
                
                //行优先，背景缓冲区已经读取过了，必须在这里处理不完整字符
                if(desc->result == STRING_TAIL_REGION_ERR)
                {
                    region.width = str_region->width - total_width;
                    if(region.width > 0)//有剩余不完整字符，才需要处理
                    {
                        char_out_rect(str, language, str_region, CHAR_FILL_SDIMAGE, 0, 0);
                    }
                }
            }
            else
            {
                //更新 region为剩余显示区域
                region.x = str_region->x + total_width;
                region.y = text_bg_desc.back_y;
                region.width = str_region->width - total_width;
            }
            break;
        }
    }

    return str;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    清除滚屏边界残留像素点
 * \param[in]    remain_region：滚屏边界残留像素点区域
 * \param[out]   none
 * \return       none
 * \note         
 *******************************************************************************/
void clear_remain_pixel_sdimage(region_t *remain_region)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    region_t buf_region;

    //读字符串背景到缓冲区中
    //计算缓冲区预设区域
    buf_region.x = remain_region->x - text_bg_desc.back_x;
    buf_region.y = remain_region->y - text_bg_desc.back_y;
    buf_region.height = text_bg_desc.height;
    buf_region.width = remain_region->width;

    //读取字符串背景图片，并设窗
    libc_memcpy(&region, remain_region, sizeof(region_t));
    region.height = text_bg_desc.height;
    read_str_bgpic(display_buffer, &buf_region, CHAR_FILL_SDIMAGE);

    //设置字符串显示参数
    set_lcd_param_hook(&region, LCD_PARAM_SCROLL);
    //传送缓冲区数据到DMA中
    dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width);
}

/*! \endcond */
