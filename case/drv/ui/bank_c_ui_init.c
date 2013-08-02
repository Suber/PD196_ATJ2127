/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>           <time>          <version >             <desc>
 *       liutingting     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_init.c
 * \brief    UI初始化模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现UI显示模块的初始化。
 * \par      EXTERNALIZED FUNCTIONS:
 *              调用语言设置和显示buffer设置函数实现语言和显示buffer初始化，并打开字库。
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#if (USE_MULTI_FONTLIB == 0)
const uint8 FontLibName[] = "UNICODE.FON";
#else
const uint8 FontLib_S_Name[] = "UNICODES.FON";
const uint8 FontLib_Name[] = "UNICODE.FON";
const uint8 FontLib_L_Name[] = "UNICODEL.FON";
#endif
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    UI驱动的初始化
 * \param[in]   none
 * \param[out]  none
 * \return      bool类型的参数，是否成功
 * \ingroup     install
 * \note
 *******************************************************************************/
int ui_init(void *null0, void*null1, void*null2)
{
#if (USE_MULTI_FONTLIB == 0)
    FontLib_fp = sys_sd_fopen(FontLibName);
    fix_font_height = 16;
    fix_font_height_actual = 16;
    fix_font_height_up_margin = 0;
    fix_font_height_line = 20;
    fix_font_width = 16;
    fix_font_size = 33;
    fix_p_char_point = char_point_buffer;
#else

#if ((USE_MULTI_FONTLIB & 0x01) != 0)
    //小字体字库
    FontLib_fp = sys_sd_fopen(FontLib_S_Name);
    libc_memcpy(&fix_font_files[0], FontLib_fp, sizeof(sd_file_t));
    sys_sd_fclose(FontLib_fp);
#endif
    
#if ((USE_MULTI_FONTLIB & 0x04) != 0)
    //大字体字库
    FontLib_fp = sys_sd_fopen(FontLib_L_Name);
    libc_memcpy(&fix_font_files[2], FontLib_fp, sizeof(sd_file_t));
    sys_sd_fclose(FontLib_fp);
#endif
    
    //中字体字库
    FontLib_fp = sys_sd_fopen(FontLib_Name);
    libc_memcpy(&fix_font_files[1], FontLib_fp, sizeof(sd_file_t));
    //默认配置为中字体字库
    fix_current_font = 1;
    fix_font_height = FONTLIB_2_HEIGHT;
    fix_font_height_actual = FONTLIB_2_HEIGHT_ACTUAL;
    fix_font_height_up_margin = (fix_font_height - fix_font_height_actual)/2;
    fix_font_height_line = fix_font_height_actual + (fix_font_height_actual/4);
    fix_font_width = FONTLIB_2_WIDTH;
    fix_font_size = FONTLIB_2_SIZE;
    fix_p_char_point = fix_char_point_buffer;
#endif
    //set_language(ENGLISH, 0, 0); //默认语言设置
    set_display_buf(JPEG_RAM_INDEX, 0, 0);//设置为 JPEG_RAM 模式

    //变量赋初值
    g_ap_res.res_fp = NULL;
    g_com_res.res_fp = NULL;
    rotate_flag = FALSE; //初始化为竖屏模式
    lang_option = 0;
    region_win.x = 0;
    region_win.y = 0;
    region_win.width = DISPLAY_LENGTH;
    region_win.height = DISPLAY_HEIGHT;

    char_fill_hook = ramf_char_fill_hook;
    dma_buffer_data_hook = ramf_dma_buffer_data_hook;
    read_str_bgpic_hook = ramf_read_str_bgpic_hook;
    get_next_char_point_hook = ramf_get_next_char_point_hook;
    next_char_ansi_hook = ramf_next_char_ansi_hook;
    next_char_unicode_hook = ramf_next_char_unicode_hook;
    set_lcd_param_hook = ramf_set_lcd_param_hook;

    put_sd_image = romf_put_sd_image;
    black_char_to_color = romf_black_char_to_color;
    char_fill = romf_char_fill;
    get_next_char_type_unicode = romf_get_next_char_type_unicode;
    get_next_char_type_ansi = romf_get_next_char_type_ansi;
    search_cache = romf_search_cache;
    put_char = romf_put_char;
    read_str_bgpic = romf_read_str_bgpic;

#ifndef PC
    arabic_string_buf = 0;
    if(sys_malloc(&arabic_string_buf, STRING_BUFFER_LEN) == -1)//申请空间失败
    {
#ifdef UI_DEBUG
        bool loop = TRUE;
        while (1)//陷入死循环
        {
            loop = TRUE;
        }
#endif
    }
#endif

    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    UI驱动的退出
 * \param[in]   none
 * \param[out]  none
 * \return      none
 * \ingroup     install
 * \note
 *******************************************************************************/
int ui_exit(void *null0, void *null1, void *null2)
{
    sys_sd_fclose(FontLib_fp); //关闭字库文件
    sys_sd_fclose(MBtoUniTab_fp); //关闭多国语言转换表

#ifndef PC
    sys_free(&arabic_string_buf);
#endif

    return 0;
}
/*! \endcond */

#ifndef PC
module_init(ui_init)
module_exit(ui_exit)
#endif
