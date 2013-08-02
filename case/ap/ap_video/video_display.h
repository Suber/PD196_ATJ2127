/*******************************************************************************
 *                              US212A
 *                            Module: VIDEO_DISPLAY
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-5-8 9:07:35           1.0              build this file
 *******************************************************************************/
/*!
 * \file     video_display.h
 * \brief    视频播放界面UI显示
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2012-5-8
 *******************************************************************************/
#ifndef _VIDEO_DISPLAY_H
#define _VIDEO_DISPLAY_H
#include "video.h"
//==============================================
#define FAST_DISPLAY
//#define VIDEO_QVGA

#define PIC_BUFFER_BASE  0x3c000

#define MAX_VOLUME  31

#define VIDEO_COLOR_BG    0x9CA

#define VIDEO_COLOR_WHITE 0xffff
#define VIDEO_COLOR_BLUE  0x549d

#define MAX_NUMBER          11
#define MAX_HEADBAR_ICON    3
#define MAX_BATTERY         6

//==============================================

//底部状态条坐标
#define P_VIDEO_BOTTOM_X 0
#define P_VIDEO_BOTTOM_Y (DISPLAY_LENGTH - 31)
#define P_VIDEO_BOTTOM_W 160
#define P_VIDEO_BOTTOM_H 31

//进度条
//进度条背景图
#define P_VIDEO_PROG_BG_X 10
#define P_VIDEO_PROG_BG_Y (P_VIDEO_BOTTOM_Y + 11)
#ifdef VIDEO_QVGA
#define P_VIDEO_PROG_LENGTH 282
#else
#define P_VIDEO_PROG_LENGTH 140
#endif
//进度条tag坐标
#define P_VIDEO_PROG_TAG_Y (P_VIDEO_PROG_BG_Y + 1)
#define P_VIDEO_PROG_TAG_H 3
#define P_VIDEO_PROG_TAG_BASE_X 11

//时间显示
#define P_VIDEO_CUR_TIME_X 17
#define P_VIDEO_CUR_TIME_Y (P_VIDEO_BOTTOM_Y + 20)
#define P_VIDEO_TOTAL_TIME_X (DISPLAY_HEIGHT - 48)
#define P_VIDEO_TOTAL_TIME_Y (P_VIDEO_BOTTOM_Y + 20)

//音量条显示
//音量条背景坐标
#define P_VIDEO_VOL_BG_X 0
#define P_VIDEO_VOL_BG_Y (P_VIDEO_BOTTOM_Y + 13)
#define P_VIDEO_VOL_LENGTH 124
//音量条tag坐标
#define P_VIDEO_VOL_TAG_Y        (P_VIDEO_BOTTOM_Y + 8)
#define P_VIDEO_VOL_TAG_H        6
#define P_VIDEO_VOL_TAG_BASE_X   17
#define P_VIDEO_VOL_TAG_W        1
#define P_VIDEO_VOL_TAG_STEP     4
//音量条forbidden图片坐标
#define P_VIDEO_FORBIDDEN_X      (DISPLAY_HEIGHT - 11)
#define P_VIDEO_FORBIDDEN_Y      (P_VIDEO_BOTTOM_Y + 15)

//headbar显示
//ap icon坐标
#define P_VIDEO_ICON_X 0
#define P_VIDEO_ICON_Y 0
//battery坐标
#define P_VIDEO_BATTERY_X (DISPLAY_HEIGHT - 20)
#define P_VIDEO_BATTERY_Y 0

//文件名显示
#define P_VIDEO_FILENAME_BASE_X 20
#define P_VIDEO_FILENAME_LENGTH (DISPLAY_HEIGHT - 40)
#define P_VIDEO_FILENAME_H      16

typedef struct
{
    uint32 font_addr;
    uint16 text_width;
    uint16 word_width;
    uint16 word_height;    
} name_cache_t;

/*! \brief video缓存图片宽高信息*/
extern res_size_t video_num_res;

extern res_size_t video_headbar_res;

extern res_size_t video_battery_res;

extern res_size_t video_progress_res;

extern res_size_t video_vol_tag_res;

extern res_size_t video_vol_forbidden_res;

extern res_size_t video_vol_bg_res;


/*! \brief video缓存图片地址信息*/
extern uint32 video_num_addr;

extern uint32 video_headbar_addr;

extern uint32 video_battery_addr;

extern uint32 video_progress_addr;

extern uint32 video_vol_tag_addr;

extern uint32 video_vol_forbidden_addr;

extern uint32 video_vol_bg_addr;


/*! \brief video 缓存数据信息*/

extern uint32 video_prev_time;

extern uint16 video_prev_progress_x;

extern uint8 video_prev_vol;

extern uint16 video_prev_vol_x;

extern uint8 g_battery_counter;

extern name_cache_t video_cache_name;

//extern void video_refresh_timeinfor(uint8 mode);
extern void read_num_pic_to_uram(void);

extern void video_fast_show_time(uint8 *time_buffer, uint16 start_x, uint16 start_y);

extern void video_fast_clr_screen(region_t region, uint16 color);

extern void video_fast_show_progress(uint32 cur_time, uint32 total_time, uint32 mode);

extern void video_fast_show_volume(uint8 cur_volume, uint8 mode);

extern void video_fast_show_headbar(uint8 mode);

extern void read_filename_to_buffer(const uint8 *str, uint8 str_len) __FAR__;

extern void video_fast_show_filename(void);

#endif

