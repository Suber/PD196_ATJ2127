/*******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_paint.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming       2011-10-27          v1.0
*******************************************************************************/
/*include file of this application, 本应用的头文件*/
#include "ap_record.h"


/******************************************************************************/
/*!
 * \par  Description:
 * \bool record_auto_start(void)
 * \自动开始录音
 * \param[in]    none
 * \param[out]   none
 * \return       bool
 * \retval
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
bool record_auto_start(void)
{
    //停止当前录音
    _recording_stop();

    //关闭当前录音
    _recording_close();

    g_scene_param.paint_flag = TRUE;
    rec_free_time = g_scene_param.free_time;
    SetFullInterface()

    //关闭中间件
    sys_free_mmm(0);

    //自动分区文件数目加1
    g_file_total++;
    
    //开始下一曲录音
    /*set record path , check disk info, check filenum, create filename*/
    if (!_record_file_setup())
    {
        return FALSE;
    }

    /*variables init*/
    rec_free_space = g_fs_param.free_space; //备份录音开始前的free space
    rec_free_time = g_scene_param.free_time; //备份录音开始前的free time
    g_scene_param.record_result = RESULT_NULL;
    rewrite_times = 0;

    sys_bank_flush(AP_BANK_FRONT_ENHANCED_1);
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);

    /*加载录音codec解码库*/
#ifndef PC
    if (-1 == sys_load_mmm("mmm_mr.al", 0))
    {
        return FALSE;
    }
#endif

    //g_mr_param.mr_status.status = MMM_MR_ENGINE_STOPPED;//看是否去掉
    _recording_start();

    return TRUE;
}


/******************************************************************************/
/*!
 * \par  Description:
 * \void alarm_fm_record_proc(void)
 * \定时录音超时
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
void alarm_fm_record_proc(void)
{
    alarm_rec_time--;
    if (alarm_rec_time == 0)
    {
        g_record_timeout = TRUE;
    }
}
/******************************************************************************/
/*!
 * \par  Description:
 * \static void _check_disk_info(void)
 * \检查录音状态，磁盘是否满，录音文件是否超过限制
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       none
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
static void _check_disk_info(void)
{
    int result;

#if 0
    result = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_SINGLE_THREAD, 0);
#endif

    /*获取中间件状态*/
    result = mmm_mr_cmd(g_mr_param.mr_handle, MMM_MR_GET_STATUS, (unsigned int) &g_mr_param.mr_status);
    if (result < 0)
    {
        goto check_disk_exit;
    }

    if (MMM_MR_ENGINE_ERROR == g_mr_param.mr_status.status)
    {
        if (REC_PLAY_STATUS == g_scene_param.record_status)
        {
            _recording_stop();
            _recording_close();
        }
        if (DISK_H == g_record_vars.path.file_path.dirlocation.disk)
        {
            if (sys_detect_disk(DRV_GROUP_STG_CARD) != 0)
            {
                //无卡提示
                gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
                fsel_exit();
                g_record_vars.path.file_path.dirlocation.disk = DISK_C;//自动切换到flash
                need_fsel_init = TRUE;
            }
        }

        if(g_entry_mode == PARAM_FROM_MAINMENU)
        {
            g_scene_param.record_result = RESULT_RECORD_MEMU;
        }
        else if(g_entry_mode == PARAM_FROM_RADIO)
        {
            g_scene_param.record_result = RESULT_RADIO_PLAY;
        }
        else
        {
            g_scene_param.record_result = RESULT_RECORD_MEMU;    
        }
        goto check_disk_exit;
    }

    /*获取磁盘空间信息*/
    g_fs_param.free_space = rec_free_space - (uint32) g_mr_param.mr_status.write_pages;
    g_scene_param.free_time = rec_free_time - (uint32) (g_mr_param.mr_status.time / 1000); /*codec返回的时间单位是ms*/

    //if (g_fs_param.free_space <= 0)
    if ((g_scene_param.free_time <= 0) || (g_fs_param.free_space <= 0))
    {
        /*录制过程中，空间不足，提示磁盘满*/
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, STR_DISK_FULL);

        if (REC_PLAY_STATUS == g_scene_param.record_status)
        {
            _recording_stop();
            _recording_close();
            g_scene_param.record_result = RESULT_SCENE_EXIT;
        }

        /*提示文件已保存*/
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, SAVED2);

        //确认保存录音文件，先关闭录音
        //_recording_close();

        if (g_entry_mode == PARAM_FROM_MAINMENU)
        {
            //如果是desktop进入ap，则录完一首后，返回到menu界面
            g_scene_param.record_result = RESULT_RECORD_MEMU;
        }
        else if (g_entry_mode == PARAM_FROM_RADIO)
        {
            //如果是fm进入ap，则退出应用返回到fm
            //g_scene_param.record_result = RESULT_SCENE_EXIT;
            g_scene_param.record_result = RESULT_RADIO_PLAY;
        }
        else
        {
            ;//qac need
        }
        goto check_disk_exit;
    }

    //自动分曲
    if (MMM_MR_ENGINE_STOPPED == g_mr_param.mr_status.status)
    {
        if ((g_record_vars.ai_source == AI_SOURCE_LINEIN) && (g_record_vars.track_mode == TRACK_ON))
        {
            record_auto_start();
            goto check_disk_exit;
        }
    }

    if (REC_FORMAT_WAV == g_record_vars.file_type)
    {
        //wav文件超过2G自动切换文件保存
        if (g_mr_param.mr_status.write_pages >= WAV_FILE_SIZE_MAX)
        {
            record_auto_start();
            goto check_disk_exit;
        }
    }
    else
    {
        //mp3文件超过2G自动切换文件保存
        if (g_mr_param.mr_status.write_pages >= MP3_FILE_SIZE_MAX)
        {
            record_auto_start();
            goto check_disk_exit;
        }
    }
    check_disk_exit: return;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _mktime(uint32 time_sec,time_timebox_t *time_buf)
 * \组织时间数据结构
 * \param[in]    time_sec 时间数值  para1
 * \param[in]    time_buf 存放时间结构的buffer
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      recoed_scene_recording.c
 * \note
 */
/*******************************************************************************/
void _mktime(uint32 time_sec, time_timebox_t *time_buf)
{
    time_buf->hour = (uint16)(time_sec / 3600);
    time_buf->minute = (uint8)((time_sec % 3600) / 60);
    time_buf->second = (uint8)(time_sec % 60);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \进度条显示
 * \param[in]    pdisplay_bitmap 刷新标志位
 * \param[in]    para TRUE 初始化 FALSE 正常刷新
 * \param[in]    type 0 timebox 1 progressbar
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      recoed_scene_recording.c
 * \note
 */
/*******************************************************************************/
void _record_show_time_progress(uint32 *pdisplay_bitmap, uint8 para, uint8 type)
{
    uint8 draw_mode;
    style_infor_t style;
    timebox_private_t timebox_data;
    progressbar_private_t progressbar_data;

    style.type = UI_AP;

    //显示timebox
    if (type == 0)
    {
        style.style_id = STY_RECORD_TIME;

        timebox_data.times[0].time_type = 'T';
        timebox_data.times[1].time_type = 'T';

        _mktime((uint32)(g_mr_param.mr_status.time / 1000), &(timebox_data.times[0].union_time.time));

        _mktime(g_scene_param.free_time, &(timebox_data.times[1].union_time.time));

        if (timebox_data.times[0].union_time.time.hour != 0)
        {
            timebox_data.times[0].mode = TIME_DISP_HHMMSS;
        }
        else
        {
            timebox_data.times[0].mode = TIME_DISP_MMSS;
        }

        if (timebox_data.times[1].union_time.time.hour != 0)
        {
            timebox_data.times[1].mode = TIME_DISP_HHMMSS;
        }
        else
        {
            timebox_data.times[1].mode = TIME_DISP_MMSS;
        }

        if(para == TRUE)
        {
            draw_mode = TIMEBOX_DRAW_ALL;
        }
        else
        {
            draw_mode = TIMEBOX_DRAW_TIME;
        }
        ui_show_timebox((void*) &style, (void*) &timebox_data, draw_mode);

        /*清显示标志*/
        *pdisplay_bitmap &= clr_recordTime;
    }
    else
    {
        style.style_id = STY_RECORD_PROGRESS;

        if (rec_free_time > 0xffff)
        {
            progressbar_data.total = (rec_free_time / 1000);
            progressbar_data.value = (uint32)(g_mr_param.mr_status.time / 1000 / 1000);
        }
        else
        {
            progressbar_data.total = (rec_free_time);
            progressbar_data.value = (uint32)(g_mr_param.mr_status.time / 1000);
        }

        if(para == TRUE)
        {
            draw_mode = PROGRESS_DRAW_ALL;
        }
        else
        {
            draw_mode = PROGRESS_DRAW_PROGRESS;
        }

        ui_show_progressbar((void*) &style, (void*) &progressbar_data, draw_mode);

        /*清显示标志*/
        *pdisplay_bitmap &= clr_recordProgress;
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 * \void _scene_record_proc_timer(void)
 * \record timer处理函数,刷新时间显示，检查录音磁盘信息
 * \param[in]    void
 * \param[out]   none
 * \return       void
 * \retval       none
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
void _scene_record_proc_timer(void)
{
    uint32 temp_val;

    //设置录音时间刷新标志
    if (get_screensave_state() == FALSE)
    {
        //g_scene_param.paint_flag = TRUE;
        g_scene_param.display_bitmap |= up_recordTime;
        g_scene_param.display_bitmap |= up_recordProgress;
    }

    //检查磁盘信息
    _check_disk_info();

    //录音断电保存
    {
        rewrite_times++;
        if (rewrite_times >= WAV_WRITE_BACK_TIMES)
        {
            temp_val = (uint32) g_mr_param.mr_status.time / 1000 / 60; //录音时间转化为分钟
            if (temp_val >= (uint32) (256 * vram_wr_cnt))
            {
                //由于RTC_ACCESS寄存器只有8bit可用，1分钟记1位，最大也只能记录255分钟，所以超过255后写一次vram
                //录音过程写vram时间较长，会导致录音断音，因此尽量减少写vram的次数
                vram_wr_cnt++;
                g_writehead.file_bytes = (uint32) g_mr_param.mr_status.write_pages * 512;
                sys_vm_write(&g_writehead, VM_AP_RECORD_WAV);
            }

            //RTC_ACCESS寄存器记录录音的分钟数
            temp_val = temp_val % 256;
            act_writel(temp_val, RTC_ACCESS);
            act_writel(0xA596, RTC_REGUPDATA);
            while (act_readl(RTC_REGUPDATA) != 0x5A69)
            {
                ;//do nothing
            }

            rewrite_times = 0;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e record_scene_recording(void)
 * \record场景处理函数
 * \param[in]    void
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       ......
 * \ingroup      record_scene_recording.c
 * \note
 */
/*******************************************************************************/
app_result_e record_scene_recording(void)
{
    bool bl_ret;
    app_result_e app_ret = RESULT_NULL;

    bl_ret = _scene_recording_init();
    g_scene_param.paint_flag = TRUE;
    rec_free_time = g_scene_param.free_time;
    SetFullInterface()

    if (bl_ret == FALSE)
    {
        if (g_entry_mode == PARAM_FROM_RADIO)
        {
            //如果是fm进入ap，则退出应用返回到fm
            return RESULT_RADIO_PLAY;
        }
        return RESULT_RECORD_MEMU;
    }

    change_app_state(APP_STATE_PLAYING);
    if(g_entry_mode == PARAM_FROM_RADIO)
    {
        change_app_state(APP_STATE_PLAYING_ALWAYS);
    }
    while (1)
    {
        if (g_scene_param.paint_flag == TRUE)
        {
            /*绘制场景界面,按照g_display_bitmap中定义的标志位来刷新图标*/
            _record_scene_paint(&g_scene_param.display_bitmap);
            g_scene_param.paint_flag = FALSE;
        }

        if((g_scene_param.display_bitmap & up_recordTime) != 0)
        {
            _record_show_time_progress(&(g_scene_param.display_bitmap), FALSE, 0);
        }

        if((g_scene_param.display_bitmap & up_recordProgress) != 0)
        {
            _record_show_time_progress(&(g_scene_param.display_bitmap), FALSE, 1);
        }

        app_ret = _scene_record_get_msg();

        //磁盘满g_scene_param.record_result != RESULT_NULL，退出循环
        if (g_scene_param.record_result != RESULT_NULL)
        {
            app_ret = g_scene_param.record_result;
        }

        //消息结果的处理
        if (app_ret == RESULT_REDRAW)
        {
            g_scene_param.paint_flag = TRUE;
            SetFullInterface()
            if (g_scene_param.record_status == REC_PAUSE_STATUS)
            {
                g_scene_param.display_bitmap &= clr_recordPlayIcon;
                g_scene_param.display_bitmap |= up_recordPauseIcon;
            }
        }
        else if (app_ret != RESULT_NULL)
        {
            break;
        }
        else
        {
            ; //QAC need
        }

        //定时录音超时的判断
        if (g_record_timeout == TRUE)
        {
            app_ret = RESULT_ALARM_QUIT;
            break;
        }
        /*挂起10ms，多任务交互*/
        sys_os_time_dly(1);
    }/*end of while*/

    /*调用场景退出函数*/
    _scene_recording_exit();
    change_app_state(APP_STATE_NO_PLAY);

    return app_ret;
}

