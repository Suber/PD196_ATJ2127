/*******************************************************************************
 *                              US212A
 *                            Module: video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-15 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     video
 * \brief    video 的主菜单列表模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include  "video.h"
#include "video_menu_cfg.h"
static const uint8 menu_entry[] =
{
    VIDEO_NOPLAY_MENU_ENTRY,
    VIDEO_PLAYING_MENU_ENTRY,
    VIDEO_LASTPLAY_MENU_ENTRY,
    VIDEO_CARD_MENU_ENTRY
};

/******************************************************************************/
/*!
 * \par  Description:
 *    正在播放菜单项执行函数
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result  菜单控件返回的结构消息
 * \retval       RESULT_NOWPLAYING  该返回值会返回到场景调度，并转换成消息
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_playing(void *param)
{
    return RESULT_NOWPLAYING;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    上一次播放菜单项执行函数
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result  菜单控件返回的结构消息
 * \retval       RESULT_LASTPLAY  该返回值会返回到场景调度，并转换成消息
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_last_play(void *param)
{
    return RESULT_LASTPLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置对比度
 * \param[in]    param 对比度值
 * \param[out]   none
 * \return       the result
 * \retval       TRUE
 * \ingroup      video_optionmenu
 * \note
 * \li  内部函数，作为参数调节控件的回调函数
 *******************************************************************************/
static bool _set_lightness_callback(int16 param)
{
    g_comval.lightness = (uint8) param;
    //设置到硬件
    com_set_contrast(g_comval.lightness);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置对比度菜单项回调函数
 * \param[in]    param  无实际意义
 * \param[out]   none
 * \return       the result  参数调节控件返回值
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_bright(void *param)
{
    //调节亮度
    uint16 style_id;
    app_result_e result;
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;

    s_param_item.min = 0;
    s_param_item.max = 11;
    s_param_item.step = 1;
    s_param_item.value = g_comval.lightness;
    s_param_item.cycle = FALSE;
    s_param_item.adjust_func = NULL;
    s_param_item.callback = _set_lightness_callback;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;

    if (g_video_scene_prev == SCENE_PLAY)
    {
        style_id = SLIDER_SIMPLE_V;
    }
    else
    {
        style_id = SLIDER_SIMPLE;
    }

    result = gui_set_parameter(style_id, &s_param_com);
    if (result == RESULT_CONFIRM)
    {
        g_comval.lightness = (uint8) s_param_com.private_data->items->value;
        result = RESULT_REDRAW;
    }
    return result; //返回上一级
}

/******************************************************************************/
/*!
 * \par  Description:
 *    删除图片菜单项执行函数
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result  ui_delete控件返回值
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_delete_video(void *param)
{
    bool ret_val;
    app_result_e result;
    del_com_data_t del_com;

    if (g_video_scene_prev == SCENE_PLAY)
    {
        //已知其location
        del_com.del_no = 0;
    }
    else
    {
        //列表模式,主动获取location
        del_com.del_no = dir_control.list_no;
        fsel_browser_select(dir_control.list_no);
        fsel_browser_get_location(&g_video_var.path.file_path, g_video_var.path.file_source);
        fsel_browser_set_file(&g_video_var.path.file_path, g_video_var.path.file_source);
    }

    vfs_get_name(video_mount, g_video_filename, LIST_ITEM_BUFFER_LEN / 2);
    if ((g_video_filename[0] != 0xff) && (g_video_filename[0] != 0xfe))
    {
        com_dot_to_shortname(g_video_filename);
    }

    del_com.del_mode = DEL_MODE_FILE;
    del_com.filename = g_video_filename;
    del_com.del_playing = FALSE;
    del_com.del_self = TRUE;
    del_com.del_func = NULL;
    result = gui_delete(&g_video_var.path, &del_com);
    if ((result == RESULT_DELETE_FILE) || (result == RESULT_DELETE_DIRLIST))
    {
        //退出当前option菜单场景
        if(g_video_var.path.file_path.dirlocation.file_total == 0)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_VIDEO_FILE);
            result = RESULT_MAIN_APP;
        }
        else
        {
            g_del_file = TRUE;

            //主动更新location
            if (g_video_var.path.file_path.dirlocation.file_num > g_video_var.path.file_path.dirlocation.file_total)
            {
                g_video_var.path.file_path.dirlocation.file_num = 1;
            }
            //获取下一曲
            ret_val = fsel_get_byno(g_video_var.path.file_path.dirlocation.filename,
                    g_video_var.path.file_path.dirlocation.file_num);

            if (ret_val == TRUE)
            {
                fsel_get_location(&g_video_var.path.file_path.dirlocation, g_video_var.path.file_source);
            }
            result = RESULT_MENU_EXIT;
        }

    }
    return result;

}

/******************************************************************************/
/*!
 * \par  Description:
 *    创建播放列表菜单项执行函数
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result
 * \retval       RESULT_CREATE_PLAYLIST 由main模块将返回值转换成消息
 * \ingroup      video_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_generate_video_playlist(void *param)
{
    return RESULT_CREATE_LIST_APP;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    设置循环模式根菜单执行函数
 * \param[in]    param 解释为入口参数，根据当前的循环模式选择叶子菜单选择值
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 * \li  根据循环模式值匹配叶子菜单active值
 *******************************************************************************/
app_result_e menu_func_repeat_mode(void *param)
{
    menu_title_action_t* retptr = (menu_title_action_t*) param;
    retptr->menu_entry = -1;

    switch (g_video_var.repeat_mode)
    {
        //normal
        case VIDEO_REPEAT_NORMAL:
        retptr->str_id = S_REPEAT_NORMAL;
        break;

        //repeate one
        case VIDEO_REPEAT_ONE:
        retptr->str_id = S_REPEAT_ONE;
        break;

        //repeate all
        case VIDEO_REPEAT_ALL:
        retptr->str_id = S_REPEAT_ALL;
        break;

        default:
        retptr->str_id = -1;
        break;
    }
    retptr->ret_str = NULL;
    retptr->source_id = -1;
    return RESULT_MENU_SON;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    设置video循环模式
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e _video_set_repeat_mode(uint8 mode)
{
    g_video_var.repeat_mode = mode;

    //设置播放模式
    fsel_set_mode(mode);

    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    普通模式叶子菜单项执行函数
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e menu_func_repeat_normal(void *param)
{
    return _video_set_repeat_mode(VIDEO_REPEAT_NORMAL);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    单曲模式叶子菜单项执行函数
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e menu_func_repeat_one(void *param)
{
    return _video_set_repeat_mode(VIDEO_REPEAT_ONE);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    全部循环模式叶子菜单项执行函数
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result
 * \ingroup      video
 * \note
 *******************************************************************************/
app_result_e menu_func_repeat_all(void *param)
{
    //设置播放模式
    return _video_set_repeat_mode(VIDEO_REPEAT_ALL);
}

app_result_e menu_func_option_option(void)
{
    //返回上一级
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Option菜单列表入口菜单
 * \param[in]    scene_prev  记录进入option菜单场景之前的菜单场景
 * \param[out]   none
 * \return       the result  菜单控件返回的结构消息
 * \ingroup      video_optionmenu
 * \note         play场景进入option菜单和filelist场景进入option菜单
 *               菜单项相同，风格不同。同时要区分有卡和无卡，正在播放
 *               /上一次播放/无后台播放的动态菜单项
 *******************************************************************************/
app_result_e video_option_menulist(void)
{
    uint8 i;
    uint16 video_menu_style_id;
    menu_com_data_t op_menu;
    engine_state_e engine_status;
    app_result_e result = RESULT_NULL;

    ui_clear_screen(NULL);

    engine_status = get_engine_state();

    if (engine_status == ENGINE_STATE_NULL)
    {
        i = 0;
    }
    else if (engine_status == ENGINE_STATE_PLAYING)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }

    if (g_video_scene_prev == SCENE_PLAY)
    {
        //播放界面下菜单为横屏风格
        com_set_gui_direction(1);
        video_menu_style_id = MENULIST_OPTION_V;
    }
    else
    {
        video_menu_style_id = MENULIST_OPTION;
    }

    /*初始化菜单控件*/
    op_menu.app_id = APP_ID_VIDEO;

    /*定义不同模式下的入口菜单*/
    op_menu.menu_entry = menu_entry[i];
    op_menu.path_id = 0;
    /* 首层菜单回调函数 */
    op_menu.menu_func = NULL;

    /*处理菜单私有消息*/
    result = gui_menulist(video_menu_style_id, &op_menu);

    if (g_video_scene_prev == SCENE_PLAY)
    {
        com_set_gui_direction(0);
    }

    if (result == RESULT_REDRAW)
    {
        if (g_video_scene_prev == SCENE_PLAY)
        {
            result = RESULT_MOVIE_PLAY;
        }
        else
        {
            result = RESULT_VIDEO_FILELIST;
        }
    }
    return result;
}
