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

extern void update_progress(uint16 freq, uint8 mode)
__FAR__;
extern app_result_e scene_autoseek_sysmsg(void)
__FAR__;

/* radio 全自动搜台按键事件映射*/
const key_map_t softseek_key_map_list[] =
{
    /*! 短按KEY_VOL 按键转换为 返回事件 */
    {{ KEY_VOL, 0, KEY_TYPE_SHORT_UP }, EVENT_RADIO_RETURN },
    /*! 最后一个映射事件 */
    {{ KEY_NULL, 0, KEY_TYPE_NULL }, MSG_NULL },
};

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_soft_autoseek(FM_SeekDir_e direction)
 * \软件全自动搜台处理
 * \param[in]    direction:   搜台方向
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   一次设置一个频点进行tune，并判断是否真台
 */
/*******************************************************************************/
app_result_e deal_soft_autoseek(FM_SeekDir_e dir)
{
    app_result_e ret = RESULT_NULL;
    uint8 space, tab_num;
    bool is_true = FALSE;
    bool result;
    uint8 dir_seek;

    //获取gui消息
    input_gui_msg_t gui_msg;
    //gui 消息对应gui 事件
    msg_apps_type_e gui_event;

    //搜台步进
    space = 100;
    if (g_menu_band == Band_Europe)
    {
        //欧洲波段, 步进调整为50K
        space = 50;
    }

    if (dir == DIR_DOWN)
    {
        dir_seek = 0;
    }
    else
    {
        dir_seek = 1;
    }

    tab_num = 0;
    while (1)
    {
        //判断当前频率是否有效台
        is_true = radio_set_softseek(g_radio_config.FMStatus.freq, dir_seek);
        if (is_true == TRUE)
        {
            //过滤96MHZ 及 108MHZ 频点
            if ((g_radio_config.FMStatus.freq != 0x7700) && (g_radio_config.FMStatus.freq != 0xa5e0))
            {
                //是有效电台，保存
                Auto_tab[tab_num] = g_radio_config.FMStatus.freq;
                is_true = FALSE;
                tab_num++;
            }
        }

        //电台列表已满
        if (tab_num >= MAX_STATION_COUNT)
        {
            update_progress(g_radio_config.FMStatus.freq, 1);
            ret = RESULT_RADIO_STATION_LIST;
            break;
        }

        if (dir == DIR_UP)
        {
            if (((g_menu_band == Band_China_US) || (g_menu_band == Band_Europe)) && ((g_radio_config.FMStatus.freq
                    + space) > MAX_US)) // 108MHz
            {
                update_progress(g_radio_config.FMStatus.freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }
            else if ((g_menu_band == Band_Japan) && ((g_radio_config.FMStatus.freq + space) > MAX_JP)) // 90MHz
            {
                update_progress(g_radio_config.FMStatus.freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }
            else
            {
                //更新频点
                g_radio_config.FMStatus.freq += space;
            }
        }
        else //if (dir == DIR_DOWN)
        {
            if (((g_menu_band == Band_China_US) || (g_menu_band == Band_Europe)) && ((g_radio_config.FMStatus.freq
                    - space) < MIN_US)) // 87.5MHz
            {
                update_progress(g_radio_config.FMStatus.freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }
            else if ((g_menu_band == Band_Japan) && ((g_radio_config.FMStatus.freq - space) < MIN_JP)) // 76MHz
            {
                update_progress(g_radio_config.FMStatus.freq, 1);
                ret = RESULT_RADIO_STATION_LIST;
                break;
            }
            else
            {
                //更新频点
                g_radio_config.FMStatus.freq -= space;
            }
        }
        //else
        //{
        //    ret = RESULT_RADIO_STATION_LIST;
        //    break;
        //}

        //根据当前频点，更新进度条
        update_progress(g_radio_config.FMStatus.freq, 0);

        //处理系统消息和gui 消息
        result = get_gui_msg(&gui_msg);

        if ((result == TRUE))
        {

            if (com_key_mapping(&gui_msg, &gui_event, softseek_key_map_list) == TRUE)
            {
                if (gui_event == EVENT_RADIO_RETURN)
                {
                    //return 停止搜台,返回电台列表
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

