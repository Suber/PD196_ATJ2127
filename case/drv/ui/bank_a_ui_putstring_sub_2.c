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
 * \file     ui_putstring_sub_2.c
 * \brief    字符串显示阶段 bank c 接口
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
 *    加载当前字符串背景图片，初始化全局结构体 text_bg_desc 
 * \param[in]    id：背景图片ID
 * \param[in]    x：背景图片的X坐标
 * \param[in]    y：背景图片的Y坐标
 * \param[in]    mode：ROW_PRIORITY_DEF 表示行优先，COL_PRIORITY_DEF 表示列优先
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void load_bgpic_sd(uint16 id, uint16 x, uint16 y, uint8 mode)
{
    res_open_infor_t *res_ptr;
    res_entry_t res_entry;
    
    //读取图片资源信息
    res_ptr = res_ptr_select(id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));

    text_bg_desc.back_id = id;
    text_bg_desc.back_x = x;
    text_bg_desc.back_y = y;
    text_bg_desc.width = res_entry.width;
    text_bg_desc.height = res_entry.height;
    if(mode == ROW_PRIORITY_DEF)
    {
        text_bg_desc.offset_col = res_entry.offset;
    }
    else
    {
        text_bg_desc.offset_col = res_entry.offset + (res_entry.width * res_entry.height * 2);
        //4字节对齐
        if((text_bg_desc.offset_col % 4) != 0)
        {
            text_bg_desc.offset_col = (text_bg_desc.offset_col & 0xfffffffc) + 4;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    显示单行完整字符串前对字符串进行修改，可以实现阿拉伯翻转以从左到右显示等功能
 * \param[in]   string_infor：输入字符串描述符
 * \param[in]   param：任意参数
 * \param[out]  string_infor：输出字符串描述符
 * \return      none
 * \note
 *******************************************************************************/
void bankaf_put_string_line_arabic_hook(string_desc_t *string_infor, uint16 disp_mode, uint16 param)
{
    libc_memcpy(arabic_string_buf, string_infor->data.str, (uint32)param);
    //将字符串反转
    arabic_do_turn(arabic_string_buf, 0, param);
    arabic_string_buf[param] = 0;
    arabic_string_buf[param + 1] = 0;
    string_infor->data.str = arabic_string_buf;
    string_infor->length = param;
    //arabic_stat = ARABIC_LINE_TURN; //临时状态，不修改
}

/******************************************************************************/
/*!
 * \par  Description:
 *    在指定的区域上打出一个字符,超出区域部分不显示-读屏方式
 * \param[in]   str 当前字符串指针
 * \param[in]   language 字符语言类型
 * \param[in]   char_region 要求输出的区域
 * \param[in]   disp_mode 字符显示模式
 * \param[in]   start_col 要求从汉字的哪一列开始输出
 * \param[in]   mode CHAR_FILL_SDIMAGE 模式下有效，ROW_PRIORITY_DEF 表示行优先，COL_PRIORITY_DEF 表示列优先
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void char_out_rect(uint8 *str, int8 language, region_t *char_region, uint16 disp_mode, uint8 start_col, uint8 mode)
{
    uint8 end;
    
    if (char_region->width == 0)
    {
        return;
    }
    
    get_next_char_point_hook(str, language, fix_p_char_point);
    
    //CHAR_FILL_SDIMAGE，从SD读取背景，再直接叠上字符串
    if((disp_mode & DISP_CHAR_FILL_OPTION) == CHAR_FILL_SDIMAGE)
    {
        if (need_arabic_deal(disp_mode) == TRUE)
        {
            end = fix_p_char_point[fix_font_size - 1] - 1;
            put_char_sd(fix_p_char_point, char_region, CHAR_DISP_RIGHT2LEFT, end - start_col, mode);
        }
        else
        {
            put_char_sd(fix_p_char_point, char_region, CHAR_DISP_LEFT2RIGHT, start_col, mode);
        }
    }
    else//其他模式
    {
        libc_memcpy(&region, char_region, sizeof(region_t));
    
        if (need_arabic_deal(disp_mode) == TRUE)
        {
            end = fix_p_char_point[fix_font_size - 1] - 1;
#if (USE_MULTI_FONTLIB == 0)
            romf_put_char(fix_p_char_point, disp_mode & DISP_CHAR_FILL_OPTION, CHAR_DISP_RIGHT2LEFT, end - start_col);
#else
            fixf_put_char(fix_p_char_point, disp_mode & DISP_CHAR_FILL_OPTION, CHAR_DISP_RIGHT2LEFT, end - start_col);
#endif
        }
        else
        {
#if (USE_MULTI_FONTLIB == 0)
            romf_put_char(fix_p_char_point, disp_mode & DISP_CHAR_FILL_OPTION, CHAR_DISP_LEFT2RIGHT, start_col);
#else
            fixf_put_char(fix_p_char_point, disp_mode & DISP_CHAR_FILL_OPTION, CHAR_DISP_LEFT2RIGHT, start_col);
#endif
        }
    }
}

/*! \endcond */
