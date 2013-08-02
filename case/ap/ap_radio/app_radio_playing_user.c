/*******************************************************************************
 *                              US212A
 *                            Module: radio_ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeayng    2011-09-14          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

extern void scroll_station_info(void)
__FAR__;

/******************************************************************************/
/*!
 * \par  Description:
 * \bool scene_play_init(void)
 * \radio 播放场景初始化
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       success
 * \retval       fail
 * \note
 */
/*******************************************************************************/
bool scene_play_init(void)
{
    bool bret;

    bret = radio_engine_init();
    if (!bret)
    {
        return FALSE;
    }
#ifdef ALARMFM
    if (g_radio_from_alarm != 0)
    {
        radio_set_mute(releaseMUTE);
        //set_pa_volume((uint32) g_comval.volume_current);//alarmfm_volume);
    }
#endif
    //进入播放场景，初始化频率表指针
    init_point_freq();

    //当前为用户播放模式
    if (g_radio_config.band_mode == Band_MAX)
    {
        //根据电台号，获取索引
        if (g_radio_config.FMStatus.station != 0)
        {
            g_userlist_index = get_index_from_num(g_radio_config.FMStatus.station);
        }
        else
        {
            g_userlist_index = 0xff;
        }
    }
    //设置为播放状态
    change_app_state(APP_STATE_PLAYING);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool scene_play_exit(void)
 * \radio 播放场景退出
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval       success
 * \retval       fail
 * \note
 */
/*******************************************************************************/
bool scene_play_exit(void)
{
    //删除用户电台名称滚屏定时器
    if (timer_flush_radioinfo != -1)
    {
        kill_app_timer(timer_flush_radioinfo);
        timer_flush_radioinfo = -1;
    }
    //设置为非播放状态
    change_app_state(APP_STATE_NO_PLAY);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool show_station_info(bool need_show)
 * \播放场景下用户电台名称显示
 * \param[in]     need_draw    是否有电台名称要显示
 * \param[out]   none
 * \return       success/fail
 * \retval
 * \retval
 * \note  need_draw = FALSE   没有电台名称显示，只刷背景
 */
/*******************************************************************************/
bool show_station_info(bool need_show)
{
    style_infor_t info_style;
    textbox_private_t info_textbox_param;

    if (!need_show)
    {
        //刷背景，清除上次电台名
        ui_show_pic(TEXTBOX_PRESET_INFOR_BG, 21, 100);
        //无须滚屏，关闭滚屏定时器
        if (timer_flush_radioinfo != -1)
        {
            kill_app_timer(timer_flush_radioinfo);
            timer_flush_radioinfo = -1;
        }
        return TRUE;
    }
    //显示电台名称
    info_style.style_id = TEXTBOX_PRESET_INFOR;
    info_style.type = UI_AP;
    //名称项编码方式
    if (encode_mode == ENCODE_UNICODE)
    {
        info_textbox_param.lang_id = UNICODEDATA;
    }
    else
    {
        info_textbox_param.lang_id = (int8) g_comval.language_id;
    }
    info_textbox_param.str_id = -1;
    info_textbox_param.str_value = g_user_station.station_name;

    if (UI_NEED_SCROLL == (ui_result_e) ui_show_textbox(&info_style, &info_textbox_param, TEXTBOX_DRAW_NORMAL))
    {
        if (timer_flush_radioinfo == -1)
        {
            timer_flush_radioinfo = set_app_timer(APP_TIMER_ATTRB_UI, 80, scroll_station_info);
        }
    }
    else
    {
        //无须滚屏，关闭滚屏定时器
        if (timer_flush_radioinfo != -1)
        {
            kill_app_timer(timer_flush_radioinfo);
            timer_flush_radioinfo = -1;
        }
    }
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool get_default_name(void)
 * \获取默认的用户电台名称
 * \param[in]    none
 * \param[out]   none
 * \return       success/fail
 * \retval       none
 * \retval       none
 * \note   使用默认的用户电台时, 根据电台号获取名称
 */
/*******************************************************************************/
bool get_default_name(void)
{
    uint8 buf[3];
    bool ret = FALSE;

    libc_memset(g_user_station.station_name, 0, 40);
    /* 当前所播放频点是已保存的用户电台*/
    if (g_radio_config.FMStatus.station > 0)
    {
        //libc_memset(g_user_station.station_name, 0, 40);
        libc_memcpy(g_user_station.station_name, default_name, sizeof(default_name));
        libc_itoa(g_radio_config.FMStatus.station, &buf[0], 2); //01, 02, 03
        //目前默认用户电台只有3  个
        g_user_station.station_name[4] = (char) buf[1]; //_CH01_   _CH02_   _CH03_
        ret = TRUE;
    }
    return ret;
}

