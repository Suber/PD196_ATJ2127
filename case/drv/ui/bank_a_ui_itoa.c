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
 * \file     ui_itoa.c
 * \brief    数字转换成字符串的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *             将数字转换成对应字符串
 * \par      EXTERNALIZED FUNCTIONS:
 *              none
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
uint8 *itoa_n(int32 num, uint8 *str, uint8 radix, uint16 counts, uint16 disp_mode)
{
    uint8 * ptr = str;

    if (num < 0)
    {
        num = -num;
        if (disp_mode == 1)
        {
            radix = ':'; //如果显示数值为负数，则加上负号
        }
        else
        {
            radix = '-';
        }
    }
    else if (radix != 0)
    {
        if (disp_mode == 1)
        {
            radix = ';'; //如果需要显示正号，则加上正号
        }
        else
        {
            radix = '+';
        }
    }
    else
    {

    }
    if (radix != 0)
    {
        ptr[0] = radix;
        ptr = ptr + 1;
    }

    libc_itoa((uint32)num, ptr, counts);
    return str;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  显示数字图片
 * \param[in]   num：当前数字
 * \param[in]   x：图片的X坐标
 * \param[in]   y：图片的Y坐标
 * \param[in]   res_entry：数字0图片的资源结构信息
 * \param[in]   res_ptr：当前资源信息结构体指针
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void show_num_pic(uint16 num, uint16 x, uint16 y, res_entry_t *res_entry, res_open_infor_t *res_ptr)
{
    region_t res_region;
    uint32 addr = res_entry->offset;
    uint32 pic_bytes = (res_entry->width * res_entry->height) * 2;
    //设窗参数设置
    res_region.x = x;
    res_region.y = y;
    res_region.width = (uint8) res_entry->width;
    res_region.height = (uint8) res_entry->height;
    //读取图片数据并刷屏
    while (num != 0)
    {
        num--;
        addr += pic_bytes;
    }
    put_sd_image(res_ptr->res_fp, addr, &res_region);
}
/*! \endcond */
