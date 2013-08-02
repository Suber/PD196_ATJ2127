/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_play_mode.c
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_setting.h"
#include "menu_callback_play_mode.h"




//一级菜单-播放模式
//播放模式|-音乐来源于...|-...来源于所有文件
//        |              |-...来源于此演唱者
//        |              |-...来源于此专辑
//        |              |-...来源于此类风格
//        |-重复播放|-关
//        |         |-单曲重复
//        |         |-全部重复
//        |-随机播放|-关
//                  |-开
////////////////////

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于option键的处理，用来显示提示信息
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：无
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e option_callback(void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    string_desc.data.id = S_SETTING_OPTION;
    string_desc.language = UNICODEID;

    dialog_com.dialog_type = DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playmode(uint8* playmode)
 * \获取循环模式&shuffle模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      menu_callback_play_mode.c
 * \note
 */
/*******************************************************************************/
bool music_get_playmode(uint8* playmode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;
    //获取当前循环模式
    reply.content = playmode;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_GET_PLAYMODE_SYNC;
    //发送同步消息
    if (music_exist_flag != 0)
    {
        bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    }
    else
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_playmode(uint8 playmode)
 * \设置循环模式&shuffle模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      menu_callback_play_mode.c
 * \note
 */
/*******************************************************************************/
bool music_set_playmode(uint8 playmode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //设置循环模式&shuffle模式
    msg.content.addr = &playmode;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_SET_PLAYMODE_SYNC;
    //发送同步消息
    if (music_exist_flag != 0)
    {
        bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    }
    else
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void switch_play_mode(repeat_mode_e mode)
 * \修改当前播放文件的循环模式
 * \param[in]    mode  para1
 * \             typedef enum
 * \             {
 * \             	LoopNor, //普通循环
 * \             	LoopOne, //循环放当前首
 * \             	LoopOneDir, //循环放当前目录
 * \             	LoopDir, //顺序播放目录
 * \             	LoopAll, //循环所有
 * \              Intro, //浏览播放
 * \              MaxRepeatMod
 * \             }repeat_mode_e;
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval
 * \ingroup      menu_callback_play_mode.c
 * \note
 */
/*******************************************************************************/
void switch_play_mode(repeat_mode_e mode)
{
    uint8 cur_mode;

    //获取引擎的循环模式信息
    music_get_playmode(&cur_mode);
    //设置播放模式
    cur_mode &= (uint8) 0xf0;
    cur_mode |= (uint8) mode;
    music_set_playmode(cur_mode);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filepath(file_path_info_t *filepath)
 * \获取当前播放文件的路径信息
 * \param[in]    filepath存放文件路径buffer的指针  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      menu_callback_play_mode.c
 * \note
 */
/*******************************************************************************/
bool music_get_filepath(file_path_info_t *filepath)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;
    //获取当前播放文件的路径信息
    reply.content = filepath;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    //发送同步消息
    if (music_exist_flag != 0)
    {
        bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    }
    else
    {
        bret = FALSE;
    }
    return bret;
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
 * \ingroup      menu_callback_play_mode.c
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
    if (music_exist_flag != 0)
    {
        bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    }
    else
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void set_music_source_to_engine(plist_type_e dest_type)
 * \切换当前播放的文件路径
 * \param[in]    dest_type  para1
 * \             typedef enum
 * \             {
 * \             	 Plist_AllSong,//music title
 * \             	 Plist_Artist, //music artist
 * \             	 Plist_Album,  //music album
 * \             	 Plist_Genre,  //music genre
 * \               Plist_Books,//audible books
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
void set_music_source_to_engine(plist_type_e dest_type)
{
    plist_location_t result_ptr;
    file_path_info_t g_file_path_info;
    void* cur_locat;
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
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置music来源于all songs
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_from_all_songs_callback(void *menu_title_void)
{
    if ((music_exist_flag == ENGINE_MUSIC)&&(playlist_exist_flag ==1))
    {    
        g_setting_vars.music_comval.music_from = VALUE_FROM_ALL_SONGS;
        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
        return (app_result_e)RESULT_NOWPLAYING;
    }
    return (app_result_e) RESULT_REDRAW; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于music来源于该演唱者
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_from_artist_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    if ((music_exist_flag != ENGINE_MUSIC)||(playlist_exist_flag !=1))
    {
        string_desc.data.id = S_NO_ARTIST;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
    }
    else
    {
        g_setting_vars.music_comval.music_from = VALUE_FROM_ARTIST;
        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
        return (app_result_e)RESULT_NOWPLAYING;
    }
    return (app_result_e) RESULT_REDRAW; //返回上一级
}
/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置music来源于该专辑
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_from_album_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    if ((music_exist_flag != ENGINE_MUSIC)||(playlist_exist_flag !=1))
    {
        string_desc.data.id = S_NO_ALBUM;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
    }
    else
    {
        g_setting_vars.music_comval.music_from = VALUE_FROM_ALBUM;
        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
        return (app_result_e)RESULT_NOWPLAYING;
    }
    return (app_result_e) RESULT_REDRAW; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置music来源于该风格
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_from_genre_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    if ((music_exist_flag != ENGINE_MUSIC)||(playlist_exist_flag !=1))
    {
        string_desc.data.id = S_NO_GENRE;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
    }
    else
    {
        g_setting_vars.music_comval.music_from = VALUE_FROM_GENRE;
        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
        return (app_result_e)RESULT_NOWPLAYING;
    }
    return (app_result_e) RESULT_REDRAW; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得音乐重复播放模式string ID号
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回字符串ID号，以便在下级菜单取得激活项
 * \retval       ：进入下一级菜单
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_music_repeat_mode_id_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_repeat_mode;
    menu_title_repeat_mode = (menu_title_action_t*) title_action;
    if (g_setting_vars.music_comval.music_repeat_mode == VALUE_MUSIC_REPEAT_ONE)
    {
        active_id = S_REPEAT_ONE;
    }
    else if (g_setting_vars.music_comval.music_repeat_mode == VALUE_MUSIC_REPEAT_ALL)
    {
        active_id = S_REPEAT_ALL;
    }
    else if (g_setting_vars.music_comval.music_repeat_mode == VALUE_MUSIC_REPEAT_OFF)
    {
        active_id = S_REPEAT_OFF;
    }
    else if(g_setting_vars.music_comval.music_repeat_mode ==VALUE_MUSIC_REPEAT_INTRO)
    {
        active_id = S_REPEAT_INTRO;
    }
    else
    {
        active_id = S_REPEAT_OFF;
    }
    menu_title_repeat_mode->menu_entry = -1; //不更新入口
    menu_title_repeat_mode->str_id = active_id; //指定active项
    menu_title_repeat_mode->ret_str = NULL;
    menu_title_repeat_mode->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置music关闭循环
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_repeat_off_callback(void *menu_title_void)
{
    g_setting_vars.music_comval.music_repeat_mode = VALUE_MUSIC_REPEAT_OFF;
    //切换循环方式
    switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置music单曲循环
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_repeat_one_callback(void *menu_title_void)
{
    g_setting_vars.music_comval.music_repeat_mode = VALUE_MUSIC_REPEAT_ONE;
    //切换循环方式
    switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于music全部循环
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_repeat_all_callback(void *menu_title_void)
{
    g_setting_vars.music_comval.music_repeat_mode = VALUE_MUSIC_REPEAT_ALL;
    //切换循环方式
    switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于music浏览播放
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_intro_callback(void *menu_title_void)
{
    g_setting_vars.music_comval.music_repeat_mode = VALUE_MUSIC_REPEAT_INTRO;
    //切换循环方式
    switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得music随机循环方式
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回字符串ID号，以便在下级菜单取得激活项
 * \retval       ：进入下一级菜单
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_music_shuflle_option(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_shuffle_option;
    menu_title_shuffle_option = (menu_title_action_t*) title_action;
    if (g_setting_vars.music_comval.music_shuffle == VALUE_MUSIC_SHUFFLE_ON)
    {
        active_id = S_ON;
    }
    else
    {
        active_id = S_OFF;
    }
    menu_title_shuffle_option->menu_entry = -1; //不更新入口
    menu_title_shuffle_option->str_id = active_id; //指定active项
    menu_title_shuffle_option->ret_str = NULL;
    menu_title_shuffle_option->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置music shuffle 关闭
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_shuffle_off_callback(void *menu_title_void)
{

    uint8 cur_mode;
    //获取引擎的播放模式信息
    music_get_playmode(&cur_mode);
    //设置shuffle模式
    cur_mode &= 0x0f;
    music_set_playmode(cur_mode);

    g_setting_vars.music_comval.music_shuffle = VALUE_MUSIC_SHUFFLE_OFF;
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置music shuffle使能
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_music_shuffle_on_callback(void *menu_title_void)
{
    uint8 cur_mode;
    //获取引擎的播放模式信息
    music_get_playmode(&cur_mode);
    //设置shuffle模式
    cur_mode |= 0x10;
    music_set_playmode(cur_mode);

    g_setting_vars.music_comval.music_shuffle = VALUE_MUSIC_SHUFFLE_ON;
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本函数恢复music 默认设置
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：无
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
void restore_factory_setting_for_music(void)
{
    uint8 cur_mode;
    if(music_exist_flag == ENGINE_MUSIC)
    {    
//不需要恢复音乐来源
//        set_music_source_to_engine((plist_type_e) g_setting_vars.music_comval.music_from);
//恢复循环方式
        switch_play_mode((repeat_mode_e) g_setting_vars.music_comval.music_repeat_mode);
//恢复shuffle 模式
        music_get_playmode(&cur_mode);    //获取引擎的播放模式信息
        if(g_setting_vars.music_comval.music_shuffle == VALUE_MUSIC_SHUFFLE_ON)
        {
            cur_mode |= 0x10; //设置shuffle on模式    
        }   
        else
        { 
            cur_mode &= 0x0f; //设置shuffle off模式
        }
        music_set_playmode(cur_mode);  
    }
}

