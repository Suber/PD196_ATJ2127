/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-volumebar
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-15 9:51     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_volumebar_sub.c
 * \brief    commonUI 音量设置控件控制流实现--UI显示部分
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种音量调节GUI。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的parambox完成控件显示流任务。
 * \version 1.0
 * \date  2012-3-8
*******************************************************************************/

#include "common_ui.h"

//显示音量条除了进度条外的其他小图标
void show_volume_extra(uint16 style_id, uint16 pic_id, uint16 offset)
{
    style_infor_t temp_style_infor;
    region_t pic_region;
    uint16 x, y;
    
    temp_style_infor.style_id = style_id;
    temp_style_infor.type = UI_COM;
    ui_get_picbox_attrb(&temp_style_infor, &pic_region, 0);
    
    x = pic_region.x;
    y = pic_region.y - offset;
    ui_show_pic(pic_id, x, y);
}

//显示音量调UI
void show_volumebar_ui(style_infor_t *style_infor, parambox_private_t *private_data, \
int16 value_last, uint8 mode, parambox_draw_mode_e draw_mode)
{
    parambox_one_t *cur_one = &(private_data->items[private_data->active]);
    uint16 total = (uint16)(cur_one->max - cur_one->min);
    uint16 current = (uint16)(cur_one->value - cur_one->min);
    uint16 last_v = (uint16)(value_last - cur_one->min);
    uint16 limit = (uint16)(sys_comval->volume_limit - cur_one->min);
    volumebar_param attrb_data;
    style_infor_t temp_style_infor;
    progressbar_private_t vol_progress;
    progress_draw_mode_e left_mode, right_mode;
    
    if(volume_enter_forbidden == 0xff)//初始化进入
    {
        left_mode = PROGRESS_DRAW_LEFT_INIT;
        right_mode = PROGRESS_DRAW_PROGRESS_RIGHT;
    }
    else
    {
        left_mode = PROGRESS_DRAW_PROGRESS_LEFT;
        right_mode = PROGRESS_DRAW_PROGRESS_RIGHT;
    }

    //获取listbox UI参数
    temp_style_infor.style_id = VOLUMEBAR_PARAM;
    temp_style_infor.type = UI_COM;
    ui_load_attributebox(&temp_style_infor, &attrb_data, sizeof(volumebar_param));

    //清除旧痕迹
    show_volume_extra(VOLUMEBAR_CURRENT_CLEAR, P_VOLUME_CURRENT_CLEAR, (last_v * attrb_data.slider_length) / total);

    //显示喇叭和音量值
    if((  (volume_enter_forbidden == 0xff) //初始化进入
        &&(  ((mode != SET_VOLUME_LIMIT) && (cur_one->value < sys_comval->volume_limit))
           ||((mode == SET_VOLUME_LIMIT) && (cur_one->value < cur_one->max))))
    || (  ((mode != SET_VOLUME_LIMIT)&&(volume_enter_forbidden == 1)&&(cur_one->value < sys_comval->volume_limit))
        ||((mode == SET_VOLUME_LIMIT)&&(volume_enter_forbidden == 1)&&(cur_one->value < cur_one->max))))
    {
        temp_style_infor.style_id = VOLUMEBAR_SILENT_CLEAR;
        temp_style_infor.type = UI_COM;
        ui_show_picbox(&temp_style_infor, NULL);

        temp_style_infor.style_id = VOLUMEBAR_SPEAKER;
        temp_style_infor.type = UI_COM;
        ui_show_picbox(&temp_style_infor, NULL);
        volume_enter_forbidden = 0;
    }
    
    //显示进度条
    ui_show_sliderbar(style_infor, private_data, (uint32)draw_mode);
    
    //音量调节UI细节绘制，绘制音量实线和虚线
    //显示音量限制线
    show_volume_extra(VOLUMEBAR_LIMIT, P_VOLUME_LIMIT_LINE, (limit * attrb_data.slider_length) / total);
    //显示当前值
    show_volume_extra(VOLUMEBAR_CURRENT, P_VOLUME_CURRENT_LINE, (current * attrb_data.slider_length) / total);
    
    //显示左右箭头
    temp_style_infor.type = UI_COM;
    vol_progress.value = current;
    vol_progress.total = total;
    temp_style_infor.style_id = VOLUMEBAR_LEFT_ARROW;
    ui_show_progressbar(&temp_style_infor, &vol_progress, left_mode);
    temp_style_infor.style_id = VOLUMEBAR_RIGHT_ARROW;
    ui_show_progressbar(&temp_style_infor, &vol_progress, right_mode);
    
    //显示音量禁止图标
    if((  (volume_enter_forbidden == 0xff) //初始化进入
       && (  ((mode != SET_VOLUME_LIMIT) && (cur_one->value >= sys_comval->volume_limit))
           ||((mode == SET_VOLUME_LIMIT) && (cur_one->value == cur_one->max))))
    || (  ((mode != SET_VOLUME_LIMIT)&&(volume_enter_forbidden == 0)&&(cur_one->value >= sys_comval->volume_limit))
        ||((mode == SET_VOLUME_LIMIT)&&(volume_enter_forbidden == 0)&&(cur_one->value == cur_one->max))))
    {
        temp_style_infor.style_id = VOLUMEBAR_SILENT;
        temp_style_infor.type = UI_COM;
        ui_show_picbox(&temp_style_infor, NULL);
        volume_enter_forbidden = 1;
    }
}
