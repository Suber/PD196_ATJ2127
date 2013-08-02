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
#include "app_radio_menu_config.h"

/* 入口菜单配置项总数*/
#define ENTRY_TOTAL_RADIO  3
/* 菜单项总数*/
#define ITEM_TOTAL_RADIO  16

//定义入口菜单
const conf_entry_head_t radio_entrymenu =
{ "ENTRY MENU   ", ENTRY_TOTAL_RADIO };

//定义各入口菜单项目
const conf_menu_entry_t radioentry[] =
{
    { MAINMENU_ENTRY, PRE_SET1, 0 }, //radio mainmenu
    { OPTION_MENU_STATION, DELONE, LIST_OPTION }, //option menu in station list
    { OPTION_PLAYWIN, STA_FMREC, 0 } //option menu in radio play scene
};

/*定义叶子菜单项*/
const conf_item_head_t item_head =
{ "MENU ITEM    ", ITEM_TOTAL_RADIO };

//叶子菜单配置
const conf_menu_item_t radioleaf[ITEM_TOTAL_RADIO] =
{
    { PRE_SET1, 0, menu_scene_saved_preset, NULL, menu_option_callback, NORMAL_MENU_ITEM, 1, 0 },
    { USER_SET, 0, menu_scene_user_preset, NULL, menu_option_callback, NORMAL_MENU_ITEM, 2, 0 },
    { MAN_TUN, 0, menu_scene_manual_tune, NULL, menu_option_callback, NORMAL_MENU_ITEM, 3, 0 },
    { AUT_TUN, MAIN_MENU, menu_scene_auto_tune, NULL, menu_option_callback, NORMAL_MENU_ITEM, 4, 0 },
    { RECFILE, MAIN_MENU, menu_scene_voice, NULL, menu_option_callback, NORMAL_MENU_ITEM, 5, 0 },

    { DELONE, LIST_OPTION, menu_scene_delone, NULL, menu_return_list, NORMAL_MENU_ITEM, 6, 0 },
    { DELALL, 0, menu_scene_delall, NULL, menu_return_list, NORMAL_MENU_ITEM, 7, 0 },

    { STA_FMREC, 0, menu_scene_start_rec, NULL, menu_return_play, NORMAL_MENU_ITEM, 8, 0 },
    { SAVE_PRESET, 0, menu_scene_saveone, NULL, menu_return_play, NORMAL_MENU_ITEM, 9, 0 },
    { DELONE1, PLAY_OPTION, menu_scene_delone_2, NULL, menu_return_play, NORMAL_MENU_ITEM, 10, 0 },
    { AUT_TUN1, PLAY_OPTION, menu_scene_auto_tune, NULL, menu_return_play, NORMAL_MENU_ITEM, 11, 0 },
    { BAND_SEL, 0, menu_scene_selband, NULL, menu_return_play, NORMAL_MENU_ITEM, 12, 0 },
    { RECFILE1, PLAY_OPTION, menu_scene_voice, NULL, menu_return_play, NORMAL_MENU_ITEM, 13, 0 },

    { BAND_CU, 0, menu_scene_uband, NULL, menu_return_play, RAIDO_MENU_ITEM, 14, 0 },
    { BAND_JP, 0, menu_scene_jband, NULL, menu_return_play, RAIDO_MENU_ITEM, 15, 0 },
    { BAND_EU, 0, menu_scene_eband, NULL, menu_return_play, RAIDO_MENU_ITEM, 16, 0 }, 
};

#ifdef PC
menu_cb_func get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((radioleaf[i].major_id == str_id) && (radioleaf[i].minor_id == str_id_sub))
        {
            return radioleaf[i].menu_func;
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
        if((radioleaf[i].major_id == str_id) && (radioleaf[i].minor_id == str_id_sub))
        {
            return radioleaf[i].callback;
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
        if((radioleaf[i].major_id == str_id) && (radioleaf[i].minor_id == str_id_sub))
        {
            return radioleaf[i].menu_option;
        }
    }

    return NULL;
}
#endif

