/*******************************************************************************
 *                              US212A
 *                            Module: Config
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     config_reclaim.c
 * \brief    nandflash reclaim 处理
 * \author   cailizhen
 * \version  1.0
 * \date     2012-9-4
 *******************************************************************************/
#include  "ap_config.h"

void config_reclaim(int argc)
{
    msg_apps_t config_msg;
    
    //发生 nandflash reclaim
    //1.安装 nandflash 驱动，然后关闭
    //安装存储驱动
    sys_drv_install(DRV_GROUP_STG_BASE, MODE_NORMAL, "nand.drv");
    sys_drv_uninstall(DRV_GROUP_STG_BASE);
    
    //2.恢复应用，music和video将续播，其他应用返回 mainmenu AP
    config_msg.type = MSG_CREAT_APP;
    
    if ((((uint32) argc >> 8) & 0xff) == APP_ID_MUSIC)
    {
        config_msg.content.data[0] = APP_ID_MUSIC;
        if((((uint32) argc >> 16) & 0xff) == ENGINE_STATE_PLAYING)
        {
            config_msg.content.data[1] = PARAM_BACKTO_PLAYING;
        }
        else
        {
            config_msg.content.data[1] = PARAM_BACKTO_PLAYED;
        }
    }
    else if ((((uint32) argc >> 8) & 0xff) == APP_ID_VIDEO)
    {
        config_msg.content.data[0] = APP_ID_VIDEO;
        if ((((uint32) argc >> 16) & 0xff) == ENGINE_STATE_PLAYING)
        {
            config_msg.content.data[1] = PARAM_BACKTO_PLAYING;
        }
        else
        {
            config_msg.content.data[1] = PARAM_BACKTO_PLAYED;
        }
    }
    else
    {
        config_msg.content.data[0] = APP_ID_MAINMENU;
        config_msg.content.data[1] = PARAM_FROM_CONFIG;
    }
    
    send_async_msg(APP_ID_MANAGER, &config_msg);
}
