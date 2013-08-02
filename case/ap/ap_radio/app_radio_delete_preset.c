/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-23          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

/* 字串最大长度定义宏*/
#define  MAX_STRING   50

/* 字串处理buffer  */
uint8 string_buf_1[MAX_STRING] _BANK_DATA_ATTR_;

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e  string_joint_del(uint8 station_num, uint8 mode)
 * \param[in]    station_num  电台号  mode   询问还是成功后提示
 * \param[out]   none
 * \return      none
 * \retval
 * \retval
 * \note   删除电台时字符串拼接处理
 */
/*************************************************************************************************/
app_result_e string_joint_del(uint8 station_num, uint8 mode)
{
    string_desc_t string_desc;
    string_desc_t string_source;
    uint8 tmp_buf[2];
    //uint8 tmp;
    dialog_com_data_t dialog_com;
    app_result_e ret;
    uint16 style_id;

    libc_memset(tmp_buf, 0, sizeof(tmp_buf));
    libc_memset(string_buf_1, 0, sizeof(string_buf_1));

    //电台号转成ascill
    libc_itoa(station_num, &tmp_buf[0], 2);

#if 0    //itoa 已经做了转换
    if(station_num <10)
    {
        //只有一位时，转换后需交换顺序
        tmp = tmp_buf[0];
        tmp_buf[0] = tmp_buf[1];
        tmp_buf[1] = tmp;
    }
#endif

    //源字串--- 电台号填充
    string_source.data.str = tmp_buf;
    string_source.length = 3;
    string_source.language = ANSIDATAAUTO;

    //目标字串
    string_desc.data.str = string_buf_1;
    string_desc.length = MAX_STRING;
    if (mode == 0)
    {
        //是否清除
        string_desc.argv = CON_CLEAR;
    }
    else
    {
        //清除成功
        string_desc.argv = SUC_CLEAR;
    }
    string_desc.language = (int8) g_comval.language_id;

    com_string_format(&string_desc, &string_source);

    //询问，含有按键的dialog
    if (mode == 0)
    {
        dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
        dialog_com.button_type = BUTTON_YESNO;
        dialog_com.active_default = BUTTON_YESNO_NO;
        style_id = DIALOG_ASK;
    }
    //成功后提示，不含按键
    else
    {
        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        style_id = DIALOG_MSG;
    }
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    ret = gui_dialog(style_id, &dialog_com);
    return ret;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \bool search_first_freq(uint16 *p_freq)
 * \param[in]    none
 * \param[out]   find freq
 * \return    success/fail
 * \retval
 * \retval
 * \note   在当前电台列表中找到第一个保存的有效频点
 */
/*************************************************************************************************/
bool search_first_freq(uint16 *p_freq)
{
    uint16 *ptr;
    uint8 i;
    uint16 freq;

    ptr = pfreq;
    for (i = 0; i < MAX_STATION_COUNT; i++)
    {
        freq = *ptr;
        if (freq != 0)
        {
            *p_freq = freq;
            //更新当前播放的电台号
            g_radio_config.FMStatus.station = i + 1;
            return TRUE;
        }
        ptr++;
    }
    return FALSE;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e do_del_process(uint8 num)
 * \param[in]    num  电台号
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   电台删除处理，将指定电台从菜单波段的列表中删除
 * \         删除后回到当前频点播放
 */
/*************************************************************************************************/
app_result_e do_del_process(uint8 num)
{
    //删除后回到播放场景
    app_result_e result = RESULT_RADIO_START_PLAY;
    bool ret;

    if ((num > MAX_STATION_COUNT) || (num == 0))
    {
        return result;
    }
    if (g_menu_band == Band_Japan)
    {
        g_radio_config.fmstation_jp[num - 1] = 0;
    }
    else if (g_menu_band == Band_Europe)
    {
        g_radio_config.fmstation_eu[num - 1] = 0;
    }
    else
    {
        g_radio_config.fmstation_us[num - 1] = 0;
    }

    //如果是用户模式，切换到预设模式
    g_radio_config.band_mode = g_menu_band;
    //更新列表指针
    init_point_freq();
    //更新频率到列表第一个已保存频点播放
    ret = search_first_freq(&g_radio_config.FMStatus.freq);

    if (!ret)
    {
        //没有已保存频点了，到最小频率播放
        g_radio_config.FMStatus.freq = min_freq[g_radio_config.band_mode];
        g_radio_config.FMStatus.station = 0;
    }

    //更新保存电台总数
    g_radio_config.FMStatus.station_count = get_station_count();

    //删除成功提示
    string_joint_del(num, 1);
    return result;
}

/************************************************************************************************/
/*!
 * \par  Description:
 * \app_result_e deal_station_del(radiolist_control_t *p_ml_control)
 * \param[in]    p_ml_control
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \note   清除单个预设电台处理
 */
/*************************************************************************************************/
app_result_e deal_station_del(radiolist_control_t *p_ml_control)
{
    app_result_e result = RESULT_NULL;
    uint16 *ptr;
    uint8 station;
    uint16 list_freq;

    //指向所浏览的列表首，因为后台播放和浏览可能不是同一列表
    ptr = pfreq_list;
    //电台号
    station = p_ml_control->list_no + 1;
    //电台列表中频率值
    list_freq = *(ptr + station - 1);

    if (list_freq == 0)
    {
        //所选择的电台号没有保存电台，不响应
        return result;
    }

    //询问
    result = string_joint_del(station, 0);

    if (result == RESULT_DIALOG_YES)
    {
        //确认删除
        result = do_del_process(station);
    }
#if 0
    else
    {
        //取消删除，重绘列表
        result = RESULT_REDRAW;
    }
#endif
    return result;

}

