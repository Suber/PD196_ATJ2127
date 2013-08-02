/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     Option Menu list
 * \brief    MainMenu的主菜单列表模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "main_menu.h"

const uint16 theme_text_table[] =
{ BLUE_S, GREEN_S, GRAY_S, PINK_S, RED_S };

const uint8 themes_ap2[5][13] =
{
    "mainmenu.sty", "mainmenu.sty",  "mainmenu.sty",  "mainmenu.sty",  "mainmenu.sty"
};
const uint8 themes_com2[5][13] =
{
    "common.sty", "cm_green.sty",  "cm_gray.sty",  "cm_pink.sty",  "cm_red.sty"
};

/******************************************************************************/
/*!
 * \par  Description:
 *	  Option菜单列表选择和模式切换
 * \param[in]    0, 无播放模式，1，正在播放模式，2，上一次播放模式
 * \param[out]   none
 * \return       the result
 * \retval       app_result_e, 菜单控件返回的结构消息
 * \ingroup      module name
 * \par          exmaple code
 * \code
 * \note
 *******************************************************************************/
app_result_e option_menulist(void)
{
    app_result_e result = 0;
    /*初始化菜单控件*/
    menu_com_data_t op_menu;
    op_menu.app_id = APP_ID_MAINMENU;
    /*定义不同模式下的入口菜单*/

    if (get_engine_state() == ENGINE_STATE_PLAYING)
    {
        op_menu.menu_entry = PLAYING_MENU;
    }
    else if (get_engine_state() == ENGINE_STATE_PAUSE)
    {
        op_menu.menu_entry = LASTPLAY_MENU;
    }
    else
    {
        op_menu.menu_entry = NOPLAY_MENU;
    }

    op_menu.path_id = 0;
    op_menu.menu_func = NULL;
    /*处理菜单私有消息*/
    result = gui_menulist(MENULIST_OPTION, &op_menu);
    return result;
}

/*!定义正在播放菜单执行函数*/
app_result_e menu_func_playing(void *param)
{
    msg_apps_t temp_msg;

    g_mainmenu_var.cfg_var.active_item = 0xff; //正在播放
    temp_msg.type = MSG_CREAT_APP;
    if (get_engine_type() == ENGINE_MUSIC)
    {
        temp_msg.content.data[0] = APP_ID_MUSIC;
    }
    else
    {
        temp_msg.content.data[0] = APP_ID_RADIO;
    }
    temp_msg.content.data[1] = PARAM_BACKTO_PLAYING;
    send_async_msg(APP_ID_MANAGER, &temp_msg);

    return RESULT_APP_QUIT;
}

/*!定义上一次播放菜单的执行函数*/
app_result_e menu_func_last_play(void *param)
{
    msg_apps_t temp_msg;

    g_mainmenu_var.cfg_var.active_item = 0xff; //正在播放
    temp_msg.type = MSG_CREAT_APP;
    if (get_engine_type() == ENGINE_MUSIC)
    {
        temp_msg.content.data[0] = APP_ID_MUSIC;
    }
    else
    {
        temp_msg.content.data[0] = APP_ID_RADIO;
    }
    temp_msg.content.data[1] = PARAM_BACKTO_PLAYED;
    send_async_msg(APP_ID_MANAGER, &temp_msg);

    return RESULT_APP_QUIT;
}

/*!定义主题菜单的active项的查询函数*/
app_result_e menu_func_theme(void *title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title;

    menu_title = (menu_title_action_t*) title_action;
    active_id = theme_text_table[g_setting_comval.g_comval.theme];
    menu_title->menu_entry = 0xffff; //不更新入口
    menu_title->str_id = active_id; //指定active项
    menu_title->ret_str = NULL;
    menu_title->source_id = 0xffff;
    return RESULT_MENU_SON; //进入下一级
}

app_result_e menu_func_return(void)
{
    return RESULT_MENU_PARENT; //返回上一级
}

/*!定义主题菜单的蓝色项的执行函数*/
app_result_e menu_func_theme_blue(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 0)
    {
        g_setting_comval.g_comval.theme = 0;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //指定active项
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

/*!定义主题菜单的绿色项的执行函数*/
app_result_e menu_func_theme_green(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 1)
    {
        g_setting_comval.g_comval.theme = 1;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //指定active项
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

/*!定义主题菜单的灰色项的执行函数*/
app_result_e menu_func_theme_gray(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 2)
    {
        g_setting_comval.g_comval.theme = 2;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //指定active项
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

/*!定义主题菜单的粉红色项的执行函数*/
app_result_e menu_func_theme_pink(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 3)
    {
        g_setting_comval.g_comval.theme = 3;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //指定active项
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

/*!定义主题菜单的红色项的执行函数*/
app_result_e menu_func_theme_red(void *param)
{
    menu_title_action_t* menu_title;
    uint16 active_id;

    menu_title = (menu_title_action_t*) param;

    if (g_setting_comval.g_comval.theme != 4)
    {
        g_setting_comval.g_comval.theme = 4;
        active_id = theme_text_table[g_setting_comval.g_comval.theme];
        menu_title->str_id = active_id; //指定active项
        ui_res_close(UI_AP);
        ui_res_open(themes_ap2[g_setting_comval.g_comval.theme], UI_AP);
        ui_res_close(UI_COM);
        ui_res_open(themes_com2[g_setting_comval.g_comval.theme], UI_COM);
        return RESULT_MENU_REDRAW_PARENT;
    }
    return RESULT_MENU_PARENT;
}

