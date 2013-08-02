/*******************************************************************************
 *                              US212A
 *                            Module: ui driver
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>       <time>           <version >             <desc>
 *       betty     2011-09-07 16:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     ui_get_listbox_attrb.c
 * \brief    获取picbox的数据信息
 * \author   betty
 * \par      GENERAL DESCRIPTION:
 *               实现lsitbox控件数据的获取
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用系统SD文件系统的接口函数解析listbox的数据
 * \version 1.0
 * \date  2011/09/07
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    获取listbox的数据信息
 * \param[in]    listbox_style 指向listbox的style结构体变量
 * \param[in]    type 要获取的数据信息的类型
 * \param[out]   attrb 返回listbox的数据信息
 * \return       ui_result_e 类型的参数，参见ui_result_e定义
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    例子1：获取 listbox 控件每页行数
    style_infor_t style_infor;
    uint8 one_page_count;
    
    style_infor.style_id = style_id;
    style_infor.type = UI_COM;
    
    ui_get_listbox_attrb(&style_infor, &one_page_count, 0);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e get_listbox_attrb(style_infor_t *listbox_style, void *attrb, uint8 type)
{
    listbox_t listbox;
    res_open_infor_t *style_ptr;
    uint32 listbox_entry_addr;

    /*根据打开资源文件的类型，将指针指向不同的全局变量*/
    if (listbox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }

    get_ctrl_addr(LISTBOX, &listbox_entry_addr, style_ptr); //获取listbox控件在UI配置文件中的开始位置
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, listbox_entry_addr + (uint32) (listbox_style->style_id & 0x0fff)
            * sizeof(listbox_t));
    sys_sd_fread(style_ptr->res_fp, &listbox, sizeof(listbox_t));
    
    if(type == 0)//获取 listbox 每页显示行数
    {
        *(uint8 *)attrb = listbox.num_one_page;
    }
    else//参数错误
    {
        return UI_PARAM_ERR;
    }

    return UI_NO_ERR;
}
/*! \endcond */
