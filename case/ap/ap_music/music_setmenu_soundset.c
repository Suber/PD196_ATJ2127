/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-21          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

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

//const data
const int8 eq_param[9][12] =
{
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //none
    { 5, 3, 0, -1, 0, 4, 5, 6, 0, 0, 0, 0 }, //rock
    { 3, 0, 0, 0, 0, -1, -2, 6, 0, 0, 0, 0 }, //funk--pop
    { 0, 0, 1, 2, 0, 1, 1, 6, 0, 0, 0, 0 }, //hiphop--soft
    { 3, 0, 0, -1, 0, 2, 4, 6, 0, 0, 0, 0 }, //Jazz
    { 0, 0, 0, -1, -1, -2, -4, 6, 0, 0, 0, 0 }, //Classic
    { 5, 2, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }, //techno--dbb
    { -9, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }, //custom--speaker
    { 10, 4, 50, 0, 0, 0, 6, 0, 0, 0, 0, 0 } //fullsound on--default value

};

const int8 srs_wowhd_param[] =
{
    8, 8, 5, 4, 6, 1, 0, 30, 1, 0 //srs
};

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
    _scene_play_seteq(g_setting_comval.music_comval.eq_setting);
}

/******************************************************************************/
/*!
 * \par  Description:
 检查fullsound设置
 * \param[in]    ask_result  返回值参数
 * \param[out]   ask_result  返回值参数
 * \return       bool
 * \retval       TRUE 未打开or关闭
 * \retval       FALSE 打开且不关闭
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
bool check_fullsound_close(app_result_e* ask_result)
{
    bool check_ret = TRUE;
    app_result_e dialog_retval;
    dialog_com_data_t fullsound_ask_dialog;
    string_desc_t fullsound_str;
    if (g_setting_comval.music_comval.fullsound == VALUE_FULLSOUND_ON)
    {
        //对话框提示是否关闭
        fullsound_str.data.id = S_FULLSOUND_EQ_MUX;
        fullsound_str.language = UNICODEID;
        //对话框类型
        fullsound_ask_dialog.dialog_type = DIALOG_BUTTON_INCLUDE;
        fullsound_ask_dialog.icon_id = -1;
        //字符串
        fullsound_ask_dialog.string_desc = &fullsound_str;

        //按键类型
        fullsound_ask_dialog.button_type = BUTTON_YESNO;
        /*! 默认激活按键项 */
        fullsound_ask_dialog.active_default = 0x01;
        dialog_retval = gui_dialog(DIALOG_ASK, &fullsound_ask_dialog);
        if (dialog_retval != RESULT_DIALOG_YES)
        {
            check_ret = FALSE;
            *ask_result = dialog_retval;
        }
    }
    return check_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 fullsound根菜单执行函数，根据当前的EQ值进行激活项匹配
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fullsound_sure(void* param)
{
    //指向返回参数的指针
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //确认字符串id
    switch (g_setting_comval.music_comval.fullsound)
    {
        //fullsound off
        case VALUE_FULLSOUND_OFF:
        retptr->str_id = S_OFF;
        break;
        //fullsound on
        case VALUE_FULLSOUND_ON:
        retptr->str_id = S_ON;
        break;
        default:
        retptr->str_id = -1;
        break;
    }
    //设置返回当前根菜单的回调函数
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    //进入下一级菜单
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 EQ设置根菜单确定执行函数，根据当前的EQ值进行激活项匹配
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eq_sure(void* param)
{
    //指向返回参数的指针
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //确认字符串id
    switch (g_setting_comval.music_comval.eq_setting)
    {
        //eq off
        case VALUE_EQ_OFF:
        retptr->str_id = S_OFF;
        break;
        //eq rock
        case VALUE_EQ_ROCK:
        retptr->str_id = S_ROCK;
        break;
        //ea funk
        case VALUE_EQ_FUNK:
        retptr->str_id = S_FUNK;
        break;
        //eq hip hop
        case VALUE_EQ_HIP_HOP:
        retptr->str_id = S_HIPHOP;
        break;
        //eq jazz
        case VALUE_EQ_JAZZ:
        retptr->str_id = S_JAZZ;
        break;
        //eq classical
        case VALUE_EQ_CLASSICAL:
        retptr->str_id = S_CLASSIC;
        break;
        //eq teckno
        case VALUE_EQ_TECHNO:
        retptr->str_id = S_TECHNO;
        break;
        //eq custom
        case VALUE_EQ_CUSTOM:
        retptr->str_id = S_CUSTOM;
        break;

        default:
        retptr->str_id = -1;
        break;
    }

    //设置返回当前根菜单的回调函数
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    //进入下一级菜单
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 SRS设置根菜单确定执行函数，根据当前的EQ值进行激活项匹配
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval
 * \retval
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_srs_sure(void* param)
{
    //指向返回参数的指针
    menu_title_action_t* retptr = (menu_title_action_t*) param;

    //确认字符串id
    switch (g_setting_comval.music_comval.srs)
    {
        //srs off
        case VALUE_SRS_OFF:
        retptr->str_id = S_OFF;
        break;
        //srs wowhd
        case VALUE_WOWHD_ON:
        retptr->str_id = S_SRS_WOWHD;
        break;
        //srs user
        case VALUE_USER_SRS_ON:
        retptr->str_id = S_SRS_USER;
        break;
        default:
        retptr->str_id = -1;
        break;
    }
    //设置返回当前根菜单的回调函数
    g_eq_restore = 1;
    gui_menulist_set_back2parent(restore_music_eq, &g_eq_restore);
    //进入下一级菜单
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 fullsound关闭叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fullsoundoff_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = 0x00;
    //eq_info.eq_para = eq_param[0];
    //libc_memcpy(eq_info.eq_para, eq_param[0], sizeof(eq_param[0]));
    //music_set_eq(&eq_info);
    //foullsound关闭
    g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 fullsound开启叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_fullsoundon_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = 0x07;
    //eq_info.eq_para = eq_param[8];
    //libc_memcpy(eq_info.eq_para, eq_param[8], sizeof(eq_param[8]));
    //music_set_eq(&eq_info);
    //foullsound打开
    g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_ON;
    //eq关闭
    g_setting_comval.music_comval.eq_setting = VALUE_EQ_OFF;
    //srs关闭
    g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
    //speed关闭
    g_setting_comval.music_comval.variable_playback = 0;
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为normal模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqnormal_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = 0x00;
    //eq_info.eq_para = eq_param[0];
    //libc_memcpy(eq_info.eq_para, eq_param[0], sizeof(eq_param[0]));
    //music_set_eq(&eq_info);
    //eq关闭
    g_setting_comval.music_comval.eq_setting = VALUE_EQ_OFF;
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为rock模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqrock_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[1], sizeof(eq_param[1]));
        //music_set_eq(&eq_info);
        //eq rock
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_ROCK;
        //foullsound关闭
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs关闭
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed关闭
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //返回到上一级菜单
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为funk模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqfunk_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[2], sizeof(eq_param[2]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_FUNK;
        //foullsound关闭
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs关闭
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed关闭
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //返回到上一级菜单
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为hiphop模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqhiphop_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[3], sizeof(eq_param[3]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_HIP_HOP;
        //foullsound关闭
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs关闭
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed关闭
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //返回到上一级菜单
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为jazz模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqjazz_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[4], sizeof(eq_param[4]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_JAZZ;
        //foullsound关闭
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs关闭
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed关闭
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //返回到上一级菜单
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为classical模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqclassical_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[5], sizeof(eq_param[5]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_CLASSICAL;
        //foullsound关闭
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs关闭
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed关闭
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //返回到上一级菜单
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为techno模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_eqtechno_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    app_result_e set_vals;
    //eq_info.eq_type = 0x07;
    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, eq_param[6], sizeof(eq_param[6]));
        //music_set_eq(&eq_info);
        //eq funk
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_TECHNO;
        //foullsound关闭
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //srs关闭
        g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
        //speed关闭
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //返回到上一级菜单
    return set_vals;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为SRS关闭叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_srsoff_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;
    //eq_info.eq_type = 0x00;

    //libc_memcpy(eq_info.eq_para, eq_param[0], sizeof(eq_param[0]));
    //music_set_eq(&eq_info);
    //srs关闭
    g_setting_comval.music_comval.srs = VALUE_SRS_OFF;
    //返回到上一级菜单
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 eq设置为SRS whd模式叶子菜单执行函数
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       热键返回值
 * \ingroup      music_setmenu_soundset.c
 * \note
 */
/*******************************************************************************/
app_result_e set_scene_srs_wowhd_sure(void *param)
{
    //设置eq模式
    //mmm_mp_eq_info_t eq_info;

    app_result_e set_vals;

    //eq_info.eq_type = 0x4f;

    if (check_fullsound_close(&set_vals) == TRUE)
    {
        //libc_memcpy(eq_info.eq_para, srs_param, sizeof(srs_param));
        //music_set_eq(&eq_info);
        //srs
        g_setting_comval.music_comval.srs = VALUE_WOWHD_ON;
        //eq关闭
        g_setting_comval.music_comval.eq_setting = VALUE_EQ_OFF;
        //foullsound关闭
        g_setting_comval.music_comval.fullsound = VALUE_FULLSOUND_OFF;
        //speed关闭
        g_setting_comval.music_comval.variable_playback = 0;
        set_vals = RESULT_MENU_PARENT;
    }

    //返回到上一级菜单
    return set_vals;
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
static app_result_e set_scene_set_real_eq(uint8 eq_type, uint8 eq_param_index)
{
    //设置eq模式
    mmm_mp_eq_info_t eq_info;

    eq_info.eq_type = eq_type;

    if (eq_type == EQ_TYPE_USR_MODE)
    {
        if (eq_param_index == EQ_PARAM_USERSRS)
        {
            libc_memcpy(eq_info.eq_para, g_setting_comval.music_comval.srs_parameter, 12);
        }
        else
        {
            libc_memcpy(eq_info.eq_para, srs_wowhd_param, sizeof(srs_wowhd_param));
        }
    }
    else if (eq_param_index == EQ_PARAM_USEREQ)
    {
        libc_memcpy(eq_info.eq_para, g_setting_comval.music_comval.eq_parameter, 12);
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
void set_scene_eqnormal_callback(void)
{
    set_scene_set_real_eq(EQ_NORMAL, EQ_PARAM_NONE);
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
void set_scene_fullsoundon_callback(void)
{
    set_scene_set_real_eq(EQ_FULLSOUND_MODE, EQ_PARAM_FULLSOUND);
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
void set_scene_eqrock_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_ROCK);
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
void set_scene_eqfunk_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_FUNK);
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
void set_scene_eqhiphop_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_HIPHOP);
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
void set_scene_eqjazz_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_JAZZ);
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
void set_scene_eqclassic_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_CALSSIC);
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
void set_scene_eqtechno_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_TECHNO);
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
void set_scene_equser_callback(void)
{
    set_scene_set_real_eq(EQ_EQ_USR_MODE, EQ_PARAM_USEREQ);
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
void set_scene_srswowhd_callback(void)
{
    set_scene_set_real_eq(EQ_TYPE_USR_MODE, EQ_PARAM_WOWSRS);
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
void set_scene_srsuser_callback(void)
{
    set_scene_set_real_eq(EQ_TYPE_USR_MODE, EQ_PARAM_USERSRS);
}

