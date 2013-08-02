/********************************************************************************
 *                               us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming         2011-10-13         v1.0              create this file
 ********************************************************************************/
 /*!
 * \file     record_main.c
 * \brief    record的入口函数模块和场景管理
 * \author   leiming
 * \version  1.0
 * \date  2011/10/13
 *******************************************************************************/
/* include file of this application, 本应用的头文件 */
#include "ap_record.h"

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_RECORD_STK_POS;
#endif

#ifdef PC
extern menu_cb_func _get_menu_func(uint16 str_id);
extern menu_cb_leaf _get_callback_func(uint16 str_id);
extern menu_cb_option _get_menu_option(uint16 str_id);
#endif

INT8U prio = AP_RECORD_PRIO;

//应用自己需要的软定时器个数
#define APP_TIMER_COUNT         2

//应用软定时器数组指针，指向应用空间的软定时器数组
app_timer_t rec_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

/* define variable, 变量定义 */
/*******************************record***************************************/
//系统公共变量
comval_t g_comval;
//record的全局变量
record_vars_t g_record_vars;
//ap 入口模式
app_param_e g_entry_mode;
//record codec params
record_mr_param_t g_mr_param;
//录音场景参数集合
record_scene_param_t g_scene_param;
//录音文件相关参数集合
record_fs_param_t g_fs_param;
//录音掉电回写文件头结构体
record_writehead_t g_writehead;
//磁盘剩余容量
uint32 rec_free_space;
//可录时长
uint32 rec_free_time;
//掉电重写的定时计数，计满清0
uint16 rewrite_times;
//录音源备份
mmm_ai_type_t g_ai_source_bak;
//是否需要初始化文件选择器的标志
bool need_fsel_init = TRUE;

//是否定时进入，0-否，1-是
uint8 g_record_from_alarm = 0;
//for alarm_fm
//定时录音的时间长度
uint32 alarm_rec_time;
//FM频段
uint16 fmrec_frequency;
//FM音量
uint8 fmrec_volume;
//是否超过定时录音时长
bool g_record_timeout = FALSE;
//写vram的次数计数
uint8 vram_wr_cnt;
//用于录音文件序号的计数
uint16 g_rec_num;
//用于fm录音文件序号的计数
uint16 g_fmrec_num;
//第一次进录音ADC开启标志
uint8 g_adc_flag;
//是否第一次进入录音应用
uint8 g_first_enter_flag;
//record文件夹文件总数
uint16 g_file_total;
//定时FM定时器
int8 g_alarm_timer;
/******************************************************************************/
/*!
 * \par  Description:
 * \static void _read_fmrecord_var(void)
 * \读取定时fm record的VM变量信息
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static void _read_fmrecord_var(void)
{
    alarm_vars_t temp_alarm_val;
    if (g_record_from_alarm != 0)
    {
        sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
        if (temp_alarm_val.magic != MAGIC_ALARM)
        {
            ;//return FALSE;
        }
        //libc_memcpy(&g_aufm, &temp_alarm_val.g_aufm, sizeof(alarm_fm_t));
        //节省空间，只取与FM有关信息
        //libc_memcpy(&record_end_time, &temp_alarm_val.g_aufm.end_time, sizeof(time_t)); //定时FM的结束时间
        alarm_rec_time = (uint32)((temp_alarm_val.g_aufm.end_time.hour * 3600 + temp_alarm_val.g_aufm.end_time.minute
                * 60) - (temp_alarm_val.g_aufm.start_time.hour * 3600 + temp_alarm_val.g_aufm.start_time.minute * 60));
        fmrec_frequency = temp_alarm_val.g_aufm.frequency; //FM频段
        fmrec_volume = temp_alarm_val.g_aufm.volume; //FM音量
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static void _read_record_var(void)
 * \读取record的VM变量信息
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static void _read_record_var(void)
{
    int result;
    setting_comval_t temp_val;
    /*读取common VM公共变量信息*/
    result = sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));

    /*初始化系统的VM变量*/
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    // 读取record VM公共变量
    result = sys_vm_read(&g_record_vars, VM_AP_RECORD, sizeof(record_vars_t));

    if (g_record_vars.maigc != MAGIC_USER1)
    {
        //设定默认值
        g_record_vars.maigc = MAGIC_USER1;
        g_record_vars.path.file_path.dirlocation.disk = DISK_C;
        g_record_vars.file_type = (uint8) com_get_config_default(RECORD_FORMAT_TYPE);//REC_FORMAT_WAV;
        g_record_vars.mp3_bitrate = (uint8) com_get_config_default(RECORD_MP3_BITRATE);//BIT_32KBPS;
        g_record_vars.wav_bitrate = (uint8) com_get_config_default(RECORD_WAV_BITRATE);//BIT_32KBPS;
        g_record_vars.act_bitrate = BIT_8KBPS;

        if (g_record_vars.file_type == REC_FORMAT_WAV)
        {
            g_record_vars.bitrate = g_record_vars.wav_bitrate;
        }
        else if(g_record_vars.file_type == REC_FORMAT_MP3)
        {
            g_record_vars.bitrate = g_record_vars.mp3_bitrate;
        }
        else
        {
            g_record_vars.bitrate = g_record_vars.act_bitrate;    
        }
        g_record_vars.ai_source = (uint8) com_get_config_default(RECORD_INPUT_SOURCE);//AI_SOURCE_LINEIN;
        g_record_vars.encode_type = (uint8) com_get_config_default(RECORD_WAV_ENCODE_TYPE);//ADPCM;
        g_record_vars.file_num = 0;
        g_record_vars.rec_num = 0;
        g_record_vars.fmrec_num = 0;
        g_record_vars.rec_num_card = 0;
        g_record_vars.fmrec_num_card = 0;
        g_record_vars.noise_cancel_level = 0;
        g_record_vars.gain_input_linein = (uint8) com_get_config_default(RECORD_GAIN_INPUT_LINEIN); //2;
        g_record_vars.gain_adc_linein = (uint8) com_get_config_default(RECORD_GAIN_ADC_LINEIN);//0x2f;
        g_record_vars.gain_input_mic = (uint8) com_get_config_default(RECORD_GAIN_INPUT_MIC);//7;
        g_record_vars.gain_adc_mic = (uint8) com_get_config_default(RECORD_GAIN_ADC_MIC);//0x33;
        g_record_vars.track_mode = (uint8) com_get_config_default(RECORD_TRACK_MODE);//TRACK_OFF;
        g_record_vars.avr_mode = (uint8) com_get_config_default(RECORD_AVR_MODE);//AVR_OFF;
        g_record_vars.track_level_breaksong = (uint8) com_get_config_default(RECORD_TRACK_LEVEL_BREAKSONG);//80;
        g_record_vars.track_level_avr = (uint8) com_get_config_default(RECORD_TRACK_LEVEL_AVR);//65; //触发电平
        g_record_vars.track_delay = (uint8) com_get_config_default(RECORD_TRACK_DELAY);//3;
    }
    //读出alarm信息
    _read_fmrecord_var();
    //磁盘初始化
    if (g_record_vars.path.file_path.dirlocation.disk == 0)
    {
        libc_memset(&g_record_vars.path, 0, sizeof(file_path_info_t));
        g_record_vars.path.file_path.dirlocation.disk = DISK_C;
    }
    if (g_record_vars.path.file_path.dirlocation.disk == DISK_C)
    {
        g_rec_num = g_record_vars.rec_num;
        g_fmrec_num = g_record_vars.fmrec_num;
    }
    else
    {
        g_rec_num = g_record_vars.rec_num_card;
        g_fmrec_num = g_record_vars.fmrec_num_card;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \static void _save_record_var(void)
 * \保存record的VM变量信息
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static void _save_record_var(void)
{
    setting_comval_t temp_val;
#ifndef PC
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&temp_val.g_comval, &g_comval, sizeof(comval_t));
    sys_vm_write(&temp_val, VM_AP_SETTING);
#else
#endif
    /*保存VM变量*/
    if (g_record_vars.path.file_path.dirlocation.disk == DISK_C)
    {
        g_record_vars.rec_num = g_rec_num;
        g_record_vars.fmrec_num = g_fmrec_num;
    }
    else
    {
        g_record_vars.rec_num_card = g_rec_num;
        g_record_vars.fmrec_num_card = g_fmrec_num;
    }
    sys_vm_write(&g_record_vars, VM_AP_RECORD);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \applib的初始化
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static bool _app_init(void)
{
    /*读取ui资源文件*/
    ui_res_open("record.sty", UI_AP);
    ui_res_open("common.sty", UI_COM);

    //open mcg file
    com_open_confmenu_file("record.mcg");

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    //初始化applib库，前台AP
    applib_init(APP_ID_RECORD, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(rec_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //初始化 applib 消息模块
    applib_message_init(&record_message_deal);

    //初始化系统计时器
    sys_timer_init();

    g_first_enter_flag = TRUE;

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_exit(void)
 * \ap退出的一些处理
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
static bool _app_exit(void)
{
    //文件选择器退出
    if (need_fsel_init == FALSE)
    {
        fsel_exit();
    }

    //备份系统计时器
    sys_timer_exit();

    //执行applib库的注销操作
    applib_quit();

    //关闭应用菜单配置脚本
    com_close_confmenu_file();

    /*关闭ui资源文件*/
    ui_res_close(UI_AP);
    ui_res_close(UI_COM);

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _scene_dispatch(app_param_e enter_mode)
 * \场景调度器
 * \param[in]    enter_mode 进入模式 para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       返回到何处
 * \retval
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_dispatch(app_param_e enter_mode)
{
    app_result_e app_ret = RESULT_SCENE_EXIT;
    /*ap进入的初始场景，根据ap传入的参数和配置决定*/
    g_entry_mode = enter_mode;

    switch (g_entry_mode)
    {
        case PARAM_FROM_MAINMENU:
        g_scene_param.record_scene = SCENE_MENU_REC;
        break;

        //case PARAM_FROM_ALARM:
        case PARAM_FROM_RADIO:
        if (g_record_from_alarm != 0)
        {
            if (radio_alarm_init() != TRUE)
            {
                return RESULT_MAIN_APP;
            }
        }
        g_scene_param.record_scene = SCENE_UI_REC;
        g_ai_source_bak = g_record_vars.ai_source;
        g_record_vars.ai_source = AI_SOURCE_FM;
        _radio_get_freq();
        break;

        default:
        if (g_scene_param.record_scene != SCENE_EXIT)
        {
            g_scene_param.record_scene = SCENE_MENU_REC;
        }
        break;
    }
    //g_scene_param.record_scene = SCENE_MENU_REC;
    /*场景管理器*/
    while (g_scene_param.record_scene != SCENE_EXIT)
    {
        switch (g_scene_param.record_scene)
        {
            //rec ui scene
            case SCENE_UI_REC:
            app_ret = record_scene_recording();
            if (app_ret == RESULT_RECORD_MEMU)
            {
                g_scene_param.record_scene = SCENE_MENU_REC;
            }
            else if (app_ret == RESULT_FMRECORD)//alarm_rec消息继续录音
            {
                g_scene_param.record_scene = SCENE_UI_REC;
                g_record_from_alarm = 1;
                g_entry_mode = PARAM_FROM_RADIO;
                //读取alarm_rec的vram
                _read_fmrecord_var();
#ifndef PC
                //从FM  录音返回，由于录音codec 覆盖了fm 驱动代码，重装驱动
                if (get_engine_type() == ENGINE_RADIO)
                {
                    //卸载fm 驱动
                    sys_drv_uninstall(DRV_GROUP_FM);
                    //安装fm  驱动
                    sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
                }
#endif
                if (radio_alarm_init() != TRUE)//初始化fm信息
                {
                    g_scene_param.record_scene = SCENE_EXIT;
                }
                g_ai_source_bak = g_record_vars.ai_source;
                g_record_vars.ai_source = AI_SOURCE_FM;
                _radio_get_freq();
            }
            else
            {
                g_scene_param.record_scene = SCENE_EXIT;
            }
            break;

            //record menu scene
            case SCENE_MENU_REC:
            app_ret = record_scene_menu();
            if (app_ret == RESULT_RECORD_REC)
            {
                g_scene_param.record_scene = SCENE_UI_REC;
            }
            else if (app_ret == RESULT_REDRAW)
            {
                g_scene_param.record_scene = SCENE_EXIT; //return退出到main
                app_ret = RESULT_MAIN_APP;
            }
            else if(app_ret == RESULT_RECORD_MEMU)
            {
                g_scene_param.record_scene = SCENE_MENU_REC;    
            }
            else
            {
                g_scene_param.record_scene = SCENE_EXIT;
            }
            break;

            default:
            g_scene_param.record_scene = SCENE_EXIT; //exit
            break;
        }
    }

    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _scene_result_deal(app_result_e result_val)
 * \场景返回值处理
 * \param[in]    result_val 场景调度返回值 para1
 * \param[out]   none
 * \return       none the result
 * \retval
 * \retval
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
void _scene_result_deal(app_result_e result_val)
{
    msg_apps_t msg;
    switch (result_val)
    {
        case RESULT_RADIO_PLAY:
        //返回到radio播放场景
        msg.type = MSG_CREAT_APP; //恢复创建radio
        msg.content.data[0] = APP_ID_RADIO;//radio ap
        msg.content.data[1] = PARAM_FROM_RECORD;//radio ap param
        send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建radio ui的进程
        //恢复设定的录音源
        g_record_vars.ai_source = g_ai_source_bak;
        break;

        case RESULT_ALARM_QUIT:
        //if (need_reinstall == TRUE)
        {
            //从FM  录音返回，由于录音codec 覆盖了fm 驱动代码，重装驱动
#ifndef PC
            //卸载fm 驱动
            sys_drv_uninstall(DRV_GROUP_FM);
            //安装fm  驱动
            sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
#endif
        }
        radio_close_engine();
        //恢复设定的录音源
        g_record_vars.ai_source = g_ai_source_bak;
        case RESULT_MAIN_APP:
        case RESULT_SCENE_EXIT:
        msg.type = MSG_CREAT_APP; //创建main manu
        msg.content.data[0] = APP_ID_MAINMENU;//main ap
        msg.content.data[1] = PARAM_FROM_RECORD;//main ap param
        send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建main manu的进程
        break;

        case RESULT_BROWSER:
        msg.type = MSG_CREAT_APP; //创建main manu
        msg.content.data[0] = APP_ID_BROWSER;//main ap
        msg.content.data[1] = PARAM_FROM_RECORD;//main ap param
        send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建main manu的进程
        break;

        case RESULT_APP_QUIT:
        if (get_engine_type() == ENGINE_RADIO)
        {
            //从FM  录音返回，由于录音codec 覆盖了fm 驱动代码，重装驱动
#ifndef PC
            //卸载fm 驱动
            sys_drv_uninstall(DRV_GROUP_FM);
            //安装fm  驱动
            sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
#endif
            //安装驱动后必须恢复驱动的全局数据空间
            radio_modual_reinstall();
        }
        //恢复设定的录音源
        g_record_vars.ai_source = g_ai_source_bak;
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int param)
 * \record ap main 主函数
 * \param[in]    int param
 * \param[out]   none
 * \return       int
 * \retval       none
 * \ingroup      record_main.c
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e app_ret;
    app_param_e enter_mode = (app_param_e) argc;

    if (enter_mode == PARAM_FROM_ALARM)
    {
        g_record_from_alarm = 1;
        enter_mode = PARAM_FROM_RADIO;
    }
    else if (enter_mode == PARAM_FROM_BROWSER)
    {
        enter_mode = PARAM_FROM_MAINMENU;
    }
    else
    {
        ;//QAC need
    }

    /* 读入VM变量 */
    _read_record_var();

    /*applib init and open ui/mcg files*/
    if (!_app_init())
    {
        g_scene_param.record_scene = SCENE_EXIT;
    }

    g_scene_param.record_status = REC_STOP_STATUS;

    //切换到LCD_BUFFER
    ui_set_display_buf(LCD_RAM_INDEX);

    //headbar init
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RECORD);

//    Codec_init(0);
    app_ret = _scene_dispatch(enter_mode);
    Codec_exit();

    _scene_result_deal(app_ret);

    /*保存VM变量*/
    _save_record_var();

    //恢复为JPEG_RAM
    ui_set_display_buf(JPEG_RAM_INDEX);

    /*注销applib，close ui/mcg files*/
    if (!_app_exit())
    {
        return 0;
    }

    return 1;
}

