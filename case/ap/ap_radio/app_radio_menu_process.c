/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        mikeyang       2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

#include "app_radio_menu_config.h"

/* 频段显示资源数组*/
const uint16 band_text_table[] =
{ BAND_CU, BAND_JP, BAND_EU };

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_return_list(void *param)
 * \预设电台列表的子菜单中option 回调函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   回到预设电台列表，正常显示
 */
/*******************************************************************************/
app_result_e menu_return_list(void)
{
    /* 返回到预设电台列表，正常显示模式*/
    g_radio_listtype = STATIONLIST_SAVE;
    g_stalist_entry = ENTER_NORMAL;
    return RESULT_RADIO_STATION_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_return_play(void *param)
 * \播放界面的子菜单中option 回调函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   回到播放界面，再按return，则回radio mainmenu
 */
/*******************************************************************************/
app_result_e menu_return_play(void)
{
    g_playwin_mode = ENTER_PLAY_FROM_MAIN;
    return RESULT_RADIO_START_PLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_auto_tune(void* param)
 * \菜单功能函数-----  确定进行自动调谐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_auto_tune(void *param)
{
    app_result_e result;

    //提示:  是否要自动调谐预设频道
    result = show_dialog(TUNE_PRE, DIALOG_BUTTON_INCLUDE, BUTTON_YESNO_YES);
    if (result == RESULT_DIALOG_YES)
    {
        //选择进行自动调谐
        result = radio_AutoSearch_scene(g_radio_config.seek_mode, DIR_UP);
        if (result == RESULT_ERROR)
        {
            //硬件出错，重绘当前界面( 可能是主菜单或播放场景子菜单)
            return RESULT_REDRAW;
        }
    }
    else
    {
        //否，重绘当前界面( 可能是主菜单或播放场景子菜单)
        //return RESULT_REDRAW;
        return result;
    }

    if (result == RESULT_RADIO_STATION_LIST)
    {
        //自动调谐后进入预设电台列表
        g_radio_listtype = STATIONLIST_SAVE;
        /* 正常浏览进入*/
        g_stalist_entry = ENTER_NORMAL;
    }

    //return RESULT_RADIO_STATION_LIST;
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_delone(void* param)
 * \清除单个预设电台
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       bool the result
 * \retval
 * \retval
 * \note   从预设电台列表的option 菜单中选择，返回时需回到
 * \         电台列表的option  菜单下
 */
/*******************************************************************************/
app_result_e menu_scene_delone(void *param)
{
    g_radio_listtype = STATIONLIST_SAVE;
    /* 选择清除预设进入*/
    g_stalist_entry = ENTER_DEL_STATION;
    from_where = 1;
    return RESULT_RADIO_STATION_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_delall(void)
 * \清除所有预设电台
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note  根据dialog 的不同选择，返回不同场景
 */
/*******************************************************************************/
app_result_e menu_scene_delall(void *param)
{
    app_result_e result;

    result = show_dialog(DELALL, DIALOG_BUTTON_INCLUDE, BUTTON_YESNO_NO);

    //选择是
    if (result == RESULT_DIALOG_YES)
    {
        clear_preset_list(g_menu_band);
        //提示－－预设已清除!
        show_dialog(PRESET_DELED, DIALOG_INFOR_WAIT, 0);

        //直接返回播放场景
        result = RESULT_RADIO_START_PLAY;
    }
    //选择否
    else if (result == RESULT_REDRAW)
    {
        g_radio_listtype = STATIONLIST_SAVE;
        g_stalist_entry = ENTER_NORMAL;
        result = RESULT_RADIO_STATION_LIST;
    }
    else
    {
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_start_rec(void *param)
 * \开始FM 录音
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note    直接返回，创建录音进程
 */
/*******************************************************************************/
app_result_e menu_scene_start_rec(void *param)
{
    return RESULT_RECORD_REC;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_saveone(void *param)
 * \将频点保存到预设电台列表
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_saveone(void *param)
{
    g_radio_listtype = STATIONLIST_SAVE;
    /* 保存到预设进入列表*/
    g_stalist_entry = ENTER_ADD_STATION;
    return RESULT_RADIO_STATION_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_delone_2(void *param)
 * \清除单个预设电台
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       bool the result
 * \retval
 * \retval
 * \note   从播放界面的option 菜单中选择，返回时需回到
 * \         播放界面的option  菜单下
 */
/*******************************************************************************/
app_result_e menu_scene_delone_2(void *param)
{
    g_radio_listtype = STATIONLIST_SAVE;
    /* 清除预设进入预设列表*/
    g_stalist_entry = ENTER_DEL_STATION;
    from_where = 2;
    return RESULT_RADIO_STATION_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_voice(void *param)
 * \FM 录制，进入放音
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_voice(void *param)
{
    if (g_option_type == SUBS_OPT_PLAY)
    {
        /* 从option 子菜单进入放音*/
        g_radio_config.enter_voice = FROM_OPTION_MENU;
    }
    else
    {
        /* 从radioUI 的主菜单进入放音*/
        g_radio_config.enter_voice = FROM_RADIO_MAINMENU;
    }
    return RESULT_VOICE_FM_MENU;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_selband(void *title_action)
 * \电台波段active 查询
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_selband(void *title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title;

    menu_title = (menu_title_action_t*) title_action;
    active_id = (uint16) g_menu_band;

    if (active_id >= Band_MAX)
    {
        active_id = Band_China_US;
    }

    menu_title->menu_entry = 0xffff; //不更新入口
    menu_title->str_id = band_text_table[active_id]; //指定active项
    menu_title->ret_str = NULL;
    menu_title->source_id = 0xffff;

    return RESULT_MENU_SON; //进入下一级
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_uband(void *param)
 * \选择电台波段---- 普通频段(  中国/ 美国电台)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note  选择波段后，回到播放场景，并更换到相应波段播放
 */
/*******************************************************************************/
app_result_e menu_scene_uband(void *param)
{
    uint8 real_num;
    g_radio_config.band_mode = Band_China_US;
    g_menu_band = g_radio_config.band_mode;
    /* 更新列表指针*/
    init_point_freq();
    /* 获取普通波段的预存电台总数*/
    g_radio_config.FMStatus.station_count = get_station_count();

    /* 该波段有已保存电台，切换到第一个保存电台*/
    if (g_radio_config.FMStatus.station_count != 0)
    {
        //获得第一个已保存电台的电台号
        real_num = get_real_station_num(1, DIR_UP);
        if ((real_num > MAX_STATION_COUNT) || (real_num == 0))
        {
            //取值超出范围
            g_radio_config.FMStatus.freq = MIN_US;
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.freq = g_radio_config.fmstation_us[real_num - 1];
            g_radio_config.FMStatus.station = real_num;
        }
    }
    /* 该波段无保存电台，切换到最小频点*/
    else
    {
        g_radio_config.FMStatus.freq = MIN_US;
        g_radio_config.FMStatus.station = 0;
    }

    return RESULT_RADIO_START_PLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_jband(void *param)
 * \选择电台波段---- 日本波段
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_jband(void *param)
{
    uint8 real_num;
    g_radio_config.band_mode = Band_Japan;
    g_menu_band = g_radio_config.band_mode;
    /* 更新列表指针*/
    init_point_freq();
    /* 获取日本波段的预存电台总数*/
    g_radio_config.FMStatus.station_count = get_station_count();

    /* 该波段有已保存电台，切换到第一个保存电台*/
    if (g_radio_config.FMStatus.station_count != 0)
    {
        //获得第一个已保存电台的电台号
        real_num = get_real_station_num(1, DIR_UP);
        if ((real_num > MAX_STATION_COUNT) || (real_num == 0))
        {
            //取值超出范围
            g_radio_config.FMStatus.freq = MIN_JP;
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.freq = g_radio_config.fmstation_jp[real_num - 1];
            g_radio_config.FMStatus.station = real_num;
        }
    }
    /* 该波段无保存电台，切换到最小频点*/
    else
    {
        g_radio_config.FMStatus.freq = MIN_JP;
        g_radio_config.FMStatus.station = 0;
    }

    return RESULT_RADIO_START_PLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_eband(void)
 * \选择电台波段--- 欧洲波段
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_eband(void *param)
{
    uint8 real_num;
    g_radio_config.band_mode = Band_Europe;
    g_menu_band = g_radio_config.band_mode;
    /* 更新列表指针*/
    init_point_freq();
    /* 获取欧洲波段的预存电台总数*/
    g_radio_config.FMStatus.station_count = get_station_count();

    /* 该波段有已保存电台，切换到第一个保存电台*/
    if (g_radio_config.FMStatus.station_count != 0)
    {
        //获得第一个已保存电台的电台号
        real_num = get_real_station_num(1, DIR_UP);
        if ((real_num > MAX_STATION_COUNT) || (real_num == 0))
        {
            //取值超出范围
            g_radio_config.FMStatus.freq = MIN_EU;
            g_radio_config.FMStatus.station = 0;
        }
        else
        {
            g_radio_config.FMStatus.freq = g_radio_config.fmstation_eu[real_num - 1];
            g_radio_config.FMStatus.station = real_num;
        }
    }
    /* 该波段无保存电台，切换到最小频点*/
    else
    {
        g_radio_config.FMStatus.freq = MIN_EU;
        g_radio_config.FMStatus.station = 0;
    }

    return RESULT_RADIO_START_PLAY;
}

