/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_menu_config.h"

const uint8 entry_favor_id1[] =
{
    OPTION_FAVOR1_NORMAL,
    OPTION_FAVOR1_MPLAYING,
    OPTION_FAVOR1_MPAUSE,
    OPTION_FAVOR1_RPLAYING,
    OPTION_FAVOR1_RPAUSE
};

const uint8 entry_favor_id2[] =
{
    OPTION_FAVOR2_NORMAL,
    OPTION_FAVOR2_MPLAYING,
    OPTION_FAVOR2_MPAUSE,
    OPTION_FAVOR2_RPLAYING,
    OPTION_FAVOR2_RPAUSE
};

const uint8 entry_favor_id3[] =
{
    OPTION_FAVOR3_NORMAL,
    OPTION_FAVOR3_MPLAYING,
    OPTION_FAVOR3_MPAUSE,
    OPTION_FAVOR3_RPLAYING,
    OPTION_FAVOR3_RPAUSE
};

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_menu_option(void)
 * \第一级菜单项的option函数(audible的列表相同)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menuoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_menu_option(void)
{
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;

    menu_com_data_t menu;
    menu.app_id = APP_ID_MUSIC;

    //引擎是否存在/是music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();

    if (engine_type == ENGINE_MUSIC)
    {
        //正在播放
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入music正在播放入口菜单
            menu.menu_entry = OPTION_MENU_MPLAYING;
        }
        else
        {
            //进入music上一次播放入口菜单
            menu.menu_entry = OPTION_MENU_MPAUSE;
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //判断是否静音
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入radio正在播放入口菜单
            menu.menu_entry = OPTION_MENU_RPLAYING;
        }
        else
        {
            //进入radio上一次播放入口菜单
            menu.menu_entry = OPTION_MENU_RPAUSE;
        }
    }
    else
    {
        //无后台引擎时的提示信息 string id(这里应该使用对话框)
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_OPTION_DESC);
        return RESULT_REDRAW;
    }
    //从跟菜单进入
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_MENU_PLAYLIST;
    menu.menu_func = NULL; //有什么用
    result_value = gui_menulist_simple(MENULIST_OPTION, &menu); //执行option菜单回调
    if (result_value == RESULT_REDRAW)
    {
        result_value = RESULT_MENU_REDRAW;
    }
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor_option(uint8 *entryid_buf)
 * \收藏夹的option函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_menuoption.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_favor_option(const uint8 *entryid_buf)
{
    app_result_e result_value;
    engine_type_e engine_type;
    engine_state_e engine_state;

    menu_com_data_t menu;
    menu.app_id = APP_ID_MUSIC;

    //menu.menu_func = NULL;
    //引擎是否存在/是music or fm
    engine_type = get_engine_type();
    engine_state = get_engine_state();
    
    if (engine_type == ENGINE_MUSIC)
    {
        //正在播放
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入入口菜单1 playnow & play
            menu.menu_entry = entryid_buf[1];

        }
        else
        {
            //进入入口菜单2 lastplayed & play
            menu.menu_entry = entryid_buf[2];
        }
    }
    else if (engine_type == ENGINE_RADIO)
    {
        //bool sound_on;
        //判断是否静音
        if (engine_state == ENGINE_STATE_PLAYING)
        {
            //进入入口菜单3 playnow & play
            menu.menu_entry = entryid_buf[3];
        }
        else
        {
            //进入入口菜单4 lastplayed & play
            menu.menu_entry = entryid_buf[4];
        }
    }
    else
    {
        //进入入口菜单5 play
        menu.menu_entry = entryid_buf[0];
    }
    //从跟菜单进入
    menu.browse_mode = 0;
    menu.path_id = MUSIC_PATH_ID_MENU_FAVORLIST;
    menu.menu_func = NULL; //有什么用
    result_value = gui_menulist_simple(MENULIST_OPTION, &menu);
    //if (result_value == RESULT_REDRAW)
    //{
    //    result_value = RESULT_MENU_REDRAW;
    //}
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor1_option(void)
 * \收藏夹列表1 option回调函数 是对list_scene_favor_option()函数的封装
 * \param[in]
 * \param[out]
 * \return
 * \retval      app_result_e 返回回调函数结果
 * \ingroup     music_listmenu_menuoption
 * \note
 */
/*********************************************************************************/
app_result_e list_scene_favor1_option(void)
{
    return list_scene_favor_option(entry_favor_id1);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor2_option(void)
 * \收藏夹列表2 option回调函数 是对list_scene_favor_option()函数的封装
 * \param[in]
 * \param[out]
 * \return
 * \retval      app_result_e 返回回调函数结果
 * \ingroup     music_listmenu_menuoption
 * \note
 */
/********************************************************************************/
app_result_e list_scene_favor2_option(void)
{
    return list_scene_favor_option(entry_favor_id2);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_favor3_option(void)
 * \收藏夹列表3 option回调函数 是对list_scene_favor_option()函数的封装
 * \param[in]
 * \param[out]
 * \return
 * \retval      app_result_e 返回回调函数结果
 * \ingroup     music_listmenu_menuoption
 * \note
 */
/********************************************************************************/
app_result_e list_scene_favor3_option(void)
{
    return list_scene_favor_option(entry_favor_id3);
}
