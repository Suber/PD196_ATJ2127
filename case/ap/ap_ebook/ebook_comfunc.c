/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_comfunc.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo		2011-11-11         v1.0              create this file
 ********************************************************************************/
#include "ebook.h"

const uint8 hex_str[8] =
{ 0x2E, 0x0, 0x42, 0x0, 0x4D, 0x0, 0x4B, 0x0 };//.bmk字符
const uint8 bmk_str[3] =
{ 0x42, 0x4D, 0x4B };//".bmk"字符


/********************************************************************************
 * Description :清屏和设置背景色
 *
 * Arguments  :
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
void _clear_and_set_screen(COLORREF pencolor, COLORREF backcolor)
{
    ui_set_pen_color(pencolor);
    ui_set_backgd_color(backcolor);
    ui_clear_screen(NULL);
#ifdef PC
    UpdateMainWnd();		//PC上，更新窗口
#endif
}
/********************************************************************************
 * Description :清屏和设置背景色(显示头部页码部分的区域)
 *
 * Arguments  :
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
void _clear_and_set_head_screen(COLORREF pencolor, COLORREF backcolor)
{
    region_t row_region;
    ui_set_pen_color(pencolor);
    ui_set_backgd_color(backcolor);
    row_region.x = 0;
    row_region.y = 0;
    row_region.width = REGION_LENGTH;
    row_region.height = REGION_HEIGHT;
    ui_clear_screen(&row_region);
#ifdef PC
    UpdateMainWnd();		//PC上，更新窗口
#endif
}

/********************************************************************************
 * Description :清屏和设置背景色(显示文本内容部分的区域)
 *
 * Arguments  :
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
void _clear_and_set_text_screen(COLORREF pencolor, COLORREF backcolor)
{
    region_t row_region;
    ui_set_pen_color(pencolor);
    ui_set_backgd_color(backcolor);
    row_region.x = 0;
    row_region.y = REGION_HEIGHT;
    row_region.width = REGION_LENGTH;
    row_region.height = REGION_HEIGHT * 9;
    ui_clear_screen(&row_region);
#ifdef PC
    UpdateMainWnd();		//PC上，更新窗口
#endif
}

/********************************************************************************
 * Description :获取需要创建的书签名字
 *
 * Arguments  :
 * Returns     :
 *                无
 * Notes       :
 *			  字符名字的长度必须跟ap_play_list中创建书签的长度一样，否则
 *			  会在应用中重新创建另一个长度不同的书签文件。
 ********************************************************************************/
uint8 _get_bmk_name(uint16 filename_length)
{
    //判断文件名是否为长名
    if ((BOOKMARK_filename[0] == 0xFF) && (BOOKMARK_filename[1] == 0xFE))
    {
        if ((filename_length > 7) && (filename_length <= MAX_FILENAME_LENGTH))
        {
            //获取需要创建书签的长名
            libc_memcpy(&BOOKMARK_filename[filename_length * 2 - 10], hex_str, 8);
        }
        else
        {
            return 0;
        }
    }
    else
    {
        if ((filename_length > 4) && (filename_length <= (MAX_FILENAME_LENGTH * 2)))
        {
            //获取需要创建书签的短名
            libc_memcpy(&BOOKMARK_filename[filename_length - 3], bmk_str, 3);
        }
        else
        {
            return 0;
        }

    }
    //定位到根目录
    vfs_cd(g_ebook_mount_id, CD_ROOT, 0);
    return 1;
}
app_result_e _deal_menu_result(void)
{
    if (is_reading_menu == TRUE)
    {
        enter_mode = ENTER_FROM_READING;
        return RESULT_EBK_READINGMENU;
    }
    else
    {
        enter_mode = ENTER_FROM_FILELIST;
        return RESULT_EBK_FILELIST;
    }
}
/********************************************************************************
 * Description :显示提示框
 *
 * Arguments  :
 *				option_id:提示
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
void _show_single_dialog(uint16 option_id, uint8 dialog_type)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    _clear_and_set_screen(0xffff, 0x0);
    string_desc.data.id = option_id;
    string_desc.language = UNICODEID;
    dialog_com.dialog_type = dialog_type;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    dialog_com.active_default = 0;
    gui_dialog(DIALOG_MSG, &dialog_com);
#ifdef PC
    UpdateMainWnd();		//PC上，更新窗口
#endif
}

/******************************************************************************/
/*!
 * \par  Description:
 *    通知后台引擎退出
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      video
 * \note
 *******************************************************************************/
void _close_engine(void)
{
    msg_apps_t msg;
    if (engine_type == ENGINE_MUSIC)
    {
        music_play_device = 0;
        engine_type = ENGINE_NULL;
        msg.type = MSG_KILL_APP_SYNC;
        msg.content.data[0] = APP_ID_MENGINE;
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
    }
    return;
}

