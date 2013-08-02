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
 * \file     config_main.c
 * \brief    开关机的处理
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

/*全局变量定义*/
//应用自己需要的软定时器个数
#define APP_TIMER_COUNT     1

/*!
 *  \brief
 *  app_timer_vector：应用软定时器数组指针，指向应用空间的软定时器数组
 */
app_timer_t conf_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];
//系统变量
setting_comval_t g_setting_comval;
//config应用变量
g_config_var_t g_config_var;
//wav录音掉电回写文件头结构体
record_writehead_t g_writehead;
//是否第一次上电标志
bool first_boot = FALSE;
//按键的配置定义
cfg_key_var_t g_cfg_key;
//充电图片id
uint8 charge_frame_id = 0;
//背光计数器
uint8 backlight_cnt = 0;
//背光标志
backlight_type_e backlight_flag = BACKLIGHT_ON;
//定时器id
int8 timer_id;
//满电标志
bool charge_full_flag = FALSE;
//u盘数据更新标志
uint32 update_flag = 0;

/*main中要填充这两个值，有系统人员分配*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
#endif
INT8U prio = AP_FRONT_LOW_PRIO;


/******************************************************************************/
/*!
 * \par  Description:
 *  系统消息处理函数
 * \param[in]    应用私有消息指针
 * \param[out]   消息结果
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
app_result_e config_msg_dispatch(private_msg_t *private_msg)
{
    app_result_e result = RESULT_NULL;

    switch (private_msg->msg.type)
    {
        /* USB B线插入消息 */
        case MSG_USB_STICK:
        {
            //显示USB 选项对话框
            result = gui_usbconnect();
            if (result == RESULT_USB_TRANS)
            {
                //创建 udisk ap
                msg_apps_t msg;

                if (g_setting_comval.g_comval.online_device == 0)
                {
                    msg.content.data[0] = APP_ID_UDISK;
                }
                else
                {
                    msg.content.data[0] = APP_ID_MTP;
                }
                msg.type = MSG_CREAT_APP; //给process manager发送创建其它进程消息
                msg.content.data[1] = (uint8) PARAM_FROM_MAINMENU;
                send_async_msg(APP_ID_MANAGER, &msg);

                result = RESULT_APP_QUIT;
            }
            else
            {
                ; //QAC need
            }
        }
        break;

        /* USB B线拔出消息 */
        case MSG_USB_UNSTICK:
        {
            //只在USB选项对话框中，或者非 udisk 应用充电播放中响应（在USB传输连接下由udisk 应用自己处理）
            //如果在USB选项对话框中
            if(usb_selecting_state == TRUE)
            {
                //返回 RESULT_USB_UNSTICK 即可（ui_usbconnect 将 RESULT_USB_UNSTICK 转为 RESULT_REDRAW 后退出对话框）
                result = RESULT_USB_UNSTICK;
            }

            g_sys_counter.battery_counter = 100;//强制读取充电状态和电池电量
        }
        break;

        /* RTC定时闹钟消息 */
        case MSG_RTCALARM:
        {
            //由tools 处理
            msg_apps_t msg;

            result = com_alarm_msg_dispatch();
            if (result == RESULT_ALARMFM)
            {
                msg.content.data[0] = APP_ID_RADIO;
            }
            else if (result == RESULT_FMRECORD)
            {
                msg.content.data[0] = APP_ID_RECORD;
            }
            else if (result == RESULT_ALARM)
            {
                msg.content.data[0] = APP_ID_ALARM;
            }
            else
            {
                break;
            }
            msg.type = MSG_CREAT_APP; //给process manager发送创建其它进程消息
            msg.content.data[1] = (uint8) PARAM_FROM_ALARM;
            send_async_msg(APP_ID_MANAGER, &msg);

            result = RESULT_APP_QUIT;
        }
        break;

        /* 系统低电消息 */
        case MSG_LOW_POWER:
        {
            style_infor_t style_infor;

            //显示低电图标
            style_infor.style_id = LOW_POWER_BG;
            style_infor.type = UI_COM;
            ui_show_picbox(&style_infor, NULL);

            //显示低电字符串
            style_infor.style_id = LOW_POWER_TEXT;
            style_infor.type = UI_AP;
            ui_show_textbox(&style_infor, NULL, TEXTBOX_DRAW_NORMAL);

            result = RESULT_ERROR;
        }
        break;

        /* 充电器 ADAPTOR 插入消息 */
        case MSG_ADAPTOR_IN:
        {
            //开启充电，但不进入充电UI
            key_chargeset(CHARGE_START, ChargeCurrent250mA, BATFULL_LEVEL1);

            g_sys_counter.battery_counter = 100;//强制读取充电状态和电池电量
        }
        break;

        /*耳机（天线）插入*/
        case MSG_EARPHONE_IN:
        {
            change_antenna_state(TRUE);
        }
        break;

        /*耳机（天线）拔出*/
        case MSG_EARPHONE_OUT:
        {
            change_antenna_state(FALSE);
        }
        break;

        default:
        break;
    }

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  读取应用VM全局变量,初始化全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
static void _read_var(void)
{
    uint8 i;
    uint16 temp_ap_file_num[MAX_APP_NUM];

    //读取common VM公共变量信息
    sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(g_setting_comval));
    //初始化系统的VM变量
    if (g_setting_comval.g_comval.magic != MAGIC_SETTING)
    {
        g_setting_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_comval);
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
    }

    com_set_sys_comval(&g_setting_comval.g_comval);

    sys_vm_read(&g_config_var, VM_AP_CONFIG, sizeof(g_config_var));
    //初始化config的VM变量
    if (g_config_var.magic != MAGIC_CONFIG)
    {
        //第一次上电
        first_boot = TRUE;
        g_config_var.magic = MAGIC_CONFIG;
        g_config_var.ap_id = APP_ID_MAINMENU;
        //获取按键配置
        com_get_config_struct(CFG_KEY_ARRAY, (uint8 *) &g_cfg_key, sizeof(g_cfg_key));
        for (i = 0; i < g_cfg_key.total; i++)
        {
            g_config_var.keyID[i] = (uint8) g_cfg_key.keyID[i];
        }
        sys_vm_write(&g_config_var, VM_AP_CONFIG);
        //将各应用文件总数写为0
        libc_memset(temp_ap_file_num, 0, sizeof(temp_ap_file_num));
        sys_vm_write(&temp_ap_file_num, VM_FILE_NUM);
    }
    //读取录音掉电信息
    sys_vm_read(&g_writehead, VM_AP_RECORD_WAV, sizeof(record_writehead_t));
    //u盘数据更新标志
    sys_vm_read(&update_flag, UDISK_VM, 4);
}


/******************************************************************************/
/*!
 * \par  Description:
 *  applib初始化
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
bool _app_init(void)
{
    //初始化applib库，前台AP
    applib_init(APP_ID_CONFIG, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(conf_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //初始化 applib 消息模块
    applib_message_init(config_msg_dispatch);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    config应用的入口函数
 * \param[in]    0-开机，其他值：关机，低16bit代表关机时的前台应用ID，高16bit表示后台引擎的状态
 * \param[out]   none
 * \return       the result
 * \retval
 * \retval
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    private_msg_t private_msg;
    app_result_e result = RESULT_NULL;
    date_t init_data;
    time_t init_time;
    bool sd_in_flag;
    bool usb_in_flag;
    bool alarm_on_flag;
    uint8* key_table_addr;
    input_gui_msg_t gui_msg;

    //应用变量初始化
    _read_var();
    //applib initial
    _app_init();
    //开机
    if (((uint32)argc & 0xff) == SWITCH_ON)
    {
        //安装按键驱动
        sys_drv_install(DRV_GROUP_KEY, 0, "key.drv");
        //配置按键值
        key_table_addr = key_getkeytabaddress();
        libc_memcpy(key_table_addr, g_config_var.keyID, sizeof(g_config_var.keyID));
        //安装显示驱动
        sys_drv_install(DRV_GROUP_LCD, 0, "drv_lcd.drv");
        sys_drv_install(DRV_GROUP_UI, 0, "drv_ui.drv");
        //设置语言
        ui_set_language(g_setting_comval.g_comval.language_id);
        //设对比度
        com_set_contrast(g_setting_comval.g_comval.lightness);
        //安装audio驱动
        sys_drv_install(DRV_GROUP_AUDIO_DEVICE, 0, "aud_dev.drv");
        //设置音量
        com_set_sound_volume(g_setting_comval.g_comval.volume_current);
        //设置按键音
        com_set_sound_keytone(g_setting_comval.g_comval.key_tone);

        //判断是否掉电，如是，初始化日期2012-1-1，时间00:00:00
        if ((act_readl(RTC_CTL) & 0x0010) == 0)
        {
            init_data.year = 2012;
            init_data.month = 1;
            init_data.day = 1;
            sys_set_date(&init_data);

            init_time.hour = 0;
            init_time.minute = 0;
            init_time.second = 0;
            sys_set_time(&init_time);
        }

        //显示开机动画
        if(g_config_var.ap_id != APP_ID_MUSIC)
        {
//            draw_logo(SWITCH_ON);
        }
        //接收并处理私有消息
        ui_res_open("common.sty", UI_COM);
        sd_in_flag = FALSE;
        usb_in_flag = FALSE;
        alarm_on_flag = FALSE;
        while (get_app_msg(&private_msg) == TRUE)
        {
            if (private_msg.msg.type == MSG_SD_IN)
            {
                sd_in_flag = TRUE;
                continue;
            }
            if (private_msg.msg.type == MSG_USB_STICK)
            {
                usb_in_flag = TRUE;
                continue;
            }
            if (private_msg.msg.type == MSG_RTCALARM)
            {
                alarm_on_flag = TRUE;
                sys_os_time_dly(50);//延时500ms，保证耳机插入消息已发送到系统消息队列中
                continue;
            }
            result = config_msg_dispatch(&private_msg);
            if ((result == RESULT_APP_QUIT) || (result == RESULT_ERROR))
            {
                break;
            }
        }
        //确认进入u盘
        if (usb_in_flag == TRUE)
        {
            private_msg.msg.type = MSG_USB_STICK;
            result = config_msg_dispatch(&private_msg);
        }
        //确认alarm消息开机
        else if(alarm_on_flag == TRUE)
        {
            private_msg.msg.type = MSG_RTCALARM;
            result = config_msg_dispatch(&private_msg);
        }
        else
        {
            ;//QAC need
        }
        ui_res_close(UI_COM);

        if (sd_in_flag == TRUE)
        {
            //因为前面获取消息时将sd card插入的消息取出来了，重新将sd card插入的消息发送回系统消息队列
            uint16 msgtype;

            msgtype = MSG_SD_IN;
            sys_mq_send(MQ_ID_SYS, &msgtype);
        }

        //录音掉电重写
        if (g_writehead.rewrite_need == TRUE)
        {
            write_head();
            g_writehead.rewrite_need = FALSE;
            sys_vm_write(&g_writehead, VM_AP_RECORD_WAV);
        }

        if (result == RESULT_APP_QUIT)
        {
            ;//在config_msg_dispatch函数中已经创建其他前台AP，直接退出即可
        }
        else if (result == RESULT_ERROR)
        {
            goto switch_off_here;
        }
        else
        {
            get_gui_msg(&gui_msg);
            if (gui_msg.data.kmsg.val == KEY_PLAY)
            {
                //this_filter_key_value = KEY_PLAY;
                com_filter_key_hold();
            }
            //创建ap
            _creat_application(PARAM_ENTER_NORMAL);
        }
    }
    //nandflash reclaim 重启
    else if(((uint32)argc & 0xff) == NANDFLASH_RECLAIM)
    {
        config_reclaim(argc);
    }
    else//以下是关机操作
    {
        if ((((uint32) argc >> 8) & 0xff) == APP_ID_RADIO)
        {
            g_config_var.ap_id = APP_ID_RADIO;
            g_config_var.engine_state = (uint8) (((uint32) argc >> 16) & 0xff);
        }
        else if ((((uint32) argc >> 8) & 0xff) == APP_ID_MUSIC)
        {
            g_config_var.ap_id = APP_ID_MUSIC;
            g_config_var.engine_state = (uint8) (((uint32) argc >> 16) & 0xff);
        }
        else if((((uint32) argc >> 8) & 0xff) == APP_ID_VIDEO)
        {
            g_config_var.ap_id = APP_ID_VIDEO;
            g_config_var.engine_state = (uint8) (((uint32) argc >> 16) & 0xff);
        }
        else
        {
            g_config_var.ap_id = APP_ID_MAINMENU;
        }
        //重新将关机时间设回0
        g_setting_comval.g_comval.sleep_timer = 0;
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
        sys_vm_write(&g_config_var, VM_AP_CONFIG);

        switch_off_here: //进入关机操作
        //如果关机时usb线连上，进入充电显示界面
        if ((act_readb(SYSTEM_VOL) & 0x80) != 0)
        {
            //if ((uint32)key_chargeget(0) == CHARGE_CHARGING)
            {
                _config_charging();
            }
        }
        else
        {
            switch_off();
        }
    }

    if (first_boot == TRUE)
    {
        _config_format_disk();
    }
    //清除菜单记录
    com_clear_all_history(PATH_HISTORY_MENU);

    //执行applib库的注销操作
    applib_quit();

    return result;
}
