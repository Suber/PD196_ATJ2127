/*******************************************************************************
 *                              US212A
 *                            Module: MainMenu
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-05 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     mainmenu_main.c
 * \brief    MainMenu的main函数模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/05
 *******************************************************************************/
#include  "main_menu.h"

/*全局变量定义*/
//应用自己需要的软定时器个数
#define APP_TIMER_COUNT		2
/*!
 *  \brief
 *  app_timer_vector：应用软定时器数组指针，指向应用空间的软定时器数组
 */
app_timer_t g_mainmenu_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];
/*mainmenu应用需要保存到vram的数据*/
g_mainmenu_var_t g_mainmenu_var;
/*系统的全局变量*/
setting_comval_t g_setting_comval;
/*激活的ap ID*/
uint16 g_active_index = 0;
/*主界面显示的应用图标总数，如果后台引擎存在，应用图标总数 = 应用总数+1*/
uint16 g_item_amount;
/*应用图片的STYLE ID数组*/
uint16 ap_pic_box[MAX_APP_NUM + 1];

uint16 ap_pic_box_bg[MAX_APP_NUM + 1];

/*应用名称的STYLE ID数组，例如音乐，视频*/
uint16 ap_text_box[FRAME_NUM][MAX_APP_NUM + 1];
/*应用信息的STYLE ID数组，例如总共：*/
uint16 ap_info_box[FRAME_NUM][MAX_APP_NUM + 1];
/*格式化显示ap标题的RES ID的文本框*/
uint16 ap_string_box[MAX_APP_NUM + 1];
/*格式化显示ap信息的RES ID的文本框*/
uint16 ap_string_info_box[MAX_APP_NUM + 1];
/*应用的文件总数*/
uint16 ap_file_amount[MAX_APP_NUM + 1];
/*ap文件数和相关动态显示信息的字符串数组*/
uint8 ap_string_infor[MAX_INFO_LEN];
//音乐标题缓冲区
uint8 g_title_buf[Id3BufSize];
//存放文件系统id
int32 g_music_mount_id;
//文件路径来源信息保存
file_path_info_t g_file_path_info;
//存放id3信息
id3_info_t g_id3_info;
//当前播放状态信息
mengine_status_t g_music_status;
//是否需要初始化文件选择器
bool need_fsel_init = TRUE;
//是否检查切歌标志
bool need_check_status = FALSE;
//第几帧动画
uint8 g_frame_num = 0;
//向上还是向下切换图片，对于上下切换显示的动态图片不一样的UI
mainmenu_ui_result_e g_direction;
//定时器ID
int8 scroll_timer = -1;
//查询状态定时器ID
int8 status_timer = -1;
//fm频点
uint32 g_fm_feq;
//记录各应用的文件总数，下标是各应用的ID编号
uint16 g_ap_file_num[MAX_APP_NUM];
//headbar更新标志
uint8 headbar_update;
//是否检测music engine状态标志位
uint8 g_check_mengine_status;

/*main中要填充这两个值，由系统人员分配*/
#ifndef PC
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS;
#endif
INT8U prio = AP_FRONT_LOW_PRIO;

#ifdef PC
extern menu_cb_func get_menu_func(uint16 str_id);
extern menu_cb_leaf get_callback_func(uint16 str_id);
extern menu_cb_option get_menu_option(uint16 str_id);
#endif

//用于有多套主界面风格
const uint8 themes_com[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };

const uint8 themes_ap[5][13] =
{ "mainmenu.sty", "mainmenu.sty", "mainmenu.sty", "mainmenu.sty", "mainmenu.sty" };


/******************************************************************************/
/*!
 * \par  Description:
 *	  获取fm频率
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
void _radio_get_freq(void)
{
    msg_apps_t msg;
    msg_reply_t msg_reply;
    uint16 temp_freq;

    msg.type = MSG_FMENGINE_GET_FREQ;
    msg_reply.content = &temp_freq;

    //发送同步消息
    send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        ;//do nothing
    }
    g_fm_feq = (uint32)temp_freq + 0x10000;//后台回复的频率舍弃了最高位，需要补上
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_file_init(file_path_info_t* init_path_locat)
 * \初始化文件选择器
 * \param[in]    init_path_locat 初始化文件路径信息指针 para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      mainmenu_main.c
 * \note
 */
/*******************************************************************************/
bool music_file_init(file_path_info_t* init_path_locat)
{
    fsel_param_t init_param;//文件选择器初始化参数
    bool init_result = FALSE;

    //文件选择器初始化接口的参数设置
    init_param.disk = init_path_locat->file_path.plist_location.disk;
    init_param.fsel_type = init_path_locat->file_source;
    init_param.fsel_mode = (g_setting_comval.music_comval.music_repeat_mode
            | g_setting_comval.music_comval.music_shuffle);

    init_param.file_type_bit = MUSIC_BITMAP;

    //文件选择器初始化
    g_music_mount_id = fsel_init(&init_param, 0);

    if (g_music_mount_id != -1)
    {
        init_result = TRUE;
    }

    return init_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  MainMenu 读取应用VM全局变量,初始化全局变量
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _read_var(void)
{
    bool ret_val;
    uint8 engine_type;

    /*读取common VM公共变量信息*/
    sys_vm_read(&g_setting_comval, VM_AP_SETTING, sizeof(g_setting_comval));
    /*初始化系统的VM变量*/
    if (g_setting_comval.g_comval.magic != MAGIC_SETTING)
    {
        g_setting_comval.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_comval);
        sys_vm_write(&g_setting_comval, VM_AP_SETTING);
    }
    com_set_sys_comval(&g_setting_comval.g_comval);

    sys_vm_read(&g_mainmenu_var, VM_AP_MAINMENU, sizeof(g_mainmenu_var_t));
    /*第一次进入，初始化VM变量*/
    if (g_mainmenu_var.magic != MAGIC_MAINMENU)
    {
        g_mainmenu_var.magic = MAGIC_MAINMENU;

        /*从配置文件获取AP ID的数组，总数，默认活动ap项*/
        com_get_config_struct(MAINMENU_AP_ID_ARRAY, (uint8 *) &g_mainmenu_var.cfg_var, sizeof(g_mainmenu_var.cfg_var));

        sys_vm_write(&g_mainmenu_var, VM_AP_MAINMENU);
    }
    /*读出各应用文件总数*/
    sys_vm_read(&g_ap_file_num, VM_FILE_NUM, sizeof(g_ap_file_num));

    engine_type = get_engine_type();
    if (engine_type == ENGINE_RADIO)
    {
        _radio_get_freq();
    }
    else if (engine_type == ENGINE_MUSIC)
    {
        music_get_filepath(&g_file_path_info);

        if (need_fsel_init == TRUE)
        {
            //文件选择器初始化
            ret_val = music_file_init(&g_file_path_info);
            need_fsel_init = FALSE;
        }
        else
        {
            ret_val = TRUE;
        }

        if (ret_val == FALSE)
        {
            //后台处于播放状态而文件选择器初始化不成功
            //关闭后台
            music_close_engine();
        }
        else
        {
            //播放状态需要检测引擎状态
            if (get_engine_state() == ENGINE_STATE_PLAYING)
            {
                need_check_status = TRUE;
            }
        }
    }
    else
    {
        ;//nothing
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  applib初始化
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_init(void)
{
    /*读入VM变量*/
    _read_var();

    /*初始化applib库，前台AP*/
    applib_init(APP_ID_MAINMENU, APP_TYPE_GUI);

    /*初始化软定时器*/
    init_app_timers(g_mainmenu_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    /*初始化 applib 消息模块*/
    applib_message_init(mainmenu_app_msg_callback);

    /*初始化系统计时器*/
    sys_timer_init();

    /*打开ap和common的style文件*/
    ui_res_open(themes_ap[g_setting_comval.g_comval.theme], UI_AP);
    ui_res_open(themes_com[g_setting_comval.g_comval.theme], UI_COM);

    /*打开菜单配置文件*/
    com_open_confmenu_file("mainmenu.mcg");

#ifdef PC
    load_menulist_simfunc(get_menu_func, get_callback_func, get_menu_option);
#endif
}


/******************************************************************************/
/*!
 * \par  Description:
 *	  applib退出
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      mainmenu
 * \note
 *******************************************************************************/
static void _app_exit(void)
{
    if (need_fsel_init == FALSE)
    {
        //文件选择器退出
        fsel_exit();
    }
    /*备份系统计时器*/
    sys_timer_exit();

    /*保存VM变量*/
    sys_vm_write(&g_mainmenu_var, VM_AP_MAINMENU);
    sys_vm_write(&g_setting_comval, VM_AP_SETTING);

    /*关闭ui资源文件*/
    ui_res_close(UI_AP);
    ui_res_close(UI_COM);

    /*关闭菜单配置文件*/
    com_close_confmenu_file();

    /*执行applib库的注销操作*/
    applib_quit();
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  MainMenu 应用的入口函数和场景管理器模块
 * \param[in]    PARAM_FROM_XXX
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
    scene_mainmenu_e scene_next = SCENE_MAINMENU;

    /*ap初始化*/
    _app_init();

    /*场景管理器*/
    while (scene_next != SCENE_EXIT)
    {
        switch (scene_next)
        {
            case SCENE_MAINMENU:
            result = mainmenu_desktop();
            if (result == RESULT_OPTION_MAINMENU)
            {
                scene_next = SCENE_OPTIONMENU;
            }
            else
            {
                scene_next = SCENE_EXIT; //exit
            }
            break;

            case SCENE_OPTIONMENU:
            result = option_menulist();
            if (result == RESULT_REDRAW)
            {
                scene_next = SCENE_MAINMENU;
            }
            else
            {
                scene_next = SCENE_EXIT; //exit
            }
            break;

            default:
            scene_next = SCENE_EXIT; //exit
            break;
        }

    }

    _app_exit();

#ifdef PC
    printf("mainmenu exit \n");
#endif
    return result;
}
