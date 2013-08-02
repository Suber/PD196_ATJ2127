/*******************************************************************************
 *                              US212A
 *                            Module: Video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-15 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     video_menu.c
 * \brief    video 的菜单列表模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include  "video.h"
#include  "video_menu_cfg.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  video 是否创建播放列表提示框
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \ingroup      video_menu
 * \note
 *******************************************************************************/
static app_result_e ask_for_creat_playlist(void)
{
    app_result_e result;
    dialog_com_data_t ask_dialog_data;
    string_desc_t ask_create_str;

    uint8 old_disk = g_video_var.path.file_path.dirlocation.disk;

    //为响应卡拔出消息，先临时更改盘符
    g_video_var.path.file_path.dirlocation.disk = DISK_H;

    //对话框是否创建播放列表
    ask_create_str.data.id = S_IS_CREATE_PLAYLIST;
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

    g_video_var.path.file_path.dirlocation.disk = old_disk;
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    切换盘符函数
 * \param[in]    disk_flag 要切换的盘符
 * \param[out]   none
 * \return       the result
 * \ingroup      video_menu
 * \note         如果需要换盘符，需要关闭后台引擎(是否只需要关闭music?),重新初始化
 *               文件选择器
 *******************************************************************************/
static app_result_e change_disk(uint8 disk_flag)
{
    //    fsel_error_e fsel_error;
    app_result_e result = RESULT_VIDEO_FILELIST;

    if (g_video_var.path.file_path.dirlocation.disk != disk_flag)
    {
        //关闭后台引擎
        music_close_engine();
        //退出文件选择器
        fsel_exit();
        //切换盘符初始化location到默认值
        libc_memset(&g_video_var.path.file_path, 0, sizeof(musfile_location_u));
        g_video_var.path.file_path.plist_location.disk = disk_flag;

        result = video_disk_initial();

        if (result == RESULT_NULL)
        {
            //初始化成功，进入播放列表浏览
            result = RESULT_VIDEO_FILELIST;
        }
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    主盘目录菜单项执行函数
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result
 * \ingroup      video_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_view_main_disk(void *param)
{
    return change_disk(DISK_C);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    插卡目录菜单项执行函数
 * \param[in]    param 无实际意义
 * \param[out]   none
 * \return       the result
 * \ingroup      video_menu
 * \note
 *******************************************************************************/
app_result_e menu_func_view_card_disk(void *param)
{
    app_result_e result;

    if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
    {
        if (get_card_plist() == FALSE)
        {
            result = ask_for_creat_playlist();
            if (result == RESULT_DIALOG_YES)
            {
                //确认的情况
                g_video_var.path.file_path.dirlocation.disk = DISK_H;
                result = RESULT_CREATE_LIST_APP;
            }
            else if (result == RESULT_DIALOG_NO)
            {
                //取消情况
                result = change_disk(DISK_H);
            }
            else
            {
                ;//do nothing 系统消息退出
            }
        }
        else
        {
            result = change_disk(DISK_H);
        }
    }
    else
    {
        //提示语
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
        //重新进入图片文件列表
        result = RESULT_VIDEO_FILELIST;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    菜单场景函数 有卡存在时才会出现该场景
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval       app_result_e, 菜单控件返回的结构消息
 * \ingroup      video_menu
 * \note
 *******************************************************************************/
app_result_e video_menu(void)
{
    app_result_e result;

    menu_com_data_t menu_data;

    //入口菜单
    menu_data.app_id = APP_ID_VIDEO;
    menu_data.menu_entry = VIDEO_CARD_MENU_ENTRY;
    menu_data.path_id = 1;
    menu_data.menu_func = NULL;
    menu_data.browse_mode = 0;

    //需要显示菜单项
    result = gui_menulist(MENULIST, &menu_data);
    if (result == RESULT_REDRAW)
    {
        result = RESULT_MAIN_APP;
    }
    return result;
}
