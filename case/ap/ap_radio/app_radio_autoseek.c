/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

//转动图片张数
#define NUM_ICON      8

extern bool radio_AutoSearch_init(FM_SeekDir_e dir)
__FAR__;
extern bool radio_AutoSearch_exit(void)
__FAR__;
extern void auto_display_init(void)
__FAR__;

/* radio 全自动搜台按键事件映射*/
const key_map_t autoseek_key_map_list[] =
{
    /*! 短按KEY_VOL 按键转换为 返回事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! 最后一个映射事件 */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 * \void radio_auto_proc(void)
 * \全自动搜台场景界面刷新定时器
 * \param[in]    none
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note  转动图片定时刷新
 */
/*******************************************************************************/
void radio_auto_proc(void)
{
    style_infor_t auto_sty;
    picbox_private_t picbox_param;

    ui_icon_count = (ui_icon_count + 1) % (NUM_ICON);

    auto_sty.style_id = PICBOX_SEARCH;
    auto_sty.type = UI_AP;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = ui_icon_count;
    //picbox_param.direction = DIRECTION_NORMAL;
    ui_show_picbox(&auto_sty, &picbox_param);
}

/****************************************************************************************/
/*!
 * \par  Description:
 * \void update_progress(uint16 freq, uint8 mode)
 * \全自动搜台过程中，根据当前频率刷新进度条
 * \param[in]    value:   当前频率值
 * \param[in]    mode:   是否到尾更新0:  否  1: 是
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   这是按照向上搜台刷新进度条, 一般全自动搜台都是SEEK UP
 */
/******************************************************************************************/
void update_progress(uint16 freq, uint8 mode)
{
    style_infor_t auto_sty;
    progressbar_private_t progressbar_param;
    uint32 tmp;

    auto_sty.type = UI_AP;
    auto_sty.style_id = PBAR_SEARCH;
    progressbar_param.total = 118;

    if (mode == 1)
    {
        //搜台到尾刷新整个进度条
        progressbar_param.value = progressbar_param.total;
    }
    else
    {
        /* 76MHz ~~ 90MHz */
        if (g_menu_band == Band_Japan)
        {
            tmp = (uint32)((freq - 0x28e0) * 118);
            progressbar_param.value = (uint16)(tmp / (0x36b0));
        }
        /* 87.5MHz ~~ 108MHz */
        else
        {
            tmp = (uint32)((freq - 0x55cc) * 118);
            progressbar_param.value = (uint16)(tmp / (0x5014));
        }
    }
    ui_show_progressbar(&auto_sty, &progressbar_param, PROGRESS_DRAW_PROGRESS);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e _scene_autoseek_sysmsg(void)
 * \全自动搜台系统消息及私有消息处理
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e scene_autoseek_sysmsg(void)
{
    private_msg_t autoseek_msg;
    app_result_e msg_result = RESULT_NULL;
    if (get_app_msg(&autoseek_msg) == TRUE)
    {
        msg_result = radio_app_msg_callback(&autoseek_msg);
    }
    return msg_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_hard_autoseek(FM_SeekDir_e dir)
 * \硬件全自动搜台处理
 * \param[in]    direction:   搜台方向
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e deal_hard_autoseek(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 tab_num, dir_seek;
    bool result, need_restart = TRUE;
    uint16 current_freq, end_freq;
    //获取gui消息
    input_gui_msg_t gui_msg;
    //gui 消息对应gui 事件
    msg_apps_type_e gui_event;

    //设置当前波段, 主要是确定space
    radio_set_band(g_menu_band);
    //起始频率
    current_freq = g_radio_config.FMStatus.freq;
    tab_num = 0;

    //先确定方向,是否回绕
    //全自动搜台，一般从最低到最高搜索一遍，不回绕
    if (dir == DIR_UP)
    {
        //向上搜台,  不回绕
        dir_seek = 0x02;
    }
    else
    {
        //向下搜台, 不回绕
        dir_seek = 0x03;
    }

    //设置结束频率
    if (g_menu_band == Band_Japan)
    {
        if (dir == DIR_UP)
        {
            end_freq = MAX_JP; // 90M
        }
        else
        {
            end_freq = MIN_JP; // 76M
        }
    }
    else
    {
        if (dir == DIR_UP)
        {
            end_freq = MAX_US; // 108M
        }
        else
        {
            end_freq = MIN_US; // 87.5M
        }
    }
    while (1)
    {
        if (need_restart == TRUE)
        {
            result = radio_set_hardseek(current_freq, dir_seek);
            need_restart = FALSE;
            if (!result)
            {
                //硬件搜台启动失败
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }
        }

        //硬件搜台启动成功,  取是否完成标志
        result = radio_get_seekflag();
        if (result == TRUE)
        {
            //此轮搜台完成,  获取当前频率
            radio_get_freq();
            current_freq = g_engine_status.FM_CurrentFreq;

            if (((current_freq >= end_freq) && (dir == DIR_UP)) || ((current_freq <= end_freq) && (dir == DIR_DOWN))
                    || ((g_engine_status.STC_flag == HARDSEEK_NOT_START)))
            {
                //已到截至频点或未搜到有效台, 退出搜台
                radio_break_hardseek();
                update_progress(current_freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }

            if (g_engine_status.STC_flag == HARDSEEK_COMPLETE)
            {
                //过滤96MHZ 及 108MHZ 频点
                if ((current_freq != 0x7700) && (current_freq != 0xa5e0))
                {
                    //保存有效电台
                    Auto_tab[tab_num] = current_freq;
                    tab_num++;
                }
            }

            //未到截至频点, 需启动新一轮搜台
            need_restart = TRUE;
            //tab_num++;
        }
        else
        {
            //此轮搜台尚未完成,  取频率主要是更新进度条
            radio_get_freq();
            current_freq = g_engine_status.FM_CurrentFreq;
        }

        //电台列表已满
        if (tab_num >= MAX_STATION_COUNT)
        {
            radio_break_hardseek();
            update_progress(current_freq, 1);
            ret = RESULT_RADIO_STATION_LIST;
            break;
        }

        //根据当前频点，更新进度条
        update_progress(current_freq, 0);

        //处理系统消息和gui 消息
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {

            if (com_key_mapping(&gui_msg, &gui_event, autoseek_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_RADIO_RETURN)
                {
                    //return 停止搜台,返回电台列表
                    radio_break_hardseek();
                    ret = RESULT_RADIO_STATION_LIST;
                    break;
                }
            }
        }
        else
        {
            //一次循环处理一种类型的消息
            ret = scene_autoseek_sysmsg();
        }

        if ((ret != RESULT_IGNORE) && (ret != RESULT_NULL))
        {
            break;
        }
        //挂起20ms 多任务交互
        sys_os_time_dly(2);
    }

    //保存电台数
    g_radio_config.FMStatus.station_count = tab_num;
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e radio_AutoSearch_scene(FM_SeekMode_e mode, FM_SeekDir_e dir)
 * \全自动搜台场景
 * \param[in]    mode:  搜台模式( 硬件搜台or  软件搜台)
 * \param[in]    dir: 搜台方向( 向上或向下)
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note 全自动搜台，清除当前菜单波段对应的列表
 * \       并按照当前菜单波段进行搜台
 */
/*******************************************************************************/
app_result_e radio_AutoSearch_scene(FM_SeekMode_e mode, FM_SeekDir_e dir)
{
    app_result_e result;
    bool bret;

#ifndef PC
    /* 初始化*/
    bret = radio_AutoSearch_init(dir);
    if (!bret)
    {
        //初始化失败返回
        return RESULT_ERROR;
    }

    //开始全自动搜台前，先mute
    radio_set_mute(SetMUTE);

    auto_display_init();

    //启动定时器
    restart_app_timer(timer_auto_seek);

    if (mode == HARDSEEK)
    {
        //处理硬件全自动搜台
        result = deal_hard_autoseek(dir);
    }
    else
    {
        //处理软件全自动搜台
        result = deal_soft_autoseek(dir);
    }

    radio_AutoSearch_exit();
#else
    result = RESULT_RADIO_STATION_LIST; 
#endif
    return result;

}

