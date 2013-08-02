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
 * \file     ui_draw_3Drect.c
 * \brief    填充具有特殊效果区域的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现了具有特色效果指定区域的填充。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用区域填充函数fill_rect实现指定区域的填充。
 * \version  1.0
 * \date  2011/9/09
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *	  实现填充具有特殊效果的区域
 * \param[in]    D3region：指向要填充的区域
 * \param[in]    type：要填充的效果类型
 * \param[out]   none
 * \return       none
 * \note
 *******************************************************************************/
void draw_3D_rect(region_t * D3region, uint8 type)
{
    COLORREF pen_color_bak;
    pen_color_bak = dc.pen_color;//保存当前画笔颜色

    switch (type)
    {
        case D3_NORMAL_DEF://加选中框
        dc.pen_color = SelectedColor_def;
        draw_rect_frame(D3region);
        break;

        case D3_KEYUP_DEF://加突出效果
        //先画两条背光边
        dc.pen_color = BackLightColor_def;
        libc_memcpy(&region, D3region, sizeof(region_t));
        region.height = 1;
        romf_fill_rect(&region, BackLightColor_def);

        libc_memcpy(&region, D3region, sizeof(region_t));
        region.width = 1;
        region.x = D3region->x + D3region->width;
        romf_fill_rect(&region, BackLightColor_def);

        //再画两条向光边
        dc.pen_color = FaceLightColor_def;
        libc_memcpy(&region, D3region, sizeof(region_t));
        region.height = 1;
        romf_fill_rect(&region, FaceLightColor_def);

        libc_memcpy(&region, D3region, sizeof(region_t));
        region.width = 1;
        romf_fill_rect(&region, FaceLightColor_def);
        break;

        case D3_KEYDOWN_DEF://加凹进效果
        //先画两条背光边
        dc.pen_color = BackLightColor_def;
        libc_memcpy(&region, D3region, sizeof(region_t));
        region.height = 1;
        romf_fill_rect(&region, BackLightColor_def);

        libc_memcpy(&region, D3region, sizeof(region_t));
        region.width = 1;
        romf_fill_rect(&region, BackLightColor_def);

        //再画两条向光边
        dc.pen_color = FaceLightColor_def;
        libc_memcpy(&region, D3region, sizeof(region_t));
        region.height = 1;
        region.y = D3region->y + D3region->height;
        romf_fill_rect(&region, FaceLightColor_def);

        libc_memcpy(&region, D3region, sizeof(region_t));
        region.width = 1;
        region.x = D3region->x + D3region->width;
        romf_fill_rect(&region, FaceLightColor_def);
        break;

        default:
        break;
    }

    dc.pen_color = pen_color_bak;//恢复画笔颜色
    return;
}
/*! \endcond */

