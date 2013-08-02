/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-15          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

app_result_e deal_soft_search(FM_SeekDir_e dir);
app_result_e deal_hard_search(FM_SeekDir_e dir);

/* radio 半自动搜台场景按键事件映射*/
const key_map_t search_key_map_list[] =
{
    /*! 短按option事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_OPTION },
    /*!短按play 停止搜台*/
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! 短按KEY_VOL 按键停止搜台*/
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*!短按prev 停止搜台*/
    {{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*!短按next 停止搜台*/
    {{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! 最后一个映射事件 */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 * \bool  scene_search_init(void)
 * \radio 半自动搜台场景初始化
 * \param[in]    none
 * \param[out]   none
 * \return       success/fail
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
bool scene_search_init(void)
{
    bool ret = TRUE;

    radio_get_freq();
    startfreq = g_engine_status.FM_CurrentFreq;
    g_radio_config.FMStatus.freq = startfreq;

    //刷背景，清除上次电台名，即使无名称显示，多刷也没关系
    ui_show_pic(TEXTBOX_PRESET_INFOR_BG, 21, 100);
    //无须滚屏，关闭滚屏定时器
    if (timer_flush_radioinfo != -1)
    {
        kill_app_timer(timer_flush_radioinfo);
        timer_flush_radioinfo = -1;
    }
    //开始半自动搜台前，先mute
    radio_set_mute(SetMUTE);
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool scene_search_exit(void)
 * \radio 半自动搜台场景退出
 * \param[in]    none
 * \param[out]   none
 * \return       success/fail
 * \retval       none
 * \retval       none
 * \note     需确定退出后的频点和电台号，便于刷新界面
 */
/*******************************************************************************/
bool scene_search_exit(void)
{
    uint8 num;
    bool result;

    //获取当前频率值
    result = radio_get_freq();
    if (result == TRUE)
    {
        //正常获取，播放当前频点
        g_radio_config.FMStatus.freq = g_engine_status.FM_CurrentFreq;

        if ((g_radio_config.FMStatus.freq > max_freq[g_radio_config.band_mode]) || ((g_radio_config.FMStatus.freq
                < min_freq[g_radio_config.band_mode])))
        {
            //获取的频点不正确，播放半自动搜台前频点
            g_radio_config.FMStatus.freq = startfreq;
        }
    }
    else
    {
        //获取失败，播放半自动搜台前频点
        g_radio_config.FMStatus.freq = startfreq;
    }

    //判断当前频点是否是已保存频点
    num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);

    //切换后的频点是未保存频点
    if (num == 0xff)
    {
        if (g_radio_config.band_mode == Band_MAX)
        {
            g_userlist_index = 0xff;
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

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e scene_play_search(FM_SeekDir_e dir)
 * \radio 播放界面半自动搜台场景
 * \param[in]    dir   半自动搜台方向
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note  进入半自动搜台场景时，引擎应该已安装，模组也已
 * \        初始化，同时界面也不需全部刷新，只更新与频率
 * \        相关项即可，包括频率图片，电台号，tunebar等
 */
/*******************************************************************************/
app_result_e scene_play_search(FM_SeekDir_e dir)
{
    bool result;
    //获取gui消息
    //input_gui_msg_t gui_msg;
    app_result_e ret = RESULT_IGNORE;

#ifndef PC
    //过滤后续按键消息
    com_filter_key_hold();
    result = scene_search_init();
    if (!result)
    {
        //返回正常播放场景
        return RESULT_IGNORE;
    }

    if (g_radio_config.seek_mode == HARDSEEK)
    {
        //处理硬件半自动搜台
        ret = deal_hard_search(dir);
    }
    else
    {
        //处理软件半自动搜台
        ret = deal_soft_search(dir);
    }
    scene_search_exit();    
#endif
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 init_start_freq(FM_SeekDir_e dir)
 * \半自动搜台初始化起始频点
 * \param[in]    direction:   搜台方向
 * \param[out]   none
 * \return       步长space
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
uint8 init_start_freq(FM_SeekDir_e dir)
{
    uint8 space;
    //搜台步进
    space = 100;
    if ((g_radio_config.band_mode == Band_Europe) || (g_radio_config.band_mode == Band_MAX))
    {
        //欧洲波段或者在用户模式，步进调整为50K
        space = 50;
    }

    /* 设置初始判断频率*/
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
    return space;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_soft_search(FM_SeekDir_e dir)
 * \软件半自动搜台处理
 * \param[in]    direction:   搜台方向
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   一次设置一个频点进行tune，并判断是否真台
 * \         找到一个真台就停止，边界回绕，搜索一轮。
 */
/*******************************************************************************/
app_result_e deal_soft_search(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    bool is_true = FALSE;
    bool result;
    uint8 space;
    uint8 num;
    uint8 dir_seek;

    //获取gui消息
    input_gui_msg_t gui_msg;
    //gui 消息对应gui 事件
    msg_apps_type_e gui_event;

    space = init_start_freq(dir);

    if (dir == DIR_DOWN)
    {
        dir_seek = 0;
    }
    else
    {
        dir_seek = 1;
    }

    while (1)
    {
        //判断当前频点是否是已保存频点
        //并更新电台号
        num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);
        if (num == 0xff)
        {
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.station = num;
        }
        //界面刷新
        show_freq_in_play();
        show_station_num();

        //判断当前频率是否有效台
        is_true = radio_set_softseek(g_radio_config.FMStatus.freq, dir_seek);
        if (is_true == TRUE)
        {
            //过滤96MHZ 及 108MHZ 频点
            if ((g_radio_config.FMStatus.freq != 0x7700) && (g_radio_config.FMStatus.freq != 0xa5e0))
            {
                //是有效电台，完成退出
                break;
            }
        }

        //退出条件
        if (dir == DIR_UP)
        {
            //边界回绕
            if (g_radio_config.FMStatus.freq >= max_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
            }
            //搜完一轮，未找到有效电台，退出
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //更新频点
                g_radio_config.FMStatus.freq += space;
            }
        }
        else //if (dir == DIR_DOWN)
        {
            //边界回绕
            if (g_radio_config.FMStatus.freq <= min_freq[g_radio_config.band_mode])
            {
                g_radio_config.FMStatus.freq = max_freq[g_radio_config.band_mode];
            }
            //搜完一轮，未找到有效电台，退出
            if (g_radio_config.FMStatus.freq == startfreq)
            {
                break;
            }
            else
            {
                //更新频点
                g_radio_config.FMStatus.freq -= space;
            }
        }
        //else
        //{
        //    break;
        //}

        //处理系统消息和gui 消息
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {
            if (com_key_mapping(&gui_msg, &gui_event, search_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_RADIO_OPTION)
                {
                    //进入option 菜单
                    ret = RESULT_RADIO_OPTION_PLAYWIN;
                    break;
                }
                else if (gui_event == EVENT_RADIO_RETURN)
                {
                    //退出半自动搜台，回到播放场景
                    ret = RESULT_NULL;
                    break;
                }
                else
                {
                    ret = RESULT_NULL;
                    break;
                }
            }
        }
        else
        {
            //一次循环处理一种类型的消息
            ret = scene_play_sysmsg();
        }

        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL))
        {
            break;
        }
        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_hard_search(FM_SeekDir_e dir)
 * \硬件半自动搜台处理
 * \param[in]    direction:   搜台方向
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e deal_hard_search(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 dir_seek;
    bool result, need_restart = TRUE;
    uint16 current_freq, end_freq;
    radio_band_e mode;
    uint8 num;

    //获取gui消息
    input_gui_msg_t gui_msg;
    //gui 消息对应gui 事件
    msg_apps_type_e gui_event;

    //菜单波段
    mode = g_menu_band;
    if (g_radio_config.band_mode == Band_MAX)
    {
        //当前如为用户模式，设置space 为50k
        mode = Band_Europe;
    }
    //设置当前波段, 主要是确定space
    radio_set_band(mode);

    init_start_freq(dir);

    //起始频率
    current_freq = g_radio_config.FMStatus.freq;

    //先确定方向,是否回绕
    if (dir == DIR_UP)
    {
        //向上搜台,  回绕
        dir_seek = 0;
    }
    else
    {
        //向下搜台, 回绕
        dir_seek = 0x01;
    }

    //设置结束频率
    end_freq = startfreq;

    while (1)
    {
        //判断当前频点是否是已保存频点
        //并更新电台号
        num = whether_in_list(g_radio_config.FMStatus.freq, pfreq);
        if (num == 0xff)
        {
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.station = num;
        }
        //界面刷新
        show_freq_in_play();
        show_station_num();

        //是否需启动新一轮硬件seek
        if (need_restart == TRUE)
        {
            result = radio_set_hardseek(current_freq, dir_seek);
            need_restart = FALSE;
            if (!result)
            {
                //硬件搜台启动失败
                //退出半自动搜台，回到播放场景
                break;
            }
        }

        //硬件搜台启动成功,  取是否完成标志
        result = radio_get_seekflag();

        if ((result) || (g_engine_status.STC_flag == HARDSEEK_NOT_START))
        {
            //此轮搜台完成或seek fail, 退出
            radio_break_hardseek();
            break;
        }
        else
        {
            //此轮搜台尚未完成，取频率显示
            radio_get_freq();
            current_freq = g_engine_status.FM_CurrentFreq;
            g_radio_config.FMStatus.freq = current_freq;
        }

        //处理系统消息和gui 消息
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {
            if (com_key_mapping(&gui_msg, &gui_event, search_key_map_list) == TRUE)
            {
                //有按键，退出半自动搜台
                radio_break_hardseek();
                if (gui_event == EVENT_RADIO_OPTION)
                {
                    //进入option 菜单
                    ret = RESULT_RADIO_OPTION_PLAYWIN;
                    break;
                }
                else if (gui_event == EVENT_RADIO_RETURN)
                {
                    //退出半自动搜台，回到播放场景
                    ret = RESULT_NULL;
                    break;
                }
                else
                {
                    ret = RESULT_NULL;
                    break;
                }
            }
        }
        else
        {
            //一次循环处理一种类型的消息
            ret = scene_play_sysmsg();
        }

        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL))
        {
            break;
        }
        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    }

    return ret;
}

