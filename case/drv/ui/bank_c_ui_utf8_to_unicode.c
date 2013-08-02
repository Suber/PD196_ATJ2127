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
 * \file     ui_utf8_to_unicode.c
 * \brief    utf8转换成unicode
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现utf8转换成unicode
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    utf8字符串转unicode字符串
 * \param[in]    src：源字符串指针
 * \param[out]   dest：目的字符串指针
 * \param[out]   size：转换成unicode的字节数
 * \return       none
 * \ingroup      string
 * \note
 *******************************************************************************/
void utf8_to_unicode(uint8* src, uint16*dest, int16* size)
{
    uint8* pin = src;
    uint8 in_val;
    int16 resultsize = 0;
    uint8 t1, t2, t3;
    uint8 tmpval[2] =
    { 0, 0 };
    while (*pin != 0)
    {
        in_val = *pin;
        if ((in_val > 0) && (in_val <= 0x7f))
        {
            tmpval[0] = in_val;
            tmpval[1] = 0;
        }
        else if ((in_val & (0xff << 5)) == 0xc0) //高2bit 是否为1
        {
            t1 = (uint8)(in_val & (0xff >> 3));//取低5bit
            pin++;
            in_val = *pin;
            if (in_val == 0)
            {
                break;
            }
            t2 = (uint8)(in_val & (0xff >> 2));//取低6bit
            //(t1的低2bit 放在最高两bit,或上t2的低6bit)
            tmpval[0] = (uint8)(t2 | ((t1 & (0xff >> 6)) << 6));
            tmpval[1] = t1 >> 2;//取低中间3bit
        }
        else if ((in_val & (0xff << 4)) == 0xe0)
        {
            t1 = (uint8)(in_val & (0xff >> 3));//取低5bit
            pin++;
            in_val = *pin;
            if (in_val == 0)
            {
                break;
            }
            t2 = (uint8)(in_val & (0xff >> 2));//取低6bit
            pin++;
            in_val = *pin;
            if (in_val == 0)
            {
                break;
            }
            t3 = (uint8)(in_val & (0xff >> 2));//取低6bit

            //Little Endian
            //(t2低2bit 放在最高2bit,或上t3的低6bit);
            tmpval[0] = (uint8)(((t2 & (0xff >> 6)) << 6) | t3);
            //(t1 的低4bit放在字节的高4bit,或上t2的中间4bit)
            tmpval[1] = (t1 << 4) | (t2 >> 2);
        }
        else
        {
            //do nothing for QAC
        }
        pin++;
        resultsize += 2;
        *dest = tmpval[1];
        *dest = *dest << 8;
        *dest |= tmpval[0];
        dest++;
    }
    resultsize += 2;
    *size = resultsize;
    *dest = 0;
}
/*! \endcond */
