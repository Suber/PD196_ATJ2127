/*******************************************************************************
 *                              US211A
 *                            Module: radio engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     mikeyang     2011-11-03 15:00     1.0             build this file
 *******************************************************************************/
#include "app_fmengine.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_seekflag(private_msg_t* cur_msg)
 * \用于硬件搜台过程中，获取硬件搜台是否结束标识
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/

app_result_e fmengine_get_seekflag(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    uint8 over_flag;

#ifndef PC
    result = fm_get_seekflag(&over_flag);
#else
    result = 1;
    over_flag = 0;
#endif

    //成功获取到标识
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;

        //硬件搜台结束
        if ((over_flag & 0x01) == 1) //bit0  为1
        {
            if ((over_flag & 0x02) != 0) //bit1 为1
            {
                //有效台
                *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_COMPLETE;
                g_fmengine_status.STC_flag = HARDSEEK_COMPLETE;
            }
            else
            {
                //未找到有效台
                *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_NOT_START;
                g_fmengine_status.STC_flag = HARDSEEK_NOT_START;
            }
        }
        else
        {
            *(hardseek_status_e*) data_ptr->reply->content = HARDSEEK_DOING;
            g_fmengine_status.STC_flag = HARDSEEK_DOING;
        }
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_status(private_msg_t* cur_msg)
 * \获取状态信息
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_status(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    Engine_Status_t engine_status_tmp;
    uint8 mode;

    //获取状态节点:   0-- 正常播放时取   1-- 硬件seek 过程中取
    mode = data_ptr->msg.content.data[0];
#ifndef PC
    result = fm_get_status(&engine_status_tmp, mode);
#else
    result = 0;
#endif
    //获取状态成功
    if (result != 0)
    {
        engine_status_tmp.FM_VolStatus = g_fmengine_status.FM_VolStatus;
        libc_memcpy(&g_fmengine_status, &engine_status_tmp, sizeof(Engine_Status_t));

        //消息应答
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        //将引擎状态回复给RadioUI
        libc_memcpy(data_ptr->reply->content, &g_fmengine_status, sizeof(Engine_Status_t));
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_band(private_msg_t* cur_msg)
 * \获取波段信息
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_band(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
#ifndef PC
    result = fm_get_band(&g_fmengine_status.FM_CurBand);
#else
    result = 1;
    g_fmengine_status.FM_CurBand = Band_China_US;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(radio_band_e*) data_ptr->reply->content = g_fmengine_status.FM_CurBand;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_freq(private_msg_t* cur_msg)
 * \获取当前频率
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_freq(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
#ifndef PC
    result = fm_get_freq(&g_fmengine_status.FM_CurrentFreq);
#else
    result = 0;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(uint16*) data_ptr->reply->content = g_fmengine_status.FM_CurrentFreq;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_intensity(private_msg_t* cur_msg)
 * \获取当前信号强度
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_intensity(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;

#ifndef PC
    result = fm_get_intensity(&g_fmengine_status.FM_SignalStg);
#else
    result = 0;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(uint16*) data_ptr->reply->content = g_fmengine_status.FM_SignalStg;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_antenna(private_msg_t* cur_msg)
 * \获取天线状态
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_antenna(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    uint8 antena_status;
#ifndef PC
    result = fm_get_antena(&antena_status);
#else
    result = 0;
#endif
    if (result != 0)
    {
        //获取状态成功
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        //回执，天线是否插入
        *(uint8*) data_ptr->reply->content = antena_status;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_stereo(private_msg_t* cur_msg)
 * \获取立体声状态
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_stereo(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
#ifndef PC
    result = fm_get_stereo(&g_fmengine_status.FM_Stereo_Status);
#else
    result = 0;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        *(FM_Audio_e*) data_ptr->reply->content = g_fmengine_status.FM_Stereo_Status;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_playflag(private_msg_t* cur_msg)
 * \获取当前引擎状态(播放or  暂停)
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_playflag(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;

    *(FM_Playstatus_e*) data_ptr->reply->content = g_fmengine_status.FM_VolStatus;
    data_ptr->reply->type = MSG_REPLY_SUCCESS;
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_volume(private_msg_t* cur_msg)
 * \获取当前音量值
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_volume(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 cur_vol;

#ifndef test
    cur_vol = (uint8) get_pa_volume();
#else
    cur_vol = (uint8)((*((REG32) (DAC_ANALOG1)) & DAC_ANALOG1_Volume_MASK)>>DAC_ANALOG1_Volume_SHIFT);
#endif
    *(uint8*) data_ptr->reply->content = cur_vol;
    data_ptr->reply->type = MSG_REPLY_SUCCESS;
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_get_modual_status(private_msg_t* cur_msg)
 * \获取FM 模组当前状态
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_get_modual_status(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;

    *(Module_Status_e*) data_ptr->reply->content = g_module_status;
    data_ptr->reply->type = MSG_REPLY_SUCCESS;
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

