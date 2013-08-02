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
 * \file     ui_scroll_string_ext.c
 * \brief    ID3滚屏专用模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现ID3滚屏
 * \par      EXTERNALIZED FUNCTIONS:
 *               none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    根据ID3的内容，实现ID3的横向和竖向滚屏
 * \param[in]   scroll_string_infor：ID3滚屏信息结构体指针
 * \param[in]   scroll_param：滚屏参数结构体变量
 * \param[in]   style_infor： 指向要滚屏的textbox的style结构体变量
 * \param[out]  none
 * \return      滚屏是否完成
 * \retval      TRUE 初始化或未滚完
 * \retval      FALSE 已滚完
 * \ingroup     string
 * \par         exmaple code
 * \code
    bool result;
    scroll_param_t scroll_param;
    style_infor_t style_infor, style_icon;
    
    style_infor.style_id = ID3_SCROLL_TEXTBOX;
    style_infor.type = UI_AP;
    style_icon.style_id = ID3_MUSIC_PICBOX;
    style_icon.type = UI_AP;
    scroll_param.direct = g_scroll_vars.direct;
    scroll_param.init = g_scroll_vars.init;
    scroll_param.active = g_scroll_vars.index;
    scroll_param.id3_text = &style_infor;
    scroll_param.id3_icon = &style_icon;
    result = ui_scroll_string_ext(g_scroll_content, &scroll_param);
 * \endcode
 * \note
 *******************************************************************************/
bool scroll_string_ext(scroll_content_t *scroll_string_infor, scroll_param_t *scroll_param, void *null3)
{
    textbox_private_t textbox_data;
    picbox_private_t picbox_data;
    bool result = 0;
    uint8 index = (uint8) scroll_param->active;
    
    //ID3滚屏显示初始化
    if (scroll_param->init == TRUE)
    {
        if(scroll_param->update == TRUE)
        {
            picbox_data.pic_id = -1;
            picbox_data.frame_id = index+1;//帧0为背景图，帧1为title
            show_picbox_id3(scroll_param->id3_icon, &picbox_data, 0);
        }

        textbox_data.str_id = (uint16) - 1;
        textbox_data.lang_id = scroll_string_infor[index].language;
        textbox_data.str_value = scroll_string_infor[index].strbuf;
        if(scroll_param->update == TRUE)
        {
            result = show_textbox(scroll_param->id3_text, &textbox_data, TEXTBOX_DRAW_NORMAL);
        }
        else
        {
            result = show_textbox(scroll_param->id3_text, &textbox_data, TEXTBOX_DRAW_ID3_HORIZONTAL);
        }
        if (result == UI_NEED_SCROLL)
        {
            return TRUE;
        }
        else//无须水平滚屏，进行垂直滚屏加载
        {
            show_textbox(scroll_param->id3_text, &textbox_data, TEXTBOX_DRAW_ID3_VERTICAL);
            return FALSE;
        }
    }
    //ID3滚屏显示
    else
    {
        if (scroll_param->direct == ID3_SCROLL_HORIZONTAL)//水平滚屏
        {
            if(show_textbox(NULL, NULL, TEXTBOX_DRAW_SCROLL) == UI_SCROLL_OVER)
            {
                textbox_data.str_id = (uint16) - 1;
                textbox_data.lang_id = scroll_string_infor[index].language;
                textbox_data.str_value = scroll_string_infor[index].strbuf;
                show_textbox(scroll_param->id3_text, &textbox_data, TEXTBOX_DRAW_NOSCROLL);
                //水平滚屏结束，进行垂直滚屏加载
                show_textbox(scroll_param->id3_text, &textbox_data, TEXTBOX_DRAW_ID3_VERTICAL);
                return FALSE;
            }
            else
            {
                return TRUE;
            }
        }
        else//竖向滚屏
        {
            scrollhead_height += 1;
            picbox_data.pic_id = -1;
            picbox_data.frame_id = index+1;//帧0为背景图，帧1为title
            show_picbox_id3(scroll_param->id3_icon, &picbox_data, scrollhead_height);
            if(verti_scroll_string(&(scroll_desc.scroll_region), scroll_string_infor, index) == TRUE)
            {
                return TRUE;
            }
            else
            {
                return FALSE;
            }
        }
    }
}

/*! \endcond */

/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    实现ID3竖向滚屏
 * \param[in]   scroll：滚屏区域
 * \param[in]   scroll_string_infor：ID3滚屏信息结构体指针
 * \param[in]   index：滚屏序号，第几个
 * \param[out]  none
 * \return      初始化或未滚完，返回TRUE，已滚完，返回FALSE
 * \note
 *******************************************************************************/
uint8 verti_scroll_string(region_t *scroll, scroll_content_t *scroll_string_infor, uint8 index)
{
#ifndef PC
    uint8 *display_buffer = (uint8 *) display_buf_addr;
#else
    uint8 *display_buffer = GET_REAL_ADDR(display_buf_addr);
#endif
    string_desc_t string_infor;
    region_t cur_region, buf_region, string_region;
    uint8 index1;
    uint16 true_len, str_len;
    str_getlen_result_t getlen_result;
    
#if (USE_MULTI_FONTLIB != 0)
    //切换字体字库
    fix_switch_fontlib(scroll_desc.disp_mode);
#endif
       
    if (index < 2)
    {
        index1 = index + 1;
    }
    else
    {
        index1 = 0;
    }
    
    display_buffer_deal();
    if (scrollhead_height <= scroll->height)
    {
        libc_memset(string_buf, 0, STRING_BUFFER_LEN);

        //重新设置画笔颜色和背景色
        romf_set_pen_color(scroll_desc.pen_color, 0, 0);
        if ((scroll_desc.disp_mode & DISP_CHAR_FILL_OPTION) == CHAR_FILL_COLOR)
        {
            romf_set_backgd_color(scroll_desc.back_id, 0, 0);
        }
        
        //构造或读取背景图片数据
        buf_region.x = 0;
        buf_region.y = 0;
        buf_region.height = text_bg_desc.height;
        buf_region.width = text_bg_desc.width;
        read_str_bgpic(display_buffer, &buf_region, scroll_desc.disp_mode & DISP_CHAR_FILL_OPTION);
        
        //if (need_arabic_deal(scroll_desc.disp_mode) == TRUE)
        //{
        //    cur_region.x = DISPLAY_LENGTH - scroll->x - scroll->width;
        //}
        //else
        {
            cur_region.x = scroll->x;
        }
        cur_region.y = scroll->y;
        cur_region.width = scroll->width;
        cur_region.height = fix_font_height_actual;
        
        //叠加上半部分字符串
        if (scrollhead_height < scroll->height)
        {
            string_infor.data.str = scroll_string_infor[index].strbuf;
            string_infor.language = scroll_string_infor[index].language;
            
            if (string_infor.language == UNICODEDATA)
            {
                true_len = (uint16)libc_strlenuni(string_infor.data.str);
            }
            else
            {
                true_len = (uint16)libc_strlen(string_infor.data.str);
            }
            
            libc_memcpy(string_buf, string_infor.data.str, (uint32)true_len);
            string_infor.data.str = string_buf;
            string_infor.length = true_len;
            
            string_region.x = scroll->x;
            string_region.y = scroll->y + scrollhead_height;
            string_region.width = scroll->width;
            string_region.height = fix_font_height_actual - scrollhead_height;
            
            //阿拉伯和希伯来语特殊处理
            arabic_stat = ARABIC_INIT;
            if(need_arabic_deal(0) == TRUE)
            {
                //1. 阿拉伯和希伯莱需要进行 UNICODE 连写处理
                if(arabic_stat == ARABIC_INIT)
                {
                    string_infor.argv = STRING_BUFFER_LEN;
                    arabic_uni_join(&string_infor);
                    
                    //阿拉伯和希伯莱语已经经过 UNICODE 连写处理
                    arabic_stat = ARABIC_UNI_JOIN;
                }
                
                //2. 阿拉伯和希伯莱需要进行西文翻转处理
                if(arabic_stat == ARABIC_UNI_JOIN)
                {
                    string_infor.argv = STRING_BUFFER_LEN;
                    arabic_ascii_turn(&string_infor);
                    
                    //阿拉伯和希伯莱语已经经过西文翻转处理
                    arabic_stat = ARABIC_ASCII_TURN;
                }
                
                true_len = string_infor.length;
            }
            
            if(string_infor.language == UNICODEDATA)
            {
                string_infor.language = UNICODELANGUAGE;
            }
            
            if(need_arabic_deal(0) == TRUE)
            {
#if (USE_MULTI_FONTLIB == 0)
                str_len = romf_get_string_length(&string_infor, string_region.width, &getlen_result);
#else
                str_len = fixf_get_string_length(&string_infor, string_region.width, &getlen_result);
#endif
                if(str_len < string_region.width)
                {
                    string_region.x += (string_region.width - str_len);
                }
            }
            
            ramf_put_string_line_hook(&string_infor, 0, true_len);
            put_string_line_sdimage(&string_infor, &string_region, 0);
        }
        
        //叠加下半部分字符串
        string_infor.data.str = scroll_string_infor[index1].strbuf;
        string_infor.language = scroll_string_infor[index1].language;
        
        if (string_infor.language == UNICODEDATA)
        {
            true_len = (uint16)libc_strlenuni(string_infor.data.str);
        }
        else
        {
            true_len = (uint16)libc_strlen(string_infor.data.str);
        }        
        libc_memcpy(string_buf, string_infor.data.str, (uint32)true_len);
        string_infor.data.str = string_buf;
        string_infor.length = true_len;
        
        string_region.x = scroll->x;
        string_region.y = scroll->y;
        string_region.width = scroll->width;
        string_region.height = scrollhead_height;
        
        //阿拉伯和希伯来语特殊处理
        arabic_stat = ARABIC_INIT;
        if(need_arabic_deal(0) == TRUE)
        {
            //1. 阿拉伯和希伯莱需要进行 UNICODE 连写处理
            if(arabic_stat == ARABIC_INIT)
            {
                string_infor.argv = STRING_BUFFER_LEN;
                arabic_uni_join(&string_infor);
                
                //阿拉伯和希伯莱语已经经过 UNICODE 连写处理
                arabic_stat = ARABIC_UNI_JOIN;
            }
            
            //2. 阿拉伯和希伯莱需要进行西文翻转处理
            if(arabic_stat == ARABIC_UNI_JOIN)
            {
                string_infor.argv = STRING_BUFFER_LEN;
                arabic_ascii_turn(&string_infor);
                
                //阿拉伯和希伯莱语已经经过西文翻转处理
                arabic_stat = ARABIC_ASCII_TURN;
            }
            
            true_len = string_infor.length;
        }
        
        if(string_infor.language == UNICODEDATA)
        {
            string_infor.language = UNICODELANGUAGE;
        }

        if(need_arabic_deal(0) == TRUE)
        {
#if (USE_MULTI_FONTLIB == 0)
            str_len = romf_get_string_length(&string_infor, string_region.width, &getlen_result);
#else
            str_len = fixf_get_string_length(&string_infor, string_region.width, &getlen_result);
#endif
            if(str_len < string_region.width)
            {
                string_region.x += (string_region.width - str_len);
            }
        }
        
        ramf_put_string_line_hook(&string_infor, 0, true_len);
        put_string_line_sdimage(&string_infor, &string_region, 0);
                
        set_lcd_param_hook(&cur_region, LCD_PARAM_STRING);
        //传送缓冲区数据到DMA中
        dma_buffer_data_hook(scroll->width * scroll->height, scroll->width);

        return TRUE;
    }
    else
    {
        scrollhead_height = 0;
        return FALSE;
    }
}
/*! \endcond */
