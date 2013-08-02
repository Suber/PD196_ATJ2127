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

extern app_result_e deal_play_guimsg(msg_apps_type_e cur_event, radio_playwin_mode_e mode)
__FAR__;
extern bool scene_play_init(void)
__FAR__;
extern bool scene_play_exit(void)
__FAR__;
extern bool get_default_name(void)
__FAR__;
extern bool show_station_info(bool need_show)
__FAR__;

/* 频率数字图片之间的像素增加值*/
const uint8 pos_x_inc[5] =
{ PIC_X_INC1, PIC_X_INC2, PIC_X_INC3, PIC_X_INC4, PIC_X_INC5 };

/* radio 播放场景按键事件映射*/
const key_map_t playing_key_map_list[] =
{
    /*! 短按NEXT 键下一频点事件*/
    {{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_FREQ },
    /*! 短按PREV 键上一频点事件 */
    {{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_PREV_FREQ },
    /*! 长按NEXT 键向后半自动搜台事件*/
    {{ KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEEK_NEXT },
    /*! 长按PREV 键向前半自动搜台事件 */
    {{ KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_RADIO_SEEK_PREV },
    /*! 短按option事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_OPTION },
    /*!短按play 下一电台事件 */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_NEXT_PRESET },
    /*! 短按KEY_VOL 按键转换为 返回事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! 最后一个映射事件 */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  scene_play_sysmsg(void)
 * \播放场景系统消息及私有消息处理
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e scene_play_sysmsg(void)
{
    private_msg_t play_msg;
    app_result_e msg_result = RESULT_NULL;
    if (get_app_msg(&play_msg) == TRUE)
    {
        msg_result = radio_app_msg_callback(&play_msg);
    }
    return msg_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void scroll_station_info(void)
 * \用户电台播放模式下, 电台名称滚屏显示
 * \param[in]    none
 * \param[out]   none
 * \return
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
void scroll_station_info(void)
{
    style_infor_t info_style;
    textbox_private_t info_textbox_param;

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
    ui_show_textbox(&info_style, &info_textbox_param, TEXTBOX_DRAW_SCROLL);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool freq_change(uint8* buf,  uint16 freq, uint8 mode)
 * \将输入的频率值转换为ascill
 * \param[in]    freq--输入频率值; mode--显示资源图片还是字符串
 * \param[out]   buf
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool freq_change(uint8* buf, uint16 freq, uint8 mode)
{
    uint32 true_freq;
    uint16 i, j;
    uint8 k, m;
    uint8 temp_buf[3];

    //先转换成实际频率, khz 为单位
    true_freq = (uint32)((1 << 16) + freq);
    i = (uint16)(true_freq / 100);
    k = (uint8)(true_freq % 100);
    //百分位
    k = k / 10;
    //整数位
    j = i / 10;
    //小数位
    m = (uint8)((i % 10) * 10 + k);

    if (j < 100)
    {
        libc_itoa(j, buf, 2);
    }
    else
    {
        libc_itoa(j, buf, 3);
    }

    if (mode == 0)
    {
        //资源图片显示时
        libc_strncat(buf, ":", 1);
    }
    else
    {
        //字符串显示时
        libc_strncat(buf, ".", 1);
    }
    libc_itoa(m, temp_buf, 2);
    temp_buf[2] = '\0';
    libc_strncat(buf, temp_buf, libc_strlen(temp_buf));
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void ui_show_FMdata(char *buf)
 * \播放场景下的频率图片刷新
 * \param[in]    buf -- 已转换为ascill 码的频率字串
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
//void ui_show_FMdata(char *buf)
//{
//    uint8 FMFreNum = (uint8) libc_strlen(buf); //不包含结束符
//    style_infor_t style_infor;
//    uint16 x, y;
//    uint8 *p_xinc;
//
//    style_infor.type = UI_AP;
//    if (FMFreNum != FMFreNumOld)
//    {
//        //位数发生变化，先清背景
//        FMFreNumOld = FMFreNum;
//        style_infor.style_id = PICBOX_FREQ_BG;
//        ui_show_picbox(&style_infor, (void*) NULL);
//    }
//    if (FMFreNum < 6)
//    {
//        //<100MHz
//        x = PIC_X_F1START;
//        p_xinc = (uint8 *) &pos_x_inc[1];
//    }
//    else
//    {
//        //>=100MHz
//        x = PIC_X_F1START_1;
//        p_xinc = (uint8 *) &pos_x_inc[0];
//    }
//    y = PIC_Y_F1START;
//    while (1)
//    {
//        if (*buf != 0)
//        {
//            ui_show_pic(FMNUM0 + *buf - 0x30, x, y);
//        }
//        else
//        {
//            break;
//        }
//        buf++;
//        x += (*p_xinc);
//        p_xinc++;
//    }
//
//    //显示FM 图片
//    x += 14;
//    ui_show_pic(FM_FM, x, 49);
//    return;
//}

/******************************************************************************/
/*!
 * \par  Description:
 * \void ui_show_freqbar(uint16 freq)
 * \根据当前频率及波段，显示播放场景下的tune bar
 * \param[in]    freq
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void ui_show_freqbar(uint16 freq, uint8 mode)
{
    uint32 true_freq;
    style_infor_t style_infor;
    progressbar_private_t freq_progress;

    //完整的频率值
    true_freq = (uint32)((1 << 16) + freq);

    //先清背景
    style_infor.type = UI_AP;
    style_infor.style_id = PBAR_BAND;

    //76M~~90M
    if (g_radio_config.band_mode == Band_Japan)
    {
        if (true_freq < 76000)
        {
            true_freq = 0;
        }
        else
        {
            true_freq -= 76000;
            true_freq /= 100;
        }
        freq_progress.value = (uint16)true_freq;
        freq_progress.total = 140;
    }
    //其他波段范围87.5M~~108M
    else
    {
        if (true_freq < 87500)
        {
            true_freq = 0;
        }
        else
        {
            true_freq -= 87500;
            true_freq /= 100;
        }
        freq_progress.value = (uint16)true_freq;
        freq_progress.total = 205;
    }

    ui_show_progressbar(&style_infor, &freq_progress, mode);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void show_freq_in_play(void)
 * \播放场景下的频率图片刷新
 * \param[in]     none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void show_freq_in_play(void)
{
    style_infor_t numbox_style;
    numbox_private_t numbox_param;
    uint32 true_freq = (uint32)((1 << 16) + g_radio_config.FMStatus.freq);

    //当前频率值转换为ascill 码存放
    //libc_memset(Freqtab, 0, sizeof(Freqtab));
    //freq_change(Freqtab, g_radio_config.FMStatus.freq, 0);
    //ui_show_FMdata(Freqtab);

    numbox_style.style_id = NUMBOX_FREQ;
    numbox_style.type = UI_AP;
    numbox_param.value = (int32)true_freq / 10;//显示以10KHz为单位
    numbox_param.total = 0;

    ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_NUMBER);

    ui_show_freqbar(g_radio_config.FMStatus.freq, PROGRESS_DRAW_PROGRESS);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void show_station_num(void)
 * \播放场景下的电台号刷新
 * \param[in]     none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note    调用此函数时，需确保g_radio_config.FMStatus.station
 * \          g_userlist_index ，g_userlist_total 等全局变量正确
 */
/*******************************************************************************/
void show_station_num(void)
{
    style_infor_t style_infor;
    textbox_private_t station_textbox_param;
    uint8 buf[3]; //电台号转换为ascill 存放buffer
    bool is_saved; //是否是已保存电台

    style_infor.type = UI_AP;

    if (g_radio_config.FMStatus.station != 0)
    {
        if (g_radio_config.band_mode != Band_MAX)
        {
            //预设电台模式
            is_saved = TRUE;
        }
        else if (g_userlist_index < g_userlist_total)
        {
            //用户模式, 需要索引号小于电台总数
            is_saved = TRUE;
        }
        else
        {
            is_saved = FALSE;
        }
    }
    else
    {
        is_saved = FALSE;
    }

    if (is_saved == TRUE)
    {
        //是已保存电台
        style_infor.style_id = TEXTBOX_PRESET_NUM;
        libc_itoa(g_radio_config.FMStatus.station, &buf[0], 2);
        buf[2] = '\0';
        station_textbox_param.str_value = &buf[0];
    }
    else
    {
        //未保存电台
        style_infor.style_id = TEXTBOX_NO_PRESET;
        station_textbox_param.str_value = "--";
        //当前播放的是未保存电台
        g_radio_config.FMStatus.station = 0;
    }
    station_textbox_param.lang_id = ANSIDATAAUTO;
    station_textbox_param.str_id = -1;
    ui_show_textbox(&style_infor, &station_textbox_param, TEXTBOX_DRAW_NORMAL);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool get_station_info(void)
 * \获取用户电台名称信息
 * \param[in]    none
 * \param[out]   none
 * \return       1: 存在用户电台名称   0: 不存在用户电台名称
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
bool get_station_info(void)
{
    bool ret;
    if (g_use_default == TRUE)
    {
        //使用默认的用户电台, 获取默认名称
        ret = get_default_name();
    }
    else
    {
        /* 如果在用户模式下，播放频率不是已保存用户电台
         则g_userlist_index =0xff,   g_radio_config.FMStatus.station = 0  */
        if (g_userlist_index != 0xff)
        {
            //根据索引取到电台名称
            radio_parse_userlist(g_user_station.station_name, INDEX_FROM_START, g_userlist_index);
            ret = TRUE;
        }
        else
        {
            ret = FALSE;
        }
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radioUI_scene_playing(radio_playwin_mode_e mode)
 * \进入radio  播放场景界面
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e radioUI_scene_playing(radio_playwin_mode_e mode)
{
    bool result;
    app_result_e ret = RESULT_IGNORE;
    style_infor_t style_infor;
    bool need_fetch = TRUE; //需要取用户电台名称
    bool have_info = FALSE; //是否有用户电台名称显示

    //获取gui消息
    input_gui_msg_t gui_msg;
    //gui 消息对应gui 事件
    msg_apps_type_e gui_event;

    result = scene_play_init();
    if (!result)
    {
        if (timer_flush_radioinfo != -1)
        {
            kill_app_timer(timer_flush_radioinfo);
            timer_flush_radioinfo = -1;
        }
        //设置为非播放状态
        change_app_state(APP_STATE_NO_PLAY);
        //return RESULT_SCENE_EXIT;
        return RESULT_RADIO_MAINMENU;
    }

    style_infor.type = UI_AP;
    paint_flag |= (uint8) PAINT_ALL;

    //设置当前频率
    if ((g_playwin_mode != ENTER_PLAY_FROM_NOWPLAY) && (need_set_freq))
    {
        radio_get_freq();

        //当前获取的频点和要设置的频点不一致或者第一次开启radio引擎时，都需要强制设置频点
        if ((g_radio_config.FMStatus.freq != g_engine_status.FM_CurrentFreq) || (g_first_open_engine == TRUE))
        {
            //选择正在播放进入, 不重设频点, 避免出现一下短暂杂音
            radio_set_freq(g_radio_config.FMStatus.freq);
        }
    }

    need_set_freq = TRUE;
    //radio 播放场景循环
    while (1)
    {

        if (get_screensave_state() == FALSE)//不在屏保模式才会显示
        {
            //刷新全部
            if ((paint_flag & PAINT_ALL) == PAINT_ALL)
            {
                style_infor_t numbox_style;
                numbox_private_t numbox_param;
                uint32 true_freq = (uint32)((1 << 16) + g_radio_config.FMStatus.freq);

                //背景
                style_infor.style_id = PICBOX_PLAY_BG;
                ui_show_picbox(&style_infor, (void*) NULL);

                //headbar
                gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RADIO);

                numbox_style.style_id = NUMBOX_FREQ;
                numbox_style.type = UI_AP;
                numbox_param.value = (int32)true_freq / 10;//显示以10KHz为单位
                numbox_param.total = 0;
                ui_show_numbox(&numbox_style, &numbox_param, NUMBOX_DRAW_ALL);

                //FM字段
                ui_show_pic(FM_FM, 86, 49);

                //竖状频率条
                ui_show_freqbar(g_radio_config.FMStatus.freq, PROGRESS_DRAW_ALL);
                paint_flag &= CLEAR_PAINT_ALL;
            }

            //刷新频率相关项
            if ((paint_flag & PAINT_FREQ) != 0)
            {
                show_freq_in_play();
                show_station_num();
                paint_flag &= (uint8) CLEAR_PAINT_FREQ;
            }

            //如果未静音，先播放，稍后再刷用户电台名称
            if ((first_wait == TRUE) && (radio_get_antenna() == TRUE))
            {
                radio_set_mute(releaseMUTE);
                first_wait = FALSE;
            }

            //刷新用户电台列表的名称项
            if ((g_radio_config.band_mode == Band_MAX) && ((paint_flag & PAINT_STATION_INFOR) != 0))
            {

                //用户模式下，切换频率后需重新取电台名称
                if (need_fetch == TRUE)
                {
                    have_info = get_station_info();
                    need_fetch = FALSE;
                }
                show_station_info(have_info);
                paint_flag &= (uint8) CLEAR_PAINT_INFOR;
            }
        }

        //获取和处理GUI消息、私有消息
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {

            if (com_key_mapping(&gui_msg, &gui_event, playing_key_map_list) == TRUE)
            {
                //if(gui_event != EVENT_RADIO_PLAY_MUTE)
                {
                    //除播放暂停事件外，其余需重新取电台名称
                    need_fetch = TRUE;
                }
                ret = deal_play_guimsg(gui_event, mode);
            }
        }
        else
        {
            //一次循环处理一种类型的消息
            ret = scene_play_sysmsg();
        }
        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL) && (ret != RESULT_REDRAW))
        {
            break;
        }
        else if (ret == RESULT_REDRAW)
        {
            //重刷全部
            paint_flag |= (uint8) PAINT_ALL;
        }
        else
        {
        }

        //挂起20ms 多任务交互
        sys_os_time_dly(2);
    }
    //播放场景退出
    scene_play_exit();

    return ret;
}

