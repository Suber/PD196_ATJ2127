/********************************************************************************
 *                               us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      wzliu         2011-10-13         v1.0              create this file
 ********************************************************************************/
 /*!
 * \file     record_alarm.c
 * \brief    定时fm录音的相关函数
 * \author   wzliu
 * \version  1.0
 * \date  2011/10/13
 *******************************************************************************/
#include "ap_record.h"
#include "fm_interface.h"
#include "fmengine.h"

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

    msg.type = MSG_FMENGINE_SETFREQ_SYNC;
    msg.content.data[0] = (uint8) (freq & 0xff);
    msg.content.data[1] = (uint8) ((freq & 0xff00) >> 8);
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
bool radio_set_volume(uint8 volume)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.type = MSG_FMENGINE_SET_VOLUME;
    msg.content.addr = &volume;

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
 * \bool radio_modual_reinstall(void)
 * \重新设置FM默认全局变量的值
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

    //任意值
    uint16 freq = 0x1247;

    msg.content.data[0] = (uint8) 0x02;
    msg.content.data[1] = (uint8) (freq & 0xff);
    msg.content.data[2] = (uint8) (freq >> 8);

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
 * \bool radio_close_engine(void)
 * \卸载引擎
 * \param[in]    void  para1
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
    
    radio_modual_reinstall();  //fm初始化数据被冲掉，此处必须重新初始化！
    
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
 * \bool radio_get_modual_status(Module_Status_e* status)
 * \获取当前模组状态
 * \param[in]    none
 * \param[out]   Module_Status_e* status
 * \return       bool the result
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
 * \bool radio_engine_init(void)
 * \引擎初始化
 * \param[in]    void  para1
 * \param[out]   none
 * \return        success/fail
 * \retval   True:   引擎已经安装，模组已经初始化
 * \retval   False:  安装引擎失败或初始化模组失败
 * \note
 */
/*******************************************************************************/
bool radio_engine_init(void)
{
    engine_type_e engine_type;
    msg_apps_t msg;
    bool result = TRUE;
    bool need_init = FALSE;

    //获取当前后台引擎类型
    engine_type = get_engine_type();
    msg.type = MSG_KILL_APP_SYNC;    //注销原有非radio 后台
    if (engine_type == ENGINE_MUSIC)
    {
        msg.content.data[0] = APP_ID_MENGINE;
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0); //发送同步消息
    }

    //当前未安装引擎，需安装
    if (engine_type != ENGINE_RADIO)
    {
        if (get_antenna_state() == FALSE)
        {
            //提示需插入耳机
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, INSERT_EARPHONE);
            return FALSE;
        }
        
        result = radio_open_engine();
        if (result == TRUE)
        {
            need_init = TRUE; //需初始化模组
        }
    }
    //当前已安装引擎，需判断模组是否已经初始化
    //录音过程可能会将fm 驱动的 常驻数据空间覆盖,  所以, 直接发一遍初始化命令
    else
    {
        //radio_get_modual_status(&status);
        //if (status != MODULE_WORK)
        {
            need_init = TRUE;
        }
    }

    if (need_init == TRUE)
    {
        // 模组初始化之前先mute
        //radio_set_mute(SetMUTE);
        //初始化模组
        result = radio_modual_init();
        //result = TRUE;    //调试阶段
#if 1     //调试阶段
        if (!result)
        {//模组初始化失败，提示硬件错误
            //show_dialog(FMERROR, DIALOG_INFOR_WAIT, 0);
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, MDIROUT);
        }
        else
#endif
        {
            //初始化后，先mute，刷新界面后，开始播放
            radio_set_mute(SetMUTE); //releaseMUTE
            //first_wait = TRUE;
        }
    }
    else
    {
        ;//first_wait = FALSE;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool radio_alarm_init(void)
 * \fm初始化
 * \param[in]    none
 * \param[out]   none
 * \return      success/fail
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
bool radio_alarm_init(void)
{
    if (!radio_engine_init())
    {
        return FALSE; //初始化失败返回
    }
    //初始化后，先mute，刷新界面后，开始播放
    if (!radio_set_mute(SetMUTE)) //releaseMUTE
    {
        return FALSE;
    }
    if (!radio_set_freq(fmrec_frequency))
    {
        return FALSE;
    }
    if (!radio_set_volume(fmrec_volume))
    {
        return FALSE;
    }
    if (!radio_set_mute(releaseMUTE))
    {
        return FALSE;
    }
    /*
     //连接耳机作为收音机天线
     show_dialog(USE_ANT, DIALOG_INFOR_WAIT, 0);

     if(timer_auto_seek != -1)
     {
     kill_app_timer(timer_auto_seek);
     }
     //设置定时器，但暂不启动
     timer_auto_seek = set_app_timer(APP_TIMER_ATTRB_UI, 300, radio_auto_proc);
     */
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _radio_get_freq(void)
 * \获取当前频率信息
 * \param[in]     none
 * \param[out]   none
 * \return       none
 * \note
 */
/*******************************************************************************/
void _radio_get_freq(void)
{
    msg_apps_t msg;
    msg_reply_t msg_reply;
    uint32 temp_freq;
    uint32 tmp;
    uint16 engine_freq;

    msg.type = MSG_FMENGINE_GET_FREQ;
    msg_reply.content = &engine_freq;

    //发送同步消息
    send_sync_msg(APP_ID_FMENGINE, &msg, &msg_reply, 0);
    //    if (msg_reply.type == MSG_REPLY_FAILED)
    //    {
    //        ;
    //    }

    temp_freq = (uint32)engine_freq + 0x10000; //获取出来的fm频率需要+0x10000
    //填写fm频点用于显示
    tmp = temp_freq / 1000;
    if (tmp < 100)
    {
        libc_itoa(tmp, g_scene_param.fm_freq, 2);
        g_scene_param.fm_freq[2] = '.';
        tmp = (temp_freq - tmp * 1000) / 100;
        libc_itoa(tmp, &g_scene_param.fm_freq[3], 1);
        g_scene_param.fm_freq[4] = 'F';
        g_scene_param.fm_freq[5] = 'M';
        g_scene_param.fm_freq[6] = 0;
    }
    else
    {
        libc_itoa(tmp, g_scene_param.fm_freq, 3);
        g_scene_param.fm_freq[3] = '.';
        tmp = (temp_freq - tmp * 1000) / 100;
        libc_itoa(tmp, &g_scene_param.fm_freq[4], 1);
        g_scene_param.fm_freq[5] = 'F';
        g_scene_param.fm_freq[6] = 'M';
        g_scene_param.fm_freq[7] = 0;
    }
}

