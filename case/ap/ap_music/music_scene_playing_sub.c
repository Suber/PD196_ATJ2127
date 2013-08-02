/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-14          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_playing.h"

#include "app_music_menu_config.h"

extern void read_history_item(uint8 layer_no, dir_control_t *p_dir_control);
extern void write_history_item(uint8 layer_no, dir_control_t *p_dir_control, bool update);
extern void write_history_item_menu(menulist_control_t *p_ml_control, bool update);
extern void read_history_item_menu(menulist_control_t *p_ml_control);
extern uint16 com_get_menu_active(uint8 menu_id, uint16 key_str);

/******************************************************************************/
/*!
 * \par  Description:
 * \update the dirlist and menulist history layer
 * \param[in]    plist_layer  layer buffer dest_type playlist type
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval
 * \ingroup      music_scene_playing_sub.c
 * \note
 */
/*******************************************************************************/
void music_playlist_update_history(file_path_info_t* path_locat, uint8 dest_type)
{
    uint8 i;
    uint16 active_str;
    dir_control_t list_control;
    menulist_control_t menu_control;
    plist_layer_t plist_layer[4];

    //获取当前playlsit的层级关系
    fsel_make_plistlayer(plist_layer, &(path_locat->file_path.plist_location));

    if (dest_type == PLIST_TITLE)
    {
        plist_layer[0].num++;
    }
    else
    {
        if(dest_type != PLIST_ALBUM)
        {
            plist_layer[1].num++;
        }
    }
    
    list_control.dir_com = NULL;
    list_control.dir_disk = g_file_path_info.file_path.dirlocation.disk;

    for (i = 0; i < 4; i++)
    {
        if (plist_layer[i].num == 0)
        {
            break;
        }

        read_history_item(i, &list_control);

        list_control.list_no = plist_layer[i].num;

        if (list_control.list_no < 6)
        {
            list_control.top = 1;
        }
        else
        {
            list_control.top = list_control.list_no - 5;
        }

        write_history_item(i, &list_control, FALSE);
    }

    write_history_item(0, &list_control, TRUE);

    menu_control.layer_no = 0;

    if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
    {
        menu_control.path_id = LISTMENU_ENTRY_DIR;
    }
    else
    {
        menu_control.path_id = LISTMENU_ENTRY;
    }

    read_history_item_menu(&menu_control);

    switch (dest_type)
    {
        case PLIST_TITLE:
        active_str = S_ALLSONG;
        break;

        case PLIST_ARTIST:
        active_str = S_ARTIST;
        break;

        case PLIST_ALBUM:
        active_str = S_ALBUM;
        break;

        case PLIST_GENRE:
        active_str = S_GENRE;
        break;

        default:
        active_str = S_ALLSONG;        
        break;
    }

    menu_control.list_no = com_get_menu_active(menu_control.path_id, active_str);

    write_history_item_menu(&menu_control, FALSE);
    write_history_item_menu(&menu_control, TRUE);
}
