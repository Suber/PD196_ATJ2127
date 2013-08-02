/*******************************************************************************
 *                              US212A
 *                            Module: TOOLS_MAIN
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      liuweizhan    2011-12-12 17:03:16           1.0              build this file
 *******************************************************************************/
/*!
 * \file     tools_menu.c
 * \brief
 * \author   liuweizhan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2011-12-12
 *******************************************************************************/
#include "ap_tools.h"

const uint8 am[3] =
{ 'A', 'M', 0 };
const uint8 pm[3] =
{ 'P', 'M', 0 };
/*
 *****************************************************************************
 * \par  Description:
 *      普通说明 : stopwatch的timer中断处理函数
 *      重点说明 : 必须RCODE
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：无
 * \note: 
 *
 ******************************************************************************
 */

void StopWatch_timer_proc(void)
{
    if(CtcCntEnable != 0)
    {
        sw_realtime.Ms++;
        sw_realtime.Second = sw_realtime.Second + sw_realtime.Ms / 100;
        sw_realtime.Ms = sw_realtime.Ms % 100;
        sw_realtime.Minute = sw_realtime.Minute + sw_realtime.Second / 60;
        sw_realtime.Second = sw_realtime.Second % 60;
        sw_realtime.Hour = sw_realtime.Hour + sw_realtime.Minute / 60;
        sw_realtime.Minute = sw_realtime.Minute % 60;
    }
}


/******************************************************************************/
/*!
 * \par  Description:
 * \
 * \处理gui消息
 * \param[in]    gui_msg  para1
 * \param[in]
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup
 * \note
 */
/*******************************************************************************/
app_result_e tool_privmsg_deal(private_msg_t *private_msg)
{
    app_result_e result = RESULT_NULL;
    //msg_reply_t m_reply;
    /*同步消息处理*/

    switch (private_msg->msg.type)
    {
        case MSG_SD_OUT:
        if (g_dir_flag == TRUE)
        {
            //正在访问卡盘文件，退回到主应用
            if (get_engine_type() == ENGINE_MUSIC)
            {
                music_close_engine();
            }
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            result = RESULT_MAIN_APP;
            g_dir_flag = FALSE;
        }
        break;
        
        case MSG_APP_QUIT:
        result = RESULT_APP_QUIT;
        break;

        default:
        result = com_message_box(private_msg->msg.type);
        break;
    }

    return result;
}



