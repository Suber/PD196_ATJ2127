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

//EQ参数类型自定义
typedef enum
{
    EQ_PARAM_NONE = 0x0,
    EQ_PARAM_ROCK,
    EQ_PARAM_FUNK,
    EQ_PARAM_HIPHOP,
    EQ_PARAM_JAZZ,
    EQ_PARAM_CALSSIC,
    EQ_PARAM_TECHNO,
    EQ_PARAM_CUSTOM,
    EQ_PARAM_FULLSOUND,
    EQ_PARAM_USEREQ,
    EQ_PARAM_USERSRS,
    EQ_PARAM_WOWSRS
}eq_param_e;


/*此表必须与music安排完全一致，由music ap决定，并且必须连续且完整。*/
static const uint16 eq_text_table[EQ_TOTAL] =
{ S_OFF, S_EQ_ROCK, S_EQ_FUNK, S_EQ_HIP_HOP, S_EQ_JAZZ, S_EQ_CLASSICAL, S_EQ_TECHNO, S_EQ_CUSTOM };

static const int8 eq_param[8][MAX_EQ_SEG] =
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

static const int8 srs_param_default[MAX_EQ_SEG] =
{ 8, 8, 5, 4, 6, 1, 0, 30, 1, 0, 0, 0 };//srs default


//一级菜单-声音设置
//声音设置
//        |-均衡器|-关
//        |       |-摇滚
//        |       |-放克
//        |       |-希普霍普
//        |       |-爵士
//        |       |-古典
//        |       |-电子乐
//        |       |-自定义
////////////////////

/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_eq(mmm_mp_eq_info_t *eq_val)
 * \设置eq信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      music_control.c
 * \note
 */
/*******************************************************************************/
static bool music_set_eq(mmm_mp_eq_info_t *eq_val)
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
 设置实际的EQ到音量回调函数
 * \param[in]    eq_type  EQ类型，参见mmm_mp_eqmode_t
 * \param[in]    eq_param_index EQ索引，参见eq_param_e
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
static app_result_e _set_real_eq(uint8 eq_type, uint8 eq_param_index)
{
    //设置eq模式
    mmm_mp_eq_info_t eq_info;

    eq_info.eq_type = eq_type;

    if (eq_type == EQ_TYPE_USR_MODE)
    {
        if (eq_param_index == EQ_PARAM_USERSRS)
        {
            libc_memcpy(eq_info.eq_para, g_setting_vars.music_comval.srs_parameter, 12);
        }
        else
        {
            libc_memcpy(eq_info.eq_para, srs_param_default, sizeof(srs_param_default));
        }
    }
    else if (eq_param_index == EQ_PARAM_USEREQ)
    {
        libc_memcpy(eq_info.eq_para, g_setting_vars.music_comval.eq_parameter, 12);
    }
    else
    {
        libc_memcpy(eq_info.eq_para, eq_param[eq_param_index], sizeof(eq_param[eq_param_index]));
    }
    music_set_eq(&eq_info);

    return RESULT_NULL;
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为NORMAL及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_normal_callback(void)
{
    _set_real_eq(EQ_NORMAL, EQ_PARAM_NONE);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为fullsound及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_fullsoundon_callback(void)
{
    _set_real_eq(EQ_FULLSOUND_MODE, EQ_PARAM_FULLSOUND);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为rock及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_rock_callback(void)
{
    _set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_ROCK);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为funk及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_funk_callback(void)
{
    _set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_FUNK);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为hiphop及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_hiphop_callback(void)
{
    _set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_HIPHOP);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为jazz及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_jazz_callback(void)
{
    _set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_JAZZ);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为classical及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_classical_callback(void)
{
    _set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_CALSSIC);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为techno及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_techno_callback(void)
{
    _set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_TECHNO);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为equser及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_custom_callback(void)
{
    _set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_USEREQ);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为srswowhd及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_eq_wowhd_callback(void)
{
    _set_real_eq(EQ_TYPE_USR_MODE, EQ_PARAM_WOWSRS);
}

/******************************************************************************/
/*!
 * \par  Description:
 设置EQ为srsuser及时回调函数
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_NULL
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
void set_user_srs_on_callback(void)
{
    _set_real_eq(EQ_TYPE_USR_MODE, EQ_PARAM_USERSRS);
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本函数用于决断fullsound是否关闭
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回fullsound关闭状况，TRUE 表示关闭，FALSE 表示不关闭
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
bool check_close_fullsound(void)
{
    bool flag;
    app_result_e result;
    dialog_com_data_t dialog_com;
    string_desc_t strings;
    if (g_setting_vars.music_comval.fullsound == VALUE_FULLSOUND_ON)
    {
        dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
        dialog_com.button_type = BUTTON_YESNO;

        strings.data.id = S_CLOSE_FULLSOUND_OR_NOT;
        strings.language = UNICODEID;

        dialog_com.string_desc = &strings;
        dialog_com.active_default = VALUE_BUTTON_YESNO_NO;
        need_quit_ap_flag = 0;
        result = gui_dialog(DIALOG_ASK, &dialog_com);
        if (result == RESULT_APP_QUIT) //特别处理，避免控件的退出消息未能处理
        {
            need_quit_ap_flag = 1;
        }
        if (result == RESULT_DIALOG_YES)
        {
            g_setting_vars.music_comval.fullsound = VALUE_FULLSOUND_OFF;
            flag = TRUE;
        }
        else
        {
            flag = FALSE;
        }
    }
    else
    {
        flag = TRUE;
    }
    return flag;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本函数用于决断srs是否关闭
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回srs关闭状况，TRUE 表示关闭，FALSE 表示不关闭
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
bool check_close_srs(void)
{
    bool flag;
    app_result_e result;
    dialog_com_data_t dialog_com;
    string_desc_t strings;
    if (g_setting_vars.music_comval.srs != VALUE_SRS_OFF)
    {
        dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
        dialog_com.button_type = BUTTON_YESNO;

        strings.data.id = S_CLOSE_SRS_OR_NOT;
        strings.language = UNICODEID;

        dialog_com.string_desc = &strings;

        dialog_com.active_default = VALUE_BUTTON_YESNO_NO;
        need_quit_ap_flag = 0;
        result = gui_dialog(DIALOG_ASK, &dialog_com);
        if (result == RESULT_APP_QUIT) //特别处理，避免控件的退出消息未能处理
        {
            need_quit_ap_flag = 1;
        }
        if (result == RESULT_DIALOG_YES)
        {
            g_setting_vars.music_comval.srs = VALUE_SRS_OFF;
            flag = TRUE;
        }
        else
        {
            flag = FALSE;
        }
    }
    else
    {
        flag = TRUE;
    }
    return flag;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _set_scene_seteq(uint8 eq_type)
 * \设置eq
 * \param[in]    eq_type  para1
 * \param[out]   none
 * \return       void the result
 * \retval       void
 * \retval       none
 * \ingroup      music_scene_playing.c
 * \note
 */
/*******************************************************************************/
void _set_scene_seteq(uint8 eq_type)
{

    if (g_setting_vars.music_comval.srs != VALUE_SRS_OFF)
    {
        //srs设置
        if (g_setting_vars.music_comval.srs == VALUE_WOWHD_ON)
        {
            set_eq_wowhd_callback();
        }
        else
        {
            //调用控件
            set_user_srs_on_callback();
        }
    }
    else if (g_setting_vars.music_comval.fullsound != VALUE_FULLSOUND_OFF)
    {
        //fullsound设置
        set_eq_fullsoundon_callback();

    }
    else if (g_setting_vars.music_comval.variable_playback != 0)
    {
        //播放速度设置
        set_scene_speed_callback((int16) g_setting_vars.music_comval.variable_playback);
    }
    else
    {
        //eq 设置
        switch (eq_type)
        {
            //eq rock
            case VALUE_EQ_ROCK:
            set_eq_rock_callback();
            break;
            //ea funk
            case VALUE_EQ_FUNK:
            set_eq_funk_callback();
            break;
            //eq hip hop
            case VALUE_EQ_HIP_HOP:
            set_eq_hiphop_callback();
            break;
            //eq jazz
            case VALUE_EQ_JAZZ:
            set_eq_jazz_callback();
            break;
            //eq classical
            case VALUE_EQ_CLASSICAL:
            set_eq_classical_callback();
            break;
            //eq teckno
            case VALUE_EQ_TECHNO:
            set_eq_techno_callback();
            break;
            //eq custom
            case VALUE_EQ_CUSTOM:
            set_eq_custom_callback();
            break;

            default:
            set_eq_normal_callback();
            break;
        }
    }
}

/******************************************************************************/
/*!
 * \par  Description:
 从叶子菜单返回时根据当前设置的EQ音量重设EQ
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool
 * \ingroup      music_setmenu_soundset.c
 * \note
 * \li  由于叶子菜单支持实时参数回调，因此可能改变实际的EQ，返回父菜单时调用该函数
 恢复或重设当前EQ
 */
/*******************************************************************************/
static void restore_music_eq(void)
{
    _set_scene_seteq(g_setting_vars.music_comval.eq_setting);
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得当前均衡器选择
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回字符串ID号，以便在下级菜单取得激活项
 * \retval       ：进入下一级菜单
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_eq_option_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_eq_option;
    menu_title_eq_option = (menu_title_action_t*) title_action;
    active_id = eq_text_table[g_setting_vars.music_comval.eq_setting];
    menu_title_eq_option->str_id = active_id; //指定active项
    //设置返回当前根菜单的回调函数
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得当前fullsound选择
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回字符串ID号，以便在下级菜单取得激活项
 * \retval       ：进入下一级菜单
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_fullsound_option_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_fullsound_option;
    menu_title_fullsound_option = (menu_title_action_t*) title_action;
    if (g_setting_vars.music_comval.fullsound == VALUE_FULLSOUND_OFF)
    {
        active_id = S_OFF;
    }
    else
    {
        active_id = S_ON;
    }
    menu_title_fullsound_option->str_id = active_id; //指定active项
    //设置返回当前根菜单的回调函数
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得当前srs选择
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回字符串ID号，以便在下级菜单取得激活项
 * \retval       ：进入下一级菜单
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_srs_option_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_srs_option;
    menu_title_srs_option = (menu_title_action_t*) title_action;
    if (g_setting_vars.music_comval.srs == VALUE_SRS_OFF)
    {
        active_id = S_OFF;
    }
    else if (g_setting_vars.music_comval.srs == VALUE_USER_SRS_ON)
    {
        active_id = S_USER_SRS;
    }
    else
    {
        active_id = S_WOWHD;
    }
    menu_title_srs_option->str_id = active_id; //指定active项
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于关闭均衡器
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_eq_normal_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;
    g_setting_vars.music_comval.eq_setting = VALUE_EQ_OFF;
    //eq_info.eq_type = COMMAND_EQ_NORMAL;
    //libc_memcpy(eq_info.eq_para, eq_param[VALUE_EQ_OFF], MAX_EQ_SEG);
    //music_set_eq(&eq_info);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置摇滚音效
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_eq_rock_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;

    g_setting_vars.music_comval.variable_playback = 0;

    if ((check_close_fullsound() == TRUE) && (check_close_srs() == TRUE))
    {
        g_setting_vars.music_comval.eq_setting = VALUE_EQ_ROCK;
        //eq_info.eq_type = COMMAND_EQ;
        //libc_memcpy(eq_info.eq_para, eq_param[g_setting_vars.music_comval.eq_setting], MAX_EQ_SEG);
        //music_set_eq(&eq_info);
    }
    if (need_quit_ap_flag == 1)
    {
        return RESULT_APP_QUIT;
    }
    else
    {
        return (app_result_e) RESULT_MENU_PARENT; //返回上一级
    }
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置funk音效
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_eq_funk_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;

    g_setting_vars.music_comval.variable_playback = 0;

    if ((check_close_fullsound() == TRUE) && (check_close_srs() == TRUE))
    {
        g_setting_vars.music_comval.eq_setting = VALUE_EQ_FUNK;
        //eq_info.eq_type = COMMAND_EQ;
        //libc_memcpy(eq_info.eq_para, eq_param[g_setting_vars.music_comval.eq_setting], MAX_EQ_SEG);
        //music_set_eq(&eq_info);
    }
    if (need_quit_ap_flag == 1)
    {
        return RESULT_APP_QUIT;
    }
    else
    {
        return (app_result_e) RESULT_MENU_PARENT; //返回上一级
    }
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置hip-hop音效
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_eq_hip_hop_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;

    g_setting_vars.music_comval.variable_playback = 0;

    if ((check_close_fullsound() == TRUE) && (check_close_srs() == TRUE))
    {
        g_setting_vars.music_comval.eq_setting = VALUE_EQ_HIP_HOP;
        //eq_info.eq_type = COMMAND_EQ;
        //libc_memcpy(eq_info.eq_para, eq_param[g_setting_vars.music_comval.eq_setting], MAX_EQ_SEG);
        //music_set_eq(&eq_info);
    }
    if (need_quit_ap_flag == 1)
    {
        return RESULT_APP_QUIT;
    }
    else
    {
        return (app_result_e) RESULT_MENU_PARENT; //返回上一级
    }
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置爵士音效
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_eq_jazz_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;

    g_setting_vars.music_comval.variable_playback = 0;

    if ((check_close_fullsound() == TRUE) && (check_close_srs() == TRUE))
    {
        g_setting_vars.music_comval.eq_setting = VALUE_EQ_JAZZ;
        //eq_info.eq_type = COMMAND_EQ;
        //libc_memcpy(eq_info.eq_para, eq_param[g_setting_vars.music_comval.eq_setting], MAX_EQ_SEG);
        //music_set_eq(&eq_info);
    }
    if (need_quit_ap_flag == 1)
    {
        return RESULT_APP_QUIT;
    }
    else
    {
        return (app_result_e) RESULT_MENU_PARENT; //返回上一级
    }
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置古典音效
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_eq_classical_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;

    g_setting_vars.music_comval.variable_playback = 0;

    if ((check_close_fullsound() == TRUE) && (check_close_srs() == TRUE))
    {
        g_setting_vars.music_comval.eq_setting = VALUE_EQ_CLASSICAL;
        //eq_info.eq_type = COMMAND_EQ;
        //libc_memcpy(eq_info.eq_para, eq_param[g_setting_vars.music_comval.eq_setting], MAX_EQ_SEG);
        //music_set_eq(&eq_info);
    }
    if (need_quit_ap_flag == 1)
    {
        return RESULT_APP_QUIT;
    }
    else
    {
        return (app_result_e) RESULT_MENU_PARENT; //返回上一级
    }
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置电子乐音效
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_eq_techno_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;

    g_setting_vars.music_comval.variable_playback = 0;

    if ((check_close_fullsound() == TRUE) && (check_close_srs() == TRUE))
    {
        g_setting_vars.music_comval.eq_setting = VALUE_EQ_TECHNO;
        //eq_info.eq_type = COMMAND_EQ;
        //libc_memcpy(eq_info.eq_para, eq_param[g_setting_vars.music_comval.eq_setting], MAX_EQ_SEG);
        //music_set_eq(&eq_info);
    }
    if (need_quit_ap_flag == 1)
    {
        return RESULT_APP_QUIT;
    }
    else
    {
        return (app_result_e) RESULT_MENU_PARENT; //返回上一级
    }
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于fullsound 关闭
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e fullsound_off_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = COMMAND_EQ_NORMAL;
    //libc_memcpy(eq_info.eq_para, eq_param_1[VALUE_EQ_OFF], MAX_EQ_SEG);
    //music_set_eq_1(&eq_info);
    g_setting_vars.music_comval.fullsound = VALUE_FULLSOUND_OFF;
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于fullsound 使能
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e fullsound_on_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;

    g_setting_vars.music_comval.variable_playback = 0;

    g_setting_vars.music_comval.fullsound = VALUE_FULLSOUND_ON;
    g_setting_vars.music_comval.eq_setting = VALUE_EQ_OFF;
    g_setting_vars.music_comval.srs = VALUE_SRS_OFF;
    //eq_info.eq_type = COMMAND_FULLSOUND;
    //libc_memcpy(eq_info.eq_para, &fullsound_param, MAX_EQ_SEG);
    //music_set_eq_1(&eq_info);
    g_setting_vars.music_comval.fullsound = VALUE_FULLSOUND_ON;
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于关闭srs
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_srs_off_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = COMMAND_EQ_NORMAL;
    //libc_memcpy(eq_info.eq_para, eq_param_1[VALUE_EQ_OFF], MAX_EQ_SEG);
    //music_set_eq_1(&eq_info);
    g_setting_vars.music_comval.srs = VALUE_SRS_OFF;
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于使能wowhd
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回“返回上一级”消息
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_wowhd_on_sure(void *menu_title_void)
{
    //mmm_mp_eq_info_t eq_info;

    g_setting_vars.music_comval.variable_playback = 0;

    g_setting_vars.music_comval.fullsound = VALUE_FULLSOUND_OFF;
    g_setting_vars.music_comval.eq_setting = VALUE_EQ_OFF;
    g_setting_vars.music_comval.srs = VALUE_WOWHD_ON;
    //eq_info.eq_type = COMMAND_SRS;
    //libc_memcpy(eq_info.eq_para, &srs_param_default, MAX_EQ_SEG);
    //music_set_eq_1(&eq_info);
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

