/*******************************************************************************
 *                              US212A
 *                            Module: VIDEO_PLAY_SUB
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-2-3 17:00:44           1.0              build this file
 *******************************************************************************/
/*!
 * \file     video_play_sub.c
 * \brief    视频解码子模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2012-2-3
 *******************************************************************************/

#include "video.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    清除视频断点信息
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_clear_breakinfor(void)
{
    g_video_var.break_point.bpbyte = 0;
    g_video_var.break_point.framecnt = 0;
    g_video_var.break_point.time = 0;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    退出视频播放，释放视频中间件所占用空间
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_decoder_free(void)
{
    int32 dec_result;

    dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_CLOSE, 0);
    if (dec_result == MMM_CMD_ERR)
    {
        while (1)
        {
            ;//todo
        }
    }
    sys_free_mmm(FALSE);
}
/******************************************************************************/
/*!
 * \par  Description:
 *    视频切换文件函数
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_play
 * \note
 *******************************************************************************/
bool video_change_file(void)
{
    bool result = FALSE;
    uint8 *file_name = g_video_var.path.file_path.dirlocation.filename;

    //清时间断点
    video_clear_breakinfor();

    //释放视频中间件空间
    video_decoder_free();

    if ((g_video_change_file & up_video_playprev) == up_video_playprev)
    {
        result = fsel_get_prevfile(file_name);
    }
    else
    {
        result = fsel_get_nextfile(file_name);
    }

    if (result == TRUE)
    {
        fsel_get_location((void *) &g_video_var.path.file_path, g_video_var.path.file_source);
    }
    g_video_change_file = (uint8)(g_video_change_file & clr_video_changefile);
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    获取视频文件信息，并根据信息设置旋转模式
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \ingroup      video_play
 * \note         
 * \li  若文件格式错误，则返回FALSE，否则返回TRUE
 *******************************************************************************/
bool video_get_file_information(void)
{
    bool ret_val = FALSE;
    int32 dec_result;
    mmm_vp_file_info_t fileinfo;

    /* 初始化图片解码器*/
    dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_SET_FILE, (uint32) (&g_video_userinfor));
    if (dec_result == MMM_CMD_OK)
    {
        /*获取视频解码信息*/
        dec_result = mmm_vp_cmd(g_video_handle, MMM_VP_GET_MEDIA_INFO, (unsigned int) &fileinfo);

        if (fileinfo.file_len != 0)
        {
            if (fileinfo.type == VIDEO_AMV)
            {
                g_video_draw_mode = DRAW_MODE_V_PIC_DEF;
                g_region_progressbar.y = g_region_adjust.amv_prog_y;
                g_region_volumebar.y = g_region_adjust.amv_vol_y;
            }
            else
            {
                g_video_draw_mode = DRAW_MODE_AVI_DEF;
                g_region_progressbar.y = g_region_adjust.avi_prog_y;
                g_region_volumebar.y = g_region_adjust.avi_vol_y;
            }
            g_total_time = fileinfo.total_time;
            
            //设置视频刷屏模式
            lcd_set_draw_mode(g_video_draw_mode);
            ret_val = TRUE;
        }

    }
    return ret_val;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    视频解码初始化
 * \param[in]    need_free_mmm  视频中间件是否已加载标志，如果已加载，退出时需卸载
 * \param[out]   void
 * \return       bool
 * \ingroup      video_play
 * \note         
 * \li  若文件格式错误，则返回FALSE，否则返回TRUE
 *******************************************************************************/
decode_status_e video_start_decode(bool *need_free_mmm)
{
    int32 dec_result;
#ifdef PC
	return DECODE_NO_ERR;
#endif
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_1);
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);
    if (-1 == sys_load_mmm("mmm_vp.al", FALSE))
    {
        *need_free_mmm = FALSE;
        return DECODE_LOAD_ERR;
    }

    //打开线程句柄
    dec_result = mmm_vp_cmd(&g_video_handle, MMM_VP_OPEN, 0);
    if (g_video_handle == NULL)
    {
        //打开文件句柄出错
        *need_free_mmm = TRUE;
        return DECODE_OPEN_ERR;
    }

    //解析文件信息
    if (FALSE == video_get_file_information())
    { 
        *need_free_mmm = TRUE;
        return DECODE_FILE_ERR;
    }
    else
    {
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_prev_window = VIDEO_FULLSCREEN;
        g_video_timer_flag |= up_video_fullscreen_timer;
        g_video_timer_flag |= up_video_filename_timer;
        g_video_timer_flag |= up_video_progress_timer;
    }
    *need_free_mmm = TRUE;
    return DECODE_NO_ERR;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video 获取私有消息的处理
 * \param[in]    msg video 的处理的各种私有消息的回调函数
 * \param[out]   none
 * \return       应用类型对应的文件总数或者电台总数
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e video_msg_callback(private_msg_t *pri_msg)
{
    app_result_e result = RESULT_NULL;
    engine_type_e engine_type;
    //msg_reply_t m_reply;
    /*同步消息处理*/
    switch (pri_msg->msg.type)
    {
        case MSG_SD_OUT:
        if (g_video_var.path.file_path.dirlocation.disk == DISK_H)
        {
            if(com_get_gui_direction() == GUI_DIRECT_NORMAL)
            {
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            }
            else
            {
                gui_dialog_msg(DIALOG_MSG_V, DIALOG_INFOR_WAIT, S_CARD_OUT);
            }

            //正在访问卡盘文件，退回到主应用
            engine_type = get_engine_type();

            if (engine_type == ENGINE_MUSIC)
            {
                music_close_engine();
            }
            result = RESULT_CARD_OUT_ERROR;
        }
        else
        {
            //播放场景关注该消息
            if(g_video_scene_next == SCENE_PLAY)
            {
                if(g_video_cur_window != VIDEO_PROGRESSBAR)
                {                    
                    result = RESULT_REDRAW;
                }
                else
                {
                    //直接更新卡图标
                    g_video_paint_flag = VIDEO_PAINT_ALL;
                    g_display_bitmap |= up_videoheaderbar;
                }
            }
        }
        break;

        case MSG_SD_IN:
        //播放场景关注该消息
        if (g_video_scene_next == SCENE_PLAY)
        {
            if(g_video_cur_window != VIDEO_PROGRESSBAR)
            {                
                result = RESULT_REDRAW;
            }
            else
            {
                //直接更新卡图标            
                g_video_paint_flag = VIDEO_PAINT_ALL;
                g_display_bitmap |= up_videoheaderbar;
            }

        }
        break;

        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(pri_msg->msg.type);
        break;
    }

    if (result == RESULT_REDRAW)
    {
        video_set_full_flag()
        g_video_cur_window = VIDEO_PROGRESSBAR;
        g_video_prev_window = VIDEO_FULLSCREEN;
        //重绘界面，强制设置为播放状态
        g_video_play_status = VIDEO_PLAY;
        //清屏
        ui_clear_screen(NULL);
    }

    //恢复刷屏模式
    lcd_set_draw_mode(g_video_draw_mode);
    return result;

}


