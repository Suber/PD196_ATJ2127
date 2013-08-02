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
#include "menu_callback_others_2.h"
#include "menu_callback_play_mode.h"

#if 0
static const int8 user_srs_param_default_1[MAX_EQ_SEG] =
{ 8, 8, 5, 4, 6, 1, 0, 30, 1, 0, 0, 0 };//srs user default

static const int8 user_eq_param_default_1[MAX_EQ_SEG] =
{ -9, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }; //user eq default
#endif

static const int8 fullsound_param_1[MAX_EQ_SEG] =
{ -9, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0 }; //fullsound on

static const int8 srs_param_default_1[MAX_EQ_SEG] =
{ 8, 8, 5, 4, 6, 1, 0, 30, 1, 0, 0, 0 };//srs default

static const int8 eq_param_2[8][MAX_EQ_SEG] =
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
////////////////////
//一级菜单项-格式化设备
//格式化设备（带卡）|-格式化主盘
//                  |-格式化卡盘
//格式化设备（不带卡）|-否
//                    |-是
////////////////////
//一级菜单项-出厂设置
//出厂设定|-是
//        |-否
////////////////////
//一级菜单项-CD安装程序
//CD安装程序|-是
//          |-否
//////////////////////


/******************************************************************************/
/*!
 * \par  Description:
 * \bool music_set_eq_2(mmm_mp_eq_info_t *eq_val)
 * \设置eq信息
 * \param[in]    void  para1
 * \param[out]   none
 * \return       bool the result
 * \retval           1 sucess
 * \retval           0 failed
 * \ingroup      menu_callback_others_2.c
 * \note
 */
/*******************************************************************************/
static bool music_set_eq_2(mmm_mp_eq_info_t *eq_val)
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
 * 创建应用
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      config
 * \note
 *******************************************************************************/
bool _config_format_disk(void)
{
	bool ret_val;
    int32 file_sys_id;
    uint8 volume_label[28];
    libc_memset(volume_label, 0, sizeof(volume_label));
    file_sys_id = sys_mount_fs(DRV_GROUP_STG_BASE, DISK_C, 0);
    if (file_sys_id == -1)
    {
        return FALSE;
    }
    com_get_config_struct(INF_UDISK_LABEL, volume_label, 11); //CMD_Inquiry_information_FOB
    com_ansi_to_unicode(volume_label);
    ret_val = vfs_create_volume(file_sys_id, volume_label);
    return TRUE;
}
/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本函数用于格式化nand设备,是一个过程函数
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：无
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
void deal_nand_format(void)
{
    uint16 g_ap_file_num[MAX_APP_NUM];
    uint16 i;   
    fsel_param_t init_param;//文件选择器初始化参数
    msg_apps_t msg;
    
//显示“请稍候”
    gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING);
    
//如果后台正在播放music，需要关闭    
    if (ENGINE_MUSIC == music_exist_flag)
    {
        //消息类型(即消息名称)杀死后台进程
        msg.type = MSG_KILL_APP_SYNC;
        msg.content.data[0] = APP_ID_MENGINE;
        //发送同步消息
        send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
        music_exist_flag = ENGINE_NULL;
    }
    if(g_vfs_mount_id != -1)
    {
        fsel_exit();
    }
//格式化nand 盘    
    if (-1 == sys_drv_install(DRV_GROUP_STG_BASE, 0, "nand.drv"))
    {
        while (1)
        {
        }
    }            
    sys_format_disk(STG_BASE_NAND_NOR, DISK_C, FORMAT_FAT32);
    _config_format_disk();

    if (-1 == sys_drv_uninstall(DRV_GROUP_STG_BASE))
    {
        while (1)
        {
        }
    }         
//更新磁盘信息 
    sys_vm_read(&g_ap_file_num, VM_FILE_NUM, sizeof(g_ap_file_num));   
    for(i=0;i<MAX_APP_NUM;i++)
    {
        if(i==APP_ID_RADIO)
        {
            continue; 
        }
        else
        {    
            g_ap_file_num[i] = 0;
        }     
    }
    
    sys_vm_write(&g_ap_file_num, VM_FILE_NUM);


    init_param.disk = DISK_C;
    init_param.fsel_type = FSEL_TYPE_COMMONDIR;
    init_param.fsel_mode = (g_setting_vars.music_comval.music_repeat_mode | g_setting_vars.music_comval.music_shuffle);
    init_param.file_type_bit = MUSIC_BITMAP;
    g_vfs_mount_id = (uint16) fsel_init(&init_param, 0);
    
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得格式化设备的菜单入口（区分带卡和不带卡）
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 返回菜单入口
 * \retval       ：进入下一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_format_device_entry(void * title_action)
{
    app_result_e result;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;

    if (sys_detect_disk(DRV_GROUP_STG_CARD) == 0) //0表示有卡插入 -1表示卡未插入
    {
        return (app_result_e)RESULT_MENU_SON; //进入下级菜单
    }
    else
    {
        // 无卡，询问后决定是否进行格式化 
        dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
        dialog_com.button_type = BUTTON_YESNO;
        
        string_desc.data.id = S_DELETE_ALL_OR_NOT;
        string_desc.language = UNICODEID;
        
        dialog_com.string_desc = &string_desc;
        dialog_com.active_default = VALUE_BUTTON_YESNO_NO;
        result = gui_dialog(DIALOG_ASK, &dialog_com);
        
        if (result == RESULT_DIALOG_YES)
        {
            deal_nand_format();
            return (app_result_e) RESULT_MENU_CUR;  //返回当前菜单

        }
        else
        {       
            return result;  //返回当前菜单
        }
    }
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于格式化主盘
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e format_disk_callback(void *menu_title_void)
{
    //格式化主盘
    app_result_e result;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    
    dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
    dialog_com.button_type = BUTTON_YESNO;
    
    string_desc.data.id = S_DELETE_ALL_OR_NOT;
    string_desc.language = UNICODEID;
    
    dialog_com.string_desc = &string_desc;
    dialog_com.active_default = VALUE_BUTTON_YESNO_NO;
    result = gui_dialog(DIALOG_ASK, &dialog_com);
    
    if (result == RESULT_DIALOG_YES)
    {
        deal_nand_format();
        return (app_result_e) RESULT_MENU_PARENT; //返回上级菜单
    }
    else
    {
        return result;
    }   
                
    
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于格式化卡盘
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e format_card_callback(void *menu_title_void)
{
    //格式化卡盘
    app_result_e result;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    fsel_param_t init_param;//文件选择器初始化参数
    msg_apps_t msg;
    uint8 card_out_flag = 0;
    
    dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
    dialog_com.button_type = BUTTON_YESNO;
    
    string_desc.data.id = S_DELETE_ALL_OR_NOT;
    string_desc.language = UNICODEID;
    
    dialog_com.string_desc = &string_desc;
    dialog_com.active_default = VALUE_BUTTON_YESNO_NO;
    result = gui_dialog(DIALOG_ASK, &dialog_com);
    
    if (result == RESULT_DIALOG_YES)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_IMMEDIATE, S_WAITING); 

//如果后台正在播放music，需要关闭    
        if (ENGINE_MUSIC == music_exist_flag)
        {
            //消息类型(即消息名称)杀死后台进程
            msg.type = MSG_KILL_APP_SYNC;
            msg.content.data[0] = APP_ID_MENGINE;
            //发送同步消息
            send_sync_msg(APP_ID_MANAGER, &msg, NULL, 0);
            music_exist_flag = ENGINE_NULL;
        }

        if (g_vfs_mount_id != -1)
        {
            fsel_exit();
        }
       
        //直接安装base驱动,不用跟VFS打交道
        if (-1 == sys_drv_install(DRV_GROUP_STG_CARD, 0, "card.drv"))
        {
            card_out_flag = 1;
        }
        else
        {                      
            sys_format_disk(STG_CARD, DISK_H, FORMAT_FAT32);
        }
        if (-1 == sys_drv_uninstall(DRV_GROUP_STG_CARD))
        {
            card_out_flag = 1;
        }           
        if ((g_vfs_mount_id != -1)||(card_out_flag == 1))
        {
            init_param.disk = DISK_C;
            init_param.fsel_type = FSEL_TYPE_COMMONDIR;
            init_param.fsel_mode = (g_setting_vars.music_comval.music_repeat_mode      
            | g_setting_vars.music_comval.music_shuffle);
            init_param.file_type_bit = MUSIC_BITMAP;
            g_vfs_mount_id = (uint16) fsel_init(&init_param, 0); 
        }
        return (app_result_e) RESULT_MENU_PARENT; //返回上级菜单    
    }
    else
    {
        return result;
    }   
                
        
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于返回上一级
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e dont_format_callback(void *menu_title_void)
{
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于询问是否恢复出厂设置
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：刷新当前级菜单
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e dialog_restore_factory_setting_callback(void *menu_title_void)
{
    app_result_e result;
    string_desc_t string_desc;
    dialog_com_data_t dialog_com;
    menu_title_action_t* menu_title_restore;
    mmm_mp_eq_info_t eq_info;
    
    dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
    dialog_com.button_type = BUTTON_YESNO;

    string_desc.data.id = S_RESTORE_OR_NOT;
    string_desc.language = UNICODEID;

    dialog_com.string_desc = &string_desc;
    dialog_com.active_default = VALUE_BUTTON_YESNO_NO;
    result = gui_dialog(DIALOG_ASK, &dialog_com);

    if (result == RESULT_DIALOG_YES)
    {
        com_get_default_setting(&g_setting_vars);
        
        //libc_memcpy(g_setting_vars.music_comval.eq_parameter,user_eq_param_default_1, MAX_EQ_SEG);
        //libc_memcpy(g_setting_vars.music_comval.srs_parameter,user_srs_param_default_1, MAX_EQ_SEG);
        
              
        if(music_exist_flag == ENGINE_MUSIC)    
        {   
            restore_factory_setting_for_music(); //恢复循环方式、shuffle方式、播放路径等
            //向引擎发送eq消息 
            if((g_setting_vars.music_comval.srs==VALUE_SRS_OFF)
                &&(g_setting_vars.music_comval.fullsound == VALUE_FULLSOUND_OFF))
            {
                eq_info.eq_type = COMMAND_EQ_NORMAL;
                libc_memcpy(eq_info.eq_para, eq_param_2[g_setting_vars.music_comval.eq_setting], MAX_EQ_SEG);
            }
            else if(g_setting_vars.music_comval.fullsound != VALUE_FULLSOUND_OFF)
            {
                eq_info.eq_type = COMMAND_FULLSOUND;
                libc_memcpy(eq_info.eq_para, fullsound_param_1, MAX_EQ_SEG);
            }
            else
            {
                eq_info.eq_type = COMMAND_SRS;
                libc_memcpy(eq_info.eq_para, srs_param_default_1, MAX_EQ_SEG);
            }            
            music_set_eq_2(&eq_info);
        }
        //恢复音量设置
        if(g_setting_vars.g_comval.volume_current>g_setting_vars.g_comval.volume_limit)
        {
            com_set_sound_volume(g_setting_vars.g_comval.volume_limit); 
        }
        else
        {
            com_set_sound_volume(g_setting_vars.g_comval.volume_current); 
        }
        
        //设置亮度
        com_set_contrast(g_setting_vars.g_comval.lightness);
        if(g_setting_vars.g_comval.key_tone == VALUE_KEY_TONE_ON) 
        {    
            com_set_sound_keytone(VALUE_KEY_TONE_ON);//使能按键音
        }
        else
        {
            com_set_sound_keytone(VALUE_KEY_TONE_OFF);//禁止按键音
        }        
        ui_set_language((uint32) g_setting_vars.g_comval.language_id);  //语言更新
        menu_title_restore = (menu_title_action_t*) menu_title_void;
        menu_title_restore->str_id = S_FACTORY_SETTING;

        string_desc.data.id = S_RESTORED;
        string_desc.language = UNICODEID;
        
        dialog_com.dialog_type = DIALOG_INFOR_WAIT;
        dialog_com.button_type = BUTTON_NO_BUTTON;
        dialog_com.icon_id = V_U16_INVALID;
        dialog_com.string_desc = &string_desc;
        gui_dialog(DIALOG_MSG, &dialog_com);
        return (app_result_e) RESULT_MENU_CUR; //刷新当前级菜单    
    }
    else
    {
        return result;
    }
            
}



/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于取得autorun  设置的string ID
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : autorun  设置项string id
 * \retval       ：进入下一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e get_autorun_setting_callback(void * title_action)
{
    uint16 active_id;
    menu_title_action_t* menu_title_autorun_setting;
    menu_title_autorun_setting = (menu_title_action_t*) title_action;
    if (g_setting_vars.g_comval.autorun_set == VALUE_DISABLE_AUTORUN)
    {
        active_id = S_DISABLE;
    }
    else
    {
        active_id = S_ENABLE;
    }
    menu_title_autorun_setting->menu_entry = -1; 
    menu_title_autorun_setting->str_id = active_id;
    menu_title_autorun_setting->ret_str = NULL;
    menu_title_autorun_setting->source_id = -1;
    return (app_result_e) RESULT_MENU_SON; //进入下一级
}


/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于不使能自动运行
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e disable_autorun_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.autorun_set = VALUE_DISABLE_AUTORUN;
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}

/*
 *****************************************************************************
 * \par  Description:
 *      普通说明:本回调函数用于使能自动运行
 *      重点说明
 * \param[in]    ：无
 * \param[out]   : 无
 * \retval       ：返回上一级
 * \note: 注意事项说明
 *
 ******************************************************************************
 */
app_result_e enable_autorun_callback(void *menu_title_void)
{
    g_setting_vars.g_comval.autorun_set = VALUE_ENABLE_AUTORUN;
    return (app_result_e) RESULT_MENU_PARENT; //返回上一级
}


