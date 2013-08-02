/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-13          1.0              create this file
 *******************************************************************************/
#include "app_music.h"

#include "app_music_playing.h"

#include "app_music_menu_config.h"

//globle variable
//存放当前场景变量
music_scene_e g_music_scene;

//进入music的方式
app_param_e g_enter_mode;

//进入play的操作
music_play_set_e g_play_set;

//显示标识
bool g_paint_flag;

//album art 解码标识
uint8 g_album_art;

//获取文件信息标识 bit0:获取文件ID3信息 bit1:获取文件路径信息
uint8 g_getfile_info;

//歌词存在标识
bool g_lyric_flag;

//播放控制器定时器id
int8 time_frush_id = 0xff;

//记录滚屏定时器状态,参见id3_deal_status_e定义
uint8 g_scroll_status;

//ID3滚屏定时器
int8 scroll_frush_id = 0xff;

//标记是否临时shuffle
uint8 g_shuffle_tmporary = FALSE;

//标记当前是否是audible
uint8 g_audible_now = FALSE;

//标志ab复读替换
bool g_ab_switch;

//当前文件总数及序号结构体
musfile_cnt_t g_file;

//连续错误歌曲数
uint16 g_error_num = 0;

//向前切
uint16 g_switch_prev = 0;
//存放id3信息
id3_info_t g_id3_info;

//标题缓冲区
uint8 g_title_buf[Id3BufSIZE];

//歌手缓冲区
uint8 g_artist_buf[ArtBufSIZE];

//专辑缓冲区
uint8 g_album_buf[ArtBufSIZE];

//滚屏内容
scroll_content_t g_scroll_content[3];

//滚屏控制参数变量
scroll_vars_t g_scroll_vars;

//region_t scroll_region;

//当前播放歌曲的文件名 歌词与album art使用
uint8 g_song_filename[64];

//专辑图片显示信息
album_picinfo_t album_art_info;

//刷新标志位变量
uint32 g_display_bitmap;

//文件路径来源信息保存，播放时候用
file_path_info_t g_file_path_info;

//浏览的路径信息，浏览时用
file_path_info_t g_browser_filepath;

//存放文件系统id
int32 g_music_mount_id = -1;

//当前播放状态信息
mengine_status_t g_music_status;

//当前播放时间和比特率信息
mengine_playinfo_t g_play_info;

//audible的断点信息
mmm_mp_bp_info_t g_audible_bkinfo;

//music ui的配置项信息
music_config_t g_music_config;

//系统全局变量
setting_comval_t g_setting_comval;

//查询引擎状态标志位
uint8 g_check_mengine_status;

//定时器计数
uint8 g_timer_counter;

//当前歌曲总时间
uint32 g_total_time;

//当前播放时间
uint32 g_cur_time;

//前一次记录播放时间
uint32 g_prev_time;

//对于nandflash，表示在VM区地址，对于其它介质，为句柄地址
uint32 g_bookmark_fp;

//快进退速率计数
uint8 g_speed_counter;

//歌词显示多屏显示标志位
bool g_lyric_multi_screen = FALSE;

//恢复默认设置eq标志
uint8 g_eq_restore;

//是否需要开PA标志
bool g_need_open_pa;

//滚屏状态机是否调度标志
bool g_scroll_deal;

//文件选择器模块类型
fsel_module_t g_fsel_module;

//前景背景颜色
DC g_dc_status;

//临时缓冲区，位于LCD的1k buffer
#ifndef PC
uint8 *g_temp_buffer_p = (uint8 *)LCD_BUF_ADDR;
#else
uint8 g_temp_buffer[1024];
uint8 *g_temp_buffer_p;
#endif

#ifdef PRINT_BANK_INFO
//打印标志位
int print_label;
#endif

//应用自己需要的软定时器个数
#define APP_TIMER_COUNT		2
/*!
 *  \brief
 *  app_timer_vector：应用软定时器数组指针，指向应用空间的软定时器数组
 */
app_timer_t music_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_MUSIC_STK_POS;
#endif

INT8U prio = AP_MUSIC_PRIO;
/******************************************************************************/
/*!
 * \par  Description:
 * \void _load_cfg(void)
 * \读取配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _load_cfg(void)
{
    //系统变量获取
    sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    if (g_setting_comval.g_comval.magic != MAGIC_SETTING)
    {
        //com_get_default_setting(&g_setting_comval);
        g_setting_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_comval);
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
    }
    com_set_sys_comval(&g_setting_comval.g_comval);
    //AP_MUSIC_PRIO music 变量获取
    sys_vm_read(&g_music_config, VM_AP_MUSIC, sizeof(music_config_t));

    if (g_music_config.magic != MAGIC_MUSIC)
    {
        g_music_config.magic = MAGIC_MUSIC;
        //是否存在audible 断点
        g_music_config.audible_bk_flag = 0;
        //播放速度
        g_music_config.playspeed = 0;

        //ab复读次数
        g_music_config.ab_count = 5;
        //ab复读间隔
        g_music_config.ab_gap = 1;
        //audible的章节模式
        g_music_config.section_mode = 0;
        //进入场景
        g_music_config.enter_scene = SCENE_LSTMENU;
        //启动引擎设置
        g_music_config.play_set = PLAY_START;

        //是否从browser进入
        g_music_config.browser_sel = FALSE;

        //以下get config就ok 现在先赋初值
        //id3获取功能
        g_music_config.id3_support = (uint8) com_get_config_default(MUSIC_ID3_SUPPORT);
        //歌词显示
        g_music_config.lrc_support = (uint8) com_get_config_default(MUSIC_LRC_SUPPORT);
        //album art显示
        g_music_config.album_art_support = (uint8) com_get_config_default(MUSIC_ALBUM_ART_SUPPORT);

        g_music_config.albumlist_sel = FALSE;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _save_cfg(void)
 * \保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
void _save_cfg(void)
{
    /*保存VM变量*/
    sys_vm_write(&g_music_config, VM_AP_MUSIC);
    sys_vm_write(&g_setting_comval, VM_AP_SETTING);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _check_bkinfo(void)
 * \检测断点有效性并设置初始化默认盘符
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \ingroup      music_main
 * \note
 * \li   该函数在standby启动时调用，用于判断music engine记录的断点信息是否有效
 music ui据此设置初始化默认盘符和相应的类型
 */
/*******************************************************************************/
void _check_bkinfo(void)
{
    uint8 disk_type;
    mengine_config_t temp_config;

    sys_vm_read(&temp_config, VM_AP_MENGINE, sizeof(mengine_config_t));

    disk_type = temp_config.location.dirlocation.disk;

    if (disk_type == DISK_H)
    {
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            disk_type = DISK_C;
        }
    }

    g_browser_filepath.file_path.plist_location.disk = disk_type;
    g_file_path_info.file_path.plist_location.disk = disk_type;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_init(void)
 * \读取配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 * \li  初始化文件选择器时设置默认盘符为主盘，文件类型为COMMONDIR
 * \li  如果后台有music播放或从browser等进入，同步location
 * \li  music ui默认没有盘符记忆，从playlist进入和退出，盘符写入browser VM变量
 */
/*******************************************************************************/
bool _app_init(void)
{
    bool app_init_ret;

    engine_type_e engine_type;

    //当前音乐的类型
    id3_type_e cur_type;

    //读取配置信息 (包括vm信息)
    _load_cfg();

    //加载ui资源文件
    ui_res_open("music.sty", UI_AP);
    ui_res_open("common.sty", UI_COM);

    com_open_confmenu_file("music.mcg");

#ifdef PC
    g_temp_buffer_p = g_temp_buffer;
    load_menulist_simfunc(get_menu_func, get_callback_func, get_menu_option);
#endif

    //初始化applib库，前台AP
    applib_init(APP_ID_MUSIC, APP_TYPE_GUI);

    applib_message_init(music_message_deal);

    //初始化软定时器
    init_app_timers(music_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //必须在init_app_timers 后面调用
    sys_timer_init();

    ui_get_DC_status(&g_dc_status);
    ui_set_pen_color(Color_WHITE_def);
    ui_set_backgd_color(Color_BLACK_def);

    //初始化文件选择器
    g_file_path_info.file_path.plist_location.disk = DISK_C;

    //初始化文件选择器为目录模式
    g_file_path_info.file_source = FSEL_TYPE_COMMONDIR;

    //引擎存在则获取当前播放文件的路径信息
    engine_type = get_engine_type();
    if (engine_type == ENGINE_MUSIC)
    {
        //获取文件路径
        music_get_filepath(&g_file_path_info);

        //同步浏览文件的location为后台播放的location
        libc_memcpy(&g_browser_filepath, &g_file_path_info, sizeof(file_path_info_t));
    }

    //盘符保持一致
    g_browser_filepath.file_path.plist_location.disk = g_file_path_info.file_path.plist_location.disk;

    //从browser选择了歌曲，则从vram中读出路径信息
    if ((g_enter_mode == PARAM_FROM_BROWSER) || (g_enter_mode == PARAM_FROM_PLAYLIST) || (g_enter_mode
            == PARAM_FROM_RECORD))
    {
        sys_vm_read(&g_browser_filepath, VM_AP_BROWSER, sizeof(file_path_info_t));

        if (g_enter_mode == PARAM_FROM_PLAYLIST)
        {
            //清除断点标志
            g_music_config.audible_bk_flag = FALSE;

            //使路径信息处于无效状态，恢复默认状态
            libc_memset(&(g_browser_filepath.file_path.dirlocation.filename), 0, 4);
        }

        //从brwoser选择了歌曲播放
        libc_memcpy(&g_file_path_info, &g_browser_filepath, sizeof(file_path_info_t));
    }
    else
    {
        if ((g_enter_mode == PARAM_BACKTO_PLAYED) || (g_enter_mode == PARAM_BACKTO_PLAYING))
        {
            if (engine_type == ENGINE_NULL)
            {
                //检测断点有效性并设置初始化默认盘符
                _check_bkinfo();
            }
        }
    }

    //根据后缀名检测文件类型music/audible
    cur_type = play_scene_checktype(g_file_path_info.file_path.dirlocation.filename);

    if ((cur_type == ID3_TYPE_AAX) || (cur_type == ID3_TYPE_AA))
    {
        g_audible_now = TRUE;
    }
    else
    {
        g_audible_now = FALSE;
    }

    //文件选择器初始化
    app_init_ret = music_file_init(&g_file_path_info);

    if (app_init_ret == TRUE)
    {
        //检测书签文件是否存在,如不存在则创建
        check_bookmark_exist();

        //设置获取文件路径信息标志位
        g_getfile_info |= GET_FILEINFO_FILEPATH;
    }
    return app_init_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _app_deinit(void)
 * \退出app的功能函数 保存配置信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
bool _app_deinit(void)
{
    //save config 写vram
    //文件选择器退出
    fsel_exit();

    //备份系统计时器
    sys_timer_exit();

    //执行applib库的注销操作
    applib_quit();

    //关闭应用菜单配置脚本
    com_close_confmenu_file();

    //关闭资源文件
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);

    ui_set_pen_color(g_dc_status.pen_color);
    ui_set_backgd_color(g_dc_status.backgd_color);

    //save config 写vram
    _save_cfg();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _scene_dispatch(music_enter_mode_e enter_mode)
 * \场景调度器
 * \param[in]    enter_mode 进入模式 根据进入模式选择第一个进入的场景
 * \param[out]   none
 * \return       app_result_e the result music根据该返回值确认下一个要创建的前台ap
 * \retval       RESULT_MAIN_APP 返回main ap
 * \retval       RESULT_RETURN_BROWSER 返回browser ap
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
app_result_e _scene_dispatch(app_param_e enter_mode)
{
    app_result_e scene_result;
    //TRUE:menu;FALSE:list
    bool list_menu = TRUE;
    //TRUE:music set;FALSE:audible set
    bool music_audible = TRUE;

    switch (g_enter_mode)
    {
        //从main menu选择了music图标
        case PARAM_FROM_MAINMENU:
        g_music_scene = SCENE_LSTMENU;
        break;

        //从brower选择了文件播放//设置播放文件发送消息给引擎
        case PARAM_FROM_RECORD:
        case PARAM_FROM_BROWSER:
        g_play_set = PLAY_START;
        g_music_config.browser_sel = TRUE;
        g_music_scene = SCENE_PLAYING;
        break;

        //从main menu选择了playnow图标
        //从其他ap的option菜单选择了正在播放/上次播放
        case PARAM_BACKTO_PLAYING:
        case PARAM_BACKTO_PLAYED:
        g_play_set = PLAY_KEEP;
        g_music_scene = SCENE_PLAYING;
        break;

        //其他情况
        default:
        g_music_scene = SCENE_LSTMENU;
        break;
    }

    //场景调度循环
    while (g_music_scene != SCENE_EXIT)
    {
        switch (g_music_scene)
        {
            //进入listmenu场景
            case SCENE_LSTMENU:

            scene_result = music_scene_listmenu(list_menu, &g_browser_filepath);

            switch (scene_result)
            {
                //选择文件 进入播放场景
                case RESULT_PLAYING_START:
                case RESULT_PLAYING_SHUFFLE:
                g_music_scene = SCENE_PLAYING;
                g_play_set = PLAY_START;
                if (scene_result == RESULT_PLAYING_SHUFFLE)
                {
                    g_shuffle_tmporary = TRUE;
                }
                else
                {
                    g_shuffle_tmporary = FALSE;
                }
                //playlist选择文件
                g_music_config.browser_sel = FALSE;
                break;

                //进入播放场景 不改变状态
                case RESULT_PLAYING_KEEP:
                g_music_scene = SCENE_PLAYING;
                g_play_set = PLAY_KEEP;
                break;

                //选择audible续播
                case RESULT_AUDIBLE_RESUME:
                g_music_scene = SCENE_PLAYING;
                g_play_set = PLAY_AUDIBLE_RESUME;
                if (g_file_path_info.file_source < FSEL_TYPE_SDFILE)
                {
                    g_music_config.browser_sel = TRUE;
                }
                else
                {
                    //playlist选择文件
                    g_music_config.browser_sel = FALSE;
                }
                break;

                //进入listmenu的菜单列表
                case RESULT_CARD_OUT_ERROR:
                g_music_scene = SCENE_LSTMENU;
                //强制修改music盘符
                g_browser_filepath.file_path.dirlocation.disk = DISK_C;
                g_file_path_info.file_path.dirlocation.disk = DISK_C;
                list_menu = TRUE;
                break;

                //其他情况退出场景调度
                default:
                g_music_scene = SCENE_EXIT;
                break;
            }
            break;

            //进入播放场景
            case SCENE_PLAYING:

            scene_result = music_scene_playing(g_enter_mode, &g_file_path_info, g_play_set);

            switch (scene_result)
            {
                //进入listmenu的文件列表
                case RESULT_LISTMENU_LIST:
                if (g_play_set == PLAY_AUDIBLE_RESUME)
                {
                    //从audible续播菜单进入，直接返回菜单列表
                    g_music_scene = SCENE_LSTMENU;
                    list_menu = TRUE;
                }
                else
                {
                    //其它情形先返回列表界面
                    g_music_scene = SCENE_LSTMENU;
                    list_menu = FALSE;
                }

                break;

                //进入listmenu的菜单列表
                case RESULT_CARD_OUT_ERROR:
                case RESULT_LISTMENU_MENU:
                g_music_scene = SCENE_LSTMENU;
                list_menu = TRUE;
                break;

                //进入audible的设置菜单
                case RESULT_SETMENU_AUDIBLE:
                g_music_scene = SCENE_SETMENU;
                music_audible = FALSE;
                break;

                //进入music的设置菜单
                case RESULT_SETMENU_MUSIC:
                g_music_scene = SCENE_SETMENU;
                music_audible = TRUE;
                break;

                //其他情况退出场景调度
                default:
                g_music_scene = SCENE_EXIT;
                break;
            }
            break;

            //进入setmenu场景
            case SCENE_SETMENU:

            scene_result = music_scene_setmenu(music_audible);

            switch (scene_result)
            {
                case RESULT_PLAYING_SCENE:
                case RESULT_PLAYING_KEEP:
                //进入播放场景 不改变状态 record正在播放/上一次播放
                g_music_scene = SCENE_PLAYING;
                g_play_set = PLAY_KEEP;
                break;

                case RESULT_CARD_OUT_ERROR:
                case RESULT_LISTMENU_MENU:
                //进入listmenu的菜单列表
                g_music_scene = SCENE_LSTMENU;
                list_menu = TRUE;
                break;
                default:
                //退出场景调度
                g_music_scene = SCENE_EXIT;
                break;
            }
            break;

            default:
            break;
        }
    }
    return scene_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int main(int argc  const char *argv[])
 * \app入口函数
 * \param[in]    argc  para1
 * \param[in]    argc  para2
 * \param[out]   none
 * \return       int the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_main
 * \note
 */
/*******************************************************************************/
int main(int argc, const char *argv[])
{

    app_result_e retval;
    g_enter_mode = argc;//进入music的模式

    //初始化
    if (_app_init() == TRUE)
    {
        retval = _scene_dispatch(argc);//场景调度器
    }
    else
    {
        //提示出错
        //gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        retval = RESULT_MAIN_APP;
    }
    //处理场景调度处理结果
    _scene_result_deal(retval);
    _app_deinit();//退出
    return retval;
}
