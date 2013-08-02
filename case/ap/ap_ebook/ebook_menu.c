/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_main.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo       2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, 本应用的头文件 */
#include "ebook.h"

/********************************************************************************
 * Description :电子书菜单选择处理函数
 *
 * Arguments  :
 *                  enter_mode:当前场景类型(参照ebook.h中的scene_enter_mode_e)
 * Returns     :
 *                  场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _ebook_option_menu(scene_enter_mode_e mode)
{
    app_result_e result = RESULT_NULL;
    uint8 card_flag, reading_flag = 0;                          //card_flag:1表示存在卡，0表示卡不存在
    menu_com_data_t op_menu;
    engine_state_e play_state = get_engine_state();

    /*初始化菜单控件*/
    card_flag = _detect_device(DRV_GROUP_STG_CARD);             //检测磁盘设备是否存在
    // temp_type = enter_type;

    //打开电子书后进入菜单的情况
    if (mode == ENTER_FROM_READING)
    {
        reading_flag = 2;
    }
    op_menu.menu_entry = (uint8) (M_NOPLAYNOREAD_NOCARD + card_flag + play_state * 4 + reading_flag);
    op_menu.app_id = APP_ID_EBOOK;
    op_menu.path_id = 0;

    /*定义不同模式下的入口菜单*/
    op_menu.menu_func = NULL;

    /*处理菜单私有消息*/
    result = gui_menulist(MENULIST_OPTION, &op_menu);           //这里显示菜单，并选择

    if ((result == RESULT_REDRAW) || (result == RESULT_CANCEL))
                                                                //重绘UI返回，通常返回后需要重新绘制当前UI，
                                                                //但不需要退出当前消息循环
                                                                //或者 取消某个事件或操作返回
    {
        if (mode == ENTER_FROM_READING)
        {
            result = RESULT_EBK_PLAY;
            enter_mode = ENTER_FROM_READINGMENU;
            is_reading_menu = FALSE;
        }
        else if (mode == ENTER_FROM_FILELIST)
        {
            result = RESULT_EBK_FILELIST;
            enter_mode = ENTER_FROM_DESKTOP;
        }
        else
        {
        }
    }
    else if ((result == RESULT_CONFIRM) && (enter_mode == ENTER_FROM_READINGMENU))
                                                                //确认某个事件或操作返回
                                                                //且 从阅读设置菜单进入场景
    {
        result = RESULT_EBK_PLAY;
    }
    else
    {

    }

    return result;
}

/********************************************************************************
 * Description :正在播放菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                  场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_nowplaying(void *param)
{
    return RESULT_NOWPLAYING;
}

/********************************************************************************
 * Description :上一次播放菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_lastplay(void *param)
{
    return RESULT_LASTPLAY;
}

/********************************************************************************
 * Description :删除电子书菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/
app_result_e _menu_func_deleteebook(void *param)
{
    uint32 file_handle;
    uint16 filename_length;
    app_result_e result;
    del_com_data_t del_com;


    //先保存书签
    if (bmk_modify_handle != 0)
    {
        _writeback_bmk_info();
    }

    if (view_file.file_handle != 0)
    {
        if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_C)
        {
            _vram_wb_bmk_headinfo();
        }
    }

    bookmark_total = 0xFF;      //标记下次重新读取书签


    fsel_browser_set_file(&g_ebook_vars.path.file_path.dirlocation, FSEL_TYPE_LISTEBOOK);
                                                    //定位到文件在磁盘的目录项位置
    filename_length = vfs_get_name(g_ebook_mount_id, BOOKMARK_filename, MAX_FILENAME_LENGTH / 2);

    del_com.filename = BOOKMARK_filename;           //书签文件名缓存空间
    del_com.del_mode = DEL_MODE_FILE;               //删除模式为:删除单个文件
    del_com.del_self = TRUE;
    del_com.del_func = _close_text_handle;          //delete 对话框中选择“是”后执行的函数，为NULL表示无需执行
    result = gui_delete(&g_ebook_vars.path, &del_com);
                                                    //删除文件或目录控件控制流实现。
    if (result == RESULT_DELETE_FILE)
    {
        if (_get_bmk_name(filename_length) != 0)
        {
            file_handle = vfs_file_dir_exist(g_ebook_mount_id, BOOKMARK_filename, FS_FILE);
            if (file_handle != (uint32) NULL)
            {
                vfs_file_dir_remove(g_ebook_mount_id, BOOKMARK_filename, FS_FILE);
            }
        }
        //需要判断是在播放列表删掉还是播放界面删掉 RESULT_EBK_PLAY
        if(enter_mode == ENTER_FROM_READING)
        {

            result =  RESULT_EBK_PLAY;         //切换到下一个电子书播放
        }
        else
        {
            result = RESULT_EBK_FILELIST;
        }
    }
    else if( (result == RESULT_REDRAW)  || (result == RESULT_ERROR))
    {
        //result = RESULT_EBK_FILELIST;     //跳到文件播放列表界面
        result =  RESULT_MENU_EXIT;         //跳回原来的界面
    }
    else
    {
        //有可能是任务退出消息
        //QAC
    }
    return result;

}
/********************************************************************************
 * Description :书签选择菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_selbookmark(void *param)
{
    return _bmk_list_enter(NULL, BMK_SEL);
}
/********************************************************************************
 * Description :删除书签菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_detbookmark(void *param)
{
    return _bmk_list_enter(NULL, BMK_DET);
}
/********************************************************************************
 * Description :添加书签菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_addbookmark(void *param)
{
    app_result_e retval;
    uint8 filename[13];
    uint8 filter_count;
    uint8 cpy_length;
    _get_bookmark_nums(BMK_DET);                            //获取书签总数

    //过滤回车换行 英文空格
    filter_count = _filterwhitespace(BOOKMARK_filename, view_file.language);
    libc_memset(filename, 0x0, 13);
    if (filter_count > (MAX_FILTER_COUNT - MAX_TITLE_LEN))
    {
        cpy_length = MAX_FILTER_COUNT - filter_count;
    }
    else
    {
        cpy_length = MAX_TITLE_LEN;
    }
    if (filter_count < BMK_NAME_LENGTH)
    {
        libc_memcpy(filename, &BOOKMARK_filename[filter_count], cpy_length);
    }
    libc_memset(BOOKMARK_filename, 0x0, BMK_NAME_LENGTH);
    if (bookmark_total >= MAX_BKMARK_NUM)
    {
        retval = _show_double_dialog(S_FULLMSG, filename);
        libc_memcpy(BOOKMARK_filename, filename, 12);
        if (retval == RESULT_DIALOG_YES)
        {
            _del_bookmark(0);
            _add_bookmark();
            return RESULT_EBK_PLAY;
        }
    }
    else
    {
        retval = _show_double_dialog(S_ADDOPTION, filename);    //弹出对话框提示是否添加书签
        libc_memcpy(BOOKMARK_filename, filename, 12);
        if (retval == RESULT_DIALOG_YES)            //选择YES按钮
        {
            _add_bookmark();
            return RESULT_EBK_PLAY;
        }
    }
    return RESULT_REDRAW;
}
/********************************************************************************
 * Description :页数选择菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_selbookpage(void *param)
{
    return _bmk_list_enter(NULL, PAGE_SEL);
}

/********************************************************************************
 * Description :主盘目录菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/
app_result_e _menu_func_flashdir(void *param)
{
    app_result_e result;
    if ((device_status & 0x03) == 0x03)
    {
        return RESULT_MAIN_APP;
    }

    if ((music_play_device != DISK_C) && (music_play_device != 0))
    {
        _close_engine();
    }
    if (g_ebook_vars.path.file_path.dirlocation.disk == DISK_H)
    {
        view_file.file_handle=0;        //切换盘，关闭文件，以便重新打开
        fsel_exit();
        libc_memset(&g_ebook_vars.path.file_path.dirlocation, 0x0, sizeof(file_location_t));
        g_ebook_vars.path.file_path.dirlocation.disk = DISK_C;
        result = _ebook_filesel_init();
        if (result == RESULT_CREATE_LIST_APP)
        {
            return RESULT_CREATE_LIST_APP;
        }
        else if(result == RESULT_REDRAW)
        {
            //恢复现场环境
            g_ebook_vars.path.file_path.dirlocation.disk = DISK_H;          //切换回C盘
            _ebook_filesel_init();                                          //初始化盘
            return result;
        }
        else
        {

        }

    }
    else if ((device_status & 0x01) == 0x01)
    {
        _show_single_dialog(S_DISK_NOFILE, DIALOG_INFOR_WAIT);
        return _deal_menu_result();
    }
    else
    {
    }
    enter_mode = ENTER_FROM_DESKTOP;
    return RESULT_EBK_FILELIST;
}



/********************************************************************************
 * Description :自动播放设置菜单选项的执行函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                场景返回值
 * Notes       :
 *
 ********************************************************************************/

app_result_e _menu_func_autoplay(void *param)
{
    app_result_e retval;
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;

    s_param_item.unit_id = -1;
    s_param_item.min = AUTOTIME_MIN;
    s_param_item.max = AUTOTIME_MAX;
    s_param_item.step = AUTOTIME_STEP;
    s_param_item.max_number = 2;
    s_param_item.value = (int16) g_ebook_vars.autoplaytime;
    s_param_item.cycle = TRUE;
    s_param_item.adjust_func = NULL;
    s_param_item.callback = NULL;//_set_autotime_callback;
    s_param_item.value_str = NULL;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;

    retval = gui_set_parameter(PARAM_SIMPLE, &s_param_com);
    if (retval == RESULT_CONFIRM)
    {
        g_ebook_vars.autoplaytime = (uint8) s_param_com.private_data->items->value;
        retval=RESULT_REDRAW; //返回上一级
    }
//    else if(retval == RESULT_REDRAW)//取消设置参数返回，重绘UI
//    {
//
//    }
//    else//其他返回结果，需要做特殊处理
//    {
//
//    }

    return retval; //返回上一级
}
/********************************************************************************
 * Description :自动播放时间间隔调节回调函数
 *
 * Arguments  :
 *              param:
 * Returns     :
 *                当前播放间隔的设置值
 * Notes       :
 *
 ********************************************************************************/

bool _set_autotime_callback(uint16 value)
{
    value = (uint8) value;
    return TRUE;
}

/********************************************************************************
 * Description :显示带按键操作的提示框
 *
 * Arguments  :
 *              option_id:提示资源文件ID
 *                    str_name:提示字符串
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/

app_result_e _show_double_dialog(uint16 option_id, uint8 *str_name)
{
    string_desc_t string_desc;
    string_desc_t string_name;
    dialog_com_data_t dialog_com;

    _clear_and_set_screen(0xffff, 0x0);
    if (str_name == NULL)
    {
        string_desc.data.id = option_id;
        string_desc.language = UNICODEID;
    }
    else
    {
        string_desc.data.str = (uint8 *)file_list;
        string_desc.length = BMK_NAME_LENGTH;
        string_desc.argv = option_id;
        string_desc.language = (int8) g_comval.language_id;

        string_name.data.str = str_name;
        string_name.length = 12;
        string_name.language = view_file.language;
        com_string_format(&string_desc, &string_name);
    }
    dialog_com.dialog_type = DIALOG_BUTTON_INCLUDE;
    dialog_com.button_type = BUTTON_YESNOCANCEL;
    dialog_com.icon_id = V_U16_INVALID;
    dialog_com.string_desc = &string_desc;
    if(option_id == S_IS_CREATE_PLAYLIST)
    {
        dialog_com.active_default = 1;
    }
    else
    {
        dialog_com.active_default = 0;
    }
    return gui_dialog(DIALOG_ASK, &dialog_com);
}
/********************************************************************************
 * Description :检测磁盘设备是否存在
 * Arguments  :
 *              device_type:磁盘设备对应的盘符
 * Returns     :
 *                TRUE or FALSE
 * Notes       :
 *
 ********************************************************************************/

bool _detect_device(uint8 device_type)
{
    if ((support_card_flag == 0) && (device_type == DRV_GROUP_STG_CARD))
    {
        return 0;
    }
    if (sys_detect_disk(device_type) == -1)
    {
        return 0;
    }
    return 1;
}

/********************************************************************************
 * Description :过滤字符串中正常字符前面的回车换行， 英文空格等字符
 *
 * Arguments  :
 *              str:需要过滤的字符串
 *              language:当前字符所属的语种(unicode或内码)
 * Returns     :
 *                无
 * Notes       :
 *
 ********************************************************************************/
uint8 _filterwhitespace(uint8 *str, int8 language)
{
    uint8 str_count = 0;//过滤字符的个数计数
    uint8 add_offset = 1; //字节增加的偏移个数
    uint16 test_str;//需要检查的字符串内容值
    uint8 str_length = MAX_FILTER_COUNT;
    while (1)
    {
        //如果字符为unicode时，则需要取两个字节
        if (language == UNICODEDATA)
        {
            test_str = (uint16) ((*(str + str_count)) * 0x100 + *((str + str_count) + 1));
            add_offset = 2;
        }
        else
        {
            test_str = *(str + str_count);
        }
        //如果字符串内容为空或者长度不大于0时结束计算
        if ((test_str == 0x0) || (str_length <= 0))
        {
            break;
        }
        //如果字符串的内容为0x0d,0x0a,0x20中的一种，刚认为存在回车和空格
        //则需要过滤
        if ((test_str == 0x0d) || (test_str == 0x0a) || (test_str == 0x20))
        {

            str_count += add_offset;
            str_length -= add_offset;
        }
        else
        {
            break;
        }
    }
    return str_count;
}

