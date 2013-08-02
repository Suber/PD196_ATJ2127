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
 * \file     ui_putstring.c
 * \brief    字符串处理及显示模块
 * \author   liutingting
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    在指定位置显示字符串
 * \param[in]    str_desc：要显示的字符串结构体指针，参见string_desc_t定义
 * \param[in]    str_region：显示字符串的区域,参见region_t定义
 * \param[in]    mode：字符串的显示模式，参见display.h中字符串显示模式格式定义
 * \param[out]   none
 * \return       uint8类型的参数，显示是否成功
 * \retval       UI_NO_ERR 正常显示
 * \retval       UI_NEED_SCROLL 需要滚屏显示
 * \ingroup      string
 * \par          exmaple code
 * \code
    string_desc_t desc;
    region_t kregion;
    uint16 disp_mode;
    
    desc.data.str = str;
    desc.length = (uint16) libc_strlen(str);
    desc.language = language;
    result = (ui_result_e) show_string(&desc, &kregion, disp_mode);
 * \endcode
 * \note
 *******************************************************************************/
uint8 show_string(string_desc_t *str_desc, region_t *str_region, uint16 mode)
{
    string_desc_t string_infor;

    libc_memset(string_buf, 0, STRING_BUFFER_LEN);
    libc_memcpy(&string_infor, str_desc, sizeof(string_desc_t));
    arabic_stat = ARABIC_INIT;
    if (str_desc->language == UNICODEID)
    {
        uint16 str_id = string_infor.data.id;

        string_infor.data.str = string_buf;
        string_infor.length = STRING_BUFFER_LEN;
        string_infor.language = UNICODEDATA;
        romf_get_unicode_data(str_id, &string_infor);
    }
    else if (str_desc->language == UTF_8DATA)
    {
        int16 true_len;
        
        //临时借用 arabic_string_buf 转换
        libc_memcpy(arabic_string_buf, string_infor.data.str, (uint32)string_infor.length);
        arabic_string_buf[string_infor.length] = 0;
        utf8_to_unicode(arabic_string_buf, (uint16 *)string_buf, &true_len);
        string_infor.data.str = string_buf;
        string_infor.length = (uint16)true_len;
        string_infor.language = UNICODEDATA;
    }
    else
    {
        uint16 true_len;

        true_len = string_infor.length;
        if (true_len > STRING_BUFFER_LEN)//一般为-1，要求有结束符，否则可能会出问题
        {
            if (string_infor.language == UNICODEDATA)
            {
                true_len = (uint16)libc_strlenuni(string_infor.data.str);
            }
            else
            {
                true_len = (uint16)libc_strlen(string_infor.data.str);
            }
            if (true_len > STRING_BUFFER_LEN)
            {
                true_len = STRING_BUFFER_LEN;
            }
        }
        libc_memcpy(string_buf, string_infor.data.str, (uint32)true_len);
        string_infor.data.str = string_buf;
        string_infor.length = true_len;
    }
    
    //语言选择菜单中，菜单项字符串特殊处理
    if((mode & DISP_LANG_LIST_MENU) != 0)
    {
        uint8 lang;
        lang = is_arabic_hebrew_string(&string_infor);
        
        //普通语言下阿拉伯和希伯莱语处理
        if(need_arabic_deal(mode) == FALSE)
        {
            if(lang != 0)//阿拉伯和希伯莱语
            {
                //进行阿拉伯连写处理，并翻转字符串
                if(lang == 1)//阿拉伯语
                {
                    arabic_do_join(&string_infor);
                }
                arabic_do_turn(string_infor.data.str, 0, string_infor.length);
            }
        }
        //希伯莱语下阿拉伯语和普通语言处理
        else if(language_id == HEBREW)
        {
            if(lang == 1)//阿拉伯语
            {
                //进行阿拉伯连写处理
                arabic_do_join(&string_infor);
            }
            else if(lang == 0)//普通语言
            {
                arabic_stat = ARABIC_LINE_TURN;//可以直接显示
            }
            else
            {
                //for QAC
            }
        }
        //阿拉伯语下普通语言处理
        else
        {
            if(lang == 0)//普通语言
            {
                arabic_stat = ARABIC_LINE_TURN;//可以直接显示
            }
        }
    }
    
    //阿拉伯和希伯来语特殊处理
    if(need_arabic_deal(mode) == TRUE)
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
    }

    //转换语言类型，转为内部处理语言，UNICODELANGUAGE和其他值
    if (string_infor.language == UNICODEDATA)
    {
        string_infor.language = UNICODELANGUAGE;
    }
    
    scroll_flag = FALSE;//在put_string中判断是否需要滚屏
    //注：put_string的编码流可能不带结束符
    put_string(&string_infor, str_region, mode);

    //需要滚屏，进行滚屏相关处理
    if (scroll_flag == TRUE)
    {
        libc_memset(scroll_buf, 0, STRING_BUFFER_LEN);
        libc_memcpy(scroll_buf, string_infor.data.str, (uint32)string_infor.length);
        scroll_str.data.str = scroll_buf;
        scroll_str.language = string_infor.language;
        scroll_str.length = string_infor.length;

        return UI_NEED_SCROLL;
    }

    return UI_NO_ERR;
}
/*! \endcond */


/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    在指定位置显示字符串
 * \param[in]    str_desc 要显示的字符串结构体指针，参见string_desc_t定义
 * \param[in]    str_region 显示字符串的区域,参见region_t定义
 * \param[in]    mode 字符串的显示模式，参见display.h中字符串显示模式格式定义
 * \param[out]   none
 * \return       uint8* 显示结束字符串指针
 * \ingroup      
 * \note
 *******************************************************************************/
uint8 *put_string(string_desc_t *string_infor, region_t *str_region, uint16 mode)
{
    string_desc_t string_backup;
    uint8 *nextchar;
    uint8 *cur_str;
    uint16 str_len;
    region_t cur_region;//单行显示区域
    region_t kregion;//不完整字符显示区域
    str_getlen_result_t getlen_result;
    uint16 length;
    uint16 adjust_x = 0;//阿拉伯和希伯莱x坐标调整
    uint16 adjust_w = 0;//阿拉伯和希伯莱x坐标调整
    uint16 length_bk;
    
#if (USE_MULTI_FONTLIB != 0)
    //切换字体字库
    fix_switch_fontlib(mode);
#endif
    
    //初始化原始参数
    kregion.y = cur_region.y = str_region->y;
    kregion.height = cur_region.height = fix_font_height_actual;
    cur_str = string_infor->data.str;
    length = string_infor->length;
    
    //显示一行字符串，计算显示区域（包括单行显示区域和不完整字符显示区域）
    put_str_again: 
#if (USE_MULTI_FONTLIB == 0)
    str_len = romf_get_string_length(string_infor, str_region->width, &getlen_result);
#else
    str_len = fixf_get_string_length(string_infor, str_region->width, &getlen_result);
#endif
    cur_region.width = getlen_result.true_length;
    kregion.width = str_region->width - getlen_result.true_length;
    
    //以省略号显示的超长字符串需要加省略号处理，并且重新计算长度
    if ((str_len > str_region->width) && ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_ELLIPSIS))
    {
        string_infor->length = ellipsis_to_longstr(string_infor, cur_region.width, &getlen_result);
        //更新字符串区域
        str_len = getlen_result.true_length;
        cur_region.width = getlen_result.true_length;
        kregion.width = str_region->width - getlen_result.true_length;
    }

    //短字符串处理，仅需输出单行字符串
    if (str_len <= str_region->width)
    {
        // 居中显示
        if ((mode & DISP_ALIGN_OPTION) == DISP_ALIGN_MEDIACY)
        {
            cur_region.x = str_region->x + (str_region->width - str_len) / 2;
        }
        // 居右显示
        else if ((mode & DISP_ALIGN_OPTION) == DISP_ALIGN_RIGHT)
        {
            if (need_arabic_deal(mode) == TRUE)
            {
                cur_region.x = str_region->x;
                adjust_x = str_region->x + str_len;
                adjust_w = str_region->width - str_len;
            }
            else
            {
                cur_region.x = (str_region->x + str_region->width) - str_len;
            }
        }
        // 居左显示
        else
        {
            if (need_arabic_deal(mode) == TRUE)
            {
                cur_region.x = (str_region->x + str_region->width) - str_len;
                adjust_x = str_region->x;
                adjust_w = str_region->width - str_len;
            }
            else
            {
                cur_region.x = str_region->x;
            }
        }
    }
    //长字符串处理，RECT和滚屏方式需要显示不完整字符
    else
    {
        if ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_SCROLL)
        {
            scroll_flag = TRUE;//需要滚屏
        }
        
        // 居中显示
        if ((mode & DISP_ALIGN_OPTION) == DISP_ALIGN_MEDIACY)
        {
            //不显示不完整字符以及多行显示才需要居中显示
            if (((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_NO_RECT) 
             || ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_MULTI_ROW))
            {
                cur_region.x = str_region->x + (str_region->width - getlen_result.true_length) / 2;
            }
            else
            {
                //等同于居左居右显示
                goto over_left_right;
            }
        }
        // 居左居右显示
        else
        {
            over_left_right: 
            if (need_arabic_deal(mode) == TRUE)
            {
                cur_region.x = (str_region->x + str_region->width) - getlen_result.true_length;
                kregion.x = cur_region.x - (str_region->width - getlen_result.true_length);
                adjust_x = str_region->x;
                adjust_w = str_region->width - getlen_result.true_length;
            }
            else
            {
                cur_region.x = str_region->x;
                kregion.x = str_region->x + getlen_result.true_length;
            }
        }
    }

    //不需要进行显示，仅仅为了处理字符串，返回是否滚屏结果
    if((mode & DISP_DISPLAY_DISABLE) != 0)
    {
        nextchar = NULL;
        goto putstring_end;
    }
    
    //进行阿拉伯翻转以实现从左到右显示等
    libc_memcpy(&string_backup, string_infor, sizeof(string_desc_t));
    ramf_put_string_line_hook(string_infor, mode, getlen_result.true_bytes);
    
    if((mode & DISP_DIVIDE_WORD_ENABLE) != 0)//自动分词
    {
        uint16 line_mode;
        
        length_bk = string_infor->length;
        string_infor->argv = cur_region.width;
        line_mode = DIVIDE_WORD_ENABLE | (mode & DISP_FONT_OPTION);
        get_text_line(string_infor, line_mode, NULL);
        string_infor->length = string_infor->result;
    }
    
    if((mode & DISP_CHAR_FILL_OPTION) == CHAR_FILL_SDIMAGE)//从SD读取背景，再直接叠上字符串
    {
        //显示单行字符串
        nextchar = put_string_line_sdimage(string_infor, &cur_region, 1);
    }
    else
    {
        //显示单行字符串
#if (USE_MULTI_FONTLIB == 0)
        nextchar = romf_put_string_line(string_infor, &cur_region, mode & DISP_CHAR_FILL_OPTION);
#else
        nextchar = fixf_put_string_line(string_infor, &cur_region, mode & DISP_CHAR_FILL_OPTION);
#endif
    }
    if((mode & DISP_DIVIDE_WORD_ENABLE) != 0)//自动分词
    {
        string_infor->length = length_bk;
    }
    arabic_stat = ARABIC_ASCII_TURN;
    
    //put_string_line 后要更新region为剩余区域，阿拉伯和希伯莱需要adjust
    if (need_arabic_deal(mode) == TRUE)
    {
        region.x = adjust_x;
        region.width = adjust_w;
        nextchar = string_backup.data.str + (nextchar - string_infor->data.str);
    }
    if (str_len > str_region->width)
    {
        string_infor->result = STRING_TAIL_REGION_ERR;
    }
    
    //后续处理
    //显示不完整字符
    if (((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_RECT) || ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_SCROLL))
    {
        // 输出最后不完整字符
        if ((string_infor->result == STRING_TAIL_REGION_ERR) && (kregion.width > 0))//剩余宽度不为0才显示不完整字符
        {
            char_out_rect(nextchar, string_infor->language, &kregion, mode, 0, 1);
        }
    }
    else if ((mode & DISP_LONGSTR_OPTION) == DISP_LONGSTR_MULTI_ROW)
    {
        switch (string_infor->result)
        {
            /*! 遇到换行符 */
            case STRING_TAIL_CHAR_LR:
            /*! 显示区域越界 */
            case STRING_TAIL_REGION_ERR:
            string_infor->length = (uint16) (string_infor->length - (nextchar - string_infor->data.str));
            string_infor->data.str = cur_str = nextchar;
            cur_region.y += fix_font_height_line;//换行显示
            if ((cur_region.y + fix_font_height_actual) <= (str_region->y + str_region->height))
            {
                goto put_str_again;
            }
            break;

            /*! 字符串有效长度用完 */
            case STRING_TAIL_LEN_ZERO:
            /*! 遇到字符串终止符或者未决符号 */
            case STRING_TAIL_EOF_NOPEND:
            /*! 其他错误 */
            case STRING_TAIL_OTHER_ERR:
            break;

            default:
            break;
        }
    }
    else
    {
        //do nothing for QAC
    }
    
    putstring_end:
    string_infor->data.str = cur_str;
    string_infor->length = length;
    return nextchar;
}
/*! \endcond */
