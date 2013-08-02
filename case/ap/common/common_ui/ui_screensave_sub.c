/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-screensave
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 17:24     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _ui_screensave_sub.c
 * \brief    commonUI 屏幕保护控制流实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了一种屏幕保护GUI，包括数字时钟，音乐专辑图片，关背光灯，演示模式等。
 * \par      EXTERNALIZED FUNCTIONS:
 *               直接调用ui 驱动中的picbox等完成控件显示流任务。
 * \version  1.0
 * \date     2011-9-19
*******************************************************************************/

#include "common_ui.h"

const uint16 album_art_x[ALBUM_ART_MAX_COUNT] = 
{0, 36, 0, 36, 0, 36};
const uint16 album_art_y[ALBUM_ART_MAX_COUNT] = 
{0, 34, 68, 0, 34, 68};

/******************************************************************************/
/*!                    
 * \par  Description:
 *    绘制数字时钟
 * \param[in]    flag 数字时钟更新模式，true表示全部更新，false表示更新部分
 * \param[out]   none    
 * \return       none
 * \note 
*******************************************************************************/
void draw_digit_clock(bool flag)
{
    date_t *p_sys_date;
    time_t tmp_sys_time;
    time_timebox_t *tmp_time;
    picbox_private_t ampm_pic;
    timebox_private_t ss_time;
    style_infor_t style_infor;
    timebox_draw_mode_e mode;
    uint8 tmp_format;
    
    if(flag == TRUE)
    {
        mode = TIMEBOX_DRAW_ALL;
    }
    else
    {
        mode = TIMEBOX_DRAW_TIME;
    }
    
    //获取系统日期并更新显示
    p_sys_date = &ss_time.times[0].union_time.date;
    sys_get_date(p_sys_date);
    
    //选取显示格式
    if(sys_comval->date_format == 1)
    {
        tmp_format = DATE_DISP_MMDDYY;
    }
    else
    {
        tmp_format = DATE_DISP_YYMMDD;
    }
    
    ss_time.times[0].time_type = 'D';
    ss_time.times[0].mode = tmp_format;
    
    //只在初始和有更新时才显示
    if((flag == TRUE) || (p_sys_date->day != g_ss_last_date))
    {
        style_infor.style_id = DIGIT_CLOCK_DATE;
        style_infor.type = UI_COM;
        ui_show_timebox(&style_infor, &ss_time, (uint32)mode);
        g_ss_last_date = p_sys_date->day;
    }
    
    //获取系统时间并更新显示
    sys_get_time(&tmp_sys_time);
    tmp_time = &(ss_time.times[0].union_time.time);
    tmp_time->hour = tmp_sys_time.hour;
    tmp_time->minute = tmp_sys_time.minute;
    tmp_time->second = tmp_sys_time.second;
    
    ss_time.times[0].time_type = 'T';
    ss_time.times[0].mode = TIME_DISP_HHMM;
    
    //只在初始和有更新时才显示
    if((flag == TRUE) || (tmp_sys_time.minute != g_ss_last_time))
    {
        if(sys_comval->time_format == 1)
        {
            style_infor.style_id = DIGIT_CLOCK_TIME_24;
            style_infor.type = UI_COM;
            ui_show_timebox(&style_infor, &ss_time, (uint32)mode);
            g_ss_last_time = tmp_sys_time.minute;
        }
        else
        {
            style_infor.style_id = DIGIT_CLOCK_TIME_12;
            style_infor.type = UI_COM;
            ui_show_timebox(&style_infor, &ss_time, (uint32)mode);
            
            style_infor.style_id = DIGIT_CLOCK_TIME_AMPM;
            ampm_pic.pic_id = V_U16_INVALID;
            if(tmp_sys_time.hour < 12)
            {
                ampm_pic.frame_id = 0;//am
            }
            else
            {
                ampm_pic.frame_id = 1;//pm
            }
            //小时转换为12小时制
            tmp_sys_time.hour = tmp_sys_time.hour % 12;
            if (tmp_sys_time.hour == 0)
            {
                tmp_sys_time.hour = 12;
            }
            ui_show_picbox(&style_infor, &ampm_pic);
            g_ss_last_time = tmp_sys_time.minute;
        }
    }
}

static void _trans_data_to_dma(uint32 width, uint32 size)
{
    lcd_dma_set_counter(size / 2, width);
    lcd_dma_start_trans(LCD_RAM_INDEX);
}

static void show_album_art(uint8 index)
{
    uint32 pic_size = 92 * 92 * 2;
    uint32 real_size;
    uint32 album_art_addr = VM_ALBUMART_INFO;
    uint8 *album_art_buf = (uint8 *)LCD_BUF_ADDR;
    region_t region;
    
    lcd_set_draw_mode(DRAW_MODE_H_DEF);
    
    region.x = album_art_x[index];
    region.y = album_art_y[index];
    region.width = 92;
    region.height = 92;
    lcd_set_window(&region);
    
    while(pic_size > 0)
    {
        if(pic_size > 92 * 4)
        {
            real_size = 92 * 4;
        }
        else
        {
            real_size = pic_size;
        }
        sys_vm_read(album_art_buf, album_art_addr, 512);
        _trans_data_to_dma(92, real_size);
        pic_size -= real_size;
        album_art_addr += 512;
    }
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    数字时钟屏幕保护软定时器服务例程，用来周期更新数字时钟
 * \param[in]    none
 * \param[out]   none    
 * \return       none
 * \note 
*******************************************************************************/
void digit_clock_handle(void)
{
    //更新数字时钟
    draw_digit_clock(FALSE);
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    音乐专辑图片屏幕保护软定时器服务例程，用来周期切换音乐专辑
 * \param[in]    none
 * \param[out]   none    
 * \return       none
 * \note 
*******************************************************************************/
void album_art_handle(void)
{
    style_infor_t style_infor;
    
    //刷背景图
    style_infor.style_id = ALBUM_ART_BG;
    style_infor.type = UI_COM;
    ui_show_picbox(&style_infor, NULL);
    
    //调用music中绘制音乐专辑图片的接口
    show_album_art(g_ss_album_art_phase);
    g_ss_album_art_phase++;
    if(g_ss_album_art_phase >= ALBUM_ART_MAX_COUNT)
    {
        g_ss_album_art_phase = 0;
    }
}
