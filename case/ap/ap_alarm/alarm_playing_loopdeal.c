/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-11-14          1.0              create this file
 *******************************************************************************/

#include "ap_alarm.h"



const key_map_t playing_key_map_list[] =
{
    /*! 短按按next切换下一曲or ab复读事件 */
    /*{{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },*/
    /*! 短按按prev切换上一曲事件 */
    /*{{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },*/
    /*! 短按option事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*!短按play播放/暂停事件 */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! 短按KEY_RETURN 按键转换为 返回事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! 结束标志 */
    {{KEY_NULL, 0, KEY_TYPE_NULL}, MSG_NULL},
};

/******************************************************************************/
/*!
 * \par  Description:
 * \  关闭music引擎
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_loopdeal.c
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
 * \  发送消息, 停止播放
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
bool music_stop(void)
{
    bool bret = TRUE;
    msg_apps_t msg;

    //消息类型(即消息名称)停止播放音乐
    msg.type = MSG_MENGINE_STOP_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  获取当前播放状态
 * \param[in]    mengine_status_t *play_status
 * \param[out]   none
 * \return       bool the result
 * \retval       1 sucess
 * \retval       0 failed
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
bool music_get_status(mengine_status_t* play_status)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;
    //获取当前播放状态等
    reply.content = play_status;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_GET_STATUS_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    return按键消息处理
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 *******************************************************************************/
app_result_e play_key_return(void)
{
    //停止播放
    music_stop();
    //关闭engine
    music_close_engine();
    
    return RESULT_ALARM_QUIT;//RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  处理gui消息
 * \param[in]    msg_apps_type_e cur_event
 * \param[out]   none
 * \return       app_result_e result
 * \retval       none
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_guimsg(msg_apps_type_e cur_event)
{
    //按键消息处理返回值
    app_result_e key_result = RESULT_IGNORE;
    //当前音乐播放状态
    //mengine_status_t *music_status = &g_music_status;
    //gui事件
    if(cur_event == EVENT_PLAYING_RETURN)
    {
        key_result = play_key_return();
    }

    return key_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  处理music的私有消息和系统消息
 * \param[in]    private_msg_t
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e alarm_message_deal(private_msg_t *msg)
{
    app_result_e result = RESULT_IGNORE;

    switch (msg->msg.type)
    {
        case MSG_SD_OUT:
        //music_get_filepath(&g_file_path_info);
        if (g_alarm.path.file_path.plist_location.disk == DISK_H)
        {
            music_close_engine();
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            result = RESULT_ALARM_QUIT;
        }
        break;

        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
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
 * \  处理gui消息
 * \param[in]    void para
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_sysmsg(void)
{
    private_msg_t alarm_msg;
    app_result_e msg_result = RESULT_IGNORE;
    if (get_app_msg(&alarm_msg) == TRUE)
    {
        msg_result = alarm_message_deal(&alarm_msg);
    }
    return msg_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  从引擎获取当前状态信息 判断是否出错 是否切换歌曲
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool result
 * \retval       none
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
bool _playing_check_status(void)
{    
    music_get_status(&g_music_status);//获取状态信息
    if (g_music_status.err_status != EG_ERR_NONE)
    {
        play_eror_deal(); //出错退出处理
        
        return FALSE;
    }
    if (g_music_status.play_status == StopSta)  // 循环播放
    {
        alarm_play();
    }
    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    alarm超时处理
 * \param[in]    void  para1
 * \param[out]   none
 * \return       none
 * \retval       
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 *******************************************************************************/
void alarm_timeout(void)
{
    //停止播放
    music_stop();
    //关闭engine
    music_close_engine();
    
    g_alarm_timeout =TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \  播放场景循环处理
 * \param[in]    void
 * \param[out]   none
 * \return       app_result_e result
 * \retval       none
 * \ingroup      alarm_playing_loopdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e playing_loop_deal(void)
{
    //获取gui消息
    input_gui_msg_t gui_msg;
    //gui 消息对应gui 事件
    msg_apps_type_e gui_event;
    style_infor_t style;
    //picbox_private_t picbox_param;
    bool result;

    app_result_e ret_vals = RESULT_IGNORE;
    //不使返回now_play，不关背光
    change_app_state(APP_STATE_PLAYING_ALWAYS);

    style.type = UI_AP;
    
    ui_show_pic(P_ALARM_BG,0,0);
    while (1)
    {
        //从引擎获取当前状态信息
        result = _playing_check_status();
        if ((result != TRUE))
        {
            ret_vals = RESULT_ALARM_QUIT;
            break;
        }

        //刷新alarm界面
        if (g_halfsec_timer == -1)
        {
            g_halfsec_timer = set_app_timer(APP_TIMER_ATTRB_UI, 500, play_scene_paint);
        }
        //播放30s定时
        if (g_tirtysec_timer == -1)
        {
            g_tirtysec_timer = set_app_timer(APP_TIMER_ATTRB_UI, ALARM_PLAY_TIME, alarm_timeout);
        }

        //获取和处理GUI消息、私有消息
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {
            if (com_key_mapping(&gui_msg, &gui_event, playing_key_map_list) == TRUE)
            {
                ret_vals = _scene_play_guimsg(gui_event);
            }
        }
        else
        {
            //一次循环处理一种类型的消息
            ret_vals = _scene_play_sysmsg();
        }
        if(g_alarm_timeout == TRUE)
        {
            ret_vals = RESULT_ALARM_QUIT;
        }

        if ((ret_vals != RESULT_IGNORE) && (ret_vals != RESULT_NULL))
        {
            break;
        }
        //挂起10ms 多任务交互
        sys_os_time_dly(2);
    }//end of while(1)
    //滚屏定时器取消
    if(g_halfsec_timer != -1)
    {
        g_halfsec_timer = -1;
        kill_app_timer(g_halfsec_timer);
    }
    if(g_tirtysec_timer != -1)
    {
        g_tirtysec_timer = -1;
        kill_app_timer(g_tirtysec_timer);
    }
    
    change_app_state(APP_STATE_NO_PLAY);
    return ret_vals;
}


