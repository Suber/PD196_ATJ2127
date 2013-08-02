/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_filelist.c
 * \brief    Picture的filelist函数模块，调用common_ui的控件来显示文件列表信息
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "picture.h"

/*全局变量定义*/
/*常量定义*/
/******************************************************************************/
/*!
 * \par  Description:
 *	  图片文件列表option回调函数,该返回值在场景循环中再做判断
 * \param[in]    path_info 当前文件的path_info,这里不需要
 * \param[in]    active    当前文件在列表序号，这里不需要
 * \param[out]   path_info
 * \return       app_result_e
 * \retval       RESULT_PIC_OPTIONMENU 返回到option菜单场景
 * \ingroup      pic_filelist
 * \note
 *******************************************************************************/
static app_result_e pic_option_callback(file_path_info_t *path_info, uint16 active)
{
    return RESULT_PIC_OPTIONMENU;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  图片文件列表场景，直接调用common的filelist控件
 * \param[in]    void
 * \param[out]   none
 * \return       the result filelist控件返回结果
 * \ingroup      pic_filelist
 * \note
 *******************************************************************************/
app_result_e pic_filelist(void)
{
    app_result_e result;
    dir_com_data_t pic_dir_data;

    if (g_picture_var.path.file_path.dirlocation.filename[0] == 0)
    {
        //从根目录开始浏览
        pic_dir_data.browse_mode = 0;
    }
    else
    {
        //从上次浏览退出的地方开始浏览
        pic_dir_data.browse_mode = 1;
    }

    if(g_del_file == TRUE)
    {
        g_del_file = FALSE;
        pic_dir_data.del_back = TRUE;
    }

    //pic文件列表不含菜单项
    pic_dir_data.menulist_cnt = 0;
    pic_dir_data.root_layer = 0;
    //option键回调函数
    pic_dir_data.list_option_func = pic_option_callback;
    //菜单列表，为空
    pic_dir_data.menulist = NULL;

    result = gui_directory(DIRLIST, &g_picture_var.path, &pic_dir_data);
    if (result == RESULT_REDRAW)
    {
        //从filelist按return键需判断是否存在卡盘
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            result = RESULT_MAIN_APP;
        }
        else
        {
            result = RESULT_PIC_MENU;
        }
    }
    else if (result == RESULT_DIR_ERROR_ENTER_DIR)
    {
        //控件错误
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        result = RESULT_MAIN_APP;
    }
    else if (result == RESULT_DIR_ERROR_NO_FILE)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PIC_FILE);
    }
    else
    {

    }
    return result;
}




