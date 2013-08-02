/*
 *******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_menu.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_record.h"
#include "record_menu_config.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e record_menu_func(void *title_action)
 * \判断选择哪个入口菜单
 * \param[in]    void
 * \param[out]   菜单title指针
 * \return       app_result_e
 * \retval       RESULT_MENU_SON 进入下一级菜单
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e record_menu_func(void *title_action)
{
    int card_exist;
    menu_title_action_t* menu_title = (menu_title_action_t*) title_action;

    card_exist = sys_detect_disk(DRV_GROUP_STG_CARD);

    if (card_exist == 0) //检测有卡
    {
        if (g_record_vars.file_type == REC_FORMAT_ACT)
        {
            menu_title->menu_entry = ENTRY_ACT_CARD;
        }
        else
        {
            menu_title->menu_entry = ENTRY_CARD;
        }
    }
    else //无卡盘
    {
        if (g_record_vars.file_type == REC_FORMAT_ACT)
        {
            menu_title->menu_entry = ENTRY_ACT_NOCARD;
        }
        else
        {
            menu_title->menu_entry = ENTRY_NOCARD;
        }
    }

    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e record_scene_menu(void)
 * \菜单管理器模块
 * \param[in]    none
 * \param[out]   none
 * \return       app_result_e
 * \retval       RESULT_IGNOR 忽略
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e record_scene_menu(void)
{
    app_result_e app_ret;
    menu_com_data_t menu_com;

    /*初始化菜单控件*/
    menu_com.app_id = APP_ID_RECORD;
    /*! 首层菜单回调函数,从叶子菜单返回可改变入口菜单*/
    menu_com.menu_func = record_menu_func;
    menu_com.menu_entry = ENTRY_CARD;
    menu_com.path_id = 0;

    app_ret = gui_menulist(MENULIST, &menu_com);

    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e start_record(void* param)
 * \进入录音界面
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_RECORD_REC 开始录音
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e start_record(void* param)
{
    app_result_e app_ret;
    app_ret = RESULT_RECORD_REC;
    return app_ret;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e show_rec_dir(void* param)
 * \显示录音文件列表
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_BROWSER 返回browser应用
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e show_rec_dir(void* param)
{
    return RESULT_BROWSER;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e select_disk(void *title_action)
 * \选择磁盘,指定下一级菜单的默认值
 * \param[in]    none
 * \param[out]   (menu_title_action_t*) title_action
 * \return       app_result_e result
 * \retval       RESULT_MENU_SON 进入下一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e select_disk(void *title_action)
{
    menu_title_action_t* menu_title_sel_disk;
    menu_title_sel_disk = (menu_title_action_t*) title_action;

    if (DISK_C == g_record_vars.path.file_path.dirlocation.disk)
    {
        menu_title_sel_disk->str_id = SEL_DISK_INTERNAL;
    }
    else if (DISK_H == g_record_vars.path.file_path.dirlocation.disk)
    {
        menu_title_sel_disk->str_id = SEL_DISK_EXTERNAL;
    }
    else
    {
    }

    menu_title_sel_disk->menu_entry = -1; //更新入口
    menu_title_sel_disk->ret_str = NULL;
    menu_title_sel_disk->source_id = -1;

    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e select_disk_internal(void* param)
 * \选择磁盘
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e select_disk_internal(void* param)
{
    g_record_vars.path.file_path.dirlocation.disk = DISK_C;
    g_rec_num = g_record_vars.rec_num;
    g_fmrec_num = g_record_vars.fmrec_num;
    if (need_fsel_init == FALSE)
    {
        fsel_exit();
        need_fsel_init = TRUE;
    }
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e select_disk_external(void* param)
 * \选择磁盘
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e select_disk_external(void* param)
{
    g_record_vars.path.file_path.dirlocation.disk = DISK_H;
    g_rec_num = g_record_vars.rec_num_card;
    g_fmrec_num = g_record_vars.fmrec_num_card;
    if (need_fsel_init == FALSE)
    {
        fsel_exit();
        need_fsel_init = TRUE;
    }
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_format(void *title_action)
 * \选择磁盘
 * \param[in]    none
 * \param[out]   (menu_title_action_t*) title_action
 * \return       app_result_e result
 * \retval       RESULT_MENU_SON 进入下一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_format(void *title_action)
{
    menu_title_action_t* menu_title_rec_format;
    menu_title_rec_format = (menu_title_action_t*) title_action;

    if (REC_FORMAT_WAV == g_record_vars.file_type)
    {
        menu_title_rec_format->str_id = SET_REC_FORMAT_WAV;
    }
    else if (REC_FORMAT_MP3 == g_record_vars.file_type)
    {
        menu_title_rec_format->str_id = SET_REC_FORMAT_MP3;
    }
    else
    {
        menu_title_rec_format->str_id = SET_REC_FORMAT_ACT;
    }

    menu_title_rec_format->menu_entry = -1; //更新入口
    menu_title_rec_format->ret_str = NULL;
    menu_title_rec_format->source_id = -1;

    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_format_wav(void* param)
 * \设置录音文件类型
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_format_wav(void* param)
{
    g_record_vars.file_type = REC_FORMAT_WAV;
    g_record_vars.bitrate = g_record_vars.wav_bitrate;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_format_mp3(void* param)
 * \设置录音文件类型
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_format_mp3(void* param)
{
    g_record_vars.file_type = REC_FORMAT_MP3;
    g_record_vars.bitrate = g_record_vars.mp3_bitrate;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_format_act(void* param)
 * \设置录音文件类型
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_format_act(void* param)
{
    g_record_vars.file_type = REC_FORMAT_ACT;
    g_record_vars.bitrate = g_record_vars.act_bitrate;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_source(void *title_action)
 * \设置录音来源
 * \param[in]    (menu_title_action_t*) title_action
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_SON 进入下一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_source(void *title_action)
{
    menu_title_action_t* menu_title_rec_source;
    menu_title_rec_source = (menu_title_action_t*) title_action;

    if (AI_SOURCE_MIC == g_record_vars.ai_source)
    {
        menu_title_rec_source->str_id = SET_REC_SOURCE_MIC;
    }
    else if (AI_SOURCE_LINEIN == g_record_vars.ai_source)
    {
        menu_title_rec_source->str_id = SET_REC_SOURCE_LINEIN;
    }
    else
    {
        ;//qac need
    }

    menu_title_rec_source->menu_entry = -1; //更新入口
    menu_title_rec_source->ret_str = NULL;
    menu_title_rec_source->source_id = -1;

    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_source_mic(void* param)
 * \设置录音来源
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_source_mic(void* param)
{
    g_record_vars.ai_source = AI_SOURCE_MIC;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_source_mic(void* param)
 * \设置录音来源
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_source_linein(void* param)
{
    g_record_vars.ai_source = AI_SOURCE_LINEIN;
    return RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得录音比特率的菜单入口（由于mp3和wav格式的比特率
 不一致）
 *      重点说明
 * \param[in]    ：none
 * \param[out]   : (menu_title_action_t*) title_action
 * \retval       ：进入下一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_rec_bitrate_entry(void *title_action)
{
    menu_title_action_t* menu_title_rec_bitrate;
    menu_title_rec_bitrate = (menu_title_action_t*) title_action;
    //uint16 next_entry;

    if (g_record_vars.file_type == REC_FORMAT_MP3)
    {
        //next_entry = ENTRY_BITRATE_MP3;
        menu_title_rec_bitrate->menu_entry = ENTRY_BITRATE_MP3;
        switch (g_record_vars.mp3_bitrate)
        {
            case BIT_32KBPS:
            menu_title_rec_bitrate->str_id = SET_REC_BITRATE_32KBPS;
            break;
            case BIT_64KBPS:
            menu_title_rec_bitrate->str_id = SET_REC_BITRATE_64KBPS;
            break;
            case BIT_128KBPS:
            menu_title_rec_bitrate->str_id = SET_REC_BITRATE_128KBPS;
            break;
            case BIT_192KBPS:
            menu_title_rec_bitrate->str_id = SET_REC_BITRATE_192KBPS;
            break;
            case BIT_256KBPS:
            menu_title_rec_bitrate->str_id = SET_REC_BITRATE_256KBPS;
            break;
            default:
            break;
        }
    }
    else if (g_record_vars.file_type == REC_FORMAT_WAV)
    {
        //next_entry = ENTRY_BITRATE_WAV;
        menu_title_rec_bitrate->menu_entry = ENTRY_BITRATE_WAV;
        switch (g_record_vars.wav_bitrate)
        {
            case BIT_512KBPS:
            menu_title_rec_bitrate->str_id = SET_REC_BITRATE_512KBPS;
            break;
            case BIT_768KBPS:
            menu_title_rec_bitrate->str_id = SET_REC_BITRATE_768KBPS;
            break;
            case BIT_1024KBPS:
            menu_title_rec_bitrate->str_id = SET_REC_BITRATE_1024KBPS;
            break;
            case BIT_1536KBPS:
            menu_title_rec_bitrate->str_id = SET_REC_BITRATE_1536KBPS;
            break;
            default:
            break;
        }
    }
    else
    {
        menu_title_rec_bitrate->menu_entry = ENTRY_BITRATE_ACT;
        menu_title_rec_bitrate->str_id = SET_REC_BITRATE_8KBPS;
    }

    //menu_title_rec_bitrate->menu_entry = next_entry; //更新入口
    menu_title_rec_bitrate->ret_str = NULL;
    menu_title_rec_bitrate->source_id = -1;

    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_8KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_8kbps(void* param)
{
    g_record_vars.act_bitrate = BIT_8KBPS;
    g_record_vars.bitrate = BIT_8KBPS;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_32KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_32kbps(void* param)
{
    g_record_vars.mp3_bitrate = BIT_32KBPS;
    g_record_vars.bitrate = BIT_32KBPS;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_64KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_64kbps(void* param)
{
    g_record_vars.mp3_bitrate = BIT_64KBPS;
    g_record_vars.bitrate = BIT_64KBPS;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_128KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_128kbps(void* param)
{
    g_record_vars.mp3_bitrate = BIT_128KBPS;
    g_record_vars.bitrate = BIT_128KBPS;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_192KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_192kbps(void* param)
{
    g_record_vars.mp3_bitrate = BIT_192KBPS;
    g_record_vars.bitrate = BIT_192KBPS;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_256KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_256kbps(void* param)
{
    g_record_vars.bitrate = BIT_256KBPS;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_512KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_512kbps(void* param)
{
    g_record_vars.wav_bitrate = BIT_512KBPS;
    g_record_vars.bitrate = BIT_512KBPS;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_768KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_768kbps(void* param)
{
    g_record_vars.wav_bitrate = BIT_768KBPS;
    g_record_vars.bitrate = BIT_768KBPS;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_1024KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_1024kbps(void* param)
{
    g_record_vars.wav_bitrate = BIT_1024KBPS;
    g_record_vars.bitrate = BIT_1024KBPS;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_rec_bitrate_1536KBPS(void* param)
 * \设置录音比特率
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_rec_bitrate_1536kbps(void* param)
{
    g_record_vars.wav_bitrate = BIT_1536KBPS;
    g_record_vars.bitrate = BIT_1536KBPS;
    return RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得avr设置的菜单入口（由于line-in和mic-in录音的avr设置
 菜单不一致）
 *      重点说明
 * \param[in]    ：none
 * \param[out]   : (menu_title_action_t*) title_action
 * \retval       ：RESULT_MENU_SON 进入下一级菜单
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_avr_track_entry(void *title_action)
{
    menu_title_action_t* menu_title_avr_track;
    menu_title_avr_track = (menu_title_action_t*) title_action;
    //uint16 next_entry;

    if (g_record_vars.ai_source == AI_SOURCE_LINEIN)
    {
        //next_entry = ENTRY_LINEIN_MODE;
        menu_title_avr_track->menu_entry = ENTRY_LINEIN_MODE;
        if (g_record_vars.track_mode == TRACK_ON)
        {
            menu_title_avr_track->str_id = SET_TRACK_MODE;
        }
        else
        {
            menu_title_avr_track->str_id = SET_NORMAL_MODE;
        }
    }
    else
    {
        //next_entry = ENTRY_MICIN_MODE;
        menu_title_avr_track->menu_entry = ENTRY_MICIN_MODE;
        if (g_record_vars.avr_mode == AVR_ON)
        {
            menu_title_avr_track->str_id = SET_AVR_MODE;
        }
        else
        {
            menu_title_avr_track->str_id = SET_NORMAL_MODE;
        }
    }

    //menu_title_avr_track->menu_entry = next_entry; //更新入口
    menu_title_avr_track->ret_str = NULL;
    menu_title_avr_track->source_id = -1;

    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_track_mode(void* param)
 * \设置自动分曲模式
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_track_mode(void* param)
{
    g_record_vars.track_mode = TRACK_ON; //自动分曲开
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_normal_mode(void* param)
 * \设置分曲模式
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_normal_mode(void* param)
{
    if (g_record_vars.ai_source == AI_SOURCE_LINEIN)
    {
        g_record_vars.track_mode = TRACK_OFF; //普通模式，自动分曲关
    }
    else
    {
        g_record_vars.avr_mode = AVR_OFF;
    }
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_normal_mode(void* param)
 * \设置分曲模式
 * \param[in]    未使用
 * \param[out]   none
 * \return       app_result_e result
 * \retval       RESULT_MENU_PARENT 返回上一级菜单
 * \retval       ......
 * \ingroup      record_menu.c
 * \note
 */
/*******************************************************************************/
app_result_e set_avr_mode(void* param)
{
    g_record_vars.avr_mode = TRACK_ON; //avr模式
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_noise_cancel_normal_mode(void *param)
 * \设置普通降噪模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the resul t
 * retval RESULT_IGNORE 忽略
 * retval
 * ingroup record_menu.c
 * note
 */
/**************************************************************************** ***/
app_result_e set_noise_cancel_normal_mode(void *param)
{
    if (g_record_vars.noise_cancel_level == 0)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, START_NOISE_CANCELLATION);
    }

    g_record_vars.noise_cancel_level = DENOISELEVEL(3, 0, 0, 100);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_noise_cancel_train_mode(void *param)
 * \设置火车场景降噪模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the resul t
 * retval RESULT_IGNORE 忽略
 * retval
 * ingroup record_menu.c
 * note
 */
/**************************************************************************** ***/
app_result_e set_noise_cancel_train_mode(void *param)
{
    if (g_record_vars.noise_cancel_level == 0)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, START_NOISE_CANCELLATION);
    }

    g_record_vars.noise_cancel_level = DENOISELEVEL(3, 0, 0, 40);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_noise_cancel_meeting_mode(void *param)
 * \设置会议场景降噪模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the resul t
 * retval RESULT_IGNORE 忽略
 * retval
 * ingroup record_menu.c
 * note
 */
/**************************************************************************** ***/
app_result_e set_noise_cancel_meeting_mode(void *param)
{
    if (g_record_vars.noise_cancel_level == 0)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, START_NOISE_CANCELLATION);
    }

    g_record_vars.noise_cancel_level = DENOISELEVEL(6, 0, 0, 150);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_noise_cancel_long_range_mode(void *param)
 * \设置远距离场景降噪模式
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the resul t
 * retval RESULT_IGNORE 忽略
 * retval
 * ingroup record_menu.c
 * note
 */
/**************************************************************************** ***/
app_result_e set_noise_cancel_long_range_mode(void *param)
{
    if (g_record_vars.noise_cancel_level == 0)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, START_NOISE_CANCELLATION);
    }
    g_record_vars.noise_cancel_level = DENOISELEVEL(3, 0, 8, 150);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_noise_cancel_disable(void *param)
 * \关闭主动降噪
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the resul t
 * retval RESULT_IGNORE 忽略
 * retval
 * ingroup record_menu.c
 * note
 */
/**************************************************************************** ***/
app_result_e set_noise_cancel_disable(void *param)
{
    g_record_vars.noise_cancel_level = 0;
    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, STOP_NOISE_CANCELLATION);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \app_result_e set_noise_cancel_sure(void *param)
 * \设置降噪级别
 * \param[in]    void  para1
 * \param[out]   none
 * \return       app_result_e the resul t
 * retval RESULT_IGNORE 忽略
 * retval
 * ingroup record_menu.c
 * note
 */
/**************************************************************************** ***/
app_result_e set_noise_cancel_sure(void *param)
{
    uint16 active_id;
    menu_title_action_t * menu_title;
    menu_title = (menu_title_action_t*) param;

    switch (g_record_vars.noise_cancel_level)
    {
        case DENOISELEVEL(3, 0, 0, 40):
        active_id = S_REC_TRAIN_SCENE;
        break;

        case DENOISELEVEL(6, 0, 0, 150):
        active_id = S_REC_MEETING_SCENE;
        break;

        case DENOISELEVEL(3, 0, 8, 150):
        active_id = S_REC_LONG_RANGE_SCENE;
        break;

        case DENOISELEVEL(3, 0, 0, 100):
        active_id = S_REC_NORMAL_SCENE;
        break;

        default:
        active_id = S_REC_STOP_NOISE_CANCELLATION;
        break;
    }

    menu_title->str_id = active_id;
    return RESULT_MENU_SON;
}

