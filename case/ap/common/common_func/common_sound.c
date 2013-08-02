/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2012-5-18 11:44     1.0             build this file 
*******************************************************************************/
/*!
 * \file     _common_sound.c
 * \brief    声音输出管理接口实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了对按键音，各应用声音输出协调管理。
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version  1.0
 * \date     2012-5-18
*******************************************************************************/

#include "common_func.h"

#define KEY_TONE_VOLUME     12//按键音固定为 PA 12

//#define AUDIO_DEVICE_PA_DDV

/*! \cond COMMON_API */

/******************************************************************************/
/*!                     
 * \par  Description:
 *    设置按键音使能与否。
 * \param[in]    kt_enable 按键音是否使能，0表示禁止，1表示使能
 * \param[out]   none    
 * \return       bool
 * \retval           TRUE 设置按键音成功
 * \retval           FALSE 设置按键音失败 
 * \note 
*******************************************************************************/
bool com_set_sound_keytone(uint8 kt_enable)
{
    soundout_state_e cur_state;
    bool last_sound_flag, sound_flag;
    bool dac_flag;
    
    //获取设置前声音状态
    cur_state = (soundout_state_e)get_soundout_state();
    if((get_keytone_enable() == TRUE) || (cur_state != SOUND_OUT_STOP))
    {
        last_sound_flag = TRUE;
    }
    else
    {
        last_sound_flag = FALSE;
    }
    
    //获取音频输出dac状态
    if((cur_state == SOUND_OUT_START) || (cur_state == SOUND_OUT_RESUME))
    {
        dac_flag = TRUE;
    }
    else
    {
        dac_flag = FALSE;
    }
    
    if(kt_enable != 0)
    {
        //enable key tone
        g_app_info_state.sound_state |= (uint8)SOUND_BIT_KT_ENABLE;
    }
    else
    {
        //disable key tone
        g_app_info_state.sound_state &= (uint8)SOUND_BIT_KT_DISABLE;
    }
    
    //获取设置后声音状态
    if((get_keytone_enable() == TRUE) || ((soundout_state_e)get_soundout_state() != SOUND_OUT_STOP))
    {
        sound_flag = TRUE;
    }
    else
    {
        sound_flag = FALSE;
    }
    
    //关闭dac
    if((dac_flag == FALSE) && (kt_enable == 0))
    {
        disable_dac();
        key_speakcheck(0);//禁止speaker输出
    }
    
    //声音状态从关到开，打开pa
    if((last_sound_flag == FALSE) && (sound_flag == TRUE))
    {
        //打开pa
#ifdef AUDIO_DEVICE_PA_DDV
        enable_pa_ddv(0, 0);
#else
        enable_pa(0, 0);
#endif
    }
    //声音状态从开到关，关闭pa
    else if((last_sound_flag == TRUE) && (sound_flag == FALSE))
    {
        //关闭pa
#ifdef AUDIO_DEVICE_PA_DDV
        disable_pa_ddv(0, 0);
#else
        disable_pa();
#endif
    }
    else
    {
        //do nothing for QAC
    }
    
    //打开dac，并设置按键音pa
    if((dac_flag == FALSE) && (kt_enable != 0))
    {
        key_speakcheck(1);//使能speaker输出
        enable_dac(2, 44);//没有音频输出，dac是关掉的，需要开启dac
        set_pa_volume(KEY_TONE_VOLUME);
        
        //设置按键音参数
        set_key_tone_vol(24);
        set_key_tone_freq(2);
        set_key_tone_period(2);
    }
    
    return TRUE;
}

/******************************************************************************/
/*!                     
 * \par  Description:
 *    设置声音音量。
 * \param[in]    volume 设置音量值
 * \param[out]   none    
 * \return       bool
 * \retval           TRUE 设置音量成功
 * \retval           FALSE 设置音量失败 
 * \note 
*******************************************************************************/
bool com_set_sound_volume(uint8 volume)
{
    bool bret = TRUE;
    
    //保存当前音量值
    set_current_volume(volume);
    
    //只有正在播放音乐或收音时才需要调整音量
    if(get_engine_state() == ENGINE_STATE_PLAYING)
    {
        msg_apps_t msg;
        engine_type_e type;
    
        type = get_engine_type();
        if(type == ENGINE_MUSIC)
        {
            //设置音量
            msg.content.addr = &volume; 
            //消息类型(即消息名称)
            msg.type = MSG_MENGINE_SET_VOLUME_SYNC; 
            //发送同步消息
            bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
        }
        else if(type == ENGINE_RADIO)
        {
            //设置音量
            msg.content.addr = &volume; 
            //消息类型(即消息名称)
            msg.type = MSG_FMENGINE_SET_VOLUME; 
            //发送同步消息
            bret = send_sync_msg(APP_ID_FMENGINE, &msg, NULL, 0);
        }
        else
        {
            //do nothing for QAC
        }
    }
    
    return bret;
}

/******************************************************************************/
/*!                    
 * \par  Description:
 *    启动或关闭音频输出。
 * \param[in]    sound_out 是否音频输出，TRUE为是，FALSE为否
 * \param[in]    state 声音输出状态，见 soundout_state_e 定义
 * \param[in]    sound_func 启动或停止设备声音输出回调函数，可以为 NULL
 * \param[out]   none 
 * \return       bool
 * \retval           TRUE 启动或关闭音频输出成功。
 * \retval           FALSE 启动或关闭音频输出失败。
 * \ingroup      common_func
 * \note 
 * \li  关闭音频输出时，如果按键音使能，则进行按键音音量设置。
*******************************************************************************/
bool com_set_sound_out(bool sound_out, soundout_state_e state, void * sound_func)
{
    bool last_sound_flag, sound_flag;
    
    sound_out = sound_out;
    //获取设置前声音状态
    if((get_keytone_enable() == TRUE) || ((soundout_state_e)get_soundout_state() != SOUND_OUT_STOP))
    {
        last_sound_flag = TRUE;
    }
    else
    {
        last_sound_flag = FALSE;
    }
    
    //设置音频输出状态
    g_app_info_state.sound_state &= (uint8)SOUND_BIT_SOUND_OUT_CLR;
    g_app_info_state.sound_state |= (uint8)state;
    
    //关闭音频输出先于关闭pa和dac
    //关闭音频输出，若有按键音，设置按键音pa
    if((state == SOUND_OUT_STOP) || (state == SOUND_OUT_PAUSE))
    {
        if(sound_func != NULL)
        {
            sound_stop_func __stop_sound = (sound_stop_func)sound_func;
            __stop_sound();
        }
        
        if(get_keytone_enable() == TRUE)
        {
            enable_dac(2, 44);//从音频输出退出，dac是关掉的，需要开启dac
            set_pa_volume(KEY_TONE_VOLUME);
            
            //设置按键音参数
            set_key_tone_vol(24);
            set_key_tone_freq(2);
            set_key_tone_period(2);
        }
        else//已经不再需要声音输出，可以禁止speaker输出
        {
            key_speakcheck(0);//禁止speaker输出
        }
    }
    
    //获取设置后声音状态
    if((get_keytone_enable() == TRUE) || ((soundout_state_e)get_soundout_state() != SOUND_OUT_STOP))
    {
        sound_flag = TRUE;
    }
    else
    {
        sound_flag = FALSE;
    }
    
    //声音状态从关到开，打开pa
    if((last_sound_flag == FALSE) && (sound_flag == TRUE) && (state == SOUND_OUT_START))
    {
        //打开pa
#ifdef AUDIO_DEVICE_PA_DDV
        enable_pa_ddv(0, 0);
#else
        enable_pa(0, 0);
#endif
    }
    //声音状态从开到关，关闭pa
    else if((last_sound_flag == TRUE) && (sound_flag == FALSE) && (state == SOUND_OUT_STOP))
    {
        //关闭pa
#ifdef AUDIO_DEVICE_PA_DDV
        disable_pa_ddv(0, 0);
#else
        disable_pa();
#endif
    }
    else
    {
        //do nothing for QAC
    }
    
    //启动音频输出后于打开pa和dac
    //启动音频输出，设置当前音量值
    if((state == SOUND_OUT_START) || (state == SOUND_OUT_RESUME))
    {
        if(get_keytone_enable() == TRUE)
        {
            disable_dac();//从按键音进入音频输出，要求dac是关掉的，需要关闭dac
        }
        else//之前没有使能speaker输出，需要在此先使能，再在AP中开启dac
        {
            key_speakcheck(1);//使能speaker输出
        }
        
        if(sound_func != NULL)
        {
            sound_start_func __start_sound = (sound_start_func)sound_func;
            __start_sound(get_current_volume());
        }
    }
        
    return TRUE;
}

extern comval_t *sys_comval;

/******************************************************************************/
/*!                     
 * \par  Description:
 *    设置录音使能与否。
 * \param[in]    on_off 进入录音或退出录音，TRUE表示进入，FALSE表示退出
 * \param[in]    type 录音类型，0表示普通录音，1表示 fm 录音
 * \param[out]   none    
 * \return       bool
 * \retval           TRUE 设置录音成功
 * \retval           FALSE 设置录音失败 
 * \note 
*******************************************************************************/
bool com_set_sound_record(bool on_off, uint8 type)
{
    if(on_off == TRUE)
    {
        if(type == 0)
        {
            if(get_keytone_enable() == TRUE)
            {
                com_set_sound_keytone(FALSE);
                g_keytone_backup = TRUE;
            }
            else
            {
                g_keytone_backup = FALSE;
            }
        }
    }
    else
    {
        if(type == 0)
        {
            if(g_keytone_backup == TRUE)
            {
                com_set_sound_keytone(TRUE);
                g_keytone_backup = FALSE;
            }
        }
    }
    
    return TRUE;
}

/*! \endcond */
