/*******************************************************************************
 *                              US212A
 *                            Module: MUSIC_ADD_FAVORITE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-11-9 16:17:43           1.0              build this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_playing.h"

#include "app_music_favorite.h"

const uint8 allsong_str[] =
{ "All Songs" };

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_ab_count(uint8 count)
 * \通知后台添加当前播放的收藏夹文件
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
bool music_add_favorite_deal(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_ADD_FAVORITE_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool ask_for_override_favorlist(void)
 * \查询当收藏夹数目超过最大数目时是否覆盖
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 覆盖
 * \retval           0 不覆盖
 * \ingroup      music_favorite
 * \note
 */
/*******************************************************************************/
static app_result_e _ask_for_override_favorlist(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    //对话框是否创建播放列表
    ask_create_str.data.id = S_IS_OVERRIDE_FAVLIST;
    ask_create_str.language = UNICODEID;
    //对话框类型
    ask_dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_dialog_data.icon_id = -1;
    //字符串
    ask_dialog_data.string_desc = &ask_create_str;

    //按键类型
    ask_dialog_data.button_type = BUTTON_YESNOCANCEL;
    /*! 默认激活按键项 */
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    if (result == RESULT_DIALOG_NO)
    {
        result = RESULT_IGNORE;
    }

    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \获取ID3信息 获取文件名字段
 * \param[in]  buffer 存放id3的buffer param1
 * \param[in]  ext_name 该文件的后缀名 param2
 * \param[out]
 * \return
 * \retval
 * \ingroup     music_favorite
 * \note
 *******************************************************************************/
void get_music_id3_title(uint8 *buffer, uint8 len, uint8 *ext_name)
{
    id3_info_t id3_infor;
    id3_type_e cur_type;
    libc_memset(&id3_infor, 0, sizeof(id3_info_t));
    id3_infor.tit2_buffer = buffer;
    id3_infor.tit2_length = len;

    cur_type = play_scene_checktype(ext_name);
    get_id3_info(&id3_infor, NULL, cur_type);
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_add_favorite_result(uint8 ret)
 * \添加收藏夹文件出错时的提示
 * \已添加的文件会提示已添加该曲目
 * \param[in] ret 添加收藏夹后的返回值 param1
 * \param[out]
 * \return    bool 添加成功/失败
 * \ingroup      music_favorite
 * \note
 *******************************************************************************/

bool check_add_favorite_result(uint8 ret)
{
    //对话框
    bool result = TRUE;

    if (ret == FALSE)
    {
        ret = fsel_get_error();
        if (ret == FSEL_ERR_EXIST)
        {
            //已添加该曲目
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_ALREADY_ADD);
        }
        result = FALSE;
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e add_favorite_nowplaying(uint8 target_type)
 * \将当前后台正在播放的歌曲添加收藏夹
 * \param[in]  target_type 加入哪个收藏夹
 * \return
 * \ingroup     music_favorite
 * \note
 * \li   正在播放加入收藏夹之后返回调用该函数菜单的上一级菜单
 * \li   添加单个文件会检测是否添加成功
 *******************************************************************************/
app_result_e add_favorite_nowplaying(uint8 target_type)
{
    uint8 ret;
    bool check_result;
    flist_location_t favor_location;
    uint16 fav_file_total;
    app_result_e ui_result;

    void *plocation = (void *) &g_file_path_info.file_path;

    fav_file_total = fsel_favorite_get_total(target_type);
    if (fav_file_total == USERPL_FILE_MAX)
    {
        ui_result = _ask_for_override_favorlist();
        if (ui_result != RESULT_DIALOG_YES)
        {
            //超过最大数目后终止当前添加操作
            return ui_result;
        }
    }

    fsel_change_fav_location(plocation, &favor_location, g_file_path_info.file_source, &g_id3_info);
    ret = fsel_favorite_add(&favor_location, target_type);
    check_result = check_add_favorite_result(ret);
    if (check_result == FALSE)
    {
        ui_result = RESULT_IGNORE;
    }
    else
    {
        if ((fav_file_total != USERPL_FILE_MAX) && (g_file_path_info.file_source == target_type))
        {
            music_add_favorite_deal();
        }
        ui_result = RESULT_MENU_PARENT;
    }

    return ui_result;

}
/******************************************************************************/
/*!
 * \par  Description:
 * \添加单个文件到收藏夹
 * \param[in]  plocation 文件路径
 * \param[in]  target_type 收藏夹类型
 * \param[in]  fav_file_total 当前收藏夹文件总数
 * \param[in]  list_no 文件序号
 * \return
 * \ingroup     music_favorite
 * \note
 * \li   正在播放加入收藏夹之后返回调用该函数菜单的上一级菜单
 * \li   添加单个文件会检测是否添加成功
 *******************************************************************************/
static app_result_e add_favorlist_single(plist_location_t * plocation, uint8 target_type, uint16 fav_file_total,
        uint16 list_no)
{
    uint8 ret = TRUE;
    app_result_e ui_result;
    flist_location_t flocation;

    //单个文件添加
    if (fav_file_total == USERPL_FILE_MAX)
    {
        ui_result = _ask_for_override_favorlist();
        if (ui_result != RESULT_DIALOG_YES)
        {
            //超过最大数目后终止当前添加操作
            return ui_result;
        }
    }
    fsel_browser_select(list_no);
    fsel_browser_get_location(plocation, FSEL_TYPE_PLAYLIST);
    fsel_change_fav_location(plocation, &flocation, FSEL_TYPE_PLAYLIST, &g_id3_info);
    ret = fsel_favorite_add(&flocation, target_type);

    if (ret == TRUE)
    {
        if (g_file_path_info.file_source == target_type)
        {
            music_add_favorite_deal();
        }

        ui_result = RESULT_REDRAW;
    }
    else
    {
        ui_result = RESULT_ERROR;
    }

    return ui_result;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \将ui_directory()中某一个文件或某一类文件添加至收藏夹
 * \param[in]  plocation 当前文件的路径信息
 * \param[in]  target_type 加入哪个收藏夹
 * \param[in]  list_no  文件序号
 * \param[out]
 * \return
 * \retval
 * \ingroup    music_favorite
 * \note
 * \li   添加单个文件会检测是否添加成功
 * \li   添加完毕之后退出菜单控件，返回ui_directory
 * \li   正常的文件序号是从1开始，且目录总数一定为0.如果文件序号为0或者目录总数不
 为0，则表示是批量添加模式，即将all song/artist等某个类型表添加至收藏夹
 *******************************************************************************/
app_result_e add_favorite_list(plist_location_t * plocation, uint8 target_type, uint16 list_no)
{
    app_result_e ui_result;
    bool check_result;
    //uint8 ret = TRUE;
    //是否批量添加收藏夹
    uint8 batch_mode;
    uint16 fav_file_total;
    dir_brow_t browser;

    //浏览当前列表的结构体初始化
    browser.name_buf = NULL;
    browser.name_len = 0;

    fsel_browser_enter_dir(CUR_DIR, 0, &browser);

    if (browser.dir_total != 0)
    {
        //当前的下一级
        fsel_browser_enter_dir(SON_DIR, list_no, &browser);
        batch_mode = TRUE;
    }
    else if (list_no == 0)
    {
        batch_mode = TRUE;
    }
    else
    {
        batch_mode = FALSE;
    }

    fav_file_total = fsel_favorite_get_total(target_type);

    //批量添加
    if (batch_mode == TRUE)
    {
        ui_result = add_favorlist_all(plocation, target_type, fav_file_total, &browser);
    }
    else
    {
        ui_result = add_favorlist_single(plocation, target_type, fav_file_total, list_no);
    }
    if (ui_result == RESULT_ERROR)
    {
        check_result = check_add_favorite_result(FALSE);

        if (check_result == FALSE)
        {
            ui_result = RESULT_IGNORE;
        }
        else
        {
            ui_result = RESULT_REDRAW;
        }
    }

    return ui_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  添加收藏夹dialog控件实现，需要组合字符串
 * \param[in]   target_type 要添加的收藏夹类型
 * \param[in]   file_name   转换后的文件名(ID3)
 * \param[in]   dialog_type 添加前提示 0 添加后提示 1
 * \return      app_result_e the result
 * \retval
 * \ingroup     music_favorite
 * \note
 * \li   将字符串组织成"将[%s](file_name)添加至[%s](target_type)"或"已将[%s]添加至
 [%s]"形式,其中两个%s和资源字符串需要使用com_string_format()进行组织

 *******************************************************************************/
app_result_e add_favorite_dialog(fsel_type_e target_type, uint8 *file_name, uint8 dialog_type)
{
    app_result_e ret;

    dialog_com_data_t dialog;
    //目的字符串
    string_desc_t dest_data;
    //源字符串
    string_desc_t source_data[2];

    //源字符串0初始化
    if (*(uint16*) file_name == 0xfeff)
    {
        source_data[0].language = UNICODEDATA;
    }
    else if ((file_name[0] == 0xef) && (file_name[1] == 0xbb) && (file_name[2] == 0xbf))
    {
        source_data[0].language = UTF_8DATA;
    }
    else
    {
        source_data[0].language = (int8) g_setting_comval.g_comval.language_id;
    }

    source_data[0].data.str = file_name;

    source_data[0].length = 32;

    //源字符串1初始化
    source_data[1].language = UNICODEID;

    if (target_type == FSEL_TYPE_LISTFAVOR1)
    {
        source_data[1].data.id = S_NOW_FAVORITE1;
    }
    else if (target_type == FSEL_TYPE_LISTFAVOR2)
    {
        source_data[1].data.id = S_NOW_FAVORITE2;
    }
    else
    {
        source_data[1].data.id = S_NOW_FAVORITE3;
    }

    //目的字符串初始化
    //dest_data.data.str = g_insert_dialog;
    dest_data.data.str = g_temp_buffer_p;

    if (dialog_type == 0)
    {
        //加入到哪个收藏夹
        dest_data.argv = S_ADD_FAVORITE1;
    }
    else
    {
        //加入到哪个收藏夹
        dest_data.argv = S_ALREADY_ADD_FAVORITE;

    }

    //dest_data.language = (int8) g_setting_comval.g_comval.language_id;
    dest_data.language = UNICODEDATA;

    //dest_data.length = sizeof(g_insert_dialog);
    dest_data.length = 128;

    //组织对话框字符串
    com_string_format(&dest_data, source_data);

    //dialog控件参数初始化
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
 * \app_result_e add_favorite_listmenu(plist_location_t* location, uint8 target_type, uint16 active)
 * \在列表菜单中选择加入播放列表（收藏夹）
 * \param[in]  location 当前文件/列表的路径信息 param1
 * \param[in]  target_type 加入到那个收藏夹 param2
 * \param[in]  active 当前的活动项 param3
 * \param[out]
 * \return    返回操作结果,是否加入成功
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e add_favorite_listmenu(plist_location_t* location, uint8 target_type, uint16 active)
{
    app_result_e dialog_result;
    app_result_e add_result;
    uint8 ext_buf[4];

    brow_decode_t decode_param;
    file_brow_t file_list;
    //判断层次
    dir_brow_t browser;

    decode_param.option = 1;
    decode_param.num = 1;

    browser.name_buf = NULL;
    browser.name_len = 0;
    fsel_browser_enter_dir(CUR_DIR, 0, &browser);

    //active = dir_control.list_no - menu_cnt 如果为0表示当前为
    //ui_directory嵌套菜单项
    if ((active == 0) && (browser.dir_total != 0))
    {
        fsel_browser_enter_dir(PARENT_DIR, 0, &g_dir_browser);
        decode_param.top = last_dir_control.list_no;
    }
    else
    {
        decode_param.top = active;
    }

    file_list.name_buf = com_name_buffer;
    libc_memcpy(file_list.ext, ext_buf, sizeof(ext_buf));
    file_list.name_len = sizeof(com_name_buffer);
    //获取文件名的title
    fsel_browser_get_items(&decode_param, &file_list);
    if ((active == 0) && (browser.dir_total == 0))
    {
        //g_song_filename 设置为allsong
        libc_memcpy(com_name_buffer, allsong_str, sizeof(allsong_str));
    }

    //防止文件名过长,将文件名转换成部分文件名+省略号形式
    com_ellipsis_to_longname(com_name_buffer, com_name_buffer, 52);

    dialog_result = add_favorite_dialog(target_type, com_name_buffer, 0);
    if (dialog_result == RESULT_DIALOG_YES)
    {
        /* 确认 */
        add_result = add_favorite_list(location, target_type, decode_param.top);
        if (add_result == RESULT_REDRAW)
        {
            //提示
            add_favorite_dialog(target_type, com_name_buffer, 1);
        }
        else if (add_result == RESULT_IGNORE)
        {
            add_result = RESULT_REDRAW;
        }
        else
        {
            ;//nothing for QAC
        }
    }

    else
    {
        /* 返回热键消息 */
        add_result = dialog_result;
    }
    if ((active == 0) && (browser.dir_total != 0))
    {
        fsel_browser_enter_dir(SON_DIR, last_dir_control.list_no, &g_dir_browser);
    }

    return add_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e add_favorite_setmenu(uint8 target_type)
 * \ 在设置菜单中选择加入播放列表
 * \param[in]  target_type 添加到哪个收藏夹 param1
 * \param[out]
 * \return    返回操作结果,是否加入成功
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e add_favorite_setmenu(uint8 target_type)
{
    //uint16 cur_num, cur_total;
    uint32 cur_direnry, cur_cluster;
    //uint8 ret = 0;
    app_result_e dialog_result;
    app_result_e add_result;

    plist_location_t* locatptr = (plist_location_t*) &g_file_path_info.file_path.plist_location;

    cur_cluster = locatptr->cluster_no;
    cur_direnry = locatptr->dir_entry;

    music_get_filepath(&g_file_path_info);

    //判断是否切歌
    if ((cur_cluster != g_file_path_info.file_path.dirlocation.cluster_no) || (cur_direnry
            != g_file_path_info.file_path.dirlocation.dir_entry))
    {
        //如果切歌，需要重新获取歌曲的ID3 TITLE
        fsel_browser_set_file(&g_file_path_info.file_path.plist_location, target_type);
        get_music_id3_title(g_title_buf, sizeof(g_title_buf), locatptr->filename);
    }

    //防止文件名过长,将文件名转换成部分文件名+省略号形式
    com_ellipsis_to_longname(g_title_buf, com_name_buffer, 52);

    dialog_result = add_favorite_dialog(target_type, com_name_buffer, 0);
    if (dialog_result == RESULT_DIALOG_YES)
    {
        /* 确认 */
        add_result = add_favorite_nowplaying(target_type);
        if (add_result != RESULT_IGNORE)
        {
            //提示
            add_favorite_dialog(target_type, com_name_buffer, 1);
        }
        add_result = RESULT_MENU_PARENT;

    }
    else
    {
        /* 返回热键消息 */
        add_result = dialog_result;
    }
    return add_result;
}

