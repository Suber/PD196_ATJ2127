/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-14          1.0              create this file
 *******************************************************************************/
#include "app_music.h"

#include "app_music_playing.h"

const key_map_t playing_key_map_list[] =
{
    /*! 长按next快进 事件 */
    {{ KEY_NEXT, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FFOREWORD },
    /*! 长按prev快退 事件 */
    {{ KEY_PREV, 0, KEY_TYPE_LONG | KEY_TYPE_HOLD }, EVENT_PLAYING_FBACKWORD },
    /*! 短按按next切换下一曲or ab复读事件 */
    {{ KEY_NEXT, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_NEXTANDAB },
    /*! 短按按prev切换上一曲事件 */
    {{ KEY_PREV, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PREVSONG },
    /*! 短按option事件 */
    {{ KEY_MODE, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_OPTION },
    /*!短按play播放/暂停事件 */
    {{ KEY_PLAY, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_PAUSE },
    /*! 短按KEY_RETURN 按键转换为 返回事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_PLAYING_RETURN },
    /*! 结束标志 */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

const uint32 low_speed_level[] =
{ 1000, 2000, 4000, 5000, 6000 };

const uint32 high_speed_level[] =
{ 4000, 12000, 24000, 48000, 96000};
/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_apoint(void)
 * \设置复读a点
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
static bool music_set_apoint(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_SET_APOINT_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_bpoint(void)
 * \设置复读b点
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
static bool music_set_bpoint(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_SET_BPOINT_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_clear_ab(void)
 * \清除ab复读
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_clear_ab(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_CLEAR_AB_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_start_ab(void)
 * \启动ab复读
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_start_ab(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_START_AB_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_stop(void)
 * \停止播放音乐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_stop(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_STOP_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_pause(void)
 * \暂停播放音乐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_pause(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_PAUSE_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_resume(void)
 * \恢复播放音乐
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_resume(void)
{
    bool bret;
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_RESUME_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_fast_forward(void)
 * \快进
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_fast_forward(void)
{
    bool bret;
    //消息类型(即消息名称)快进
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_FFWD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_fast_backward(void)
 * \快退
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_fast_backward(void)
{
    bool bret;
    //消息类型(即消息名称)快退
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_FBWD_SYNC);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    调节music播放过程中快进退等级 共5级可调
 * \param[in]    none
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_play
 * \note
 * \li   等级越大，快进退间隔越大
 */
/*******************************************************************************/
bool music_fastplay_step(void)
{
    bool bret;
    uint32 fastplay_step;

    uint8 level = g_speed_counter / 5;
    if (level > 4)
    {
        level = 4;
    }

    if (g_total_time < 600000)
    {
        fastplay_step = low_speed_level[level];
    }
    else
    {

        fastplay_step = high_speed_level[level];
    }

    bret = music_send_msg(&fastplay_step, NULL, MSG_MENGINE_SET_FFBSTEP_SYNC);

    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_cancel_ffb(void)
 * \取消快进退
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
bool music_cancel_ffb(void)
{
    bool bret;
    //消息类型(即消息名称)取消快进退
    bret = music_send_msg(NULL, NULL, MSG_MENGINE_CANCEL_FFB_SYNC);
    
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool _scene_play_fast_keyup(void)
 * \处理快进退的消息
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       bool the result
 * \retval       true up消息
 * \retval       false 继续快进退
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
bool _scene_play_fast_keyup(void)
{
    input_gui_msg_t ui_msg;

    bool key_ret = FALSE;

    while (get_gui_msg(&ui_msg) == TRUE)
    {
        if ((ui_msg.data.kmsg.type & KEY_TYPE_SHORT_UP) == 0)
        {
            continue;
        }
        else
        {
            key_ret = TRUE;
            break;
        }
    }
    return key_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_prev(mengine_status_t* cur_status)
 * \处理prev 按键消息
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_prev(mengine_status_t* cur_status)
{

    if (cur_status->ab_status != PlayAB_Null)
    {
        //clear AB
        music_clear_ab();
        //等待设置a点
        music_start_ab();
        g_display_bitmap = g_display_bitmap | up_musicplaystatus;//绘制状态
        g_paint_flag = TRUE;//需要重新绘制界面
    }
    else
    {
        //快进退时取消快进退
        if (cur_status->play_status == PlayFast)
        {
            music_cancel_ffb();
            g_display_bitmap |= ( up_musicplaystatus | up_musictimeinfo | up_musicprogress);//绘制状态
            g_paint_flag = TRUE;//需要重新绘制界面
        }
        else //if (cur_status->play_status == PlaySta)
        {
            music_play_prev();//正常播放切换到上一曲
            //为出错处理使用
            g_switch_prev = 1;
            //SetSwitchInterface()//切换歌曲
            //g_getid3_info = TRUE;//需要重新获取id3信息
        }
        /*else
         {
         ;//其他状态保持keep
         }*/
    }

    //清消息队列，不处理过多的按键消息
    sys_mq_flush(MQ_ID_GUI);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_next(mengine_status_t* cur_status)
 * \处理next按键消息
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_next(mengine_status_t* cur_status)
{

    //停快进快退
    if (cur_status->play_status == PlayFast)
    {
        music_cancel_ffb();
        g_display_bitmap |= ( up_musicplaystatus | up_musictimeinfo | up_musicprogress);//绘制状态
        g_paint_flag = TRUE;//需要重新绘制界面
    }
    else
    {
        switch (cur_status->ab_status)
        {
            case PlayAB_Null:
            music_play_next();//正常播放切换到下一曲          
            //g_getid3_info = TRUE;//需要重新获取id3信息
            break;

            case PlayAB_A:
            music_set_apoint();
            g_display_bitmap = g_display_bitmap | up_musicplaystatus;//绘制状态
            g_paint_flag = TRUE;//需要重新绘制界面
            break;

            case PlayAB_B:
            music_set_bpoint();
            g_display_bitmap = g_display_bitmap | up_musicplaystatus;//绘制状态
            g_paint_flag = TRUE;//需要重新绘制界面
            break;

            case PlayAB_Do:
            //clear AB
            music_clear_ab();
            g_display_bitmap = g_display_bitmap | up_musicplaystatus;//绘制状态
            g_paint_flag = TRUE;//需要重新绘制界面
            break;

            default:
            break;
        }
    }
    //清消息队列，不处理过多的按键消息
    sys_mq_flush(MQ_ID_GUI);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_play(mengine_status_t* cur_status)
 * \处理next按键消息
 * \param[in]    status  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_play(mengine_status_t* cur_status)
{
    if ((cur_status->play_status == PauseSta) || (cur_status->play_status == StopSta))
    {
        //play
        music_resume();
    }
    else if ((cur_status->play_status == PlaySta) || (cur_status->play_status == PlayAB))
    {
        if (cur_status->ab_status != PlayAB_Null)
        {
            //clear AB
            music_clear_ab();
        }
        //pause
        music_pause();
    }
    else
    {
        ;//nothing
    }
    g_display_bitmap = g_display_bitmap | up_musicplaystatus;//绘制状态
    g_paint_flag = TRUE;//需要重新绘制界面
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint16 play_key_return(void)
 * \处理return按键消息
 * \param[in]    void  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_keymsg.c
 * \note
 */
/*******************************************************************************/
app_result_e play_key_return(void)
{
    uint16 retval;
    switch (g_enter_mode)
    {
        //从brower选择了文件播放//设置播放文件发送消息给引擎
        case PARAM_FROM_RECORD:
        case PARAM_FROM_BROWSER:
        //send message create browser
        retval = RESULT_RETURN_BROWSER;
        break;
        
        case PARAM_BACKTO_PLAYING:
        case PARAM_BACKTO_PLAYED:
        //从main menu选择了music图标
        case PARAM_FROM_MAINMENU:
        //获取文件路径
        music_get_filepath(&g_file_path_info);
        if (g_file_path_info.file_source < FSEL_TYPE_SDFILE)
        {
            if(g_music_config.browser_sel == TRUE)
            {
                //如果是从browser选歌,则重新返回browser
                retval = RESULT_RETURN_BROWSER;
            }
            else
            {
                //否则返回main ap,例如standby起来断点已不存
                //从磁盘重新选歌
                retval = RESULT_MAIN_APP;
            }
        }
        else
        {
            retval = RESULT_LISTMENU_LIST;
        }
        break;

        default://其他情况
        retval = RESULT_LISTMENU_LIST;
        break;
    }
    return retval;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \int _scene_play_gui(msg_gui_t *gui_msg)
 * \处理gui消息
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_playdeal.c
 * \note
 */
/*******************************************************************************/
app_result_e _scene_play_guimsg(input_gui_msg_t *gui_msg)
{
    //gui 消息对应gui 事件
    msg_apps_type_e cur_event;

    //按键消息处理返回值
    app_result_e key_result = RESULT_IGNORE;

    //当前音乐播放状态
    mengine_status_t *music_status = &g_music_status;

    if ((g_music_status.play_status == PlayFast) && ((gui_msg->data.kmsg.type & KEY_TYPE_SHORT_UP) != 0))
    {
        //快进退状态收到任何up消息都转换为结束快进退事件
        cur_event = EVENT_PLAYING_NEXTANDAB;
    }
    else
    {
        //未找到有效的GUI映射
        if (com_key_mapping(gui_msg, &cur_event, playing_key_map_list) == FALSE)
        {
            //退出当前GUI事件循环
            goto msg_end;
        }
    }

    //处理gui事件
    switch (cur_event)
    {
        //发送消息快进
        case EVENT_PLAYING_FFOREWORD:
        //发送消息快退
        case EVENT_PLAYING_FBACKWORD:
        if (music_status->play_status != PlayFast)
        {
            g_speed_counter = 0;
            
            //设置初始步长
            music_fastplay_step(); 

            //设置快进退状态
            if (cur_event == EVENT_PLAYING_FFOREWORD)
            {
                music_fast_forward();
            }
            else
            {
                music_fast_backward();
            }
            g_display_bitmap |= up_musicplaystatus;
            g_paint_flag = TRUE;
        }
        else
        {
            if (_scene_play_fast_keyup() == TRUE)
            {
                music_cancel_ffb();
                g_display_bitmap |= ( up_musicplaystatus | up_musictimeinfo | up_musicprogress);
                g_paint_flag = TRUE;
            }
            else
            {
                if ((g_speed_counter % 5) == 0)
                {
                    music_fastplay_step();
                }
            }
        }
        break;

        //发送消息上一曲
        case EVENT_PLAYING_PREVSONG:
        key_result = play_key_prev(music_status);
        break;

        //发送消息下一曲/AB复读
        case EVENT_PLAYING_NEXTANDAB:
        key_result = play_key_next(music_status);
        break;

        //option按键
        case EVENT_PLAYING_OPTION:
        if (g_audible_now == TRUE)
        {
            key_result = RESULT_SETMENU_AUDIBLE;
        }
        else
        {
            key_result = RESULT_SETMENU_MUSIC;
        }
        break;

        //播放暂停
        case EVENT_PLAYING_PAUSE:
        key_result = play_key_play(music_status);
        break;

        //返回按键
        case EVENT_PLAYING_RETURN:
        key_result = play_key_return();
        break;

        default:
        stop_app_timer(scroll_frush_id);
        stop_app_timer(time_frush_id);

        key_result = com_message_box(cur_event);
        //恢复界面刷新定时器
        if ((key_result == RESULT_IGNORE) || (key_result == RESULT_NULL))
        {
            key_result = RESULT_REDRAW;
        }
        restart_app_timer(scroll_frush_id);
        //1s定时器重新启动
        restart_app_timer(time_frush_id);
        break;
    }
    if (key_result == RESULT_REDRAW)
    {
        //刷新界面标识
        SetFullInterface()
        g_paint_flag = TRUE;//需要重新绘制界面
        key_result = RESULT_NULL;
    }

    if (g_paint_flag == TRUE)
    {
        //获取当前播放状态
        //music_get_status(&g_music_status);
        g_check_mengine_status = TRUE;
    }
    msg_end: return key_result;
}

