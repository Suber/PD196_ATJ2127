/*******************************************************************************
 *                              US212A
 *                            Module: Video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     video
 * \brief    video的filelist函数模块，调用common_ui的控件来显示文件列表信息
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "video.h"

/*全局变量定义*/

/*常量定义*/

/******************************************************************************/
/*!
 * \par  Description:
 *	  视频文件列表option回调函数,该返回值在场景循环中再做判断
 * \param[in]    path_info 当前文件的path_info,这里不需要
 * \param[in]    active    当前文件在列表序号，这里不需要
 * \param[out]   path_info
 * \return       app_result_e
 * \retval       RESULT_VIDEO_MENU 返回到option菜单场景
 * \ingroup      video_filelist
 * \note
 *******************************************************************************/
static app_result_e _video_option_callback(file_path_info_t *path_info, uint16 active)
{
    return RESULT_VIDEO_OPTIONMENU;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  视频文件列表模块
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MAIN_APP  回到主应用
 * \retval       RESULT_VIDEO_MENU 返回video菜单场景
 * \retval       RESULT_VIDEO_OPTIONMENU 返回video option菜单场景
 * \ingroup      video_filelist
 * \note
 *******************************************************************************/
app_result_e video_filelist(void)
{

    app_result_e result;
    dir_com_data_t video_dir_data;

    if (g_video_var.path.file_path.dirlocation.filename[0] == 0)
    {
        video_dir_data.browse_mode = 0;
    }
    else
    {
        video_dir_data.browse_mode = 1;
    }

    if(g_del_file == TRUE)
    {
        g_del_file = FALSE;
        video_dir_data.del_back = TRUE;
    }

    ui_clear_screen(NULL);
    //文件列表不含菜单项
    video_dir_data.menulist_cnt = 0;
    video_dir_data.root_layer = 0;
    //option键回调函数
    video_dir_data.list_option_func = _video_option_callback;
    //菜单列表，为空
    video_dir_data.menulist = NULL;

    result = gui_directory(DIRLIST, &g_video_var.path, &video_dir_data);

    switch(result)
    {
        case RESULT_REDRAW:
        //从filelist按return键需判断是否存在卡盘
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            result = RESULT_MAIN_APP;
        }
        else
        {
            result = RESULT_VIDEO_MENU;
        }  
        break;

        case RESULT_DIR_ERROR_ENTER_DIR:
        //控件错误
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        break;

        case RESULT_DIR_ERROR_NO_FILE:
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_VIDEO_FILE);
        break;

        default:
        break;
    }
    
    return result;
}


