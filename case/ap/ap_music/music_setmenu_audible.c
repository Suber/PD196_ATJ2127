/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-23          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_menu_config.h"

#include "app_music_bookmark.h"


//pos file buffer
//uint8 pos_name_buffer[256] _BANK_DATA_ATTR_;

//pos file extra name
const uint8 pos_ext[] =
{
    '.', 0x00,
    'P', 0x00,
    'O', 0x00,
    'S', 0x00,
    0x00, 0x00
};

const uint8 pos_ext_name[] =
{ "POS" };
/******************************************************************************/
/*!
 * \par  Description:
 * \void _delete_audible_posfile(uint8* aud_file_name,uint16 length)
 * \删除audible的pos文件
 * \param[in]    aud_file_name  para1
 * \param[in]    aud_file_name  audible文件名;
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 failed
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void _delete_audible_posfile(uint8* aud_file_name, uint16 length)
{
    //path save
    uint8 temp_offset_save[8];

    //file name len
    uint16 name_lenth = length;

    //把MUSIC的后缀名改成.pos
    if ((aud_file_name[0] == 0xff) && (aud_file_name[1] == 0xfe))
    {
        uint8 i;
        name_lenth <<= 1;
        for (i = 0; i < name_lenth; i += 2)
        {
            if ((uint16) aud_file_name[i] == 0x002e)
            {
                break;
            }
        }
        //确保buffer不溢出
        if (i > 246)
        {
            i = 246;
        }
        libc_memcpy(&aud_file_name[i], pos_ext, sizeof(pos_ext));
    }
    else
    {
        libc_memcpy(&aud_file_name[8], pos_ext_name, sizeof(pos_ext_name));
    }

    //save current path
    vfs_file_dir_offset(g_music_mount_id, NULL, temp_offset_save, 0);
    //remove pos file
    vfs_file_dir_remove(g_music_mount_id, aud_file_name, FS_FILE);
    //restore current path
    vfs_file_dir_offset(g_music_mount_id, NULL, temp_offset_save, 1);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_deleaudible_sure(void)
 * \删除当前audible文件
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_delaudible_sure(void *param)
{
    app_result_e result_value;
    del_com_data_t del_com;
    uint16 file_name_len;
    uint8 section_mode;

    //对话框参数
    //获取当前播放文件的路径信息
    music_get_filepath(&g_file_path_info);

    //定位到文件
    fsel_browser_set_file(&(g_file_path_info.file_path.plist_location), g_file_path_info.file_source);

    //取出长名
    file_name_len = vfs_get_name(g_music_mount_id, g_song_filename, 32);

    del_com.del_mode = DEL_MODE_FILE;

    del_com.filename = g_song_filename;

    //删除正在播放的文件
    del_com.del_playing = TRUE;
    del_com.del_self = TRUE;
    del_com.del_func = NULL;
    result_value = gui_delete(&g_file_path_info, &del_com);

    if (result_value == RESULT_DELETE_FILE)
    {
        if (g_file_path_info.file_path.plist_location.file_total == 1)
        {
            //未找到歌曲!
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);
            //只剩下一首歌曲，通知引擎退出
            music_close_engine();
            result_value = RESULT_LISTMENU_MENU;
        }
        else
        {
            section_mode = g_music_config.section_mode;
            g_music_config.section_mode = SWITCH_TITLE;
            music_play_next();            
            music_play();
            g_music_config.section_mode = section_mode;
        }
        //删除其bookmark
        del_bookmark_file(&g_file_path_info.file_path.dirlocation);
        //取出长名
        file_name_len = vfs_get_name(g_music_mount_id, g_temp_buffer_p, 128);
        //删除其pos文件
        _delete_audible_posfile(g_temp_buffer_p, file_name_len);
    }

    if ((result_value == RESULT_DELETE_FILE) || (result_value == RESULT_REDRAW) || (result_value == RESULT_ERROR))
    {
        result_value = RESULT_PLAYING_SCENE;
    }
    return result_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_section_mode(uint8 section_mod)
 * \设置audible的章节模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_set_section_mode(uint8 *section_mod)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //设置音量
    msg.content.addr = section_mod;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_SET_SECTION_MODE_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_switchmode_sure(void)
 * \进入下一级菜单,确定是哪个active
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_switchmode_sure(void *param)
{
    //app_result_e result_value;
    //指向返回参数的指针
    menu_title_action_t* retptr = (menu_title_action_t*) param;
    //根据下一级菜单strid定位当前active项
    retptr->menu_entry = -1;
    //确认字符串id
    if (g_music_config.section_mode == 0)
    {
        retptr->str_id = S_SWITCH_SONG;
    }
    else
    {
        retptr->str_id = S_SWITCH_SECTION;
    }
    retptr->ret_str = NULL;
    retptr->source_id = -1;
    //进入下一级菜单
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \app_result_e set_scene_setsection_sure(void)
 * \设置切换模式为章节模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_setsection_sure(void *param)
{
    g_music_config.section_mode = 0x01;
    //发送消息设置章节模式
    music_set_section_mode(&(g_music_config.section_mode));
    //返回上一级
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_settitle_sure(void)
 * \设置切换模式为标题模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_settitle_sure(void *param)
{
    file_path_info_t *path_locat = &g_file_path_info;
    g_music_config.section_mode = 0x00;

    //发送消息设置标题模式
    music_set_section_mode(&(g_music_config.section_mode));

    g_file.total = path_locat->file_path.dirlocation.file_total;
    g_file.num = path_locat->file_path.dirlocation.file_num;

    //返回上一级
    return RESULT_MENU_PARENT;
}
