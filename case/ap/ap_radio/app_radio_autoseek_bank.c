/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

extern void radio_auto_proc(void)
__FAR__;

//extern bool radio_get_antenna(void)
//__FAR__;

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_engine_init(void)
 * \引擎初始化
 * \param[in]    void  para1
 * \param[out]   none
 * \return        success/fail
 * \retval   True:   引擎已经安装，模组已经初始化
 * \retval   False:  安装引擎失败或初始化模组失败
 * \note
 */
/*******************************************************************************/
bool radio_engine_init(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;
    bool result = TRUE;
    bool need_init = FALSE;
    Module_Status_e status;

    //获取当前后台引擎类型
    engine_type = get_engine_type();
    msg.type = MSG_KILL_APP_SYNC; //MSG_KILL_APP;   //注销原有非radio 后台
    if (engine_type == ENGINE_MUSIC)
    {
        msg.content.data[0] = APP_ID_MENGINE;
        //发送同步消息
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    //当前未安装引擎，需安装
    if (engine_type != ENGINE_RADIO)
    {

#ifndef PC
        //通过common  接口判断耳机是否连上, 如未连耳机, 不装FM  引擎
        //避免再次插上耳机时打开PA
        //如已经安装引擎, 则可进入播放场景, 但静音
        if (get_antenna_state() == FALSE)
        {
            //提示需插入耳机
            show_dialog(INSERT_EARPHONE, DIALOG_INFOR_WAIT, 0);
            result = FALSE;
            return result;
        }
#endif
        result = radio_open_engine();
        if (result == TRUE)
        {
            //需初始化模组
            need_init = TRUE;
            g_first_open_engine = TRUE;
        }
    }
    //当前已安装引擎，需判断模组是否已经初始化
    else
    {
        g_first_open_engine = FALSE;
        if (need_reinstall == TRUE)
        {
            //从FM  录音返回，由于录音codec 覆盖了fm 驱动代码，重装驱动
#ifndef PC
            //卸载fm 驱动
            sys_drv_uninstall(DRV_GROUP_FM);
            //安装fm  驱动
            sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
#endif
            need_reinstall = FALSE;
            //need_init = TRUE;
            //从录音返回, 需重新初始化, 注意先关之前的声音
            //com_set_sound_out(FALSE, SOUND_OUT_STOP, NULL);
            radio_modual_reinstall();
            need_init = FALSE;
        }
        else
        {
            radio_get_modual_status(&status);
            if (status != MODULE_WORK)
            {
                need_init = TRUE;
            }
        }
    }

    if (need_init == TRUE)
    {
        //初始化模组
        result = radio_modual_init();

        if (!result)
        {
            //模组初始化失败，提示硬件错误
            show_dialog(FMERROR, DIALOG_INFOR_WAIT, 0);
        }

        else
        {
            //初始化后，先mute，刷新界面后，开始播放
            radio_set_mute(SetMUTE);
            first_wait = TRUE;
        }
    }
    else
    {
        first_wait = FALSE;
    }

    return result;
}

/****************************************************************************************/
/*!
 * \par  Description:
 * \void clear_preset_list(radio_band_e band)
 * \清除指定波段的电台列表
 * \param[in]   radio_band_e band
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 1.  如果当前播放为用户模式，则清除指定波段的列表后直接返回，
 仍为用户模式
 2.  否则，清空当前波段列表
 清空电台列表后，当前电台频率为指定波段的默认频率
 */
/******************************************************************************************/
void clear_preset_list(radio_band_e band)
{
    radio_band_e menu_band = band;
    bool user_mode = FALSE;

    if (g_radio_config.band_mode == Band_MAX)
    {
        //当前播放为用户模式
        user_mode = TRUE;
    }

    /* 菜单指示为普通波段*/
    if (menu_band == Band_China_US)
    {
        libc_memset(g_radio_config.fmstation_us, 0, MAX_STATION_COUNT * 2);
        if (!user_mode)
        {
            //非用户模式，更新当前播放状态到默认频点
            g_radio_config.FMStatus.station = 0;
            g_radio_config.FMStatus.station_count = 0;
            g_radio_config.FMStatus.freq = MIN_US;
        }
    }
    /* 菜单指示为日本波段*/
    else if (menu_band == Band_Japan)
    {
        libc_memset(g_radio_config.fmstation_jp, 0, MAX_STATION_COUNT * 2);
        if (!user_mode)
        {
            g_radio_config.FMStatus.station = 0;
            g_radio_config.FMStatus.station_count = 0;
            g_radio_config.FMStatus.freq = MIN_JP;
        }
    }
    /* 菜单指示为欧洲波段*/
    else if (menu_band == Band_Europe)
    {
        libc_memset(g_radio_config.fmstation_eu, 0, MAX_STATION_COUNT * 2);
        if (!user_mode)
        {
            g_radio_config.FMStatus.station = 0;
            g_radio_config.FMStatus.station_count = 0;
            g_radio_config.FMStatus.freq = MIN_EU;
        }
    }
    else
    {
        //菜单波段只能为上述中一种
        //;
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void set_start_freq(FM_SeekDir_e dir)
 * \全自动搜台起始频率设置
 * \param[in]    dir
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void set_start_freq(FM_SeekDir_e dir)
{
    //起始频率设置
    if (g_menu_band == Band_China_US)
    {
        if (dir == DIR_UP)
        {
            g_radio_config.FMStatus.freq = MIN_US; //87.5MHz
        }
        else
        {
            g_radio_config.FMStatus.freq = MAX_US; //108MHz
        }
    }
    else if (g_menu_band == Band_Japan)
    {
        if (dir == DIR_UP)
        {
            g_radio_config.FMStatus.freq = MIN_JP; //76MHz
        }
        else
        {
            g_radio_config.FMStatus.freq = MAX_JP; //90MHz
        }
    }
    else
    {
        if (dir == DIR_UP)
        {
            g_radio_config.FMStatus.freq = MIN_EU; //87.5MHz
        }
        else
        {
            g_radio_config.FMStatus.freq = MAX_EU; //108MHz
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_AutoSearch_init(FM_SeekDir_e dir)
 * \全自动搜台场景参数初始化
 * \param[in]    dir: 搜台方向
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool radio_AutoSearch_init(FM_SeekDir_e dir)
{
    bool bret;

    bret = radio_engine_init();
    if (!bret)
    {
        //初始化失败返回
        return FALSE;
    }

    //连接耳机作为收音机天线
    show_dialog(USE_ANT, DIALOG_INFOR_WAIT, 0);

#ifndef PC
    //增加处理-- 如未连接耳机, 不进行自动搜台
    if (get_antenna_state() == FALSE)
    {
        //提示需插入耳机
        show_dialog(INSERT_EARPHONE, DIALOG_INFOR_WAIT, 0);
        return FALSE;
    }
#endif

    //清空当前菜单波段的电台列表
    clear_preset_list(g_menu_band);

    //根据搜台方向设置起始频率
    set_start_freq(dir);
    //清空缓存电台列表
    libc_memset(&Auto_tab[0], 0, MAX_STATION_COUNT * 2);

    if (timer_auto_seek != -1)
    {
        kill_app_timer(timer_auto_seek);
    }
    //设置定时器，但暂不启动
    timer_auto_seek = set_app_timer(APP_TIMER_ATTRB_UI, 300, radio_auto_proc);
    stop_app_timer(timer_auto_seek);

    //headbar，自动搜台不刷headbar
    gui_set_headbar_mode(HEADBAR_MODE_NULL, HEADBAR_ICON_RADIO);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_AutoSearch_exit(void)
 * \全自动搜台场景退出
 * \param[in]    none
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool radio_AutoSearch_exit(void)
{
    uint16 play_freq;
    //删除定时器
    if (timer_auto_seek != -1)
    {
        kill_app_timer(timer_auto_seek);
    }

    if (g_radio_config.FMStatus.station_count != 0)
    {
        //有搜索到的电台，选择第一个播放
        g_radio_config.FMStatus.station = 1;
        g_radio_config.FMStatus.freq = Auto_tab[0];
    }
    else
    {
        //一个电台也未搜索到，选择默认频点播放
        g_radio_config.FMStatus.station = 0;
        if (g_menu_band == Band_Japan)
        {
            play_freq = MIN_JP;
        }
        else
        {
            play_freq = MIN_US;
        }
        g_radio_config.FMStatus.freq = play_freq;
    }

    //将波段信息返回相应波段
    if (g_menu_band == Band_China_US)
    {
        libc_memcpy(&g_radio_config.fmstation_us[0], &Auto_tab[0], MAX_STATION_COUNT * 2);
    }
    else if (g_menu_band == Band_Japan)
    {
        libc_memcpy(&g_radio_config.fmstation_jp[0], &Auto_tab[0], MAX_STATION_COUNT * 2);
    }
    else
    {
        libc_memcpy(&g_radio_config.fmstation_eu[0], &Auto_tab[0], MAX_STATION_COUNT * 2);
    }

    //设置频点播放
    radio_set_freq(g_radio_config.FMStatus.freq);
    if (radio_get_antenna() == TRUE)
    {
        //有连接耳机, 才打开PA
        radio_set_mute(releaseMUTE);
    }

    //自动搜台后，切换到相应波段，不再是用户模式
    g_radio_config.band_mode = g_menu_band;
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void auto_display_init(void)
 * \全自动搜台场景界面首次刷新
 * \param[in]    none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void auto_display_init(void)
{
    /*pic_box 接口变量*/
    style_infor_t auto_sty;
    picbox_private_t picbox_param;
    progressbar_private_t progressbar_param;
    textbox_private_t textbox_param;

    ui_icon_count = 0;
    auto_sty.type = UI_AP;

    /*显示背景图片*/
    auto_sty.style_id = PICBOX_SEARCH_BG;
    ui_show_picbox(&auto_sty, (void*) NULL);

    /* 显示第一张转动图标*/
    auto_sty.style_id = PICBOX_SEARCH;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = 0;
    //picbox_param.direction = DIRECTION_NORMAL;
    ui_show_picbox(&auto_sty, &picbox_param);

    /* 显示初始进度条*/
    auto_sty.style_id = PBAR_SEARCH;
    progressbar_param.total = 118;
    progressbar_param.value = 0;
    ui_show_progressbar(&auto_sty, &progressbar_param, PROGRESS_DRAW_ALL);

    /* 显示正在搜索提示*/
    auto_sty.style_id = TEXTBOX_SEARCH;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_id = SEARCH1;
    textbox_param.str_value = NULL;
    ui_show_textbox(&auto_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

}

