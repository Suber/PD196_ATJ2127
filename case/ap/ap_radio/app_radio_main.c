/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       mikeyang         2011-09-13          1.0              create this file
 *******************************************************************************/
#include "app_radio.h"
#include "app_radio_menu_config.h"

extern app_result_e radioUI_scene_dispatch(app_param_e enter_mode)
__FAR__;
extern bool radio_modual_standby(void);
extern bool radio_close_engine(void);

#if 0    //多风格支持
//风格资源名称
const uint8 themes_com[5][13] =
{
    "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty"
};

const uint8 themes_ap[5][13] =
{
    "radio.sty", "ra_green.sty", "ra_gray.sty", "ra_pink.sty", "ra_red.sty"
};
#endif

/* radio ap 1个用于滚屏，1 个用于自动搜台
 **/
#define APP_TIMER_COUNT		2
/*!
 *  \brief
 *  app_timer_vector：应用软定时器数组指针，指向应用空间的软定时器数组
 */
app_timer_t radio_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

/*! for alarm_fm */
/* radio进入模式 */
uint8 g_radio_from_alarm = 0;
/* alarm_fm的结束时间 */
time_t alarmfm_end_time;
/* alarm_fm频段 */
//uint16 alarmfm_frequency;
/* alarm_fm音量 */
uint8 alarmfm_volume;

/* Radio UI 任务栈*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_RADIO_STK_POS;
#endif

/* Radio UI 进程的主线程优先级*/
INT8U prio = AP_RADIO_PRIO;

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_load_cfg(void)
 * \载入应用初始化信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radio_load_cfg(void)
{
    setting_comval_t temp_comval;
    alarm_vars_t temp_alarm_val;
    uint8 default_band;

    //系统变量获取
    sys_vm_read(&temp_comval, VM_AP_SETTING, sizeof(setting_comval_t));

    /*初始化系统的VM变量*/
    if (temp_comval.g_comval.magic != MAGIC_SETTING)
    {
        temp_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_comval);
        sys_vm_write(&temp_comval, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_comval.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    //获取radio AP 配置信息
    sys_vm_read(&g_radio_config, VM_AP_RADIO, sizeof(radio_config_t));

    //统一为0x55AA
    if (g_radio_config.magic != MAGIC_USER1)
    {
        g_radio_config.magic = MAGIC_USER1;

        //三个波段预设电台列表清零
        libc_memset(g_radio_config.fmstation_us, 0, MAX_STATION_COUNT * 2* 3 );

        default_band = (uint8)com_get_config_default(RADIO_AP_ID_BAND);
        g_radio_config.FMStatus.freq = min_freq[default_band];
        g_radio_config.FMStatus.station = 0;
        g_radio_config.FMStatus.station_count = 0;
        g_radio_config.band_mode = (radio_band_e)default_band; // Band_China_US

        //默认未进入voice 进程
        g_radio_config.enter_voice= NO_ERTER_VOICE;
        //默认搜台方式
        g_radio_config.seek_mode = (FM_SeekMode_e)com_get_config_default(RADIO_AP_ID_SEEKMODE);

        sys_vm_write(&g_radio_config, VM_AP_RADIO);
    }
    //alarm_fm模式 读取alarm VM信息
#ifdef ALARMFM
    if (g_radio_from_alarm != 0)
    {
        sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
        if (temp_alarm_val.magic != MAGIC_ALARM)
        {
            ;//return FALSE;
        }
        //libc_memcpy(&g_aufm, &temp_alarm_val.g_aufm, sizeof(alarm_fm_t));
        //节省空间，只取与FM有关信息
        libc_memcpy(&alarmfm_end_time, &temp_alarm_val.g_aufm.end_time, sizeof(time_t)); //定时FM的结束时间
        g_radio_config.FMStatus.freq = temp_alarm_val.g_aufm.frequency; //FM频段
        alarmfm_volume = temp_alarm_val.g_aufm.volume; //FM音量
        g_comval.volume_current = temp_alarm_val.g_aufm.volume; //FM音量
    }
#endif
    /* 上一次退出RadioUI 时，播放的不为用户电台*/
    if(g_radio_config.band_mode != Band_MAX)
    {
        /* 菜单波段激活项*/
        g_menu_band = g_radio_config.band_mode;
    }
    else
    {
        /* 上次退出播放的是用户电台，则菜单中默认激活为欧洲波段*/
        /* 用户电台可编辑最小步长和欧洲波段一样，为50k */
	 /* 解决首次搜台后播放用户电台退出，再进入又提示需搜台问题 */	
        g_menu_band = (uint8)com_get_config_default(RADIO_AP_ID_BAND);
    }

    //用户电台列表清零，重新解析
    libc_memset(g_user_station.fmstation_user, 0, MAX_STATION_COUNT*2);
    //用户电台显示名称字串清零
    libc_memset(g_user_station.station_name, 0, 40);
    //用户电台列表索引表清零
    libc_memset(g_userlist_table, 0, MAX_STATION_COUNT);

    //不使用默认用户电台列表
    g_use_default = FALSE;

    /* 指针指向当前播放的频率表首*/
    init_point_freq();

    //初始化所浏览的列表指针
    if(g_menu_band == Band_China_US)
    {
        pfreq_list = &g_radio_config.fmstation_us[0];
    }
    else if(g_menu_band == Band_Japan)
    {
        pfreq_list = &g_radio_config.fmstation_jp[0];
    }
    else
    {
        pfreq_list = &g_radio_config.fmstation_eu[0];
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_save_cfg(void)
 * \保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radio_save_cfg(void)
{
    //各应用文件数
    uint16 ap_file_num[MAX_APP_NUM];
    setting_comval_t temp_comval;
#ifdef ALARMFM
    if (g_radio_from_alarm != 0)
    {
        return;
    }
#endif
    sys_vm_read(&temp_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&temp_comval.g_comval, &g_comval, sizeof(comval_t));
    /*保存VM变量*/
    sys_vm_write(&temp_comval, VM_AP_SETTING);
    sys_vm_write(&g_radio_config, VM_AP_RADIO);

    /*读出各应用文件总数*/
    sys_vm_read(&ap_file_num, VM_FILE_NUM, sizeof(ap_file_num));
    ap_file_num[APP_ID_RADIO] = g_radio_config.FMStatus.station_count;
    sys_vm_write(&ap_file_num, VM_FILE_NUM);

    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radioUI_app_init(void)
 * \RadioUI 应用信息初始化
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radioUI_app_init(void)
{
    bool app_init_ret = TRUE;
    uint8 i;

    //读取配置信息 (包括vm信息)
    radio_load_cfg();

    /*加载UI  资源文件*/
#if 0
    app_init_ret = ui_res_open(themes_com[g_comval.theme], UI_COM);
    app_init_ret = ui_res_open(themes_ap[g_comval.theme], UI_AP);
#endif
    app_init_ret = ui_res_open("common.sty", UI_COM);
    app_init_ret = ui_res_open("radio.sty", UI_AP);
    if (!app_init_ret)
    {
        return FALSE;
    }

    /* 打开菜单配置脚本文件*/
    app_init_ret = com_open_confmenu_file("radio.mcg");
    if (!app_init_ret)
    {
        return FALSE;
    }

#ifdef PC
    load_menulist_simfunc(get_menu_func, get_callback_func, get_menu_option);
#endif

    //初始化applib库，前台AP
    applib_init(APP_ID_RADIO, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(radio_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    applib_message_init(radio_app_msg_callback);

    //初始化系统软定时器
    sys_timer_init();

    //进行用户电台列表的完整解析，获取所有频点信息
    app_init_ret = radio_parse_userlist(NULL, ALL_FREQ, 0);
    if (!app_init_ret)
    {
        //解析出错，使用默认的用户电台列表
        g_use_default = TRUE;
        encode_mode = ENCODE_MUL;

        libc_memset(g_user_station.fmstation_user, 0, MAX_STATION_COUNT * 2);
        libc_memset(g_user_station.station_name, 0, 40);
        //默认用户电台列表填充
        for (i = 0; i < 3; i++)
        {
            g_user_station.fmstation_user[i] = default_freq[i];
            g_userlist_table[i] = i + 1; //CH1, CH2, CH3
        }
        //默认用户电台暂定3  个
        g_userlist_total = 3;
        libc_memcpy(g_user_station.station_name, default_name, sizeof(default_name));
    }
    else
    {
        /* 成功解析，不使用默认列表*/
        g_use_default = FALSE;
    }
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radioUI_app_exit(void)
 * \退出app的功能函数 保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radioUI_app_exit(void)
{
    //删除系统软定时器
    sys_timer_exit();

    //配置信息回写VRAM
    radio_save_cfg();

    //关闭资源文件
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //关闭应用菜单配置脚本
    com_close_confmenu_file();

    /*执行applib库的注销操作*/
    applib_quit();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void radioUI_scene_result_deal(app_result_e result_val)
 * \场景返回值处理
 * \param[in]    result_val 场景调度返回值 para1
 * \param[out]   none
 * \return       none the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void radioUI_scene_result_deal(app_result_e result_val)
{
    msg_apps_t msg;
    bool need_create_app = TRUE;

    //创建其他应用进程
    msg.type = MSG_CREAT_APP;
    switch (result_val)
    {
        //退出FM UI进程，创建Browser 进程( Browser 根据传参过滤录音库文件)
        case RESULT_VOICE_FM_MENU:
        msg.content.data[0] = APP_ID_BROWSER; //browser ap
        break;

        //创建main menu进程
        case RESULT_MAIN_APP:
        msg.content.data[0] = APP_ID_MAINMENU;//main ap
        break;

        //创建FM record 进程
        case RESULT_RECORD_REC:
        msg.content.data[0] = APP_ID_RECORD; //fm record ap
        break;

        //回到music正在播放前台进程
        case RESULT_MUSIC:
        msg.content.data[0] = APP_ID_MUSIC; //music ui ap
        break;

        case RESULT_APP_QUIT:
        need_create_app = FALSE;
        break;

        //默认回到main
        default:
        msg.content.data[0] = APP_ID_MAINMENU;//main ap
        break;
    }

    if (need_create_app == TRUE)
    {
        msg.content.data[1] = (uint8) PARAM_FROM_RADIO;
        if (result_val == RESULT_MUSIC)
        {
            msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        }
        //发送异步消息，创建其他前台进程
        send_async_msg(APP_ID_MANAGER, &msg);
    }
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc, const char *argv[])
 * \radioUI app 入口函数
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e retval;
    bool result;
    app_param_e enter_mode = (app_param_e)(argc);
    if (enter_mode == PARAM_FROM_ALARM)
    {
        g_radio_from_alarm = 1;
    }
    //    app_param_e enter_mode = (app_param_e) (argc & 0x1f);
    //    //如何获知进入模式?
    //    g_radio_from_alarm = (uint8) (argc & 0xe0);
    result = radioUI_app_init();//初始化
    if (!result)
    {
        //初始化失败，返回主应用
        retval = RESULT_MAIN_APP;
    }
    else
    {
        retval = radioUI_scene_dispatch(enter_mode);//场景调度器
    }
#ifdef ALARMFM
    if (g_radio_from_alarm != 0)
    {
        //对于返回值为RESULT_APP_QUIT的返回值，一般为插入USB后由manager主导前后台
        //的退出，此时不需要应用控制ap的退出和引擎的关闭
        if(retval != RESULT_APP_QUIT)
        {
            if(radio_get_vol_status() == TRUE)
            {
                radio_close_engine();
            }
        }
    }
#endif
    //场景调度返回结果处理
    radioUI_scene_result_deal(retval);
    radioUI_app_exit();//退出
    return retval;
}

