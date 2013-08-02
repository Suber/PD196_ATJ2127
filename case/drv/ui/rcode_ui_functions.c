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
 * \file     ui_functions.c
 * \brief    处理屏幕坐标信息的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               设置lcd相关的一些参数
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *     获取当前位置字符类型--UNICODE编码
 * \param[in]    str：当前字符串指针
 * \param[in]    language：字符串编码语言类型
 * \param[out]   byte_cnt：传回当前有效字符的字节数
 * \return       char_type_e：返回字符类型
 * \note         这里的字符是一个更丰富的概念，定义为完整显示的字符单元。
 *******************************************************************************/
char_type_e ramf_next_char_unicode_hook(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    *byte_cnt = 2;
    return CHAR_NORMAL;
}
/******************************************************************************/
/*!
 * \par  Description:
 *     获取当前位置字符类型--内码编码
 * \param[in]    str：当前字符串指针
 * \param[in]    language：字符串编码语言类型
 * \param[out]   byte_cnt：传回当前有效字符的字节数
 * \return       char_type_e：返回字符类型
 * \note         这里的字符是一个更丰富的概念，定义为完整显示的字符单元。
 *******************************************************************************/
char_type_e ramf_next_char_ansi_hook(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    if((*str >= 0x80) && (language_2byte == TRUE))
    {
        if(length < 2)//数据不足，返回 NOPEND
        {
            return CHAR_NOPEND;
        }
        else if(*(str + 1) == 0)//内容错误，返回 NOPEND，适用于有结束符但结束符前2字节字符不完整情况
        {
            return CHAR_NOPEND;
        }
        else
        {
            *byte_cnt = 2;
        }
    }
    else
    {
        *byte_cnt = 1;
    }
    return CHAR_NORMAL;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    获取当前字符的点阵
 * \param[in]   str:当前字符指针
 * \param[in]   language：当前字符编码类型
 * \param[out]  pbuffer:保存字符点阵的buffer
 * \return      当前字符的字节数，2字节或1字节
 * \note
 *******************************************************************************/
uint8 ramf_get_next_char_point_hook(uint8 *str, int8 language, uint8 *pbuffer)
{
    uint16 uni_code = 0;
    uint8 pt_step;

    if ((language == UNICODELANGUAGE) || ((*str> ASCIIDISCODE) && (language_2byte == TRUE)))
    {
        uni_code = *str + ((uint16) (*(str + 1)) << 8);
        pt_step = 2;
    }
    else
    {
        uni_code = *str;
        pt_step = 1;
    }

    //转换为Unicode编码，Unicode语言和小于0x80的内码无须转换
    if ((language != UNICODELANGUAGE) && (uni_code >= ASCIIDISCODE))
    {
        uni_code = romf_multi_char_to_unicode(uni_code);
    }
    //add for thai languange show by liutingting
    if ((uni_code >= 0x0E01) && (uni_code <= 0x0E7F))
    {
        thai_point(str, language, uni_code, pbuffer, &pt_step);
    }
    else
    {
#if (USE_MULTI_FONTLIB == 0)
        romf_get_next_char_point(uni_code, pbuffer);
#else
        fixf_get_next_char_point(uni_code, pbuffer);
#endif
    }
    return pt_step;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    读取指定区域的屏幕背景
 * \param[in]   bgpic_region:要读取的区域信息
 * \param[in]   mode：字符显示模式
 * \param[out]  display_buffer:保存屏幕数据的buffer
 * \return      none
 * \note
 *******************************************************************************/
void ramf_read_str_bgpic_hook(uint8 *display_buffer, region_t *bgpic_region, char_fill_mode_e mode)
{
/*    //modify--vzhan
    uint32 pixel_cnt, pixel_offset;
    display_buffer_deal();
    //从GRAM读取字符串背景
    if (mode == CHAR_FILL_NORMAL)
    {
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

        //设窗,读取背景
        lcd_set_window(bgpic_region);

        //读取GRAM点阵
        pixel_cnt = (uint32) bgpic_region->width * bgpic_region->height;
        lcd_get_buff_data((uint8 *) display_buffer, pixel_cnt);
    }
    //从flash（SD区，要求列优先存放）读取字符串背景
    else if (mode == CHAR_FILL_SDIMAGE)
    {
        res_open_infor_t *res_ptr = res_ptr_select(text_bg_desc.back_id);
    
        pixel_cnt = (uint32) bgpic_region->width * bgpic_region->height * 2;
        //注：行优先读取背景时，x必须为0，当前应用满足该条件
        pixel_offset = (uint32) bgpic_region->height * bgpic_region->x * 2;
        sys_sd_fseek(res_ptr->res_fp, SEEK_SET, text_bg_desc.offset_col + pixel_offset);
        sys_sd_fread(res_ptr->res_fp, display_buffer, pixel_cnt);
    }
    else
    {
        //for QAC
    }*/
    //CHAR_FILL_COLOR 已经在romf_read_str_bgpic处理完
}
/******************************************************************************/
/*!
 * \par  Description:
 *    设置LCD的模式和显示窗口
 * \param[in]   win_region:要显示的区域信息
 * \param[in]   mode：数据类型，0表示图片，1表示字符串
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void ramf_set_lcd_param_hook(region_t *win_region, uint8 mode)
{
    //重新初始化DMA为RGB模式
    lcd_controller_init(RGB_MODE);
#ifndef PC
    if (rotate_flag == FALSE)
    {
        if(mode == LCD_PARAM_SCROLL)
        {
            lcd_set_draw_mode(DRAW_MODE_H_SCROLL_DEF); //设置竖屏字符串滚屏模式
        }
        else
        {
            lcd_set_draw_mode(DRAW_MODE_H_DEF); //设置竖屏模式
        }
    }
    else
    {
        if(mode == LCD_PARAM_SCROLL)
        {
    	    lcd_set_draw_mode(DRAW_MODE_V_SCROLL_DEF); //设置横屏字符串滚屏模式
    	}
    	else
    	{
            lcd_set_draw_mode(DRAW_MODE_V_DEF); //设置横屏模式
        }
    }
#else
    if(mode == LCD_PARAM_SCROLL)
    {
        lcd_set_draw_mode(DRAW_MODE_H_SCROLL_DEF); //设置竖屏字符串滚屏模式
    }
    else
    {
        lcd_set_draw_mode(DRAW_MODE_H_DEF); //设置竖屏模式
    }
#endif
    
    lcd_set_window(win_region);
    display_buffer_deal();
}
/******************************************************************************/
/*!
 * \par  Description:
 *    将显示buffer中的数据通过DMA送屏
 * \param[in]   pixel_cnt:要显示的像素点的个数
 * \param[in]   data_width：每行的像素点宽度
 * \param[out]  none
 * \return      DMA传输是否成功，成功返回TRUE，失败返回FALSE
 * \note
 *******************************************************************************/
bool ramf_dma_buffer_data_hook(uint32 pixel_cnt, uint16 data_width)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
	lcd_buff_data_trans(display_buffer,0);
	return TRUE;

//    //DMA到GRAM中
//    lcd_dma_set_counter(pixel_cnt, data_width);
//    //开始DMA传输
//    return (bool) (uint32) lcd_dma_start_trans(lcd_buffer_type);
}
/******************************************************************************/
/*!
 * \par  Description:
 *    在指定的区域上打出一个像素点
 * \param[in]   point_region:像素点的输出区域
 * \param[in]   trans_font：要输出的像素点的数据
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void ramf_char_fill_hook(region_t *point_region, uint8 *trans_font)
{
    lcd_set_window(point_region);
    lcd_buff_data_trans(trans_font, 1);
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
void ramf_put_string_line_hook(string_desc_t *string_infor, uint16 disp_mode, uint16 param)
{
    if ((need_arabic_deal(disp_mode) == TRUE) && (arabic_stat == ARABIC_ASCII_TURN))
    {
        bankaf_put_string_line_arabic_hook(string_infor, disp_mode, param);
    }
}

/*! \endcond */

/*! \cond */

void display_buffer_deal(void)
{
    if (lcd_buffer_type == JPEG_RAM_INDEX)
    {
#ifndef PC
        if (sys_get_drv_install_info(DRV_GROUP_STG_BASE) > 0)
        {
            base_op_entry(1, 0, 0, BASE_UPDATE);
        }
#endif
        //lcd_mcu_set_JRAM_clk();
    }

}
//modify--vzhan
bool u_show_pic(uint16 id, uint16 x, uint16 y)
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
    return u_put_sd_image(res_ptr->res_fp, res_entry.offset, &res_region);
}

/*! \endcond */
