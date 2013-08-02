/*
 *******************************************************************************
 *                ACTOS AP
 *        system setting ap of sdk 3.0
 *
 *        (c) Copyright, Actions Co,Ld.
 *             All Right Reserved
 *
 *  $Id: main.c,v 1.7 2009/05/22 11:29:01 vicky Exp $
 *******************************************************************************
 */
#include "case_include.h"
#include "psp_includes.h"
#include "ap_alarm.h"

/*系统公共VM变量*/
comval_t g_comval;
//setting_comval_t g_setting_comval;
/*ALARM VM变量*/
alarm_alarm_t g_alarm;
/*ALARM_FM VM变量*/
alarm_fm_t g_alarmfm;
/**/
uint8 timer_flag;
/*临时保存snooze时间*/
time_t stimer_tmp;
/*播放状态信息*/
mengine_status_t g_music_status;

/*闹钟动画ID*/
uint8 alarm_pid = 0;
/*闹钟动画效果*/
bool g_alarm_pid = TRUE;
/*超时退出标志*/
bool g_alarm_timeout = FALSE;

/*定时器id*/
/*0.5s定时器*/
int8 g_halfsec_timer = 0xff;
/*30s定时器*/
int8 g_tirtysec_timer = 0xff;
/*闹钟超时计数*/
uint8 g_timer_60 = 0;

//应用自己需要的软定时器个数
#define APP_TIMER_COUNT        2
/*!
 *  \brief
 *  app_timer_vector：应用软定时器数组指针，指向应用空间的软定时器数组
 */
app_timer_t alarm_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
#endif

INT8U prio = AP_FRONT_LOW_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 * \
 * \处理gui消息
 * \param[in]    gui_msg  para1
 * \param[out]   none
 * \return       app_result_e result
 * \retval
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
app_result_e alarm_privmsg_deal(private_msg_t *private_msg)
{
    app_result_e result = RESULT_NULL;

    /*同步消息处理*/
    switch (private_msg->msg.type)
    {
        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(private_msg->msg.type);
        break;
    }//end of switch

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \读取VM变量信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool result
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
bool _load_cfg(void)
{
    alarm_vars_t temp_alarm_val;
    bool result;
    setting_comval_t temp_val;
    result = TRUE;
    //系统变量获取
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        temp_val.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    //alarm vm变量获取
    sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
    //防止变量未初始化被误保存
    timer_flag = temp_alarm_val.timer_flag;
    if (temp_alarm_val.magic != MAGIC_ALARM)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        result = FALSE;
    }
    libc_memcpy(&g_alarm, &temp_alarm_val.g_alarm, sizeof(alarm_alarm_t));
    libc_memcpy(&g_alarmfm, &temp_alarm_val.g_aufm, sizeof(alarm_fm_t)); //读出fm时间

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \保存VM变量信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    alarm_vars_t temp_alarm_val;
    sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
    //保存改变的alarm
    libc_memcpy(&temp_alarm_val.g_alarm, &g_alarm, sizeof(alarm_alarm_t));
    temp_alarm_val.timer_flag = timer_flag;
    libc_memcpy(&temp_alarm_val.snooze_time, &stimer_tmp, sizeof(time_t));
    sys_vm_write(&temp_alarm_val, VM_ALARM);
    //sys_vm_write(&g_setting_comval, VM_AP_SETTING);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \alarm ap的初始化
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool result
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
bool _app_init(void)
{
    bool app_init_ret = TRUE;

    //读取配置信息 (包括vm信息)
    if (_load_cfg() == FALSE)
    {
        app_init_ret = FALSE;
    }
    //加载ui资源文件
    ui_res_open("alarm.sty", UI_AP);
    ui_res_open("common.sty", UI_COM);

    //#ifdef PC
    //    load_menulist_simfunc(get_menu_func, get_callback_func, get_menu_option);
    //#endif

    //初始化applib库，前台AP
    applib_init(APP_ID_ALARM, APP_TYPE_GUI);

    applib_message_init(alarm_privmsg_deal);
    //初始化软定时器
    init_app_timers(alarm_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);
    //必须在init_app_timers 后面调用
    sys_timer_init();

    return app_init_ret;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \alarm ap退出处理
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //备份系统计时器
    sys_timer_exit();

    //执行applib库的注销操作
    applib_quit();

    //关闭应用菜单配置脚本
    com_close_confmenu_file();

    //关闭资源文件
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //save config 写vram
    _save_cfg();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    alarm ap返回值处理函数，将对应的返回值转换成消息处理
 * \param[in]    app_result_e result各个场景返回值
 * \param[in]    app_result_e enter_ap ap进入模式
 * \param[out]   none
 * \return       the result
 * \ingroup      alarm_main.c
 * \note
 *******************************************************************************/
static app_result_e _deal_result(app_result_e result, uint8 enter_ap, uint8 engine_sta)
{
    msg_apps_t msg;
    engine_state_e engine_status;
    engine_type_e engine_type;
    bool need_send_msg = TRUE;

    engine_status = get_engine_state();
    engine_type = get_engine_type();
    //避免出现未关engine状态
    if ((engine_status != ENGINE_STATE_NULL) && (engine_type == ENGINE_MUSIC))
    {
        if (result != RESULT_APP_QUIT)
            music_close_engine();
    }

    //给process manager发送创建其它进程消息
    msg.type = MSG_CREAT_APP;
    msg.content.data[1] = PARAM_FROM_MAINMENU;
    switch (result)
    {
        case RESULT_ALARM_QUIT:
        if (enter_ap < APP_ID_MAX)
        {
            msg.content.data[0] = enter_ap;
            //开机启动alarm返回mainmenu
            if (enter_ap == APP_ID_CONFIG)
            {
                msg.content.data[0] = APP_ID_MAINMENU;
            }
        }

        if (get_last_app() == APP_ID_VIDEO)
        {
            if (g_app_info_state.video_state == 1)
            {
                msg.content.data[0] = APP_ID_VIDEO;
                msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
        }

        if (enter_ap == APP_ID_MUSIC)
        {
            if (engine_sta == ENGINE_STATE_PLAYING)
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYING;
            }
            else //if(g_app_last_state.last_state == ENGINE_STATE_PAUSE)
            {
                msg.content.data[1] = PARAM_BACKTO_PLAYED;
            }
        }
        else if (enter_ap == APP_ID_RADIO)
        {
            msg.content.data[1] = PARAM_FROM_CONFIG;
        }
        else
        {
            ;//nothing
        }
        break;

        case RESULT_APP_QUIT:
        need_send_msg = FALSE;
        break;
        //默认退出至mainmenu
        default:
        msg.content.data[0] = APP_ID_MAINMENU;
        break;
    }//end of switch (result)

    if (need_send_msg == TRUE)
    {
        //向process manager发送创建进程消息 异步消息
        send_async_msg(APP_ID_MANAGER, &msg);
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc  const char *argv[])
 * \alarm ap 主函数
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval
 * \ingroup      alarm_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e result;
    time_t alarmtime_temp; //缓存snooze时间
    uint8 time_format_tmp;
    uint8 flag;
    engine_state_e engine_status;
    engine_type_e engine_type;
    //app_param_e enter_mode = (app_param_e)argc;
    uint8 engine_sta = ENGINE_STATE_NULL; //记录进入时engine状态
    uint8 enter_mode = g_app_last_state.last_app; //记录从何ap进入

    //根据engine状态更改app id
    engine_status = get_engine_state();
    engine_type = get_engine_type();

    if (engine_status != ENGINE_STATE_NULL)
    {
        if (engine_type == ENGINE_MUSIC)
        {
            enter_mode = APP_ID_MUSIC;
        }
        else if (engine_type == ENGINE_RADIO)
        {
            enter_mode = APP_ID_RADIO;
        }
        else
        {
            //QAC
        }
        engine_sta = engine_status;
    }

    //applib初始化
    _app_init();

    result = alarm_scene_playing();

    //无按键超时退出
    if (g_alarm_timeout == TRUE)
    {
        g_alarm.alarm_count++;
        if (g_alarm.alarm_count < 3)
        {
            //重置闹铃中断+5m
            alarmtime_temp.hour = g_alarm.start_time.hour;
            alarmtime_temp.minute = g_alarm.start_time.minute;
            alarmtime_temp.minute = alarmtime_temp.minute + 5* g_alarm .alarm_count;
            //矫正时间溢出情况
            if (alarmtime_temp.minute > 59)
            {
                alarmtime_temp.minute -= 60;
                alarmtime_temp.hour++;
                time_format_tmp = g_comval.time_format;
                g_comval.time_format = VALUE_24HOURS_FORMAT;
                if (alarmtime_temp.hour > 23)
                {
                    alarmtime_temp.hour = 0;
                }
                g_comval.time_format = time_format_tmp;
            }
            //libc_print("hr1",alarmtime_temp.hour);
            //libc_print("mn1",alarmtime_temp.minute);
            /*加入时间比较，确定下组alarm*/
            if (g_alarmfm.enable)
            {
                time_t time_tmp;
                sys_get_time(&time_tmp); //获取当前系统时间
                flag = compare_curtime(&time_tmp, &g_alarmfm.start_time, &alarmtime_temp, 3);
                if (flag == 1)
                {
                    timer_flag = 0;
                    sys_set_alarm_time(&g_alarmfm.start_time);
                }
                else //if(flag == 2)
                {
                    timer_flag = 1;
                    libc_memcpy(&stimer_tmp, &alarmtime_temp, sizeof(time_t));
                    sys_set_alarm_time(&alarmtime_temp);
                }
            }
            else //if(g_alarmfm.enable == 0)
            {
                timer_flag = 1;
                libc_memcpy(&stimer_tmp, &alarmtime_temp, sizeof(time_t));
                sys_set_alarm_time(&alarmtime_temp);
            }
            // 将临时时间设入
            //sys_set_alarm_time(&alarmtime_temp);
        }
        else //3次过后的处理
        {
            g_alarm.alarm_count = 0;
            /*加入时间比较，确定下组alarm*/
            if (g_alarmfm.enable)
            {
                time_t time_tmp;
                sys_get_time(&time_tmp); //获取当前系统时间
                flag = compare_curtime(&time_tmp, &g_alarmfm.start_time, &g_alarm.start_time, 3);
                //flag = (uint8)CompareAndSet_timer_flag(&g_alarmfm, &g_alarm);
                if (flag == 1)
                {
                    timer_flag = 0;
                    sys_set_alarm_time(&g_alarmfm.start_time);
                }
                else //if (flag == 2)
                {
                    timer_flag = 1;
                    sys_set_alarm_time(&g_alarm.start_time);
                }
            }
            else //if(g_alarmfm.enable == 0)
            {
                timer_flag = 1;
                sys_set_alarm_time(&g_alarm.start_time);
            }
            // 将原时间设回
            //sys_set_alarm_time(&g_alarm.start_time);
        }
    }
    else //非超时退出的处理
    {
        g_alarm.alarm_count = 0;
    }
    set_current_volume(g_comval.volume_current);//恢复系统音量
    _deal_result(result, enter_mode, engine_sta);
    //applib卸载
    _app_deinit();

    return result;
}

