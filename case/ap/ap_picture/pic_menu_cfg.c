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
 * \file     pic_menu_cfg.c
 * \brief    picture的菜单配置项模块，由菜单配置工具
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "picture.h"
#include  "pic_menu_cfg.h"

/*! \brief 定义入口菜单,以"ENTRY MENU"为关键词查找，必须定义在文件的开始位置*/
const conf_entry_head_t pic_entrymenu =
{
    "ENTRY MENU", 7
};

/*定义各入口菜单项目*/
const conf_menu_entry_t pic_entry[] =
{
    { NOPLAY_NOPREVIEW_MENU, S_NOPLAY_NOPREVIEW, 0 },
    { PLAYING_NOPREVIEW_MENU, S_PLAYING_NOPREVIEW, 0 },
    { LASTPLAY_NOPREVIEW_MENU, S_LASTPLAY_NOPREVIEW, 0 },
    { NOPLAY_PREVIEW_MENU, S_NOPLAY_PREVIEW, 0 },
    { PLAYING_PREVIEW_MENU, S_PLAYING_PREVIEW, 0 },
    { LASTPLAY_PREVIEW_MENU, S_LASTPLAY_PREVIEW, 0 },
    { PIC_CARD_MENU_ENTRY, S_CARD_MENU, 0}
};

/*定义叶子菜单项*/
const conf_item_head_t item_head =
{
    "MENU ITEM", 24
};

const conf_menu_item_t item[] =
{
    { S_NOW_PLAYING, 0, menu_func_playing, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 0, 0},
    { S_LAST_PLAY, 0, menu_func_last_play, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 1, 0 },
    { S_BRIGHT, 0, menu_func_bright, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 2, 0 },
    { S_BACKLIGHT, 0, menu_func_backlight_timer, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 3, 0 },
    { S_BACKLIGHT, 0, NULL, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 3, 0 },
    //{ S_SLIDE_SET, 0, menu_func_slideshow_set, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 4, 0 },
    { S_SLIDE_SET, 0, NULL, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 4, 0 },
    { S_DELETE_PIC, 0, menu_func_delete_pic, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 5, 0 },
    { S_CREATE_PLAYLIST, 0, menu_func_generate_pic_playlist, NULL, menu_func_option_option, RAIDO_MENU_ITEM, 6, 0},
    { S_LOCAL_FOLDER, 0,  menu_func_view_main_disk, NULL, menu_func_option_option, RAIDO_MENU_ITEM, 7, 0},
    { S_CARD_FOLDER, 0,  menu_func_view_card_disk, NULL, menu_func_option_option, RAIDO_MENU_ITEM, 8, 0},
    { S_TIME_10S, 0, menu_func_timer_10s, NULL, menu_func_option_option,RAIDO_MENU_ITEM, 10, 0 },
    { S_TIME_20S, 0, menu_func_timer_20s, NULL, menu_func_option_option,RAIDO_MENU_ITEM, 11, 0 },
    { S_TIME_30S, 0, menu_func_timer_30s, NULL, menu_func_option_option,RAIDO_MENU_ITEM,12, 0 },
    { S_TIME_ALWAYS, 0, menu_func_timer_all_time, NULL, menu_func_option_option,RAIDO_MENU_ITEM, 13, 0 },
    { S_TIME_PER_SLIDE, 0, menu_func_time_per_slide, NULL,menu_func_option_option,NORMAL_MENU_ITEM, 14, 0 },
    { S_REPEAT, 0, menu_func_time_repeat, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 15, 0 },
    { S_ON, S_REPEAT, menu_func_time_repeat_on, NULL, menu_func_option_option,RAIDO_MENU_ITEM, 16, 0 },
    { S_OFF, S_REPEAT, menu_func_time_repeat_off, NULL, menu_func_option_option,RAIDO_MENU_ITEM, 17, 0 },
    { S_SHUFFLE, 0, menu_func_time_shuffle, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 18, 0 },
    { S_ON, S_SHUFFLE, menu_func_time_shuffle_on, NULL, menu_func_option_option,RAIDO_MENU_ITEM, 19, 0 },
    { S_OFF, S_SHUFFLE, menu_func_time_shuffle_off, NULL, menu_func_option_option,RAIDO_MENU_ITEM, 20, 0 },
    { S_PREVIEW, 0, menu_func_preview, NULL, menu_func_option_option,NORMAL_MENU_ITEM, 9, 0 },
    { S_ON, S_PREVIEW, menu_func_preview_on, NULL, menu_func_option_option,RAIDO_MENU_ITEM, 21, 0 },
    { S_OFF, S_PREVIEW, menu_func_preview_off, NULL, menu_func_option_option,RAIDO_MENU_ITEM, 22, 0 }
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
