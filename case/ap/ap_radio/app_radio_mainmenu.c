/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-23          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"
#include "app_radio_menu_config.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \RadioUI 主菜单场景
 * \app_result_e radioUI_mainmenu(void)
 * \param[in]    no
 * \param[in]
 * \param[out]   none
 * \return       int the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e radioUI_mainmenu(void)
{
    app_result_e ret_vals;
    menu_com_data_t menu_param;

    //菜单解析器
    menu_param.app_id = APP_ID_RADIO;
    menu_param.menu_entry = MAINMENU_ENTRY;
    menu_param.path_id = 0;
    menu_param.browse_mode = 0;
    menu_param.menu_func = NULL;

    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RADIO);

    /* 调用控件，显示主菜单树*/
    ret_vals = gui_menulist(MENULIST, &menu_param);
    return ret_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_option_callback(void)
 * \Radio 主菜单option 回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_option_callback(void)
{
    show_dialog(S_OPTION_DESC, DIALOG_INFOR_WAIT, 0);
    //重绘全部菜单
    return RESULT_REDRAW;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_saved_preset(void* param)
 * \菜单功能函数-----  确认进入预设电台列表
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e menu_scene_saved_preset(void *param)
{
    app_result_e result;
    uint8 count;
    uint16 *tmp;

    g_stalist_entry = ENTER_NORMAL;
    result = RESULT_RADIO_STATION_LIST;

    if (g_radio_config.band_mode == Band_MAX)
    {
        //如果当前播放为用户模式,  g_radio_config.FMStatus.station_count 记录的是用户电台数
        //先备份指针
        tmp = pfreq;

        if (g_menu_band == Band_Japan)
        {
            pfreq = &g_radio_config.fmstation_jp[0];
        }
        else if (g_menu_band == Band_Europe)
        {
            pfreq = &g_radio_config.fmstation_eu[0];
        }
        else
        {
            pfreq = &g_radio_config.fmstation_us[0];
        }
        //获得当前菜单波段所指示的列表中的电台数
        count = get_station_count();
        //恢复指针，仍指向用户列表首
        pfreq = tmp;
    }
    else
    {
        //非用户模式下，g_radio_config.FMStatus.station_count 记录的为当前波段电台数
        count = g_radio_config.FMStatus.station_count;
    }

    if (count == 0)
    {
        //当前电台表中无电台项时，需显示提示框
        //是否要自动调谐预设
        result = show_dialog(TUNE_PRE, DIALOG_BUTTON_INCLUDE, BUTTON_YESNO_YES);
        if (result == RESULT_DIALOG_YES)
        {
            //选择进行自动调谐
            result = radio_AutoSearch_scene(g_radio_config.seek_mode, DIR_UP);
            if (result == RESULT_ERROR)
            {
                //硬件出错，重绘当前级( radio UI mainmenu)
                return RESULT_REDRAW;
            }
            //自动调谐后，进入预设列表显示
        }
        else if (result == RESULT_REDRAW)
        {
            //不进行自动调谐，显示空电台列表
            result = RESULT_RADIO_STATION_LIST;
        }
        else
        {
        }
    }
    //进入预设电台列表
    //return RESULT_RADIO_STATION_LIST;
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_user_preset(void *param)
 * \菜单功能函数-----  确认进入用户电台列表
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note 用户电台列表不支持小机端添加和删除，只进行显示
 */
/*******************************************************************************/
app_result_e menu_scene_user_preset(void *param)
{
    //进入用户电台列表，此时已经获取用户电台频率值
    return RESULT_RADIO_USER_LIST;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e menu_scene_manual_tune(void *param)
 * \菜单功能函数----  确定进行手动调谐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note  手动调谐选择频点策略
 * \1. 当前有FM 电台播放，回到当前频点( 包括用户电台模式)
 * \2. 当前后台FM 引擎未装载，首先选择当前波段电台列表中
 * \    第一个电台播放
 * \3. 如果当前电台列表为空，选择当前波段的默认频点播放
 * \如上2，3 点，如果之前播放为用户电台，则仍选择之前的
 * \用户电台播放
 */
/*******************************************************************************/
app_result_e menu_scene_manual_tune(void *param)
{
    //连接耳机作为收音机天线
    show_dialog(USE_ANT, DIALOG_INFOR_WAIT, 0);
    return RESULT_RADIO_START_PLAY;
}

