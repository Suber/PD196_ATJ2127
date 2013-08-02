/*******************************************************************************
 *                              US212A
 *                            Module: Video
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     Video
 * \brief    Video 的main函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "video.h"
#include "video_menu_cfg.h"
#include "music_common.h"

//全局变量定义

/*! \brief video应用VM变量*/
g_video_var_t g_video_var;

/*! \brief 通用VM变量*/
comval_t g_comval;

/*! \brief video 挂载文件系统的mount ID*/
int32 video_mount;

/*! \brief 视频文件总时间*/
uint32 g_total_time;

/*! \brief 用于刷新进度条的定时器ID，1s定时*/
int8 timer_flush_progress;

/*! \brief 用于滚动显示文件名的定时器ID，80ms定时*/
int8 timer_flush_filename;

/*! \brief 用于全屏态与窗口态切换的定时器ID, 5s定时*/
int8 timer_flush_screen;

/*! \brief 用于控制定时器的开启与关闭 bit0: 全屏定时器 bit1:文件名定时器 bit2:进度条定时器*/
uint8 g_video_timer_flag;

/*! \brief 刷新位标志变量*/
uint32 g_display_bitmap;

/*! \brief video应用进入模式*/
app_param_e g_video_enter_mode;

/*! \brief 视频解码句柄*/
void* g_video_handle;

/*! \brief 视频播放状态*/
video_play_status_e g_video_play_status;

/*! \brief 视频播放状态备份*/
video_play_status_e g_video_play_status_backup;

/*! \brief 视频当前窗口状态*/
video_window_e g_video_cur_window;

/*! \brief 视频之前窗口状态*/
video_window_e g_video_prev_window;

/*! \brief 刷新标志，标识是否刷新*/
uint8 g_video_paint_flag;

/*! \brief 应用获取视频解码信息的变量*/
mmm_vp_userparam_t g_video_userinfor;

/*! \brief 应用控制屏窗口变量*/
ctlinfor_t g_video_ctlinfor;

/*! \brief 满屏窗参数*/
wregion_t g_region_fullscreen;

/*! \brief 时间进度条窗参数*/
wregion_t g_region_progressbar;

/*! \brief 音量条窗参数*/
wregion_t g_region_volumebar;

/*! \brief 进度条和音量条高度调整参数*/
adjust_region_t g_region_adjust;

/*! \brief 自动播放切换文件标志 bit0: 0:切下一曲 1:切上一曲 bit1:是否切换歌曲*/
uint8 g_video_change_file;

/*! \brief video刷新模式 */
uint8 g_video_draw_mode;

/*! \brief video快进快退速度控制 */
uint8 g_video_speed_control;

/*! \brief video时间计数 */
uint8 g_video_timer_counter;

/*! \brief 文件名缓冲区 */
uint8 g_video_filename[LIST_ITEM_BUFFER_LEN];

/*! \brief 文件名是否滚动显示标志 */
bool need_scroll_filename;

/*! \brief  应用软定时器数组指针，指向应用空间的软定时器数组*/
app_timer_t video_app_timer_vector[APP_TIMER_TOTAL];

/*! \brief video要进入的场景*/
scene_video_e g_video_scene_next;

/*! \brief video上一次调度的场景*/
scene_video_e g_video_scene_prev;

/*! \brief video文件总数计数*/
uint16 g_file_total;

/*! \brief video错误文件计数*/
uint16 g_file_err;

/*! \brief video是否删除文件*/
uint8 g_del_file;

uint8 g_fast_back_end;
#ifndef PC
/*! \brief video使用的堆栈指针 */
OS_STK *ptos = (OS_STK *) AP_VIDEO_STK_POS;
#endif
/*! \brief video使用的进程优先级 */
INT8U prio = AP_VIDEO_PRIO;

/*! \brief 公共资源文件(common style)名 */
const uint8 com_themes[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };

/*! \brief video资源文件名 */
const uint8 video_sty[] =
{ "video.sty" };

/*! \brief video菜单数据文件 */
const uint8 video_mcg[] =
{ "video.mcg" };

/******************************************************************************/
/*!
 * \par  Description:
 *    获取后台待播放文件的盘符
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_main
 * \note
 *******************************************************************************/
uint8 music_get_disk_type(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;

    file_path_info_t music_file_path;

    //获取当前播放文件的路径信息
    reply.content = &music_file_path;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);

    return music_file_path.file_path.dirlocation.disk;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  Video 读取应用VM全局变量,初始化全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_main
 * \note
 *******************************************************************************/
static void _video_read_var(void)
{
    int result;
    engine_type_e engine_type;
    setting_comval_t temp_val;

    //读取common VM公共变量信息
    result = sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));

    //初始化系统的VM变量
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    //读取music VM公共变量信息
    result = sys_vm_read(&g_video_var, VM_AP_VIDEO, sizeof(g_video_var));

    //第一次进入AP，初始化VM变量
    if (g_video_var.magic != MAGIC_VIDEO)
    {
        g_video_var.magic = MAGIC_VIDEO;
        libc_memset(&g_video_var.path.file_path, 0, sizeof(musfile_location_u));
        g_video_var.path.file_path.flist_location.disk = DISK_BASE;
        g_video_var.file_ext_bitmap = VIDEO_BITMAP;
        g_video_var.repeat_mode = VIDEO_REPEAT_NORMAL;
        g_video_var.prev_enter_mode = PARAM_FROM_MAINMENU;
        libc_memset(&g_video_var.break_point, 0, sizeof(g_video_var.break_point));
    }

    if ((g_video_enter_mode == PARAM_FROM_PLAYLIST)
        || (g_video_enter_mode == PARAM_BACKTO_PLAYING)
        || (g_video_enter_mode == PARAM_BACKTO_PLAYED))
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            //检测不到卡盘，则初始化成默认盘符
            g_video_var.path.file_path.dirlocation.disk = DISK_BASE;
        }
    }
    else
    {
        g_video_var.path.file_path.dirlocation.disk = DISK_BASE;
    }

    if (g_video_enter_mode == PARAM_FROM_BROWSER)
    {
        sys_vm_read(&g_video_var.path, VM_AP_BROWSER, sizeof(file_path_info_t));
    }

    //引擎存在则获取当前播放文件的路径信息
    engine_type = get_engine_type();
    if (engine_type == ENGINE_MUSIC)
    {
        //更改盘符
        g_video_var.path.file_path.flist_location.disk = music_get_disk_type();
    }

    g_app_info_state.video_state = 0; 
}

/******************************************************************************/
/*!
 * \par  Description:
 *    video 应用返回值处理函数，将对应的返回值转换成消息处理
 * \param[in]    result app_result_e 各个场景返回值
 * \param[out]   none
 * \return       the result
 * \ingroup      video_main
 * \note
 *******************************************************************************/
static app_result_e _deal_video_result(app_result_e video_result)
{
    msg_apps_t msg;
    engine_state_e engine_status;
    engine_type_e engine_type;
    bool need_send_msg = TRUE;

    engine_status = get_engine_state();
    engine_type = get_engine_type();

    //给process manager发送创建其它进程消息
    msg.type = MSG_CREAT_APP;
    msg.content.data[1] = (uint8) PARAM_FROM_VIDEO;

    if(video_result == RESULT_CARD_OUT_ERROR)
    {
        //检测不到卡盘
        video_clear_breakinfor();      
        if(g_video_enter_mode == PARAM_FROM_BROWSER)
        {
            video_result = RESULT_RETURN_BROWSER;
        }
        else
        {
            video_result = RESULT_MAIN_APP;
        }
    }

    switch (video_result)
    {
        case RESULT_RETURN_BROWSER:
        msg.content.data[0] = APP_ID_BROWSER;
        break;

        case RESULT_NOWPLAYING:
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;           
        }
        else if (engine_type == ENGINE_RADIO)
        {
            msg.content.data[0] = APP_ID_RADIO;
        }
        else
        {
            ;//nothing
        }
        break;

        case RESULT_LASTPLAY:
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYED;
        if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;

        }
        else if (engine_type == ENGINE_RADIO)
        {
            msg.content.data[0] = APP_ID_RADIO;
        }
        else
        {
            ;//nothing
        }
        break;

        case RESULT_CREATE_LIST_APP:
        msg.content.data[0] = APP_ID_PLAYLIST;
        //只更新当前盘符的playlist
        if (g_video_var.path.file_path.dirlocation.disk == DISK_C)
        {
            msg.content.data[1] = (PLIST_DISK_C | PARAM_FROM_VIDEO);
        }
        else
        {
            msg.content.data[1] = (PLIST_DISK_H | PARAM_FROM_VIDEO);
        }
        //保存进入模式
        g_video_var.prev_enter_mode = g_video_enter_mode;
        break;

        case RESULT_APP_QUIT:
        need_send_msg = FALSE;
        //保存进入模式
        g_video_var.prev_enter_mode = g_video_enter_mode;        
        break;

        default:
        //默认的其它返回值均返回main
        msg.content.data[0] = APP_ID_MAINMENU;
        break;
    }

    if (TRUE == need_send_msg)
    {
        //向process manager发送创建进程消息 同步消息还是异步消息
        send_async_msg(APP_ID_MANAGER, &msg);
    }
    return video_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    video 完成播放控制参数初始化以及场景调度选择
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      video_main
 * \note
 *******************************************************************************/
void _video_data_init(void)
{
    uint8 other_enter_mode = 0;
    //初始化视频的三个窗区域
    style_infor_t video_style;
    video_style.type = UI_AP;

    //读取三个窗坐标参数
    video_style.style_id = STY_VIDOE_FULLSCREEN_REGION;
    ui_get_textbox_attrb(&video_style, &g_region_fullscreen, 0);

    video_style.style_id = STY_VIDEO_PROGRESS_REGION;
    ui_get_textbox_attrb(&video_style, &g_region_progressbar, 0);

    video_style.style_id = STY_VIDEO_VOLUME_REGION;
    ui_get_textbox_attrb(&video_style, &g_region_volumebar, 0);

    //由于amv和avi绘图起始点并不相同，因此这里要做出区分
    //       |------------------|
    // avi-> |----------------->|
    // 起始点|----------------->|
    //       |----------------->|
    //       |------------------|
    //       |------------------|
    g_region_adjust.avi_prog_y = g_region_progressbar.y;
    g_region_adjust.avi_vol_y = g_region_volumebar.y;

    //       |------------------|
    //       |------------------|
    //       |----------------->|
    //       |----------------->|
    // amv-> |----------------->|
    // 起始点|------------------|
    g_region_adjust.amv_prog_y = g_region_fullscreen.h - g_region_progressbar.y - g_region_progressbar.h;
    g_region_adjust.amv_vol_y = g_region_fullscreen.h - g_region_volumebar.y - g_region_volumebar.h;

    //mount_ID 中间件据此操作文件并解析文件
    g_video_userinfor.mountindex = (uint32) video_mount;

    //断点参数
    g_video_userinfor.pbreakinfor = &g_video_var.break_point;

    //三个窗参数
    g_video_userinfor.pregion1 = &g_region_fullscreen;
    g_video_userinfor.pregion2 = &g_region_progressbar;
    g_video_userinfor.pregion3 = &g_region_volumebar;

    //初始化播放控制参数
    g_video_ctlinfor.fadeout = 0;
    g_video_ctlinfor.softvolume = 0;
    g_video_ctlinfor.progressbar = 0;
    g_video_ctlinfor.volumebar = 0;
    g_video_userinfor.plcminfor = &g_video_ctlinfor;

    //从播放列表进入，选择之前进入的模式
    if ((g_video_enter_mode != PARAM_FROM_MAINMENU) && (g_video_enter_mode != PARAM_FROM_BROWSER))
    {
        if((g_video_enter_mode != PARAM_BACKTO_PLAYED)
            && (g_video_enter_mode != PARAM_BACKTO_PLAYING))
        {
            other_enter_mode = 1; 
        } 
        else
        {
            other_enter_mode = 2;
        }
        g_video_enter_mode = g_video_var.prev_enter_mode;
    }

    //从主界面进入，首先根据有无卡盘选择进入菜单场景还是列表场景
    if (g_video_enter_mode == PARAM_FROM_MAINMENU)
    {

        this_headbar_icon_id = HEADBAR_ICON_VIDEO;
        g_video_var.path.file_source = FSEL_TYPE_LISTVIDEO;
        g_video_var.path.file_path.plist_location.list_type = PLIST_TITLE;

        if ((sys_detect_disk(DRV_GROUP_STG_CARD) == -1) || (other_enter_mode == 1))
        {
            g_video_scene_next = SCENE_FILELIST;
        }
        else
        {
            g_video_scene_next = SCENE_MENU;
        }
    }
    else
    {
        //其他模式进入，直接进入播放场景
        g_video_scene_next = SCENE_PLAY;
        this_headbar_icon_id = HEADBAR_ICON_BROWSER_V;
    }

    //standby起来播放，直接进入播放场景
    if(other_enter_mode == 2)
    {
        g_video_scene_next = SCENE_PLAY;    
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video 应用的初始化
 * \param[in]    void
 * \param[out]   void
 * \return       app_result_e
 * \retval       RESULT_NULL    success
 * \retval       other value    fail
 * \ingroup      video_main
 * \note
 *******************************************************************************/
app_result_e _video_app_init(void)
{
    app_result_e result;

    //初始化applib库，前台AP
    applib_init(APP_ID_VIDEO, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(video_app_timer_vector, APP_TIMER_TOTAL);

    //初始化 applib 消息模块
    applib_message_init(video_msg_callback);

    //初始化common的两个定时器函数，必须在初始化完软定时器后调用
    sys_timer_init();

    //初始ui资源文件
    ui_res_open(com_themes[g_comval.theme], UI_COM);

    ui_res_open(video_sty, UI_AP);

    //打开前台应用菜单配置脚本
    com_open_confmenu_file(video_mcg);

    //初始化全局变量
    _video_data_init();

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    //初始化文件选择器
    result = video_disk_initial();
    if (result == RESULT_MAIN_APP)
    {
        g_video_scene_next = SCENE_EXIT;
        return result;
    }
    else if(result == RESULT_VIDEO_MENU)
    {
        g_video_scene_next = SCENE_MENU;
    }
    else
    {
        ;//nothing
    }    
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video 应用的退出
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      video_main
 * \note
 *******************************************************************************/
bool _video_app_deinit(void)
{
    //文件选择器退出
    fsel_exit();

    //关闭应用菜单配置脚本
    com_close_confmenu_file();

    //卸载common定时器服务例程
    sys_timer_exit();

    //关闭ui资源文件
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    //执行applib库的注销操作
    applib_quit();
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    video保存comval VM变量,避免使用setting_comval_t这个更大的结构体
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      video_main
 * \note
 *******************************************************************************/
static void _video_save_comval(void)
{
#ifndef PC
    setting_comval_t temp_val;
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&temp_val.g_comval, &g_comval, sizeof(comval_t));
    sys_vm_write(&temp_val, VM_AP_SETTING);
#else
    setting_comval_t temp_val;
    char temp_buffer[512];
    sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&temp_val.g_comval, &g_comval, sizeof(comval_t));
    libc_memcpy(temp_buffer, &temp_val, sizeof(setting_comval_t));
    sys_vm_write(temp_buffer, VM_AP_SETTING);
#endif

}
/******************************************************************************/
/*!
 * \par  Description:
 *	  video下一个要调度的场景
 * \param[in]    app_result_e 返回值
 * \param[out]   处理后的返回值
 * \return       void
 * \ingroup      video_main
 * \note
 * \li  video有4个场景:文件列表场景,option菜单项场景，播放场景和菜单场景(有卡时才有)
 *******************************************************************************/
static void _video_select_next_scene(app_result_e *ui_result)
{
    app_result_e result = *ui_result;
    g_video_scene_prev = g_video_scene_next;

    if (result == RESULT_DIR_ERROR_NO_FILE)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //存在卡盘
            result = RESULT_VIDEO_MENU;
        }
        else
        {
            result = RESULT_MAIN_APP;
        }
    }

    switch (result)
    {
        case RESULT_VIDEO_MENU:
        g_video_scene_next = SCENE_MENU;
        break;

        case RESULT_VIDEO_OPTIONMENU:
        g_video_scene_next = SCENE_OPTIONMENU;
        break;

        case RESULT_VIDEO_FILELIST:
        g_video_scene_next = SCENE_FILELIST;
        break;

        case RESULT_MOVIE_PLAY:
        g_video_scene_next = SCENE_PLAY;
        break;

        default:
        g_video_scene_next = SCENE_EXIT;
        break;
    }

    *ui_result = result;
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  video 应用的入口函数和场景管理器模块
 * \param[in]    argc 由进程管理器传入的参数，表示从何种模式进入
 * \param[in]    argv 无用参数
 * \param[out]   none
 * \return       the result
 * \retval       0 sucess
 * \retval       1 failed
 * \ingroup      video_main
 * \note
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    DC dc_status;
    app_result_e result;

    g_video_enter_mode = (app_param_e) argc;

    /* 读入VM变量 */
    _video_read_var();

    ui_get_DC_status(&dc_status);
    ui_set_pen_color(Color_WHITE_def);
    ui_set_backgd_color(Color_BLACK_def);

    result = _video_app_init();

    /*场景管理器*/
    while (g_video_scene_next != SCENE_EXIT)
    {
        switch (g_video_scene_next)
        {
            case SCENE_FILELIST:
            result = video_filelist();
            break;

            case SCENE_OPTIONMENU:
            result = video_option_menulist();
            break;

            case SCENE_PLAY:
            result = video_scene_playing();
            break;

            case SCENE_MENU:
            result = video_menu();

            default:
            g_video_scene_next = SCENE_EXIT; //exit
            break;
        }
        _video_select_next_scene(&result);
    }

    ui_set_pen_color(dc_status.pen_color);
    ui_set_backgd_color(dc_status.backgd_color);

    _deal_video_result(result);
    //保存VM变量
    fsel_get_location((void *) &g_video_var.path.file_path.flist_location, FSEL_TYPE_PLAYLIST);
    sys_vm_write(&g_video_var, VM_AP_VIDEO);
    _video_save_comval();
    _video_app_deinit();

    return result;
}

