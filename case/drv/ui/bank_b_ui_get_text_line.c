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
 * \file     ui_get_text_line.c
 * \brief    文本字符串长度处理统计模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               根据字符的编码类型，对文本字符串进行长度处理统计，用于文本断行等功能
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#define SEP_WORD_COUNT      18//19 去掉所有格符号

//单词分隔符表
static const uint8 sep_word[SEP_WORD_COUNT] = 
{ 
    0x00, //结束符
    0x20, //空格
    0x21, //感叹号!
    0x22, //双引号"
    //0x27, //所有格符’ 所有格符号应该连写
    0x2c, //逗号,
    0x2e, //句号.
    0x2f, //斜杠/
    0x3a, //冒号:
    0x3b, //分号;
    0x3c, //左书名号<
    0x3d, //等于号=
    0x3e, //右书名号>
    0x3f, //问号?
    0x40, //邮箱地址符号@
    0x5b, //左中括号[
    0x5d, //右中括号]
    0x7b, //左花括号{
    0x7d, //右花括号}
};

/*! \cond */

//判断是否word字符，目前支持英文单词
static bool check_word_char(uint8 *str, int8 language)
{
    uint16 tmp_code;
    uint8 i;
    
    if (language == UNICODELANGUAGE)
    {
        tmp_code = *str + ((uint16) *(str + 1) << 8);
    }
    else
    {
        tmp_code = *str;
    }
    
    if (tmp_code >= 0x80)//非ASCII码
    {
        return FALSE;
    }
    
    for (i = 0; i < SEP_WORD_COUNT; i++)
    {
        if (sep_word[i] == (uint8)tmp_code)
        {
            return FALSE;//分隔符
        }
    }
    
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取当前位置字符类型--utf8编码
 * \param[in]    str：当前字符串起始位置
 * \param[in]    length：当前有效字节数
 * \param[in]    language：字符串编码语言类型
 * \param[out]   byte_cnt：传回当前有效字符的字节数
 * \return       char_type_e：返回字符类型
 * \note         这里的字符是一个更丰富的概念，定义为完整显示的字符单元。
 *******************************************************************************/
char_type_e get_next_char_type_utf8(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt)
{
    uint8 tmp_char;

    if (*str == 0x00)
    {
        *byte_cnt = 0;
        return CHAR_EOF;
    }
    else if (*str == 0x0a)
    {
        *byte_cnt = 1;
        return CHAR_LR;//linux
    }
    else
    {
        //do nothing for QAC
    }
    if (length >= 2)
    {
        if ((*str == 0x0d) && (*(str + 1) == 0x0a))
        {
            *byte_cnt = 2;
            return CHAR_LR;//windows
        }
        else if (*str == 0x0d)
        {
            *byte_cnt = 1;
            return CHAR_LR;//mac
        }
        else
        {
            //do nothing for QAC
        }
        if (length >= 3)
        {
            if ((*str == 0xef) && (*(str + 1) == 0xbb) && (*(str + 2) == 0xbf))
            {
                *byte_cnt = 3;
                return CHAR_UNI_FLAG;
            }
        }
    }
    else
    {
        if (*str == 0x0d)
        {
            *byte_cnt = 0;
            return CHAR_NOPEND;
        }
    }

    //普通字符判断
    tmp_char = *str;
    if ((tmp_char < 0x80) && (length >= 1))//0h0#######
    {
        *byte_cnt = 1;
        return CHAR_NORMAL;
    }
    else if ((tmp_char < 0xe0) && (length >= 2)) //0h110#####
    {
        *byte_cnt = 2;
        return CHAR_NORMAL;
    }
    else if ((tmp_char < 0xf0) && (length >= 3)) //0h1110####
    {
        *byte_cnt = 3;
        return CHAR_NORMAL;
    }
    else
    {
        //do nothing for QAC
    }

    *byte_cnt = 0;
    return CHAR_NOPEND;
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
static uint8 s_get_next_char_point_hook(uint8 *str, int8 language, uint8 *pbuffer)
{
    uint16 uni_code = 0;
    uint8 pt_step = 0;

    if (language == UTF_8DATA)
    {
        if (*str < 0x80)//0h0#######
        {
            uni_code = *str;
            pt_step = 1;
        }
        else if (*str < 0xe0) //0h110#####
        {
            uni_code = ((uint16) (*str) & 0x1f) << 6;
            uni_code += ((uint16) (*(str + 1)) & 0x3f);
            pt_step = 2;
        }
        else if (*str < 0xf0) //0h1110####
        {
            uni_code = ((uint16) (*str) & 0xf) << 12;
            uni_code += ((uint16) (*(str + 1)) & 0x3f) << 6;
            uni_code += ((uint16) (*(str + 2)) & 0x3f);
            pt_step = 3;
        }
        else
        {
            //do nothing for QAC
        }
    }
    else if ((language == UNICODELANGUAGE) || ((*str> ASCIIDISCODE) && (language_2byte == TRUE)))
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
    if (((language != UNICODELANGUAGE) && (language != UTF_8DATA)) && (uni_code >= ASCIIDISCODE))
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
 *    缓存点阵到缓冲区中
 * \param[in]   point_buf 点阵缓冲区
 * \param[in]   pbuffer 当前字符点阵
 * \param[in]   pos 当前位置
 * \param[in]   max_width 缓冲区行宽度
 * \return      none
 * \note
 *******************************************************************************/
static void save_char_point(uint8 *point_buf, uint8 *pbuffer, uint16 pos, uint16 max_width)
{
    uint8 i, j;
    uint8 src_data;//0表示空白点，1表示字点
    uint8 temp_width;//1表示16*8字模，2表示16×16字模
    uint8 font_width;
    uint8 *point_buf_base;
    uint8 shift_mask;
    uint8 index;

    //初始基础参数
    font_width = pbuffer[fix_font_size - 1];
    temp_width = (font_width + 7)/8;

    for (i = 0; i < fix_font_height_actual; i++)//loop for height
    {
        point_buf_base = point_buf + (i * ((max_width + 7) / 8));
        for (j = 0; j <= (font_width - 1); j++)//loop for char width
        {
            if ((j % 8) == 0)
            {
                index = ((i + fix_font_height_up_margin) * temp_width) + (j/8);
                shift_mask = 0x80;
            }
            
            src_data = (uint8)(pbuffer[index] & shift_mask);
            shift_mask >>= 1;

            if (src_data != 0)
            {
                *(point_buf_base + ((pos + j)/8)) |= (uint8)(0x80 >> ((pos + j)%8));
            }
        }
    }
}

/*! \endcond */

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    获取字符串长度
 * \param[in]    str_desc：字符串描述符结构体指针，用于文本断行功能等
 * \param[in]    line_mode：行处理模式
 * \param[in]    param：任意参数
 * \param[out]   none
 * \return       text_end_mode_e 见 text_end_mode_e 定义
 * \ingroup      string
 * \par          exmaple code
 * \code
    string_desc_t str_desc;
    uint8 page_mode;
    bool page_miss;
    
    str_desc.data.str = str_buf;
    str_desc.language = UNICODEDATA;
    str_desc.length = -1;
    page_miss = (bool)(uint32)ui_get_text_line(&str_desc, page_mode);
 * \endcode
 * \note
 *******************************************************************************/
text_end_mode_e get_text_line(string_desc_t *str_desc, uint16 line_mode, void *param)
{
    char_type_e (*get_next_char_type)(uint8 *str, uint16 length, int8 language, uint8 *byte_cnt);
    uint8 *str, *str_col;
    uint16 length;
    int8 language;
    uint8 char_bytes;
    uint16 str_width = 0;//字符串长度（像素点）
    uint16 char_width;
    char_type_e char_type;
    uint16 word_index = 0;//单词开始位置
    uint16 word_count = 0;//当前行单词数
    uint16 word_state = 0;//0--单词间隔中，1--正在扫描单词
    uint16 col = 0;
    uint16 lines_cnt = 0;//总行数
    text_end_mode_e end_mode = FALSE;
    bool scan_over = FALSE;//当前行扫描是否结束
    textline_param_t *textline_param;
    
#if (USE_MULTI_FONTLIB != 0)
    //切换字体字库
    fix_switch_fontlib(line_mode);
#endif
    
    if((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS)
    {
        textline_param = (textline_param_t *)param;
        libc_memset(textline_param->point_buf, 0x00, ((str_desc->argv + 7)/8)*fix_font_height_actual);
    }

    //基本参数初始化
    str = str_desc->data.str;
    length = str_desc->length;
    language = str_desc->language;
    if ((language == UNICODEDATA) || (language == UNICODELANGUAGE))
    {
        language = UNICODELANGUAGE;
        get_next_char_type = get_next_char_type_unicode;
    }
    else if (language == UTF_8DATA)
    {
        get_next_char_type = get_next_char_type_utf8;
    }
    else
    {
        get_next_char_type = get_next_char_type_ansi;
    }
    
    str_col = str;
    //单行字符串获取字符串长度
    while (1)
    {
        scan_string:
        //字符串数据已经完毕
        if (length == 0)
        {
            lines_cnt++;
            end_mode = TEXT_END_PAGE_MISS;
            goto text_line_ret;
        }
        //获取下一个字符，分为结束符，换行符和普通字符
        char_type = get_next_char_type(str, length, language, &char_bytes);
        switch(char_type)
        {
        case CHAR_EOF://结束符
            lines_cnt++;
            end_mode = TEXT_END_NUL;
            goto text_line_ret;

        case CHAR_NOPEND://数据不够
            lines_cnt++;
            end_mode = TEXT_END_PAGE_MISS;
            goto text_line_ret;

        case CHAR_UNI_FLAG://Unicode 字符串标志 小端Unicode
            str += char_bytes;
            col += char_bytes;
            length -= char_bytes;
            goto scan_string;//继续扫描

        case CHAR_LR://换行符
            if( ((line_mode & FUNC_MODE_OPTION) == FUNC_GET_LINE_CHARS)
             || ((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS) )
            {
                //不过滤空行，行尾换行符需要在这里过滤掉，后面的换行符不过滤
                if ((line_mode & FILTER_BLANK_OPTION) == FILTER_BLANK_DISABLE)
                {
                    str += char_bytes;
                    col += char_bytes;
                    length -= char_bytes;
                }

                end_mode = TEXT_END_LR;
                goto text_line_ret;
            }
            else
            {
                str += char_bytes;
                col += char_bytes;
                length -= char_bytes;
                scan_over = TRUE;
            }
            break;

        default://普通字符处理
            //分词检测
            if ((line_mode & DIVIDE_WORD_OPTION) == DIVIDE_WORD_ENABLE)
            {
                //正在分词
                if (word_state == 1)
                {
                    //遇到非word字符，表示单词结束
                    if(check_word_char(str, language) == FALSE)
                    {
                        word_count++;
                        word_state = 0;
                    }
                }
                //未开始分词
                else
                {
                    //遇到word字符，表示单词开始
                    if (check_word_char(str, language) == TRUE)
                    {
                        word_index = col;
                        str_col = str;
                        word_state = 1;
                    }
                    else//其他字符，包括分隔符，当作word处理
                    {
                        word_count++;
                    }
                }
            }
            break;
        }
        
        //扫描好一行
        if(scan_over == TRUE)
        {
            lines_cnt++;
            str_width = 0;
            col = 0;
            word_count = 0;
            word_state = 0;
            scan_over = FALSE;
            continue;
        }

        //考虑nor 方案裁减字库或内码字库
        char_bytes = s_get_next_char_point_hook(str, language, fix_p_char_point);

        //字符串宽度累计
        char_width = fix_p_char_point[fix_font_size - 1];
        if ((str_width + char_width) > str_desc->argv)//已经超过最大长度，无需再计算
        {
            //回滚一个单词
            if (  ((line_mode & DIVIDE_WORD_OPTION) == DIVIDE_WORD_ENABLE) //分词检测
               && (word_state == 1)//正在分词
               && (word_count > 0))//当前行已经分出其他单词出来，避免超长单词无法分词 bug
            {
                length += (col - word_index);
                col = word_index;
                str = str_col;
            }
            end_mode = TEXT_END_OVER;
            if( ((line_mode & FUNC_MODE_OPTION) == FUNC_GET_LINE_CHARS)
             || ((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS) )
            {
                goto text_line_ret;
            }
            else
            {
                scan_over = TRUE;
            }
        }
        
        if(scan_over == FALSE)
        {
            if((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS)//缓存点阵
            {
                save_char_point(textline_param->point_buf, fix_p_char_point, str_width, str_desc->argv);
            }
            str += char_bytes;
            length -= char_bytes;
            col += char_bytes;
            str_width += char_width;
        }
        else
        {
            lines_cnt++;
            str_width = 0;
            col = 0;
            word_count = 0;
            word_state = 0;
            scan_over = FALSE;
        }
    }
    
    text_line_ret://返回结果
    if((line_mode & FUNC_MODE_OPTION) == FUNC_GET_LINE_CHARS)
    {
        str_desc->result = col;//返回当前行字节数
    }
    else if((line_mode & FUNC_MODE_OPTION) == FUNC_GET_CHAR_POINTS)
    {
        textline_param->true_width = str_width;
        textline_param->true_height = fix_font_height_actual;
        str_desc->result = col;//返回当前行字节数
    }
    else
    {
        str_desc->result = lines_cnt;//返回字符串总行数
    }

    return end_mode;
}

/*! \endcond */
