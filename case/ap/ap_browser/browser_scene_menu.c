/*******************************************************************************
 *                              US212A
 *                            Module: BROWSER_SCENE_MENU
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-3-29 9:47:16           1.0              build this file
 *******************************************************************************/
/*!
 * \file     browser_scene_menu.c
 * \brief    browser菜单模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2012-3-29
 *******************************************************************************/
#include "browser.h"
#include "browser_menu_cfg.h"

/*! \brief record目录长文件名数组 */
static const uint8 record_name[] =
{
    0xff, 0xfe,  //flag
    'R',  0x00,
    'E',  0x00,
    'C',  0x00,
    'O',  0x00,
    'R',  0x00,
    'D',  0x00,
    0x00, 0x00  //end flag
};

file_path_info_t temp_file_path _BANK_DATA_ATTR_;


extern bool _enter_record(void)
__FAR__;
/******************************************************************************/
/*!
 * \par  Description:
 *	  切换盘符后重新初始化文件选择器
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
static app_result_e _browser_filelist_init(uint8 diskflag)
{
    fsel_error_e fsel_error;
    app_result_e result = RESULT_BROWSER_FILELIST;

    uint8 *cur_disk = &(g_browser_var.path.file_path.dirlocation.disk);

    if (*cur_disk != diskflag)
    {
        //关闭音乐引擎
        music_close_engine();

        //退出当前文件选择器
        fsel_exit();

        //初始化location为无效location
        libc_memset(&g_browser_var.path.file_path, 0, sizeof(musfile_location_u));

        //更改盘符
        *cur_disk = diskflag;

        //初始化picture应用的文件选择器
        fsel_error = browser_fsel_initial(*cur_disk);

        if (fsel_error == FSEL_NO_ERR)
        {
            result = RESULT_BROWSER_FILELIST;
        }
        else
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
            result = RESULT_MAIN_APP;
            g_browser_scene_next = SCENE_EXIT;
        }
    }

    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  主盘目录菜单回调函数 初始化文件选择器
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_main_disk(void *param)
{
    return _browser_filelist_init(DISK_C);
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  插卡目录菜单回调函数 初始化文件选择器
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_card_disk(void *param)
{
    if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
    {
        return _browser_filelist_init(DISK_H);
    }
    else
    {
        //提示语
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);

        //重新进入文件列表
        return RESULT_BROWSER_FILELIST;
    }
}
#if 0
/******************************************************************************/
/*!
 * \par  Description:
 *	  遍历删除文件函数
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
static void _delete_all_file(dir_brow_t *browser_dir_browser, file_path_info_t *path_info)
{
    bool result;
    scanfile_result_e browser_result;

    //显示正在删除文件的对话框
    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_DEL_FILE_ING);

    //文件夹遍历浏览初始化
    fsel_browser_scanfile_init(browser_dir_browser);

    //遍历文件夹并删除文件及空文件夹
    while (1)
    {
        browser_result = fsel_browser_scanfile(path_info, browser_dir_browser, SCANFILE_TYPE_DELETE);
        if (browser_result == SCANFILE_RESULT_FILE)//返回文件

        {
            //删除当前文件
            result = fsel_browser_delete(BROW_DEL_FILE, 0, &(path_info->file_path));
        }
        else//返回文件夹，包括子文件夹 ,父文件夹, 文件夹自身

        {
            //如果文件夹为空，删除之
            if (browser_result == SCANFILE_RESULT_PARENTDIR)
            {
                //返回到父目录，则该子目录可以删除
                result = fsel_browser_delete(BROW_DEL_DIR, 1, &(path_info->file_path));
                //更新当前子文件目录总数
                browser_dir_browser->dir_total--;
            }

            if (browser_result == SCANFILE_RESULT_SELF)//文件夹自身

            {
                //删除所有文件完毕，显示已删除动画，退出应用
                deleted_file_display("RECORD");
                return;
            }
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser 关闭后台引擎提示函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 * \li   按全部删除菜单提示是否关闭后台音乐
 *******************************************************************************/
static app_result_e ask_for_close_mengine(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    //对话框是否创建播放列表
    ask_create_str.data.id = S_CLOSE_MUSIC_DEL;
    ask_create_str.language = UNICODEID;
    //对话框类型
    ask_dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_dialog_data.icon_id = -1;
    //字符串
    ask_dialog_data.string_desc = &ask_create_str;

    //按键类型
    ask_dialog_data.button_type = BUTTON_YESNO;
    /*! 默认激活按键项 */
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser 删除record目录所有目录项提示函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
static app_result_e _ask_for_del_all(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    //对话框是否删除文件
    if ((g_browser_var.prev_enter_mode == PARAM_FROM_RADIO) || (g_browser_enter_mode == PARAM_FROM_RADIO))
    {
        ask_create_str.data.id = S_DEL_RADIO_FILE;
    }
    else
    {
        ask_create_str.data.id = S_DEL_RECORD_FILE;
    }

    ask_create_str.language = UNICODEID;
    //对话框类型
    ask_dialog_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_dialog_data.icon_id = -1;
    //字符串
    ask_dialog_data.string_desc = &ask_create_str;

    //按键类型
    ask_dialog_data.button_type = BUTTON_YESNO;
    //默认激活按键项
    ask_dialog_data.active_default = 0x01;

    result = gui_dialog(DIALOG_ASK, &ask_dialog_data);

    return result;
}
#endif
/******************************************************************************/
/*!
 * \par  Description:
 *	  遍历删除文件函数
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
static app_result_e _delete_all_file(file_path_info_t *path_info)
{
    app_result_e result;
    del_com_data_t del_com;

    libc_memcpy(browser_file_name, record_name, sizeof(record_name));

    //调用common控件删除之
    del_com.del_no = 0;
    del_com.filename = browser_file_name;
    del_com.del_playing = FALSE;
    del_com.del_func = NULL;
    //包括删除节点
    del_com.del_self = FALSE;
    result = gui_delete(path_info, &del_com);

    switch (result)
    {
        //成功删除
        case RESULT_DELETE_DIRLIST_NOSELF:
        result = RESULT_BROWSER_EXIT;
        break;

        //无法删除
        case RESULT_ERROR:
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DEL_FAILED);
        //重新进入record目录
        _enter_record();
        result = RESULT_REDRAW;
        break;

        default:
        break;
    }

    return result;

}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser全部删除叶子菜单项函数
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_del_all(void *param)
{
    app_result_e ui_result;
    bool result;

    dir_brow_t brow;

    //不获取目录名
    brow.name_len = 0;
    //初始化进入目录环境
    result = fsel_browser_enter_dir(CUR_DIR, 0, &brow);

    if (result == TRUE)
    {
        //成功进入该目录
        ui_result = _delete_all_file(&g_browser_var.path);
    }
    else
    {
        ui_result = RESULT_DIR_ERROR_ENTER_DIR;
    }
    return ui_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser正在播放叶子菜单项函数
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_playing(void * param)
{
    return RESULT_NOWPLAYING;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser上一次播放叶子菜单项函数
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_last_play(void * param)
{
    return RESULT_LASTPLAY;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser删除叶子菜单项函数
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_del_file(void *param)
{
    bool ret_val;
    app_result_e result;
    del_com_data_t del_com;
    dir_brow_t brow;
    brow_decode_t decode_param;
    file_brow_t file_list;

    //文件序号等于当前的文件激活项序号减去嵌套菜单个数
    uint16 list_num = dir_control.list_no - dir_menu_title.count;

    brow.name_buf = browser_file_name;
    brow.name_len = LIST_ITEM_BUFFER_LEN;

    //确定删除模式:文件夹或文件
    fsel_browser_enter_dir(CUR_DIR, 0, &brow);

    //如果当前文件序号小于目录总数，则是删除文件夹及其文件
    if (list_num <= brow.dir_total)
    {
        del_com.del_mode = DEL_MODE_LIST;

        fsel_browser_select(list_num);

        fsel_browser_get_location(&(g_browser_var.path.file_path.dirlocation), g_browser_var.path.file_source);

        libc_memcpy(&temp_file_path, &g_browser_var.path, sizeof(file_path_info_t));

    }
    else
    {
        del_com.del_mode = DEL_MODE_FILE;
    }

    //获取要删除目录项文件名
    fsel_browser_select(list_num);
    decode_param.option = 1;
    decode_param.num = 1;
    decode_param.top = list_num;
    file_list.name_buf = browser_file_name;
    file_list.name_len = LIST_ITEM_BUFFER_LEN;
    fsel_browser_get_items(&decode_param, &file_list);

    //定位待删除文件夹或文件
    if (del_com.del_mode == DEL_MODE_LIST)
    {
        //进入当前目录
        ret_val = fsel_browser_enter_dir(SON_DIR, list_num, &brow);
        if(ret_val == FALSE)
        {
            //超过八级目录，无法删除
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DEL_FAILED);
            return RESULT_REDRAW;
        }
    }
    else
    {
        //定位到待删除文件
        fsel_browser_get_location(&g_browser_var.path.file_path, g_browser_var.path.file_source);
        fsel_browser_set_file(&g_browser_var.path.file_path, g_browser_var.path.file_source);
    }

    //调用common控件删除之
    del_com.del_no = list_num;
    del_com.filename = browser_file_name;
    del_com.del_playing = FALSE;
    del_com.del_func = NULL;
    //包括删除节点
    del_com.del_self = TRUE;
    result = gui_delete(&g_browser_var.path, &del_com);

    if (del_com.del_mode == DEL_MODE_LIST)
    {
        //如果删除模式为删除文件夹，而最终又没有操作，则返回上一级目录
        if (result == RESULT_REDRAW)
        {
            //因为之前进入当前子目录，现在要重新返回
            fsel_browser_enter_dir(PARENT_DIR, 0, &brow);
        }
        else if (result == RESULT_ERROR)
        {
            //超过八级目录，无法删除
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DEL_FAILED);
            fsel_browser_set_location(&(temp_file_path.file_path.dirlocation), temp_file_path.file_source);
            result = RESULT_REDRAW;
        }
        else
        {
            ;//nothing
        }
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser option键出来的菜单控件按option键回调函数
 * \param[in]    param  no use
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_option_option(void)
{
    //返回上一级
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    browser 显示消息提示
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_browsing
 * \note
 *******************************************************************************/
app_result_e _browser_show_information(void)
{
    return gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_OPTION_DESC);
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser ui_directory()控件出来的ui_menulist()控件函数，作为文件列表的option
 *	          键回调函数被调用
 * \param[in]    path_info  ui_directory()控件传递的path_info
 * \param[in]    active     相应的激活项序号
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_menu
 * \note
 * \li  该函数为ui_directory()的option键回调函数
 *******************************************************************************/
app_result_e browser_menu_option(file_path_info_t *path_info, uint16 active)
{
    app_result_e result;
    engine_state_e play_sta;
    //初始化菜单控件
    menu_com_data_t op_menu;

    //一般文件列表激活项都是从1开始计数，只有出现
    //嵌套菜单时才会值为0
    if (active == 0)
    {
        //属于record的第一个菜单项，则只给出提示语
        return _browser_show_information();
    }

    play_sta = get_engine_state();

    //定义不同模式下的入口菜单
    if (play_sta == ENGINE_STATE_NULL)
    {
        op_menu.menu_entry = BROWSER_MENU_ENTRY_NOPLAY;
    }
    else if (play_sta == ENGINE_STATE_PLAYING)
    {
        op_menu.menu_entry = BROWSER_MENU_ENTRY_PLAYING;
    }
    else
    {
        op_menu.menu_entry = BROWSER_MENU_ENTRY_LASTPLAY;
    }

    op_menu.app_id = APP_ID_BROWSER;
    //一个ap可能有多个菜单场景，为每个菜单场景分配独立的路径记忆
    op_menu.path_id = 1;
    op_menu.menu_func = NULL;
    op_menu.browse_mode = 0;

    result = gui_menulist(MENULIST_OPTION, &op_menu);

    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser 菜单场景处理模块
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_scene_menu
 * \note
 * \li  显示主盘目录和插卡目录
 *******************************************************************************/
app_result_e browser_scene_menu(void)
{
    app_result_e result;

    menu_com_data_t browser_menu;

    browser_menu.app_id = APP_ID_BROWSER;

    //browser仅有此入口菜单
    browser_menu.menu_entry = BROWSER_MENU_ENTRY_CARD;
    browser_menu.path_id = 0;
    browser_menu.menu_func = NULL;
    browser_menu.browse_mode = 0;

    //需要显示菜单项
    result = gui_menulist(MENULIST, &browser_menu);
    if (result == RESULT_REDRAW)
    {
        result = RESULT_BROWSER_EXIT;
    }
    return result;
}

