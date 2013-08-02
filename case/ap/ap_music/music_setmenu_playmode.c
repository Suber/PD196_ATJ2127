/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-19          1.0              create this file
 *******************************************************************************/
#include "app_music.h"

#include "app_music_menu_config.h"

//{ "MUSIC.LIB" }long name

const char mplist_name[] =
{
    0xff, 0xfe,//unicode flag
    0x4d, 0x00,//M
    0x55, 0x00,//U
    0x53, 0x00,//S
    0x49, 0x00,//I
    0x43, 0x00,//C
    0x2e, 0x00,//.
    0x4c, 0x00,//L
    0x49, 0x00,//I
    0x42, 0x00,//B
    0x00, 0x00 //end flag
};

/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_plist_exit(void)
 * \检测列表是否存在
 * \param[in]    none
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
bool check_plist_exit(void)
{
    uint32 plist_fp;
    bool check_ret;
    vfs_cd(g_music_mount_id, CD_ROOT, 0);
    //打开文件
    plist_fp = vfs_file_open(g_music_mount_id, mplist_name, R_FAST_SEEK);
    if (0 == plist_fp)
    {
        check_ret = FALSE;
    }
    else
    {
        vfs_file_close(g_music_mount_id, plist_fp);
        check_ret = TRUE;
    }
    return check_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_change_filepath(file_path_info_t *filepath)
 * \更改播放文件的路径信息
 * \param[in]    filepath存放文件路径buffer的指针  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
bool music_change_filepath(file_path_info_t *filepath)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //更改播放文件的路径信息
    msg.content.addr = filepath;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_CHANGE_FILEPATH_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void set_scene_switchpath(plist_type_e dest_type)
 * \切换当前播放的文件路径
 * \param[in]    dest_type  para1
 * \             typedef enum
 * \             {
 * \             	 Plist_AllSong //music title
 * \             	 Plist_Artist  //music artist
 * \             	 Plist_Album   //music album
 * \             	 Plist_Genre   //music genre
 * \               Plist_Books //audible books
 * \               Plist_Author //audible author
 * \             }plist_type_e
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
void set_scene_switchpath(plist_type_e dest_type)
{
    plist_location_t result_ptr;
    void* cur_locat;
    fsel_type_e cur_type = g_file_path_info.file_source;
    if (check_plist_exit() == TRUE)
    {
        //获取文件路径
        music_get_filepath(&g_file_path_info);
        cur_locat = &g_file_path_info.file_path.dirlocation;
        //更换当前文件的路径
        fsel_change_filepath(cur_locat, &result_ptr, g_file_path_info.file_source, dest_type);
        //修改文件选择类型
        g_file_path_info.file_source = FSEL_TYPE_PLAYLIST;
        //设置当前路径到引擎
        libc_memcpy(&(g_file_path_info.file_path.plist_location), &result_ptr, sizeof(plist_location_t));
        //更改文件路径,如果暂停,则重新开始播放
        music_change_filepath(&g_file_path_info);
        if (cur_type != g_file_path_info.file_source)
        {
            fsel_exit();
            music_file_init(&g_file_path_info);
            libc_memcpy(&g_browser_filepath, &g_file_path_info, sizeof(file_path_info_t));
        }
        //修改音乐来源变量
        g_setting_comval.music_comval.music_from = dest_type;

        //更改进入模式为DESKTOP
        g_enter_mode = PARAM_FROM_MAINMENU;

        g_music_config.browser_sel = FALSE;

        g_file.total = g_file_path_info.file_path.dirlocation.file_total;

        g_file.num = g_file_path_info.file_path.dirlocation.file_num;

        g_music_config.albumlist_sel = FALSE;

    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_fromallsong_sure(void *param)
 * \更改当前播放文件的路径为标题列表
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_PLAYING_SCENE 回到播放场景
 * \retval       RESULT_IGNORE 忽略
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fromallsong_sure(void *param)
{
    //切换路径
    set_scene_switchpath(PLIST_TITLE);
    //返回到播放场景
    return RESULT_PLAYING_SCENE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_fromartist_sure(void *param)
 * \更改当前播放文件的路径为歌手列表
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_PLAYING_SCENE 回到播放场景
 * \retval       RESULT_IGNORE 忽略
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fromartist_sure(void *param)
{
    //切换路径
    set_scene_switchpath(PLIST_ARTIST);
    //返回到播放场景
    return RESULT_PLAYING_SCENE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_fromalbum_sure(void *param)
 * \更改当前播放文件的路径为专辑列表
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_PLAYING_SCENE 回到播放场景
 * \retval       RESULT_IGNORE 忽略
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fromalbum_sure(void *param)
{
    //切换路径
    set_scene_switchpath(PLIST_ALBUM);
    //返回到播放场景
    return RESULT_PLAYING_SCENE;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_fromgenre_sure(void *param)
 * \更改当前播放文件的路径为流派列表
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_PLAYING_SCENE 回到播放场景
 * \retval       RESULT_IGNORE 忽略
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fromgenre_sure(void *param)
{
    set_scene_switchpath(PLIST_GENRE);
    //返回到播放场景
    return RESULT_PLAYING_SCENE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_repeat_sure(void *param)
 * \选择播放模式 定位active项
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE 忽略
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_repeat_sure(void *param)
{
    //指向返回参数的指针
    menu_title_action_t* retptr = (menu_title_action_t*) param;
    //根据下一级菜单strid定位当前active项
    retptr->menu_entry = -1;
    //确认字符串id
    switch (g_setting_comval.music_comval.music_repeat_mode)
    {
        //normal
        case VALUE_MUSIC_REPEAT_OFF:
        retptr->str_id = S_REPEAT_OFF;
        break;
        //repeate one
        case VALUE_MUSIC_REPEAT_ONE:
        retptr->str_id = S_REPEAT_ONE;
        break;
        //repeate all
        case VALUE_MUSIC_REPEAT_ALL:
        retptr->str_id = S_REPEAT_ALL;
        break;
        case VALUE_MUSIC_REPEAT_INTRO:
        retptr->str_id = S_REPEAT_INTRO;
        break;
        default:
        retptr->str_id = -1;
        break;
    }
    retptr->ret_str = NULL;
    retptr->source_id = -1;
    //进入下一级菜单
    return RESULT_MENU_SON;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void set_scene_switchmode(fsel_mode_e mode)
 * \修改当前播放文件的循环模式
 * \param[in]    mode  para1
 * \             typedef enum
 * \             {
 * \             	LoopNor  //普通循环
 * \             	LoopOne  //循环放当前首
 * \             	LoopOneDir  //循环放当前目录
 * \             	LoopDir  //顺序播放目录
 * \             	LoopAll  //循环所有
 * \              Intro  //浏览播放
 * \              MaxRepeatMod
 * \             }repeat_mode_e;
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
void set_scene_switchmode(fsel_mode_e mode)
{
    uint8 cur_mode = g_setting_comval.music_comval.music_shuffle;

    //获取引擎的循环模式信息
    //music_get_playmode(&cur_mode);

    //设置播放模式
    //cur_mode &= (uint8) 0xf0;
    cur_mode |= (uint8) mode;

    g_setting_comval.music_comval.music_repeat_mode = mode;

    music_set_playmode(cur_mode);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_repeatoff_sure(void *param)
 * \选择重复播放关
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE 忽略
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_repeatoff_sure(void *param)
{
    //设置播放模式
    set_scene_switchmode(FSEL_MODE_NORMAL);
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_repeatone_sure(void *param)
 * \选择单曲循环
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE 忽略
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_repeatone_sure(void *param)
{
    //设置播放模式
    set_scene_switchmode(FSEL_MODE_LOOPONE);
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_repeatall_sure(void *param)
 * \选择全部循环
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE 忽略
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_repeatall_sure(void *param)
{
    //设置播放模式
    set_scene_switchmode(FSEL_MODE_LOOPALL);
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_repeatintro_sure(void *param)
 * \选择浏览播放
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE 忽略
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_repeatintro_sure(void *param)
{
    //设置播放模式
    set_scene_switchmode(FSEL_MODE_INTRO);
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_shuffle_sure(void *param)
 * \选择shuffle开关 定位active项
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE 忽略
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_shuffle_sure(void* param)
{
    //指向返回参数的指针
    menu_title_action_t* retptr = (menu_title_action_t*) param;
    //根据下一级菜单strid定位当前active项
    retptr->menu_entry = -1;
    //确认字符串id
    switch (g_setting_comval.music_comval.music_shuffle)
    {
        //shuffle off
        case VALUE_MUSIC_SHUFFLE_OFF:
        retptr->str_id = S_OFF;
        break;
        //shuffle on
        case VALUE_MUSIC_SHUFFLE_ON:
        retptr->str_id = S_ON;
        break;
        default:
        retptr->str_id = -1;
        break;
    }
    retptr->ret_str = NULL;
    retptr->source_id = -1;
    //进入下一级菜单
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_shuffleoff_sure(void *param)
 * \设置shuffle关
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE 忽略
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_shuffleoff_sure(void *param)
{
    uint8 cur_mode = g_setting_comval.music_comval.music_repeat_mode;

    //获取引擎的播放模式信息
    //music_get_playmode(&cur_mode);

    //设置shuffle模式
    //cur_mode &= 0x0f;

    g_setting_comval.music_comval.music_shuffle = 0;

    g_shuffle_tmporary = FALSE;

    music_set_playmode(cur_mode);
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_scene_shuffleon_sure(void *param)
 * \设置shuffle开
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_IGNORE 忽略
 * \retval
 * \ingroup      music_setmenu_playmode.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_shuffleon_sure(void *param)
{
    uint8 cur_mode = g_setting_comval.music_comval.music_repeat_mode;
    //获取引擎的播放模式信息
    //music_get_playmode(&cur_mode);

    //设置shuffle模式
    cur_mode |= 0x10;

    g_setting_comval.music_comval.music_shuffle = 0x10;

    g_shuffle_tmporary = FALSE;

    music_set_playmode(cur_mode);

    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}
