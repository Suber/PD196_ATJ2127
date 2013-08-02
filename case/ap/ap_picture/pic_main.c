/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-19 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_main.c
 * \brief    picture主模块，负责进程初始化，退出处理，场景调度
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include "picture.h"
#include "picture_preview.h"
#include "music_common.h"
#include "pic_menu_cfg.h"

// 全局变量定义

/*! \brief picture应用VM变量*/
g_picture_var_t g_picture_var;

/*! \brief 通用VM变量*/
comval_t g_comval;

/*! \brief picture 挂载文件系统的mount ID*/
int32 picture_mount;

#ifndef PC
/*! \brief picture使用的堆栈指针 */
OS_STK *ptos = (OS_STK *) AP_PICTURE_STK_POS;
#endif

/*! \brief picture使用的进程优先级 */
INT8U prio = AP_PICTURE_PRIO;

/*! \brief 传给codec的图片window */
wregion_t region_in;

/*! \brief 实际解码用的图片window */
wregion_t region_out;

/*! \brief 图片解码线程句柄 */
void *picture_handle;

/*! \brief 传给codec的图片解码信息 */
ColorLcmInfor g_picture_userinfor;

/*! \brief 应用进入模式*/
app_param_e g_picture_enter_mode;

/*! \brief 自动播放或滚动显示文件名的定时器ID */
int8 timer_pic_play;

/*! \brief 自动播放切换文件标志 */
uint8 g_picture_change_file;

/*! \brief 自动播放标志 */
bool g_picture_auto_play;

/*! \brief 文件名需要滚动显示标志 */
bool g_picture_scroll_flag;

/*! \brief picture下一个场景 */
scene_picture_e g_picture_scene_next;

/*! \brief picture上一个场景*/
scene_picture_e g_picture_scene_prev;

/*! \brief 需要重新启动music引擎标志 */
bool g_need_resume_engine;

/*! \brief 图片解码库是否空闲标志 */
bool g_mmm_id_free;

/*! \brief 文件名缓冲区 */
uint8 g_picture_filename[LIST_ITEM_BUFFER_LEN];

/*! \brief 文件选择器是否初始化 */
bool is_fsel_init;

/*! \brief 是否关闭music后台 */
uint8 g_stop_music;

/*! \brief 文件总数 */
uint16 g_file_total;

/*! \brief 错误文件总数*/
uint16 g_file_err;

/*! \brief 是否删除文件*/
uint8  g_del_file;

/*! \brief 应用软定时器数组指针，指向应用空间的软定时器数组 */
app_timer_t picture_app_timer_vector[APP_TIMER_TOTAL];

/*! \brief 公共资源文件(common style)名 */
const uint8 com_themes[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };

/*! \brief picture资源文件名 */
const uint8 picture_sty[] =
{ "picture.sty" };

/*! \brief picture菜单数据文件名 */
const uint8 picture_mcg[] =
{ "picture.mcg" };

/******************************************************************************/
/*!
 * \par  Description:
 *    获取后台待播放文件的盘符
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_main
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
 *	  picture 读取应用VM全局变量,初始化全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_main
 * \note
 *******************************************************************************/
static void _pic_read_var(void)
{
    int result;
    setting_comval_t temp_val;
    engine_type_e engine_type = get_engine_type();

    /*读取common VM公共变量信息*/
    result = sys_vm_read(&temp_val, VM_AP_SETTING, sizeof(setting_comval_t));

    /*初始化系统的VM变量*/
    if (temp_val.g_comval.magic != MAGIC_SETTING)
    {
        temp_val.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&temp_val);
        sys_vm_write(&temp_val, VM_AP_SETTING);
    }
    libc_memcpy(&g_comval, &temp_val.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);

    //读取photo VM公共变量信息
    result = sys_vm_read(&g_picture_var, VM_AP_PICTURE, sizeof(g_picture_var));

    //第一次进入AP，初始化VM变量
    if (g_picture_var.magic != MAGIC_PICTURE)
    {
        g_picture_var.magic = MAGIC_PICTURE;

        //读取配置值，是否支持缩略图
        g_picture_var.thumbnail_flag = (uint8) com_get_config_default(PHOTO_AP_ID_THUMBNAIL);
        g_picture_var.revolve_flag = (uint8) com_get_config_default(PHOTO_AP_ID_REVOLVE);
        libc_memset(&g_picture_var.path.file_path, 0, sizeof(musfile_location_u));
        g_picture_var.path.file_path.dirlocation.disk = DISK_BASE;
        g_picture_var.prev_enter_mode = PARAM_FROM_MAINMENU;
    }

    if (g_picture_enter_mode == PARAM_FROM_PLAYLIST)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            //检测不到卡盘，则初始化成默认盘符
            g_picture_var.path.file_path.dirlocation.disk = DISK_BASE;
        }
    }
    else
    {
        g_picture_var.path.file_path.dirlocation.disk = DISK_C;
    }

    if (g_picture_enter_mode == PARAM_FROM_BROWSER)
    {
        //从browser或recode进入
        sys_vm_read(&g_picture_var.path, VM_AP_BROWSER, sizeof(file_path_info_t));
    }

    //引擎存在则获取当前播放文件的路径信息
    if (engine_type == ENGINE_MUSIC)
    {
        //更改盘符
        g_picture_var.path.file_path.dirlocation.disk = _get_music_disk_type();
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *    picture 应用返回值处理函数，将对应的返回值转换成消息处理
 * \param[in]    result app_result_e 各个场景返回值
 * \param[out]   none
 * \return       the result
 * \ingroup      pic_main
 * \note
 * \li   RESULT_RETURN_BROWSER 返回browser应用
 * \li   RESULT_NOWPLAYING/RESULT_LASTPLAY     返回music/radio对应的前台UI进程
 * \li   RESULT_CREATE_LIST_APP 进入播放列表ap创建图片播放列表
 * \li   RESULT_APP_QUIT        由common处理的事件会返回该返回值，picture不再做处理
 * \li   其它返回值             返回main
 *******************************************************************************/
static app_result_e _deal_picture_result(app_result_e pic_result)
{
    msg_apps_t msg;
    bool need_send_msg = TRUE;
    engine_type_e engine_type = get_engine_type();

    //给process manager发送创建其它进程消息
    msg.type = MSG_CREAT_APP;
    msg.content.data[1] = (uint8) PARAM_FROM_PICTURE;

    switch (pic_result)
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

        }
        break;

        case RESULT_CREATE_LIST_APP:
        msg.content.data[0] = APP_ID_PLAYLIST;
        //只更新当前盘符的playlist
        if (g_picture_var.path.file_path.dirlocation.disk == DISK_C)
        {
            msg.content.data[1] = (PLIST_DISK_C | PARAM_FROM_PICTURE);
        }
        else
        {
            msg.content.data[1] = (PLIST_DISK_H | PARAM_FROM_PICTURE);
        }
        //保存进入模式
        g_picture_var.prev_enter_mode = g_picture_enter_mode;
        break;

        case RESULT_APP_QUIT:
        need_send_msg = FALSE;
        //保存进入模式
        g_picture_var.prev_enter_mode = g_picture_enter_mode;
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
    return pic_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    picture 应用的初始化
 * \param[in]    void
 * \param[out]   void
 * \return       the result
 * \retval       1  success
 * \retval       0  fail
 * \ingroup      pic_main
 * \note
 *******************************************************************************/
app_result_e _pic_app_init(void)
{
    app_result_e result = RESULT_NULL;

    //初始化applib库，前台AP
    applib_init(APP_ID_PICTURE, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(picture_app_timer_vector, APP_TIMER_TOTAL);

    //初始化 applib 消息模块
    applib_message_init(pic_msg_callback);

    //初始化common的两个定时器函数，必须在初始化完软定时器后调用
    sys_timer_init();

    //初始ui资源文件
    ui_res_open(com_themes[g_comval.theme], UI_COM);

    ui_res_open(picture_sty, UI_AP);

    //打开前台应用菜单配置脚本
    com_open_confmenu_file(picture_mcg);

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif

    result = pic_data_init();
    if (result != RESULT_NULL)
    {
        is_fsel_init = FALSE;
        g_picture_scene_next = SCENE_EXIT;
        return result;
    }

    result = pic_disk_initial();
    if (result == RESULT_MAIN_APP)
    {
        g_picture_scene_next = SCENE_EXIT;
        return result;
    }
    else if(result == RESULT_PIC_MENU)
    {
        g_picture_scene_next = SCENE_MENU;
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
 *    picture 应用的退出处理
 * \param[in]    void
 * \param[out]   void
 * \return       the result
 * \retval       1  success
 * \retval       0  fail
 * \ingroup      pic_main
 * \note
 *******************************************************************************/
bool _pic_app_deinit(void)
{
    //文件选择器退出
    if (is_fsel_init == TRUE)
    {
        fsel_exit();
    }
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
 *    picture 保存comval VM变量
 * \param[in]    void
 * \param[out]   void
 * \return       void
 * \ingroup      picture
 * \note
 *******************************************************************************/
static void _pic_save_comval(void)
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
 *	  picture下一个要调度的场景
 * \param[in]    app_result_e 返回值
 * \param[out]   处理后的返回值
 * \return       void
 * \ingroup      pic_main
 * \note
 * \li  picture有5个场景:文件列表/缩略图场景,option菜单项场景，播放场景和
 菜单场景(有卡时才有)
 *******************************************************************************/
static void _pic_select_next_scene(app_result_e *result)
{
    app_result_e ui_result = *result;
    g_picture_scene_prev = g_picture_scene_next;

    if (ui_result == RESULT_DIR_ERROR_NO_FILE)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) != -1)
        {
            //存在卡盘
            ui_result = RESULT_PIC_MENU;
        }
        else
        {
            //返回main ap
            ui_result = RESULT_MAIN_APP;
        }
    }
    else if( ui_result == RESULT_CARD_OUT_ERROR)
    {
        if(g_picture_enter_mode == PARAM_FROM_BROWSER)
        {
            ui_result = RESULT_RETURN_BROWSER;
        }
        else
        {
            ui_result = RESULT_MAIN_APP;
        }    
    }
    else
    {
        ;//nothing
    }
    
    switch (ui_result)
    {
        case RESULT_PIC_MENU:
        g_picture_scene_next = SCENE_MENU;
        break;

        case RESULT_PIC_OPTIONMENU:
        g_picture_scene_next = SCENE_OPTIONMENU;
        break;

        case RESULT_PIC_FILELIST:
        if ((g_picture_var.thumbnail_flag & VALUE_PREVIEW_MODULE_ON) == VALUE_PREVIEW_MODULE_ON)
        {
            g_picture_scene_next = SCENE_PREVIEW;
        }
        else
        {
            g_picture_scene_next = SCENE_FILELIST;
        }
        break;

        case RESULT_PICTURE_PLAY:
        g_picture_scene_next = SCENE_PLAY;
        break;

        default:
        g_picture_scene_next = SCENE_EXIT;
        break;
    }
    *result = ui_result;
    return;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 应用的入口函数和场景管理器模块
 * \param[in]    argc 由进程管理器传入的参数，表示从何种模式进入
 * \param[in]    argv 无用参数
 * \param[out]   none
 * \return       the result 由于应用返回值已经处理，这里返回值可以忽略
 * \ingroup      pic_main
 * \note
 * \li   main函数在进入时根据argc判断进入模式
 * \li   在初始化完成之后就进入场景调度循环
 * \li   退出场景循环后对返回值进行判断，并创建相应的应用
 *******************************************************************************/
int main(int argc, const char *argv[])
{
    DC dc_status;
    app_result_e result;

    //如何获知进入模式?
    g_picture_enter_mode = (app_param_e) argc;

    //读入VM变量
    _pic_read_var();

    ui_get_DC_status(&dc_status);
    ui_set_pen_color(Color_WHITE_def);
    ui_set_backgd_color(Color_BLACK_def);

    result = _pic_app_init();

    /*场景管理器*/
    while (g_picture_scene_next != SCENE_EXIT)
    {
        switch (g_picture_scene_next)
        {
            case SCENE_FILELIST:
            result = pic_filelist();
            break;

            case SCENE_OPTIONMENU:
            result = pic_option_menulist();
            break;

            case SCENE_PLAY:
            result = pic_play();
            break;

            case SCENE_PREVIEW:
            result = pic_preview();
            break;

            case SCENE_MENU:
            result = pic_menu();

            default:
            g_picture_scene_next = SCENE_EXIT; //exit
            break;
        }

        //选择下一个调度场景
        _pic_select_next_scene(&result);
    }

    ui_set_pen_color(dc_status.pen_color);
    ui_set_backgd_color(dc_status.backgd_color);

    //将返回值转换为消息发给进程管理器
    _deal_picture_result(result);
    //保存AP VM变量
    sys_vm_write(&g_picture_var, VM_AP_PICTURE);
    //保存系统 VM变量
    _pic_save_comval();
    //applib等退出处理
    _pic_app_deinit();

    return result;
}

