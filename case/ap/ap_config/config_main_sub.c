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
 * \file     config_main_sub.c
 * \brief    开关机的处理
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

//config应用变量
extern g_config_var_t g_config_var;
//是否第一次上电标志
extern bool first_boot;
//u盘数据更新标志
extern uint32 update_flag;

static const uint8 nand_drv_name[] = "nand.drv";
static const uint8 card_drv_name[] = "card.drv";
static const uint8 uhost_drv_name[] = "uhost.drv";

/******************************************************************************/
/*!
 * \par  Description:
 *  装载物理介质和文件系统
 * \param[in]    disk_type 磁盘类型
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
int32 _config_fs_init(uint8 disk_type)
{
    uint8 drv_type;
    uint8 *drv_name;
    int32 file_sys_id;

    if (disk_type == DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
        drv_name = (uint8 *) nand_drv_name;
    }
    else if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
        drv_name = (uint8 *) card_drv_name;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
        drv_name = (uint8 *) uhost_drv_name;
    }
    else
    {
        return -1;
    }

    //装载物理介质
    if (sys_drv_install(drv_type, 0, drv_name) != 0)
    {
        return -2;
    }

    //装载文件系统
    file_sys_id = sys_mount_fs(drv_type, disk_type, 0);

    if (file_sys_id == -1)
    {
        //未部署文件系统，格式化磁盘
        sys_format_disk(DRV_GROUP_STG_BASE, DISK_C, FORMAT_FAT32);

        //装载文件系统
        file_sys_id = sys_mount_fs(drv_type, disk_type, 0);

        if (file_sys_id == -1)
        {
            return -3;
        }
    }

    return file_sys_id;
}
/******************************************************************************/
/*!
 * \par  Description:
 *  卸载物理介质和文件系统
 * \param[in]    disk_type 磁盘类型
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
bool _config_fs_deinit(uint8 disk_type, int32 file_sys_id)
{
    int32 ret_val;
    uint8 drv_type;

    if (file_sys_id != -1)
    {
        ret_val = sys_unmount_fs(file_sys_id);
        file_sys_id = -1;
    }
    else
    {
        return TRUE;
    }

    if (0 > ret_val)
    {
        return FALSE;
    }

    if (disk_type == DISK_C)
    {
        drv_type = DRV_GROUP_STG_BASE;
    }
    else if (disk_type == DISK_H)
    {
        drv_type = DRV_GROUP_STG_CARD;
    }
    else if (disk_type == DISK_U)
    {
        drv_type = DRV_GROUP_STG_UHOST;
    }
    else
    {
        return FALSE;
    }

    ret_val = sys_drv_uninstall(drv_type);

    if (0 > ret_val)
    {
        return FALSE;
    }

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * 创建应用
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
bool _config_format_disk(void)
{
    bool ret_val;
    int32 file_sys_id;
    uint8 volume_label[28];

    libc_memset(volume_label, 0, sizeof(volume_label));

    //装载主盘
    file_sys_id = _config_fs_init(DISK_C);
    if (file_sys_id < 0)
    {
        return FALSE;
    }

    //从配置项读取卷标名字,最多支持11个英文字符
    com_get_config_struct(INF_UDISK_LABEL, volume_label, 11); //CMD_Inquiry_information_FOB

    //短名转换成长名形式
    com_ansi_to_unicode(volume_label);

    //统一由长名创建卷标名
    ret_val = vfs_create_volume(file_sys_id, volume_label);

    _config_fs_deinit(DISK_C, file_sys_id);

    return ret_val;
}

/******************************************************************************/
/*!
 * \par  Description:
 * 创建应用
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void _creat_application(uint8 param)
{
    msg_apps_t config_msg;
    app_result_e result = RESULT_NULL;

    if (param == PARAM_ENTER_ALARM)
    {
        result = com_alarm_msg_dispatch();
        if (result == RESULT_ALARMFM)
        {
            config_msg.content.data[0] = APP_ID_RADIO;
        }
        else if (result == RESULT_FMRECORD)
        {
            config_msg.content.data[0] = APP_ID_RECORD;
        }
        else if (result == RESULT_ALARM)
        {
            config_msg.content.data[0] = APP_ID_ALARM;
        }
        else
        {
            ;//do nothing
        }
        config_msg.type = MSG_CREAT_APP; //给process manager发送创建其它进程消息
        config_msg.content.data[1] = (uint8) PARAM_FROM_ALARM;
        send_async_msg(APP_ID_MANAGER, &config_msg);
        return;
    }

    //判断是否需要更新播放列表
    if (((update_flag & 0x00000001) != 0) || ((update_flag & 0x00010000) != 0))
    {
        config_msg.type = MSG_CREAT_APP;
        config_msg.content.data[0] = APP_ID_PLAYLIST;//playlist ap
        config_msg.content.data[1] = PARAM_FROM_CONFIG;
        if ((update_flag & 0x00000001) != 0)
        {
            config_msg.content.data[1] |= (uint8) PLIST_DISK_C;//playlist ap param
            update_flag &= 0xfffffffe; //清除flash盘写标志
        }
        if ((update_flag & 0x00010000) != 0)
        {
            config_msg.content.data[1] |= PLIST_DISK_H;//playlist ap param
            update_flag &= 0xfffeffff; //清除卡盘写标志
        }
        sys_vm_write(&update_flag, UDISK_VM);
    }
    else
    {
        //第一次开机，创建播放列表
        if (first_boot == TRUE)
        {
            config_msg.type = MSG_CREAT_APP;
            config_msg.content.data[0] = APP_ID_PLAYLIST;
            config_msg.content.data[1] = PARAM_FROM_CONFIG | PLIST_DISK_ALL;
        }
        else
        {
            //根据关机前的状态创建应用，目前做法是关机前有后台，即创建该后台对应的前台应用，如果无后台，进入主界面
            config_msg.type = MSG_CREAT_APP;
            config_msg.content.data[0] = g_config_var.ap_id;
            if (g_config_var.ap_id != APP_ID_MAINMENU)
            {
                if (g_config_var.ap_id == APP_ID_RADIO)
                {
                    //radio 进程, 直接传参播放
                    config_msg.content.data[1] = PARAM_FROM_CONFIG;
                }
                else if (g_config_var.engine_state == ENGINE_STATE_PLAYING)
                {
                    config_msg.content.data[1] = PARAM_BACKTO_PLAYING;
                }
                else
                {
                    config_msg.content.data[1] = PARAM_BACKTO_PLAYED;
                }
            }
        }
    }
    send_async_msg(APP_ID_MANAGER, &config_msg);
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  显示开关机动画
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void draw_logo(uint8 param)
{
    animation_com_data_t temp_animation;
    style_infor_t temp_sty;

    temp_animation.direction = 0;
    temp_animation.interval = 10;

    if (param == SWITCH_ON)
    {
        temp_sty.style_id = STY_POWER_ON;
    }
    else
    {
        temp_sty.style_id = STY_POWER_OFF;
    }
    temp_sty.type = UI_AP;
    ui_res_open("config.sty", UI_AP);
    gui_logo(&temp_sty, &temp_animation);
    ui_res_close(UI_AP);
}

/******************************************************************************/
/*!
 * \par  Description:
 *  关机函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void switch_off(void)
{
    //关机动画
    draw_logo(SWITCH_OFF);
    com_app_sleep(2, NULL);
    //关背光
    lcd_backlight_on_off(FALSE);
    lcd_standby_screen(FALSE);
    //配置长按键时间
    act_writel(act_readl(SYSTEM_CTL_RTCVDD) & (~(0x00000003 << SYSTEM_CTL_RTCVDD_SYSON_TIME_SHIFT)), SYSTEM_CTL_RTCVDD);
    //断电
    act_writel(act_readl(SYSTEM_CTL_RTCVDD) & (~(0x00000001 << SYSTEM_CTL_RTCVDD_REG_ENPMU)), SYSTEM_CTL_RTCVDD);

    /* wait for power off because of capacitance effect.  xiaomaky.  2013/1/25 11:38:32.  */
    act_writel(act_readl(WD_CTL) | 0x08, WD_CTL);
    while (1)
    {
        ;//nothing for QAC
    }
}
