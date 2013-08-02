/*******************************************************************************
 *                              US212A
 *                            Module: Video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     Video
 * \brief    Video的菜单配置项模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include  "../video.h"
#include "../video_menu_cfg.h"

/*define variable, 变量定义*/
/*定义入口菜单*/
const conf_entry_head_t entrymenu =
{
    "ENTRY MENU", 4
};

/*定义各入口菜单项目*/
const conf_menu_entry_t video_entry[] =
{
    { VIDEO_NOPLAY_MENU_ENTRY, S_NOPLAY_MENU, 0 },
    { VIDEO_PLAYING_MENU_ENTRY, S_PLAYING_MENU, 0 },
    { VIDEO_LASTPLAY_MENU_ENTRY, S_LASTPLAY_MENU, 0 },
    { VIDEO_CARD_MENU_ENTRY, S_CARD_MENU, 0 }
};

/*定义叶子菜单项*/
const conf_item_head_t item_head =
{
    "MENU ITEM", 11
};

const conf_menu_item_t item[] =
{
    { S_NOW_PLAYING, 0, menu_func_playing, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 0, 0 },
    { S_LAST_PLAY, 0, menu_func_last_play, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 1, 0 },
    { S_BRIGHT, 0, menu_func_bright, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 2, 0 },
    { S_REPEAT_MODE, 0, menu_func_repeat_mode, NULL, menu_func_option_option, NORMAL_MENU_ITEM, 3, 0},
    { S_REPEAT_NORMAL, S_REPEAT_MODE, menu_func_repeat_normal, NULL, menu_func_option_option, RAIDO_MENU_ITEM, 4, 0},
    { S_REPEAT_ONE, S_REPEAT_MODE, menu_func_repeat_one, NULL, menu_func_option_option, RAIDO_MENU_ITEM, 5, 0},    
    { S_REPEAT_ALL, S_REPEAT_MODE, menu_func_repeat_all, NULL, menu_func_option_option, RAIDO_MENU_ITEM, 6, 0},
    { S_DELETE_VIDEO, 0, menu_func_delete_video, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 7, 0 },
    { S_CREATE_PLAYLIST, 0, menu_func_generate_video_playlist, NULL, menu_func_option_option, NORMAL_MENU_ITEM, 8, 0},
    { S_LOCAL_FOLDER, 0,  menu_func_view_main_disk, NULL, menu_func_option_option, NORMAL_MENU_ITEM, 9, 0},
    { S_CARD_FOLDER, 0,  menu_func_view_card_disk, NULL, menu_func_option_option, NORMAL_MENU_ITEM, 10, 0}

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
