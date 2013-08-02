/*******************************************************************************
 *                              US211A
 *                            Module: music engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     fiona.yang     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
#include "app_mengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_status_deal(void)
 * \引擎状态处理,处理当前音乐播放状态
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
void mengine_status_deal(void)
{
    mmm_mp_status_t mmm_status;
    bool bret = TRUE;

    //获取当前播放状态
    mmm_mp_cmd(mp_handle, MMM_MP_GET_STATUS, (unsigned int) &mmm_status);
            
    //存放当前的播放时间和比特率
    if ((g_eg_status_p->play_status != PauseSta) && (g_eg_status_p->play_status != StopSta))
    {
        g_eg_playinfo_p->cur_time = (uint32) mmm_status.cur_time;
    }

    switch (mmm_status.status)
    {
        case MMM_MP_ENGINE_PLAYING://正常播放
        if (g_eg_status_p->play_status == PlayAB)/*ab 复读状态*/
        {
            //复读结束,回到正常播放状态
            if ((g_eg_status_p->ab_status == PlayAB_Do) && (mmm_status.ab_play_flag == AB_PLAY_CLEAR))
            {
                g_eg_status_p->ab_status = PlayAB_A;
            }
        }
        else if (g_eg_status_p->play_status == PlaySta)//正常播放
        {
            //处理浏览播放切换歌曲
            if ((g_eg_cfg_p->repeat_mode == FSEL_MODE_INTRO) && ((uint32) mmm_status.cur_time >= (g_intro_start
                    + INTRO_INTERVAL)))
            {
                mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_NORMAL);
            }

            //异常情况处理
            if (g_eg_status_p->fast_status != FBPlay_Null)
            {
                mmm_mp_cmd(mp_handle, MMM_MP_SET_FFB, (unsigned int) 0);
                g_eg_status_p->fast_status = FBPlay_Null;
            }
        }
        else
        {
            //快进退不理会浏览播放
            ;//do nothing
        }
        break;

        case MMM_MP_ENGINE_STOPPED://停止

        //sd 文件播放只需要停止
        if (g_eg_cfg_p->fsel_type == FSEL_TYPE_SDFILE)
        {
            _stop(STOP_NORMAL);//停止播放,将文件关闭
            break;
        }

        //正常播放到尾 or 快进到尾 or 快退到头停止
        if ((g_eg_status_p->play_status == PlaySta) || (g_eg_status_p->play_status == PlayFast)
                || (g_eg_status_p->play_status == PlayAB))
        {
            //正常播放到尾
            if (g_eg_status_p->fast_status == FBPlay_Null)
            {
                mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_NORMAL);
            }
            else if (g_eg_status_p->fast_status == FFPlaySta)
            {
                if (g_is_audible != TRUE)
                {
                    //快进到尾
                    mengine_file_switch(STOP_NORMAL, END_SWITCH, PLAY_FFON);
                }
                else
                {
                    //将文件标记为已播放完毕，清除之前的断点信息
                    //pos文件第15字节表示文件是否播放完毕
                    g_audible_info_p->playbackPosFilebuffer[15] = 0x01;
                }
            }
            else
            {
                if (g_is_audible != TRUE)
                {
                    //非audible歌曲支持从一首歌曲快退到另外一首歌曲
                    mengine_file_switch(STOP_NORMAL, FORCE_SWITCH_PREV, PLAY_FBON);
                }
            }
        }
        break;

        case MMM_MP_ENGINE_ERROR://出错

        //前台不是music UI或者前台进入屏保状态，出现错误直接切下一曲
        if ((g_app_info_state.app_state != APP_STATE_PLAYING) || (g_app_info_state.screensave_state == TRUE))
        {
            //前台不是music ui,则直接切下一曲
            _error_timeout_handle();
        }
        else
        {
            //首先获取错误号，因为stop命令会清除该错误号
            mengine_save_errno_no();

            //否则等待前台响应UI状态
            bret = _stop(STOP_NORMAL);//停止播放

            //恢复成默认状态,防止快进退遇到格式错误文件再次快进退
            g_eg_status_p->play_status = StopSta;
            g_eg_status_p->fast_status = FBPlay_Null;
            g_eg_status_p->ab_status = PlayAB_Null;
        }
        break;

        default:
        break;
    }
    if ((g_eg_status_p->play_status == StopSta) || (g_eg_status_p->play_status == PauseSta))
    {
        g_app_info_state.state = ENGINE_STATE_PAUSE;
    }
    else
    {
        g_app_info_state.state = ENGINE_STATE_PLAYING;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void _check_megine_status(void)
 * \检测引擎状态定时器函数，设置标志位
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
static void _check_megine_status(void)
{
    g_check_status_flag = TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \void mengine_control_block(void)
 * \引擎功能处理函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \ingroup      mengine_control.c
 * \note
 */
/*******************************************************************************/
app_result_e mengine_control_block(void)
{
    g_mengine_result = RESULT_NULL;

//    Codec_init(0);
    //250ms查询一次中间件状态
    g_status_timer = set_app_timer(APP_TIMER_ATTRB_CONTROL, 250, _check_megine_status);
    while (1)
    {
        //查询当前播放状态
        //if ((g_check_status_flag == TRUE) || (mengine_info.eg_status.play_status == PlayFast))
        if (g_check_status_flag == TRUE)
        {
            g_check_status_flag = FALSE;
            mengine_status_deal();
        }

        //获取并处理详细
        if (get_app_msg_for_engine(&g_cur_msg) == TRUE)
        {
            switch (g_cur_msg.msg.type)
            {
                /*获取信息相关*/
                case MSG_MENGINE_GET_PLAYMODE_SYNC://获取循环模式+shuffle
                mengine_get_playmode(&g_cur_msg);
                break;

                case MSG_MENGINE_GET_FILEPATH_SYNC://获取文件location信息
                mengine_get_filepath(&g_cur_msg);
                break;

                case MSG_MENGINE_GET_FILEINFO_SYNC://获取文件格式、b/s、总时间等信息
                mengine_get_fileinfo(&g_cur_msg);
                break;

                case MSG_MENGINE_GET_STATUS_SYNC://获取当前播放状态
                mengine_get_status(&g_cur_msg);
                break;

                case MSG_MENGINE_GET_PLAYINFO_SYNC://获取当前播放信息
                mengine_get_playinfo(&g_cur_msg);
                break;

                default:
                g_mengine_result = mengine_message_done_bank(&g_cur_msg);
                break;
            }
        }

        if (g_mengine_result == RESULT_APP_QUIT)
        {
            if (g_is_audible == TRUE)
            {
                mengine_save_audible_breakinfo();
            }
            //退出应用关闭处理
            _stop(STOP_NORMAL);
            break;
        }

        //挂起10ms 多任务交互
        sys_os_time_dly(g_delay_time);
    }
    
    Codec_exit();
    
    kill_app_timer(g_status_timer);
    return g_mengine_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e mengine_reply_msg(void* msg_ptr, bool ret_vals)
 * \回复同步消息
 * \param[in]    void  msg_ptr 消息结构体
 * \param[in]    bool  ret_vals 相关事件处理结果 TRUE 成功 FALSE 失败
 * \param[out]   none
 * \return       app_result_E
 * \retval       RESULT_IGNOR 忽略
 * \ingroup      mengine_control.c
 * \note  回复消息时，根据ret_vals的结果，设置应答成功或失败
 */
/*******************************************************************************/
void mengine_reply_msg(void* msg_ptr, bool ret_vals)
{
    //消息指针
    private_msg_t* data_ptr = (private_msg_t*) msg_ptr;

    if (ret_vals == TRUE)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }

    //回复同步消息(发送信号量)
    //reply_sync_msg(msg_ptr);
    libc_sem_post(data_ptr->sem);

    return;

    //return RESULT_IGNORE;
}

