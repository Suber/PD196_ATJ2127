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
 * \file     fix_ui_string_2.c
 * \brief    固化代码修复
 * \author   cailizhen
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date     2012-1-12
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond UI_DRIVER_API */

#if (USE_MULTI_FONTLIB != 0)
/******************************************************************************/
/*!
 * \par  Description:
 *    获取字符串长度
 * \param[in]    desc：字符串描述符
 * \param[in]    max_length：字符串最大长度，当扫描到字符串长度大于该长度则终止返回
 * \param[out]   result：附带结果指针，可以为空
 * \return       字符串长度（像素点）
 * \ingroup      string
 * \note         1）阿拉伯必须连写但无需逆序。
 *               2）desc->result 返回计算字符串长度终止原因。
 *******************************************************************************/
uint16 fixf_get_string_length(string_desc_t *desc, uint16 max_length, str_getlen_result_t *result)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
    uint8 *str;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    uint16 str_width = 0;//字符串长度（像素点）
    char_type_e char_type;

    //基本参数初始化
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
    if (result != NULL)
    {
        result->true_length = 0;
        result->true_bytes = 0;
    }

    //单行字符串获取字符串长度
    while (1)
    {
        //字符串数据已经完毕
        if (length == 0)
        {
            desc->result = STRING_TAIL_LEN_ZERO;
            break;
        }
        //获取下一个字符，分为结束符，换行符和普通字符
        char_type = get_next_char_type(str, length, language, &char_bytes);
        if ((char_type == CHAR_EOF) || (char_type == CHAR_NOPEND))//结束符或者不够数据显示
        {
            desc->result = STRING_TAIL_EOF_NOPEND;
            break;
        }
        else if (char_type == CHAR_UNI_FLAG)
        {
            str += 2;
            length -= 2;
            if (result != NULL)
            {
                result->true_bytes += 2;
            }
            continue;
        }
        else if (char_type == CHAR_LR)//换行符
        {
            desc->result = STRING_TAIL_CHAR_LR;
            break;
        }
        else
        {
            //do nothing for QAC
        }

        //考虑nor 方案裁减字库或内码字库
        char_bytes = get_next_char_point_hook(str, language, fix_p_char_point);

        //字符串宽度累计
        str_width += fix_p_char_point[fix_font_size - 1];
        if (str_width > max_length)//已经超过最大长度，无需再计算
        {
            desc->result = STRING_TAIL_REGION_ERR;
            break;
        }

        //指针递增
        str += char_bytes;
        length -= char_bytes;

        if (result != NULL)
        {
            result->true_length += fix_p_char_point[fix_font_size - 1];
            result->true_bytes += char_bytes;
        }
    }

    return str_width;
}

//切换字体字库
void fix_switch_fontlib(uint16 disp_mode)
{
#ifndef PC
    uint32 *font_cache_buffer = font_cache_addr;
#else
    uint32 *font_cache_buffer = GET_REAL_ADDR(font_cache_addr);
#endif

#if ((USE_MULTI_FONTLIB & 0x01) != 0)
    if((disp_mode & DISP_FONT_OPTION) == DISP_FONT_SMALL)
    {
        //切换到小字体字库
        if(fix_current_font != 0)
        {
            libc_memcpy(FontLib_fp, &fix_font_files[0], sizeof(sd_file_t));
            fix_current_font = 0;
            fix_font_height = FONTLIB_1_HEIGHT;
            fix_font_height_actual = FONTLIB_1_HEIGHT_ACTUAL;
            fix_font_height_up_margin = (fix_font_height - fix_font_height_actual)/2;
            fix_font_height_line = fix_font_height_actual + (fix_font_height_actual/4);
            fix_font_width = FONTLIB_1_WIDTH;
            fix_font_size = FONTLIB_1_SIZE;
            *font_cache_buffer = 0x0;//清除font缓冲区
        }
        return ;
    }
#endif

#if ((USE_MULTI_FONTLIB & 0x04) != 0)
    if((disp_mode & DISP_FONT_OPTION) == DISP_FONT_LARGE)
    {
        //切换到大字体字库
        if(fix_current_font != 2)
        {
            libc_memcpy(FontLib_fp, &fix_font_files[2], sizeof(sd_file_t));
            fix_current_font = 2;
            fix_font_height = FONTLIB_3_HEIGHT;
            fix_font_height_actual = FONTLIB_3_HEIGHT_ACTUAL;
            fix_font_height_up_margin = (fix_font_height - fix_font_height_actual)/2;
            fix_font_height_line = fix_font_height_actual + (fix_font_height_actual/4);
            fix_font_width = FONTLIB_3_WIDTH;
            fix_font_size = FONTLIB_3_SIZE;
            *font_cache_buffer = 0x0;//清除font缓冲区
        }
        return ;
    }
#endif

    //切换到中字体字库
    if(fix_current_font != 1)
    {
        libc_memcpy(FontLib_fp, &fix_font_files[1], sizeof(sd_file_t));
        fix_current_font = 1;
        fix_font_height = FONTLIB_2_HEIGHT;
        fix_font_height_actual = FONTLIB_2_HEIGHT_ACTUAL;
        fix_font_height_up_margin = (fix_font_height - fix_font_height_actual)/2;
        fix_font_height_line = fix_font_height_actual + (fix_font_height_actual/4);
        fix_font_width = FONTLIB_2_WIDTH;
        fix_font_size = FONTLIB_2_SIZE;
        *font_cache_buffer = 0x0;//清除font缓冲区
    }
    return ;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    显示单个字符，支持不完整字符显示
 * \param[in]    psoure_buf：字符字模缓冲区，33 个字节（16*16，buf[32]表示宽度）
 * \param[in]    mode：字符填充模式
 * \param[in]    char_disp：字符（字模）显示模式
 * \param[in]    start_col：字符（字模）显示开始位置
 * \param[out]   none
 * \return       none
 * \note         1）start_col 不能大于等于字符宽度。
 *               2）必须在外面把 char_region 拷贝到 全局 region 中。
 *******************************************************************************/
bool fixf_put_char(uint8 *psoure_buf, char_fill_mode_e mode, char_disp_e char_disp, uint8 start_col)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    region_t buf_region;
    
    //直接打点模式，当LCD无法读屏时使用
    if (mode == CHAR_FILL_POINT)
    {
        fixf_char_fill(psoure_buf, char_disp, start_col);
    }
    //先把字符缓冲到显示缓冲区中，再DMA显示出来
    else
    {
        //先把字符缓冲到显示缓冲区中，再DMA显示出来
        //计算缓冲区预设区域
        buf_region.x = 0;
        buf_region.y = 0;
        buf_region.height = fix_font_height_actual;
        buf_region.width = region.width;
    
        //读取字符串背景图片，并设窗
        read_str_bgpic(display_buffer, &region, mode);
    
        fixf_black_char_to_color(psoure_buf, &buf_region, char_disp, start_col);
        //设置字符串显示参数
        set_lcd_param_hook(&region, LCD_PARAM_STRING);
        //传送缓冲区数据到DMA中
        if (dma_buffer_data_hook(buf_region.width * buf_region.height, buf_region.width) == FALSE)
        {
            return FALSE;//dma传输失败
        }
    }

    return TRUE;
}
#endif
    
/*! \endcond */
