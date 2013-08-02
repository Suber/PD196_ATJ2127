/*******************************************************************************
 *                              US211A
 *                            Module: radio engine
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>        <time>           <version >            <desc>
 *     mikeyang     2011-09-06 15:00     1.0             build this file
 *******************************************************************************/
#include "app_fmengine.h"

const uint8 VolumeTbl[32]=
{40,40,39,39,38, 38, 37, 36, 35, 34,
 33, 32, 31, 30, 28, 27, 26, 25, 24, 23,
 22, 20,19, 17, 16, 14, 12,10, 8, 6, 4, 0
};


/******************************************************************************/
/*!
 * \par  Description:
 * \uint32 volume_deal(uint8 vol_in)
 * \音量转换
 * \param[in]    uint8 vol_in
 * \param[out]   none
 * \return       uint32 vol_out
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/
uint32 volume_deal(uint8 vol_in)
{
    uint32 volum_set;
    uint8 tab_index;

    tab_index = (31 - vol_in);
    volum_set = (VolumeTbl[tab_index]& 0x3f);

    return volum_set;

}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e fmengine_message_deal(private_msg_t* cur_msg)
 * \引擎消息处理,处理引擎接收到的私有消息和系统消息
 * \param[in]    private_msg_t* cur_msg
 * \param[out]   none
 * \return       int the result
 * \retval       RESULT_IGNOR 忽略
 * \retval       RESULT_APP_QUIT 退出app
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_message_deal(private_msg_t* cur_msg)
{
    app_result_e ret_value = RESULT_NULL;

    uint32 temp = 0;
    uint32 display_vol = 0;
    uint32 set_vol = 0;

    temp = temp;
    display_vol = display_vol;
    set_vol = set_vol;

    switch (cur_msg->msg.type)
    {
        /* 模组初始化及卸载相关*/

        /* 同步初始化FM  模组*/
        case MSG_FMENGINE_INIT:
        ret_value = fm_module_init(cur_msg);
        break;

        case MSG_FMENGINE_RESINTALL:
        ret_value = fm_module_reinstall(cur_msg);
        break;

        /* FM 模组进入Standby */
        case MSG_FMENGINE_STANDBY:
        ret_value = fm_module_standby();
        break;

        /* 要求引擎退出，返回后卸载驱动*/
        case MSG_FMENGINE_CLOSE:
        case MSG_APP_QUIT: //退出app
        /*其他进程也可向FM 引擎发MSG_FMENGINE_CLOSE  消息要求FM 引擎退出*/
        if (g_module_status != MODULE_STANDBY)
        {
            fm_module_standby();
        }
        ret_value = RESULT_APP_QUIT;
        break;

        /* 参数设置相关*/

        /* 同步设置频点播放*/
        case MSG_FMENGINE_SETFREQ_SYNC:
        ret_value = fmengine_set_freq_sync(cur_msg);
        break;

        /* 静音或解除静音*/
        case MSG_FMENGINE_SETMUTE:
        ret_value = fmengine_mute_sync(cur_msg);
        break;

        /* 设置电台波段*/
        case MSG_FMENGINE_SETBAND:
        ret_value = fmengine_set_band_sync(cur_msg);
        break;

        /* 设置搜台门限*/
        case MSG_FMENGINE_SETTHRED:
        ret_value = fmengine_set_throd(cur_msg);
        break;

        /* 启动软件搜台*/
        case MSG_FMENGINE_SET_SOFTSEEK:
        ret_value = fmengine_set_softseek(cur_msg);
        break;

        /* 启动硬件搜台*/
        case MSG_FMENGINE_SET_HARDSEEK:
        ret_value = fmengine_set_hardseek(cur_msg);
        break;

        /* 退出硬件seek 过程*/
        case MSG_FMENGINE_SET_SEEKBREAK:
        ret_value = fmengine_seek_break(cur_msg);
        break;

        /* 设置音量*/
        case MSG_FMENGINE_SET_VOLUME:

        display_vol = (uint32)(*(uint8*) (cur_msg->msg.content.addr));

        set_vol = volume_deal((uint8)display_vol);

#ifndef test
        set_pa_volume(set_vol);
#else
        /*PA volume level : 0 - 41*/
        temp = *((REG32) (DAC_ANALOG1)) & (~(DAC_ANALOG1_Volume_MASK));
        temp |= (set_vol) << DAC_ANALOG1_Volume_SHIFT) & DAC_ANALOG1_Volume_MASK;
        *((REG32) (DAC_ANALOG1)) = temp;
#endif
        cur_msg->reply->type = MSG_REPLY_SUCCESS;
        //回复同步消息(发送信号量)
        reply_sync_msg(cur_msg);
        ret_value = RESULT_NULL;
        break;

        /* 获取信息相关*/

        /* 获取硬件自动搜台是否完成标识*/
        case MSG_FMENGINE_AUTO_SEEK_OVER:
        ret_value = fmengine_get_seekflag(cur_msg);
        break;

        /* 获取当前状态信息*/
        case MSG_FMENGINE_GET_STATUS:
        ret_value = fmengine_get_status(cur_msg);
        break;

        /* 获取当前波段信息*/
        case MSG_FMENGINE_GET_BAND:
        ret_value = fmengine_get_band(cur_msg);
        break;

        /* 获取当前频率信息*/
        case MSG_FMENGINE_GET_FREQ:
        ret_value = fmengine_get_freq(cur_msg);
        break;

        /* 获取当前电台信号强度信息*/
        case MSG_FMENGINE_GET_INTENSITY:
        ret_value = fmengine_get_intensity(cur_msg);
        break;

        /* 获取天线状态( 耳机是否插入) */
        case MSG_FMENGINE_GET_ANTENNA:
        ret_value = fmengine_get_antenna(cur_msg);
        break;

        /* 获取立体声状态信息*/
        case MSG_FMENGINE_GET_STEREO:
        ret_value = fmengine_get_stereo(cur_msg);
        break;

        /* 获取当前引擎状态*/
        case MSG_FMENGINE_GET_PLAYSTATUS:
        ret_value = fmengine_get_playflag(cur_msg);
        break;

        /* 获取当前音量*/
        case MSG_FMENGINE_GET_VOLUME:
        ret_value = fmengine_get_volume(cur_msg);
        break;

        /* 获取当前模组状态*/
        case MSG_FMENGINE_GET_MODUAL_STATUS:
        ret_value = fmengine_get_modual_status(cur_msg);
        break;

        default:
        break;
    }

    return ret_value;
}

/******************************************************************************/
/*!
 * \Description: FM 引擎应用为被动模块，也是其他应用进程
 * \ 和FM 驱动通讯的接口模块
 * \app_result_e fmengine_control_block(void)
 * \引擎功能处理函数
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval       none
 * \note
 */
/*******************************************************************************/
app_result_e fmengine_control_block(void)
{
    app_result_e result = RESULT_NULL;
    private_msg_t fmengine_msg;

    while (1)
    {
        //获取并处理消息
        if (get_app_msg_for_engine(&fmengine_msg) == TRUE)
        {
            result = fmengine_message_deal(&fmengine_msg);
            if (result == RESULT_APP_QUIT)
            {
                break;
            }
        }
        //挂起10ms 多任务交互
        sys_os_time_dly(1);
    }
    return result;
}
