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
 * \file     ui_draw_rect_frame.c
 * \brief    填充具有特殊效果区域的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现了指定区域边框的特殊效果的填充。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用区域填充函数fill_rect实现边框填充。
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *	  填充具有特殊效果的区域边框
 * \param[in]    frame_region：需要填充区域的指针
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/

void draw_rect_frame(region_t * frame_region)
{
    libc_memcpy(&region, frame_region, sizeof(region_t));
    //画上边
    region.height = 1;
    romf_fill_rect(&region, (uint16) (dc.pen_color & 0xffff));

    //画下边
    region.y = frame_region->y + frame_region->height;
    romf_fill_rect(&region, (uint16) (dc.pen_color & 0xffff));

    libc_memcpy(&region, frame_region, sizeof(region_t));

    //画左边
    region.width = 1;
    romf_fill_rect(&region, (uint16) (dc.pen_color & 0xffff));

    //画右边
    region.x = frame_region->x + frame_region->width - 1;
    romf_fill_rect(&region, (uint16) (dc.pen_color & 0xffff));

    return;
}
/*! \endcond */

