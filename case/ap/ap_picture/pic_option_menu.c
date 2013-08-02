/*******************************************************************************
 *                              US212A
 *                            Module: Picture
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-09-15 10:00     1.0             build this file
 *******************************************************************************/
/*!
 * \file     pic_option_menu.c
 * \brief    Picture 的option菜单列表模块
 * \author   zhangxs
 * \version  1.0
 * \date  2011/9/15
 *******************************************************************************/
#include  "picture.h"
#include  "pic_menu_cfg.h"

#define VALUE_SLIDE_TIME_MIN  2
#define VALUE_SLIDE_TIME_MAX  30
#define VALUE_SLIDE_TIME_STEP 1


const uint16 backlight_timer_text_table[] =
{ S_TIME_ALWAYS, S_TIME_10S, S_TIME_20S, S_TIME_30S };

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture正在播放叶子菜单执行函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_playing(void *param)
{
    return RESULT_NOWPLAYING;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture上一次播放叶子菜单执行函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_last_play(void *param)
{
    return RESULT_LASTPLAY;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture设置背光亮度功能回调函数
 * \param[in]    param  设置的背光参数
 * \param[out]   none
 * \return       bool   TRUE 设置成功
 * \ingroup      pic_optionmenu
 * \note
 * \li  作为menu_func_bright参数框控件回调函数被调用
 *******************************************************************************/
static bool _set_lightness_callback(int16 param)
{
    g_comval.lightness = (uint8) param;

    //设置到硬件
    com_set_contrast((uint8) param);
    return TRUE;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture设置背光亮度叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 * \li  作为menu_func_bright参数框控件回调函数被调用
 * \li  参数框可以看作一个独立的子场景，支持单参数和多参数
 *******************************************************************************/
app_result_e menu_func_bright(void *param)
{
    //调节亮度
    app_result_e result;
    //参数框控件内容结构体
    param_com_data_t s_param_com;
    //parambox私有数据内容
    parambox_private_t s_param_private;
    //parambox参数设置内容
    parambox_one_t s_param_item;

    //参数单位
    s_param_item.unit_id = -1;
    //背光最小值
    s_param_item.min = VALUE_LIGHTNESS_MIN;
    //背光最大值
    s_param_item.max = VALUE_LIGHTNESS_MAX;
    //参数显示时最大位数
    s_param_item.max_number = 2;
    //参数调节步进
    s_param_item.step = VALUE_LIGHTNESS_STEP;
    //参数当前值
    s_param_item.value = g_comval.lightness;
    //参数框控件调节是否可以循环
    s_param_item.cycle = FALSE;
    //参数修正函数
    s_param_item.adjust_func = NULL;
    //调节参数时实时响应函数
    s_param_item.callback = _set_lightness_callback;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    //多参数时指定当前的激活项
    s_param_private.active = 0;
    //参数个数
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;
    s_param_com.private_data = &s_param_private;

    //参数框模块，对应的参数内容
    result = gui_set_parameter(SLIDER_SIMPLE, &s_param_com);
    //只有确认键时才生效
    if (result == RESULT_CONFIRM)
    {
        gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_BRIGHT_SETTED);
        g_comval.lightness = (uint8) s_param_com.private_data->items->value;
        result = RESULT_REDRAW;
    }
    return result;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture背光时间根菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 * \li  通过传入的param参数与字符串进行匹配，确定当前的激活项
 * \li
 *******************************************************************************/
app_result_e menu_func_backlight_timer(void *param)
{
    menu_title_action_t *menu_title;
    menu_title = (menu_title_action_t*) param;

    //light_timer以0.5秒为单位，0表示背光常亮
    menu_title->str_id = backlight_timer_text_table[g_comval.light_timer / 20];
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 10s背光叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_timer_10s(void *param)
{
    g_comval.light_timer = 10 * 2;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 20s背光叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_timer_20s(void *param)
{
    g_comval.light_timer = 20 * 2;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 30s背光叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_timer_30s(void *param)
{
    g_comval.light_timer = 30 * 2;
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 常亮背光叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_timer_all_time(void *param)
{
    g_comval.light_timer = 0;
    return RESULT_MENU_PARENT;
}
#if 0
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 幻灯片放映根菜单执行函数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note 由于该根菜单下级菜单不需要进行字符串匹配，因此不需要该执行函数
 *******************************************************************************/
app_result_e menu_func_slideshow_set(void *param)
{
    return RESULT_MENU_SON;
}
#endif

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 幻灯片播放时间叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_per_slide(void *param)
{
    app_result_e result;
    param_com_data_t s_param_com;
    parambox_private_t s_param_private;
    parambox_one_t s_param_item;

    //参数单位为秒
    s_param_item.unit_id = S_SECOND;
    s_param_item.min = VALUE_SLIDE_TIME_MIN;
    s_param_item.max = VALUE_SLIDE_TIME_MAX;
    s_param_item.max_number = 2;
    s_param_item.step = VALUE_SLIDE_TIME_STEP;
    s_param_item.value = g_comval.slide_time;
    s_param_item.value_str = NULL;
    //允许参数值循环调节
    s_param_item.cycle = TRUE;
    s_param_item.adjust_func = NULL;
    s_param_item.callback = NULL;

    s_param_private.back_id = -1;
    s_param_private.title_id = -1;
    s_param_private.icon_id = -1;
    s_param_private.active = 0;
    s_param_private.param_cnt = 1;
    s_param_private.items = &s_param_item;
    s_param_private.sign_flag = 0;

    s_param_com.private_data = &s_param_private;

    result = gui_set_parameter(PARAM_SIMPLE, &s_param_com);
    if (result == RESULT_CONFIRM)
    {
        g_comval.slide_time = (uint8) s_param_com.private_data->items->value;
        result = RESULT_REDRAW;
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 幻灯片重复播放根菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_repeat(void *param)
{
    uint16 active_id;
    menu_title_action_t * menu_title;
    menu_title = (menu_title_action_t*) param;
    if (g_comval.slide_repeat_mode == FSEL_MODE_NORMAL)
    {
        //loopnormal
        active_id = S_OFF;
    }
    else
    {
        //loopall
        active_id = S_ON;
    }
    menu_title->str_id = active_id;
    return RESULT_MENU_SON;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片播放重复开叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_repeat_on(void *param)
{
    uint8 cur_mode = (g_comval.slide_shuffle << 4);
    g_comval.slide_repeat_mode = FSEL_MODE_LOOPALL;
    cur_mode |= g_comval.slide_repeat_mode;
    fsel_set_mode(cur_mode);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片重复播放关叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_repeat_off(void *param)
{
    uint8 cur_mode = (g_comval.slide_shuffle << 4);
    g_comval.slide_repeat_mode = FSEL_MODE_NORMAL;
    cur_mode |= g_comval.slide_repeat_mode;
    fsel_set_mode(cur_mode);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片随机播放根菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_shuffle(void *param)
{
    uint16 active_id;
    menu_title_action_t * menu_title;
    menu_title = (menu_title_action_t*) param;
    if (g_comval.slide_shuffle == 0)
    {
        active_id = S_OFF;
    }
    else
    {
        active_id = S_ON;
    }
    //指定active项
    menu_title->str_id = active_id;
    return RESULT_MENU_SON;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片随机播放开叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_shuffle_on(void *param)
{
    uint8 cur_mode = g_comval.slide_repeat_mode;
    g_comval.slide_shuffle = 1;
    cur_mode |= 0x10;
    fsel_set_mode(cur_mode);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片随机播放关叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_time_shuffle_off(void *param)
{
    uint8 cur_mode = g_comval.slide_repeat_mode;
    g_comval.slide_shuffle = 0;
    fsel_set_mode(cur_mode);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片缩略图设置根菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_preview(void *param)
{
    uint16 active_id;
    menu_title_action_t * menu_title;
    menu_title = (menu_title_action_t*) param;
    if ((g_picture_var.thumbnail_flag & VALUE_PREVIEW_ON) == 0)
    {
        active_id = S_OFF;
    }
    else
    {
        active_id = S_ON;
    }
    menu_title->str_id = active_id;
    return RESULT_MENU_SON;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片缩略图开执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_preview_on(void *param)
{
    g_picture_var.thumbnail_flag = (uint8)(g_picture_var.thumbnail_flag | VALUE_PREVIEW_ON);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片缩略图关执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_preview_off(void *param)
{
    g_picture_var.thumbnail_flag = (uint8)(g_picture_var.thumbnail_flag & VALUE_PREVIEW_OFF);
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片删除文件叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 * \li  该函数会调用common的ui_delete控件实现文件删除功能
 * \li  如果后台有music播放，会先停止music播放，待删除完毕再重新开启
 *******************************************************************************/
app_result_e menu_func_delete_pic(void *param)
{
    bool ret_val;
    app_result_e result;
    del_com_data_t del_com;

    if (g_picture_scene_prev == SCENE_PLAY)
    {
        //已知其location
        del_com.del_no = 0;
    }
    else
    {
        //列表模式,主动获取location
        del_com.del_no = dir_control.list_no;
        fsel_browser_select(dir_control.list_no);
        fsel_browser_get_location(&g_picture_var.path.file_path, g_picture_var.path.file_source);
        fsel_browser_set_file(&g_picture_var.path.file_path, g_picture_var.path.file_source);
    }

    vfs_get_name(picture_mount, g_picture_filename, LIST_ITEM_BUFFER_LEN / 2);
    if ((g_picture_filename[0] != 0xff) && (g_picture_filename[0] != 0xfe))
    {
        com_dot_to_shortname(g_picture_filename);
    }

    del_com.del_mode = DEL_MODE_FILE;
    del_com.filename = g_picture_filename;
    del_com.del_playing = FALSE;
    del_com.del_func = NULL;
    del_com.del_self = TRUE;
    result = gui_delete(&g_picture_var.path, &del_com);
    if ((result == RESULT_DELETE_FILE) || (result == RESULT_DELETE_DIRLIST))
    {
        //退出当前option菜单场景
        if(g_picture_var.path.file_path.dirlocation.file_total == 0)
        {
            gui_dialog_msg(DIALOG_MSG, DIALOG_INFOR_WAIT, S_NO_PIC_FILE);
            result = RESULT_MAIN_APP;
        }
        else
        {
            g_del_file = TRUE;

            //更新location信息
            if (g_picture_var.path.file_path.dirlocation.file_num > g_picture_var.path.file_path.dirlocation.file_total)
            {
                g_picture_var.path.file_path.dirlocation.file_num = 1;
            }
            //获取下一曲
            ret_val = fsel_get_byno(g_picture_var.path.file_path.dirlocation.filename,
                    g_picture_var.path.file_path.dirlocation.file_num);

            if (ret_val == TRUE)
            {
                fsel_get_location(&g_picture_var.path.file_path.dirlocation, g_picture_var.path.file_source);
            }
            result = RESULT_MENU_EXIT;
        }
    }
    return result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 图片创建播放列表叶子菜单执行函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_generate_pic_playlist(void *param)
{
    return RESULT_CREATE_LIST_APP;
}


/******************************************************************************/
/*!
 * \par  Description:
 *	  picture 菜单项按option键的回调函数
 * \param[in]    void *param
 * \param[out]   none
 * \return       none
 * \ingroup      pic_optionmenu
 * \note
 *******************************************************************************/
app_result_e menu_func_option_option(void)
{
    //返回上一级
    return RESULT_MENU_PARENT;
}

/******************************************************************************/
/*!
 * \par  Description:
 *    Option菜单场景函数
 * \param[in]    none
 * \param[out]   none
 * \return       the result
 * \retval       app_result_e, 菜单控件返回的结构消息
 * \ingroup      pic_optionmenu
 * \note
 * \li  根据当前是否有后台播放以及缩略图功能是否支持确定入口菜单编号
 *******************************************************************************/
app_result_e pic_option_menulist(void)
{
    uint8 i, j;
    engine_state_e play_sta;
    app_result_e result = RESULT_NULL;

    //菜单控件参数
    menu_com_data_t op_menu;
    op_menu.app_id = APP_ID_PICTURE;

    play_sta = get_engine_state();

    if (play_sta == ENGINE_STATE_NULL)
    {
        i = 0;
    }
    else if (play_sta == ENGINE_STATE_PLAYING)
    {
        i = 1;
    }
    else
    {
        i = 2;
    }

    if ((g_picture_var.thumbnail_flag & VALUE_PREVIEW_SUPPORT) == 0)
    {
        j = 0;
    }
    else
    {
        j = 1;
    }

    //定义不同模式下的入口菜单
    op_menu.menu_entry = i + j * 3;
    //当前菜单控件路径记忆编号
    op_menu.path_id = 0;
    //首层菜单回调函数
    op_menu.menu_func = NULL;
    //浏览模式
    op_menu.browse_mode = 0;

    result = gui_menulist(MENULIST_OPTION, &op_menu);
    if (result == RESULT_REDRAW)
    {
        if (g_picture_scene_prev == SCENE_PLAY)
        {
            result = RESULT_PICTURE_PLAY;
        }
        else
        {
            result = RESULT_PIC_FILELIST;
        }
    }
    return result;
}

