/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MUSIC_FAVORITE_H__
#define __APP_MUSIC_FAVORITE_H__

#include "psp_includes.h"
#include "case_include.h"

extern app_result_e add_favorite_listmenu(plist_location_t* location, uint8 target_type, uint16 active);

extern app_result_e del_favorite_listmenu(uint8 target_type, uint16 active);

extern app_result_e clr_favorite_menu(uint8 target_type);

extern app_result_e add_favorite_setmenu(uint8 target_type);

extern app_result_e del_favorite_setmenu(uint8 target_type);

extern app_result_e add_favorlist_all(plist_location_t * plocation, uint8 target_type, uint16 fav_file_total,
        dir_brow_t *browser);

#endif //__APP_MUSIC_FAVORITE_H__
