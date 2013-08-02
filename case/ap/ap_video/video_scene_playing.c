/*******************************************************************************
 *                              US212A
 *                            Module: VIDEO_SCENE_PLAYING
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-2-3 17:01:20           1.0              build this file
 *******************************************************************************/
/*!
 * \file     video_scene_playing.c
 * \brief    视频播放场景模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2012-2-3
 *******************************************************************************/
#include "video.h"

static const uint16 number_res[MAX_NUMBER] =
{
    VIDEO_NUM0, VIDEO_NUM1, VIDEO_NUM2, VIDEO_NUM3, VIDEO_NUM4,
    VIDEO_NUM5, VIDEO_NUM6, VIDEO_NUM7, VIDEO_NUM8, VIDEO_NUM9,
    VIDEO_NUMCOLON
};

static const uint16 headbar_res[MAX_HEADBAR_ICON] =
{
    P_CARD_IN, P_CARD_PLAY, P_APP_LOCK_V
};

static const uint16 battery_res[MAX_BATTERY] =
{
    P_BATTERY_VLOW, P_BATTERY_V0, P_BATTERY_V1,
    P_BATTERY_V2,    P_BATTERY_V3, P_BATTERY_V4
};

static const uint16 volume_res[] =
{
    P_VIDEO_VOL_UP_TAG, P_VIDEO_VOL_DOWN_TAG,
    P_VIDEO_VOL_CUR_TAG, P_VIDEO_VOL_LIMIT
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
 *    关闭所有后台存在的引擎
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void close_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;
    engine_state_e engine_status;

    engine_status = get_engine_state();

    if (engine_status != ENGINE_STATE_NULL)
    {
        engine_type = get_engine_type();

        //向process manager发消息要求后台退出
        msg.type = MSG_KILL_APP_SYNC;
        if (engine_type == ENGINE_RADIO)
        {
            msg.content.data[0] = APP_ID_FMENGINE;
        }
        else if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MENGINE;
        }
        else
        {
            ;//bug
        }
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video开启音乐播放时
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      video_play
 * \note
 *******************************************************************************/
bool _video_start_sound(uint8 cur_volume)
{
    uint8 volum_set;
    uint8 tab_index;

    enable_dac(4, 22);
    //转换音量
    tab_index = (31 - cur_volume);
    volum_set = (video_volume_tbl[tab_index]& 0x3f);
    set_pa_volume((uint32) volum_set);
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video 播放场景初始化函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      video_play
 * \note
 *******************************************************************************/
bool _video_stop_sound(void)
{
    disable_dac();
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video 播放场景初始化函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      video_play
 * \note
 *******************************************************************************/
bool video_scene_playing_init(void)
{
    bool ret_val;
    uint32 cur_cluster_no;
    uint32 cur_dir_entry;

    //先关引擎
    close_engine();

    com_set_sound_out(TRUE, SOUND_OUT_START, _video_start_sound);
    com_set_gui_direction(GUI_DIRECT_HORIZONTAL);

    if (g_video_var.path.file_path.dirlocation.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            gui_dialog_msg(DIALOG_MSG_V, DIALOG_INFOR_WAIT, S_CARD_OUT);
            return FALSE;
        }
        change_card_state(CARD_STATE_CARD_PLAY);
    }

    if (g_video_enter_mode == PARAM_FROM_MAINMENU)
    {
        this_headbar_icon_id = HEADBAR_ICON_VIDEO_V;
    }

    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    timer_flush_filename = set_app_timer(APP_TIMER_ATTRB_UI, 100, filename_flag_update);
    timer_flush_progress = set_app_timer(APP_TIMER_ATTRB_UI, 1000, play_flag_update);
    timer_flush_screen = set_app_timer(APP_TIMER_ATTRB_UI, 5000, video_full_display);
    g_video_change_file = 0;

    //设置要播放文件路径
    ret_val = fsel_set_location(&g_video_var.path.file_path, g_video_var.path.file_source);

    if (ret_val == FALSE)
    {
        ret_val = fsel_get_nextfile(g_video_var.path.file_path.dirlocation.filename);
        if (ret_val == TRUE)
        {
            ret_val = fsel_get_location(&g_video_var.path.file_path, g_video_var.path.file_source);
        }
    }

    cur_cluster_no = g_video_var.path.file_path.dirlocation.cluster_no;
    cur_dir_entry = g_video_var.path.file_path.dirlocation.dir_entry;
    if ((g_video_var.bk_cluster_no != cur_cluster_no) || (g_video_var.bk_dir_entry != cur_dir_entry))
    {
        //清时间断点
        video_clear_breakinfor();
    }
    change_app_state(APP_STATE_PLAYING_ALWAYS);
    read_num_pic_to_uram();
    sys_set_video_app(2);
    return ret_val;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video 播放场景退出函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      video_play
 * \note
 *******************************************************************************/
void video_scene_playing_exit(void)
{
    sys_set_video_app(0);
    //保存当前文件路径信息
    fsel_get_location((void *) &g_video_var.path.file_path, g_video_var.path.file_source);
    kill_app_timer(timer_flush_filename);
    kill_app_timer(timer_flush_progress);
    kill_app_timer(timer_flush_screen);

    g_video_var.bk_cluster_no = g_video_var.path.file_path.dirlocation.cluster_no;
    g_video_var.bk_dir_entry = g_video_var.path.file_path.dirlocation.dir_entry;

    com_set_gui_direction(GUI_DIRECT_NORMAL);
    change_app_state(APP_STATE_NO_PLAY);
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);

    com_set_sound_out(FALSE, SOUND_OUT_STOP, _video_stop_sound);

    if (g_video_var.path.file_path.dirlocation.disk == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            change_card_state(CARD_STATE_CARD_NULL);
        }
        else
        {
            change_card_state(CARD_STATE_CARD_IN);
        }
    }
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video 播放场景函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      video_play
 * \note
 *******************************************************************************/
app_result_e video_scene_playing(void)
{
    uint8 ret_val;
    app_result_e ui_result;

    headbar_icon_e headbar_backup = this_headbar_icon_id;

    ret_val = video_scene_playing_init();

    if (ret_val == TRUE)
    {
        ui_result = video_play();
    }
    else
    {
        ui_result = RESULT_VIDEO_RETURN;
    }

    video_scene_playing_exit();

    this_headbar_icon_id = headbar_backup;
    if (ui_result == RESULT_VIDEO_RETURN)
    {
        if (g_video_enter_mode == PARAM_FROM_MAINMENU)
        {
            ui_result = RESULT_VIDEO_FILELIST;
        }
        else
        {
            ui_result = RESULT_RETURN_BROWSER;
        }

        if (g_video_var.path.file_path.dirlocation.disk == DISK_H)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
            {
                ui_result = RESULT_CARD_OUT_ERROR;
            }
        }

    }

    if (ui_result == RESULT_APP_QUIT)
    {
        //诸如低电关机,alarm,usb线接入，保存
        //当前video播放状态
        g_app_info_state.video_state = 1;
    }
    else
    {
        g_app_info_state.video_state = 0;
    }

    return ui_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video 播放场景初始化缓存图片数据
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      video_play
 * \note
 * \li  缓存video播放场景所需图片到URAM1/URAM3所在空间，不要和中间件地址冲突
 *******************************************************************************/
void read_num_pic_to_uram(void)
{
    uint8 i;
    uint32 pic_size = 0;

    //缓存时间控件元素
#ifndef PC
    video_num_addr = PIC_BUFFER_BASE;
#else
    video_num_addr = GET_REAL_ADDR(PIC_BUFFER_BASE);
#endif

    for (i = 0; i < MAX_NUMBER; i++)
    {
        ui_read_pic_to_buffer(number_res[i], (uint8 *) (video_num_addr + i * pic_size), &video_num_res);
        if (i == 0)
        {
            pic_size = video_num_res.width * video_num_res.height * 2;
        }
    }

    video_headbar_addr = video_num_addr + pic_size * MAX_NUMBER;
    for (i = 0; i < MAX_HEADBAR_ICON; i++)
    {
        ui_read_pic_to_buffer(headbar_res[i], (uint8 *) (video_headbar_addr + i * pic_size), &video_headbar_res);
        if (i == 0)
        {
            pic_size = video_headbar_res.width * video_headbar_res.height * 2;
        }
    }

    video_battery_addr = video_headbar_addr + pic_size * MAX_HEADBAR_ICON;
    for (i = 0; i < MAX_BATTERY; i++)
    {
        ui_read_pic_to_buffer(battery_res[i], (uint8 *) (video_battery_addr + i * pic_size), &video_battery_res);
        if (i == 0)
        {
            pic_size = video_battery_res.width * video_battery_res.height * 2;
        }
    }
    video_progress_addr = video_battery_addr + pic_size * MAX_BATTERY;
    ui_read_pic_to_buffer(VIDEO_PRO_BG, (uint8 *) video_progress_addr, &video_progress_res);
    pic_size = video_progress_res.width * video_progress_res.height * 2;

    video_vol_tag_addr = video_progress_addr + pic_size;
    for (i = 0; i < 4; i++)
    {
        ui_read_pic_to_buffer(volume_res[i], (uint8 *) (video_vol_tag_addr + i * pic_size), &video_vol_tag_res);
        if (i == 0)
        {
            pic_size = video_vol_tag_res.width * video_vol_tag_res.height * 2;
        }
    }

    video_vol_forbidden_addr = video_vol_tag_addr + pic_size * 4;
    ui_read_pic_to_buffer(P_VIDEO_VOL_FORBIDDEN, (uint8 *) video_vol_forbidden_addr, &video_vol_forbidden_res);
    pic_size = video_vol_forbidden_res.width * video_vol_forbidden_res.height * 2;

    video_vol_bg_addr = video_vol_forbidden_addr + pic_size;
    ui_read_pic_to_buffer(P_VIDEO_VOL_BG, (uint8 *) video_vol_bg_addr, &video_vol_bg_res);
    pic_size = video_vol_bg_res.width * video_vol_bg_res.height * 2;

    video_cache_name.font_addr = video_vol_bg_addr + pic_size;

    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video 播放场景初始化缓存文件名
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \ingroup      video_play
 * \note
 * \li  缓存video播放场景所需图片到URAM1/URAM3所在空间，不要和中间件地址冲突
 *******************************************************************************/
void read_filename_to_buffer(const uint8 *str, uint8 str_len)
{
    uint16 font_mode;
    style_infor_t video_style;
    string_desc_t str_desc;
    textline_param_t textline_param;
    region_t textline_region;

    video_style.style_id = STY_VIDEO_FILENAME_TEXTBOX;
    video_style.type = UI_AP;

    //获取字符属性
    ui_get_textbox_attrb(&video_style, &font_mode, 1);
    //获取字符串区域属性
    ui_get_textbox_attrb(&video_style, &textline_region, 0);

    if ((g_video_filename[0] != 0xff) || (g_video_filename[1] != 0xfe))
    {
        com_dot_to_shortname(g_video_filename);
        str_desc.language = (int8) g_comval.language_id;
    }
    else
    {
        str_desc.language = UNICODEDATA;
    }
    str_desc.data.str = g_video_filename;
    str_desc.length = str_len;
    //区域的最大宽度
    str_desc.argv = textline_region.width;

    textline_param.point_buf = (uint8 *)(video_cache_name.font_addr);

    ui_get_text_line(&str_desc, (font_mode & DISP_FONT_OPTION) | FUNC_GET_CHAR_POINTS, &textline_param);

    video_cache_name.word_height = textline_param.true_height;
    video_cache_name.word_width = textline_param.true_width;
    video_cache_name.text_width = textline_region.width;

    return;
}

