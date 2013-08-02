/*******************************************************************************
 *                              US212A
 *                            Module: ADD_FAVORITE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-9-23 11:11:27           1.0              build this file
 *******************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_ab_count(uint8 count)
 * \通知后台删除当前播放的收藏夹文件
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
bool music_del_favorite_deal(uint16 del_num)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //传入删除的文件序号
    msg.content.addr = &del_num;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_DEL_FAVORITE_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_del_favorite_result(uint8 ret)
 * \删除收藏夹文件出错时的提示
 * \已删除的文件再删除会提示无法再播放列表中
 * \找到此歌曲
 * \param[in]  ret 提示选择 param1
 * \param[out]
 * \return      bool
 * \retval
 * \ingroup     music_favorite
 * \note
 *******************************************************************************/

bool check_del_favorite_result(uint8 ret)
{
    bool result = TRUE;
    if (ret == FALSE)
    {
        /* 无法在播放列表中找到此歌曲! */
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NOT_IN_FAVORITE);
        result = FALSE;
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_dialog(fsel_type_e target_type, uint8 *file_name,uint8 dialog_type)
 * \删除收藏夹dialog控件实现
 * \param[in]  target_type 收藏夹定位 param1
 * \param[in]  file_name 文件名 param2
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_favorite
 * \note
 *******************************************************************************/
app_result_e del_favorite_dialog(fsel_type_e target_type, uint8 *file_name, uint8 dialog_type)
{
    app_result_e ret;

    dialog_com_data_t dialog;

    //字符串组织
    string_desc_t dest_data;
    string_desc_t source_data[2];

    source_data[0].language = UNICODEID;

    if (*(uint16*) file_name == 0xfeff)
    {
        source_data[1].language = UNICODEDATA;
    }
    else if ((file_name[0] == 0xef) && (file_name[1] == 0xbb) && (file_name[2] == 0xbf))
    {
        source_data[1].language = UTF_8DATA;
    }
    else
    {
        source_data[1].language = (int8) g_setting_comval.g_comval.language_id;
    }

    if (target_type == FSEL_TYPE_LISTFAVOR1)
    {
        source_data[0].data.id = S_NOW_FAVORITE1;
    }
    else if (target_type == FSEL_TYPE_LISTFAVOR2)
    {
        source_data[0].data.id = S_NOW_FAVORITE2;
    }
    else
    {
        source_data[0].data.id = S_NOW_FAVORITE3;
    }

    source_data[1].data.str = file_name;

    source_data[1].length = 32;

    //dest_data.data.str = g_insert_dialog;
    dest_data.data.str = g_temp_buffer_p;
    dest_data.length = 128;

    if (dialog_type == 0)
    {
        //从哪个收藏夹删除
        dest_data.argv = S_DEL_FROM_FAVORITE1;
    }
    else
    {
        //从哪个收藏夹删除
        dest_data.argv = S_ALREADY_DEL_FAVORITE;
    }

    //dest_data.language = (int8) g_setting_comval.g_comval.language_id;
    dest_data.language = UNICODEDATA;

    //组织对话框字符串
    com_string_format(&dest_data, source_data);
    dialog.icon_id = -1;
    dialog.string_desc = &dest_data;
    if (dialog_type == 0)
    {
        //询问
        //对话框参数设置
        dialog.dialog_type = DIALOG_BUTTON_INCLUDE;

        dialog.button_type = BUTTON_YESNO;
        dialog.active_default = 0;
        ret = gui_dialog(DIALOG_ASK, &dialog);
    }
    else
    {
        //提示
        //对话框参数设置
        dialog.dialog_type = DIALOG_INFOR_WAIT;
        dialog.active_default = 0;
        ret = gui_dialog(DIALOG_MSG, &dialog);
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_nowplaying(uint8 target_type)
 * \删除收藏夹记录，如果当前播放的正是收藏夹文件且只有该文件
 * \删除之后关闭音乐引擎，返回收藏夹菜单
 * \需要知道是否是从收藏夹选择歌曲播放?
 * \删除收藏夹记录来源有两种:收藏夹，播放界面
 * \分两种处理情景:从播放界面删除，从列表界面删除
 * \param[in]  target_type 收藏夹定位 param1
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_favorite
 * \note
 *******************************************************************************/
app_result_e del_favorite_nowplaying(uint8 target_type)
{
    app_result_e retval;
    uint8 ret;
    bool check_val;
    //uint16 item_total;
    bool need_check_engine = FALSE;

    dir_brow_t get_brower_info;
    uint8 ext_buf[4];
    get_brower_info.name_buf = ext_buf;
    get_brower_info.name_len = 4;

    libc_memcpy(&g_browser_filepath, &g_file_path_info, sizeof(file_path_info_t));
    //当前是否在收藏夹模式播放
    if (g_file_path_info.file_source == target_type)
    {
        need_check_engine = TRUE;
    }
    else
    {
        //g_browser_filepath.file_source = target_type;
        fsel_exit();
        music_file_init(&g_browser_filepath);
        //定位到新的类型表项
        fsel_browser_set_location(NULL, target_type);
    }

    //ret = fsel_browser_delete(BROW_DEL_FILE, 0, (void *) &g_browser_filepath.file_path.dirlocation);
    ret = fsel_favorite_delete(target_type, 0, &g_browser_filepath.file_path.flist_location);

    check_val = check_del_favorite_result(ret);

    if(check_val == TRUE)
    {
        del_favorite_dialog(target_type, com_name_buffer, 1);
        
        if (need_check_engine == TRUE) 
        {
            //item_total = fsel_get_total();
            fsel_browser_enter_dir(ROOT_DIR, 0, &get_brower_info);
            if (get_brower_info.file_total == 0)
            {
                //只剩下一首歌曲，通知引擎退出
                music_close_engine();

                //未找到歌曲!
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);                

                //如论从哪里进入，都需要重新回到
                retval = RESULT_LISTMENU_MENU;
            }
            else
            {
                music_del_favorite_deal(g_browser_filepath.file_path.flist_location.file_num);
                //通知引擎切下一曲
                music_play_next();

                //重新回到上一级菜单
                retval = RESULT_MENU_PARENT;
            }
        }
        else
        {
            retval = RESULT_MENU_PARENT;
        }
        
    }
    else
    {
        retval = RESULT_MENU_PARENT;
    }
    
    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_setmenu(uint8 target_type)
 * \将当前后台正在播放歌曲从收藏夹中删除
 * \param[in]  target_type 收藏夹定位 param1
 * \param[out]
 * \return   返回操作结果,是否加入成功
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e del_favorite_setmenu(uint8 target_type)
{
    //uint16 cur_num, cur_total;
    uint32 cur_direnry, cur_cluster;
    app_result_e ret;
    app_result_e set_del_result;

    plist_location_t* locatptr = (plist_location_t*) &g_file_path_info.file_path.plist_location;

    cur_cluster = locatptr->cluster_no;
    cur_direnry = locatptr->dir_entry;

    music_get_filepath(&g_file_path_info);

    if ((cur_cluster != g_file_path_info.file_path.dirlocation.cluster_no) || (cur_direnry
            != g_file_path_info.file_path.dirlocation.dir_entry))
    {
        //如果切歌，需要重新获取歌曲的ID3 TITLE
        fsel_browser_set_file(locatptr, target_type);
        get_music_id3_title(g_title_buf, sizeof(g_title_buf), locatptr->filename);
    }

    //防止文件名过长,将文件名转换成部分文件名+省略号形式
    com_ellipsis_to_longname(g_title_buf, com_name_buffer, 52);

    ret = del_favorite_dialog(target_type, com_name_buffer, 0);
    if (ret == RESULT_DIALOG_YES)
    {
        /* 确认 */
        set_del_result = del_favorite_nowplaying(target_type);
        if (set_del_result != RESULT_IGNORE)
        {
            
        }
        else
        {
            set_del_result = RESULT_MENU_PARENT;
        }
    }
    else
    {
        /* 返回热键消息 */
        set_del_result = ret;
    }
    return set_del_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_list(uint8 target_type, uint16 index)
 * \将ui_directory某一歌曲从收藏夹中删除
 * \param[in]  target_type 收藏夹定位 param1
 * \param[in]  index 当前文件序号 param2
 * \param[out]
 * \return    返回操作结果,是否加入成功
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e del_favorite_list(uint8 target_type, uint16 index)
{
    app_result_e del_result;
    uint8 ret;
    //uint16 item_total;
    bool check_ret;
    bool need_check_engine = FALSE;

    dir_brow_t get_brower_info_list;
    uint8 ext_buf_list[4];

    //dialog_com_data_t dialog_hint;

    //string_desc_t hint_str;

    //是否后台存在music
    engine_type_e engine_type = get_engine_type();

    get_brower_info_list.name_buf = ext_buf_list;
    get_brower_info_list.name_len = 4;

    if (engine_type == ENGINE_MUSIC)
    {
        music_get_filepath(&g_file_path_info);

        if (g_file_path_info.file_source == target_type)
        {
            need_check_engine = TRUE;
        }
    }

    ret = fsel_favorite_delete(target_type, index, NULL);

    check_ret = check_del_favorite_result(ret);

    fsel_browser_enter_dir(ROOT_DIR, 0, &get_brower_info_list);
    //通知后台引擎删除了当前播放的收藏夹的文件
    if ((g_file_path_info.file_source == target_type) && (check_ret == TRUE))
    {
        music_del_favorite_deal(index);
    }

    if ((need_check_engine == TRUE) && (check_ret == TRUE))
    {
        if (get_brower_info_list.file_total == 0)
        {
            //只剩下一首歌曲，通知引擎退出
            //未找到歌曲!
            music_close_engine();
            del_result = RESULT_DELETE_FILE;
        }
        else
        {
            if (g_file_path_info.file_path.flist_location.file_num == index)
            {
                //通知引擎切下一曲
                music_play_next();
            }
            //重新回到上一级菜单
            del_result = RESULT_DELETE_FILE;
        }
    }
    else
    {
        if (check_ret == FALSE)
        {
            del_result = RESULT_IGNORE;
        }
        else
        {
            del_result = RESULT_DELETE_FILE;
        }
    }
    return del_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e del_favorite_listmenu(uint8 target_type, uint16 active)
 * \将ui_directory某一歌曲从收藏夹中删除
 * \param[in]  title_action
 * \param[out] 返回操作结果,是否加入成功
 * \return
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e del_favorite_listmenu(uint8 target_type, uint16 active)
{
    app_result_e dialog_result;
    app_result_e del_result;
    uint8 ext_buf[4];
    //uint8 file_name[12];

    brow_decode_t decode_param;
    file_brow_t file_list;

    decode_param.option = 1;
    decode_param.num = 1;
    decode_param.top = active;

    file_list.name_buf = com_name_buffer;
    file_list.name_len = sizeof(com_name_buffer);
    libc_memcpy(file_list.ext, ext_buf, sizeof(ext_buf));

    //获取文件名的title
    fsel_browser_get_items(&decode_param, &file_list);

    //防止文件名过长,将文件名转换成部分文件名+省略号形式
    com_ellipsis_to_longname(com_name_buffer, com_name_buffer, 52);

    dialog_result = del_favorite_dialog(target_type, com_name_buffer, 0);

    if (dialog_result == RESULT_DIALOG_YES)
    {
        /* 确认 */
        del_result = del_favorite_list(target_type, active);
        if (del_result != RESULT_IGNORE)
        {
            del_favorite_dialog(target_type, com_name_buffer, 1);
        }
        else
        {
            del_result = RESULT_REDRAW;
        }
    }
    else
    {
        /* 返回热键消息or取消 */
        del_result = dialog_result;
    }
    return del_result;
}

/*! \endcond */
