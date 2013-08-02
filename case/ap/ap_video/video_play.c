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
 * \brief    video的play函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/22
 *******************************************************************************/
#include  "video.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    video 检测卡盘是否存在函数
 * \param[in]    void
 * \param[out]   void
 * \return       app_resule_e the result
 * \ingroup      video_play
 * \note
 * \li   如果当前使用是卡盘而卡盘不存在，返回RESULT_CARD_OUT_ERROR
 *******************************************************************************/
app_result_e video_check_card_out(void)
{
    app_result_e ui_result = RESULT_NULL;
    if (g_video_var.path.file_path.dirlocation.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            gui_dialog_msg(DIALOG_MSG_V, DIALOG_INFOR_WAIT, S_CARD_OUT);
            ui_result = RESULT_CARD_OUT_ERROR;
        }
    }
    return ui_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    video 播放场景解码状态检测函数
 * \param[in]    void
 * \param[out]   void
 * \return       app_resule_e the result
 * \ingroup      video_play
 * \note
 * \li   在主循环中不断调用该函数，实现视频解码，快进退功能
 * \li   通过传递不同的参数值(cur_step),实现视频快进退速度可调
 *******************************************************************************/
app_result_e video_check_play_status(void)
{
    app_result_e ret_val = RESULT_NULL;
    int32 dec_result;
    uint8 cur_step = g_video_speed_control * 4;
    //    cur_step = 1;
#ifdef PC
    return RESULT_NULL;
#endif
    if (g_fast_back_end == TRUE)
    {
        return RESULT_NULL;
    }

    /*处理各播放状态的解码控制*/
    switch (g_video_play_status)
    {
        case VIDEO_PLAY:
        /*解一个VA*/
        dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_PLAY, 0);
        break;

        case VIDEO_FF:
        /*快进4个VA*/
        dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_FAST_FORWARD, cur_step);
        break;

        case VIDEO_FB:
        /*快退8个VA*/
        dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_FAST_BACKWARD, cur_step);
        break;

        default:
        dec_result = MMM_CMD_OK;
        break;
    }

    if (dec_result != MMM_CMD_OK)
    {
        //出现错误首先检测卡是否存在
        ret_val = video_check_card_out();

        if (ret_val == RESULT_CARD_OUT_ERROR)
        {
            return ret_val;
        }

        switch (dec_result)
        {

            case MMM_CMD_FILEEND: //到文件尾
            g_video_change_file = up_video_playnext;
            break;

            case MMM_CMD_FILEHEAD: //到文件头
            g_fast_back_end = TRUE;
            //g_video_change_file = up_video_playprev;
            break;

            case MMM_CMD_ERR://解码出错
            //正常解码出现格式错误
            g_video_change_file |= up_video_changefile;
            gui_dialog_msg(DIALOG_MSG_V, DIALOG_INFOR_WAIT, S_FORMAT_ERROR);
            g_file_err++;
            break;

            default:
            break;
        }
    }

    //注意不要改变正常的刷新标志位
    if ((g_video_play_status == VIDEO_FF) || (g_video_play_status == VIDEO_FB))
    {
        if (video_prev_time != (g_video_var.break_point.time / 1000))
        {
            g_video_paint_flag = VIDEO_PAINT_PARTIAL;
            g_display_bitmap |= up_videotimeinfor | up_videoprogress;
        }
    }
    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  play界面文件名滚屏定时器执行函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 * \li  只设置标志位，在主循环执行刷屏动作
 *******************************************************************************/
void filename_flag_update(void)
{
    if ((g_video_cur_window == VIDEO_PROGRESSBAR) && (need_scroll_filename == TRUE))
    {
        g_display_bitmap |= up_videofileinfor;
        g_video_paint_flag = 1;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  play界面播放时间和时间进度条定时器执行函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_play
 * \note
 * \li  只设置标志位，在主循环执行刷屏动作
 *******************************************************************************/
void play_flag_update(void)
{
    g_display_bitmap |= (up_videoprogress | up_videotimeinfor | up_videoheaderbar);
    g_video_paint_flag = 1;
    if ((g_video_play_status == VIDEO_FB) || (g_video_play_status == VIDEO_FF))
    {
        g_video_timer_counter++;
        if (g_video_timer_counter == VIDEO_FFB_LEVEL_1)
        {
            g_video_speed_control = 2;
            g_display_bitmap |= up_videospeed;
        }
        else if (g_video_timer_counter == VIDEO_FFB_LEVEL_2)
        {
            g_video_speed_control = 3;
            g_display_bitmap |= up_videospeed;
        }
        else
        {
        }
    }
    g_battery_display++;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    全屏定时器回调函数
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 * \li  5s计时时间到，从时间进度条或音量条显示状态转为全屏视频播放
 *      需要停止非全屏状态刷新计时器
 *******************************************************************************/
void video_full_display(void)
{
    //停止界面刷新计时器
    g_video_timer_flag |= clr_video_fullscreen_timer;

    if (g_video_play_status == VIDEO_PAUSE)
    {
        if (g_video_cur_window == VIDEO_VOLUMEBAR)
        {
            g_video_cur_window = VIDEO_PROGRESSBAR;
            g_video_paint_flag = VIDEO_PAINT_ALL;
            //开启滚屏和进度条定时器
            g_video_timer_flag |= up_video_filename_timer;
            g_video_timer_flag |= up_video_progress_timer;
        }
    }
    else
    {
        g_video_cur_window = VIDEO_FULLSCREEN;
        g_video_timer_flag |= clr_video_filename_timer;
        g_video_timer_flag |= clr_video_progress_timer;
        //全屏时只清除背景图，其它不显示
        g_display_bitmap = 0;
        g_display_bitmap |= up_videoBackGDPic;
        g_video_paint_flag = VIDEO_PAINT_PARTIAL;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    检测窗口是否发生变化
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 * \li  如果要显示的窗口和当前窗口不一致，需要发命令给视频中间件，调整刷屏参数
 *******************************************************************************/
static void _video_check_window(void)
{
    if (g_video_cur_window != g_video_prev_window)
    {
        //需要进行全屏，PLAYBAR和VOLUMEBAR三种界面的转换
        if (g_video_cur_window == VIDEO_PROGRESSBAR)
        {
            mmm_vp_cmd(g_video_handle, MMM_VP_PROGRESSBAR, (uint32) & g_video_userinfor);
            g_video_timer_flag |= up_video_filename_timer;
            g_video_timer_flag |= up_video_progress_timer;
            g_video_timer_flag |= up_video_fullscreen_timer;
            g_video_paint_flag = VIDEO_PAINT_ALL;
        }
        else if (g_video_cur_window == VIDEO_VOLUMEBAR)
        {
            mmm_vp_cmd(g_video_handle, MMM_VP_VOLUMEBAR, (uint32) & g_video_userinfor);
            if (g_video_prev_window == VIDEO_PROGRESSBAR)
            {
                //需要消去上面的banner,因此启动全屏定时器
                //对于播放状态，会从VOLUMEBAR-->FULLSCREEN
                //对于暂停状态，会从VOLUMEBAR-->PROGRESSBAR
                g_video_timer_flag |= up_video_fullscreen_timer;
            }
            g_video_timer_flag |= clr_video_filename_timer;
            g_video_timer_flag |= clr_video_progress_timer;
        }
        else
        {
            mmm_vp_cmd(g_video_handle, MMM_VP_NOPROGRESS, (uint32) & g_video_userinfor);
        }
        g_video_prev_window = g_video_cur_window;
    }
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    开启或停止定时器
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 * \li  video在播放过程中有三个定时器，在主循环中根据g_video_timer_flag标志位
 *      设置或清除相应的标志位
 *******************************************************************************/
void _video_deal_timer(void)
{
    uint8 i;
    int8 timer_id[] =
    { timer_flush_screen, timer_flush_filename, timer_flush_progress }; //bit0,1,2

    for (i = 0; i < 3; i++)
    {
        if ((g_video_timer_flag & (1 << i)) != 0)
        {
            restart_app_timer(timer_id[i]);
            g_video_timer_flag &= (uint8)(~(1 << i));
        }
    }

    for (i = 3; i < 6; i++)
    {
        if ((g_video_timer_flag & (1 << i)) != 0)
        {
            stop_app_timer(timer_id[i - 3]);
            g_video_timer_flag &= (uint8)(~(1 << i));
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video 播放场景处理函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       返回值在video_scene_exit()会进一步处理
 * \ingroup      video_play
 * \note
 *******************************************************************************/
app_result_e video_play(void)
{
    //定义消息变量
    input_gui_msg_t ui_msg;
    private_msg_t pri_msg;
    decode_status_e dec_status;

    bool result = FALSE;
    bool need_free_mmm = FALSE;
    bool need_play = TRUE;

    app_result_e ui_result = RESULT_NULL;

    while (1)
    {
        if ((g_video_change_file & up_video_changefile) != 0)
        {
            if (video_change_file() == TRUE)
            {
                need_play = TRUE;
            }
            else
            {
                ui_result = RESULT_VIDEO_RETURN;
                need_free_mmm = FALSE;
                goto dec_end;
            }
        }
        if (need_play == TRUE)
        {
            need_play = FALSE;
            ui_clear_screen(NULL);
            g_video_play_status = VIDEO_PLAY;
            g_file_total = g_video_var.path.file_path.dirlocation.file_total;
            dec_status = video_start_decode(&need_free_mmm);
            if (dec_status == DECODE_NO_ERR)
            {
                vfs_get_name(video_mount, g_video_filename, LIST_ITEM_BUFFER_LEN / 2);
                read_filename_to_buffer(g_video_filename, LIST_ITEM_BUFFER_LEN);
                g_file_err = 0;
                g_video_paint_flag = VIDEO_PAINT_ALL;
            }
            else
            {
                //出现错误首先检测卡是否存在
                ui_result = video_check_card_out();

                if (ui_result == RESULT_CARD_OUT_ERROR)
                {
                    goto dec_end;
                }

                if (dec_status == DECODE_FILE_ERR)
                {
                    gui_dialog_msg(DIALOG_MSG_V, DIALOG_INFOR_WAIT, S_FORMAT_ERROR);
                    if (g_file_err >= g_file_total)
                    {
                        ui_result = RESULT_VIDEO_RETURN;
                        goto dec_end;
                    }
                    else
                    {
                        //切换歌曲
                        g_file_err++;
                        g_video_change_file |= up_video_changefile;
                        continue;
                    }
                }
                else
                {
                    ui_result = RESULT_VIDEO_RETURN;
                    goto dec_end;
                }

            }
        }

        _video_deal_timer();

        //获取和处理gui消息
        result = get_gui_msg(&ui_msg);
        if (result == TRUE)
        {
            ui_result = video_playing_proc_gui_msg(&ui_msg);
        }
        else
        {
            //处理ap私有消息
            result = get_app_msg(&pri_msg);
            if (result == TRUE)
            {
                ui_result = video_msg_callback(&pri_msg);
                //恢复刷屏模式
                lcd_set_draw_mode(g_video_draw_mode);
            }

        }

        if ((ui_result == RESULT_NULL) || (ui_result == RESULT_REDRAW))
        {
            //是播放界面有效状态，要检测解码状态
            ui_result = video_check_play_status();

            if (ui_result != RESULT_NULL)
            {
                //遇见无效文件,返回
                goto dec_end;
            }
        }
        else
        {
            //非有效状态，要退出播放界面循环
            //关闭文件句柄
            goto dec_end;
        }

        //检测当前窗状态并调整刷新标志和定时器
        _video_check_window();

        if (g_video_paint_flag != VIDEO_PAINT_NONE)
        {
            if (g_video_paint_flag == VIDEO_PAINT_PARTIAL)
            {
                //界面部分更新
                video_refresh_screen(FALSE);
            }
            else
            {
                video_set_full_flag()
                video_refresh_screen(TRUE);
            }
            g_video_paint_flag = VIDEO_PAINT_NONE;
        }

        sys_os_time_dly(1);
    }

    dec_end: if (need_free_mmm == TRUE)
    {
        video_decoder_free();
    }

    //恢复RGB刷屏模式
    lcd_controller_init(RGB_MODE);

    return ui_result;
}

