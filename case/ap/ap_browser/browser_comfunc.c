/*******************************************************************************
 *                              US212A
 *                            Module: BROWSER_COMFUNC
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2012-3-7 9:16:42           1.0              build this file
 *******************************************************************************/
/*!
 * \file     browser_comfunc.c
 * \brief    browser公有模块函数
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2012-3-7
 *******************************************************************************/
#include "browser.h"

#define MAX_SUPPORT_STG_DEVICE 3

/*! \brief 支持三种物理存储介质的盘符 */
static const uint8 driver_type[MAX_SUPPORT_STG_DEVICE] =
{ DISK_C, DISK_H, DISK_U };

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser文件选择器初始化
 * \param[in]    uint8 disk_flag 盘符
 * \param[out]   none
 * \return       fsel_error_e 文件选择器初始化返回值
 * \ingroup      browser_comfunc
 * \note
 * \li  调用文件选择器接口初始化，并返回操作结果
 *******************************************************************************/
fsel_error_e browser_fsel_initial(uint8 disk_flag)
{
    uint8 i;
    fsel_param_t browser_fsel_param;
    fsel_error_e fsel_error = FSEL_NO_ERR;

    browser_fsel_param.fsel_type = FSEL_TYPE_COMMONDIR;
    browser_fsel_param.fsel_mode = FSEL_MODE_BROWSER;
    browser_fsel_param.brow_type = FSEL_BROW_ALL;
    browser_fsel_param.disk = disk_flag;
    browser_fsel_param.file_type_bit = ALLFILE_BITMAP;

    for (i = 0; i < MAX_SUPPORT_STG_DEVICE; i++)
    {
        if (disk_flag == driver_type[i])
        {
            break;
        }
    }

    if (i == MAX_SUPPORT_STG_DEVICE)
    {
        fsel_error = FSEL_ERR_INPUT;
    }
    else
    {
        browser_mount = fsel_init(&browser_fsel_param, 0);

        fsel_error = fsel_get_error();
    }
    return fsel_error;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser初始化文件选择器
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e 处理结果
 * \ingroup      browser_comfunc
 * \note
 * \li  属于对browser_fsel_initial()进一步封装，返回结果以app_result_e给出
 *******************************************************************************/
app_result_e browser_disk_initial(void)
{
    fsel_error_e fsel_error;
    app_result_e result;
    uint8 disk_flag = g_browser_var.path.file_path.dirlocation.disk;

    //初始化browser应用的文件选择器
    fsel_error = browser_fsel_initial(disk_flag);

    if (fsel_error == FSEL_NO_ERR)
    {
        result = RESULT_NULL;
    }
    else if ((fsel_error == FSEL_ERR_STG) || (fsel_error == FSEL_ERR_FS))
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);

        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //存在卡盘,先将盘符设置为非法值以便重新初始化文件选择器
            g_browser_var.path.file_path.dirlocation.disk = 0xff;
            result = RESULT_BROWSER_MENU;
        }
        else
        {
            //返回main ap
            result = RESULT_MAIN_APP;
        } 

    }
    else
    {
        result = RESULT_MAIN_APP;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    通知后台引擎退出
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      browser
 * \note
 *******************************************************************************/
void music_close_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;

    engine_type = get_engine_type();

    if (engine_type == ENGINE_MUSIC)
    {
        //向process manager发消息要求后台退出
        msg.type = MSG_KILL_APP_SYNC;

        msg.content.data[0] = APP_ID_MENGINE;

        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
    return;
}

