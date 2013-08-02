/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-10-26          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_favorite.h"

#include "app_music_menu_config.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_shuffleplay_sure(void* param)
 * \所有歌曲列表下菜单项：全部随机播放
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listmenu.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_shuffleplay_sure(void* param)
{
    dir_brow_t brow;
    uint16 active;
    music_set_playmode(FSEL_MODE_NORMAL | 0x10);//临时shuffle

    fsel_browser_enter_dir(CUR_DIR, 0, &brow);

    if(brow.file_total == 0)
    {
        return RESULT_MAIN_APP;
    }

    active = (uint16)((sys_random() % brow.file_total) + 1);

    //获取当前列表下的文件
    fsel_browser_select(active);

    fsel_browser_get_location(&g_browser_filepath.file_path.plist_location, FSEL_TYPE_PLAYLIST);

    libc_memcpy(&g_file_path_info, &g_browser_filepath, sizeof(file_path_info_t));

    return RESULT_PLAYING_SHUFFLE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e list_scene_artistplay_sure(void)
 * \歌手/风格列表下的菜单项：所有歌曲
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_listmenu_listmenu.c
 * \note
 */
/*******************************************************************************/
app_result_e list_scene_allsongplay_sure(void* param)
{

    //获取当前列表下的文件
    fsel_browser_select(0xffff);
    fsel_browser_get_location(&g_browser_filepath.file_path.plist_location, FSEL_TYPE_PLAYLIST);

    libc_memcpy(&g_file_path_info, &g_browser_filepath, sizeof(file_path_info_t));

    return RESULT_PLAYING_START;
}
