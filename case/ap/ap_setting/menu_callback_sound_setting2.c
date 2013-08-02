/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_sound_setting.C
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_setting.h"
#include "menu_callback_sound_setting.h"

//一级菜单-声音设置
//声音设置|-fullsound|-关
//        |          |-开
//        |-SRS设置|-关
//        |        |-SRS WOWHD
//        |        |-自定义
//        |-音量限制
//        |-按键音
/*此表必须与music安排完全一致，由music ap决定，并且必须连续且完整。*/
//static const uint16 eq_text_table[EQ_TOTAL] =
//{ S_OFF_3, S_EQ_ROCK, S_EQ_FUNK, S_EQ_HIP_HOP, S_EQ_JAZZ, S_EQ_CLASSICAL, S_EQ_TECHNO, S_EQ_CUSTOM };

static const int8 eq_param_1[8][MAX_EQ_SEG] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //none
    { 5, 3, 0, -1, 0, 4, 5, 6, 0, 0, 0, 0 }, //rock
    { 3, 0, 0, 0, 0, -1, -2, 6, 0, 0, 0, 0 }, //funk--pop
    { 0, 0, 1, 2, 0, 1, 1, 6, 0, 0, 0, 0 }, //hiphop--soft
    { 3, 0, 0, -1, 0, 2, 4, 6, 0, 0, 0, 0 }, //Jazz
    { 0, 0, 0, -1, -1, -2, -4, 6, 0, 0, 0, 0 }, //Classic
    { 5, 2, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }, //techno--dbb
    { -9, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 } //custom--speaker
};

static const int8 fullsound_param[MAX_EQ_SEG] =
{ -9, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }; //fullsound on


static const int8 user_eq_param_selected_table[5] =
{ 1, 3, 5, 7, 9 }; //从12个参数中选取其中的5个作为自定义参数调节项

static const uint16 user_eq_uint_string_id[5] =
{ S_USER_B, S_USER_L, S_USER_M, S_USER_H, S_USER_T }; //5个作为自定义参数调节项提示语


static const int8 user_srs_param_selected_table[5] =
{ 6, 8, 9, 10, 11 }; //从12个参数中选取其中的5个作为自定义参数调节项

static const uint16 user_srs_uint_string_id[5] =
{ S_USER_B, S_USER_L, S_USER_M, S_USER_H, S_USER_T }; //5个作为自定义参数调节项提示语

//恢复默认设置eq标志
uint8 g_eq_restore;
//参数框全局变量
param_com_data_t s_param_com;
parambox_private_t s_param_private;
parambox_one_t s_param_item[5];

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_eq_1(mmm_mp_eq_info_t *eq_val)
 * \设置eq信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      menu_callback_sound_setting2.c
 * \note
 */
/*******************************************************************************/
static bool music_set_eq_1(mmm_mp_eq_info_t *eq_val)
{
    bool bret = TRUE;
    msg_apps_t msg;
    /*设置eq*/
    msg.content.addr = eq_val;
    /*消息类型(即消息名称)*/
    msg.type = MSG_MENGINE_SET_EQ_SYNC;
    /*发送同步消息*/
    bret = send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
    return bret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \eq回调函数
 * \param[in]    sound_param  参数值
 * \param[in]    eq_type      eq类型
 * \param[in]    end_index    有效参数个数
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 fail
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/************************** ************************************************ *****/
static bool eq_callback(parambox_one_t *sound_param, uint8 eq_type, uint8 end_index)
{
    uint8 i;
    //设置eq模式
    mmm_mp_eq_info_t eq_info;

    if (eq_type == EQ_TYPE_USR_MODE)
    {
        for (i = 0; i < end_index; i++)
        {
            eq_info.eq_para[i] = (char) (sound_param[i].value + 5);
        }

        for (i = end_index; i < 12; i++)
        {
            eq_info.eq_para[i] = (char) (g_setting_vars.music_comval.srs_parameter[i]);
        }
    }
    else if (eq_type == EQ_EQ_USR_MODE)
    {
        for (i = 0; i < end_index; i++)
        {
            eq_info.eq_para[i] = (char) (sound_param[i].value);
        }

        for (i = end_index; i < 12; i++)
        {
            eq_info.eq_para[i] = (char) (g_setting_vars.music_comval.eq_parameter[i]);
        }
    }
    else
    {
        ;//nothing
    }

    eq_info.eq_type = eq_type;

    music_set_eq_1(&eq_info);

    return TRUE;
}
/******************************************************************************/
/*!
 * \par  Description:
 * \SRS eq回调函数
 * \param[in]    active
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 fail
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/************************** ************************************************ *****/
static bool set_scene_srs_user_callback(int16 active)
{
    return eq_callback(s_param_item, EQ_TYPE_USR_MODE, 5);
}
/******************************************************************************/
/*!
 * \par  Description:
 * \用户 eq回调函数
 * \param[in]    active
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 fail
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/************************** ************************************************ *****/

static bool set_scene_eq_user_callback(int16 active)
{
    return eq_callback(s_param_item, EQ_EQ_USR_MODE, 5);
}

/******************************************************************************/
/*!
 * \par  Description:
 * \bool set_scene_speed_callback(uint16 speed_value)
 * \设置播放速度回调函数
 * \param[in]    speed_value播放速度值  para1
 * \param[out]   none
 * \return       bool the result
 * \retval       1 success
 * \retval       0 fail
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/************************** ************************************************ *****/
bool set_scene_speed_callback(int16 speed_value)
{
    //设置播放速度
    mmm_mp_eq_info_t eq_info;
    eq_info.eq_type = 0x77;
    eq_info.eq_para[0] = (char) speed_value;
    //g_setting_comval.music_comval.variable_playback = (int8) speed_value;
    music_set_eq_1(&eq_info);
    return TRUE;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置自定义音效
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_eq_custom_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;
    uint8 i;
    app_result_e result;

    //初始化参数
    for (i = 0; i < 5; i++)
    {
        s_param_item[i].min = -5;
        s_param_item[i].max = 5;
        s_param_item[i].step = 1;
        s_param_item[i].cycle = FALSE;
        s_param_item[i].adjust_func = NULL;
        s_param_item[i].callback = set_scene_eq_user_callback;
        s_param_item[i].max_number = 0;//不显示数字
        s_param_item[i].value_str = NULL;
        s_param_item[i].value = g_setting_vars.music_comval.eq_parameter[user_eq_param_selected_table[i]];
        s_param_item[i].unit_id = user_eq_uint_string_id[i];
    }

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 5;
    s_param_private.items = (parambox_one_t *) s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;
    result = gui_set_parameter(SLIDER_SETEQ, &s_param_com); //调节自定义EQ参数
    if (result == RESULT_CONFIRM)//确定设置参数返回
    {
        if ((check_close_fullsound() == FALSE) || (check_close_srs() == FALSE))
        {
            return (app_result_e) RESULT_MENU_PARENT; //返回上一级
        }

        //设置用户EQ
        g_setting_vars.music_comval.variable_playback = 0;
        g_setting_vars.music_comval.eq_setting = VALUE_EQ_CUSTOM;
        for (i = 0; i < 5; i++)
        {
            g_setting_vars.music_comval.eq_parameter[i] = (int8) s_param_com.private_data->items[i].value;
        }//保存参数


        //eq_info.eq_type = COMMAND_EQ;
        //libc_memcpy(eq_info.eq_para, g_setting_vars.music_comval.eq_parameter, MAX_EQ_SEG);
        //music_set_eq(&eq_info);
        return (app_result_e) RESULT_MENU_PARENT; //返回上一级
    }
    else
    {
        return result;
    }

}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置自定义SRS音效
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_user_srs_on_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;
    uint8 i;
    app_result_e result;

    g_setting_vars.music_comval.variable_playback = 0;

    g_setting_vars.music_comval.fullsound = VALUE_FULLSOUND_OFF;
    g_setting_vars.music_comval.eq_setting = VALUE_EQ_OFF;
    g_setting_vars.music_comval.srs = VALUE_USER_SRS_ON;

    for (i = 0; i < 5; i++)
    {
        s_param_item[i].min = -5;
        s_param_item[i].max = 5;
        s_param_item[i].step = 1;
        s_param_item[i].cycle = FALSE;
        s_param_item[i].adjust_func = NULL;
        s_param_item[i].callback = set_scene_srs_user_callback;
        s_param_item[i].max_number = 0;//不显示数字
        s_param_item[i].value_str = NULL;
        s_param_item[i].value = g_setting_vars.music_comval.srs_parameter[user_srs_param_selected_table[i]];
        s_param_item[i].unit_id = user_srs_uint_string_id[i];
    }

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 5;
    s_param_private.items = (parambox_one_t *) s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;
    result = gui_set_parameter(SLIDER_SETEQ, &s_param_com);
    if (result == RESULT_CONFIRM)//确定设置参数返回
    {
        if (check_close_fullsound() == FALSE)
        {
            return (app_result_e) RESULT_MENU_PARENT; //返回上一级
        }

        //libc_memcpy(g_setting_vars.music_comval.srs_parameter,srs_param_default, MAX_EQ_SEG);

        for (i = 0; i < 5; i++)
        {
            g_setting_vars.music_comval.srs_parameter[i] = (int8) s_param_com.private_data->items[i].value;
        }//保存参数

        //eq_info.eq_type = COMMAND_SRS;
        //libc_memcpy(eq_info.eq_para, g_setting_vars.music_comval.srs_parameter, MAX_EQ_SEG);
        //music_set_eq_1(&eq_info);
        return (app_result_e) RESULT_MENU_PARENT; //返回上一级
    }
    else
    {
        return result;
    }
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置音量限制
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_volume_limit_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    app_result_e result;

    result = gui_volumebar(&g_setting_vars.g_comval.volume_current, &g_setting_vars.g_comval.volume_limit,
            SET_VOLUME_LIMIT);
    if (result == RESULT_CONFIRM)
    {
        string_desc.data.id = S_SET_VOLUME_END;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
        return (app_result_e) RESULT_REDRAW; //返回上一级
    }
    else
    {
        return result;
    }

}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得按键音设置状况
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：进入下一级菜单
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_key_tone_setting_callback(void *title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_key_tone_setting;
    menu_title_key_tone_setting = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.key_tone == VALUE_KEY_TONE_ON)
    {
        active_id = S_ON;
    }
    else
    {
        active_id = S_OFF;
    }
    menu_title_key_tone_setting->menu_entry = -1;
    menu_title_key_tone_setting->str_id = active_id;
    menu_title_key_tone_setting->ret_str = NULL;
    menu_title_key_tone_setting->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于使能按键音
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e enable_key_tone_setting_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.key_tone = VALUE_KEY_TONE_ON;

    com_set_sound_keytone(VALUE_KEY_TONE_ON);//使能按键音

    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于关闭按键音
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e disable_key_tone_setting_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.key_tone = VALUE_KEY_TONE_OFF;

    com_set_sound_keytone(VALUE_KEY_TONE_OFF);//禁止按键音

    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

