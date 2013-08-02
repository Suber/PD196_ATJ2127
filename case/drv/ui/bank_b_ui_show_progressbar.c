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
 * \file     ui_show_progressbar.c
 * \brief    显示progressbar的函数模块
 * \author   liutingting
 * \par      GENERAL DESCRIPTION:
 *               实现了progressbar控件的显示。
 * \par      EXTERNALIZED FUNCTIONS:
 *               调用系统sd文件系统接口及图片显示函数来完成progressbar的解析和显示。
 * \version 1.0
 * \date  2011-9-05
 *******************************************************************************/
#include "display.h"
#include "ui_driver.h"
//progressbar attribute bits location
#define PROGRESS_SHOW_EN                (0x0001 << 0)
#define PROGRESS_DIRECTION_OPTION_SHIFT (1)
#define PROGRESS_DIRECTION_OPTION       (0x0001 << PROGRESS_DIRECTION_OPTION_SHIFT)
#define PROGRESS_DIRECTION_HORIZONTAL   (0x0000 << PROGRESS_DIRECTION_OPTION_SHIFT)
#define PROGRESS_DIRECTION_VERTICAL     (0x0001 << PROGRESS_DIRECTION_OPTION_SHIFT)
#define PROGRESS_STYLE_OPTION_SHIFT     (2)
#define PROGRESS_STYLE_OPTION           (0x0003 << PROGRESS_STYLE_OPTION_SHIFT)
#define STYLE_OPTION_FILL               (0x0000 << PROGRESS_STYLE_OPTION_SHIFT)
#define STYLE_OPTION_CURSOR             (0x0001 << PROGRESS_STYLE_OPTION_SHIFT)
#define STYLE_OPTION_BOTH               (0x0002 << PROGRESS_STYLE_OPTION_SHIFT)
/*! \cond UI_DRIVER_API */
/******************************************************************************/
/*!
 * \par  Description:
 *    显示progressbar
 * \param[in]    progressbar_style 指向控件style信息的结构体变量
 * \param[in]    progressbar_data 指向progressba私有数据结构体的变量
 * \param[in]    mode progressbar的刷新模式，参见progress_draw_mode_e定义
 * \return       ui_result_e 类型的参数，参见ui_result_e定义
 * \ingroup      ui_controls
 * \par          exmaple code
 * \code
    例子1：初始化显示进度条
    style_infor_t progressbar_style;
    progressbar_private_t progressbar_param;
    
    progressbar_style.style_id = PROGRESS_ID1;
    progressbar_style.type = UI_AP;
    progressbar_param.value = g_cur_time;
    progressbar_param.total = g_total_time;
    
    ui_show_progressbar(&progressbar_style, &progressbar_param, PROGRESS_DRAW_ALL);
    
    例子2：更新显示进度条
    其他代码一样，最后模式为 PROGRESS_DRAW_PROGRESS
    ui_show_progressbar(&progressbar_style, &progressbar_param, PROGRESS_DRAW_PROGRESS);
 * \endcode
 * \note
 *******************************************************************************/
ui_result_e show_progressbar(style_infor_t *progressbar_style, progressbar_private_t *progressbar_data,
        progress_draw_mode_e mode)
{
    progressbar_t progressbar;
    uint32 progressbar_entry_addr;
    res_open_infor_t *style_ptr;
    uint16 width_offset = 0;
    transparent_pic_t trs_pic;
    res_entry_t tmp_entry;
    uint16 tmp_prev_num;
    uint16 tmp_y;
    
    tmp_entry.height = 0;//for QAM

    /*根据打开资源文件的类型，将指针指向不同的全局变量*/
    if (progressbar_style->type == UI_AP)
    {
        style_ptr = &g_ap_res;
    }
    else
    {
        style_ptr = &g_com_res;
    }
    get_ctrl_addr(PROGRESSBAR, &progressbar_entry_addr, style_ptr);
    sys_sd_fseek(style_ptr->res_fp, SEEK_SET, progressbar_entry_addr + (uint32) (progressbar_style->style_id & 0x0fff)
            * sizeof(progressbar_t));
    sys_sd_fread(style_ptr->res_fp, &progressbar, sizeof(progressbar_t));
    //不显示progressbar
    if ((progressbar.attrib & PROGRESS_SHOW_EN) != PROGRESS_SHOW_EN)
    {
        return UI_NO_DISPLAY;
    }
    //没有数据，返回
    if (progressbar_data == NULL)
    {
        if (progressbar.back_id != (uint16) - 1)
        {
            romf_show_pic(progressbar.back_id, progressbar.back_x, progressbar.back_y);
        }
        return UI_PARAM_ERR;
    }
    if (progressbar_data->total > 0)
    {
        uint32 tmp_width_offset;
        
        tmp_width_offset = progressbar_data->value * progressbar.length * 100;
        tmp_width_offset /= progressbar_data->total;
        if ((tmp_width_offset != 0) && (tmp_width_offset < 100))
        {
            tmp_width_offset = 100;
        }
        width_offset = (uint16)(tmp_width_offset/100);
    }
    
    if ((mode == PROGRESS_DRAW_PROGRESS_RIGHT) || (mode == PROGRESS_DRAW_RIGHT_INIT))//右箭头，用prev_num[1]
    {
        tmp_prev_num = prev_num[1];
    }
    else
    {
        tmp_prev_num = prev_num[0];
    }
    
    if ((mode == PROGRESS_DRAW_PROGRESS) && (width_offset == tmp_prev_num))//不需要更新进度，立即退出
    {
        return UI_NO_ERR;
    }
    
    trs_pic.back_id = progressbar.widget_bg;
    trs_pic.back_color = 0;
    if((progressbar.attrib & PROGRESS_DIRECTION_OPTION) == PROGRESS_DIRECTION_HORIZONTAL)
    {
        trs_pic.direct = 0;//横向进度条
        trs_pic.back_x = progressbar.back_x;
        trs_pic.back_y = (progressbar.back_y < progressbar.tag_y)? progressbar.back_y : progressbar.tag_y;
    }
    else
    {
        trs_pic.direct = 1;//竖向进度条
        trs_pic.back_x = (progressbar.back_x < progressbar.tag_x)? progressbar.back_x : progressbar.tag_x;
        trs_pic.back_y = progressbar.back_y;
    }
    trs_pic.pic_id = progressbar.tag_id;
    
    if((progressbar.attrib & PROGRESS_DIRECTION_OPTION) == PROGRESS_DIRECTION_VERTICAL)//竖向从低向上刷
    {
        style_ptr = res_ptr_select(progressbar.tag_id);
        sys_sd_fseek(style_ptr->res_fp, SEEK_SET, \
            style_ptr->pic_entry_offset + (uint32) (REAL_RES_ID(progressbar.tag_id) * RESHEADITEM));
        sys_sd_fread(style_ptr->res_fp, &tmp_entry, sizeof(res_entry_t));
    }
    
    //初始显示进度条，刷背景，清除所有痕迹
    if (mode == PROGRESS_DRAW_ALL)
    {
        if (trs_pic.back_id != (uint16) - 1)
        {
            //romf_show_pic(progressbar.back_id, progressbar.back_x, progressbar.back_y);
            romf_show_pic(trs_pic.back_id, trs_pic.back_x, trs_pic.back_y);
        }
        
        //PROGRESS_DRAW_PROGRESS_RIGHT 模式不会使用 PROGRESS_DRAW_ALL 全部刷新，故直接使用 prev_num[0] 即可
        tmp_prev_num = prev_num[0] = 0;
    }
    
    //仅刷游标模式
    if ((progressbar.attrib & PROGRESS_STYLE_OPTION) == STYLE_OPTION_CURSOR)
    {
        //先清除旧游标痕迹
        if ((mode != PROGRESS_DRAW_ALL) && (mode != PROGRESS_DRAW_LEFT_INIT) && (mode != PROGRESS_DRAW_RIGHT_INIT))
        {
            if((progressbar.attrib & PROGRESS_DIRECTION_OPTION) == PROGRESS_DIRECTION_HORIZONTAL)
            {
                //romf_show_pic(progressbar.tag_clear_id, progressbar.prog_start_x + tmp_prev_num, progressbar.tag_y);
                trs_pic.pic_x = progressbar.prog_start_x + tmp_prev_num;
                trs_pic.pic_y = progressbar.tag_y;
                trs_pic.clear = 1;//仅仅清除痕迹
                show_pic_transparent(&trs_pic);
            }
            else
            {
                tmp_y = progressbar.prog_start_y - (tmp_entry.height - 1) - tmp_prev_num;
                //romf_show_pic(progressbar.tag_clear_id, progressbar.tag_x, tmp_y);
                trs_pic.pic_x = progressbar.tag_x;
                trs_pic.pic_y = tmp_y;
                trs_pic.clear = 1;//仅仅清除痕迹
                show_pic_transparent(&trs_pic);
            }
        }
        
        //再刷新游标
        if((progressbar.attrib & PROGRESS_DIRECTION_OPTION) == PROGRESS_DIRECTION_HORIZONTAL)
        {
            //romf_show_pic(progressbar.tag_id, progressbar.prog_start_x + width_offset, progressbar.tag_y);
            trs_pic.pic_x = progressbar.prog_start_x + width_offset;
            trs_pic.pic_y = progressbar.tag_y;
            trs_pic.clear = 0;//刷透明游标
            show_pic_transparent(&trs_pic);
        }
        else
        {
            tmp_y = progressbar.prog_start_y - (tmp_entry.height - 1) - width_offset;
            //romf_show_pic(progressbar.tag_id, progressbar.tag_x, tmp_y);
            trs_pic.pic_x = progressbar.tag_x;
            trs_pic.pic_y = tmp_y;
            trs_pic.clear = 0;//刷透明游标
            show_pic_transparent(&trs_pic);
        }
    }
    //带进度填充模式，包括进度填充+游标，仅进度填充
    else if (((progressbar.attrib & PROGRESS_STYLE_OPTION) == STYLE_OPTION_FILL) 
          || ((progressbar.attrib & PROGRESS_STYLE_OPTION) == STYLE_OPTION_BOTH))
    {
        progressbar.prog_start_x += tmp_prev_num;
        //有游标，先清除旧游标痕迹
        if ((progressbar.attrib & PROGRESS_STYLE_OPTION) == STYLE_OPTION_BOTH)
        {
            if ((mode != PROGRESS_DRAW_ALL) && (mode != PROGRESS_DRAW_LEFT_INIT) && (mode != PROGRESS_DRAW_RIGHT_INIT))
            {
                if((progressbar.attrib & PROGRESS_DIRECTION_OPTION) == PROGRESS_DIRECTION_HORIZONTAL)
                {
                    //romf_show_pic(progressbar.tag_clear_id, progressbar.prog_start_x, progressbar.tag_y);
                    trs_pic.pic_x = progressbar.prog_start_x;
                    trs_pic.pic_y = progressbar.tag_y;
                    trs_pic.clear = 1;//仅仅清除痕迹
                    show_pic_transparent(&trs_pic);
                }
                else
                {
                    tmp_y = progressbar.prog_start_y - (tmp_entry.height - 1) - tmp_prev_num;
                    //romf_show_pic(progressbar.tag_clear_id, progressbar.tag_x, tmp_y);
                    trs_pic.pic_x = progressbar.tag_x;
                    trs_pic.pic_y = tmp_y;
                    trs_pic.clear = 1;//仅仅清除痕迹
                    show_pic_transparent(&trs_pic);
                }
            }
        }

        //暂时不支持竖向进度填充
        if((progressbar.attrib & PROGRESS_DIRECTION_OPTION) == PROGRESS_DIRECTION_HORIZONTAL)
        {
            //进度增加
            if (width_offset > tmp_prev_num)
            {
                region_t fill_region;
    
                fill_region.x = progressbar.prog_start_x;
                fill_region.y = progressbar.fill_y;
                fill_region.width = width_offset - tmp_prev_num;
                fill_region.height = 0;
                progressbar.prog_start_x = fill_region.x + fill_region.width;
    
                fill_rect_dump(&fill_region, progressbar.fill_id, DUMP_LEFT2RIGHT);
            }
            //进度减少
            else if (width_offset < tmp_prev_num)
            {
                region_t fill_region;
    
                fill_region.y = progressbar.fill_y;
                fill_region.width = tmp_prev_num - width_offset;
                fill_region.x = progressbar.prog_start_x - fill_region.width;
                fill_region.height = 0;
                progressbar.prog_start_x = fill_region.x;
    
                fill_rect_dump(&fill_region, progressbar.unfill_id, DUMP_LEFT2RIGHT);
            }
            else
            {
                //do nothing for QAC
            }
        }
        
        //有游标，再刷新游标
        if ((progressbar.attrib & PROGRESS_STYLE_OPTION) == STYLE_OPTION_BOTH)
        {
            if((progressbar.attrib & PROGRESS_DIRECTION_OPTION) == PROGRESS_DIRECTION_HORIZONTAL)
            {
                //romf_show_pic(progressbar.tag_id, progressbar.prog_start_x + width_offset, progressbar.tag_y);
                trs_pic.pic_x = progressbar.prog_start_x + width_offset;
                trs_pic.pic_y = progressbar.tag_y;
                trs_pic.clear = 0;//刷透明游标
                show_pic_transparent(&trs_pic);
            }
            else
            {
                tmp_y = progressbar.prog_start_y - (tmp_entry.height - 1) - width_offset;
                //romf_show_pic(progressbar.tag_id, progressbar.tag_x, tmp_y);
                trs_pic.pic_x = progressbar.tag_x;
                trs_pic.pic_y = tmp_y;
                trs_pic.clear = 0;//刷透明游标
                show_pic_transparent(&trs_pic);
            }
        }
    }
    else
    {
        //do nothing for QAC
    }
    if ((mode == PROGRESS_DRAW_PROGRESS_RIGHT) || (mode == PROGRESS_DRAW_RIGHT_INIT))//右箭头，用prev_num[1]
    {
        prev_num[1] = width_offset;
    }
    else
    {
        prev_num[0] = width_offset;
    }
    return UI_NO_ERR;
}
/*! \endcond */
