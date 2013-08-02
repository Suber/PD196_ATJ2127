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
 * \file     ui_get_textbox_attrb.c
 * \brief    获取textbox的显示区域
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现了textbox的显示区域的获取。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用sd文件系统接口函数读取textbox的显示区域信息。
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    获取textbox的数据信息
 * \param[in]    textbox_style 指向textbox的style结构体变量
 * \param[in]    type 要获取的数据信息的类型
 * \param[out]   attrb 返回textbox的数据信息
 * \return       ui_result_e 类型的参数，参见ui_result_e定义
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    例子1：获取 textbox 控件显示区域
    style_infor_t style_infor;
    region_t textbox_region;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_textbox_attrb(&style_infor, &textbox_region, 0);
    
    例子2：获取 textbox 控件属性，包括字体大小等
    style_infor_t style_infor;
    uint16 attrib;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_textbox_attrb(&style_infor, &attrib, 1);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e get_textbox_attrb(style_infor_t *textbox_style, void *attrb, uint8 type)
{
    textbox_t textbox;
    res_open_infor_t *res_ptr;
    uint32 textbox_entry_addr;
    
    /*根据打开资源文件的类型，将指针指向不同的全局变量*/
    if (textbox_style->type == UI_AP)
    {
        res_ptr = &g_ap_res;
    }
    else
    {
        res_ptr = &g_com_res;
    }
    get_ctrl_addr(TEXTBOX, &textbox_entry_addr, res_ptr);
    //读取当前textbox的style信息
    sys_sd_fseek(res_ptr->res_fp, SEEK_SET, textbox_entry_addr + (uint32) (textbox_style->style_id & 0x0fff)
            * sizeof(textbox_t));
    sys_sd_fread(res_ptr->res_fp, &textbox, sizeof(textbox_t));
    
    if (type == 0)//获取 textbox 显示区域
    {
        region_t *tmp_region = (region_t *) attrb;
        
        tmp_region->x = textbox.x;
        tmp_region->y = textbox.y;
        tmp_region->width = textbox.width;
        tmp_region->height = textbox.height;
    }
    else if(type == 1)//获取textbox控件属性
    {
        uint16 *attrib = (uint16 *) attrb;
        
        *attrib = textbox.attrib;
    }
    else//参数错误
    {
        return UI_PARAM_ERR;
    }
    
    return UI_NO_ERR;
}
/*! \endcond */
