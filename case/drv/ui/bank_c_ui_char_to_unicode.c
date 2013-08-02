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
 * \file     ui_char_to_unicode.c
 * \brief    内码转换成unicode
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现内码转换成unicode
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用SD文件系统接口函数sys_sd_fread和sys_sd_fseek
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    内码字符串转unicode字符串
 * \param[in]   src：源字符串指针
 * \param[in]   len：要转换的内码字符串长度
 * \param[out]  dest：目的字符串指针
 * \return      是否转换成功
 * \retval      TRUE 转换成功
 * \retval      FALSE 转换失败
 * \ingroup     string
 * \note        目的字符串缓冲区由用户保证不会溢出，缓冲区大小需要加上结束符。
 *******************************************************************************/
bool char_to_unicode(uint8 *dest, uint8 *src, uint16 len)
{
    int s_cnt = 0;
    int d_cnt = 0;
    uint16 font_code;

    while (1)
    {
        if ((s_cnt >= len) || (src[s_cnt] == 0))//到尾或者结束符
        {
            break;
        }
        
        if (((uint8) src[s_cnt] >= 0x80) && (language_2byte == TRUE))
        {
            font_code = src[s_cnt + 1];
            font_code = font_code << 8;
            font_code |= src[s_cnt];
            s_cnt += 2;
        }
        else
        {
            font_code = src[s_cnt];
            s_cnt++;
        }
        //转换为Unicode编码，小于0x80的内码无须转换
        if (font_code >= 0x80)
        {
            font_code = romf_multi_char_to_unicode(font_code);
        }
        
        dest[d_cnt] = *((uint8*) &font_code); //低字节
        d_cnt++;
        dest[d_cnt] = *((uint8*) &font_code + 1); //高字节
        d_cnt++;
    }
    dest[d_cnt] = 0;
    d_cnt++;
    dest[d_cnt] = 0;
    return TRUE;
}
/*! \endcond */
