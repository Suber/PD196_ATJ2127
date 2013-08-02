/*******************************************************************************
 *                              US212A
 *                            Module: BROWSER_MAIN
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-10-25 11:28:13           1.0              build this file
 *******************************************************************************/
/*!
 * \file     browser_main.c
 * \brief    browser主模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2011-10-25
 *******************************************************************************/
/* 本应用的头文件 */
#include "browser.h"
#include "browser_menu_cfg.h"

/*! \brief browser应用VM变量*/
browser_var_t g_browser_var;

/*! \brief 通用VM变量*/
comval_t g_comval;

/*! \brief browser下一个场景 */
scene_browser_e g_browser_scene_next;

/*! \brief browser 挂载文件系统的mount ID*/
int32 browser_mount;

/*! \brief 应用进入模式*/
app_param_e g_browser_enter_mode;

/*! \brief 是否是从其他ap返回browser*/
bool g_return_browser;

/*! \brief browser文件名 */
uint8 browser_file_name[LIST_ITEM_BUFFER_LEN];

/*! \brief 应用空间的软定时器数组 */
app_timer_t browser_app_timer_vector[APP_TIMER_TOTAL];

#ifndef WIN32
OS_STK *ptos = (OS_STK *) AP_BROWSER_STK_POS;
#endif
INT8U prio = AP_BROWSER_PRIO;

/*! \brief 公共资源文件(common style)名 */
const uint8 com_themes[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };

/*! \brief browser资源文件名 */
const uint8 browser_sty[] =
{ "browser.sty" };

/*! \brief browser菜单数据文件名 */
const uint8 browser_mcg[] =
{ "browser.mcg" };

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser 获取后台引擎盘符
 * \param[in]    none
 * \param[out]   none
 * \return       disk_type
 * \ingroup      browser_main
 * \note
 *******************************************************************************/
static uint8 _get_music_disk_type(void)
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
 *	  browser 读取应用VM全局变量,初始化全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      browser_main
 * \note
 * \li  关于盘符初始化默认盘符设定，需要考虑是否是从其它ap返回，后台是否播歌
 *      卡盘是否存在，根据这些条件设定盘符。其中默认设置为主盘。
 *******************************************************************************/
static void _browser_read_var(void)
{
    int result;
    setting_comval_t temp_val;
    engine_type_e engine_type;

    //读取common VM公共变量信息
    result = sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));

    //初始化系统的VM变量
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        temp_val.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    // 读取browser VM公共变量
    result = sys_vm_read(&g_browser_var, VM_AP_BROWSER, sizeof(browser_var_t));

    // 第一次进入AP,初始化VM变量
    if (g_browser_var.magic != MAGIC_BROWSER)
    {
        g_browser_var.magic = MAGIC_BROWSER;
        libc_memset(&g_browser_var.path.file_path, 0, sizeof(musfile_location_u));
        g_browser_var.path.file_path.dirlocation.disk = DISK_BASE;
        g_browser_var.prev_enter_mode = PARAM_FROM_MAINMENU;
    }

    if ((g_browser_enter_mode == PARAM_FROM_MAINMENU) || (g_browser_enter_mode == PARAM_FROM_RADIO)
            || (g_browser_enter_mode == PARAM_FROM_RECORD))
    {
        //从主界面进入，则默认成主盘
        g_browser_var.path.file_path.dirlocation.disk = DISK_BASE;
        g_return_browser = FALSE;
    }
    else
    {
        //其它情形检测是否存在卡盘
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            //检测不到卡盘，则初始化成默认盘符
            g_browser_var.path.file_path.dirlocation.disk = DISK_BASE;
        }
        g_return_browser = TRUE;
    }

    //一定要强制写成COMMONDIR
    g_browser_var.path.file_source = FSEL_TYPE_COMMONDIR;

    //引擎存在则获取当前播放文件的路径信息
    engine_type = get_engine_type();
    if (engine_type == ENGINE_MUSIC)
    {
        //更改盘符
        g_browser_var.path.file_path.dirlocation.disk = _get_music_disk_type();
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  browser 初始化进入的场景
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      browser_main
 * \note
 * \li   检测到卡，进入菜单场景,否则进入列表场景
 *******************************************************************************/
static void _browser_data_init(void)
{
    if (g_return_browser == FALSE)
    {
        //从主界面进入browser需要先判断是否有卡盘，因此先
        //进入menu假场景
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            g_browser_scene_next = SCENE_FILELIST;
        }
        else
        {
            g_browser_scene_next = SCENE_MENU;
        }
    }
    else
    {
        //从其他ap返回browser直接进入filelist场景
        g_browser_scene_next = SCENE_FILELIST;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 *    browser 应用的初始化
 * \param[in]    void
 * \param[out]   void
 * \return       app_result_e
 * \retval       RESULT_NULL  初始化成功
 * \retval       其它值       文件选择器初始化失败，返回main ap
 * \ingroup      browser_main
 * \note
 *******************************************************************************/
app_result_e _browser_app_init(void)
{
    app_result_e result = TRUE;

    //初始化applib库，前台AP
    applib_init(APP_ID_BROWSER, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(browser_app_timer_vector, APP_TIMER_TOTAL);

    //this_headbar_icon_id = HEADBAR_ICON_BROWSER;

    //初始化 applib 消息模块
    applib_message_init(browser_msg_callback);

    //初始化common的两个定时器函数，必须在初始化完软定时器后调用
    sys_timer_init();

    //初始ui资源文件
    ui_res_open(com_themes[g_comval.theme], UI_COM);

    ui_res_open(browser_sty, UI_AP);

    //打开前台应用菜单配置脚本
    com_open_confmenu_file(browser_mcg);

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    //确定初始化进入的场景
    _browser_data_init();

    //初始化文件选择器
    result = browser_disk_initial();
    if (result != RESULT_NULL)
    {
        g_browser_scene_next = SCENE_EXIT;
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    browser 应用的退出
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      browser_main
 * \note
 *******************************************************************************/
bool _browser_app_deinit(void)
{
    //文件选择器退出
    fsel_exit();

    //卸载common定时器
    sys_timer_exit();

    //关闭应用菜单配置脚本
    com_close_confmenu_file();

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
 *    browser保存comval VM变量
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      browser_main
 * \note
 * \li  对于模拟器，如果不设置读取的缓冲区为512字节，有可能在模拟器上运行不正确
 *      因此采用buffer读取方式
 *******************************************************************************/
static void _browser_save_comval(void)
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
 *	  browser结果处理模块
 * \param[in]    none
 * \param[out]   none
 * \return       the result app_result_e
 * \retval       根据选择的文件后缀发消息创建各自ap
 * \ingroup      browser_main
 * \note
 * \li  在该模块会根据相应应用的bitmap过滤文件总数和文件序数
 * \li  在选择从browser返回和进入其它ap之前，需要记录当前路径
 *      使用prev_enter_mode记录上一次进入模式
 *      使用enter_mode记录当前进入模式
 *      main/record/radion <==> browser <==> music/picture/video/ebook
 *******************************************************************************/
static app_result_e _browser_proc_result(app_result_e result)
{
    //应用消息结构实体
    msg_apps_t msg;
    uint32 bit_map;
    bool need_modify_location = FALSE;
    bool need_send_msg = TRUE;
    engine_type_e engine_type = get_engine_type();

    //if (g_return_browser == FALSE)
    //{
        //g_return_browser为FALSE表明是从主界面等进入，需要保存
        //这次进入的模式
    //    g_browser_var.prev_enter_mode = g_browser_enter_mode;
    //}
    //else

    if (g_return_browser == TRUE)
    {
        //如果不是从主界面或FM录音界面进入，则是从其它ap返回browser
        //需要知道上一个进入browser的入口参数
        g_browser_enter_mode = g_browser_var.prev_enter_mode;

        //以后返回路径都是main menu
        g_browser_var.prev_enter_mode = PARAM_FROM_MAINMENU;
    }

    if (result == RESULT_BROWSER_EXIT)
    {
        if (g_browser_enter_mode == PARAM_FROM_RECORD)
        {
            result = RESULT_RECORD_MEMU;
        }
        else if(g_browser_enter_mode == PARAM_FROM_RADIO)
        {
            //退出到FM应用
            result = RESULT_RADIO_MAINMENU;
        }
        else
        {
            //退出到主应用
            result = RESULT_MAIN_APP;
        }
    }

    //给process manager发送创建其它进程消息
    msg.type = MSG_CREAT_APP;
    //消息内容data[1]为传递给要创建的ap的入口参数，表明是由browser创建
    msg.content.data[1] = (uint8) PARAM_FROM_BROWSER;

    switch (result)
    {
        case RESULT_AUDIBLE_PLAY:
        msg.content.data[0] = APP_ID_MUSIC;
        bit_map = AUDIBLE_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_MUSIC_PLAY:
        msg.content.data[0] = APP_ID_MUSIC;
        if (g_browser_enter_mode != PARAM_FROM_MAINMENU)
        {
            //从record或radio进入时，都默认成record进入，因为二者在music ap
            //行为一致，不需要区分。只需要在返回browser时由browser做出判断
            //即可
            msg.content.data[1] = PARAM_FROM_RECORD;
        }
        bit_map = MUSIC_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_MOVIE_PLAY:
        msg.content.data[0] = APP_ID_VIDEO;
        bit_map = VIDEO_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_PICTURE_PLAY:
        msg.content.data[0] = APP_ID_PICTURE;
        bit_map = PICTURE_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_EBOOK_PLAY:
        msg.content.data[0] = APP_ID_EBOOK;
        bit_map = TEXT_BITMAP;
        need_modify_location = TRUE;
        g_browser_var.prev_enter_mode = g_browser_enter_mode;
        break;

        case RESULT_RADIO_MAINMENU:
        msg.content.data[0] = APP_ID_RADIO;
        break;

        case RESULT_RECORD_MEMU:
        msg.content.data[0] = APP_ID_RECORD;
        break;

        case RESULT_NOWPLAYING:
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;
            msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        }
        else
        {
            msg.content.data[0] = APP_ID_RADIO;

        }
        break;

        case RESULT_LASTPLAY:
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYED;
        if (engine_type == ENGINE_MUSIC)
        {
            msg.content.data[0] = APP_ID_MUSIC;
        }
        else
        {
            msg.content.data[0] = APP_ID_RADIO;
        }
        break;

        case RESULT_APP_QUIT:
        //由common默认处理的事件common已经发过消息，ap直接退出即可
        need_send_msg = FALSE;
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

    if (TRUE == need_modify_location)
    {
        //browser中文件总数和文件和相应
        //fsel_browser_set_location(&(g_browser_var.path.file_path.dirlocation), FSEL_TYPE_COMMONDIR);
        fsel_browser_modify_location(&(g_browser_var.path.file_path.dirlocation), bit_map);
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser下一个要调度的场景
 * \param[in]    app_result_e 返回值
 * \param[out]   处理后的返回值
 * \return       void
 * \ingroup      browser_main
 * \note
 * \li  browser有两个场景:菜单场景和列表场景，如果无卡插入，则只有列表场景
 *******************************************************************************/
void _browser_select_next_scene(app_result_e *result)
{
    app_result_e ui_result = *result;
    if (ui_result == RESULT_DIR_ERROR_NO_FILE)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //存在卡盘
            ui_result = RESULT_BROWSER_MENU;
        }
        else
        {
            ui_result = RESULT_MAIN_APP;
        }
    }

    switch (ui_result)
    {
        case RESULT_BROWSER_MENU:
        g_browser_scene_next = SCENE_MENU;
        break;

        case RESULT_BROWSER_FILELIST:
        g_browser_scene_next = SCENE_FILELIST;
        break;

        default:
        g_browser_scene_next = SCENE_EXIT;
        break;
    }
    *result = ui_result;
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser 应用的入口函数和场景管理器模块
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      browser_main
 * \note
 * \li   main函数在进入时根据argc判断进入模式
 * \li   在初始化完成之后就进入场景调度循环
 * \li   退出场景循环后对返回值进行判断，并创建相应的应用
 *******************************************************************************/
int main(int argc, char *argv[])
{
    app_result_e result;

    g_browser_enter_mode = (app_param_e) argc;

    //读入VM变量
    _browser_read_var();

    //applib等初始化
    result = _browser_app_init();

    // 场景循环
    while (g_browser_scene_next != SCENE_EXIT)
    {
        switch (g_browser_scene_next)
        {
            case SCENE_FILELIST:
            result = browser_scene_filelist();
            break;

            case SCENE_MENU:
            result = browser_scene_menu();
            break;

            default:
            g_browser_scene_next = SCENE_EXIT;
            break;
        }

        //选择下一个调度场景
        _browser_select_next_scene(&result);
    }

    //处理控件，场景返回值，转换成消息处理
    _browser_proc_result(result);

    //保存AP VM变量
    sys_vm_write(&g_browser_var, VM_AP_BROWSER);

    //保存系统 VM变量
    _browser_save_comval();

    //applib等退出处理
    _browser_app_deinit();
    return result;
}

