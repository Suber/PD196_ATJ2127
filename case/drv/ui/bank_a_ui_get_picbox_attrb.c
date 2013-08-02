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
 * \file     ui_get_picbox_attrb.c
 * \brief    获取picbox的数据信息
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现了picbox的数据信息获取。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用sd文件系统接口函数读取picbox的数据信息。
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    获取picbox的数据信息
 * \param[in]    picbox_style 指向picbox的style结构体变量
 * \param[in]    type 要获取的数据信息的类型
 * \param[out]   attrb 返回picbox的数据信息
 * \return       ui_result_e 类型的参数，参见ui_result_e定义
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    例子1：获取 picbox 控件显示区域
    style_infor_t style_infor;
    region_t pic_region;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_picbox_attrb(&style_infor, &pic_region, 0);//type传0，表示显示区域
    
    例子2：获取 picbox 控件帧数
    style_infor_t style_infor;
    uint8 frame_cnt;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_picbox_attrb(&style_infor, &frame_cnt, 1);//type传1，表示帧数
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e get_picbox_attrb(style_infor_t *picbox_style, void *attrb, uint8 type)
{
    picbox_t picbox;
    res_open_infor_t *style_ptr;

    /*根据打开资源文件的类型，将指针指向不同的全局变量*/
    if (picbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, style_ptr->style_picbox_offset + (uint32) (picbox_style->style_id
            & 0x0fff) * sizeof(picbox_t));
    sys_sd_fread(style_ptr->res_fp, &picbox, sizeof(picbox_t));

    switch (type)
    {
        case 0://获取显示区域
        {
            region_t *picbox_region = (region_t *) attrb;

            picbox_region->x = picbox.x;
            picbox_region->y = picbox.y;
            break;
        }

        case 1://获取picbox帧数
        {
            uint8 *frame_cnt = (uint8 *) attrb;

            *frame_cnt = (uint8)(((picbox.attrib) >> 8) & 0xff);
            break;
        }

        default://参数错误
        {
            return UI_PARAM_ERR;
        }
    }

    return UI_NO_ERR;
}
/*! \endcond */
