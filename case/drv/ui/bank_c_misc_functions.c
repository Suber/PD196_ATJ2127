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
 * \file     misc_functions.c
 * \brief    其他功能函数，比如清屏，清理 font 缓冲区标志，设置屏幕模式，设置显示buffer，等等
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现了清屏，清理 font 缓冲区标志，设置屏幕模式，设置显示buffer，等功能。
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    清屏
 * \param[in]    clrregion：需要清屏区域的指针,参见region_t定义
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
    例子1: 清除指定区域
    region_t region;
    
    region.x = 0;
    region.y = 10;
    region.width = 20;
    region.height = 40;
    ui_set_backgd_color(0x0000);//黑色
    ui_clear_screen(&region);
    
    例子2: 清全屏
    ui_set_backgd_color(0x0000);//黑色
    ui_clear_screen(NULL);
 * \endcode
 * \note
 *******************************************************************************/
void clear_screen(region_t *clrregion, void *null2, void *null3)
{
//    if (clrregion == NULL)
//    {
//        romf_fill_rect(&region_win, (uint16) (dc.backgd_color & 0xffff));
//    }
//    else//在ui驱动内部，直接使用 romf_fill_rect 接口进行局部区域清屏以减少bank 切换
//    {
//        romf_fill_rect(clrregion, (uint16) (dc.backgd_color & 0xffff));
//    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    清理 font 缓冲区标志
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \note
 *******************************************************************************/
void flush_font_buffer(void *null1, void *null2, void *null3)
{
#ifndef PC
    uint32 *font_cache_buffer = font_cache_addr;
#else
    uint32 *font_cache_buffer = GET_REAL_ADDR(font_cache_addr);
#endif
    if(*font_cache_buffer == FONT_CACHE_MAGIC)
    {
        *font_cache_buffer = 0x0;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置屏幕模式，横屏或竖屏
 * \param[in]    screen_mode：需要设置的屏幕模式，0表示竖屏，1表示横屏
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
    例子1: 设置为默认模式，即竖屏模式
    ui_set_screen_direction(0);
    
    例子2: 设置为旋转模式，即横屏模式
    ui_set_screen_direction(1);
 * \endcode
 * \note
 *******************************************************************************/
void set_screen_direction(uint8 screen_mode, void *null2, void *null3)
{
    if (screen_mode == 1)
    {
        rotate_flag = TRUE;
        region_win.x = 0;
        region_win.y = 0;
        region_win.width = DISPLAY_HEIGHT;
        region_win.height = DISPLAY_LENGTH;
    }
    else
    {
        rotate_flag = FALSE;
        region_win.x = 0;
        region_win.y = 0;
        region_win.width = DISPLAY_LENGTH;
        region_win.height = DISPLAY_HEIGHT;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置显示buffer
 * \param[in]    LCD_RAM_INDEX表示UI 1K buffer,JPEG_RAM_INDEX表示JPEG 8K buffer
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
    例子1: 设置UI 1K buffer
    ui_set_display_buf(LCD_RAM_INDEX);
    
    例子2: 设置JPEG 8K buffer
    ui_set_display_buf(JPEG_RAM_INDEX);
 * \endcode
 * \note
 *******************************************************************************/
void set_display_buf(uint8 mode, void *null2, void *null3)
{
    if (mode == JPEG_RAM_INDEX)
    {
        display_buf_addr = JPEG_BUF_ADDR;
        display_buf_len = JPEG_BUF_LEN;
        lcd_buffer_type = JPEG_RAM_INDEX;
        font_cache_addr = JPEG_BUF_ADDR + JPEG_BUF_LEN;//在6KB buffer后1KB/2KB
        font_cache_len = 2048;
        //头部大小：选择固化代码默认字库时，1k缓冲区为64字节，2k缓冲区为122字节
        font_cache_header = ((font_cache_len - 6) / (MAX_CHARPOINTE_SIZE + 2)) * 2 + 6;
        font_cache_flag = TRUE;
        lcd_mcu_set_JRAM_clk();
    }
    else
    {
        display_buf_addr = LCD_BUF_ADDR;
        display_buf_len = LCD_BUF_LEN;
        lcd_buffer_type = LCD_RAM_INDEX;
        //lcd_restore_JRAM_clk();
        font_cache_flag = FALSE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置显示buffer
 * \param[in]    mode 详见 display.h 中 语言特殊选项 说明
 * \param[out]   none
 * \return       none
 * \ingroup      settings
 * \par          exmaple code
 * \code
    例子1: 设置进入语言列表菜单
    ui_set_lang_option(LANG_LIST_MENU_ENTER);
    
    例子2: 设置退出语言列表菜单
    ui_set_lang_option(LANG_LIST_MENU_EXIT);
 * \endcode
 * \note
 *******************************************************************************/
void set_lang_option(uint16 mode, void *null2, void *null3)
{
    lang_option = mode;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    读取SD区图片资源保存到buffer中
 * \param[in]    id：要读取的图片资源id
 * \param[out]   buffer:保存图片资源数据的buffer
 * \param[out]   res_size:保存图片资源尺寸信息的结构体指针
 * \return       none
 * \ingroup      picture
 * \note
 *******************************************************************************/
void read_pic_to_buffer(uint16 id, uint8 *buffer, res_size_t *res_size)
{
    res_open_infor_t *res_ptr;
    res_entry_t res_entry;

    //读取图片资源信息
    res_ptr = res_ptr_select(id);
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(id) * RESHEADITEM));
    sys_sd_fread(res_ptr->res_fp, &res_entry, sizeof(res_entry_t));

    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, res_entry.offset);
    sys_sd_fread(res_ptr->res_fp, buffer, res_entry.length);

    res_size->width = res_entry.width;
    res_size->height = res_entry.height;
}
//modify--vzhan
/******************************************************************************/
/*!
 * \par  Description:
 *    显示单行字符串
 * \param[in]    desc：字符串描述符
 * \param[in]    str_region：单行字符串显示区域
 * \param[in]    mode：字符填充模式
 * \param[out]   none
 * \return       返回字符串显示结束指针
 * \note         1）要求传进来的字符串编码必须是最终用于显示的编码。
 *               2）显示区域是最终用于显示完整字符部分字符串的区域。
 *               3）str_region->height必须为UNICODE_HEIGHT_DEF。
 *               4）desc->result 返回单行字符串显示终止原因。
 *******************************************************************************/
uint8 *fixft_put_string_line(string_desc_t *desc, region_t *str_region, char_fill_mode_e mode)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
    uint8 *str;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    char_type_e char_type;
    uint8 fontwidth;

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

    //直接打点模式
    libc_memcpy(&region, str_region, sizeof(region_t));

    //逐个字符处理整串字符串
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
            continue;
        }
        else if (char_type == CHAR_LR)//换行符
        {
            str += char_bytes;
            desc->result = STRING_TAIL_CHAR_LR;
            break;
        }
        else
        {
            //do nothing for QAC
        }

        //考虑nor 方案裁减字库或内码字库
        char_bytes = get_next_char_point_hook(str, language, char_point_buffer);
        fontwidth = char_point_buffer[32];

        //检查显示区域（空间）是否足够显示 
        if ((fontwidth > region.width)//当前字符显示超出 region 窗口
         || ((region.x + fontwidth) > (region_win.x + region_win.width)))//当前字符显示超出物理X轴
        {
            desc->result = STRING_TAIL_REGION_ERR;
            break;
        }

        //直接打点模式，当LCD无法读屏时使用
        char_fill(char_point_buffer, CHAR_DISP_NORMAL, 0);

        //指针递增
        str += char_bytes;
        length -= char_bytes;
    }

    return str;
}


/*! \endcond */
