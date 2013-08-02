/*******************************************************************************
 *                              US212A
 *                            Module: MUSIC_CLEAR_FAVORITE
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-11-9 16:12:18           1.0              build this file
 *******************************************************************************/

#include "app_music.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	   清除某一收藏夹列表，如果当前播放歌曲正好是该收藏夹的曲目
 *     则停止音乐播放，返回菜单界面
 * \param[in]  target_type 目标收藏夹类型
 * \param[out] 返回操作结果,是否加入成功
 * \return
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e clr_favorite_list(uint8 target_type)
{
    uint8 ret = 0;
    bool need_check_engine = FALSE;

    dialog_com_data_t dialog_clr;
    //目标字符串
    string_desc_t ret_clr_str;
    //源字符串
    string_desc_t clr_src_str;

    app_result_e clr_result;

    //是否后台存在music
    engine_type_e engine_type = get_engine_type();

    if (engine_type == ENGINE_MUSIC)
    {
        music_get_filepath(&g_file_path_info);
        if (g_file_path_info.file_source == target_type)
        {
            need_check_engine = TRUE;
        }
    }

    dialog_clr.dialog_type = DIALOG_INFOR_WAIT;
    dialog_clr.icon_id = -1;

    //字符串
    dialog_clr.string_desc = &ret_clr_str;
    
    ret = fsel_favorite_clear(target_type);
    //关闭引擎
    if ((need_check_engine == TRUE)&&(ret == TRUE))
    {
        music_close_engine();
    }
    if (ret == TRUE)
    {
        //现在播放列表xxx已清除
        clr_src_str.language = UNICODEID;

        //清除哪个收藏夹
        if (target_type == FSEL_TYPE_LISTFAVOR1)
        {
            clr_src_str.data.id = S_NOW_FAVORITE1;
        }
        else if (target_type == FSEL_TYPE_LISTFAVOR2)
        {
            clr_src_str.data.id = S_NOW_FAVORITE2;
        }
        else
        {
            clr_src_str.data.id = S_NOW_FAVORITE3;
        }
        
        ret_clr_str.argv = S_ALREADY_CLR_FAVORITE;
        
        //ret_clr_str.data.str = g_insert_dialog;
        ret_clr_str.data.str = g_temp_buffer_p;
        ret_clr_str.length = 128;
        
        //ret_clr_str.language = (int8) g_setting_comval.g_comval.language_id;
        ret_clr_str.language = UNICODEDATA;
         //组织对话框字符串
        com_string_format(&ret_clr_str, &clr_src_str);
    
        gui_dialog(DIALOG_MSG, &dialog_clr);
    }

    if (need_check_engine == TRUE)
    {
        //未找到歌曲
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT,S_NO_SONG);

    }
    //返回回调该函数的菜单项
    clr_result = RESULT_LISTMENU_MENU;
    return clr_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	   清除播放列表菜单回调函数
 * \param[in]  target_type 收藏夹类型
 * \param[out] 返回操作结果,是否加入成功
 * \return
 * \retval
 * \ingroup    music_favorite
 * \note
 *******************************************************************************/

app_result_e clr_favorite_menu(uint8 target_type)
{

    dialog_com_data_t ask_clr_data;
    //目标字符串
    string_desc_t ask_clr_str;   
    //字符串组织
    string_desc_t format_data;

    app_result_e ret;

    app_result_e clr_result;
    
    format_data.language = UNICODEID;

    //清除哪个收藏夹
    if (target_type == FSEL_TYPE_LISTFAVOR1)
    {
        format_data.data.id = S_NOW_FAVORITE1;
    }
    else if (target_type == FSEL_TYPE_LISTFAVOR2)
    {
        format_data.data.id = S_NOW_FAVORITE2;
    }
    else
    {
        format_data.data.id = S_NOW_FAVORITE3;
    }
    
    ask_clr_str.argv = S_CLEAR_FAVORITE1;
    
    //ask_clr_str.data.str = g_insert_dialog;
    ask_clr_str.data.str = g_temp_buffer_p;
    ask_clr_str.length = 128;
    
    //ask_clr_str.language = (int8) g_setting_comval.g_comval.language_id;
    ask_clr_str.language = UNICODEDATA;
     //组织对话框字符串
    com_string_format(&ask_clr_str, &format_data);
    
    //对话框类型
    ask_clr_data.dialog_type = DIALOG_BUTTON_INCLUDE;
    ask_clr_data.icon_id = -1;
    //字符串
    ask_clr_data.string_desc = &ask_clr_str;

    //按键类型
    ask_clr_data.button_type = BUTTON_YESNO;
    /*! 默认激活按键项 */
    ask_clr_data.active_default = 0x00;

    ret = gui_dialog(DIALOG_ASK, &ask_clr_data);

    if (ret == RESULT_DIALOG_YES)
    {
        clr_result = clr_favorite_list(target_type);
    }
    else if (ret == RESULT_REDRAW)
    {
        clr_result = RESULT_LISTMENU_MENU;
    }
    else
    {
        clr_result = ret;
    }
    return clr_result;
}

/*! \endcond */
