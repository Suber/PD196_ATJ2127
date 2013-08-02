/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, 本应用的头文件 */
#include "ebook.h"

/****************************************************************************/

/* define variable, 变量定义 */
const uint8 app_id[3] =
{ APP_ID_MAINMENU, APP_ID_MUSIC, APP_ID_RADIO };

 uint8 device_drv_name[MAX_DEVICE_NUM][10] = //为了省空间，去掉const属性
{ "nand.drv", "card.drv" };

const uint8 device_drv_index[MAX_DEVICE_NUM] =
{ DISK_C, DISK_H };


const uint8 key_drv_name[] =
{ "key.drv" };

const uint8 com_themes[5][13] =
{ "common.sty", "cm_green.sty", "cm_gray.sty", "cm_pink.sty", "cm_red.sty" };
const uint8 ebook_sty[] =
{ "ebook.sty" };
const uint8 ebook_mcg[] =
{ "ebook.mcg" };

#ifndef PC
OS_STK *ptos = (OS_STK *) AP_EBOOK_STK_POS;     //电子书应用的任务栈
#endif

INT8U prio = AP_EBOOK_PRIO;                     //电子书应用的主线程优先级
    
/********************************************************************************
 * Description :应用的入口函数
 *
 * Arguments  :
 *              argc:进入应用场景的参数
 *              argv:
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
int main(int argc, const char *argv[])
{
    app_result_e retval;  
    engine_type = get_engine_type();            //获取后台引擎类型 
    if (argc == PARAM_FROM_BROWSER)             //如果从 browser ap 进入
    {
        enter_mode = ENTER_FROM_FILELIST;       //则标记从文件列表进入场景
    }

    /*!ebook应用初始化*/
    retval = _ebook_init();

    if (retval == RESULT_NULL)                       //假如初始化成功
    {
        /*!ebook应用功能场景调度*/
        retval = _ebook_scene_dispatch(argc);   //这里就会切换到打开ebook的界面， ebook 的场景切换

        if (argc == PARAM_FROM_BROWSER)         //如果 从 browser ap 进入
        {
            //browser退回播放列表
            if((retval == RESULT_EBK_FILELIST) || (retval == RESULT_MAIN_APP))   
            {
                retval = RESULT_BROWSER;            //浏览器进入消息
            }
        }
    }
    else if(retval == RESULT_ERROR)
    {   
        device_status |= 0x01;                  //标记flash没有文件
        retval = RESULT_MAIN_APP;               //回到 MainMenu 应用
    }
    else
    {
        //剩下 retval=RESULT_CREATE_LIST_APP
    }
    

    /*!退出ebook应用*/
    _deal_exit_result(retval);  //退出ebook应用时 处理相应的返回结果
    _ebook_exit();

    return argc;
}


/********************************************************************************
 * Description :电子书应用的场景调度
 *
 * Arguments  :
 *              param:场景模式
 * Returns     :
 *                应用处理后的返回结果
 * Notes       :
 *
 ********************************************************************************/
int _ebook_scene_dispatch(int param)
{
    //    scene_enter_mode_e enter_mode;
    app_result_e scene_next, retval = RESULT_NULL;      //val;
    if (param == PARAM_FROM_BROWSER)                    //如果从 browser ap 进入
    {
        scene_next = RESULT_EBK_PLAY;                   //标记下一个界面为：ebook 的播放界面
    }
    else
    {
        scene_next = RESULT_EBK_FILELIST;               //否则标记下一个界面为：ebook 的文件列表界面
    }
    enter_mode = ENTER_FROM_DESKTOP;                    //从main ap 或browser进入场景

    /*场景管理器*/
    while (retval != RESULT_EBOOK_BASE)                 //等待 电子书应用返回
    {
        /*清屏显示图片*/
        _clear_and_set_screen(0xffff, 0x0);             //先清屏，保证界面干净
        numboxflag = TRUE;                              //标记需要全刷numbox

        switch (scene_next)
        {
            case RESULT_EBK_FILELIST:                   //文件列表场景
            scene_next = _ebook_filelist(enter_mode);   //进入电子书文件列表选择菜单
            break;
            case RESULT_EBK_SETTINGMENU:                //菜单设置场景
            case RESULT_EBK_READINGMENU:
            scene_next = _ebook_option_menu(enter_mode);
            break;
            case RESULT_EBK_PLAY:                       //阅读界面场景
            scene_next = _ebook_reading(enter_mode);    //电子书阅读界面的入口函数：切换到ebook 的播放界面，并标记起来
            if( (param == PARAM_FROM_BROWSER) && (scene_next == RESULT_EBK_FILELIST) )
            {
                //如果退回到文件播放列表，而从BROWSER进入AP的，则退出AP，即可进入BROWSER的文件列表
                retval = RESULT_EBOOK_BASE;             //标记：电子书应用返回
            }
            break;
            default:
            retval = RESULT_EBOOK_BASE;                 //标记：电子书应用返回
            break;
        }
        

        
    }

    if(g_ebook_mount_id >= 0)
    {
        _close_text_file(1);                        //关闭已打开的文件文件句柄
    }
    return scene_next;
}


/********************************************************************************
 * Description :应用配置信息初始化
 *
 * Arguments  :
 *
 * Returns     :
 *                0-文件选择器初始化失败。1-初始化成功
 * Notes       :
 *            主要包括vram区数据校验，初始化，资源文件加载等功能
 ********************************************************************************/
app_result_e _ebook_init(void)
{
    /* 读入VM变量 ，VM空间用于保存应用环境变量*/
    _ebook_read_var();                                      //全局变量读取和初始化
    _check_play_device();                                   //检查后台播放的盘符
    g_ebook_vars.path.file_source = FSEL_TYPE_LISTEBOOK;    //EBOOK列表
    if (enter_mode == ENTER_FROM_FILELIST)                  //如果从文件列表进入场景
    {
        cur_file_diroffset = g_ebook_vars.path.file_path.dirlocation.dir_entry; 
                                                            //当前文本文件中目录信息中的目录偏移

        sys_vm_read(&g_ebook_vars.path, VM_AP_BROWSER, sizeof(g_ebook_vars.path));
                                                            //读取VM保存的电子书文件的播放路径信息

        g_ebook_vars.path.file_source = FSEL_TYPE_COMMONDIR;//目录播放

        if (    (music_play_device != g_ebook_vars.path.file_path.dirlocation.disk) 
                                                            //如果后台音乐播放的盘符不是VM保存的
            &&  (music_play_device != 0)                    //且后台音乐播放的盘符不为0
            )
        {
            _close_engine();                                //通知后台引擎退出
        }
    }
    else if ((music_play_device != g_ebook_vars.path.file_path.dirlocation.disk) && (music_play_device != 0))
    {
        g_ebook_vars.path.file_path.dirlocation.disk = music_play_device;
    }
    else
    {
    }
    //初始化应用内的全局变量
    _ebook_param_init();
    
    //初始化applib库，前台AP
    applib_init(APP_ID_EBOOK, APP_TYPE_GUI);

    //初始化软定时器
    init_app_timers(ebook_app_timer_vector, COMMON_TIMER_COUNT + APP_TIMER_COUNT);

    //初始化 applib 消息模块
    applib_message_init(_ebook_scene_msg_dispatch);

    sys_timer_init();

    /*读取ui资源文件*/
    ui_res_open(ebook_sty, UI_AP);
    ui_res_open(com_themes[g_comval.theme], UI_COM);
    com_open_confmenu_file(ebook_mcg);

#ifdef PC
    load_menulist_simfunc(_get_menu_func, _get_callback_func, _get_menu_option);
    com_clear_all_history(PATH_HISTORY_MENU);
#endif

    /*初始化文件选择器*/
    if(_ebook_filesel_init() == RESULT_CREATE_LIST_APP)
    {
        return RESULT_CREATE_LIST_APP;
    }

    /*文件选择器初始化报错退出*/
    if (fsel_get_error() != FSEL_NO_ERR)
    {
        return RESULT_ERROR;
    }
    
    return RESULT_NULL;
}

/********************************************************************************
 * Description :退出应用时配置信息保存和资源卸载
 *
 * Arguments  :
 *              param:场景模式
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
void _ebook_exit(void)
{
#ifdef PC
    uint8 temp[512];                    //先开辟512字节的栈空间，以便setting_comval读写512字节的时候有足够空间
                                        //否则在PC机上读取512字节，超出范围。
#endif
    setting_comval_t setting_comval; 
    
    //读取共用的变量 
    sys_vm_read(&setting_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&setting_comval.g_comval, &g_comval, sizeof(comval_t));
    /*保存VM变量*/
    sys_vm_write(&g_ebook_vars, VM_AP_EBOOK);
    sys_vm_write(&setting_comval, VM_AP_SETTING);

    fsel_exit();
    sys_timer_exit();

    //关闭应用菜单配置脚本
    com_close_confmenu_file();

    //关闭ui资源文件
    ui_res_close(UI_COM);
    ui_res_close(UI_AP);
    //    ui_set_display_buf(JPEG_RAM_INDEX);
    //执行applib库的注销操作
    applib_quit();
}
/********************************************************************************
 * Description :应用内的全局变量初始化
 *
 * Arguments  :
 *              无
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
void _ebook_param_init(void)
{
    bookmark_total = 0xFF;
    support_card_flag = 1;
}
/********************************************************************************
 * Description :全局变量读取和初始化
 *
 * Arguments  :
 *              无
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/

void _ebook_read_var(void)
{
    int result;
    setting_comval_t setting_comval;
    //读取共用的变量
    result = sys_vm_read(&setting_comval, VM_AP_SETTING, sizeof(setting_comval_t));
    libc_memcpy(&g_comval, &setting_comval.g_comval, sizeof(comval_t));
    com_set_sys_comval(&g_comval);
    if (g_comval.magic != MAGIC_COMVAL)
    {
        g_comval.magic = MAGIC_SETTING;
        com_get_default_setting(&g_comval);
        sys_vm_write(&g_comval, VM_AP_SETTING);
    }
    //    g_comval.language_id = CHINESE_SIMPLIFIED;

    //读取ebook使用的变量
    result = sys_vm_read(&g_ebook_vars, VM_AP_EBOOK, sizeof(g_ebook_vars));
    if ((g_ebook_vars.magic != MAGIC_EBOOK) || (g_ebook_vars.path.file_path.dirlocation.disk == 0x0))
    {
        //设定默认值
        libc_memset(&g_ebook_vars, 0, sizeof(g_ebook_vars));
        g_ebook_vars.path.file_source = FSEL_TYPE_LISTEBOOK;
        g_ebook_vars.path.file_path.dirlocation.disk = DISK_C;
        g_ebook_vars.autoplaytime = 10;
        g_ebook_vars.magic = MAGIC_EBOOK;
        sys_vm_write(&g_ebook_vars, VM_AP_EBOOK);
    }
}

/********************************************************************************
 * Description :ebook应用有关文件选择器的初始化
 *
 * Arguments  :
 *
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
app_result_e _ebook_filesel_init(void)
{
    fsel_param_t ebook_fsel_param;//文件选择器初始化参数
    uint8 i = 0, disk_count = 0;
    uint8 count_num=2;
    uint8 fsel_err_msg;
    uint16 error_id;
    app_result_e result;
    
    /*初始化ebook应用的文件选择器*/
    ebook_fsel_param.fsel_type = g_ebook_vars.path.file_source;             //FSEL_TYPE_LISTEBOOK;//FSEL_TYPE_COMMONDIR;
    ebook_fsel_param.fsel_mode = FSEL_MODE_NORMAL;                          //普通播放
    ebook_fsel_param.brow_type = FSEL_BROW_ALL;                             //显示全部(文件和目录)
    ebook_fsel_param.disk = g_ebook_vars.path.file_path.dirlocation.disk;   //加载盘符
    ebook_fsel_param.file_type_bit = TEXT_BITMAP;                           //标记应用后缀名,用于只显示txt文件

    /* 先检测是否需要初始化SD卡 */
    if(ebook_fsel_param.disk == DISK_H)                                     //假如需要初始化SD卡
    {
        if(_detect_device(DRV_GROUP_STG_CARD) == FALSE)                     //假如SD卡不存在
        {
            ebook_fsel_param.disk   = g_ebook_vars.path.file_path.dirlocation.disk
                                    = device_drv_index[0];                  //切换到Flash盘
            count_num=1;                                                    //用于标记等下的while循环不需要再检测SD卡
        }
    }

    /* 轮流检测两个盘符 */
    while (i < 2)                                                           //检测两个盘是否正常
    {
        if (ebook_fsel_param.disk == device_drv_index[i]) 
        {
            disk_count++;                                      
            g_ebook_mount_id = fsel_init(&ebook_fsel_param, 0);             //初始化盘        
            fsel_err_msg=fsel_get_error();                                  //获取初始化结果
            if(fsel_err_msg==FSEL_ERR_LIST_NOEXIST)                         //假如收藏夹或播放列表不存在
            {
                result = _show_double_dialog(S_IS_CREATE_PLAYLIST, NULL);      //弹出创建播放列表菜单选择
                if (result == RESULT_DIALOG_YES)
                {
                    g_ebook_vars.path.file_path.dirlocation.disk = device_drv_index[i];      //保存磁符
                    //_close_engine();                                            //通知后台引擎退出
//                    if(g_ebook_mount_id < 0)
//                    {
//                        fsel_exit();                                                //退出盘
//                    }
                    return RESULT_CREATE_LIST_APP;                              //进入创建播放列表ap
                }
                else if (result != RESULT_DIALOG_NO  )                          //退出对话框
                {
                    //有可能是 RESULT_REDRAW
                    return result;
                }
                else
                {
                    //QAC
                }

                
                //否则按照正常情况处理                
                if(i==0)
                {
                    error_id=S_DISK_NOFILE;                                 //标记磁盘无文件
                }
                else
                {
                    error_id=S_CARD_NOFILE;                                 //标记卡无文件
                }                
            }
            else 
            {
                error_id=S_DISK_ERROR;                                      //标记磁盘错误                   
            }
            if(fsel_err_msg!=FSEL_NO_ERR)                                   //假如有报错
            {
                _show_single_dialog(error_id, DIALOG_INFOR_WAIT);           //在界面上显示错误
            }
            if ((g_ebook_mount_id == -1) && (disk_count < count_num))       //应用文件系统的索引号为-1，且盘没检测完
            {                
                if(ebook_fsel_param.disk == device_drv_index[0])            //假如检测的是Flash
                {
                    if(_detect_device(DRV_GROUP_STG_CARD) == FALSE)         //假如Flash不存在(没格式化)
                    {
                        break;
                    }
                }
                fsel_exit();                                                //退出盘
                i = 1 - i;                                                  //切换到下一个盘符  
                ebook_fsel_param.disk   = g_ebook_vars.path.file_path.dirlocation.disk
                                        = device_drv_index[i];              
                
            }
            else
            {
                break;
            }
        }
        else
        {
            i++;
        }
    }
    return RESULT_NULL;
}

/********************************************************************************
 * Description :退出ebook应用时处理相应的返回结果
 *
 * Arguments  :
 *              result:应用返回需要处理的结果
 *
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/

void _deal_exit_result(app_result_e result)
{
    msg_apps_t msg;
    uint8 need_send_msg = TRUE;
    //给process manager发送创建其它进程消息
    msg.type = MSG_CREAT_APP;                               //异步创建应用程序消息
    msg.content.data[1] = (uint8) PARAM_FROM_EBOOK;         //从 ebook ap 进入 
    switch (result)
    {
        case RESULT_MAIN_APP:                               //回到 MainMenu 应用
        msg.content.data[0] = APP_ID_MAINMENU;
        break;

        case RESULT_BROWSER:                                //浏览器进入消息
        msg.content.data[0] = APP_ID_BROWSER;
        break;
        case RESULT_CREATE_LIST_APP:                        //进入创建播放列表ap
        msg.content.data[0] = APP_ID_PLAYLIST;
        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_H)
        {
            //只创建卡盘
            msg.content.data[1] = (PARAM_FROM_EBOOK | PLIST_DISK_H);//playlist ap param
        }
        else
        {
            //创建所有盘
            msg.content.data[1] = (PARAM_FROM_EBOOK | PLIST_DISK_ALL);//playlist ap param
        }
        break;
        case RESULT_NOWPLAYING:                             //正在播放进入消息
        msg.content.data[0] = app_id[engine_type];
        if (engine_type == ENGINE_NULL)
        {
            msg.content.data[1] = (uint8) PARAM_FROM_EBOOK;
        }
        else
        {
            msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYING;
        }
        break;
        case RESULT_LASTPLAY:
        msg.content.data[0] = app_id[engine_type];
        if (engine_type == ENGINE_NULL)
        {
            msg.content.data[1] = (uint8) PARAM_FROM_EBOOK;
        }
        else
        {
            msg.content.data[1] = (uint8) PARAM_BACKTO_PLAYED;
        }
        break;
        case RESULT_USB_TRANS:
        msg.content.data[0] = APP_ID_UDISK;
        msg.content.data[1] = PARAM_NULL;
        break;

        default:
        need_send_msg = FALSE;
        break;
    }
    //向process manager发送创建进程异步消息
    if (need_send_msg == TRUE)
    {
        send_async_msg(APP_ID_MANAGER, &msg);
    }
}

/********************************************************************************
 * Description :检查后台播放的盘符
 *
 * Arguments  :   无
 *
 *
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
void _check_play_device(void)
{
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;
    file_path_info_t music_file_path;
    // engine_type_e engine_type = get_engine_type();
    //获取当前播放文件的路径信息
    reply.content = &music_file_path;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    //发送同步消息
    send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    if ((engine_type == ENGINE_MUSIC))
    {
        music_play_device = music_file_path.file_path.dirlocation.disk;
    }
}

