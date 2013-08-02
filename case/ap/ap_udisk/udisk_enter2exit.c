/*******************************************************************************
 *                              US212A
 *                            Module: udiskap
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       yliang     2011-11-07      1.0             build this file
 *******************************************************************************/
/*!
 * \file     udisk_enter2exit.c
 * \brief    udisk init & exit deal
 * \author   yliang
 * \version  1.0
 * \date  2011/10/07
 *******************************************************************************/
#include  "ap_udisk.h"

/******************************************************************************/
/*!
 * \par  Description:
 *udisk ap init
 * \param[in]
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      main_udisk.c
 * \note
 *******************************************************************************/
void udisk_init_all(void)
{
    picbox_private_t picbox_data;
    textbox_private_t textbox_param;
    style_infor_t style_infor;
    setting_comval_t u_g_setting_vars;
    //bool  result=0;
    uint8 i = 0;
    ram_clk_to_mcu();
    /*显示连接界面*/
    style_infor.type = UI_AP;
    style_infor.style_id = UDISK_LINK_BG;
    ui_show_picbox(&style_infor, NULL);

  /*  style_infor.style_id = UDISK_MP3;
    ui_show_picbox(&style_infor, NULL);

    style_infor.style_id = UDISK_LINK_TYPE;
    picbox_data.pic_id = -1;
    picbox_data.frame_id = 0;
    ui_show_picbox(&style_infor, &picbox_data);
*/
   /* for (i = 0; i < 5; i++)
    {
        style_infor.style_id = style_usb_point[i];
        picbox_data.pic_id = -1;
        picbox_data.frame_id = 0;
        //显示界面
        ui_show_picbox(&style_infor, &picbox_data);
    }
    if (timer_usb_connect_id == -1)
    {
        timer_usb_connect_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, timer_usb_display_connect);
    }
    style_infor.style_id = UDISK_STATE_STR;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_id = S_UDISK_LINKING;
    textbox_param.str_value = NULL;
    ui_show_textbox(&style_infor, &textbox_param, TEXTBOX_DRAW_NORMAL);
*/
    ui_show_battery();
    //保存需要动态刷新的图片到内存
    //read_connect_pic_to_ram();
    read_battery_pic_to_ram();
    //切换到LCD_BUFFER
    ui_set_display_buf(LCD_RAM_INDEX);

    //直接安装base驱动,不用跟VFS打交道
    if (-1 == sys_drv_install(DRV_GROUP_STG_BASE, 0, "nand.drv"))
    {
        while (1)
        {
            ;//死循环
        }
    }
    //Udisk buffer与flash驱动cache冲突,更改flash驱动写cache地址,必须先安装flash驱动才能调用
    base_op_entry((void*) 1, (void*) 0x9FC3B000, 0, BASE_UPDATE);

    //==================================================================================
    //**********************以下函数调用次序不能改动***************************************
    //==================================================================================
    //UDiskSetDiskNum(g_setting_comval.g_comval.online_device);       //设置联机模式
    UDiskSetDiskNum(0);
    //AlwaysShowCardDisk(g_setting_comval.g_comval.support_card);      //设置是否显示多盘符  //reagan modify 2009-1-6
//    AlwaysShowCardDisk(1);
    //U盘初始化,输入参数:callback函数,是否支持Udisk序列号(随机数,每个Udisk均不一致),是否是Udisk认证模式
    sys_vm_read(&u_g_setting_vars, VM_AP_SETTING, sizeof(setting_comval_t));
    AutoRunSet = u_g_setting_vars.g_comval.autorun_set;
    usbinit(NULL, 1);
    usb_setup(); //启动USB引擎
    //return 1;
}

app_result_e udisk_exit_all(int argc)
{
    app_result_e ret = 0;
    msg_apps_t msg;
    void (*p_adfu_launcher)(void) = 0xbfc004b9;
    udisk_exit();
    /*读取VM信息*/
    sys_vm_read(&update_flag, UDISK_VM, 4);
    //modify--not creat playlist
    update_flag &= 0xfffffffe; //清除flash盘写标志
    update_flag &= 0xfffeffff; //清除卡盘写标志
    msg.type = MSG_CREAT_APP; //创建播放列表app
    //按键退出
    if (usbstatus == 0x00)
    {
        if (returnkey_press == 1)
        {
            goto exit_0;
        }

    }
    else if (usbstatus == 0x70)
    {
        goto exit_0;
    }
    else
    {
        //拔线,弹出
        goto exit_0;
    }

    exit_0:
    //创建playlist
    if (((update_flag & 0x00000001) != 0) || ((update_flag & 0x00010000) != 0))
    {
        ret = RESULT_CREATE_LIST_APP;
        msg.content.data[0] = (uint8)APP_ID_PLAYLIST;//playlist ap
        msg.content.data[1] = (uint8)PARAM_FROM_UDISK;
        if ((update_flag & 0x00000001) != 0)
        {
            msg.content.data[1] |= (uint8)PLIST_DISK_C;//playlist ap param
            update_flag &= 0xfffffffe; //清除flash盘写标志
        }
        if ((update_flag & 0x00010000) != 0)
        {
            msg.content.data[1] |= (uint8)PLIST_DISK_H;//playlist ap param
            update_flag &= 0xfffeffff; //清除卡盘写标志
        }
        sys_vm_write(&update_flag, UDISK_VM);
    }
    //返回上一次的AP
    else
    {
        ret = RESULT_LASTPLAY;
        if ((argc & 0x00ff) == APP_ID_RADIO)
        {
            msg.content.data[0] = APP_ID_RADIO;
            //直接回radio播放.  传参为PARAM_BACKTO_PLAYING时不会重设频率
            if ((((uint32) argc >> 16) & 0x00ff) == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_FROM_CONFIG; //PARAM_BACKTO_PLAYING;
            }
            else
            {
                msg.content.data[1] = PARAM_FROM_CONFIG; //PARAM_BACKTO_PLAYED;
            }
        }
        else if ((argc & 0x00ff) == APP_ID_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;
            if ((((uint32) argc >> 16) & 0x00ff) == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
            else
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYED;
            }
        }
        else if ((argc & 0x00ff) == APP_ID_VIDEO)
        {
            msg.content.data[0] = APP_ID_VIDEO;
            if ((((uint32) argc >> 16) & 0x00ff) == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
            else
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYED;
            }
        }       
        else
        {
            msg.content.data[0] = APP_ID_MAINMENU;
            msg.content.data[1] = PARAM_FROM_UDISK;
        }
    }
    send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建播放列表app的进程
    //exit:
    //恢复flash驱动写cache地址,必须在flash驱动卸载前才能调用
    base_op_entry((void*) 1, (void*) 0x9FC32000, 0, BASE_UPDATE);
    sys_drv_uninstall(DRV_GROUP_STG_BASE);

    if (0x51 == usbstatus)
    {
        sys_adjust_clk(FREQ_24M, 0);
        /* rise vcc 3.3v and vdd 1.3v, improve usb signal.   xiaomaky.   2012-9-11 11:55.  */
        act_writel(((act_readl(VOUT_CTL)&0xFFFFFF80)|0x6A), VOUT_CTL);
        sys_udelay(200);
        ui_set_pen_color(Color_WHITE_def);
        ui_set_backgd_color(Color_BLACK_def);
        ui_clear_screen(NULL);
        act_writel(0, INTC_CFG0);
        act_writel(0, INTC_CFG1);
        act_writel(0, INTC_CFG2); //assgin usb intrrupt to IP2
        act_writel(0x58, WD_CTL);
        p_adfu_launcher();
    }
    return ret;
}

