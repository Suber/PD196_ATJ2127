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
 * \file     ui_copy_arabic.c
 * \brief    arabic转换
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *              实现Arabic字符串copy
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond */ 
/******************************************************************************/
/*!
 * \par  Description:
 *    阿拉伯字符串copy，进行编码转换和连写处理等。
 * \param[in]   desc：源字符串描述符指针
 * \param[out]  desc：转换后字符串描述符指针
 * \return      none
 * \note        desc->argv 表示缓冲区大小，在内码转Unicode码时使用。
 *******************************************************************************/
void arabic_uni_join(string_desc_t *desc)
{
    //内码转换为Unicode码，以便进行阿拉伯连写处理
    if (desc->language != UNICODEDATA)
    {
        arabic_char_to_unicode(desc);
        desc->language = UNICODEDATA;
    }
    
    //阿拉伯连写处理
    if (language_id == ARABIC)
    {
        arabic_do_join(desc);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    阿拉伯语内码转换为unicode
 * \param[in]   desc：源字符串描述符指针
 * \param[out]  desc：转换后字符串描述符指针
 * \return      返回结果字符串有效字节数，并存储到 desc->length 中。
 * \note        desc->argv 表示缓冲区大小，使用arabic_string_buf 进行转换
 *******************************************************************************/
uint16 arabic_char_to_unicode(string_desc_t *desc)
{
    uint8 *scr, *dest;
    uint16 length = desc->length;
    uint16 real_len;
    uint16 temp_code;
    uint16 i;

    //先统计有效字符数
    scr = desc->data.str;
    real_len = 0;
    for (i = 0; i < length; i++)
    {
        if (*scr == 0x0)
        {
            break;
        }
        real_len++;
        scr++;
    }

    //如果字符串过长，仅转换部分
    if ((real_len * 2) > (desc->argv - 2))
    {
        real_len = (desc->argv - 2) / 2;
    }

    //Unicode编码转换
    char_to_unicode(arabic_string_buf, desc->data.str, real_len);
    libc_memcpy(desc->data.str, arabic_string_buf, real_len * 2 + 2);
    desc->length = real_len * 2;
    return real_len * 2;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    arabic_ascii_turn
 * \param[in]   desc：字符串描述符指针
 * \param[out]  none
 * \return      none
 * \note        支持内码和Unicode转换
 *******************************************************************************/
void arabic_ascii_turn(string_desc_t *desc)
{
    uint8 *str = desc->data.str;
    uint16 length = desc->length;

    uint16 Glyph;
    uint16 Glyph_prev = 0;
    uint8 start, end;
    uint8 i = 0;
    uint8 char_count;
    //uint8 char_is_arabic = 0;

    while (1)
    {
        if (i >= length)//字符串转换完毕
        {
            break;
        }

        Glyph = ((uint16)str[i + 1]  << 8) + str[i];
        if (Glyph == 0x0)//结束符，字符串转换完毕
        {
            break;
        }

        char_count = 0;
        start = i;
        while ((i <= length) && (Glyph < 0x80) //ASCII码
            && (((Glyph >= 0x41) && (Glyph <= 0x5a)) //大写字母
             || ((Glyph >= 0x61) && (Glyph <= 0x7a)) //小写字母
             || ((Glyph >= 0x30) && (Glyph <= 0x39)) //数字
             || (Glyph == 0x27) || (Glyph == 0x2d) || (Glyph == 0x2e) || (Glyph == 0x20)))//单词连接字符
        {
            char_count++;
            Glyph_prev = Glyph;
            //获取下一个字符
            i += 2;//可能会大于length，但在while循环条件中被过滤
            Glyph = ((uint16)str[i + 1]  << 8) + str[i];
        }
        
        if (char_count > 0)
        {
            i -= 2;
        }
        end = i;

        if ((Glyph_prev == 0x20) || (Glyph_prev == 0x27) || (Glyph_prev == 0x2d) || (Glyph_prev == 0x2e))
        {
            char_count--;
            end -= 2;
        }
        //多字符连续，需要置换字符顺序
        if (char_count > 1)
        {
            arabic_do_turn(str, start, end + 2);//end以结束位置传参
        }
        i += 2;//从下个字符继续扫描
    }
}

void arabic_do_turn(uint8 *str, uint16 start, uint16 end)
{
    uint8 temp_byte;

    if (end > 0)
    {
        end -= 2;//指向最后一个有效字符
    }
    while (end > start)
    {
        temp_byte = str[end];
        str[end] = str[start];
        str[start] = temp_byte;
        temp_byte = str[end + 1];
        str[end + 1] = str[start + 1];
        str[start + 1] = temp_byte;

        end -= 2;
        start += 2;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    is_arabic_hebrew_string
 * \param[in]   desc：字符串描述符指针
 * \param[out]  none
 * \return      uint8 1表示阿拉伯语，2表示希伯来语，0表示其他语言
 * \note        支持内码和Unicode转换
 *******************************************************************************/
uint8 is_arabic_hebrew_string(string_desc_t *desc)
{
    uint8 *str = desc->data.str;
    uint16 length = desc->length;

    uint16 Glyph;
    uint8 i = 0;

    while (1)
    {
        if (i >= length)//字符串转换完毕
        {
            break;
        }

        Glyph = ((uint16)str[i + 1]  << 8) + str[i];
        if (Glyph == 0x0)
        {
            break;
        }
        if (is_arabic_char(Glyph) == TRUE)
        {
            return 1;
        }
        if (is_hebrew_char(Glyph) == TRUE)
        {
            return 2;
        }
        
        i = i + 2;
    }
    return 0;
}

/*! \endcond */
