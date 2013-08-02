/*******************************************************************************
 *                              us212A
 *                            Module: common
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-7-24 11:14     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _antenna_detect.c
 * \brief    common 耳机（天线）检测及处理接口实现
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               耳机（天线）检测及处理接口实现。
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-7-24
*******************************************************************************/

#include "common_ui.h"

extern bool fm_set_mute(uint8 mode);

/*! \cond */

/******************************************************************************/
/*!
 * \par  Description:
 * \bool fm_set_mute(FM_MUTE_e mode)
 * \设置静音或解除静音
 * \param[in]    mode    0:   解除静音  1: 静音
 * \param[out]   none
 * \return       void the result
 * \retval           1 sucess
 * \retval           0 failed
 * \note
 */
/*******************************************************************************/
bool fm_set_mute(uint8 mode)
{
    bool bret = TRUE;
    msg_apps_t msg;
    msg_reply_t msg_reply;

    msg.content.data[0] = mode;
    msg.content.data[1] = sys_comval->volume_current;

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
 *    耳机（天线）状态更改处理
 * \param[in]    in_out TRUE 表示检测到插入，FALSE 表示检测到拔出
 * \param[out]   none
 * \return       none
 * \note 
*******************************************************************************/
void deal_antenna_detect(bool in_out)
{
    //更改耳机（天线）状态
    g_app_info_state.antenna_state = in_out;
    
    if(get_engine_type() == ENGINE_RADIO)
    {
        if(g_this_app_info->app_id == APP_ID_RECORD)//FM 录音中，直接操作 PA 音量
        {
            if(in_out == FALSE)
            {
                //静音
                set_pa_volume(0);
            }
            else
            {
                //解除静音，设置为当前音量
                set_pa_volume(sys_comval->volume_current);
            }
        }
        else
        {
            if(in_out == FALSE)
            {
                //耳机拔出, 静音
                fm_set_mute(1);  
            }
            else
            {
                //耳机插入, 解除静音
                fm_set_mute(0);
            }
        }
    }  
}

/*! \endcond */
