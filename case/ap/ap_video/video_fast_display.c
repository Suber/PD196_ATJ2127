/*******************************************************************************
 *                              US212A
 *                            Module: VIDEO_FAST_DISPLAY
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-4-24 9:59:34           1.0              build this file
 *******************************************************************************/
/*!
 * \file     video_fast_display.c
 * \brief    video视频播放界面快速刷新模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2012-4-24
 *******************************************************************************/

#include "video.h"


//低电电压 3.0V
#define BATLOW_VEL      0x51

//电池电量等级对应 ADC 参考值
static const uint8 bat_grade[6] =
{
    0, //填充
    BATLOW_VEL, //低电        返回 0
    0x59, //<= 3.5V     空格电
    0x5e, //3.5-3.6V    一格电
    0x60, //3.6-3.65V   两格电
    0x68,       //3.65-3.85V   三格电；3.85V以上   满格电
};

/*! \brief video缓存图片宽高信息*/

res_size_t video_num_res;

res_size_t video_headbar_res;

res_size_t video_battery_res;

res_size_t video_progress_res;

res_size_t video_vol_tag_res;

res_size_t video_vol_forbidden_res;

res_size_t video_vol_bg_res;

/*! \brief video缓存图片地址信息*/

uint32 video_num_addr;

uint32 video_headbar_addr;

uint32 video_battery_addr;

uint32 video_progress_addr;

uint32 video_vol_tag_addr;

uint32 video_vol_forbidden_addr;

uint32 video_vol_bg_addr;

name_cache_t video_cache_name;

/*! \brief video 缓存数据信息*/
uint32 video_prev_time;

uint16 video_prev_progress_x;

uint8 video_prev_vol;

uint16 video_prev_vol_x;

uint8 g_battery_counter;

/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 将URAM缓存的数据传送至DMA输出
 * \param[in]    width 刷屏宽度
 * \param[in]    size  数据长度
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
static void _trans_data_to_dma(uint32 width, uint32 size)
{
    lcd_dma_set_counter(size / 2, width);

    lcd_dma_start_trans(JPEG_RAM_INDEX);
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 输出URAM缓存的图片数据
 * \param[in]    pic_cache_buffer 数据缓存起始地址
 * \param[in]    region  图片数据坐标
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
static void _show_pic(uint32 pic_cache_buffer, region_t region)
{
    uint32 real_size;
#ifndef PC
    uint32 start_addr = JPEG_BUF_ADDR;
#else
    uint32 start_addr = GET_REAL_ADDR(JPEG_BUF_ADDR);
#endif
    uint32 pic_size = region.width * region.height * 2;
    uint32 first_size = pic_size;

    lcd_set_window(&region);

    if (pic_size > JPEG_BUF_LEN)
    {
        first_size = JPEG_BUF_LEN;
    }

    while (pic_size > 0)
    {
        if (pic_size > first_size)
        {
            real_size = first_size;
        }
        else
        {
            real_size = pic_size;
        }
        libc_memcpy(start_addr, pic_cache_buffer, real_size);
        _trans_data_to_dma(region.width, real_size);
        pic_size -= real_size;
        pic_cache_buffer += real_size;
    }
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 快速显示当前时间/总时间
 * \param[in]    time_buffer 时间图片缓存地址
 * \param[in]    start_x  图片数据起始x坐标
 * \param[in]    start_y  图片数据起始y坐标
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_fast_show_time(uint8 *time_buffer, uint16 start_x, uint16 start_y)
{
    uint8 i, j;
    uint32 width_size;
    uint32 pic_size;
    uint32 line_addr;
    region_t region;

#ifndef PC
    uint32 start_addr = JPEG_BUF_ADDR;
#else
    uint32 start_addr = GET_REAL_ADDR(JPEG_BUF_ADDR);
#endif

    width_size = video_num_res.width * 2;
    pic_size = width_size * video_num_res.height;
    for (i = 0; i < video_num_res.height; i++)
    {
        for (j = 0; j < 8; j++)
        {
            //总共8个图标每一行的地址
            line_addr = video_num_addr + time_buffer[j] * pic_size + width_size * i;
            libc_memcpy((uint8 *) start_addr, (uint8 *) (line_addr), width_size);
            start_addr += width_size;
        }
    }

    region.x = start_x;
    region.y = start_y;
    region.width = video_num_res.width * 8;
    region.height = video_num_res.height;
    lcd_set_window(&region);
    _trans_data_to_dma(region.width, pic_size * 8);
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 快速刷屏函数
 * \param[in]    region   刷屏坐标
 * \param[in]    color    刷屏颜色
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_fast_clr_screen(region_t region, uint16 color)
{
    uint32 i;
    uint32 first_size;
    uint32 region_size = region.width * region.height * 2;
    uint32 real_size;
#ifndef PC
    uint32 start_addr = JPEG_BUF_ADDR;
#else
    uint32 start_addr = GET_REAL_ADDR(JPEG_BUF_ADDR);
#endif

    lcd_set_window(&region);
    first_size = region_size;
    if (region_size > JPEG_BUF_LEN)
    {
        first_size = JPEG_BUF_LEN;
    }

    for (i = 0; i < (first_size / 2); i++)
    {
        *((uint16 *) start_addr + i) = color;
    }

    while (region_size > 0)
    {
        if (region_size >= first_size)
        {
            real_size = first_size;
        }
        else
        {
            real_size = region_size;
        }
        _trans_data_to_dma(region.width, real_size);
        region_size -= real_size;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 快速显示进度条函数
 * \param[in]    cur_time   当前时间
 * \param[in]    total_time 总时间
 * \param[in]    mode       刷屏模式 是否刷背景图
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_fast_show_progress(uint32 cur_time, uint32 total_time, uint32 mode)
{
    uint8 prog_diff;
    region_t region;
    if (mode == TRUE)
    {
        region.x = P_VIDEO_PROG_BG_X;
        region.y = P_VIDEO_PROG_BG_Y;
        region.width = video_progress_res.width;
        region.height = video_progress_res.height;
        _show_pic(video_progress_addr, region);
        video_prev_time = 0;
        video_prev_progress_x = P_VIDEO_PROG_TAG_BASE_X;
    }

    region.height = P_VIDEO_PROG_TAG_H;
    region.y = P_VIDEO_PROG_TAG_Y;

    if (total_time == 0)
    {
        return;
    }

    if (cur_time > video_prev_time)
    {
        prog_diff = (uint8)((cur_time * P_VIDEO_PROG_LENGTH / total_time) - (video_prev_time * P_VIDEO_PROG_LENGTH
                / total_time));
    }
    else
    {
        prog_diff = (uint8)((video_prev_time * P_VIDEO_PROG_LENGTH / total_time) - (cur_time * P_VIDEO_PROG_LENGTH
                / total_time));
    }

    //刷新的格数大于1才进行刷新
    if (prog_diff > 1)
    {
        region.width = prog_diff;
        if (cur_time > video_prev_time)
        {
            region.x = video_prev_progress_x;
            lcd_set_window(&region);
            video_fast_clr_screen(region, VIDEO_COLOR_WHITE);
            video_prev_progress_x = region.x + region.width;
        }
        else
        {
            region.x = video_prev_progress_x - region.width;
            lcd_set_window(&region);
            video_fast_clr_screen(region, VIDEO_COLOR_BG);
            video_prev_progress_x = region.x;
        }
        video_prev_time = cur_time;
    }
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 快速显示音量条函数
 * \param[in]    cur_volume 当前音量
 * \param[in]    mode       刷屏模式 是否刷背景图
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_fast_show_volume(uint8 cur_volume, uint8 mode)
{
    uint8 i;
    region_t region;
    uint16 cur_vol_x = 0; //avoid warning
    uint32 pic_size = video_vol_tag_res.width * video_vol_tag_res.height * 2;

    if (mode == TRUE)
    {
        //显示背景大图
        region.x = P_VIDEO_VOL_BG_X;
        region.y = P_VIDEO_VOL_BG_Y;
        region.width = video_vol_bg_res.width;
        region.height = video_vol_bg_res.height;
        _show_pic(video_vol_bg_addr, region);

        video_prev_vol = 0;
        video_prev_vol_x = P_VIDEO_VOL_TAG_BASE_X;
    }

    region.y = P_VIDEO_VOL_TAG_Y;
    region.width = video_vol_tag_res.width;
    region.height = video_vol_tag_res.height;

    if (mode == TRUE)
    {
        region.x = P_VIDEO_VOL_TAG_BASE_X;
        for (i = 0; i <= MAX_VOLUME; i++)
        {
            if (i < cur_volume)
            {
                //显示已有音量
                _show_pic(video_vol_tag_addr, region);
            }
            else if (i == cur_volume)
            {
                //记录此时坐标
                cur_vol_x = region.x;
            }
            else if (i == g_comval.volume_limit)
            {
                //显示音量限制
                _show_pic(video_vol_tag_addr + pic_size * 3, region);
            }
            else
            {
                //显示还剩余音量值
                _show_pic(video_vol_tag_addr + pic_size * 1, region);
            }
            region.x += P_VIDEO_VOL_TAG_STEP;
        }
    }
    else
    {
        region.x = video_prev_vol_x;
        if (video_prev_vol > cur_volume)
        {
            //值减小
            _show_pic(video_vol_tag_addr + pic_size * 1, region);
        }
        else
        {
            //值增大
            _show_pic(video_vol_tag_addr, region);
        }
        cur_vol_x = P_VIDEO_VOL_TAG_BASE_X + cur_volume * P_VIDEO_VOL_TAG_STEP;
    }

    //显示当前音量
    region.x = cur_vol_x;
    _show_pic(video_vol_tag_addr + pic_size * 2, region);

    if ((video_prev_vol == g_comval.volume_limit) || (cur_volume == g_comval.volume_limit))
    {
        region.x = P_VIDEO_FORBIDDEN_X;
        region.y = P_VIDEO_FORBIDDEN_Y;
        region.width = video_vol_forbidden_res.width;
        region.height = video_vol_forbidden_res.height;

        if (cur_volume == g_comval.volume_limit)
        {
            _show_pic(video_vol_forbidden_addr, region);
        }
        else
        {
            if(video_prev_vol == g_comval.volume_limit)
            {
                video_fast_clr_screen(region, VIDEO_COLOR_BG);
            }
        }
    }

    //保存当前的video值和坐标
    video_prev_vol = cur_volume;
    video_prev_vol_x = cur_vol_x;
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 快速显示电池图标
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
static void _show_battery(uint8 display)
{
    uint32 pic_size;
    uint32 battery_addr;
    region_t region;

    if (display >= MAX_BATTERY)
    {
        //无效值
        return;
    }
    region.x = P_VIDEO_BATTERY_X;
    region.y = P_VIDEO_BATTERY_Y;
    region.width = video_battery_res.width;
    region.height = video_battery_res.height;
    pic_size = region.width * region.height * 2;
    battery_addr = video_battery_addr + display * pic_size;
    _show_pic(battery_addr, region);
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 快速显示ap icon
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
static void _show_icon(void)
{
    uint8 i;
    region_t region;
    uint32 pic_size;
    uint32 icon_addr;
    card_state_e card_status;

    region.x = P_VIDEO_ICON_X;
    region.y = P_VIDEO_ICON_Y;
    region.width = video_headbar_res.width;
    region.height = video_headbar_res.height;
    pic_size = region.width * region.height * 2;

    if (get_keylock_state() == TRUE)
    {
        //检测到锁状态
        i = 2;
    }
    else
    {
        card_status = get_card_state();
        if (card_status == CARD_STATE_CARD_IN)
        {
            i = 0;
        }
        else if (card_status == CARD_STATE_CARD_PLAY)
        {
            i = 1;
        }
        else
        {
            i = 3;
        }

    }

    if (i < 3)
    {
        icon_addr = video_headbar_addr + i * pic_size;
        _show_pic(icon_addr, region);
    }
    else
    {
        //清屏
        video_fast_clr_screen(region, VIDEO_COLOR_BG);
    }

}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取电池电量，转换为电量等级。
 * \param[in]    none
 * \param[out]   none
 * \return       uint8 返回电池电量等级，1 ~ BATTERY_GRADE_MAX 为正常，0表示低电。
 * \ingroup      misc_func
 * \note
 * \li
*******************************************************************************/
static uint8 get_battery_grade(void)
{
    uint8 battery_value;
    uint8 i;

    battery_value = act_readb(BATADC_DATA);//0-6bit有效
    for (i = BATTERY_GRADE_MAX; i > 0; i--)
    {
        if (battery_value >= bat_grade[i])
        {
            return i;
        }
    }

    return 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 快速显示headbar
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_fast_show_headbar(uint8 mode)
{
    if (mode == TRUE)
    {
        _show_icon();
        change_charge_state((uint8)(int)key_chargeget(0));
        change_bat_value(get_battery_grade());
        g_battery_counter = 0;
    }

    //正在充电
    if (get_charge_state() == CHARGE_CHARGING)
    {
        if (g_battery_counter > BATTERY_GRADE_MAX)
        {
            //充电时电池值显示范围为：空格电 ~ 满格电
            g_battery_counter = 0;
        }

        _show_battery(g_battery_counter);
    }
    else
    {
        //无充电，电池供电
        if (get_charge_state() == CHARGE_NONE)
        {
            _show_battery(get_bat_value());
        }
        //电池已满
        else if (get_charge_state() == CHARGE_FULL)
        {
            _show_battery(BATTERY_GRADE_MAX);
        }
        //无检测到电池
        else
        {
            _show_battery(0);
        }
    }
    g_battery_counter++;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    Video 显示一行文件名
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
static void _deal_one_line(uint8 *font_data, uint8 font_width, uint16 *display_buffer)
{
    uint8 i, j;
    uint8 src_data;
    uint16 color;
    uint8 data_cnt;
    uint8 *data_ptr = font_data;

    data_cnt = (font_width + 7) / 8;

    for (i = 0; i < data_cnt; i++)
    {
        for (j = 0; j < 8; j++)
        {
            src_data = (uint8)(*data_ptr & ((uint8)(0x80 >> j)));

            color = VIDEO_COLOR_WHITE;
            //空白点处理
            if (src_data == 0)
            {
                //跳过无需转换
                color = VIDEO_COLOR_BG;
            }
            *display_buffer = color;
            display_buffer++;
        }
        data_ptr++;
    }

}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 快速显示视频文件名
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_fast_show_filename(void)
{
    uint8 i;
    uint16 k;
    //显示区域坐标
    region_t region;
    //辅助缓存坐标
    region_t buf_region;

    uint16 *display_buffer;

    uint8 *font_data;
    uint16 str_width;

    uint16 word_width = video_cache_name.word_width;
    uint16 byte_count;
    uint8 *start_addr = (uint8 *) video_cache_name.font_addr;
#ifndef PC
    uint16 *dest_addr = (uint16 *) JPEG_BUF_ADDR;
#else
    uint16 *dest_addr = GET_REAL_ADDR(JPEG_BUF_ADDR);
#endif

    buf_region.height = video_cache_name.word_height;

    if (buf_region.height == 0)
    {
        return;
    }

    //最大缓存的字符数
    buf_region.width = ((JPEG_BUF_LEN / 2) / buf_region.height);

    //当前字符数小于最大缓存字符数，则使用当前字符数
    if (buf_region.width > word_width)
    {
        buf_region.width = word_width;
    }

    buf_region.x = 0;
    buf_region.y = 0;

    region.x = P_VIDEO_FILENAME_BASE_X;
    region.y = 0;
    region.width = buf_region.width;
    region.height = buf_region.height;

    byte_count = video_cache_name.text_width / 8;

    str_width = word_width;

    while (str_width > 0)
    {
        for (i = 0, k = buf_region.y; i < buf_region.height; i++, k++)//loop for height
        {
            font_data = start_addr + i * byte_count + (buf_region.x / 8);
            display_buffer = dest_addr + (k * buf_region.width + buf_region.x);
            _deal_one_line(font_data, (uint8) video_cache_name.text_width, display_buffer);
        }

        lcd_set_window(&region);
        _trans_data_to_dma(buf_region.width, buf_region.height * buf_region.width * 2);
        region.x += region.width;
        str_width -= region.width;
    }

    return;
}
