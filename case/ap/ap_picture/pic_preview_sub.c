/*******************************************************************************
 *                              US212A
 *                            Module: PIC_PREVIEW_SUB
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan    2011-12-9 11:16:08           1.0              build this file
 *******************************************************************************/
/*!
 * \file     pic_preview_sub.c
 * \brief    图片缩略图按键响应函数
 * \author   wuyufan
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date    2011-12-9
 *******************************************************************************/
#include "picture.h"
#include "picture_preview.h"

/******************************************************************************/
/*!
 * \par  Description:
 *    计算缩略图dir_control变量相关成员值
 * \param[in]    none
 * \param[out]   none
 * \return       none
 * \ingroup      pic_preview
 * \note
 *******************************************************************************/
void init_list_para(void)
{
    uint16 cur_group;
    dir_control.old = dir_control.active;
    cur_group = (dir_control.active / PIC_ONE_SCREEN);
    dir_control.top = cur_group * PIC_ONE_SCREEN;
    dir_control.bottom = dir_control.top + PIC_ONE_SCREEN - 1;
    if (dir_control.bottom > (dir_control.list_total - 1))
    {
        dir_control.bottom = (dir_control.list_total - 1);
    }
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
 * \param[in]    cur_event 根据按键消息转换得到的事件值
 * \param[out]   none
 * \return       app_result_e 事件处理返回值
 * \ingroup      pic_preview
 * \note
 *******************************************************************************/
app_result_e pic_preview_proc_key_event(msg_apps_type_e cur_event, dir_control_t *p_dir_control)
{
    //dir_control_t dir_control;
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
        if (g_dir_browser.layer_no == 0)
        {
            ui_result = RESULT_PIC_RETURN;
        }
        else
        {
            //返回上一级目录
            if (fsel_browser_enter_dir(PARENT_DIR, 0, &g_dir_browser) == TRUE)
            {
                p_dir_control->enter_dir = TRUE;
                ui_result = RESULT_REDRAW;
            }
            else
            {
                ui_result = RESULT_PIC_RETURN;
            }
        }
        break;

        case EVENT_PICTURE_OPTION:
        //保存当前层次文件浏览记录
        _pic_save_history(p_dir_control);
        ui_result = RESULT_PIC_OPTIONMENU;
        break;

        case EVENT_DIRECTORY_ENTER_SON:
        //保存当前层次文件浏览记录
        _pic_save_history(p_dir_control);
        pic_decoder_free();
        if (dir_control.active < g_dir_browser.dir_total)
        {
            if (fsel_browser_enter_dir(SON_DIR, dir_control.active + 1, &g_dir_browser) == TRUE)
            {
                p_dir_control->enter_dir = TRUE;
                ui_result = RESULT_REDRAW;
            }
            else
            {
                ui_result = RESULT_PIC_RETURN;
            }
        }
        else
        {
            //设置选中文件
            fsel_browser_select(dir_control.list_no);
            //获取当前文件路径
            fsel_browser_get_location(&g_picture_var.path.file_path, g_picture_var.path.file_source);
            ui_result = RESULT_PICTURE_PLAY;
        }
        break;

        default:
        pic_decoder_free();
        ui_result = com_message_box(cur_event);
        if ((ui_result == RESULT_NULL) || (ui_result == RESULT_CONFIRM))
        {
            ui_result = RESULT_REDRAW;
        }
        break;
    }
    return ui_result;
}

