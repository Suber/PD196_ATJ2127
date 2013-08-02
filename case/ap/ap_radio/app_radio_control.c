/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang    2011-09-22          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_open_engine(void)
 * \装载FM 后台引擎进程
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 * \note
 */
/*******************************************************************************/
bool radio_open_engine(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = APP_ID_FMENGINE;
    msg.content.data[1] = 0;
    //创建radio 后台引擎进程
    msg.type = MSG_CREAT_APP_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MANAGER, &msg, &msg_reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_close_engine(void)
 * \卸载引擎
 * \param[in]    void
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note  radioUI 中一般不需要进行引擎的卸载，但预留此接口
 * \        FM 引擎的卸载一般由其他进程发送卸载命令
 */
/*******************************************************************************/
bool radio_close_engine(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //杀死radio 后台引擎
    msg.type = MSG_KILL_APP_SYNC;
    msg.content.data[0] = APP_ID_FMENGINE;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    return bret;
}

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

    msg.content.data[0] = (uint8) g_radio_config.band_mode;

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
 * \bool radio_modual_reinstall(void)
 * \初始化FM  模组
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed--- 同步消息发送失败，或模组初始化失败
 * \note
 */
/*******************************************************************************/
bool radio_modual_reinstall(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = (uint8) g_radio_config.band_mode;
    msg.content.data[1] = (uint8) (g_radio_config.FMStatus.freq & 0xff);
    msg.content.data[2] = (uint8) (g_radio_config.FMStatus.freq >> 8);

    //初始化FM 模组
    msg.type = MSG_FMENGINE_RESINTALL;
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
 * \bool radio_modual_standby(void)
 * \FM 模组进入Standby
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_modual_standby(void)
{
    bool bret = TRUE;
    msg_apps_t msg;

    //FM 模组进入standby
    msg.type = MSG_FMENGINE_STANDBY;
    //发送异步消息
    bret = send_async_msg(APP_ID_FMENGINE, &msg);
    return bret;
}

/*******************************************************************************
 *                                          设置信息相关
 ********************************************************************************/

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_set_band(radio_band_e mode)
 * \设置电台波段
 * \param[in]    mode
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_band(radio_band_e mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = (uint8) mode;

    msg.type = MSG_FMENGINE_SETBAND;
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
 * \bool radio_set_throd(uint8 val)
 * \设置搜台门限
 * \param[in]    val 电台门限值
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_throd(uint8 val)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = val;

    msg.type = MSG_FMENGINE_SETTHRED;
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
 * \bool radio_set_volume(uint8 vol)
 * \设置音量值
 * \param[in]     vol
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess   设置音量成功
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_set_volume(uint8 vol)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_VOLUME;
    msg.content.addr = &vol;//g_comval.volume_current;

    //发送同步消息
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if (msg_reply.type == MSG_REPLY_FAILED)
    {
        bret = FALSE;

    }
    return bret;
}

/*******************************************************************************
 *                                          获取信息相关
 ********************************************************************************/

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_get_status(uint8 mode)
 * \获取当前引擎状态信息
 * \param[in]     mode
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note   mode 获取状态节点:   0-- 正常播放时取
 * \note   1-- 硬件seek 过程中取
 */
/*******************************************************************************/
bool radio_get_status(uint8 mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_STATUS;
    msg.content.data[0] = mode;
    msg_reply.content = &g_engine_status;

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
 * \获取当前波段信息
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_band(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_BAND;
    msg_reply.content = &g_engine_status.FM_CurBand;

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
 * \bool radio_get_intensity(void)
 * \获取当前信号强度
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_intensity(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_INTENSITY;
    msg_reply.content = &g_engine_status.FM_SignalStg;

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
 * \bool radio_get_antenna(void)
 * \获取当前天线状态
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess    天线已插入
 * \retval           0 failed     获取失败或天线未插入
 * \note
 */
/*******************************************************************************/
bool radio_get_antenna(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;
    uint8 status;

    msg.type = MSG_FMENGINE_GET_ANTENNA;
    msg_reply.content = &status;

    //发送同步消息
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    if ((msg_reply.type == MSG_REPLY_FAILED) || (status == 0))
    {
        bret = FALSE;

    }
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_get_stereo(void)
 * \获取当前立体声状态
 * \param[in]     none
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_stereo(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_STEREO;
    msg_reply.content = &g_engine_status.FM_Stereo_Status;

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
 * \bool radio_get_vol_status(void)
 * \获取当前引擎状态，播放or  暂停
 * \param[in]    none
 * \param[out]   none
 * \return       status
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_vol_status(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_PLAYSTATUS;
    msg_reply.content = &g_engine_status.FM_VolStatus;
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
 * \bool radio_get_volume(void)
 * \获取当前音量值
 * \param[in]    none
 * \param[out]   none
 * \return       status
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_volume(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_VOLUME;
    msg_reply.content = &g_comval.volume_current;
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
 * \bool radio_get_modual_status(Module_Status_e* status)
 * \获取当前模组状态
 * \param[in]    none
 * \param[out]   none
 * \return       status
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool radio_get_modual_status(Module_Status_e* status)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_GET_MODUAL_STATUS;
    msg_reply.content = status;
    //发送同步消息
    bret = send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void alarmfm_in_playing(void)
 * \alarmfm 处理
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \retval           
 * \retval           
 * \note
 */
/*******************************************************************************/
void alarmfm_in_playing(void)
{
    alarm_vars_t temp_alarm_val;
    if (g_radio_from_alarm != 0)
    {
        sys_vm_read(&temp_alarm_val, VM_ALARM, sizeof(alarm_vars_t));
        //节省空间，只取与FM有关信息
        libc_memcpy(&alarmfm_end_time, &temp_alarm_val.g_aufm.end_time, sizeof(time_t)); //定时FM的结束时间
        g_radio_config.FMStatus.freq = temp_alarm_val.g_aufm.frequency; //FM频段
        //alarmfm_volume = temp_alarm_val.g_aufm.volume; //FM音量
        g_comval.volume_current = temp_alarm_val.g_aufm.volume; //FM音量
        radio_set_freq(g_radio_config.FMStatus.freq);
        radio_set_volume(g_comval.volume_current);//alarmfm_volume);
    }
}
