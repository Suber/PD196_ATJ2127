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
 * \file     config_charging
 * \brief    关机后充电的处理，包括充电状态的显示，满电的监测以及按键，拔线的处理
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "ap_config.h"

const key_map_t charge_key_map_list[] =
{
    { { KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_CONFIG_KEY_UP },
    { { KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_CONFIG_KEY_HOLD },
    { { KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD}, EVENT_CONFIG_KEY_HOLD },
    { { KEY_MODE, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_CONFIG_KEY_HOLD },
    { { KEY_PLAY, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_CONFIG_KEY_HOLD },
    { { KEY_VOL, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_CONFIG_KEY_HOLD },
    { { KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};
/******************************************************************************/
/*!
 * \par  Description:
 *  standby，目前还是跑高频
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
int standby(void)
{
    uint32 temp_flag;
    uint32 bak_clk;
    uint32 bak_mrcr;
    int ret_value = 0;

    bak_clk = act_readl(CLKENCTL);
    bak_mrcr = act_readl(MRCR);

    //考虑功耗的话，还需要关闭一些模块
    act_writel(0x00000c00, CLKENCTL); //disable all module clk, except USB

    temp_flag = sys_local_irq_save();

    while (1)
    {
        //拔线断电
        if ((act_readb(SYSTEM_VOL) & 0x80) == 0)
        {
            switch_off();
        }
        //play键，退出循环
        if ((act_readl(SYSTEM_CTL_RTCVDD) & 0x80000000) != 0)
        {
            //等待抬键
            while ((act_readl(SYSTEM_CTL_RTCVDD) & 0x80000000) != 0)
            {
                ;//do nothing
            }
            ret_value = GUI_OUTOF_STANDBY;
            break;
        }
        //其他按键，退出循环
        if ((act_readb(LRADC1_DATA) & 0x3f) != 0x3f)
        {
            //等待抬键
            while ((act_readb(LRADC1_DATA) & 0x3f) != 0x3f)
            {
                ;//do nothing
            }
            ret_value = GUI_OUTOF_STANDBY;
            break;
        }
        //alarm到来
        if ((act_readl(RTC_CTL) & 0x00000001) == 1)
        {
            //清pending
            act_writel(act_readl(RTC_CTL), RTC_CTL);
            ret_value = ALARM_OUTOF_STANDBY;
            break;
        }
    }

    act_writel(bak_clk, CLKENCTL);
    //act_writel(bak_mrcr, MRCR);
    sys_local_irq_restore(temp_flag);
    return ret_value;
}

/******************************************************************************/
/*!
 * \par  Description:
 *  判断是否充满电，显示充电状态
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void _display_charging(void)
{
    picbox_private_t temp_pic;
    style_infor_t temp_sty;

    if ((uint32) key_chargeget(0) == CHARGE_FULL)
    {
        //充满电，亮背光30s
        if (charge_full_flag == FALSE)
        {
            charge_frame_id = BATTERY_GRADE_MAX - 1;
            lcd_standby_screen(TRUE);
            lcd_backlight_on_off(TRUE);
            backlight_flag = BACKLIGHT_ON;
            backlight_cnt = 0;
            charge_full_flag = TRUE;
        }
    }
    else
    {
        charge_frame_id++;
        if (charge_frame_id >= BATTERY_GRADE_MAX)
        {
            charge_frame_id = 0;
        }
    }

    //背光亮时显示充电图标
    if (backlight_flag == BACKLIGHT_ON)
    {
        //显示充电图标
        temp_sty.style_id = STY_CFG_CHARGING;
        temp_sty.type = UI_AP;
        temp_pic.pic_id = V_U16_INVALID;
        temp_pic.frame_id = charge_frame_id;
        ui_show_picbox(&temp_sty, &temp_pic);

        //背光计时，>30s关背光
        backlight_cnt++;
        if (backlight_cnt >= 30)
        {
            backlight_flag = BACKLIGHT_OFF;
            lcd_backlight_on_off(FALSE);
            lcd_standby_screen(FALSE);
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *  充电的处理
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
void _config_charging(void)
{
    input_gui_msg_t gui_msg;
    style_infor_t temp_sty;
    private_msg_t private_msg;
    int standby_result;
    uint8 temp_mode = PARAM_ENTER_NORMAL;
    msg_apps_type_e gui_event;

    ui_res_open("config.sty", UI_AP);

    //画背景
    temp_sty.style_id = STY_CFG_BACKGROUND;
    temp_sty.type = UI_AP;
    ui_show_picbox(&temp_sty, NULL);

    //设定1s的定时器，用于更新界面和判断是否满电
    timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, (timer_proc) _display_charging);

    while (1)
    {
        //任意按键唤醒
        if (get_gui_msg(&gui_msg) == TRUE)
        {
            if (com_key_mapping(&gui_msg, &gui_event, charge_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_CONFIG_KEY_HOLD)
                {
                    //清除按键抬起消息
                    com_filter_key_hold();
                }
                //如果背光关，只亮背光
                if (backlight_flag == BACKLIGHT_OFF)
                {
                    lcd_standby_screen(TRUE);
                    lcd_backlight_on_off(TRUE);
                    backlight_flag = BACKLIGHT_ON;
                    //背光计时器设回0
                    backlight_cnt = 0;

                }
                else
                {
                    //退出循环
                    break;
                }
            }
        }
        if ((charge_full_flag == TRUE) && (backlight_cnt >= 30))
        {
            //满电并且背光已经亮起30s，不需要再开定时器
            if (timer_id != -1)
            {
                kill_app_timer(timer_id);
                timer_id = -1;
            }
            //进入standby，一直循环
            standby_result = standby();
            //alarm消息起来
            if (standby_result == ALARM_OUTOF_STANDBY)
            {
                temp_mode = PARAM_ENTER_ALARM;
                break;
            }
            else
            {
                //由于按键从standby起来，重新启动定时器，显示充电满图标
                lcd_standby_screen(TRUE);
                lcd_backlight_on_off(TRUE);
                backlight_flag = BACKLIGHT_ON;
                backlight_cnt = 0;
                timer_id = set_app_timer(APP_TIMER_ATTRB_UI, 1000, (timer_proc) _display_charging);
            }
        }
        //拔usb线，关机断电
        if ((act_readb(SYSTEM_VOL) & 0x80) == 0)
        {
            if (timer_id != -1)
            {
                kill_app_timer(timer_id);
                timer_id = -1;
            }
            switch_off();
        }
        //alarm消息处理
        if (get_app_msg(&private_msg) == TRUE)
        {
            if (private_msg.msg.type == MSG_RTCALARM)
            {
                temp_mode = PARAM_ENTER_ALARM;
                break;
            }
        }
    }

    if (timer_id != -1)
    {
        kill_app_timer(timer_id);
        timer_id = -1;
    }
    ui_res_close(UI_AP);
    if (backlight_flag == BACKLIGHT_OFF)
    {
        lcd_standby_screen(TRUE);
        lcd_backlight_on_off(TRUE);
    }
    _creat_application(temp_mode);
}
