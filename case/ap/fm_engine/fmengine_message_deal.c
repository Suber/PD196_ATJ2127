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

extern uint32 volume_deal(uint8 vol_in) __FAR__;

#ifdef test

//测试代码，暂不调用Analog 驱动，在此处操作硬件寄存器，使FM  出声音
void init_fm_pa_out(void)
{
    int i;

    /***********************init pa and fm in************************/

    //set dac clock
    *((REG32) (CLKENCTL)) |= (1 << CLKENCTL_DACCLKEN);

    //enable DAC Module, so dac reg(include pa reg can be set ?)
    *((REG32) (MRCR)) |= (1 << MRCR_DACIISReset);

    //set adc clock
    *((REG32) (CLKENCTL)) |= (1 << CLKENCTL_ADCCLKEN);

    //enable ADC Module, so adc reg
    *((REG32) (MRCR)) |= (1 << MRCR_ADCReset);

    //set analog source input: 1 for fm
    *((REG32)(ADC_CTL)) = ((*((REG32)(ADC_CTL)) & (~(ADC_CTL_ANAINS_MASK))) | (1 << ADC_CTL_ANAINS_SHIFT));

    //enable L & R channel
    *((REG32)(ADC_CTL)) |= (1 << ADC_CTL_AINLEN);
    *((REG32)(ADC_CTL)) |= (1 << ADC_CTL_AINREN);

    /*2 for 0db as default: gain level 0-7*/
    *((REG32)(ADC_CTL)) = (*((REG32)(ADC_CTL)) & (~(ADC_CTL_AING_MASK))) | (2 << ADC_CTL_AING_SHIFT);

    /*delay*/
    for (i = 0; i < 0x2000; i++)
    {
        //i += 1;
    }

    //MIXEN, analog mixer and PA enable
    *((REG32) (DAC_ANALOG1)) |= 1 << DAC_ANALOG1_PMP_PAEN;
    /*pavcc connect vcc*/
    *((REG32) (DAC_ANALOG1)) |= 1 << DAC_ANALOG1_PVCTV;
    //PMP mode AA MUTE disable
    *((REG32)(DAC_ANALOG1)) |= 1 << DAC_ANALOG1_PMP_AAMUTE;

    /*32 for PA volume level : 0 - 41*/
    *((REG32) (DAC_ANALOG1)) |= (32 << DAC_ANALOG1_Volume_SHIFT) & DAC_ANALOG1_Volume_MASK;

    /*pa output stage enable, can output now*/
    *((REG32) (DAC_ANALOG2)) |= 1 << DAC_ANALOG2_OSEN;

    /*delay*/
    for (i = 0; i < 0x4000; i++)
    {
        //i += 1;
    }

}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_init(private_msg_t* cur_msg)
 * \FM 模组初始化消息处理，负责调用驱动执行初始化操作
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note   FM 模组初始化配置，并配置PA , FM 增益等
 */
/*******************************************************************************/
app_result_e fm_module_init(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 band_info, level;
    int result;
    app_result_e ret;
    uint32 fm_ain_type = FM_AIN_TYPE;
    uint32 fm_ain_gain = FM_AIN_GAIN;

    band_info = (data_ptr->msg.content.data[0]); //初始化配置频段信息
    level = g_fmengine_cfg.fm_threshold; //初始化设置搜台门限

#ifndef PC
    result = fm_open(band_info, level, 0);
#else
    result = 1;
#endif

    //初始化成功
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;
        /* 硬件操作*/
#ifndef test
        //if (g_fmengine_cfg.paddv_mode == PA_DDV_MODE)
        //{
        //    enable_pa_ddv(0, 0); //静音，使能PA
        //}
        //else
        //{
        //    enable_pa(0, 0); //静音，使能PA
        //}
        com_set_sound_out(TRUE, SOUND_OUT_START, NULL);
        enable_ain(fm_ain_type, fm_ain_gain);
#endif

        //测试代码，硬件操作
#ifdef test
        init_fm_pa_out();
#endif

        //模组初始化成功，处于work 状态
        g_module_status = MODULE_WORK;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL; //模组初始化失败，退出引擎AP
        g_module_status = MODULE_NO_INIT;
    }

    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_reinstall(private_msg_t* cur_msg)
 * \ Restore FM Driver global data，so no need to initial fm driver
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note
 *    调用该接口恢复FM驱动所使用的全局数组WriteBuffer内容，因此可避免重新初始化FM模组
 */
/*******************************************************************************/
app_result_e fm_module_reinstall(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 band_info, level;
    int result;
    app_result_e ret;
    uint16 freq;

    band_info = (data_ptr->msg.content.data[0]); //初始化配置频段信息
    freq = data_ptr->msg.content.data[2];
    freq <<= 8;
    freq |= (data_ptr->msg.content.data[1]);
    level = g_fmengine_cfg.fm_threshold; //初始化设置搜台门限

#ifndef PC
    result = fm_open(band_info, level, freq);
#else
    result = 1;
#endif

    //初始化成功
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        ret = RESULT_IGNORE;

        //模组初始化成功，处于work 状态
        g_module_status = MODULE_WORK;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
        ret = RESULT_NULL; //模组初始化失败，退出引擎AP
        g_module_status = MODULE_NO_INIT;
    }

    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);

    return ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fm_module_standby(void)
 * \FM 模组进Standby  消息处理，负责调用驱动执行
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note
 */
/*******************************************************************************/
app_result_e fm_module_standby(void)
{
    int result;
    uint32 fm_ain_type = FM_AIN_TYPE;

    /* 有些模组可能存在漏电，在未初始化时就需要让其进入standby */
    if (g_module_status != MODULE_STANDBY)
    {
#ifndef PC
        result = fm_close();
#else
        result = 1;
#endif
        //进入Standby 成功
        if (result != 0)
        {
#ifndef test
            if (g_module_status == MODULE_WORK)
            {
                /*硬件操作*/
                set_pa_volume(0);
                disable_ain(fm_ain_type);
                //if (g_fmengine_cfg.paddv_mode == PA_DDV_MODE)
                //{
                //    disable_pa_ddv(); //直驱
                //}
                //else
                //{
                //    disable_pa(); //非直驱
                //}
                com_set_sound_out(FALSE, SOUND_OUT_STOP, NULL);
            }
#endif
            //模组进入Standby 状态
            g_module_status = MODULE_STANDBY;
        }
    }
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
 * \同步设置单个频点，开始FM 播放
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note:   频率值的设置，对于FM ui 及引擎应用而言是统一的
 * \ 具体针对各种模组的转换在驱动中完成
 **/
/*******************************************************************************/
app_result_e fmengine_set_freq_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint16 freq;
    int result;

    freq = *(uint16*) &(data_ptr->msg.content.data[0]);
#ifndef PC
    result = fm_set_freq(freq);
#else
    result = 1;
#endif

    /* 设置频点成功*/
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
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
 * \app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
 * \设置静音或者解除静音
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_mute_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 vol_bak;
    FM_MUTE_e mode;
    int result = 0;
    uint32 set_vol;

    mode = (FM_MUTE_e) data_ptr->msg.content.data[0];
    vol_bak = data_ptr->msg.content.data[1];

#ifndef PC
    result = fm_mute(mode);
#else
    result = 1;
#endif

#ifndef test
    /* 解除静音*/
    if (mode == releaseMUTE)
    {
        //set_pa_volume((uint32) vol_bak);
        /* 按音量表转成软件音量 */
        set_vol = volume_deal(vol_bak);
        set_pa_volume(set_vol);
    }
    /* 静音*/
    else if (mode == SetMUTE)
    {
        set_pa_volume(0);
    }
    else
    {
        ; /* 一般不会跑这里 */
    }
#endif

    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        if (mode == releaseMUTE)
        {
            g_fmengine_status.FM_VolStatus = STATUS_PLAY;

            change_engine_state(ENGINE_STATE_PLAYING);
        }
        else if (mode == SetMUTE)
        {
            g_fmengine_status.FM_VolStatus = STATUS_PAUSE;

            change_engine_state(ENGINE_STATE_PAUSE);
        }
        else
        {
            //;
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
 * \app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
 * \设置电台波段
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_set_band_sync(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;
    radio_band_e band_info;

    //需设置的电台波段，见radio_band_e
    band_info = (radio_band_e) data_ptr->msg.content.data[0];
#ifndef PC
    result = fm_set_band(band_info);
#else
    result = 1;
#endif

    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
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
 * \app_result_e fmengine_set_throd(private_msg_t* cur_msg)
 * \设置搜台门限
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_set_throd(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint8 level;
    int result;

    level = data_ptr->msg.content.data[0];
#ifndef PC
    result = fm_set_throd(level);
#else
    result = 1;
#endif
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        g_fmengine_cfg.fm_threshold = level;
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
 * \app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
 * \进行软件搜台
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       RESULT_APP_QUIT 失败退出
 * \note:  单个频点设置，并判断是否真台，真台回复成功
 **/
/*******************************************************************************/
app_result_e fmengine_set_softseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint16 now_freq;
    int result;
    uint8 dir;

    //软件搜台接口函数参数
    now_freq = *(uint16*) &(data_ptr->msg.content.data[0]); //软件搜台当前判断频点
    dir = data_ptr->msg.content.data[2]; //搜台方向

#ifndef PC
    result = fm_soft_seek(now_freq, dir);
#else
    result = 0;
#endif
    g_fmengine_status.FM_CurrentFreq = now_freq; //更新引擎层当前频点值

    if (result != 0)
    {
        //有效电台
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
    }
    else
    {
        //无效电台
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
 * \启动硬件搜台
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       硬件搜台启动成功，回复成功
 * \note:   搜台方向说明
 * bit0   ~~~  向上或向下搜台。0:   UP;    1: DOWN
 * bit1   ~~~  是否边界回绕。     0:   回绕;    1:  不回绕
 **/
/*******************************************************************************/
app_result_e fmengine_set_hardseek(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    uint16 now_freq;
    uint8 dir;
    int result;

    //硬件搜台接口函数参数
    now_freq = *(uint16*) &(data_ptr->msg.content.data[0]); //硬件搜台起始频点
    dir = data_ptr->msg.content.data[2]; //搜台方向

    //启动硬件搜台
#ifndef PC
    result = fm_hard_seek(now_freq, dir);
#else
    result = 1;
#endif

    if (result != 0)
    {
        //启动成功
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
        g_fmengine_status.STC_flag = HARDSEEK_DOING;
    }
    else
    {
        //启动失败
        data_ptr->reply->type = MSG_REPLY_FAILED;
        g_fmengine_status.STC_flag = HARDSEEK_NOT_START;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_seek_break(private_msg_t* cur_msg)
 * \用于硬件搜台过程中，手动停止自动搜台过程
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略;
 * \retval       成功停止seek  过程，回复成功；否则回复失败
 * \note:
 **/
/*******************************************************************************/
app_result_e fmengine_seek_break(private_msg_t* cur_msg)
{
    private_msg_t* data_ptr = (private_msg_t*) cur_msg;
    int result;

#ifndef PC
    result = fm_set_breakseek();
#else
    result = 1;
#endif

    //成功停止硬件搜台
    if (result != 0)
    {
        data_ptr->reply->type = MSG_REPLY_SUCCESS;
#ifndef PC
        fm_get_freq(&g_fmengine_status.FM_CurrentFreq); //更新当前频率
#endif
        g_fmengine_status.STC_flag = HARDSEEK_COMPLETE;
    }
    else
    {
        data_ptr->reply->type = MSG_REPLY_FAILED;
    }
    //回复同步消息(发送信号量)
    reply_sync_msg(cur_msg);
    return RESULT_IGNORE;

}

