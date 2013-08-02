/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : menu_callback_others.C
 * History:
 *      <johnsen>    <2011-9-25 20:43>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_setting.h"
#include "menu_callback_others.h"
#include "sys_info.h"

const uint8 legal_file[] = "legal.txt";
const uint8 music_type_file[] = "m_type.txt";

//一级菜单项-定时关机
//定时关机(关闭)|-关
//              |-关机设定
//定时关机(开启)|-关
//              |-关机设定
////////////////////
//一级菜单项-电脑连接首选项
//电脑连接首选项|-MSC
//              |-MTP
//              |-什么是MSC
//              |-什么是MTP
////////////////////
//一级菜单项-信息
//信息|-播放器信息
//    |-磁盘空间信息
//    |-支持文件|-音乐
//              |-图片
//              |-视频
////////////////////
//一级菜单项-法律信息
//法律信息-直接显示法律信息


/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得定时关机的菜单入口（由于开启和关闭定时关机
 界面不一致）
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回菜单入口
 * \retval       ：进入下一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_shut_down_entry(void * title_action)
{
    uint16 next_entry;
    string_desc_t string_desc;
    string_desc_t string_name;
    uint8 temp_string[5] =
    { 0 };

    menu_title_action_t* menu_title_shut_down;
    menu_title_shut_down = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.sleep_timer == VALUE_SHUT_DOWN_TIMER_OFF)
    {
        next_entry = MENU_ENTRY_ID_SHUT_DOWN_OFF;
        menu_title_shut_down->str_id = S_OFF;
        menu_title_shut_down->ret_str = NULL;
        menu_title_shut_down->source_id = -1;
        menu_title_shut_down->style_id = MENULIST;
    }
    else
    {

        string_desc.data.str = string_buffer;
        string_desc.length = TIPS_BUFFER_SIZE;
        string_desc.argv = S_REMAIN_TIME;
        string_desc.language = (int8) g_setting_vars.g_comval.language_id;

        libc_itoa((uint32)(g_setting_vars.g_comval.sleep_timer - g_sys_counter.sleep_counter / 120),
                &temp_string[1], 2); //将时间转成字符
        temp_string[0] = '[';
        temp_string[3] = ']';
        temp_string[4] = 0;
        string_name.data.str = temp_string;
        string_name.length = 5;
        string_name.language = (int8) g_setting_vars.g_comval.language_id;

        com_string_format(&string_desc, &string_name);

        next_entry = MENU_ENTRY_ID_SHUT_DOWN_ON; //更新入口
        menu_title_shut_down->str_id = S_SHUT_DOWN_SETTING;
        menu_title_shut_down->ret_str = string_buffer;
        menu_title_shut_down->source_id = -1;
        menu_title_shut_down->style_id = MENULIST_TITLE;
    }
    menu_title_shut_down->menu_entry = next_entry; //更新入口
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于关闭定时关机
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e shut_down_timer_off_callback(void *menu_title_void)
{
    menu_title_action_t* menu_title_shut_down_timer;
    g_setting_vars.g_comval.sleep_timer = VALUE_SHUT_DOWN_TIMER_OFF;

    menu_title_shut_down_timer = (menu_title_action_t*) menu_title_void;
    menu_title_shut_down_timer->str_id = S_OFF;

    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于调整关机时间的最小值
 *      重点说明：由于调节参数值区域较大超过了像素点，为了能形成倍数关系，做出调整
 *      99/33=3 即以一个刻度表示3个数值，数值变化3更新一个刻度
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
adjust_result_e adjust_min_time(parambox_one_t *params, uint8 active, bool inc)
{
    if (params->value == 0)
    {
        params->value = 1;
    }
    return ADJUST_RESULT_VALUE;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于调整定时关机时间
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e shut_down_time_adjust_callback(void *menu_title_void)
{
    app_result_e return_result_temp;

    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    menu_title_action_t* menu_title_shut_down_timer;
    uint8 sleep_timer = g_setting_vars.g_comval.sleep_timer;

    if (sleep_timer == 0)
    {
        sleep_timer = 1;
    }
    s_param_item.min = VALUE_SHUT_DOWN_TIME_MIN - 1;
    s_param_item.max = VALUE_SHUT_DOWN_TIME_MAX;
    s_param_item.step = VALUE_SHUT_DOWN_TIME_STEP;
    s_param_item.value = (int16) sleep_timer; //value以分钟为单位
    s_param_item.cycle = FALSE;
    s_param_item.adjust_func = adjust_min_time;
    s_param_item.value_str = NULL;
    s_param_item.callback = NULL;
    s_param_item.unit_id = S_UINT_MIN;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;
    s_param_private.scale = 2;

    s_param_com.private_data = &s_param_private;

    return_result_temp = gui_set_parameter(SLIDER_NUM, &s_param_com);

    if(return_result_temp== RESULT_CONFIRM)
    {
        g_setting_vars.g_comval.sleep_timer = (uint8) s_param_com.private_data->items->value; //value以分钟为单位

        string_desc.data.id = S_SET_SHUT_DOWN_TIMER_END;
        string_desc.language = UNICODEID;

        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
        menu_title_shut_down_timer = (menu_title_action_t*) menu_title_void;
        menu_title_shut_down_timer->str_id = S_SHUT_DOWN_SETTING;
        menu_title_shut_down_timer->source_id = -1;
        menu_title_shut_down_timer->style_id = MENULIST_TITLE;

        return (app_result_e) RESULT_MENU_CUR; //重新进入当前级，可能有特殊处理
    }
    else
    {
        return return_result_temp;
    }

}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得PC连接首选项的string ID
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回PC连接首选项string id
 * \retval       ：进入下一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_pc_connect_information_id_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_pc_connect_information;
    menu_title_pc_connect_information = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.online_device == VALUE_STORAGE_MSC)
    {
        active_id = S_STORAGE_MSC;
    }
    else
    {
        active_id = S_STORAGE_MTP;
    }
    menu_title_pc_connect_information->menu_entry = -1; //更新入口
    menu_title_pc_connect_information->str_id = active_id;
    menu_title_pc_connect_information->ret_str = NULL;
    menu_title_pc_connect_information->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置PC连接首选项为MSC
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_storage_msc_callback(void *menu_title_void)
{

    menu_title_action_t* menu_title_storage_msc;

    g_setting_vars.g_comval.online_device = VALUE_STORAGE_MSC;

    menu_title_storage_msc = (menu_title_action_t*) menu_title_void;
    menu_title_storage_msc->str_id = S_PC_CONNECT_OPTION;
    return (app_result_e) RESULT_MENU_PARENT;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于设置PC连接首选项为MTP
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e set_storage_mtp_callback(void *menu_title_void)
{
    menu_title_action_t* menu_title_storage_mtp;

    g_setting_vars.g_comval.online_device = VALUE_STORAGE_MTP;

    menu_title_storage_mtp = (menu_title_action_t*) menu_title_void;
    menu_title_storage_mtp->str_id = S_PC_CONNECT_OPTION;
    return (app_result_e) RESULT_MENU_PARENT;

}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于解释什么是MSC
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e explain_msc_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    string_desc.data.id = S_NOTE_MSC;
    string_desc.language = UNICODEID;

    dialog_com.dialog_type = DIALOG_BUTTON_ANYKEY;  //也可考虑DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于解释什么是MTP
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e explain_mtp_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    string_desc_t string_name;
    uint8 *temp_string = "TOOLS";
    dialog_com_data_t dialog_com;

    string_desc.data.str = string_buffer;
    string_desc.length = TIPS_BUFFER_SIZE;
    string_desc.argv = S_NOTE_MTP;
    string_desc.language = (int8) g_setting_vars.g_comval.language_id;

    string_name.data.str = temp_string;
    string_name.length = 5;
    string_name.language = (int8) g_setting_vars.g_comval.language_id;

    com_string_format(&string_desc, &string_name); //将“tools”整合到S_NOTE_MTP中


    dialog_com.dialog_type = DIALOG_BUTTON_ANYKEY;//也可考虑DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于显示播放器信息
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e show_player_information_callback(void *menu_title_void)
{

    string_desc_t string_desc;
    string_desc_t string_name[3];
    uint8 software_version[FW_INFO_VERSION_LEN+2];
    uint8 *player_model = "US212A";
    uint8 *p_software_version;
    uint8 *support_website = "www.actions-semi.com";
    dialog_com_data_t dialog_com;

    software_version[0] = 'V';
//    software_version[1] = 'e';
//    software_version[2] = 'r';
    software_version[FW_INFO_VERSION_LEN+1] = 0;
    sys_get_fw_info ((uint8 *)&software_version[1], FW_INFO_VERSION_ADDR, FW_INFO_VERSION_LEN); //读取版本号
    p_software_version = software_version;

    string_desc.data.str = string_buffer;
    string_desc.length = TIPS_BUFFER_SIZE;
    string_desc.argv = S_NOTE_PLAYER;
    string_desc.language = (int8) g_setting_vars.g_comval.language_id;

    string_name[0].data.str = player_model;
    string_name[0].length = 6;
    string_name[0].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[1].data.str = p_software_version;
    string_name[1].length = 11;
    string_name[1].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[2].data.str = support_website;
    string_name[2].length = 20;
    string_name[2].language = (int8) g_setting_vars.g_comval.language_id;
    com_string_format(&string_desc, string_name); //将player 信息整合到S_NOTE_PLAYER中

    dialog_com.dialog_type = DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于显示磁盘空间信息，需要区分带卡和无卡
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e show_disk_information_callback(void *menu_title_void)
{
    uint32 usabel_space_sec;
    uint32 total_space_sec;
    uint8 usable_space[6] =
    { 0 };
    uint8 total_space[6] =
    { 0 };
    uint8 total_musics[6] =
    { 0 };
    uint8 total_pictures[6] =
    { 0 };
    uint8 total_vedios[6] =
    { 0 };
    string_desc_t string_desc;
    string_desc_t string_name[5];
    dialog_com_data_t dialog_com;
    uint8 count = 0;
    uint16 g_ap_file_num[MAX_APP_NUM];

    if (g_vfs_mount_id == -1)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_DISK_ERROR);
        return RESULT_REDRAW;
    }

    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING);


    sys_vm_read(&g_ap_file_num, VM_FILE_NUM, sizeof(g_ap_file_num));

    vfs_get_space((void*) g_vfs_mount_id, (void*) &usabel_space_sec, GET_SPARE_SPACE);
    vfs_get_space((void*) g_vfs_mount_id, (void*) &total_space_sec, GET_TOTAL_SPACE);

    usabel_space_sec = usabel_space_sec / 2048; //sector change to Mbytes
    total_space_sec = total_space_sec / 2048; //sector change to Mbytes

    libc_itoa((uint32) usabel_space_sec, usable_space, count); //将可用空间转成字符串
    libc_itoa((uint32) total_space_sec, total_space, count); //将总空间转成字符串
    libc_itoa((uint32) g_ap_file_num[APP_ID_MUSIC], total_musics, count); //将音乐总数转成字符串
    libc_itoa((uint32) g_ap_file_num[APP_ID_PICTURE], total_pictures, count); //将图片总数转成字符串
    libc_itoa((uint32) g_ap_file_num[APP_ID_VIDEO], total_vedios, count); //将视频总数转成字符串

    string_desc.data.str = string_buffer;
    string_desc.length = TIPS_BUFFER_SIZE;
    //可用空间：[%s] MB
    //总空间：[%s] MB
    //歌曲：[%s]
    //图片：[%s]
    //视频：[%s]
    string_desc.argv = S_DISK_INFO_DETAIL;

    string_desc.language = (int8) g_setting_vars.g_comval.language_id;

    string_name[0].data.str = usable_space; //可用空间
    string_name[0].length = 6;
    string_name[0].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[1].data.str = total_space; //总空间
    string_name[1].length = 6;
    string_name[1].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[2].data.str = total_musics; //歌曲总数
    string_name[2].length = 6;
    string_name[2].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[3].data.str = total_pictures; //图片总数
    string_name[3].length = 6;
    string_name[3].language = (int8) g_setting_vars.g_comval.language_id;

    string_name[4].data.str = total_vedios; //视频总数
    string_name[4].length = 6;
    string_name[4].language = (int8) g_setting_vars.g_comval.language_id;
    com_string_format(&string_desc, string_name); //将磁盘信息整合到S_DISK_INFO_DETAIL中

    dialog_com.dialog_type = DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);
    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于显示音乐所支持类型信息
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e show_music_type_callback(void *menu_title_void)
{
    uint8 music_type[12];
    app_result_e result;

    libc_memcpy(music_type, music_type_file, 12);
    result = gui_text_read(TEXTREAD_BG, music_type);

    return result; //正常情况下返回 RESULT_REDRAW，重绘当前级菜单
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于显示图片所支持类型信息
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e show_picture_type_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    string_desc.data.id = S_NOTE_PICTURE;
    string_desc.language = UNICODEID;

    dialog_com.dialog_type = DIALOG_BUTTON_ANYKEY; //也可考虑DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于显示视频所支持类型信息
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e show_video_type_callback(void *menu_title_void)
{
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    string_desc.data.id = S_NOTE_VEDIO;
    string_desc.language = UNICODEID;

    dialog_com.dialog_type = DIALOG_BUTTON_ANYKEY; //也可考虑DIALOG_INFOR_WAIT;
    dialog_com.button_type = BUTTON_NO_BUTTON;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    gui_dialog(DIALOG_MSG, &dialog_com);

    return RESULT_REDRAW;
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于显示法律信息
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e show_legal_information_callback(void *menu_title_void)
{
    uint8 legal_name[12];
    app_result_e result;

    libc_memcpy(legal_name, legal_file, 12);
    result = gui_text_read(TEXTREAD_BG, legal_name);

    return result; //正常情况下返回 RESULT_REDRAW，重绘当前级菜单
}

