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
 * \file     ui_show_parambox_2.c
 * \brief    显示parambox的title和 unit的模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现title和 unit项控件数据的解析并显示title和 unit
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用系统SD文件系统的接口函数解析并显示title和 unit
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

#define PARAMBOX_LISTDIRECT_OPTION_SHIFT    (4)
#define PARAMBOX_LISTDIRECT_OPTION      (0x0001 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)
#define PARAMBOX_LISTDIRECT_HORIZONTAL  (0x0000 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)
#define PARAMBOX_LISTDIRECT_VERTICAL    (0x0001 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)

#define TITLE_ALIGNMENT_OPTION_SHIFT    (0)
#define TITLE_ALIGNMENT_OPTION          (0x0003 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_ALIGNMENT_OPTION_LEFT     (0x0000 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_ALIGNMENT_OPTION_CENTER   (0x0001 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_ALIGNMENT_OPTION_RIGHT    (0x0002 << TITLE_ALIGNMENT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_SHIFT         (2)
#define TITLE_FONT_OPTION               (0x0003 << TITLE_FONT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_SMALL         (0x0000 << TITLE_FONT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_MIDDLE        (0x0001 << TITLE_FONT_OPTION_SHIFT)
#define TITLE_FONT_OPTION_LARGE         (0x0002<< TITLE_FONT_OPTION_SHIFT)

#define UNIT_ALIGNMENT_OPTION_SHIFT     (0)
#define UNIT_ALIGNMENT_OPTION           (0x0003 << UNIT_ALIGNMENT_OPTION_SHIFT)
#define UNIT_ALIGNMENT_OPTION_LEFT      (0x0000 << UNIT_ALIGNMENT_OPTION_SHIFT)
#define UNIT_ALIGNMENT_OPTION_CENTER    (0x0001 << UNIT_ALIGNMENT_OPTION_SHIFT)
#define UNIT_ALIGNMENT_OPTION_RIGHT     (0x0002 << UNIT_ALIGNMENT_OPTION_SHIFT)
#define UNIT_FONT_OPTION_SHIFT          (2)
#define UNIT_FONT_OPTION                (0x0003 << UNIT_FONT_OPTION_SHIFT)
#define UNIT_FONT_OPTION_SMALL          (0x0000 << UNIT_FONT_OPTION_SHIFT)
#define UNIT_FONT_OPTION_MIDDLE         (0x0001 << UNIT_FONT_OPTION_SHIFT)
#define UNIT_FONT_OPTION_LARGE          (0x0002 << UNIT_FONT_OPTION_SHIFT)
/*! \cond */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示parambox的title
 * \param[in]   addr：parambox的title在资源文件中的偏移地址
 * \param[in]   title_id：应用传递的title资源ID
 * \param[in]   style_ptr：当前资源信息结构体指针
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_parambox_title(uint32 addr, uint16 title_id, res_open_infor_t *style_ptr)
{
    parambox_title_t parambox_title;
    region_t title_region;
    string_desc_t desc;
    uint16 id;
    uint16 align_mode = 0;

    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, addr);
    sys_sd_fread(style_ptr->res_fp, &parambox_title, sizeof(parambox_title_t));
    //atrib<2-3>0表示小字体,1表示中号字体，2表示大字体，默认中号字体
    if ((parambox_title.attrib & TITLE_FONT_OPTION) == TITLE_FONT_OPTION_SMALL)
    {
        align_mode |= DISP_FONT_SMALL;
    }
    else if ((parambox_title.attrib & TITLE_FONT_OPTION) == TITLE_FONT_OPTION_LARGE)
    {
        align_mode |= DISP_FONT_LARGE;
    }
    else
    {
        align_mode |= DISP_FONT_NORMAL; //默认中号字体
    }

    romf_set_pen_color(parambox_title.color, 0, 0);
    //显示区域设置
    title_region.x = parambox_title.title_x;
    title_region.y = parambox_title.title_y;
    title_region.width = parambox_title.title_width;
    title_region.height = parambox_title.title_height;

    if (title_id != (uint16) - 1)
    {
        id = title_id;
    }
    else if (parambox_title.title_id != (uint16) - 1)
    {
        id = parambox_title.title_id;
    }
    else
    {
        id = (uint16) - 1;
    }
    //居左显示
    if ((parambox_title.attrib & TITLE_ALIGNMENT_OPTION) == TITLE_ALIGNMENT_OPTION_LEFT)
    {
        align_mode |= DISP_ALIGN_LEFT;
    }
    //居右显示
    else if ((parambox_title.attrib & TITLE_ALIGNMENT_OPTION) == TITLE_ALIGNMENT_OPTION_RIGHT)
    {
        align_mode |= DISP_ALIGN_RIGHT;
    }
    //默认居中显示
    else
    {
        align_mode |= DISP_ALIGN_MEDIACY;
    }

    desc.data.id = id;
    desc.language = UNICODEID;
    show_string(&desc, &title_region, align_mode); //默认居中对齐
}

/******************************************************************************/
/*!
 * \par  Description:
 *    显示parambox的unit
 * \param[in]   parambox：parambox控件数据结构体变量
 * \param[in]   parambox_data：应用传递的parambox的私有数据结构体指针
 * \param[in]   itemnum：第几个参数项
 * \param[in]   style_ptr：当前资源信息结构体指针
 * \param[out]  none
 * \return      none
 * \note
 *******************************************************************************/
void show_parambox_unit(parambox_t *parambox, parambox_private_t *parambox_data, uint16 itemnum,
        res_open_infor_t *style_ptr)
{
    parambox_unit_t parambox_unit;
    region_t unit_region;
    uint16 align_mode = 0;

    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, parambox->unit_addr);
    sys_sd_fread(style_ptr->res_fp, &parambox_unit, sizeof(parambox_unit_t));
    //set_textout_mode(0,0,0); //默认为透明模式
    romf_set_pen_color(parambox_unit.uint_color, 0, 0);
    //显示区域设置
    if(parambox->param_cnt > 1)//多参数
    {
        if((parambox->attrib & PARAMBOX_LISTDIRECT_OPTION) == PARAMBOX_LISTDIRECT_HORIZONTAL)//横向排列
        {
            unit_region.x = parambox->list_x[itemnum];
            unit_region.y = parambox_unit.unit_y;
            unit_region.width = parambox->list_width[itemnum];
            unit_region.height = parambox_unit.unit_height;
        }
        else//竖向排列
        {
            unit_region.x = parambox_unit.unit_x;
            unit_region.y = parambox->list_y[itemnum];
            unit_region.width = parambox_unit.unit_width;
            unit_region.height = parambox->list_height[itemnum];
        }
    }
    else//单参数
    {
        unit_region.x = parambox_unit.unit_x;
        unit_region.y = parambox_unit.unit_y;
        unit_region.width = parambox_unit.unit_width;
        unit_region.height = parambox_unit.unit_height;
    }
    
    //atrib<11-12>0表示小字体,1表示中号字体，2表示大字体，默认中号字体
    if ((parambox_unit.attrib & UNIT_FONT_OPTION) == UNIT_FONT_OPTION_SMALL)
    {
        align_mode |= DISP_FONT_SMALL;
    }
    else if ((parambox_unit.attrib & UNIT_FONT_OPTION) == UNIT_FONT_OPTION_LARGE)
    {
        align_mode |= DISP_FONT_LARGE;
    }
    else
    {
        align_mode |= DISP_FONT_NORMAL; //默认中号字体
    }
    //set_font_size(font_size);
    //居左显示
    if ((parambox_unit.attrib & UNIT_ALIGNMENT_OPTION) == UNIT_ALIGNMENT_OPTION_LEFT)
    {
        align_mode |= DISP_ALIGN_LEFT;
    }
    //居右显示
    else if ((parambox_unit.attrib & UNIT_ALIGNMENT_OPTION) == UNIT_ALIGNMENT_OPTION_RIGHT)
    {
        align_mode |= DISP_ALIGN_RIGHT;
    }
    //默认居中显示
    else
    {
        align_mode |= DISP_ALIGN_MEDIACY;
    }
    align_mode |= DISP_ARABIC_DISABLE;
    if (parambox_data->items[itemnum].unit_id != (uint16) - 1)
    {
        string_desc_t desc;

        desc.data.id = parambox_data->items[itemnum].unit_id;
        desc.language = UNICODEID;
        show_string(&desc, &unit_region, align_mode); //默认居中对齐
    }
}
/*! \endcond */
