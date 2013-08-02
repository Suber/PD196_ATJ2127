/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-22          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include "app_music_playing.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e music_deal_sd_out(void)
 * \处理SD卡拔出消息
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
app_result_e music_deal_sd_out(void)
{
    app_result_e result = RESULT_NULL;
    engine_type_e engine_type;

    engine_type = get_engine_type();

    if (engine_type == ENGINE_MUSIC)
    {
        music_get_filepath(&g_file_path_info);

        if (g_file_path_info.file_path.plist_location.disk == DISK_H)
        {
            music_close_engine();
            //文件选择器退出
            fsel_exit();
            g_file_path_info.file_path.plist_location.disk = DISK_C;
            g_file_path_info.file_source = FSEL_TYPE_COMMONDIR;
            //文件选择器初始化
            music_file_init(&g_file_path_info);
        }
    }

    if (g_browser_filepath.file_path.plist_location.disk == DISK_H)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
        if ((g_enter_mode == PARAM_FROM_BROWSER) || (g_enter_mode == PARAM_FROM_RECORD))
        {
            result = RESULT_RETURN_BROWSER;
        }
        else
        {
            result = RESULT_CARD_OUT_ERROR;
        }

        g_browser_filepath.file_path.plist_location.disk = DISK_C;
    }
    return result;

}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e music_message_deal(private_msg_t *msg)
 * \处理music的私有消息和系统消息
 * \param[in]    msg消息指针  para1
 * \param[in]    private_msg_t
 *               typedef struct
 *               {
 *                 unsigned char sync;		//同步标志，1B
 *                 sem_id_t sem_id; 		  //信号量ID，1B
 *                 msg_apps_t msg;		    //消息内容，6B
 *               } private_msg_t;
 *               typedef struct
 *               {
 *                 msg_apps_type_e  type;	//消息类型，2B
 *                 union
 *                 {
 *	                 unsigned char data[4];
 *                   unsigned char *addr;
 *                 }content;					//消息内容或其地址，由消息自己约定，4B
 *               } msg_apps_t;//私有消息队列的消息结构体
 * \param[out]   none
 * \return       app_result_e the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
app_result_e music_message_deal(private_msg_t *msg)
{
    app_result_e result = RESULT_NULL;
    engine_type_e engine_type;

    switch (msg->msg.type)
    {
        case MSG_SD_OUT:
        result = music_deal_sd_out();
        break;

        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        case MSG_APP_BACKTO_PLAYING:
        if (g_ss_delay_flag == TRUE)//delay退出屏保
        {
            gui_screen_save_exit();
            g_ss_delay_flag = FALSE;
        }
        if (g_music_scene != SCENE_PLAYING)
        {
            engine_type = get_engine_type();

            if(engine_type == ENGINE_MUSIC)
            {
                result = RESULT_PLAYING_KEEP;
            }
            else
            {
                result = RESULT_NOWPLAYING;
            }

        }
        break;

        default:
        result = com_message_box(msg->msg.type);
        break;
    }
    return result;
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
 * \ingroup      mengine_main.c
 * \note
 */
/*******************************************************************************/
bool music_file_init(file_path_info_t* init_path_locat)
{
    fsel_param_t init_param;//文件选择器初始化参数

    //uint8* drv_name;

    bool init_result = FALSE;
    fsel_error_e fsel_error;

    //文件选择器初始化接口的参数设置
    init_param.disk = init_path_locat->file_path.plist_location.disk;

    init_param.fsel_type = init_path_locat->file_source;

    init_param.fsel_mode = (g_setting_comval.music_comval.music_repeat_mode
            | g_setting_comval.music_comval.music_shuffle);

    if (g_audible_now == TRUE)
    {
        init_param.file_type_bit = AUDIBLE_BITMAP;
    }
    else
    {
        init_param.file_type_bit = MUSIC_BITMAP;
    }

    //文件选择器初始化
    g_music_mount_id = fsel_init(&init_param, 0);

    if (g_music_mount_id != -1)
    {
        g_fsel_module.list_type = init_param.fsel_type;
        g_fsel_module.disk_type = init_param.disk;
        init_result = TRUE;
    }
    else
    {
        //初始化失败
        init_result = FALSE;

        fsel_error = fsel_get_error();

        switch (fsel_error)
        {
            case FSEL_ERR_STG:
            case FSEL_ERR_FS:
            case FSEL_ERR_LIST_NOEXIST:
            if(fsel_error == FSEL_ERR_LIST_NOEXIST)
            {
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PLAYLIST);
            }
            else
            {
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
            }
            break;

            default:
            break;

        }
    }

    return init_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_close_engine(void)
 * \卸载引擎
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_close_engine(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //msg_reply_t temp_reply;
    //杀死music 引擎
    msg.type = MSG_KILL_APP_SYNC;
    msg.content.data[0] = APP_ID_MENGINE;
    //msg.content.data[1] = 0x00;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_bookmark(mmm_mp_bp_info_t *bookmark)
 * \播放音乐书签
 * \param[in]    bookmark指向书签断点的指针  para1
 * \param[in]    mmm_mp_bp_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_play_bookmark(mmm_mp_bp_info_t *bookmark)
{
    bool bret;

    //消息类型(即消息名称)播放音乐书签
    bret = music_send_msg(bookmark, NULL, MSG_MENGINE_PLAY_BOOKMARK_SYNC);
    return bret;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_filepath(void)
 * \删除当前播放文件
 * \param[in]    void para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
/*bool music_delete_file(void)
 {
 bool bret = TRUE;
 msg_apps_t msg;

 //消息类型(即消息名称)删除当前播放文件
 msg.type = MSG_MENGINE_DELFILE_MUSUI_SYNC;
 //发送同步消息
 bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
 return bret;
 }*/
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_eq(mmm_mp_eq_info_t *eq_val)
 * \设置eq信息、播放速度
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_set_eq(mmm_mp_eq_info_t *eq_val)
{
    bool bret;
    bret = music_send_msg(eq_val, NULL, MSG_MENGINE_SET_EQ_SYNC);
    return bret;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_playmode(uint8 playmode)
 * \设置循环模式&shuffle模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_set_playmode(uint8 playmode)
{
    bool bret;
    bret = music_send_msg(&playmode, NULL, MSG_MENGINE_SET_PLAYMODE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_section_info(musfile_cnt_t *file_info)
 * \获取章节总数和序号
 * \param[in]    file_info music文件总数和序数结构体
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_get_section_info(musfile_cnt_t *file_info)
{
    bool bret;
    bret = music_send_msg(NULL, file_info, MSG_MENGINE_GET_SECTION_INFO_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \standby播歌处理
 * \param[in]    none
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_standby_play(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_STANDBY_PLAY_SYNC);
    return bret;    
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void _scene_result_deal(app_result_e result_val)
 * \场景返回值处理
 * \param[in]    result_val 场景调度返回值 para1
 * \param[out]   none
 * \return       none the result
 * \ingroup      music_main
 * \note
 * \li  根据场景调度返回值创建相应的ap
 * \li  普通ap不能直接创建相应的ap，必须通过发送异步消息由manager创建
 * \li  返回值为RESULT_APP_QUIT的情形由common发送过消息，ap不需要关心
 * \li  default情形会返回main ap，防止异常情形发生
 */
/*******************************************************************************/
void _scene_result_deal(app_result_e result_val)
{
    msg_apps_t msg;
    browser_var_t broswer_vm_data;
    switch (result_val)
    {
        case RESULT_NOWPLAYING:
        //返回到radio播放场景
        msg.type = MSG_CREAT_APP; //恢复创建radio
        msg.content.data[0] = APP_ID_RADIO;//radio ap
        msg.content.data[1] = PARAM_BACKTO_PLAYING;//radio ap param
        send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建radio ui的进程
        break;

        case RESULT_LASTPLAY:
        //返回到radio播放场景
        msg.type = MSG_CREAT_APP; //恢复创建radio
        msg.content.data[0] = APP_ID_RADIO;//radio ap
        msg.content.data[1] = PARAM_BACKTO_PLAYED;//radio ap param
        send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建radio ui的进程
        break;

        case RESULT_MAIN_APP:
        msg.type = MSG_CREAT_APP; //创建main manu
        msg.content.data[0] = APP_ID_MAINMENU;//main ap
        msg.content.data[1] = PARAM_FROM_MUSIC;//main ap param
        send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建main manu的进程
        break;

        case RESULT_RETURN_BROWSER:
        msg.type = MSG_CREAT_APP; //创建browser app
        msg.content.data[0] = APP_ID_BROWSER;//browser ap
        msg.content.data[1] = PARAM_FROM_MUSIC;//browser ap param
        send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建browser的进程
        break;

        case RESULT_CREATE_LIST_APP:
        msg.type = MSG_CREAT_APP; //创建 创建播放列表app
        msg.content.data[0] = APP_ID_PLAYLIST;//playlist ap
        if (g_file_path_info.file_path.plist_location.disk == DISK_H)
        {
            //只创建卡盘
            msg.content.data[1] = (PARAM_FROM_MUSIC | PLIST_DISK_H);
        }
        else
        {
            //创建所有盘
            msg.content.data[1] = (PARAM_FROM_MUSIC | PLIST_DISK_ALL);
        }

        //发送异步消息,创建播放列表app的进程
        send_async_msg(APP_ID_MANAGER, &msg);

        //music ui本身不记忆盘符,只有在创建播放列表时才关心盘符，用browser的
        //VM 变量临时记录一下
        sys_vm_read(&broswer_vm_data, VM_AP_BROWSER, sizeof(browser_var_t));

        libc_memcpy(&broswer_vm_data.path, &g_browser_filepath, sizeof(file_path_info_t));

        g_browser_filepath.file_source = FSEL_TYPE_COMMONDIR;

        sys_vm_write(&broswer_vm_data, VM_AP_BROWSER);

        break;

        case RESULT_APP_QUIT:
        break;

        default:
        msg.type = MSG_CREAT_APP; //创建main manu
        msg.content.data[0] = APP_ID_MAINMENU;//main ap
        msg.content.data[1] = PARAM_FROM_MUSIC;//main ap param
        send_async_msg(APP_ID_MANAGER, &msg); //发送异步消息,创建main manu的进程
        break;
    }
}


