/*******************************************************************************
 *                              US212A
 *                            Module: PIC_PLAY_SUB
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-12-9 15:04:19           1.0              build this file
 *******************************************************************************/
/*!
 * \file     pic_play_sub.c
 * \brief    图片解码子模块
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               该模块完成图片解码功能，实现解码初始化，与图片中间件的交互等
 * \par      EXTERNALIZED FUNCTIONS:
 *               需要调用图片中间件实现解码功能
 * \version 1.0
 * \date    2011-12-9
 *******************************************************************************/
#include "app_music.h"
#include "app_music_albumlist.h"

static ColorLcmInfor userinfor _BANK_DATA_ATTR_;
//static mmm_id_status_t pic_decoder_status _BANK_DATA_ATTR_;
//dir layer save
//static pdir_layer_t sys_layer_buf _BANK_DATA_ATTR_;

/******************************************************************************/
/*!
 * \par  Description:
 确定图片旋转时的坐标
 * \param[in]    region1 应用传递给中间件的窗坐标
 * \param[in]    region2 中间件返回给ap的图片实际坐标，需要对该坐标进行变换
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   由于底层未对图片旋转模式进行坐标变化，需要ap完成这一工作
 * \li   对于bmp图片,坐标原点在左下角，因此坐标只需xy交换位置
 * \li   对于jpg,bmp图片，坐标原点在左上角，需要在交换完xy坐标后再调整y坐标
 *******************************************************************************/
static void _format_region(wregion_t *region1, wregion_t *region2)
{
    uint16 temp_x = region2->x;
    uint16 region_width = region1->w;
    uint16 region_height = region1->h;

    if ((userinfor.returnflag & 0x01) != 0)
    {
        //旋转坐标变换
        region2->x = region2->y;

        region2->y = DISPLAY_LENGTH - temp_x - region1->w;

        //居中处理
        region2->x += (region_height - region2->w) / 2;
        region2->y += (region_width - region2->h) / 2;
    }
    else
    {
        //居中处理
        region2->x += (region_width - region2->w) / 2;
        region2->y += (region_height - region2->h) / 2;
    }

    return;

}
/******************************************************************************/
/*!
 * \par  Description:
 *    设置图片刷屏模式及设定图片窗口坐标
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   注意清屏函数会更改刷屏模式，因此一定要在清屏后设置刷屏模式
 *******************************************************************************/
static void _pic_set_window(wregion_t *region1, wregion_t *region2)
{
    uint8 draw_mode;

    //计算窗坐标
    _format_region(region1, region2);

    draw_mode = DRAW_MODE_H_DEF;

    lcd_set_draw_mode(draw_mode);

    //设置图片显示窗口
    lcd_set_window(region2);

}

/******************************************************************************/
/*!
 * \par  Description:
 * \void play_scene_albumart(void)
 * \绘制专辑封面album art
 * \param[in]    albumart_mode 1 显示albumart 0 显示专辑列表图片
 * \param[in]    full_mode     1 小数倍显示模式 0 整数倍显示模式
 * \param[in]    file_offset   图片在文件内偏移，对于非内嵌图片值为0
 * \param[in]    region1     输入窗参数
 * \param[out]   region2     输出窗参数
 * \return       void the result
 * \retval       none
 * \retval       none
 * \ingroup      music_playing_paint.c
 * \note
 */
/*******************************************************************************/
void music_paint_albumart(uint8 albumart_mode, uint8 full_mode, uint32 file_offset, wregion_t *region1,
        wregion_t *region2)
{
    int result;
    style_infor_t style;
    //解码库句柄
    void* mmm_pic_handle = NULL;
    style.type = UI_AP;

    //初始化解码变量
    userinfor.mountindex = (uint32) g_music_mount_id;
    //文件偏移
    userinfor.data_offset = file_offset;

    userinfor.playflag = 0;

    if (full_mode == FALSE)
    {
        //bypass模式
        userinfor.playflag |= 0x04;
    }

    userinfor.pregion1 = region1;
    userinfor.pregion2 = region2;

    //将enhance bank刷掉
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_1);
    sys_bank_flush(AP_BANK_FRONT_ENHANCED_2);

    //清除font cache
    ui_flush_font_buffer();

    //加载中间件驱动
    sys_load_mmm("mmm_id.al", FALSE);//待完善

    //打开解码库
    mmm_id_cmd(&mmm_pic_handle, MMM_ID_OPEN, (uint32) NULL);

    if (mmm_pic_handle != NULL)
    {
        //设置解码参数
        result = mmm_id_cmd(mmm_pic_handle, MMM_ID_SET_ALBUM_INFO, (unsigned int) &userinfor);
        if (result == 0)
        {
            if (albumart_mode == TRUE)
            {
                if ((region1->w != region2->w) || (region1->h != region2->h))
                {
                    //显示背景
                    style.style_id = ALBUM_ART_BGPIC;
                    ui_show_picbox(&style, NULL);
                }
            }

            _pic_set_window(region1, region2);

            //开始解码
            result = mmm_id_cmd(mmm_pic_handle, MMM_ID_DECODE, (unsigned int) NULL);
            while (1)
            {
                //获取状态
                if (userinfor.dec_status == MMM_ID_FREE)
                {
                    break;
                }
                else
                {
                    sys_os_time_dly(2);
                }
            }
        }
        else
        {
            if (albumart_mode == TRUE)
            {
                //为不支持的专辑图片类型，显示默认图片
                style.style_id = ALBUM_ART_PICBOX;
                ui_show_picbox(&style, NULL);
            }
            else
            {
                style.style_id = ALBUM_NO;
                ui_show_pic(ALBUM_NO, region1->x, region1->y);
            }
        }

    }
    //关闭解码库
    mmm_id_cmd(mmm_pic_handle, MMM_ID_CLOSE, (uint32) NULL);

    sys_free_mmm(FALSE);
}
/******************************************************************************/
/*!
 * \par  Description:
 *    显示一张专辑列表图片
 * \param[in]    active  当前专辑列表项序号
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   注意清屏函数会更改刷屏模式，因此一定要在清屏后设置刷屏模式
 *******************************************************************************/
void pic_paint(uint8 active)
{
    //path save
    //uint8 cur_offset_save[8];
    uint8 full_mode = TRUE;

    //save current path
    //vfs_file_dir_offset(g_music_mount_id, &sys_layer_buf, cur_offset_save, 0);

    vfs_file_dir_offset(g_music_mount_id, NULL, &(g_album_list[active].cluster_no), 1);

    if ((g_app_info_state.type == ENGINE_MUSIC) && (g_app_info_state.state == ENGINE_STATE_PLAYING))
    {
        full_mode = FALSE;
    }

    music_paint_albumart(0, full_mode, 0, &region_in, &region_out);

    //restore path
    //vfs_file_dir_offset(g_music_mount_id, &sys_layer_buf, cur_offset_save, 1);
}

/******************************************************************************/
/*!
 * \par  Description:
 *    初始化g_album_list参数
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_play
 * \note
 * \li   由于该变量为bank_data,因此需要注意参数值的恢复
 *******************************************************************************/
void pic_init_display_buffer(void)
{
    uint8 i;

    uint8 *temp_buffer = g_temp_buffer_p;

    for (i = 0; i < PIC_ONE_SCREEN; i++)
    {
        g_album_list[i].name_buf = &temp_buffer[LIST_ITEM_BUFFER_LEN * i];
        g_album_list[i].name_len = LIST_ITEM_BUFFER_LEN;
    }

    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    图片解码初始化
 * \param[in]  p_dir_control 文件浏览结构体指针
 * \param[in]  path_info     文件路径信息
 * \param[out] none
 * \return     app_result_e
 * \ingroup    pic_play
 * \note
 *******************************************************************************/
app_result_e pic_decode_init(dir_control_t *p_dir_control, file_path_info_t *path_info)
{
    app_result_e ret_val = RESULT_NULL;

    ui_clear_screen(NULL);

    //缩略图模式窗口这里先不设置
    gui_set_headbar_mode(HEADBAR_MODE_NORMAL, this_headbar_icon_id);

    //申请用于80ms滚动播放的定时器
    scroll_frush_id = set_app_timer(APP_TIMER_ATTRB_UI, 80, scroll_play_flag);
    stop_app_timer(scroll_frush_id);

    //初始列表项缓冲区空间
    g_dir_browser.name_len = 0;

    //从路径的第一级目录浏览
    if (p_dir_control->dir_com->browse_mode == 0)
    {
        //进入并获取根目录的目录结构体
        if (fsel_browser_enter_dir(ROOT_DIR, 0, &g_dir_browser) == FALSE)
        {
            return RESULT_DIR_ERROR_ENTER_DIR;
        }
    }
    else//直接跳到路径指向目录浏览
    {
        if (fsel_browser_set_location(&(path_info->file_path), path_info->file_source) == TRUE)
        {
            //获取当前路径的目录结构体
            if (fsel_browser_enter_dir(CUR_DIR, 0, &g_dir_browser) == FALSE)
            {
                return RESULT_DIR_ERROR_ENTER_DIR;
            }
        }
        else
        {
            if (p_dir_control->dir_com->root_layer == 0)
            {
                //进入并获取根目录的目录结构体
                if (fsel_browser_enter_dir(ROOT_DIR, 0, &g_dir_browser) == FALSE)
                {
                    return RESULT_DIR_ERROR_ENTER_DIR;
                }

                //获取 ROOT 文件路径，确保下次进来时设置成功
                fsel_browser_get_location(&(path_info->file_path), path_info->file_source);

                //找不到路径，清除该应用文件浏览记忆
                com_clear_app_history(PATH_HISTORY_DIR, g_this_app_info->app_id, p_dir_control->dir_disk);
            }
            else
            {
                //顶层不是根目录，错误返回，由上层自己处理
                return RESULT_DIR_ERROR_SETLOC;
            }
        }
    }
    return ret_val;
}


/******************************************************************************/
/*!
 * \par  Description:
 *    根据按键方向计算新的激活项
 * \param[in]    move_direction 按键方向 MOVE_UP/MOVE_DOWN/MOVE_PREV/MOVE_NEXT
 * \param[out]   none
 * \return       app_result_e 事件返回值
 * \retval       RESULT_PIC_NEXTSCREEN 更新整个屏幕显示
 * \retval       RESULT_PIC_NEXTITEM   更新激活项和非激活项显示
 * \retval       RESULT_NULL           不进行更新显示
 * \ingroup      pic_preview
 * \note
 *******************************************************************************/
static app_result_e move_sel(uint8 move_direction)
{
    uint16 cur_group, old_group;
    uint16 adjust_total = dir_control.list_total;
    uint8 adjust_value = (uint8) dir_control.list_total % PIC_PER_COLUME;
    uint16 list_active = dir_control.active;
    uint16 list_total = dir_control.list_total;

    app_result_e ui_result = RESULT_PIC_NEXTITEM;

    dir_control.old = dir_control.active;
    old_group = (dir_control.active / PIC_ONE_SCREEN);
    if (adjust_value != 0)
    {
        //如果当前文件总数不是3的倍数，需要将list_total进行修正
        adjust_total += PIC_PER_COLUME - adjust_value;
    }

    if ((list_total == 0) || (adjust_total == 0))
    {
        return ui_result;
    }

    switch (move_direction)
    {
#if 0
        case MOVE_UP:
        list_active = (list_active + adjust_total - PIC_PER_COLUME) % adjust_total;
        if (list_active > (list_total - 1))
        {
            list_active = list_active - PIC_PER_COLUME;
        }
        break;

        case MOVE_DOWN:
        list_active = (list_active + adjust_total + PIC_PER_COLUME) % adjust_total;
        if (list_active > (list_total - 1))
        {
            list_active = list_active % PIC_PER_COLUME;
        }
        break;
#endif
        case MOVE_PREV:
        list_active = (list_active + list_total - 1) % list_total;
        break;

        case MOVE_NEXT:
        list_active = (list_active + list_total + 1) % list_total;
        break;

        default:
        break;
    }
    cur_group = list_active / PIC_ONE_SCREEN;

    if (cur_group != old_group)
    {
        //需要换屏显示
        ui_result = RESULT_PIC_NEXTSCREEN;
    }
    else
    {
        if (list_active != dir_control.old)
        {
            //需要重新更新激活项和非激活项
            ui_result = RESULT_PIC_NEXTITEM;
        }
        else
        {
            //经过按键选择的激活项仍是该项，不进行任何更新
            ui_result = RESULT_NULL;
        }
    }
    dir_control.active = list_active;
    dir_control.list_no = list_active + 1;
    return ui_result;
}
/******************************************************************************/
/*!
 * \par  Description:
 *   保存当前路径记忆
 * \param[in]    p_dir_control 文件浏览结构体指针
 * \param[out]   none
 * \return       none
 * \ingroup      pic_preview
 * \note
 * \li   路径记忆需要保存list_no,list_top这两项的内容
 *******************************************************************************/

static void _pic_save_history(dir_control_t *p_dir_control)
{
    if (p_dir_control->list_no < p_dir_control->one_page_count)
    {
        p_dir_control->top = 1;
    }
    else
    {
        p_dir_control->top = p_dir_control->list_no - p_dir_control->one_page_count + 1;
    }
    write_history_item(g_dir_browser.layer_no, p_dir_control, FALSE);
    return;
}
/******************************************************************************/
/*!
 * \par  Description:
 *    缩略图模块GUI消息处理函数，根据不同事件值进行处理
 * \param[in]    path_info 路径信息
 * \param[in]    p_dir_control 文件浏览结构体指针
 * \param[in]    cur_event 根据按键消息转换得到的事件值
 * \param[out]   none
 * \return       app_result_e 事件处理返回值
 * \ingroup      pic_preview
 * \note
 *******************************************************************************/
app_result_e pic_preview_proc_key_event(file_path_info_t *path_info, dir_control_t *p_dir_control,
        msg_apps_type_e cur_event)
{
    app_result_e ui_result = RESULT_NULL;
    switch (cur_event)
    {
        case EVENT_PICTURE_PREVIEW_MOVEDOWN:
        ui_result = move_sel(MOVE_DOWN);
        break;

        case EVENT_PICTURE_PREVIEW_MOVEUP:
        ui_result = move_sel(MOVE_UP);
        break;

        case EVENT_PICTURE_PREVIEW_MOVEPREV:
        ui_result = move_sel(MOVE_PREV);
        break;

        case EVENT_PICTURE_PREVIEW_MOVENEXT:
        ui_result = move_sel(MOVE_NEXT);
        break;

        case EVENT_PICTURE_RETURN:
        _pic_save_history(p_dir_control);
        ui_result = RESULT_PIC_RETURN;
        break;

        case EVENT_PICTURE_OPTION:
        if (p_dir_control->dir_com->list_option_func != NULL)
        {
            ui_result = p_dir_control->dir_com->list_option_func(path_info, p_dir_control->list_no);
            if ((ui_result != RESULT_NULL) && (ui_result != RESULT_REDRAW))
            {
                _pic_save_history(p_dir_control);
            }
        }
        break;

        case EVENT_DIRECTORY_ENTER_SON:
        //保存当前层次文件浏览记录
        _pic_save_history(p_dir_control);
        if (fsel_browser_enter_dir(SON_DIR, p_dir_control->active + 1, &g_dir_browser) == TRUE)
        {
            fsel_browser_select(1);
            fsel_browser_get_location(&(path_info->file_path), path_info->file_source);
            ui_result = RESULT_ENTER_TITLE_LIST;
        }
        else
        {
            ui_result = RESULT_PIC_RETURN;
        }
        break;

        default:
        //pic_decoder_free();
        ui_result = com_message_box(cur_event);
        if ((ui_result == RESULT_NULL) || (ui_result == RESULT_CONFIRM))
        {
            ui_result = RESULT_REDRAW;
        }
        break;
    }
    return ui_result;
}

