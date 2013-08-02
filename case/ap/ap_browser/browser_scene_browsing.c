/*******************************************************************************
 *                              US212A
 *                            Module: BROWSER_SCENE_BROWSING
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-8-24 9:40:29           1.0             build this file
 *******************************************************************************/
/*!
 * \file     browser_scene_browsing.c
 * \brief    browser列表场景模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2011-8-24
 *******************************************************************************/
#include "browser.h"
#include "browser_menu_cfg.h"

#define BROWSER_START_INITIAL_LAYER 0   //从初始目录开始浏览
#define BROWSER_START_DEST_LAYER 1   //从目标层级开始浏览

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

static const uint8 record_short_name[] = "RECORD     ";
/******************************************************************************/
/*!
 * \par  Description:
 *    browser 显示消息提示
 * \param[in]    none
 * \param[out]   none
 * \return       bool 是否成功进入record目录
 * \retval       TRUE 进入record目录
 * \retval       FALSE 进入失败
 * \ingroup      browser_scene_browsing
 * \note
 * \li   为提高操作效率，这里先使用了fsel接口函数进入指定文件名目录，
 * 	     然后使用bs接口获取record目录相关信息
 *******************************************************************************/
bool _enter_record(void)
{
    bool result = TRUE;
    uint16 str_id;
    dir_brow_t brow;

    brow.name_len = 0;

    fsel_enter_dir(CD_ROOT, NULL);

    //先找长名目录
    if (fsel_enter_dir(CD_SUB, (char *)record_name) == TRUE)
    {
        fsel_browser_enter_dir(CUR_DIR, 0, &brow);
        if ((brow.dir_total + brow.file_total) == 0)
        {
            result = FALSE;
        }
    }
    else
    {
        //找不到长名的RECORD目录，寻找RECORD短文件名目录
        if (fsel_enter_dir(CD_SUB, (char *)record_short_name) == TRUE)
        {
            fsel_browser_enter_dir(CUR_DIR, 0, &brow);
            if ((brow.dir_total + brow.file_total) == 0)
            {
                result = FALSE;
            }
        }
        else
        {
            result = FALSE;
        }
    }

    if (result == FALSE)
    {
        if (g_browser_enter_mode == PARAM_FROM_RADIO)
        {
            str_id = S_NO_RADIO_FILE;
        }
        else
        {
            str_id = S_NO_RECORD_FILE;
        }
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, str_id);
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser 列表场景处理模块
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e  the result
 * \ingroup      browser_scene_browsing
 * \note
 * \li   返回选择文件对应的返回值
 * \li   返回出错结果和热键返回值
 *******************************************************************************/
app_result_e browser_scene_filelist(void)
{
    uint16 str_id;
    app_result_e result;
    dir_com_data_t browser_dir_data;

    //嵌套菜单结构
    list_menu_t menu_item_insert;

    //默认的browser文件列表是没有嵌套菜单的
    browser_dir_data.root_layer = 0;
    browser_dir_data.menulist = NULL;
    browser_dir_data.menulist_cnt = 0;

    if (g_browser_enter_mode == PARAM_FROM_MAINMENU)
    {
        browser_dir_data.browse_mode = BROWSER_START_INITIAL_LAYER;
    }
    else if ((g_browser_enter_mode == PARAM_FROM_RECORD) || (g_browser_enter_mode == PARAM_FROM_RADIO))
    {
        retry:
        //从record目录即第一层目录开始浏览
        browser_dir_data.root_layer = 1;
        browser_dir_data.browse_mode = BROWSER_START_INITIAL_LAYER;
        if (_enter_record() == FALSE)
        {
            return RESULT_BROWSER_EXIT;
        }

        //显示嵌套的菜单项"全部删除"
        menu_item_insert.app_id = APP_ID_BROWSER;
        menu_item_insert.layer = 1;
        menu_item_insert.list_menu = BROWSER_MENU_ENTRY_DEL_ALL;
        browser_dir_data.menulist = &menu_item_insert;
        browser_dir_data.menulist_cnt = 1;
    }
    else
    {
        //从其它ap返回browser应用的文件列表界面
        browser_dir_data.browse_mode = BROWSER_START_DEST_LAYER;

        //之前是从radio或record进入
        if ((g_browser_var.prev_enter_mode == PARAM_FROM_RECORD) || (g_browser_var.prev_enter_mode == PARAM_FROM_RADIO))
        {
            //如果之前不是从mainmenu进入，即从record或FM进入，则
            //在文件列表显示"全部删除"菜单项
            browser_dir_data.root_layer = 1;
            //嵌套菜单属于browser ap
            menu_item_insert.app_id = APP_ID_BROWSER;
            //嵌套菜单在文件列表layer为1
            menu_item_insert.layer = 1;
            //嵌套菜单的入口菜单编号
            menu_item_insert.list_menu = BROWSER_MENU_ENTRY_DEL_ALL;
            browser_dir_data.menulist = &menu_item_insert;
            browser_dir_data.menulist_cnt = 1;
        }
    }

    //文件列表option键回调函数
    //browser_dir_data.list_option_func = _browser_show_information;
    browser_dir_data.list_option_func = browser_menu_option;

    result = gui_directory(DIRLIST, &g_browser_var.path, &browser_dir_data);

    if (result == RESULT_REDRAW)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            result = RESULT_BROWSER_EXIT;
        }
        else
        {
            result = RESULT_BROWSER_MENU;
        }
    }
    else if (result == RESULT_DIR_ERROR_NO_FILE)
    {
        if (g_browser_var.path.file_path.dirlocation.disk == DISK_BASE)
        {
            str_id = S_DISK_NOFILE;
        }
        else
        {
            str_id = S_CARD_NOFILE;
        }
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, str_id);
    }
    else if (result == RESULT_DIR_ERROR_ENTER_DIR)
    {
        //gui_directory()控件出错
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        result = RESULT_BROWSER_EXIT;
    }
    else if (result == RESULT_DIR_ERROR_SETLOC)
    {
        //当前文件location设置失败，获取当前目录下第一个文件
        goto retry;
    }
    else
    {

    }

    return result;
}

