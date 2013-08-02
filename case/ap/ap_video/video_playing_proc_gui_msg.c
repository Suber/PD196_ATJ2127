/*******************************************************************************
 *                              US212A
 *                            Module: VIDEO_PLAYING_PROC_GUI_MSG
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-12-8 9:03:17           1.0              build this file
 *******************************************************************************/
/*!
 * \file     video_playing_proc_gui_msg.c
 * \brief    video播放界面对按键事件值的响应模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 * 				 在暗屏状态下无按键事件，因此本模块不会被调用，减少切bank次数
 * \version 1.0
 * \date    2011-12-8
 *******************************************************************************/
#include "video.h"

/*! \brief 物理按键与事件消息映射表*/
const key_map_t playing_key_map_list[] =
{
    /*! 长按next快进 事件 */
    {{ KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VIDEO_PLAYING_FFORWORD },
    /*! 长按prev快退 事件 */
    {{ KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD},   EVENT_VIDEO_PLAYING_FBACKWORD },
    /*! 短按next切换下一曲or结束全屏 */
    {{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_NEXT },
    /*! 短按prev切换上一曲事or结束全屏  */
    {{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_PREV },
    /*! 短按option事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_OPTION },
    /*!短按play播放/暂停事件 */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_PAUSE },
    /*!短按vol返回事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_VIDEO_PLAYING_RETURN },
    /*!vol事件 */
    {{ KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_VIDEO_PLAYING_VOLUME },
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

static const uint8 video_volume_tbl[32]=
{
    40, 40, 39, 39, 38, 38, 37, 36, 35, 34,
    33, 32, 31, 30, 28, 27, 26, 25, 24, 23,
    22, 20, 19, 17, 16, 14, 12,10, 8, 6, 4, 
    0
};

/******************************************************************************/
/*!
 * \par  Description:
 *	  video 修改音量
 * \param[in]    is_key_next 音量加减
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
static void _video_change_volume(uint8 is_key_next)
{
    uint8 volum_set;
    uint8 tab_index;
    
    //音量调节
    if (is_key_next == TRUE)
    {
        if (g_comval.volume_current < g_comval.volume_limit)
        {
            g_comval.volume_current++;
            g_display_bitmap |= up_videovoladd;
        }
    }
    else
    {
        if (g_comval.volume_current > 0)
        {
            g_comval.volume_current--;
            g_display_bitmap |= up_videovolsub;
        }
    }
    //局部刷新，同时开启全屏计时器
    g_video_paint_flag = VIDEO_PAINT_PARTIAL;
    g_video_timer_flag |= up_video_fullscreen_timer;

    //转换音量
    tab_index = (31 - g_comval.volume_current);
    volum_set = (video_volume_tbl[tab_index]& 0x3f);

    //设置硬件音量
    set_pa_volume((uint32)volum_set);

    //保存当前音量值
    set_current_volume(g_comval.volume_current);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video nex键处理
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_next(void)
{
    if (g_video_cur_window == VIDEO_FULLSCREEN)
    {
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_timer_flag |= up_video_fullscreen_timer;
    }
    else if (g_video_cur_window == VIDEO_VOLUMEBAR)
    {
        _video_change_volume(TRUE);
    }
    else
    {
        if (g_video_play_status == VIDEO_FF)
        {
            //快进速率重设为0
            g_video_speed_control = 0;
            g_video_timer_counter = 0;
            //更新播放标识
            g_video_play_status = g_video_play_status_backup;
            g_display_bitmap |= up_videoplaystatus | up_videospeed;
        }
        else
        {
            //切下一曲
            g_video_change_file = up_video_playnext;
        }
    }
    return RESULT_NULL;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video prev键处理
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_prev(void)
{
    if (g_video_cur_window == VIDEO_FULLSCREEN)
    {
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_timer_flag |= up_video_fullscreen_timer;
    }
    else if (g_video_cur_window == VIDEO_VOLUMEBAR)
    {
        _video_change_volume(FALSE);
    }
    else
    {
        if (g_video_play_status == VIDEO_FB)
        {
            //快退速率为0
            g_video_speed_control = 0;
            g_video_timer_counter = 0;
            g_video_play_status = g_video_play_status_backup;
            g_display_bitmap |= up_videoplaystatus | up_videospeed;
            if(g_fast_back_end == TRUE)
            {
                g_video_change_file = up_video_playprev;
                g_fast_back_end = FALSE;
            }
        }
        else
        {
            //切上一曲
            g_video_change_file = up_video_playprev;
        }
    }
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video play键处理
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_play(void)
{
    if (g_video_cur_window != VIDEO_PROGRESSBAR)
    {
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_timer_flag |= up_video_fullscreen_timer;
    }
    else
    {
        g_video_paint_flag = VIDEO_PAINT_PARTIAL;
        g_display_bitmap |= up_videoplaystatus;
        if (g_video_play_status == VIDEO_PAUSE)
        {
            g_video_play_status = VIDEO_PLAY;
        }
        else
        {
            //非全屏播放状态转入暂停状态
            g_video_play_status = VIDEO_PAUSE;
            //暂停状态下PROGRESSBAR模式需停止全屏计时器
            g_video_timer_flag |= clr_video_fullscreen_timer;
        }
    }
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video 等待按键抬起处理
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
static bool _video_wait_key_up(void)
{
    input_gui_msg_t ui_msg;

    while (get_gui_msg(&ui_msg) == TRUE)
    {
        if ((ui_msg.data.kmsg.type & KEY_TYPE_SHORT_UP) == 0)
        {
            continue;
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video 快进退处理
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_fastplay(uint8 is_key_next)
{

    if (g_video_cur_window == VIDEO_VOLUMEBAR)
    {
        _video_change_volume(is_key_next);
    }
    else
    {
        if ((g_video_play_status != VIDEO_FF) && (g_video_play_status != VIDEO_FB))
        {
            g_video_play_status_backup = g_video_play_status;
            if (is_key_next == TRUE)
            {
                g_video_play_status = VIDEO_FF;
            }
            else
            {
                g_video_play_status = VIDEO_FB;
            }
            //设置初始快进快退速率
            g_video_speed_control = 1;
            g_video_cur_window = VIDEO_PROGRESSBAR;
            g_display_bitmap |= up_videoplaystatus | up_videotimeinfor | up_videospeed;
            g_video_timer_flag |= clr_video_fullscreen_timer;
        }

        //视频快进退时为防止快进退速度过慢导致消息队列满，无法
        //接收到KEY_UP消息，应用主动清除多余的KEY_HOLD消息以保证
        //可以截获KEY_UP消息并结束快进退
        if (_video_wait_key_up() == TRUE)
        {
            g_video_speed_control = 0;
            g_video_timer_counter = 0;
            g_video_play_status = g_video_play_status_backup;
            g_display_bitmap |= up_videoplaystatus | up_videospeed;
        }

    }
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video 音量键处理
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_play_key_vol(void)
{
    //需要重新设窗
    g_video_cur_window = VIDEO_VOLUMEBAR;
    g_display_bitmap |= up_videovolume | up_videoBackGDPic;
    g_video_paint_flag = VIDEO_PAINT_PARTIAL;

    g_video_timer_flag |= up_video_fullscreen_timer;

    //过滤掉后续的VOL按键消息
    com_filter_key_hold();
    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  处理video GUI消息
 * \param[in]   cur_event:按键事件值
 * \param[out]  none
 * \return      the result：按键事件处理返回值
 * \ingroup    VIDEO
 * \note
 *******************************************************************************/
app_result_e video_playing_proc_gui_msg(input_gui_msg_t *ui_msg)
{
    //播放状态屏幕有三种情形，是否需要转换
    app_result_e ui_result = RESULT_NULL;
    msg_apps_type_e cur_event;

    if (com_key_mapping(ui_msg, &cur_event, playing_key_map_list) == TRUE)
    {
        switch (cur_event)
        {
            case EVENT_VIDEO_PLAYING_NEXT:
            ui_result = _video_play_key_next();
            break;

            case EVENT_VIDEO_PLAYING_PREV:
            ui_result = _video_play_key_prev();
            break;

            case EVENT_VIDEO_PLAYING_PAUSE:
            ui_result = _video_play_key_play();
            break;

            case EVENT_VIDEO_PLAYING_FBACKWORD:
            ui_result = _video_play_key_fastplay(FALSE);
            break;

            case EVENT_VIDEO_PLAYING_FFORWORD:
            ui_result = _video_play_key_fastplay(TRUE);
            break;

            case EVENT_VIDEO_PLAYING_VOLUME:
            ui_result = _video_play_key_vol();
            break;

            case EVENT_VIDEO_PLAYING_RETURN:
            ui_result = RESULT_VIDEO_RETURN;
            break;

            case EVENT_VIDEO_PLAYING_OPTION:
            ui_result = RESULT_VIDEO_OPTIONMENU;
            break;

            case EVENT_ENTER_KEYLOCK:
            //锁键消息处理
            change_keylock_state(TRUE);
            break;
            case EVENT_ENTER_KEYLOCK_UN:
            //锁键消息处理
            change_keylock_state(FALSE);
            break;
            case EVENT_ENTER_KEYLOCK_HOLD:
            if (g_video_cur_window != VIDEO_PROGRESSBAR)
            {
                g_video_cur_window = VIDEO_PROGRESSBAR;
            }
            g_display_bitmap |= up_videoheaderbar;
            g_video_paint_flag = VIDEO_PAINT_ALL;
            break;

            default:
            //视频如果响应热键消息，是否应该先暂停当前解码??
            ui_result = com_message_box(cur_event);
            if (ui_result == RESULT_REDRAW)
            {
                video_set_full_flag()
                g_video_cur_window = VIDEO_PROGRESSBAR;
                g_video_prev_window = VIDEO_FULLSCREEN;
                //重绘界面，强制设置为播放状态
                g_video_play_status = VIDEO_PLAY;
                ui_clear_screen(NULL);

            }
            //恢复刷屏模式
            lcd_set_draw_mode(g_video_draw_mode);
            break;
        }

        if ((g_video_cur_window != VIDEO_FULLSCREEN) && (g_video_play_status == VIDEO_PLAY))
        {
            g_video_timer_flag |= up_video_fullscreen_timer;
        }
    }


    return ui_result;
}



