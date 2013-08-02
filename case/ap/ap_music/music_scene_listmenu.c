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

#include "app_music_albumlist.h"

//extern void save_breakinfo(void);

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_filelist_init(fsel_type_e cur_sel_save, file_path_info_t* browser_path)
 * \进入列表或专辑图片时判断是否需要重新初始化文件选择器
 * \param[in]    cur_sel_save  之前的文件选择器类型
 * \param[in]    browser_path  路径信息
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_scene_listmenu.c
 * \note
 */
/*******************************************************************************/
static bool music_filelist_init(file_path_info_t* browser_path)
{
    bool init_ret = TRUE;

    //文件列表控件
    //if (*cur_sel_save != browser_path->file_source)
    if((g_fsel_module.list_type != browser_path->file_source) 
        ||(g_fsel_module.disk_type != browser_path->file_path.dirlocation.disk))
    {
        //退出文件选择器
        fsel_exit();

        //初始化文件选择器
        init_ret = music_file_init(browser_path);

        if (init_ret == TRUE)
        {
            if (browser_path->file_source == FSEL_TYPE_LISTAUDIBLE)
            {
                g_audible_now = TRUE;
            }
        }
    }
    return init_ret;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \_deal_listmenu_result(file_path_info_t *browser_path, app_result_e ret_vals)
 * \处理列表或专辑图片控件返回值
 * \param[in]    browser_path  路径信息
 * \param[in]    ret_vals  控件返回值
 * \param[out]   none
 * \return        the result
 * \ingroup      music_scene_listmenu.c
 * \note
 */
/*******************************************************************************/
static app_result_e _deal_listmenu_result(file_path_info_t *browser_path, app_result_e ret_vals)
{
    //check返回值
    switch (ret_vals)
    {
        case RESULT_DIR_ERROR_ENTER_DIR:
        case RESULT_DIR_ERROR_NO_FILE:
        //对话框提示出错
        if (ret_vals == RESULT_DIR_ERROR_NO_FILE)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_SONG);
        }
        else
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        }
        case RESULT_ERROR:
        if (browser_path->file_source == FSEL_TYPE_LISTAUDIBLE)
        {
            g_audible_now = FALSE;
        }
        case RESULT_NULL:
        case RESULT_LISTMENU_MENU:
        ret_vals = RESULT_LISTMENU_MENU;
        break;

        case RESULT_MUSIC_PLAY:
        case RESULT_PLAYING_SHUFFLE:        
        case RESULT_PLAYING_START:
        //保存audible 播放断点写vram
        //if (g_audible_now == TRUE)
        //{
        //    save_breakinfo();
        //}
        case RESULT_AUDIBLE_PLAY:
        g_music_config.albumlist_sel = FALSE;
        libc_memcpy(&g_file_path_info, browser_path, sizeof(file_path_info_t));
        if(ret_vals != RESULT_PLAYING_SHUFFLE)
        {
            ret_vals = RESULT_PLAYING_START;
        }
        break;

        default:
        break;
    }
    return ret_vals;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e music_scene_listmenu_menu(uint8 browse_mode, file_path_info_t* browser_path)
 * \列表菜单场景菜单子场景控件
 * \param[in]    browse_mode  进入菜单时的浏览方式 0 从第一级菜单开始浏览 1 从最深处返回
 * \param[in]    browser_path 路径信息
 * \param[out]   none
 * \return       the result
 * \ingroup      music_scene_listmenu.c
 * \note
 */
/*******************************************************************************/
app_result_e music_scene_listmenu_menu(uint8 browse_mode, file_path_info_t* browser_path)
{
    bool init_ret;
    uint16 list_type;
    app_result_e ret_vals;

    //菜单参数
    menu_com_data_t menu_param;

    //菜单解析器
    menu_param.app_id = APP_ID_MUSIC;

    menu_param.menu_entry = LISTMENU_ENTRY;

    menu_param.menu_func = NULL; //有什么用
    //从根菜单进入
    menu_param.browse_mode = browse_mode;

    list_type = browser_path->file_path.plist_location.list_type;

    if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
    {
        menu_param.menu_entry = LISTMENU_ENTRY_DIR;
    }
    else
    {
        menu_param.menu_entry = LISTMENU_ENTRY;
    }
    menu_param.path_id = MUSIC_PATH_ID_MAIN;

    ret_vals = gui_menulist(MENULIST, &menu_param);

    //check返回值
    switch (ret_vals)
    {
        case RESULT_ENTER_TITLE_LIST://allsong
        case RESULT_ENTER_ARTIST_LIST://artist
        case RESULT_ENTER_ALBUM_LIST://album
        case RESULT_ENTER_GENRE_LIST://genre
        browser_path->file_source = FSEL_TYPE_PLAYLIST;
        browser_path->file_path.plist_location.list_type = (uint16)(PLIST_TITLE + ret_vals - RESULT_ENTER_TITLE_LIST);
        ret_vals = RESULT_LISTMENU_LIST;
        break;

        case RESULT_ENTER_FAVOR_LIST:
        browser_path->file_source = FSEL_TYPE_M3ULIST;
        browser_path->file_path.plist_location.list_type = PLIST_M3U;
        ret_vals = RESULT_LISTMENU_LIST;
        break;

        case RESULT_ENTER_ALBUM_THUMBNAIL:
        browser_path->file_source = FSEL_TYPE_PLAYLIST;
        browser_path->file_path.plist_location.list_type = PLIST_ALBUM;
        break;

        case RESULT_ENTER_BOOKS_LIST://book
        browser_path->file_source = FSEL_TYPE_LISTAUDIBLE;
        browser_path->file_path.plist_location.list_type = PLIST_BOOK;
        ret_vals = RESULT_LISTMENU_LIST;
        break;

        case RESULT_ENTER_AUTHORS_LIST://author
        browser_path->file_source = FSEL_TYPE_LISTAUDIBLE;
        browser_path->file_path.plist_location.list_type = PLIST_AUTHOR;
        ret_vals = RESULT_LISTMENU_LIST;
        break;

        default:
        if ((ret_vals == RESULT_NULL) || (ret_vals == RESULT_REDRAW))
        {
            ret_vals = RESULT_MAIN_APP;
        }
        break;
    }

    if ((ret_vals == RESULT_LISTMENU_LIST) || (ret_vals == RESULT_ENTER_ALBUM_THUMBNAIL))
    {
        //先初始化文件选择器
        init_ret = music_filelist_init(browser_path);
        if (init_ret == TRUE)
        {
            //定位新的播放列表
            if (list_type != (browser_path->file_path.plist_location.list_type))
            {
                com_clear_app_history(PATH_HISTORY_DIR, g_this_app_info->app_id,
                        browser_path->file_path.dirlocation.disk);
                libc_memset(&(browser_path->file_path.plist_location.filename), 0,
                        sizeof(browser_path->file_path.plist_location.filename));
            }

            fsel_browser_set_location(&(browser_path->file_path), browser_path->file_source);
        }
        else
        {
            ret_vals = RESULT_LISTMENU_MENU;
        }
    }
    return ret_vals;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e music_scene_listmenu_list(uint8 browse_mode, uint8 root_layer,
 fsel_type_e cur_sel_save, file_path_info_t* browser_path)
 * \列表菜单场景列表子场景控件
 * \param[in]    browse_mode  进入列表时的浏览方式 0 从顶层开始 1 从最底层开始
 * \param[in]    root_layer   根目录层级
 * \param[in]    browser_path 路径信息
 * \param[out]   none
 * \return       the result
 * \ingroup      music_scene_listmenu.c
 * \note
 */
/*******************************************************************************/

app_result_e music_scene_listmenu_list(uint8 browse_mode, uint8 root_layer, file_path_info_t* browser_path)
{
    bool init_ret = TRUE;

    app_result_e ret_vals;
    //列表参数
    dir_com_data_t list_param;

    list_menu_t menu_item_insert;

    list_param.list_option_func = list_scene_list_option_callback;
    list_param.root_layer = root_layer;
    list_param.del_back = FALSE;
    list_param.menulist_cnt = 0;

    menu_item_insert.app_id = APP_ID_MUSIC;

    list_param.menulist = NULL;
    list_param.browse_mode = browse_mode;
    if (browser_path->file_source == FSEL_TYPE_PLAYLIST)
    {
        switch (browser_path->file_path.plist_location.list_type)
        {
            case PLIST_ARTIST:
            case PLIST_GENRE:
            menu_item_insert.layer = 1;
            menu_item_insert.list_menu = ALLSONG_PLAY;
            list_param.menulist = &menu_item_insert;
            list_param.menulist_cnt = 1;
            break;
            case PLIST_TITLE:
            menu_item_insert.layer = 0;
            menu_item_insert.list_menu = SHUFFLE_PLAY;
            list_param.menulist = &menu_item_insert;
            list_param.menulist_cnt = 1;
            break;
            default:
            break;
        }
    }
    else if (browser_path->file_source == FSEL_TYPE_M3ULIST)
    {
        menu_item_insert.layer = 0;
        menu_item_insert.list_menu = M3ULIST_MENU_FAVOR;
        list_param.menulist = &menu_item_insert;
        list_param.menulist_cnt = 1;
        list_param.browse_mode |= (uint8)(0x80);
    }
    else
    {
        ;//nothing
    }

    //文件列表控件初始化
    init_ret = music_filelist_init(browser_path);

    if (init_ret == TRUE)
    {
        ret_vals = gui_directory(DIRLIST, browser_path, &list_param);
    }
    else
    {
        ret_vals = RESULT_ERROR;
    }

    //check返回值
    switch (ret_vals)
    {

        case RESULT_REDRAW:
        if (root_layer == 0)
        {
            switch (browser_path->file_source)
            {
                case FSEL_TYPE_LISTFAVOR1://favor1
                case FSEL_TYPE_LISTFAVOR2://favor2
                case FSEL_TYPE_LISTFAVOR3://favor3
                browser_path->file_source = FSEL_TYPE_M3ULIST;
                browser_path->file_path.plist_location.list_type = PLIST_M3U;
                ret_vals = RESULT_LISTMENU_LIST;
                break;

                default:
                ret_vals = RESULT_LISTMENU_MENU;
                break;
            }
        }
        else
        {
            ret_vals = RESULT_ENTER_ALBUM_THUMBNAIL;
        }
        break;

        case RESULT_ENTER_FAVOR1_LIST://favor1
        case RESULT_ENTER_FAVOR2_LIST://favor2
        case RESULT_ENTER_FAVOR3_LIST://favor3
        //这里对cur_sel_save的判断需要修改
        browser_path->file_source = (uint8)(FSEL_TYPE_LISTFAVOR1 + ret_vals - RESULT_ENTER_FAVOR1_LIST);
        ret_vals = RESULT_LISTMENU_LIST;
        break;

        default:
        ret_vals = _deal_listmenu_result(browser_path, ret_vals);
        break;
    }
    return ret_vals;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e music_scene_listmenu_albumlist(uint8 browse_mode, fsel_type_e cur_sel_save,
 file_path_info_t* browser_path)
 * \列表菜单场景列表子场景控件
 * \param[in]    browse_mode  进入列表时的浏览方式 0 从顶层开始 1 从最底层开始
 * \param[in]    browser_path 路径信息
 * \param[out]   none
 * \return       the result
 * \ingroup      music_scene_listmenu.c
 * \note
 */
/*******************************************************************************/

app_result_e music_scene_listmenu_albumlist(uint8 browse_mode, file_path_info_t* browser_path)
{
    bool init_ret = TRUE;

    app_result_e ret_vals;

    //列表参数
    dir_com_data_t list_param;

    list_param.list_option_func = list_scene_list_option_callback;
    list_param.root_layer = 0;
    list_param.del_back = FALSE;
    list_param.menulist_cnt = 0;

    list_param.menulist = NULL;
    list_param.browse_mode = browse_mode;

    init_ret = music_filelist_init(browser_path);

    if (init_ret == TRUE)
    {
        //文件列表控件
        ret_vals = pic_preview(browser_path, &list_param);
    }
    else
    {
        ret_vals = RESULT_ERROR;
    }
    //check返回值
    ret_vals = _deal_listmenu_result(browser_path, ret_vals);
    if (ret_vals == RESULT_PLAYING_START)
    {
        g_music_config.albumlist_sel = 1;
    }
    return ret_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint6 music_scene_listmenu(bool music_audible)
 * \列表菜单场景
 * \param[in]    menu_list  para1
 * \param[in]    bool  para1
 * \             1 进入菜单
 * \             0 进入列表
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_scene_listmenu.c
 * \note
 */
/*******************************************************************************/
app_result_e music_scene_listmenu(bool menu_list, file_path_info_t* browser_path)
{
    uint8 sub_scene_cur;
    uint8 menu_browser_mode = MUSIC_BROWSER_ROOT_LAYER;
    uint8 list_browser_mode = MUSIC_BROWSER_DEST_LAYER;
    uint8 albumlist_browser_mode = 1;
    uint8 root_layer = 0;
    app_result_e ret_vals = RESULT_NULL;

    sub_scene_cur = menu_list;

    if (sub_scene_cur == 0)
    {
        if (g_music_config.albumlist_sel == 1)
        {
            root_layer = 1;
        }
    }

    if (g_enter_mode == PARAM_FROM_PLAYLIST)
    {
        browser_path->file_source = FSEL_TYPE_PLAYLIST;
        browser_path->file_path.plist_location.list_type = PLIST_TITLE;

        sub_scene_cur = MUSIC_LISTSCENE_LIST;
        list_browser_mode = MUSIC_BROWSER_ROOT_LAYER;

        //清除进入模式为playlist
        g_enter_mode = PARAM_FROM_MAINMENU;
    }

    while (sub_scene_cur != MUSIC_LISTSCENE_EXIT)
    {
        switch (sub_scene_cur)
        {
            case MUSIC_LISTSCENE_LIST:
            ret_vals = music_scene_listmenu_list(list_browser_mode, root_layer, browser_path);
            break;

            case MUSIC_LISTSCENE_MENU:
            ret_vals = music_scene_listmenu_menu(menu_browser_mode, browser_path);
            break;

            case MUSIC_LISTSCENE_ALBUMLIST:
            ret_vals = music_scene_listmenu_albumlist(albumlist_browser_mode, browser_path);
            break;

            default:
            break;
        }

        switch (ret_vals)
        {
            case RESULT_LISTMENU_LIST:
            sub_scene_cur = MUSIC_LISTSCENE_LIST;
            root_layer = 0;
            list_browser_mode = MUSIC_BROWSER_ROOT_LAYER;
            break;

            case RESULT_LISTMENU_MENU:
            sub_scene_cur = MUSIC_LISTSCENE_MENU;
            menu_browser_mode = MUSIC_BROWSER_DEST_LAYER;
            break;

            case RESULT_ENTER_ALBUM_THUMBNAIL:
            sub_scene_cur = MUSIC_LISTSCENE_ALBUMLIST;
            albumlist_browser_mode = MUSIC_BROWSER_ROOT_LAYER;
            break;

            case RESULT_PLAYING_START:
            if (root_layer == 1)
            {
                g_music_config.albumlist_sel = TRUE;
            }
            sub_scene_cur = MUSIC_LISTSCENE_EXIT;
            break;

            case RESULT_ENTER_TITLE_LIST:
            sub_scene_cur = MUSIC_LISTSCENE_LIST;
            root_layer = MUSIC_BROWSER_DEST_LAYER;
            break;

            default:
            sub_scene_cur = MUSIC_LISTSCENE_EXIT;
            break;
        }
    }

    return ret_vals;
}

