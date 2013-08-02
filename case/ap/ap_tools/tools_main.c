/*******************************************************************************
 *                              US212A
 *                            Module: TOOLS_MAIN
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      liuweizhan    2011-12-12 17:03:16           1.0              build this file
 *******************************************************************************/
/*!
 * \file     tools_main.c
 * \brief
 * \author   liuweizhan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2011-12-12
 *******************************************************************************/
#include "ap_tools.h"

/*main中要填充这两个值，会在运行时库的 __start 中引用（由系统人员分配）*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS; //线程栈顶指针
#endif
uint8 prio = AP_FRONT_LOW_PRIO;  //任务优先级

#ifdef PC
extern menu_cb_func _get_menu_func(uint16 str_id);
extern menu_cb_leaf _get_callback_func(uint16 str_id);
extern menu_cb_option _get_menu_option(uint16 str_id);
#endif

/*系统公共VM变量*/
comval_t g_comval;
/*ALARM VM变量*/
alarm_vars_t  g_alarm_vars;
/*日期公共变量*/
date_t date;
/*时间公共变量*/
//time_t time;
/*alarm set的退出回调变量*/
uint8 alarm_set_flag = 1;
/*alarmfm set的退出回调变量*/
uint8 fmrec_set_flag = 1;

//for stopwatch
/*stopwatch计时变量*/
StopWatch_time_t sw_realtime;
/*保存stopwatch的5项计时变量*/
StopWatch_time_t swtime[5];
/*stopwatch计时状态*/
sw_status_e  sw_status;
//uint8  sw_itemnum;
/*stopwatch显示标志*/
uint8  sw_showflag;
/*stopwatch计时使能变量*/
uint8 CtcCntEnable = 0;

//0.5s定时器
int8 g_halfsec_timer = 0xff;
//MSG_SD_OUT使用，GUI_DIRECTORY置起
bool g_dir_flag = FALSE;

/*软定时器个数*/
#define APP_TIMER_COUNT 2
app_timer_t tools_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

/******************************************************************************/
/*!
 * \par  Description:
 *    读取VM变量
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     TOOLS
 * \note
 *******************************************************************************/
static void _tools_read_var(void)
{
    setting_comval_t temp_val;
    date_t cur_date;
    time_t cur_time;
    uint8 time_format_tmp;
    //系统变量获取
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    /*初始化系统的VM变量*/
#ifndef PC
    if (temp_val.g_comval.magic != MAGIC_SETTING)
#endif
    {
        temp_val.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    /*读取alarm VM公共变量信息*/
    sys_vm_read(&g_alarm_vars, VM_ALARM, sizeof(g_alarm_vars));
    /*初始化alarm VM变量*/
#ifndef PC
    if (g_alarm_vars.magic != MAGIC_ALARM)
#endif
    {
        //公共VM变量
        g_alarm_vars.magic = MAGIC_ALARM;
        g_alarm_vars.timer_flag = 0;
        sys_get_time(&cur_time);
        sys_get_date(&cur_date);

        //alarm VM变量
        g_alarm_vars.g_alarm.enable = 0;
        g_alarm_vars.g_alarm.alarm_count = 0;
        g_alarm_vars.g_alarm.cycle_flag = 1;
        libc_memset(g_alarm_vars.g_alarm.cycle.select_week.week_day, 0, 7);
        g_alarm_vars.g_alarm.cycle.one_time.year = cur_date.year;
        g_alarm_vars.g_alarm.cycle.one_time.month = cur_date.month;
        g_alarm_vars.g_alarm.cycle.one_time.day = cur_date.day;
        g_alarm_vars.g_alarm.start_time.hour = cur_time.hour;
        g_alarm_vars.g_alarm.start_time.minute = cur_time.minute;
        g_alarm_vars.g_alarm.start_time.second = 0;
        g_alarm_vars.g_alarm.volume = 15;

        g_alarm_vars.g_alarm.ring_flag = 0;
        libc_memset(&g_alarm_vars.g_alarm.path, 0, sizeof(file_path_info_t)); //初始化 alarm path 信息
        g_alarm_vars.g_alarm.path.file_source = FSEL_TYPE_COMMONDIR;
        g_alarm_vars.g_alarm.path.file_path.dirlocation.disk = DISK_H;

        //alarm_fm VM变量
        g_alarm_vars.g_aufm.enable = 0;
        g_alarm_vars.g_aufm.cycle_flag = 1;
        libc_memset(g_alarm_vars.g_aufm.cycle.select_week.week_day, 0, 7);
        g_alarm_vars.g_aufm.cycle.one_time.year = cur_date.year;
        g_alarm_vars.g_aufm.cycle.one_time.month = cur_date.month;
        g_alarm_vars.g_aufm.cycle.one_time.day = cur_date.day;
        g_alarm_vars.g_aufm.start_time.hour = cur_time.hour;
        g_alarm_vars.g_aufm.start_time.minute = cur_time.minute;
        g_alarm_vars.g_aufm.start_time.second = 0;
        g_alarm_vars.g_aufm.end_time.hour = g_alarm_vars.g_aufm.start_time.hour;
        if(g_alarm_vars.g_aufm.start_time.minute > 56)//矫正时间溢出情况
        {
            g_alarm_vars.g_aufm.end_time.hour ++;
            time_format_tmp = g_comval.time_format;
            g_comval.time_format = VALUE_24HOURS_FORMAT;
            if(g_alarm_vars.g_aufm.end_time.hour > 23)
            {
                g_alarm_vars.g_aufm.end_time.hour = 0;
            }
            g_comval.time_format = time_format_tmp;
        }
        g_alarm_vars.g_aufm.end_time.minute = (g_alarm_vars.g_aufm.start_time.minute + 3)%60;
        g_alarm_vars.g_aufm.end_time.second = 0;
        g_alarm_vars.g_aufm.frequency = 0x55cc; //默认频段87.5
        g_alarm_vars.g_aufm.volume = 15;
        g_alarm_vars.g_aufm.fmrec_enable = 0;

        sys_vm_write(&g_alarm_vars, VM_ALARM);
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ap进入初始化
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     TOOLS
 * \note
 *******************************************************************************/
static bool _tools_app_init(void)
{
    //初始化applib库，前台AP
    applib_init(APP_ID_TOOLS, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(tools_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //初始化 applib 消息模块
    applib_message_init(tool_privmsg_deal);

    sys_timer_init();

    //初始ui资源文件
    ui_res_open("common.sty", UI_COM);
    ui_res_open("tools.sty", UI_AP);

    //打开前台应用菜单配置脚本
    com_open_confmenu_file("tools.mcg");

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    ap退出处理
 * \param[in]
 * \param[out]
 * \return
 * \retval
 * \ingroup     TOOLS
 * \note
 *******************************************************************************/
static bool _tools_app_deinit(void)
{
    //关闭应用菜单配置脚本
    com_close_confmenu_file();

    sys_timer_exit();

    //关闭ui资源文件
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //执行applib库的注销操作
    applib_quit();
    return TRUE;
}
uint8 _detect_usb_in (void)
{
    if(((act_readb(LINESTATUS) & 0x18) == 0) &&
        ((act_readb(CHG_CTL) & 0x80) != 0))
/*
    if(((act_readb(DPDMCTRL) & 0x40) != 0) &&
        ((act_readb(CHG_CTL) & 0x80) != 0))*/
    {
        return 1;
    }
    return 0;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    tools 应用返回值处理函数，将对应的返回值转换成消息处理
 * \param[in]    result app_result_e 各个场景返回值
 * \param[out]   none
 * \return       the result
 * \ingroup      TOOLS
 * \note
 *******************************************************************************/
static app_result_e _deal_tools_result(app_result_e tools_result)
{
    msg_apps_t msg;
    //engine_state_e engine_status;
    engine_type_e engine_type;
    bool need_send_msg = TRUE;

    //engine_status = get_engine_state();
    //给process manager发送创建其它进程消息
    switch (tools_result)
    {
        case RESULT_MAIN_APP:
        msg.content.data[0] = APP_ID_MAINMENU;
        msg.content.data[1] = PARAM_FROM_TOOLS;
        break;

        case RESULT_DATAEXCH:
        msg.content.data[0] = APP_ID_ALARM;
        msg.content.data[1] = PARAM_FROM_TOOLS;
        break;

        case RESULT_UDISK:
        //engine_type = get_engine_type();
        //if(engine_type != ENGINE_NULL)
        //{
        //    msg.type = MSG_KILL_APP_SYNC;
        //    if (engine_type == ENGINE_MUSIC)
        //    {
        //        msg.content.data[0] = APP_ID_MENGINE;
        //    }
        //    else// if (engine_type == ENGINE_RADIO)
        //    {
        //        msg.content.data[0] = APP_ID_FMENGINE;
        //    }
        //    //发送同步消息 杀死引擎
        //    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
        //}
        msg.content.data[0] = APP_ID_UDISK;
        msg.content.data[1] = PARAM_FROM_TOOLS;
        break;

        case RESULT_AUTO_UPGRADE:
        msg.content.data[0] = APP_ID_UPGRADE;
        msg.content.data[1] = PARAM_FROM_TOOLS;
        break;

        case RESULT_APP_QUIT:
        default:
        need_send_msg = FALSE;
        break;
    }

    if (need_send_msg == TRUE)
    {
        private_msg_t private_msg;

        if(tools_result == RESULT_UDISK)
        {
            //交给manager处理,不需要关engine
            msg.type = MSG_USB_TRANS;
            //向process manager发送创建进程消息 同步消息还是异步消息
            send_async_msg(APP_ID_MANAGER, &msg);

            //等待AP退出消息 MSG_APP_QUIT ，再返回 RESULT_APP_QUIT，应用直接退出
            while(1)
            {
                //挂起10ms，多任务调度
                sys_os_time_dly(1);
                if((get_app_msg(&private_msg) == TRUE) && (private_msg.msg.type == MSG_APP_QUIT))
                {
                    tools_result = RESULT_APP_QUIT;
                    break;
                }
            }
        }
        else
        {
            msg.type = MSG_CREAT_APP;
            //向process manager发送创建进程消息 同步消息还是异步消息
            send_async_msg(APP_ID_MANAGER, &msg);
        }
    }
    return tools_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Tools 应用的入口函数和场景管理器模块
 * \param[in]    argc 由进程管理器传入的参数，表示从何种模式进入
 * \param[in]    argv 无用参数
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      module name
 * \par          exmaple code
 * \code
 *               list example code in here
 * \endcode
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e result;
    scene_tools_e next_scene;

    _tools_app_init();

    _tools_read_var();

    next_scene = SCENE_MENULIST;

    while (next_scene != SCENE_EXIT)
    {
        switch (next_scene)
        {
            case SCENE_MENULIST:
            result = tools_menulist();
            switch (result)
            {
                case RESULT_CALENDAR:
                next_scene = SCENE_CALENDAR;
                break;
                case RESULT_STOPWATCH:
                next_scene = SCENE_STOPWATCH;
                break;
                case RESULT_DATAEXCH:
                //result = RESULT_MAIN_APP; //暂时退出至mainmanu
                next_scene = SCENE_EXIT;
                break;
                case RESULT_UDISK:
                if (_detect_usb_in() == 0)
                {
                    break;
                }
                next_scene = SCENE_EXIT;
                break;
                case RESULT_APP_QUIT:
                next_scene = SCENE_EXIT;
                break;

                case RESULT_AUTO_UPGRADE:
                next_scene = SCENE_EXIT;
                break;

                default:
                result = RESULT_MAIN_APP;
                next_scene = SCENE_EXIT;
                break;
            }
            break;

            case SCENE_CALENDAR:
            result = calendar_main(FROM_MENU);
            if(result == RESULT_TOOLS_MENULIST)
            {
                next_scene = SCENE_MENULIST;
            }
            else if(result == RESULT_APP_QUIT)
            {
                next_scene = SCENE_EXIT;
            }
            else
            {
                //for QAC
            }
            break;

            case SCENE_STOPWATCH:
            result = stopwatch_main();
            if(result == RESULT_TOOLS_MENULIST)
            {
                next_scene = SCENE_MENULIST;
            }
            else if(result == RESULT_APP_QUIT)
            {
                next_scene = SCENE_EXIT;
            }
            else
            {
                //for QAC
            }
            break;

            default:
            next_scene = SCENE_EXIT;
            break;
        }
    }

    _deal_tools_result(result);

    _tools_app_deinit();

    return result;//dummy
}
/*! \endcond */

