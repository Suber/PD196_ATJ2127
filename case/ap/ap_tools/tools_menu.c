/*******************************************************************************
 *                              US212A
 *                            Module: TOOLS
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      liuweizhan    2011-12-12 17:03:16           1.0              build this file
 *******************************************************************************/
/*!
 * \file     tools_menu.c
 * \brief
 * \author   liuwzh
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2011-12-12
 *******************************************************************************/
#include "ap_tools.h"

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于option键的处理，用来显示提示信息
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：无
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e option_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    string_desc.data.id = S_TOOLS_OPTION;
    string_desc.language = UNICODEID;

    dialog_com.dialog_type = DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    //	dialog_com.msg_callback = NULL;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:tools主菜单处理函数
 * \param[in]    ：无
 * \param[out]   : app_result_e result
 * \retval       ：无
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e tools_menulist(void)
{
    menu_com_data_t menu_com_tools;
    app_result_e result;

    menu_com_tools.app_id = APP_ID_TOOLS;
    //判断是否有卡盘
    if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
    {
        menu_com_tools.menu_entry = MENU_ENTRY_ID_MAIN_NOCARD;
    }
    else
    {
        menu_com_tools.menu_entry = MENU_ENTRY_ID_MAIN;
    }
    //menu_com_tools.menu_entry = MENU_ENTRY_ID_MAIN;
    menu_com_tools.path_id = 0;
    menu_com_tools.menu_func = NULL;
    //    menu_com.msg_callback = NULL;
    result = gui_menulist(MENULIST, &menu_com_tools);
    return result;
}
/*
 *****************************************************************************
 * \par  Description:
 *      普通说明 : 子菜单的回调函数处理
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：无
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e calendar_menu_entry(void *menu_title_void)
{
    return (app_result_e) RESULT_CALENDAR;
}
app_result_e stopwatch_menu_entry(void *menu_title_void)
{
    return (app_result_e) RESULT_STOPWATCH;
}
app_result_e fwupdata_menu_entry(void *menu_title_void)
{
    return (app_result_e) RESULT_UDISK;
}
app_result_e dataexch_menu_entry(void *menu_title_void)
{
    return (app_result_e) RESULT_DATAEXCH;
}
/*
 *****************************************************************************
 * \par  Description:
 *      普通说明 : 子菜单的回调函数处理
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：无
 * \note:    alarm与FMalarm的回调入口函数与上面3个不同，
 *
 ******************************************************************************
 */
app_result_e alarm_menu_entry(void *menu_title_void)
{
    gui_menulist_set_back2parent(alarm1_exit_callback, &alarm_set_flag);
    return (app_result_e) RESULT_MENU_SON;
}
app_result_e fmrec_menu_entry(void *menu_title_void)
{
    gui_menulist_set_back2parent(fmrec_exit_callback, &fmrec_set_flag);
    return (app_result_e) RESULT_MENU_SON;
}
/*
 *****************************************************************************
 * \par  Description:
 *  普通说明     : 本回调函数用于退出 alarm/alarm_fm 设置
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：
 * \note: 此处退出认为是保存退出，其他方式退出均不保存设置
 *
 ******************************************************************************
 */
void alarm1_exit_callback(void *menu_title_void)
{
    g_alarm_vars.g_alarm.alarm_count = 0; //重设闹铃后清Snooze标志
    set_timer_msg(&g_alarm_vars.g_aufm, &g_alarm_vars.g_alarm, &g_alarm_vars.timer_flag);
    sys_vm_write(&g_alarm_vars, VM_ALARM);
    //return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}
void fmrec_exit_callback(void *menu_title_void)
{
    g_alarm_vars.g_alarm.alarm_count = 0; //重设闹铃后清Snooze标志
    set_timer_msg(&g_alarm_vars.g_aufm, &g_alarm_vars.g_alarm, &g_alarm_vars.timer_flag);
    sys_vm_write(&g_alarm_vars, VM_ALARM);
    //return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/******************************************************************************/
/*!
 * \par  Description:
 *    本回调函数用于取得 设置alarm1铃声 的函数入口
 * \param[in]    void
 * \param[out]   none
 * \return       RESULT_MENU_SON
 * \retval       进入下一级
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
app_result_e set_alarm1_ring_callback(void * title_action)
{
    menu_title_action_t* menu_title_shut_down;
    menu_title_shut_down = (menu_title_action_t*) title_action;
    if(g_alarm_vars.g_alarm.ring_flag == 0)
    {
        menu_title_shut_down->str_id = S_RING1_SD;
    }
    else if(g_alarm_vars.g_alarm.ring_flag == 1)
    {
        menu_title_shut_down->str_id = S_RING1_DISK;
    }
    else if(g_alarm_vars.g_alarm.ring_flag == 2)
    {
        menu_title_shut_down->str_id = S_RING1_CARD;
    }
    else
    {
    }
    menu_title_shut_down->menu_entry = -1; //更新入口
    menu_title_shut_down->ret_str = NULL;
    menu_title_shut_down->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

app_result_e alarm1_ring_sd_callback(void *menu_title_void)
{
    g_alarm_vars.g_alarm.ring_flag = 0;
    
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}
app_result_e alarm1_ring_card_callback(void *menu_title_void)
{
    return alarm_ring_callback(2);
}
app_result_e alarm1_ring_flash_callback(void *menu_title_void)
{
    return alarm_ring_callback(1);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    调用directory选取file
 * \param[in]    uint8 disk_flag
 * \param[out]   none
 * \return       app_result_e result
 * \retval       none
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
app_result_e alarm_ring_callback(uint8 disk_flag)
{
    app_result_e result;
    dir_com_data_t dir_com;
    fsel_param_t fsel_param;
    uint16 str_id;
    /*! \brief browser 挂载文件系统的mount ID*/
    int32 browser_mount;
    
    //更改盘符
    if(disk_flag == 1)
    {
        fsel_param.disk = DISK_C;
    }
    else if(disk_flag == 2)
    {
        //重新检测卡是否存在
        if (sys_detect_disk(DRV_GROUP_STG_CARD) == -1)
        {
            //卡已拔出
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_CARD_OUT);
            return RESULT_REDRAW;
        }
        fsel_param.disk = DISK_H;
    }
    else
    {
        return RESULT_ERROR;
    }
        
    if(browser_filelist_init(fsel_param.disk) != RESULT_NULL)
    {
        //初始化location为无效location
        libc_memset(&g_alarm_vars.g_alarm.path.file_path, 0, sizeof(musfile_location_u));
    }

    //初始化文件浏览器
    dir_com.list_option_func = NULL;
    dir_com.menulist = NULL;
    dir_com.menulist_cnt = 0;
    //dir_com.root_layer = 0;
    dir_com.browse_mode = 0; //从根目录开始浏览
    //初始化文件选择器参数
    fsel_param.fsel_type = FSEL_TYPE_COMMONDIR;
    fsel_param.fsel_mode = FSEL_MODE_BROWSER;
    fsel_param.brow_type = FSEL_BROW_ALL;
    fsel_param.file_type_bit = MUSIC_BITMAP;
    //初始化文件选择器
    browser_mount = fsel_init(&fsel_param, 0);
    if(browser_mount == -1)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        return RESULT_REDRAW;
    }
    if(fsel_param.disk == DISK_H)
    {
        g_dir_flag = TRUE;
    }

    result = gui_directory(DIRLIST, &g_alarm_vars.g_alarm.path, &dir_com);
    
    if(g_dir_flag == TRUE)
    {
        g_dir_flag = FALSE;
    }
    if (result != 0)
    {
        if (result == RESULT_MUSIC_PLAY)
        {
            g_alarm_vars.g_alarm.ring_flag = disk_flag;
            g_alarm_vars.g_alarm.path.file_path.dirlocation.disk = fsel_param.disk;
            result = RESULT_MENU_PARENT;
        }
        else if (result == RESULT_DIR_ERROR_NO_FILE) //无文件
        {
            if (fsel_param.disk == DISK_C)
            {
                str_id = S_DISK_NOFILE;
            }
            else
            {
                str_id = S_CARD_NOFILE;
            }
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, str_id);
            result = RESULT_REDRAW;
        }
        else if (result == RESULT_DIR_ERROR_ENTER_DIR) //进入目录出错
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR); //出错信息
            result = RESULT_REDRAW;
        }
        else
        {
            //返回热键结果
        }
    }
    //文件选择器退出
    fsel_exit();
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    通知后台music引擎退出
 * \param[in]    void
 * \param[out]   none
 * \return       none
 * \retval       none
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
void music_close_engine(void)
{
    msg_apps_t msg;
    //向process manager发消息要求后台退出
    msg.type = MSG_KILL_APP_SYNC;

    msg.content.data[0] = APP_ID_MENGINE;

    send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  browser 获取后台引擎盘符
 * \param[in]    none
 * \param[out]   none
 * \return       disk_type
 * \ingroup      menu_callback_alarm_tools.c
 * \note
 *******************************************************************************/
uint8 music_get_disk_type(void)
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
 *	  browser 初始化browser_fsel
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      menu_callback_alarm_tools.c
 * \note         检测是否有后台music播放
 * 				 检测盘符与后台是否一致
 *******************************************************************************/
app_result_e browser_filelist_init(uint8 diskflag)
{
    app_result_e result = RESULT_NULL;
    uint8 fsel_disk;
    //查看engine是否存在
    if (get_engine_type() == ENGINE_MUSIC)
    {
        //获取后台盘符
        fsel_disk = music_get_disk_type();
        if (fsel_disk != diskflag)
        {
            //关闭音乐引擎
            music_close_engine();
            result = RESULT_REDRAW;
        }
    }

    return result;
}

