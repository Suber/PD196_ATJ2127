/******************************************************************************
 *                               USDK
 *                            Module: app
 *                 Copyright(c) 2002-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File: browser_cfg_menu.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wuyufan    2011-8-22 13:29:40      1.0             build this file
 ******************************************************************************/
 /*!
 * \file     browser_menu_cfg.c
 * \brief    browser菜单配置模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               该模块实现菜单树的配置，包括入口菜单配置和叶子菜单配置
 * \par      EXTERNALIZED FUNCTIONS:
 *               该模块不会被应用调用，工具通过解析该文件生成的目标文件从而生成
 *               菜单数据文件
 * \version 1.0
 * \date    2011-8-22
 *******************************************************************************/
#include "browser.h"
#include "browser_menu_cfg.h"

/* \brief 定义browser入口菜单头 */
const conf_entry_head_t browser_entry_head =
{
    "ENTRY MENU", 5
};

/* \brief 定义browser的入口菜单 */
const conf_menu_entry_t browser_entry_menu[] =
{
    { BROWSER_MENU_ENTRY_NOPLAY, S_NOPLAY, 0 },
    { BROWSER_MENU_ENTRY_PLAYING, S_PLAYING, 0},
    { BROWSER_MENU_ENTRY_LASTPLAY, S_LASTPLAY, 0},
    { BROWSER_MENU_ENTRY_CARD, S_CARDMENU, 0},
    { BROWSER_MENU_ENTRY_DEL_ALL, S_DEL_ALL, 0 }
};
/* \brief 定义browser叶子菜单头 */
const conf_item_head_t item_head =
{
    "MENU ITEM", 6
};

/* \brief 定义browser叶子菜单项 */
const conf_menu_item_t item[] =
{
    { S_LOCAL_FOLDER, 0, menu_func_main_disk, NULL, menu_func_option_option, NORMAL_MENU_ITEM, 0, 0 },
    { S_CARD_FOLDER, 0, menu_func_card_disk, NULL, menu_func_option_option, NORMAL_MENU_ITEM, 1, 0 },
    { S_DEL_ALL, 0, menu_func_del_all, NULL, NULL, NORMAL_MENU_ITEM, 2, 0 },
    { S_NOW_PLAYING, 0, menu_func_playing, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 3, 0},
    { S_LAST_PLAY, 0, menu_func_last_play, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 4, 0 },
    { S_BROWSER_DEL, 0, menu_func_del_file, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 5, 0 }
};

#ifdef PC
menu_cb_func _get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_func;
        }
    }

    return NULL;
}

menu_cb_leaf _get_callback_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].callback;
        }
    }

    return NULL;
}

menu_cb_option _get_menu_option(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_option;
        }
    }

    return NULL;
}
#endif
