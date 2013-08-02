/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-11-14          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

extern app_result_e scene_play_search(FM_SeekDir_e dir);

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 get_index_from_num(uint8 num)
 * \用户电台中根据电台号获取索引
 * \param[in]    用户电台电台号
 * \param[in]
 * \param[out]   none
 * \return       索引号
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
uint8 get_index_from_num(uint8 num)
{
    uint8 index;

    for (index = 0; index < MAX_STATION_COUNT; index++)
    {
        if (g_userlist_table[index] == num)
        {
            break;
        }
    }

    if (index == MAX_STATION_COUNT)
    {
        index = 0xff;
    }
    return index;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  select_station(FM_SeekDir_e dir)
 * \切换到上一个或下一个已保存电台
 * \param[in]    dir 切换方向
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e select_station(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_IGNORE;
    /* 当前播放为用户电台模式*/
    if (g_radio_config.band_mode == Band_MAX)
    {
        if (dir == DIR_UP)
        {
            //非保存电台切到第一个保存电台
            if (g_userlist_index == 0xff)
            {
                g_userlist_index = 0;
            }
            else if (g_userlist_index < (g_userlist_total - 1))
            {
                g_userlist_index++;
            }
            else
            {
                //最后回第一项
                g_userlist_index = 0;
            }
        }
        else
        {
            //非保存电台切到第一个保存电台
            if (g_userlist_index == 0xff)
            {
                g_userlist_index = 0;
            }
            else if (g_userlist_index > 0)
            {
                g_userlist_index--;
            }
            else
            {
                //第一项回最后
                g_userlist_index = g_userlist_total - 1;
            }
        }
        //电台号
        g_radio_config.FMStatus.station = g_userlist_table[g_userlist_index];
        //通过电台号获取频率值
        if ((g_radio_config.FMStatus.station > 0) && (g_radio_config.FMStatus.station <= MAX_STATION_COUNT))
        {
            g_radio_config.FMStatus.freq = g_user_station.fmstation_user[g_radio_config.FMStatus.station - 1];
        }
        //用户电台刷新电台名称
        paint_flag |= PAINT_STATION_INFOR;
    }
    /* 当前播放非用户模式*/
    else
    {
        //预设列表中有已保存电台
        if (g_radio_config.FMStatus.station_count > 0)
        {
            if (dir == DIR_UP)
            {
                if (g_radio_config.FMStatus.station < MAX_STATION_COUNT)
                {
                    g_radio_config.FMStatus.station++;
                }
                else
                {
                    //最后回第一项
                    g_radio_config.FMStatus.station = 1;
                }
                g_radio_config.FMStatus.station = get_real_station_num(g_radio_config.FMStatus.station, DIR_UP);
            }
            else
            {
                if (g_radio_config.FMStatus.station > 1)
                {
                    g_radio_config.FMStatus.station--;
                }
                else if (g_radio_config.FMStatus.station == 0)
                {
                    //非保存电台切到第一个保存电台
                    g_radio_config.FMStatus.station = 1;
                }
                else
                {
                    //第一项回最后
                    g_radio_config.FMStatus.station = MAX_STATION_COUNT;
                }
                g_radio_config.FMStatus.station = get_real_station_num(g_radio_config.FMStatus.station, DIR_DOWN);
            }
            //通过电台号获取频率值
            g_radio_config.FMStatus.freq = *(pfreq + g_radio_config.FMStatus.station - 1);
        }
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e change_freq(FM_SeekDir_e dir)
 * \切换到上一个或下一个频率
 * \param[in]    dir 切换方向
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e change_freq(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_IGNORE;
    uint8 space;
    uint8 num;

    space = 100;
    if ((g_radio_config.band_mode == Band_Europe) || (g_radio_config.band_mode == Band_MAX))
    {
        //用户模式或欧洲频段,  以50k  步进
        space = 50;
    }
    //if (g_radio_config.band_mode > Band_MAX)
    //{
    //    g_radio_config.band_mode = Band_China_US;
    //}

    if (dir == DIR_UP)
    {
        if (g_radio_config.FMStatus.freq < max_freq[g_radio_config.band_mode])
        {
            g_radio_config.FMStatus.freq += space;
        }
        else
        {
            //最大频点切换到最小
            g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        }
    }
    else
    {
        if (g_radio_config.FMStatus.freq > min_freq[g_radio_config.band_mode])
        {
            g_radio_config.FMStatus.freq -= space;
        }
        else
        {
            //最小频点切换到最大
            g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
        }
    }

    //判断切换后的频点是否在当前所播放的列表中
    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);

    //切换后的频点是未保存频点
    if (num == 0xff)
    {
        if (g_radio_config.band_mode == Band_MAX)
        {
            g_userlist_index = 0xff;
            //用户电台刷新电台名称
            paint_flag |= PAINT_STATION_INFOR;
        }
        g_radio_config.FMStatus.station = 0;
    }
    //切换后的频点是已保存频点
    else
    {
        if (g_radio_config.band_mode == Band_MAX)
        {
            //根据电台号获取索引
            g_userlist_index = get_index_from_num(num);
            if (g_userlist_index == 0xff)
            {
                //没找到索引，认为未保存
                num = 0;
            }
            //用户电台刷新电台名称
            paint_flag |= PAINT_STATION_INFOR;
        }
        g_radio_config.FMStatus.station = num;
    }

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_playstatus(void)
 * \radio 播放场景播放暂停切换处理
 * \param[in]    none
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e deal_playstatus(void)
{
    app_result_e ret = RESULT_IGNORE;
    bool status;

    //获取当前音量状态
    status = radio_get_vol_status();

    if (g_engine_status.FM_VolStatus == STATUS_PLAY)
    {
        //播放->  暂停
        radio_set_mute(SetMUTE);
    }
    else
    {
        //暂停-> 播放
        radio_set_mute(releaseMUTE);
    }
    //刷新音量图标
    paint_flag |= PAINT_VOL;
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e playwin_key_return(radio_playwin_mode_e mode)
 * \radio 播放场景return 键处理
 * \param[in]    mode
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e playwin_key_return(radio_playwin_mode_e mode)
{
    app_result_e ret = RESULT_IGNORE;

    if (mode == ENTER_PLAY_FROM_NOWPLAY)
    {
        //返回mainmenu ap
        ret = RESULT_MAIN_APP;
    }
    else if (mode == ENTER_PLAY_FROM_LIST)
    {
        if (g_radio_listtype == STATIONLIST_USER)
        {
            //用户列表进来
            ret = RESULT_RADIO_USER_LIST;
        }
        else
        {
            //预设列表进来
            g_stalist_entry = ENTER_NORMAL;
            ret = RESULT_RADIO_STATION_LIST;
        }
    }
    else
    {
        //其他情况，返回radio  主菜单
        ret = RESULT_RADIO_MAINMENU;
#ifdef ALARMFM
        if (g_radio_from_alarm != 0)
        {
            ret = RESULT_ALARM_QUIT;
        }
#endif
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_play_guimsg(msg_apps_type_e cur_event, radio_playwin_mode_e mode)
 * \radio 播放场景gui消息处理
 * \param[in]    cur_event  按键触发的消息事件
 * \param[in]    mode  进入播放场景的模式, 决定了返回到哪个场景
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e deal_play_guimsg(msg_apps_type_e cur_event, radio_playwin_mode_e mode)
{
    app_result_e result = RESULT_IGNORE;
    bool set_freq = FALSE;
    bool need_play = FALSE;

    switch (cur_event)
    {
        //下一个保存电台
        case EVENT_RADIO_NEXT_PRESET:
        result = select_station(DIR_UP);
        set_freq = TRUE;
        break;

        //上一个保存电台
        case EVENT_RADIO_PREV_PRESET:
        result = select_station(DIR_DOWN);
        set_freq = TRUE;
        break;

        //切换到下一个频率
        case EVENT_RADIO_NEXT_FREQ:
        result = change_freq(DIR_UP);
        set_freq = TRUE;
        break;

        //切换到上一个频率
        case EVENT_RADIO_PREV_FREQ:
        result = change_freq(DIR_DOWN);
        set_freq = TRUE;
        break;

        //向后半自动搜台
        case EVENT_RADIO_SEEK_NEXT:
        result = scene_play_search(DIR_UP);
        set_freq = TRUE;
        need_play = TRUE;
        break;

        //向前半自动搜台
        case EVENT_RADIO_SEEK_PREV:
        result = scene_play_search(DIR_DOWN);
        set_freq = TRUE;
        need_play = TRUE;
        break;

        //进入播放场景的option 菜单
        case EVENT_RADIO_OPTION:
        result = RESULT_RADIO_OPTION_PLAYWIN;
        break;

        //播放暂停切换
        case EVENT_RADIO_PLAY_MUTE:
        result = deal_playstatus();
        break;

        //返回按键
        case EVENT_RADIO_RETURN:
        result = playwin_key_return(mode);
        break;

        default:
        //热键消息处理
        if (timer_flush_radioinfo != -1)
        {
            kill_app_timer(timer_flush_radioinfo);
            timer_flush_radioinfo = -1;
        }
        result = com_message_box(cur_event);

        if ((result == RESULT_IGNORE) || (result == RESULT_NULL) || (result == RESULT_CONFIRM))
        {
            result = RESULT_REDRAW;
        }
        else if (result == RESULT_ALARMFM) //播放过程中遇fm alarm消息
        {
            g_radio_from_alarm = 1;
            alarmfm_in_playing();
            result = RESULT_REDRAW;
        }
        else
        {
            ; //qac
        }
        break;
    }
    if (result == RESULT_REDRAW)
    {
        paint_flag |= (uint8) PAINT_ALL;
    }
    if (set_freq == TRUE)
    {
        //刷新频率相关显示
        paint_flag |= PAINT_FREQ;
        //更新频点播放
        radio_set_freq(g_radio_config.FMStatus.freq);

    }
    if (need_play == TRUE)
    {
        //半自动搜台后，释放静音播放
        if (get_antenna_state() == TRUE)
        {
            //有连接耳机, 才打开PA
            //为避免搜到台后,由于按键仍按下,导致耳机检测异常出现无法释放静音,此处调用common接口判断耳机连接状态
            radio_set_mute(releaseMUTE);
        }
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_mute(FM_MUTE_e mode)
 * \设置静音或解除静音
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_mute(FM_MUTE_e mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = (uint8) mode;
    msg.content.data[1] = (uint8) g_comval.volume_current;

    msg.type = MSG_FMENGINE_SETMUTE;
    //发送同步消息
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;

    }
    return bret;
}
