/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_stopwatch_tools.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_tools.h"

const uint8 fw_file_name[] =
{
    0xff,0xfe,
    'U', 0x00,
    'P', 0x00,
    'G', 0x00,
    'R', 0x00,
    'A', 0x00,
    'D', 0x00,
    'E', 0x00,
    '.', 0x00,
    'H', 0x00,
    'E', 0x00,
    'X', 0x00,
    0x00,0x00
};

const uint8 fw_ascfile_name[] = {"UPGRADE HEX"};

const uint8 fw_real_name[]={"UPGRADE.HEX"};


/******************************************************************************/
/*!
 * \par  Description:
 *	  browser 获取后台引擎盘符
 * \param[in]    none
 * \param[out]   none
 * \return       disk_type
 * \ingroup      menu_callback_autoupgrade_tools.c
 * \note
 *******************************************************************************/
static uint8 music_get_disk_type(void)
{
    bool bret = TRUE;
    msg_apps_t msg;
    //返回消息内容存放变量
    msg_reply_t reply;

    file_path_info_t music_file_path;

    //获取当前播放文件的路径信息
    reply.content = &music_file_path;
    //消息类型(即消息名称)
    msg.type = MSG_MENGINE_GET_FILEPATH_SYNC;
    //发送同步消息
    bret = send_sync_msg(APP_ID_MENGINE, &msg, &reply, 0);

    return music_file_path.file_path.dirlocation.disk;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    检测后台类型
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      tools
 * \note
 *******************************************************************************/
static void _check_music_engine(void)
{
    msg_apps_t msg;
    engine_type_e engine_type;

    engine_type = get_engine_type();
    if (engine_type == ENGINE_MUSIC)
    {
        if (music_get_disk_type() != DISK_C)
        {
            //当前播放非主盘音乐
            //向process manager发消息要求后台退出
            msg.type = MSG_KILL_APP_SYNC;
            msg.content.data[0] = APP_ID_MENGINE;
            send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
        }
    }

    return;
}
/*
 *****************************************************************************
 * \par  Description:
 *      询问是否进行自动升级
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：app_result_e
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e ask_for_auto_upgrade(void)
{
    uint8 temp_buffer[64];
    dialog_com_data_t dialog;
    string_desc_t string_source;//源字符串
    string_desc_t string_dest;//目的字符串

    string_source.data.str = fw_real_name;
    string_source.length = 12;
    string_source.language = (int8) g_comval.language_id;

    string_dest.data.str = temp_buffer;
    string_dest.length = 64;
    string_dest.language = (int8) g_comval.language_id;
    string_dest.argv = S_SELECT_FILE_UPGRADE;

    com_string_format(&string_dest, &string_source);

    //dialog控件参数初始化
    dialog.icon_id = -1;
    dialog.string_desc = &string_dest;

    //询问
    //对话框参数设置
    dialog.dialog_type = DIALOG_BUTTON_INCLUDE;

    dialog.button_type = BUTTON_YESNO;
    dialog.active_default = 0;
    return gui_dialog(DIALOG_ASK, &dialog);
}
/*
 *****************************************************************************
 * \par  Description:
 *      叶子菜单回调函数自动升级
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：app_result_e
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e autoupgrade_menu_entry(void *menu_title_void)
{
    int32 ret_val;
    int32 file_sys_id;
    app_result_e ui_result;
    uint32 file_handle;

    _check_music_engine();

    ret_val = sys_drv_install(DRV_GROUP_STG_BASE, 0, "nand.drv");
    if (ret_val != 0)
    {
        return RESULT_REDRAW;
    }

    //装载文件系统
    file_sys_id = sys_mount_fs(DRV_GROUP_STG_BASE, DISK_C, 0);
    if (file_sys_id != 0)
    {
        return RESULT_REDRAW;
    }

    vfs_cd(file_sys_id, CD_ROOT, 0);

    file_handle = vfs_file_open(file_sys_id, fw_file_name, R_NORMAL_SEEK);

    if (file_handle == 0)
    {
        file_handle = vfs_file_open(file_sys_id, fw_ascfile_name, R_NORMAL_SEEK);
    }

    if (file_handle == 0)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_FILE_NO_FILE);
        ui_result = RESULT_REDRAW;
    }
    else
    {
        vfs_file_close(file_sys_id, file_handle);

        ui_result = ask_for_auto_upgrade();

        if (ui_result == RESULT_DIALOG_YES)
        {
            ui_result = RESULT_AUTO_UPGRADE;
        }
    }

    sys_unmount_fs(file_sys_id);

    sys_drv_uninstall(DRV_GROUP_STG_BASE);

    return ui_result;
}
