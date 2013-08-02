/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona        2011-09-14          1.0              create this file
 *******************************************************************************/

#include "ap_alarm.h"
#include "fmengine.h"
//sd 闹铃文件名
const char g_sd_filename[12] = "alarm1.mp3";

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_modual_init(void)
 * \初始化FM  模组
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed--- 同步消息发送失败，或模组初始化失败
 * \note
 */
/*******************************************************************************/
bool radio_modual_init(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = Band_China_US;

    //初始化FM 模组
    msg.type = MSG_FMENGINE_INIT;
    //发送同步消息
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  设置音量值
 * \param[in]    vol
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   设置音量成功
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool alarm_set_volume(uint8 vol)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_MENGINE_SET_VOLUME_SYNC;
    msg.content.addr = &vol;

    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  装载引擎
 * \param[in]    mengine_enter_mode_e
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool music_open_engine(mengine_enter_mode_e start_mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t temp_reply;
    //无消息内容
    msg.content.data[0] = APP_ID_MENGINE;
    msg.content.data[1] = (uint8)(start_mode | g_alarm.path.file_path.dirlocation.disk);
    //消息类型(同步创建应用程序消息)
    msg.type = MSG_CREAT_APP_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MANAGER, &msg, &temp_reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  设置播放文件的路径信息
 * \param[in]    filepath存放文件路径buffer的指针  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool music_set_filepath(file_path_info_t *filepath)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //设置播放文件的路径信息
    msg.content.addr = filepath;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_SET_FILEPATH_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  设置循环模式&shuffle模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool music_set_playmode(uint8 playmode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //设置循环模式&shuffle模式
    msg.content.addr = &playmode;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_SET_PLAYMODE_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  发送播放音乐消息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool alarm_play(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //消息类型(即消息名称)开始播放音乐
    msg.type = MSG_MENGINE_PLAY_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \  播放场景初始化
 * \param[in]    uint8 from_flag
 * \param[out]   none
 * \return       int the result
 * \retval       ......
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_init(uint8 from_flag)
{
    engine_type_e engine_type;
    msg_apps_t msg;
    uint8 cur_mode;
    int disk_exist = 0;
    //挂载文件系统的mount ID
    int32 browser_mount;
    fsel_param_t fsel_param;
    bool result;
    //file_location_t* locatptr;//指向location的指针
    //id3_type_e cur_type;//当前音乐的类型
    //fsel_type_e comp_type = g_alarm.path.file_source;

    engine_type = get_engine_type();
    //注销radio后台
    if(engine_type != ENGINE_NULL)
    {
        if (engine_type == ENGINE_RADIO)
        {
            //从FM 录音返回，由于录音codec 覆盖了fm 驱动代码，重装驱动
            sys_drv_uninstall(DRV_GROUP_FM);
            sys_drv_install(DRV_GROUP_FM, 0, "drv_fm.drv");
            //装载完驱动后重新初始化模组
            radio_modual_init();
            
            msg.type = MSG_KILL_APP_SYNC;//MSG_KILL_APP;
            msg.content.data[0] = APP_ID_FMENGINE;
            send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);    //发送同步消息
        }
        else
        {
            music_close_engine();
        }
    }
    if(from_flag == 1) // DISK_C
    {
        disk_exist = sys_detect_disk(DRV_GROUP_STG_BASE);
    }
    else if(from_flag == 2) // DISK_H
    {
        disk_exist = sys_detect_disk(DRV_GROUP_STG_CARD);
    }
    else //SD file
    {
        g_alarm.path.file_path.dirlocation.disk = DISK_C;
    }

    if (disk_exist == -1)
    {
        //检测不到卡盘，则初始化成默认盘符
        from_flag = 0;
        g_alarm.path.file_path.dirlocation.disk = DISK_C;
    }
    
    //if (engine_type != ENGINE_MUSIC)
    {
        music_open_engine(ENTER_ALARM);
    }
        
    if(from_flag != 0)   //选择歌曲播放
    {
        //初始化文件选择器参数
        fsel_param.disk = g_alarm.path.file_path.dirlocation.disk;
        fsel_param.fsel_type = FSEL_TYPE_COMMONDIR;
        fsel_param.fsel_mode = FSEL_MODE_BROWSER;
        fsel_param.brow_type = FSEL_BROW_ALL;
        fsel_param.file_type_bit = MUSIC_BITMAP;
        //初始化文件选择器
        browser_mount = fsel_init(&fsel_param, 0);
        // mount出错退出
        if(browser_mount == -1)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
            return RESULT_ALARM_QUIT;
        }

        g_alarm.path.file_source = FSEL_TYPE_COMMONDIR;
        result = fsel_set_location(&g_alarm.path.file_path, FSEL_TYPE_COMMONDIR);
        //文件选择器退出
        fsel_exit();
        
        //if(music_set_filepath(&g_alarm.path) != TRUE)  //出错处理
        if(result != TRUE)  //出错处理
        {
            goto sd_play;
        }
        music_set_filepath(&g_alarm.path);
    }
    else  //SD_PLAY
    {
        sd_play:
        g_alarm.path.file_source = FSEL_TYPE_SDFILE;
        libc_memcpy(g_alarm.path.file_path.dirlocation.filename,g_sd_filename,12);
        if(music_set_filepath(&g_alarm.path) == FALSE)  //设置文件路径
        //if(fsel_set_location(&g_alarm.path,FSEL_TYPE_SDFILE) == FALSE)  //设置文件路径
        {
            return FALSE;
        }
    }
    set_current_volume(g_alarm.volume);
    //设置默认音量
    alarm_set_volume(g_alarm.volume); 
    //设置循环模式
    //cur_mode &= (uint8) 0xf0;
    cur_mode = FSEL_MODE_LOOPONE;
    music_set_playmode(cur_mode);
    
    alarm_play();

    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  播放场景处理
 * \param[in]    void param
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       
 * \ingroup      alarm_playing_init.c
 * \note
 */
/*******************************************************************************/
app_result_e alarm_scene_playing(void)
{
    bool result;
    app_result_e ret_vals;
    //style_infor_t style_infor;

    result = _scene_play_init(g_alarm.ring_flag);
    if (result == FALSE)
    {
        return RESULT_SCENE_EXIT;
    }

    //场景循环
    ret_vals = playing_loop_deal();
    
    return ret_vals;
}
