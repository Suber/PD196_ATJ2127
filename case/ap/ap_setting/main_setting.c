/********************************************************************************
 *                              USDK
 *                             Module: SETTING
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : main_setting.c
 * History:
 *      <johnsen>    <2011-8-25 13:55>           <V1.0 >             <desc>
 *
 ********************************************************************************/
/* include file of this application, 本应用的头文件 */
#include "ap_setting.h"

/* define variable, 变量定义 */
setting_comval_t g_setting_vars; //系统公共变量
//软时间中断相关数据;方便挂接中断入口函数
app_timer_t setting_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT]; 
style_infor_t style; //风格文件参数
textbox_private_t textbox_param; //文本控件参数
uint8 string_buffer[TIPS_BUFFER_SIZE];   //数据处理缓冲区
uint8 *res_filename;    //存放SD文件名指针
menu_com_data_t menu_com;   //菜单列表控件初始化结构体
date_t date;    //日期
time_t time;    //时间
//music引擎存在状况标识，关注的值为ENGINE_MUSIC/ENGINE_NULL
engine_type_e music_exist_flag; 
uint8 playlist_exist_flag;  //以便正确提示
uint8 current_disk; //用于表示当前播放的盘号
//日期设置由于参数较多且位置可变，为方便设立如下变量记录年月日所在相对位置
uint8 date_dd_location, date_mm_location, date_yy_location;
int32 g_vfs_mount_id;   //记录当前访问磁盘ID，-1表示未格式化
uint8 need_quit_ap_flag;    //用于记录是否需要马上退出ap
uint8 set_language_id_argv;

#ifdef PC
extern menu_cb_func _get_menu_func(uint16 str_id);
extern menu_cb_leaf _get_callback_func(uint16 str_id);
extern menu_cb_option _get_menu_option(uint16 str_id);
#else
OS_STK *ptos = (OS_STK *) AP_FRONT_LOW_STK_POS; //线程栈顶指针
#endif
INT8U prio = AP_FRONT_LOW_PRIO;     //任务优先级

const uint8 theme_setting_res[5][13] =
{ "setting.sty", "setgreen.sty", "set_gray.sty", "set_pink.sty", "set_red.sty" };

const uint8 themes_com[5][13] =
{
    "common.sty", //默认风格，名字用模块名，蓝色风格
    "cm_green.sty", //其他风格，名字用缩写+风格类别名，绿色风格
    "cm_gray.sty", //灰色风格
    "cm_pink.sty", //粉红色风格
    "cm_red.sty" //红色风格
};

const uint8 flash_drv_name[] =
{ "nand.drv" };

const uint8 card_drv_name[] =
{ "card.drv" };

#if 0
static const int8 user_srs_param_default[MAX_EQ_SEG] =
{ 8, 8, 5, 4, 6, 1, 0, 30, 1, 0, 0, 0 };//srs user default

static const int8 user_eq_param_default[MAX_EQ_SEG] =
{ -9, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }; //user eq default
#endif

#ifdef USE_83NAME_CREAT
const char mplist_name[]={"MUSIC   LIB"};
#else
const char mplist_name[]={"MUSIC.LIB"};
#endif

/******************************************************************************/
/*                    
 * \par  Description: mbyte to wchar
 *	  
 * \param[in]    dest，src

 * \param[out]      
 
 * \return  UNICODE 个数，不包含标识符和结束符     
  
 * \note    ASC转成UNICODE，自动加入标识符和结束符
*******************************************************************************/
static uint16 mbyte_to_wchar(uint8 *dest,uint8 *src)
{
    uint16 num=0;
    
#ifdef USE_83NAME_CREAT

    while(*src!=0x00)
    {
        *dest=*src;
        dest++;
        src++;
        num++;
    }
    
    dest=0x00;

#else    

    *dest=0xff;
    *(dest+1)=0xfe;
    dest+=2;
    
    while(*src!=0x00)
    {
        *dest=*src;        
        *(dest+1)=0x00;     
        dest+=2;       
        src++;
        num++;
    }
    
    *dest=0x00;
    *(dest+1)=0x00;
    
#endif
    
    return num;
    
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool check_plist_exit(void)
 * \检测列表是否存在
 * \param[in]    none  
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      main_setting.c
 * \note
 */
/*******************************************************************************/
bool check_plist_exit(void)
{
    uint32 plist_fp;
    bool check_ret;
    vfs_cd(g_vfs_mount_id,CD_ROOT,0);
     //打开文件
    mbyte_to_wchar(string_buffer,(uint8 *)mplist_name);
    plist_fp = vfs_file_open(g_vfs_mount_id, string_buffer, R_FAST_SEEK);     
    if (0 == plist_fp)
    {
        check_ret = FALSE;
    }
    else
    {
        vfs_file_close(g_vfs_mount_id, plist_fp);
        check_ret = TRUE;
    }	
    return check_ret;   
}

/*
 *****************************************************************************
 * \par  Description:读取VM变量信息
 *      普通说明
 *      重点说明
 * \param[in]    int:
 * \param[in]    void *:
 * \param[out]   void *:
 * \retval       bool
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
void read_var(void)
{
    /*读取Setting VM公共变量信息*/
    sys_vm_read(&g_setting_vars, VM_AP_SETTING, sizeof(g_setting_vars));
    /*第一次进入，初始化VM变量*/
    if (g_setting_vars.g_comval.magic != MAGIC_SETTING)
    {
        g_setting_vars.g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_setting_vars);
        //libc_memcpy(g_setting_vars.music_comval.eq_parameter,user_eq_param_default, MAX_EQ_SEG);
        //libc_memcpy(g_setting_vars.music_comval.srs_parameter,user_srs_param_default, MAX_EQ_SEG);
        sys_vm_write(&g_setting_vars, VM_AP_SETTING);
    }
    com_set_sys_comval(&g_setting_vars.g_comval);
    sys_vm_read(&g_setting_vars, VM_AP_SETTING, sizeof(g_setting_vars));
    sys_vm_write(&g_setting_vars, VM_AP_SETTING);
    sys_vm_read(&g_setting_vars, VM_AP_SETTING, sizeof(g_setting_vars));
    sys_vm_write(&g_setting_vars, VM_AP_SETTING);
    
    music_exist_flag = get_engine_type(); //后台音乐是否存在
}


/******************************************************************************/
/*!
 * \par  Description:
 * \void  setting_callback(void)
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       ap退出或者创建其他ap    
 * \ingroup      setting
 * \note
 */
/*******************************************************************************/
app_result_e setting_callback(private_msg_t *private_msg)
{
    app_result_e result = RESULT_NULL;
    engine_type_e engine_type;
    msg_apps_t msg;
    
    switch (private_msg->msg.type)
    {
        case MSG_SD_OUT:
        if (current_disk == DISK_H)
        {
            //正在访问卡盘文件，退回到主应用
            engine_type = get_engine_type();

            if (engine_type == ENGINE_MUSIC)
            {
                //消息类型(即消息名称)杀死后台进程
                msg.type = MSG_KILL_APP_SYNC;
                msg.content.data[0] = APP_ID_MENGINE;
                //发送同步消息
                send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
                music_exist_flag = ENGINE_NULL;
            }
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            result = RESULT_MAIN_APP;
        }
        break;
        
        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(private_msg->msg.type);
        break;
    }  
    return result;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e setting_menu_func(void *menu_title_void)
 * \param[in]    void *menu_title_void 根菜单项确定函数返回的执行结构体指针
 * \param[out]   填入的更新菜单结构数据
 * \return       菜单行为
 * \ingroup      setting
 * \note
 */
/*******************************************************************************/
app_result_e setting_menu_func(void *menu_title_void)
{
    menu_title_action_t* menu_title_restore;

    menu_title_restore = (menu_title_action_t*) menu_title_void;
    if(g_setting_vars.g_comval.online_device == VALUE_STORAGE_MTP)
    {
        menu_title_restore->menu_entry = MENU_ENTRY_ID_MAIN_MTP; //更新入口    
    }
    else
    {
        menu_title_restore->menu_entry = MENU_ENTRY_ID_MAIN; //更新入口
    }    
    menu_title_restore->str_id = -1;
    menu_title_restore->ret_str = NULL;
    menu_title_restore->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_file_init(file_path_info_t* init_path_locat)
 * \初始化文件选择器
 * \param[in]    init_path_locat 初始化文件路径信息指针 para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      setting
 * \note
 */
/*******************************************************************************/
bool music_file_init(file_path_info_t* init_path_locat)
{
    fsel_param_t init_param;//文件选择器初始化参数
    uint8* drv_name;

    bool init_result = FALSE;

    //文件选择器初始化接口的参数设置
    init_param.disk = init_path_locat->file_path.plist_location.disk;

    init_param.fsel_type = init_path_locat->file_source;

    init_param.fsel_mode = (g_setting_vars.music_comval.music_repeat_mode | g_setting_vars.music_comval.music_shuffle);

    init_param.file_type_bit = MUSIC_BITMAP;

    if (init_param.disk == DISK_C)
    {
        drv_name = (uint8*) flash_drv_name;
    }
    else
    {
        drv_name = (uint8*) card_drv_name;
    }
    //文件选择器初始化
    g_vfs_mount_id =  fsel_init(&init_param, 0);

    if (g_vfs_mount_id != -1)
    {
        init_result = TRUE;
    }

    return init_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filepath0(file_path_info_t *filepath)
 * \获取当前播放文件的路径信息
 * \param[in]    filepath存放文件路径buffer的指针  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      setting
 * \note
 */
/*******************************************************************************/
bool music_get_filepath0(file_path_info_t *filepath)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;
    //获取当前播放文件的路径信息
    reply.content = filepath;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    setting 应用的初始化
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      setting
 * \note
 *******************************************************************************/
bool setting_app_init(void)
{
    //文件路径来源信息保存
    file_path_info_t g_file_path_info; //播放时候用

    /*初始化applib库，前台AP;必须存在*/
    if (FALSE == applib_init(APP_ID_SETTING, APP_TYPE_GUI))
    {
        while (1)
        {
            ; //do nothing
        }
    }

    /*初始化软定时器;必须存在*/
    init_app_timers(setting_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    /*初始化 applib 消息模块，必须存在 */
    applib_message_init(setting_callback);

    /*初始化系统计时器;必须存在且在读取VM之后*/
    sys_timer_init();

    //初始ui资源文件
#if 0
    if (FALSE == ui_res_open("common.sty", UI_COM))
    {
        while (1)
        {
            ; //do nothing
        }
    }

    if (FALSE == ui_res_open("setting.sty", UI_AP))
    {
        while (1)
        {
            ; //do nothing
        }
    }
#else
    ui_res_open(theme_setting_res[g_setting_vars.g_comval.theme], UI_AP);
    ui_res_open(themes_com[g_setting_vars.g_comval.theme], UI_COM);
#endif

    //打开前台应用菜单配置脚本
    if (FALSE == com_open_confmenu_file("setting.mcg"))
    {
        while (1)
        {
            ; //do nothing
        }
    }

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
#endif
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_SETTING);

    
    //初始化文件选择器
    g_file_path_info.file_path.plist_location.disk = DISK_C; //DISK_H 卡盘 DISK_U uhost 盘
    current_disk = DISK_C;
    //初始化文件选择器为目录模式，一定成功
    g_file_path_info.file_source = FSEL_TYPE_COMMONDIR;

    //引擎存在则获取当前播放文件的路径信息
    if (music_exist_flag == ENGINE_MUSIC)
    {
        music_get_filepath0(&g_file_path_info);//获取文件路径
        current_disk = g_file_path_info.file_path.plist_location.disk;
    }
    //文件选择器初始化
    music_file_init(&g_file_path_info);
    if (g_vfs_mount_id != -1)
    {
        if(TRUE==check_plist_exit())
        {
            playlist_exist_flag = 1;     
        }
        else
        {
            playlist_exist_flag = 0;
        }    
            
    }
    else
    {
        playlist_exist_flag = 0;
    }    
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    setting 应用的退出
 * \param[in]    void
 * \param[out]   void
 * \return       bool
 * \retval       1    success
 * \retval       0    fail
 * \ingroup      setting
 * \note
 *******************************************************************************/
bool setting_app_deinit(void)
{

    //文件选择器退出
    fsel_exit();

    //保存计数值
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
 *	  setting结果处理模块
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \ingroup      setting
 * \note
 *******************************************************************************/

static app_result_e setting_proc_result(app_result_e result)
{
    msg_apps_t msg;
    uint8 temp_flag = 0 ;
    msg.type = MSG_CREAT_APP; //给process manager发送创建其它进程消息

    switch (result)
    {
        case RESULT_MAIN_APP:
        msg.content.data[0] = APP_ID_MAINMENU;
        msg.content.data[1] = (uint8) PARAM_FROM_SETTING;
        break;

        case RESULT_NOWPLAYING:
        msg.content.data[0] = APP_ID_MUSIC;
        msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        break;        
        

        case RESULT_MUSIC_PLAY:
        msg.content.data[0] = APP_ID_MUSIC;
        msg.content.data[1] = (uint8) PARAM_FROM_SETTING;
        break;

        case RESULT_USB_TRANS:
        msg.content.data[0] = APP_ID_UDISK;
        msg.content.data[1] = (uint8) PARAM_FROM_SETTING;
        break;
        case RESULT_APP_QUIT:
        temp_flag = 1;
        break;
        default:
        msg.content.data[0] = APP_ID_MAINMENU;
        msg.content.data[1] = (uint8) PARAM_FROM_SETTING;
        break;
    }
    //向process manager发送创建进程消息
    if(temp_flag!=1)
    {
        send_async_msg(APP_ID_MANAGER, &msg);
    }
    return result;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明
 *      重点说明
 * \param[in]    int:
 * \param[in]    void *:
 * \param[out]   void *:
 * \retval       bool
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
int main(int argc, const char *argv[])
{
    app_result_e result;
    scene_e setting_next_scene;

    /* 读入VM变量;必须存在 */
    read_var();

    /*初始化*/
    if (setting_app_init() == FALSE)
    {
        result = RESULT_MAIN_APP;
        setting_next_scene = SCENE_EXIT;
    }
    else
    {
        setting_next_scene = SETTING_MENU_LAYER;
    }

    /*场景管理器*/
    while (setting_next_scene != SCENE_EXIT)
    {
        menu_com.app_id = APP_ID_SETTING;
        if (g_setting_vars.g_comval.online_device == VALUE_STORAGE_MTP)
        {
            menu_com.menu_entry = MENU_ENTRY_ID_MAIN_MTP;
        }
        else
        {
            menu_com.menu_entry = MENU_ENTRY_ID_MAIN;
        }

        menu_com.menu_func = setting_menu_func;
        menu_com.path_id = 0;
        result = gui_menulist(MENULIST, &menu_com);
        {
            setting_next_scene = SCENE_EXIT;
        }
    }

    setting_proc_result(result);

    /*保存VM变量*/
    sys_vm_write(&g_setting_vars, VM_AP_SETTING);
    setting_app_deinit();
    return result;
}
