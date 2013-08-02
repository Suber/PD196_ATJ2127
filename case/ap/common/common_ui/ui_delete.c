/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.  
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-19 21:47     1.0             build this file  
*******************************************************************************/
/*! 
 * \file     _ui_delete.c
 * \brief    gui 控制流：文件或目录删除控件控制流实现。
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               实现了文件或目录删除功能。
 * \par      EXTERNALIZED FUNCTIONS:
 *               kernel，libc，applib，enhanced_bs，gui dialog，等等。
 * \version  1.0 
 * \date     2011-9-19 
*******************************************************************************/

#include "common_ui.h"
#include "music_common.h"

bool delete_just_file(uint16 list_no, file_path_info_t *path_info, brow_del_e brow_del, bool del_playing) __FAR__;
app_result_e deleted_all(file_path_info_t *path_info, del_com_data_t *del_com) __FAR__;
app_result_e deleted_file_display(uint8 *filename); 
void delete_string_format(string_desc_t *string, uint16 str_id, uint8 *file_name);

/******************************************************************************/
/*!
 * \par  Description:
 * \static _resume_music_after_delete( file_path_info_t *path_info)
 * \重启music播放并检测状态
 * \param[in]    path_info  文件路径
 * \param[out]   none
 * \return       void the result
 * \ingroup
 * \note
 * \li   如果当前无可播music文件，则关闭music引擎
 */
/*******************************************************************************/
static void _resume_music_after_delete( file_path_info_t *path_info)
{
    msg_apps_t msg;
    msg_reply_t reply;

    //向 manager ap 发消息恢复后台音乐播放
    msg.type = MSG_MENGINE_RESUME_SYNC;
    send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);

    if(reply.type == MSG_REPLY_FAILED)
    {
        //获取当前播放文件的路径信息
        reply.content = path_info;
        //消息类型(即消息名称)
        msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;

        send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);

        if(path_info->file_path.dirlocation.file_total == 0)
        {
            //杀死music后台
            msg.type = MSG_KILL_APP_SYNC;
            msg.content.data[0] = APP_ID_MENGINE;
            send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
        }
    }

}
/*! \cond COMMON_API */

/******************************************************************************/
/*!                     
 * \par  Description:
 *    删除文件或目录控件控制流实现。
 * \param[in]    path_info 要删除文件（目录）的路径结构体
 * \param[in]    del_com 删除控制结构体指针
 * \param[out]   path_info 删除文件（目录）后更新的路径结构体
 * \return       app_result_e
 * \retval           RESULT_REDRAW 询问是否删除对话框取消返回AP，重绘UI。
 * \retval           RESULT_DELETE_FILE 删除文件成功返回。
 * \retval           RESULT_DELETE_DIRLIST 删除文件夹或列表成功返回。
 * \retval           RESULT_DELETE_DIRLIST_NOSELF 删除文件夹或列表（不删除自身）成功返回。
 * \retval           RESULT_ERROR 删除文件失败返回 。
 * \retval           dialog other 直接返回其他从 dialog 返回的消息，详细参见 app_result_e 定义。
 * \retval           delete other 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义。
 * \ingroup      controls
 * \par          exmaple code
 * \code
    del_com_data_t del_com;
    app_result_e result;
    
    fsel_browser_select(list_no);
    fsel_browser_get_location(&path_info.file_path, FSEL_TYPE_COMMONDIR);//path_info在之前已经初始化过
    del_com.filename = g_file_name;
    del_com.del_mode = DEL_MODE_FILE;
    del_com.del_playing = FALSE;
    del_com.del_self = TRUE;
    del_com.del_func = ebook_del_func;
    result = gui_delete(&path_info, &del_com);
    if(result == RESULT_DELETE_FILE)//删除文件成功返回
    {
        //返回更新列表等
    } 
    else if(result == RESULT_REDRAW)//取消删除，返回重绘
    {
    
    }
    else//其他返回结果，需要做特殊处理
    {
    
    }
 * \endcode 
 * \note
 * \li  用户必须确保文件系统和文件选择器已经初始化过，并且 path_info 指向要删除的文件或列表。
*******************************************************************************/
app_result_e gui_delete(file_path_info_t *path_info, del_com_data_t *del_com)
{
    msg_apps_t msg;
    dialog_com_data_t dialog_com;
    string_desc_t delete_ask_str;
    app_result_e result = RESULT_NULL;
    bool ret, restart_music = FALSE;
    uint8 tag_backup;
    gui_direct_e tmp_direct;
    
    tag_backup = g_this_app_timer_tag;//get_app_timer_tag();
    change_app_timer_tag(APP_TIEMR_TAG_DELETE); 
    tmp_direct = com_get_gui_direction();
    
    //把文件名转换为 "部分..." 形式，在整个 gui_delete 生命周期有效
    com_ellipsis_to_longname(del_com->filename, com_name_buffer, 52);
    
    //询问是否要删除文件或目录
    delete_ask_str.data.str = com_str_buffer;
    
    //初始化对话框
    dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE; 
    dialog_com.button_type = BUTTON_YESNO;
    dialog_com.icon_id = -1;
    dialog_com.string_desc = &delete_ask_str;
    dialog_com.active_default = 1;
    
    //for 16KB page nandflash
    if((g_app_info_state.type == ENGINE_MUSIC)//正在播放音乐
    &&(g_app_info_state.state == ENGINE_STATE_PLAYING)
    && ((g_this_app_info->app_id != APP_ID_MUSIC) || (del_com->del_playing != TRUE)))//允许删除正在播放的音乐文件
    {
        delete_string_format(&delete_ask_str, S_CLOSE_MUSIC_DEL, com_name_buffer);
        
        //询问是否 先关闭音乐播放器后再删除文件
        result = gui_dialog (DIALOG_ASK, &dialog_com);
        if(result != RESULT_DIALOG_YES)
        {
            g_this_app_timer_tag = tag_backup;//change_app_timer_tag(tag_backup);
            return result;
        }
        
        //向 manager ap 发消息要求后台退出
        msg.type = MSG_MENGINE_PAUSE_SYNC;
        send_sync_msg(APP_ID_MENGINE, &msg, NULL, 0);
        
        restart_music = TRUE;//删除完成后恢复音乐播放
    }
    else
    {
        delete_string_format(&delete_ask_str, S_DEL_FILE_ASK, com_name_buffer);
        
        //询问是否 删除文件
        if(tmp_direct == GUI_DIRECT_NORMAL)//正常竖屏
        {
            result = gui_dialog (DIALOG_ASK, &dialog_com);
            if(result != RESULT_DIALOG_YES) 
            {
                g_this_app_timer_tag = tag_backup;//change_app_timer_tag(tag_backup);
                return result;
            }
        }
        else//横屏模式
        {
            result = gui_dialog (DIALOG_ASK_V, &dialog_com);
            if(result != RESULT_DIALOG_YES) 
            {
                g_this_app_timer_tag = tag_backup;//change_app_timer_tag(tag_backup);
                return result;
            }
        }
    }
    
    //显示 正在删除文件
    if(tmp_direct == GUI_DIRECT_NORMAL)//正常竖屏
    {
        gui_dialog_msg (DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_DEL_FILE_ING);
    }
    else//横屏模式
    {
        gui_dialog_msg (DIALOG_MSG_V, DIALOG_INFOR_IMMEDIATE, S_DEL_FILE_ING);
    }
    
    //执行删除回调函数
    if(del_com->del_func != NULL)
    {
        del_com->del_func();
    }
    
    //确定删除，删除文件
    if(del_com->del_mode == DEL_MODE_FILE)
    {
        //执行删除文件
        ret = delete_just_file(0, path_info, BROW_DEL_FILE, del_com->del_playing);
        if(ret == TRUE) 
        {
            result = deleted_file_display(com_name_buffer);
            if((result == RESULT_NULL) || (result == RESULT_REDRAW))
            {
                result = RESULT_DELETE_FILE; //删除文件完成返回
            }
        }
        else
        {
            result = RESULT_ERROR;
        }
    }
    //确定删除，删除文件夹或播放列表
    else
    {
        result = deleted_all(path_info, del_com);
        if (result == RESULT_DELETE_DIRLIST)
        {
            result = deleted_file_display(com_name_buffer);
            if(result == RESULT_NULL)
            {
                if(del_com->del_self == TRUE)
                {
                    result = RESULT_DELETE_DIRLIST; //删除文件夹或列表完成返回
                }
                else
                {
                    result = RESULT_DELETE_DIRLIST_NOSELF; //删除文件夹或列表（不删除自身）完成返回
                }
            }
        }
    }
    
    //恢复音乐播放
    if(restart_music == TRUE)
    {
        //向 manager ap 发消息恢复后台音乐播放
        _resume_music_after_delete(path_info);
    }
    g_this_app_timer_tag = tag_backup;//change_app_timer_tag(tag_backup);
    return result;
}

/*! \endcond */

/*! \cond */

/******************************************************************************/
/*!                     
 * \par  Description:
 *    提示已删除（动画提示）
 * \param[in]    path_info 要删除文件（目录）的路径结构体
 * \param[out]   none 
 * \return       app_result_e
 * \retval           RESULT_NULL 动画显示正常结束后返回AP
 * \retval           RESULT_REDRAW 应用消息处理返回 RESULT_REDRAW，包含在other2 中 
 * \retval           other1 动画显示终止返回（因用户gui输入而终止，依据key_map_list映射）
 * \retval           other2 收到返回值不为 RESULT_NULL 应用消息返回，见 app_result_e 定义
 * \note 
*******************************************************************************/
app_result_e deleted_file_display(uint8 *filename)
{
    style_infor_t style_infor, style_infor1, style_infor2;
    textbox_private_t deleted_infor;
    string_desc_t delete_str;
    animation_com_data_t deleted_anim;
    app_result_e result;
    uint8 mode_backup, icon_backup; 
    gui_direct_e tmp_direct;
    
    gui_get_headbar_mode(&mode_backup, &icon_backup);
    gui_set_headbar_mode(HEADBAR_MODE_NULL, this_headbar_icon_id);
    tmp_direct = com_get_gui_direction();
    
    style_infor.type = UI_COM;
    style_infor1.type = UI_COM;
    style_infor2.type = UI_COM;
    
    //显示背景图片
    if(tmp_direct == GUI_DIRECT_NORMAL)//正常竖屏
    {
        style_infor.style_id = DELETED_BG;
        style_infor1.style_id = DELETED_INFOR;
        style_infor2.style_id = DELETED_ANIM;
    }
    else
    {
        style_infor.style_id = DELETED_BG_V;
        style_infor1.style_id = DELETED_INFOR_V;
        style_infor2.style_id = DELETED_ANIM_V;
    }
    ui_show_picbox(&style_infor, NULL); 
    
    //显示“已删除成功”
    delete_str.data.str = com_str_buffer;
    delete_string_format(&delete_str, S_DEL_FILE_OK, filename); 
    
    deleted_infor.lang_id = UNICODEDATA;
    deleted_infor.str_value = com_str_buffer;
    deleted_infor.str_id = -1;
    ui_show_textbox(&style_infor1, &deleted_infor, TEXTBOX_DRAW_NORMAL); 
    
    //显示删除成功动画
    deleted_anim.direction = DIRECTION_NORMAL; 
    deleted_anim.interval = 250;
    deleted_anim.forbid = 0;
    deleted_anim.callback = NULL;
    result = gui_animation(&style_infor2, &deleted_anim); 
    
    gui_set_headbar_mode(mode_backup, icon_backup); 
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    整合删除询问字符串。
 * \param[in]    str_id 删除询问多国语言字符串ID
 * \param[in]    file_name 要删除的文件名字
 * \param[out]   string 删除询问字符串缓冲区
 * \return       none
 * \note
 *******************************************************************************/
void delete_string_format(string_desc_t *string, uint16 str_id, uint8 *file_name)
{
    string_desc_t temp_string;
    
    string->argv = str_id;
    string->language = UNICODEDATA; 
    string->length = 128;
    
    temp_string.data.str = file_name;
    temp_string.length = -1;
    if((file_name[0] == 0xff)
    && (file_name[1] == 0xfe))
    {
        temp_string.language = UNICODEDATA; 
    }
    else
    {
        temp_string.language = ANSIDATAAUTO; 
    }

    com_string_format(string, &temp_string);
}

/*! \endcond */
