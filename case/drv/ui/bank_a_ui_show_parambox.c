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
 * \file     ui_show_parambox.c
 * \brief    显示ParamBox的函数模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现parambox控件数据的解析并显示parambox
 * \par      EXTERNALIZED FUNCTIONS:
 *              直接调用系统SD文件系统的接口函数解析并显示parmabox
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"

//parambox attribute bits location
#define PARAMBOX_SHOW_EN                (0x0001 << 0)
#define PARAMBOX_SHOW_TITLE_EN          (0x0001 << 1)
#define PARAMBOX_SHOW_NUM_EN            (0x0001 << 2)
#define PARAMBOX_SHOW_UNIT_EN           (0x0001 << 3)
#define PARAMBOX_LISTDIRECT_OPTION_SHIFT    (4)
#define PARAMBOX_LISTDIRECT_OPTION      (0x0001 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)
#define PARAMBOX_LISTDIRECT_HORIZONTAL  (0x0000 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)
#define PARAMBOX_LISTDIRECT_VERTICAL    (0x0001 << PARAMBOX_LISTDIRECT_OPTION_SHIFT)
#define PARAMBOX_SHOW_TAGPIC_EN         (0x0001 << 5)
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示parambox
 * \param[in]   parambox_style 指向parambox的style结构体变量
 * \param[in]   parambox_data 应用传递的parambox的私有数据结构体指针
 * \param[in]   mode parambox的刷新模式,参见parambox_draw_mode_e定义
 * \param[out]  none
 * \return      ui_result_e 类型的参数，参见ui_result_e定义
 * \ingroup     ui_controls
 * \par         exmaple code
 * \code
    由 gui_set_parameter 封装，以下实例是直接从 gui_set_parameter 拷贝
    style_infor_t parambox_style;
    parambox_private_t private_data;
    parambox_one_t param_item;
    
    parambox_style.style_id = PARAMBOX_ID1;
    parambox_style.type = UI_AP;
    
    param_item.unit_id = V_U16_INVALID;
    param_item.min = 0;
    param_item.max = 40;
    param_item.step = 1;
    param_item.value = 24;
    param_item.cycle = FALSE;
    param_item.max_number = 2;
    param_item.value_str = NULL;
    param_item.adjust_func = NULL;
    param_item.callback = volumn_cb;
    
    private_data.back_id = V_U16_INVALID;
    private_data.icon_id = P_SPEAKER;
    private_data.title_id = V_U16_INVALID;
    private_data.param_cnt = 1;
    private_data.items = &param_item;
    private_data.active = 0;
    private_data.sign_flag = 0;
    private_data.scale = 0;//非slider模板，无须理会
    
    ui_show_parambox(&parambox_style, &private_data, PARAMBOX_DRAW_ALL);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e show_parambox(style_infor_t *parambox_style, parambox_private_t *parambox_data, parambox_draw_mode_e mode)
{
    parambox_t parambox;
    DC oldDC_sav;
    uint32 parambox_entry_addr;
    res_open_infor_t *style_ptr;
    uint8 i;
    uint8 type;
    /*根据打开资源文件的类型，将指针指向不同的全局变量*/
    if (parambox_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    //获取parambox在资源文件中的起始地址
    get_ctrl_addr(PARAMBOX, &parambox_entry_addr, style_ptr);
    //获取当前parambox的style信息
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, parambox_entry_addr + (uint32) (parambox_style->style_id & 0x0fff)
            * sizeof(parambox_t));
    sys_sd_fread(style_ptr->res_fp, &parambox, sizeof(parambox_t));

    //不显示parambox
    if ((parambox.attrib & PARAMBOX_SHOW_EN) != PARAMBOX_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    //保存DC
    romf_get_DC_status(&oldDC_sav, 0, 0);
    //初始化全部更新
    if (mode == PARAMBOX_DRAW_ALL)
    {
        //显示parambox的背景，优先显示用户指定的背景图片
        if (parambox_data->back_id != (uint16) - 1)
        {
            u_show_pic(parambox_data->back_id, parambox.back_x, parambox.back_y);
        }
        else if (parambox.back_id != (uint16) - 1)
        {
            u_show_pic(parambox.back_id, parambox.back_x, parambox.back_y);
        }
        else
        {
            //do nothing for QAC
        }
        
        //显示参数分隔符，最多支持两个分隔符
        for(i = 0; i < 2; i++)
        {
            if (parambox.param_part_id[i] != (uint16) - 1)
            {
                u_show_pic(parambox.param_part_id[i], parambox.param_part_x[i], parambox.param_part_y[i]);
            }
        }
        
        //显示参数标志图
        if ((parambox.attrib & PARAMBOX_SHOW_TAGPIC_EN) == PARAMBOX_SHOW_TAGPIC_EN)
        {
            if (parambox_data->icon_id != (uint16) - 1)
            {
                u_show_pic(parambox_data->icon_id, parambox.icon_x, parambox.icon_y);
            }
            else if (parambox.icon_id != (uint16) - 1)
            {
                u_show_pic(parambox.icon_id, parambox.icon_x, parambox.icon_y);
            }
            else
            {
                //do nothing for QAC
            }
        }
        
        //显示标题字符串
        if ((parambox.attrib & PARAMBOX_SHOW_TITLE_EN) == PARAMBOX_SHOW_TITLE_EN)
        {
            show_parambox_title(parambox.title_addr, parambox_data->title_id, style_ptr);
        }
        
        //显示参数值列表
        for (i = 0; i < parambox_data->param_cnt; i++)
        {
            if ((parambox.attrib & PARAMBOX_SHOW_UNIT_EN) == PARAMBOX_SHOW_UNIT_EN)
            {
                show_parambox_unit(&parambox, parambox_data, i, style_ptr);
            }
            if (i == parambox_data->active)
            {
                type = 1;
            }
            else
            {
                type = 0;
            }
            if ((parambox.attrib & PARAMBOX_SHOW_NUM_EN) == PARAMBOX_SHOW_NUM_EN)
            {
                show_parambox_num(&parambox, parambox_data, i, type, mode, style_ptr);
            }
        }
    }
    else if (mode == PARAMBOX_DRAW_PARAM)
    {
        if ((parambox.attrib & PARAMBOX_SHOW_NUM_EN) == PARAMBOX_SHOW_NUM_EN)
        {
            show_parambox_num(&parambox, parambox_data, parambox_data->old, 0, mode, style_ptr);
            show_parambox_num(&parambox, parambox_data, parambox_data->active, 1, mode, style_ptr);
        }
    }
    else if (mode == PARAMBOX_DRAW_VALUE)
    {
        if ((parambox.attrib & PARAMBOX_SHOW_NUM_EN) == PARAMBOX_SHOW_NUM_EN)
        {
            show_parambox_num(&parambox, parambox_data, parambox_data->active, 1, mode, style_ptr);
        }
    }
    else
    {
        //do nothing for QAC
    }
    //恢复画笔颜色
    romf_set_pen_color(oldDC_sav.pen_color, 0, 0);
    romf_set_backgd_color(oldDC_sav.backgd_color, 0, 0);
    return UI_NO_ERR;
}
/*! \endcond */
