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
 * \file     MainMenu
 * \brief    MainMenu的菜单配置项模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "main_menu.h"

/*define variable, 变量定义*/
/*定义入口菜单*/
const conf_entry_head_t entrymenu =
{ "ENTRY MENU  ", 3 };

/*定义各入口菜单项目*/
const conf_menu_entry_t menu_entry[] =
{
    { NOPLAY_MENU, THEME_S, 0 },
    { PLAYING_MENU, NOWPLAYING_S, 0 },
    { LASTPLAY_MENU, LASTPLAY_S, 0 } 
};

/*定义叶子菜单项*/
const conf_item_head_t item_head =
{ "MENU ITEM   ", 8 };

const conf_menu_item_t mainmenu_item[] =
{
    { NOWPLAYING_S, 0, menu_func_playing, NULL, menu_func_return, NORMAL_MENU_ITEM, 1, 0 },
    { LASTPLAY_S, 0, menu_func_last_play, NULL, menu_func_return, NORMAL_MENU_ITEM, 2, 0 },
    { THEME_S, 0, menu_func_theme, NULL, menu_func_return, NORMAL_MENU_ITEM, 3, 0 },
    { BLUE_S, 0, menu_func_theme_blue, NULL, menu_func_return, RAIDO_MENU_ITEM, 4, 0 },
    { GREEN_S, 0, menu_func_theme_green, NULL, menu_func_return, RAIDO_MENU_ITEM, 5, 0 },
    { GRAY_S, 0, menu_func_theme_gray, NULL, menu_func_return, RAIDO_MENU_ITEM, 6, 0 },
    { PINK_S, 0, menu_func_theme_pink, NULL, menu_func_return, RAIDO_MENU_ITEM, 7, 0 },
    { RED_S, 0, menu_func_theme_red, NULL, menu_func_return, RAIDO_MENU_ITEM, 8, 0 },
};

#ifdef PC
menu_cb_func get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;
    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((mainmenu_item[i].major_id == str_id) && (mainmenu_item[i].minor_id == str_id_sub))
        {
            return mainmenu_item[i].menu_func;
        }
    }
    return NULL;
}

menu_cb_leaf get_callback_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;
    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((mainmenu_item[i].major_id == str_id) && (mainmenu_item[i].minor_id == str_id_sub))
        {
            return mainmenu_item[i].callback;
        }
    }

    return NULL;
}

menu_cb_option get_menu_option(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;
    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((mainmenu_item[i].major_id == str_id) && (mainmenu_item[i].minor_id == str_id_sub))
        {
            return mainmenu_item[i].menu_option;
        }
    }
    return NULL;
}

#endif

