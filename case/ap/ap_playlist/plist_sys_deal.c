/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    这里填写文件的概述
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

static const char stg_drv_name[][10] =
{ "nand.drv", "card.drv", "uhost.drv" };

/******************************************************************************/
/*!
 * \par  Description:
 *    通知后台引擎退出
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      video
 * \note
 *******************************************************************************/
static void close_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;
    engine_state_e engine_status;

    engine_status = get_engine_state();

    if (engine_status != ENGINE_STATE_NULL)
    {
        engine_type = get_engine_type();

        //向process manager发消息要求后台退出
        msg.type = MSG_KILL_APP_SYNC;
        if (engine_type == ENGINE_RADIO)
        {
            msg.content.data[0] = APP_ID_FMENGINE;
        }
        else if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MENGINE;
        }
        else
        {
            ;//bug
        }
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }

    return;
}

//退出ap处理
void _deal_playlist_result(app_param_e enter_mode)
{
    msg_apps_t msg;
    //    bool need_send_msg = TRUE;

    //给process manager发送创建其它进程消息
    msg.type = MSG_CREAT_APP;
    msg.content.data[1] = (uint8) PARAM_FROM_PLAYLIST;

    //退出前，清music的audible断点信息
    libc_memset(temp_plist_buf, 0, SECTOR_SIZE);
    sys_vm_write(temp_plist_buf, VM_AP_MUSIC_AUDBK);

    switch (enter_mode)
    {
        case PARAM_FROM_MUSIC:
        msg.content.data[0] = APP_ID_MUSIC;
        break;

        case PARAM_FROM_PICTURE:
        msg.content.data[0] = APP_ID_PICTURE;
        break;

        case PARAM_FROM_VIDEO:
        msg.content.data[0] = APP_ID_VIDEO;
        break;

        case PARAM_FROM_EBOOK:
        msg.content.data[0] = APP_ID_EBOOK;
        break;

        case PARAM_FROM_CONFIG:
        msg.content.data[0] = APP_ID_MAINMENU;
        break;

        case PARAM_FROM_UDISK:
        //需要同步MTP数据
        //        msg.content.data[0] = APP_ID_MTPSYNC;//APP_ID_MAINMENU
        msg.content.data[0] = APP_ID_MAINMENU;
        break;

        default:
        msg.content.data[0] = APP_ID_MAINMENU;
        break;
    }

    //    if (TRUE == need_send_msg)
    //    {
    //        //向process manager发送创建进程消息 同步消息还是异步消息
    //        send_async_msg(APP_ID_MANAGER, &msg);
    //    }

    send_async_msg(APP_ID_MANAGER, &msg);

    return;
}

//system enter init
void plist_sys_init(void)
{

    //先关闭后台
    close_engine();

    //安装nand存储驱动
    if (sys_drv_install(DRV_GROUP_STG_BASE, MODE_NORMAL, stg_drv_name[0]) != -1)
    {
        /****
         因为要用到nand默认的cache页面地址0x9fc34000来建表，
         所以需要把nand的cache切到别的页面
         ****/
#ifndef PC
        base_op_entry((void*) 1, (void*) NAND_CACHE_ADDR, 0, BASE_UPDATE);
#endif
        sys_drv_uninstall(DRV_GROUP_STG_BASE);
    }

}

//system enter exit
void plist_sys_exit(app_param_e enter_mode)
{

    _deal_playlist_result(enter_mode);

    // AP 退出前
    //重装nand驱动，恢复nand 的cache默认地址0x9fc34000

    //安装nand存储驱动
    if (sys_drv_install(DRV_GROUP_STG_BASE, MODE_NORMAL, stg_drv_name[0]) != -1)
    {
#ifndef PC
        //恢复nand的cache页面
        base_op_entry((void*) 1, (void*) NAND_DEFAULT_ADDR, 0, BASE_UPDATE);
#endif
        sys_drv_uninstall(DRV_GROUP_STG_BASE);
    }

}

