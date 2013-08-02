/*******************************************************************************
 *                              US212A
 *                            Module: video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     video
 * \brief    video显示play播放界面函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/22
 *******************************************************************************/
#include  "video.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    将以毫秒为单位的时间转换为time_t结构的时间
 * \param[in]    time_sec
 * \param[out]   time_buf
 * \return       void
 * \ingroup      video_play
 * \note
 *******************************************************************************/
static void _mktime(uint32 time_msec, uint8 *time_buffer)
{
    uint32 time_sec;
    uint8 time_hour, time_minute, time_second;
    time_sec = (time_msec / 1000);
    time_hour = (uint8)(time_sec / 3600);
    time_minute = (uint8)((time_sec % 3600) / 60);
    time_second = (uint8)(time_sec % 60);

    time_buffer[0] = time_hour / 10;
    time_buffer[1] = time_hour % 10;
    time_buffer[3] = time_minute / 10;
    time_buffer[4] = time_minute % 10;
    time_buffer[6] = time_second / 10;
    time_buffer[7] = time_second % 10;

    time_buffer[2] = time_buffer[5] = 10;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    play界面显示刷新函数
 * \param[in]    mode TRUE：初次刷新，需要重新初始化进度条 FALSE:循环刷新
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_refresh_screen(uint8 mode)
{
    uint8 draw_mode;
    uint8 time_buffer[8];
    uint16 up_color, down_color;

    region_t region;
    style_infor_t video_style;
    picbox_private_t video_picbox_param;

    video_style.type = UI_AP;

    lcd_controller_init(RGB_MODE);
    lcd_set_draw_mode(DRAW_MODE_V_DEF);

    if (up_videoBackGDPic == (g_display_bitmap & up_videoBackGDPic))
    {
        if (g_video_cur_window == VIDEO_PROGRESSBAR)
        {
            up_color = VIDEO_COLOR_BG;
            down_color = VIDEO_COLOR_BG;
        }
        else if (g_video_cur_window == VIDEO_VOLUMEBAR)
        {
            up_color = 0;
            down_color = VIDEO_COLOR_BG;
        }
        else
        {
            up_color = 0;
            down_color = 0;
        }

        //清除上面部分的显示
        region.x = region.y = 0;
        region.width = g_region_progressbar.w;
        region.height = g_region_adjust.avi_prog_y;
        video_fast_clr_screen(region, up_color);

        //底层状态条
        region.y = g_region_adjust.avi_prog_y + g_region_progressbar.h;
        region.width = g_region_progressbar.w;
        region.height = g_region_fullscreen.h - region.y;
        video_fast_clr_screen(region, down_color);
    }
    if (up_videofileinfor == (g_display_bitmap & up_videofileinfor))
    {
        video_fast_show_filename();
    }
    if (up_videoheaderbar == (g_display_bitmap & up_videoheaderbar))
    {
        video_fast_show_headbar(mode);
    }
    if (up_videoplaystatus == (g_display_bitmap & up_videoplaystatus))
    {
        //播放状态图标
        video_style.style_id = STY_VIDEO_PLAYICON_PICBOX;
        video_picbox_param.frame_id = g_video_play_status;
        video_picbox_param.pic_id = -1;
        ui_show_picbox(&video_style, &video_picbox_param);
    }
    if (up_videoprogress == (g_display_bitmap & up_videoprogress))
    {
        video_fast_show_progress(g_video_var.break_point.time / 1000, g_total_time / 1000, mode);
    }
    if (up_videotimeinfor == (g_display_bitmap & up_videotimeinfor))
    {
        _mktime(g_video_var.break_point.time, time_buffer);
        video_fast_show_time(time_buffer, P_VIDEO_CUR_TIME_X, P_VIDEO_CUR_TIME_Y);

        if (mode == TRUE)
        {
            _mktime(g_total_time, time_buffer);
            video_fast_show_time(time_buffer, P_VIDEO_TOTAL_TIME_X, P_VIDEO_TOTAL_TIME_Y);
        }
    }

    if (up_videospeed == (g_display_bitmap & up_videospeed))
    {
        video_style.style_id = STY_VIDEO_FASTFLAG_PICBOX;
        video_picbox_param.frame_id = g_video_speed_control;
        video_picbox_param.pic_id = -1;
        ui_show_picbox(&video_style, &video_picbox_param);
    }

    if (g_video_cur_window == VIDEO_VOLUMEBAR)
    {
        if (up_videovolume == (g_display_bitmap & up_videovolume))
        {
            draw_mode = TRUE;
        }
        else
        {
            draw_mode = FALSE;
        }
        // 显示音量进度条
        video_fast_show_volume(g_comval.volume_current, draw_mode);
    }
    
    g_display_bitmap = 0;
    lcd_set_draw_mode(g_video_draw_mode);
}
