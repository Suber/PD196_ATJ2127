/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      mikeyang    2011-11-9 16:45:04           1.0              build this file
 *******************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \RadioUI  获取私有消息的处理
 * \param[in]    msg  私有消息结构
 * \param[out]   none
 * \return  app_result_e 结果消息
 * \note RadioUI 的处理的各种私有消息的回调函数
 *******************************************************************************/
app_result_e radio_app_msg_callback(private_msg_t *pri_msg)
{
    app_result_e ret_result = RESULT_NULL;
    engine_type_e engine_type;

    switch (pri_msg->msg.type)
    {
        case MSG_APP_QUIT:
        ret_result = RESULT_APP_QUIT;
        break;

        /* 回到正在播放界面 */
        case MSG_APP_BACKTO_PLAYING:
        if (g_ss_delay_flag == TRUE)//delay退出屏保
        {
            gui_screen_save_exit();
            g_ss_delay_flag = FALSE;
        }

        //获取当前后台引擎类型
        engine_type = get_engine_type();
        if (engine_type == ENGINE_MUSIC)
        {
            //当前后台为music, 需回到音乐
            ret_result = RESULT_MUSIC;
        }
        else
        {
            //回到radio播放场景下, 不重设频率,避免杂音
            need_set_freq = FALSE;
            ret_result = RESULT_RADIO_START_PLAY;
        }
        break;

        default:
        ret_result = com_message_box(pri_msg->msg.type);
        if (ret_result == RESULT_ALARMFM) //播放过程中遇fm alarm消息
        {
            g_radio_from_alarm = 1;
            alarmfm_in_playing();
            ret_result = RESULT_REDRAW;
        }
        break;
    }

    return ret_result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void init_point_freq(void)
 * \初始化指向播放频率表的指针
 * \param[in]    void
 * \param[out]   none
 * \return
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
void init_point_freq(void)
{
    /* 当前播放为用户电台模式*/
    if (g_radio_config.band_mode == Band_MAX)
    {
        pfreq = &g_user_station.fmstation_user[0];
    }
    /* 当前播放为预设电台模式*/
    else
    {
        if (g_radio_config.band_mode == Band_Japan)
        {
            pfreq = &g_radio_config.fmstation_jp[0];
        }
        else if (g_radio_config.band_mode == Band_Europe)
        {
            pfreq = &g_radio_config.fmstation_eu[0];
        }
        else
        {
            pfreq = &g_radio_config.fmstation_us[0];
        }
    }
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \uint8 get_station_count(void)
 * \param[in]    none
 * \param[out]   none
 * \return    saved station number
 * \retval
 * \retval
 * \note   获取当前模式( 波段) 下的保存电台总数
 */
/*************************************************************************************************/
uint8 get_station_count(void)
{
    uint16 *ptr = pfreq;
    uint8 num, i;

    num = 0;
    for (i = 0; i < MAX_STATION_COUNT; i++)
    {
        //if(*ptr !=0)
        if ((*ptr <= max_freq[g_menu_band]) && (*ptr >= min_freq[g_menu_band]))
        {
            num++;
        }
        ptr++;
    }
    return num;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \uint8 whether_in_list( uint16  freq, uint16 *p_freq)
 * \判断指定频率的电台是否在当前的保存列表中
 * \param[in]    freq
 * \param[in]    p_freq
 * \param[out]   none
 * \return       当前频率在指定的列表中返回电台号，否则返回0xff
 * \retval       none
 * \retval       none
 * \note
 */
/*******************************************************************************/
uint8 whether_in_list(uint16 freq, uint16 *p_freq)
{
    uint8 ret;
    uint16 *ptr;
    uint8 num;
    uint16 tmp;

    /* 当前波段电台列表*/
    ptr = p_freq;

    for (num = 0; num < MAX_STATION_COUNT; num++)
    {
        tmp = *(ptr + num);
        if (tmp == freq)
        {
            //在列表中已找到
            break;
        }
    }

    //在列表中未找到
    if (num == MAX_STATION_COUNT)
    {
        ret = 0xff;
    }
    else
    {
        ret = num + 1;
    }
    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e show_dialog(uint16 id, uint8 dialog_type, uint8 default_icon)
 * \RadioUI 使用的dialog 子函数
 * \param[in]    id  显示的字符串id；dialog_type  对话框类型; default_icon:默认项
 * \param[out]   none
 * \return   app_result_e 结果消息
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e show_dialog(uint16 id, uint8 dialog_type, uint8 default_icon)
{
    app_result_e result;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    uint16 style_id;

    string_desc.data.id = id;
    string_desc.language = UNICODEID;

    dialog_com.dialog_type = dialog_type;
    if (dialog_type != DIALOG_BUTTON_INCLUDE)
    {
        dialog_com.button_type = BUTTON_NO_BUTTON;
        style_id = DIALOG_MSG;
    }
    else
    {
        //目前看，radioUI 中只使用到是和否按钮
        dialog_com.button_type = BUTTON_YESNO;
        dialog_com.active_default = default_icon;
        style_id = DIALOG_ASK;
    }
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    result = gui_dialog(style_id, &dialog_com);
    return result;
}

/******************************************************************************
 *   部分常用命令放在常驻段
 *******************************************************************************/
/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_freq(uint16 freq)
 * \同步设置频率播放
 * \param[in]    freq  设置的频率值( AP 层)
 * \param[out]   none
 * \return
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_freq(uint16 freq)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    //静音, 避免频率切换过程中的杂音
    //set_pa_volume(0);
    radio_set_mute(SetMUTE);
    msg.type = MSG_FMENGINE_SETFREQ_SYNC;
    msg.content.data[0] = (uint8)(freq & 0xff);
    msg.content.data[1] = (uint8)((freq & 0xff00) >> 8);
    //发送同步消息
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;
    }
    //解除静音，设置为当前音量
    //set_pa_volume(g_comval.volume_current);
    radio_set_mute(releaseMUTE);
    return bret;
}

#if 0  //常驻rcode空间不够, 此函数放bank
/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_mute(FM_MUTE_e mode)
 * \设置静音或解除静音
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_mute(FM_MUTE_e mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = (uint8) mode;
    msg.content.data[1] = (uint8) g_comval.volume_current;

    msg.type = MSG_FMENGINE_SETMUTE;
    //发送同步消息
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;

    }
    return bret;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_get_seekflag(void)
 * \获取硬件搜台是否完成标志
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   本次硬件搜台完成
 * \retval           0 failed     获取失败或硬件搜台尚未完成
 * \note
 */
/*******************************************************************************/
bool radio_get_seekflag(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_AUTO_SEEK_OVER;
    msg_reply.content = &g_engine_status.STC_flag;

    //发送同步消息
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if ((msg_reply.type == MSG_REPLY_FAILED) || (g_engine_status.STC_flag == HARDSEEK_DOING))
    {
        bret = FALSE;

    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_softseek(uint16 freq)
 * \设置软件搜台频率
 * \param[in]    freq, dir
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note   软件搜台:   如果当前频率为真台，返回TRUE
 */
/*******************************************************************************/
bool radio_set_softseek(uint16 freq, uint8 dir)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_SOFTSEEK;
    msg.content.data[0] = (uint8)(freq & 0xff);
    msg.content.data[1] = (uint8)((freq & 0xff00) >> 8);
    msg.content.data[2] = dir;
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
 * \bool radio_set_hardseek(uint16 freq, uint8 dir)
 * \设置启动硬件搜台
 * \param[in]    freq-- 硬件搜台起始频率
 * \param[in]    dir --- 搜台方向
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   启动硬件搜台成功
 * \retval           0 failed
 * \note  搜台方向说明
 * bit0   ~~~  向上或向下搜台。0:   UP;    1: DOWN
 * bit1   ~~~  是否边界回绕。     0:   回绕;    1:  不回绕
 */
/*******************************************************************************/
bool radio_set_hardseek(uint16 freq, uint8 dir)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_HARDSEEK;
    msg.content.data[0] = (uint8)(freq & 0xff);
    msg.content.data[1] = (uint8)((freq & 0xff00) >> 8);
    msg.content.data[2] = dir;
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
 * \bool radio_break_hardseek(void)
 * \退出硬件搜台过程
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   退出硬件搜台成功
 * \retval           0 failed
 * \note   用于在硬件搜台过程中响应按键终止硬件搜台
 */
/*******************************************************************************/
bool radio_break_hardseek(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_SEEKBREAK;

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
 * \bool radio_get_band(void)
 * \获取当前频率信息
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_freq(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_FREQ;
    msg_reply.content = &g_engine_status.FM_CurrentFreq;

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
 * \bool radio_get_band(void)
 * \获取电台列表中下一个或上一个已保存电台的电台号
 * \param[in]     num 初始电台号  dir   方向
 * \param[out]   获得的电台号
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
uint8 get_real_station_num(uint8 num, FM_SeekDir_e dir)
{
    uint16 freq;
    uint8 i;

    //原始频率值
    //freq = *(pfreq+g_radio_config.FMStatus.station-1);

    for (i = 0; i < MAX_STATION_COUNT; i++)
    {
        freq = *(pfreq + num - 1);
        if ((freq >= min_freq[g_radio_config.band_mode]) && (freq <= max_freq[g_radio_config.band_mode]))
        {
            //找到有效台，退出
            g_radio_config.FMStatus.station = num;
        }
        else
        {
            if (dir == DIR_UP)
            {
                num++;
                if (num > MAX_STATION_COUNT)
                {
                    num = 1;
                }
            }
            else
            {
                num--;
                if (num == 0)
                {
                    num = MAX_STATION_COUNT;
                }
            }
        }
    }
    return g_radio_config.FMStatus.station;
}

/*! \endcond */

