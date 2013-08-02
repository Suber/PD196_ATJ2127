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
/*!
 * \file     music_control.c
 * \brief    music常驻代码区，较频繁使用的代码会放在该文件
 * \author   fiona.yang
 * \par      GENERAL DESCRIPTION:
 *
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date    2011-09-22
 *******************************************************************************/
#include "app_music.h"

#include "app_music_playing.h"

#if 0
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_send_sync_msg(msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
 * \music ui向引擎发送消息函数
 * \param[in]    msg 消息内容指针
 * \param[in]    reply  消息回执指针
 * \param[out]   timeout 超时控制
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 * \li  由于applib的send_sync_msg和定时器函数同属UI bank,为减小bank切换，将该函数
        简化后的实现放在music常驻代码,也可以不使用该代码
 */
/*******************************************************************************/
bool music_send_sync_msg(msg_apps_t *msg, msg_reply_t *reply, uint32 timeout)
{
    private_msg_t cur_send_pmsg;

    msg_reply_t temp_reply;
    //uint8 i;
    os_event_t *sem;//同步消息同步信号量指针
    bool retvalue = TRUE;

    //获取后台引擎类型，如果mengine不存在，则发送失败
    engine_type_e engine_type = get_engine_type();

    if(engine_type != ENGINE_MUSIC)
    {
        return FALSE;
    }

    //无限时等待信号量，同时只能有1个线程发送同步消息
    libc_sem_wait(thread_mutex, 0);

    //填充私有消息结构体
    libc_memcpy(&(cur_send_pmsg.msg), msg, sizeof(msg_apps_t));
    libc_sem_init(&sem, 0);
    cur_send_pmsg.sem = sem;
    if(reply == NULL)
    {
        cur_send_pmsg.reply = &temp_reply;
    }
    else
    {
        cur_send_pmsg.reply = reply;
    }

    //发送消息到队列中
    if(sys_mq_send(MQ_ID_EGN, (void *)&cur_send_pmsg) == -1)
    {
        libc_sem_destroy(&sem);
        libc_sem_post(thread_mutex);
        return FALSE;
    }

    //等待同步消息回应
    if(libc_sem_wait(sem, timeout) < 0)
    {
        retvalue = FALSE;
    }

    libc_sem_destroy(&sem);
    libc_sem_post(thread_mutex);
    return retvalue;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_param(void *param, uint32 msg_type)
 * \获取引擎相关参数函数
 * \param[in]    input_param 输入参数指针
 * \param[in]    msg_type  消息类型
 * \param[out]   output_param 输出参数指针
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_send_msg(void *input_param, void *output_param, uint32 msg_type)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;

    //存放输入参数的地址
    msg.content.addr = input_param;

    //存放获取参数的地址
    reply.content = output_param;

    //消息类型(即消息名称)
    msg.type = msg_type;

#if 0
    //发送同步消息
    bret = music_send_sync_msg(&msg, &reply, 0);
#else
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
#endif

    if (reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_filepath(file_path_info_t *filepath)
 * \获取当前播放文件的路径信息
 * \param[in]    filepath存放文件路径buffer的指针  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_filepath(file_path_info_t *filepath)
{
    return music_send_msg(NULL, filepath, MSG_MENGINE_GET_FILEPATH_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_fileinfo(mmm_mp_file_info_t* fileinfo)
 * \获取当前播放的文件格式、总时间、比特率等信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_fileinfo(mmm_mp_file_info_t* fileinfo)
{
    return music_send_msg(NULL, fileinfo, MSG_MENGINE_GET_FILEINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_status(mengine_status_t* play_status)
 * \获取当前播放状态
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_status(mengine_status_t* play_status)
{
    return music_send_msg(NULL, play_status, MSG_MENGINE_GET_STATUS_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playinfo(mengine_playinfo_t* play_info)
 * \获取当前播放的时间和比特率信息
 * \param[in]    play_info  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_playinfo(mengine_playinfo_t* play_info)
{
    return music_send_msg(NULL, play_info, MSG_MENGINE_GET_PLAYINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_bkinfo(mmm_mp_bp_info_t* curbk_info)
 * \获取当前播放的断点信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_bkinfo(mmm_mp_bp_info_t* curbk_info)
{
    return music_send_msg(NULL, curbk_info, MSG_MENGINE_GET_BKINFO_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_get_playmode(uint8* playmode)
 * \获取循环模式&shuffle模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_get_playmode(uint8* playmode)
{
    return music_send_msg(NULL, playmode, MSG_MENGINE_GET_PLAYMODE_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play(void)
 * \开始播放音乐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play(void)
{
    return music_send_msg(NULL, NULL, MSG_MENGINE_PLAY_SYNC);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_filepath(file_path_info_t *filepath)
 * \设置播放文件的路径信息
 * \param[in]    filepath存放文件路径buffer的指针  para1
 * \param[in]    file_path_info_t  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_set_filepath(file_path_info_t *filepath)
{
    if(filepath->file_path.dirlocation.disk == DISK_H)
    {
        if(sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            music_deal_sd_out();
            return FALSE;
        }
    }
    return music_send_msg(filepath, NULL, MSG_MENGINE_SET_FILEPATH_SYNC);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_next(void)
 * \播放下一曲
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play_next(void)
{
    bool bret;
    uint32 msg_type;

    if ((g_audible_now == TRUE)
        && (g_music_config.section_mode == SWITCH_SECTION)
        && (g_music_status.err_status == EG_ERR_NONE))
    {
        //消息类型(即消息名称)播放下一章
        msg_type = MSG_MENGINE_CAHPTER_NEXT_SYNC;
        //只需要更新文件序号
        g_display_bitmap |= up_musictracks;
    }
    else
    {
        msg_type = MSG_MENGINE_PLAY_NEXT_SYNC;
        //设置切换文件刷新标志位
        SetSwitchInterface()
    }
    //发送同步消息
    bret = music_send_msg(NULL, NULL, msg_type);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_play_prev(void)
 * \播放上一曲
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool music_play_prev(void)
{
    bool bret;
    uint32 msg_type;

    if ((g_audible_now == TRUE)
        && (g_music_config.section_mode == SWITCH_SECTION)
        && (g_music_status.err_status == EG_ERR_NONE))
    {
        //消息类型(即消息名称)播放上一章
        msg_type = MSG_MENGINE_CHAPTER_PREV_SYNC;
        //只需要更新文件序号
        g_display_bitmap |= up_musictracks;
    }
    else
    {
        msg_type = MSG_MENGINE_PLAY_PREV_SYNC;
        //设置切换文件刷新标志位
        SetSwitchInterface()
    }

    //发送同步消息
    bret = music_send_msg(NULL, NULL, msg_type);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_flag_update(void)
 * \1秒定时设置播放标识
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void play_flag_update(void)
{
    g_timer_counter++;
    g_check_mengine_status = TRUE;
    if (g_timer_counter == 4)
    {
        g_timer_counter = 0;
        g_display_bitmap |= up_musictimeinfo;//播放时间
        g_display_bitmap |= up_musicprogress;//进度条

        if (((g_music_status.play_status != StopSta) && (g_music_status.play_status != PauseSta)) && (g_lyric_flag
                == TRUE))
        {
            g_display_bitmap |= up_lyricflag;//歌词
        }
        //g_paint_flag = TRUE;//需要重新绘制界面
        if (g_music_status.ab_status != PlayAB_Null)
        {
            g_display_bitmap |= up_musicplaystatus;//播放状态
            g_ab_switch = !g_ab_switch;
        }

        if (g_music_status.play_status == PlayFast)
        {
            g_speed_counter++;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool play_scroll_deal(void)
 * \滚屏操作
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       0 失败需要更换方向/更换内容
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
bool play_scroll_deal(void)
{
    bool result;

    scroll_param_t scroll_param;
    style_infor_t style_infor, style_icon;

    //获取滚屏区域
    style_infor.style_id = ID3_SCROLL_TEXTBOX;
    style_infor.type = UI_AP;

    //以垂直滚屏方式显示PICBOX，要求为多帧，并且帧0必须为背景图标
    if (g_audible_now == TRUE)
    {
        style_icon.style_id = ID3_AUDIBLE_PICBOX;
    }
    else
    {
        style_icon.style_id = ID3_MUSIC_PICBOX;
    }
    style_icon.type = UI_AP;

    //滚屏参数填充
    scroll_param.direct = g_scroll_vars.direct;
    scroll_param.init = g_scroll_vars.init;
    scroll_param.update = g_scroll_vars.update;
    scroll_param.active = g_scroll_vars.index;
    scroll_param.id3_text = &style_infor;
    scroll_param.id3_icon = &style_icon;

    //调用滚屏
    result = (bool)(int32) ui_scroll_string_ext(g_scroll_content, &scroll_param);

    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scroll_update(void)
 * \滚屏定时器服务函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void play_scroll_update(void)
{
    g_scroll_deal = TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void id3_scroll_deal(void)
 * \ID3滚屏状态处理函数
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
void id3_scroll_deal(void)
{
    switch (g_scroll_status)
    {
        //case ID3_DEAL_INIT:
        //play_scroll_init();
        //break;

        case ID3_DEAL_SCROLL:
        if (g_scroll_vars.update == TRUE)
        {
            g_scroll_vars.update = FALSE;
            modify_app_timer(scroll_frush_id, 80);
        }

        if (play_scroll_deal() == FALSE)
        {
            if (g_scroll_vars.init == TRUE)
            {
                //如果发现横屏不需要滚动，立即修改为竖屏模式
                g_scroll_vars.init = FALSE;
                //修改为垂直方向
                g_scroll_vars.direct = ID3_SCROLL_VERTICAL;
            }
            else
            {
                //当前模式滚屏结束，开启3s定时器切换滚屏模式
                modify_app_timer(scroll_frush_id, 3000);
                g_scroll_status = ID3_DEAL_SWITCH;
            }
        }
        else
        {
            g_scroll_vars.init = FALSE;
        }
        break;

        case ID3_DEAL_SWITCH:
        if (g_scroll_vars.direct == ID3_SCROLL_VERTICAL)
        {
            g_scroll_vars.index++;

            if (g_scroll_vars.index > 2)
            {
                g_scroll_vars.index = 0;
            }

            //下一个横向滚屏
            g_scroll_vars.direct = ID3_SCROLL_HORIZONTAL;
            g_scroll_vars.init = TRUE;
        }
        else
        {
            //修改为垂直方向
            g_scroll_vars.direct = ID3_SCROLL_VERTICAL;
        }
        modify_app_timer(scroll_frush_id, 80);
        g_scroll_status = ID3_DEAL_SCROLL;
        break;

        default:
        break;
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _scene_play_sysmsg(void)
 * \处理系统消息
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_control
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_sysmsg(void)
{
    //get message
    private_msg_t music_msg;
    app_result_e msg_result = RESULT_IGNORE;
    if (get_app_msg(&music_msg) == TRUE)
    {
        msg_result = music_message_deal(&music_msg);
    }
    return msg_result;
}
