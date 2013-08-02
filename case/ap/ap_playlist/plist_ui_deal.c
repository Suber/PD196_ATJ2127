/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    这里填写文件的概述
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/
#include "plist.h"
#include "rdata_ex.h"

#define ID_ICON_MAX  7
#define ID_PERCENT_STR_MAX 11

static const uint8 display_percent_str[ID_PERCENT_STR_MAX][5] =
{ "0%", "10%", "20%", "30%", "40%", "50%", "60%", "70%", "80%", "90%", "100%" };

//定时刷新字符窗口
void plist_reflash_string(uint8 str_id)
{
    style_infor_t plist_sty;
    textbox_private_t textbox_param;

    if (ui_cur_strID != str_id)
    {
        ui_cur_strID = str_id;
        // 显示search字符串
        plist_sty.style_id = PLAYLIST_STRING;
        plist_sty.type = UI_AP;
        textbox_param.lang_id = UNICODEID;
        textbox_param.str_id = str_id;
        textbox_param.str_value = NULL;
        ui_show_textbox(&plist_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);
    }
    return;
}

//app定时器,定时处理程序
void plist_timer_proc(void)
{
    style_infor_t plist_sty;
    picbox_private_t picbox_param;

    //循环刷新locading图标
    ui_icon_count--;
    if (ui_icon_count > ID_ICON_MAX)
    {
        ui_icon_count = ID_ICON_MAX;
    }

    plist_sty.style_id = PLAYLIST_LOAD_CYCLE;
    plist_sty.type = UI_AP;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = ui_icon_count;
    ui_show_picbox(&plist_sty, &picbox_param);

    plist_reflash_string(S_PLIST_SEARCH);

}

//更新进度条和百分比
void plist_reflash_progress(uint8 prog_value)
{
    style_infor_t plist_sty;
    progressbar_private_t progressbar_param;
    textbox_private_t textbox_param;

    bool reflash_flag = TRUE;

    if (prog_value > (ID_PERCENT_STR_MAX - 1))
    {
        ui_precent_count++;

        if (ui_precent_count > (ID_PERCENT_STR_MAX - 2))
        {
            ui_precent_count = ID_PERCENT_STR_MAX - 2;//停在90%
            reflash_flag = FALSE;
        }
    }
    else
    {
        ui_precent_count = prog_value;
    }

    if (FALSE == reflash_flag)
    {
        return;
    }

    //刷新进度条
    plist_sty.style_id = PLAYLIST_PROGRESS;
    plist_sty.type = UI_AP;
    progressbar_param.total = 120;
    progressbar_param.value = ui_precent_count * 12;
    ui_show_progressbar(&plist_sty, &progressbar_param, PROGRESS_DRAW_PROGRESS);

    //刷新百分比
    plist_sty.style_id = PLAYLIST_PERCENT;
    plist_sty.type = UI_AP;
    textbox_param.lang_id = ANSIDATAAUTO;
    textbox_param.str_value = (uint8*) display_percent_str[ui_precent_count];
    ui_show_textbox(&plist_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);

    if (ui_precent_count == 0x0a)
    {
        //刷新字符串显示
        plist_sty.style_id = PLAYLIST_STRING;
        plist_sty.type = UI_AP;
        textbox_param.lang_id = UNICODEID;
        textbox_param.str_id = S_PLIST_COMPLETE;
        textbox_param.str_value = NULL;
        ui_show_textbox(&plist_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);
        ui_cur_strID = S_PLIST_COMPLETE;
    }

}

//初始化启动界面
void _app_display_init(void)
{
    /*pic_box 接口变量*/
    style_infor_t plist_sty;
    picbox_private_t picbox_param;
    progressbar_private_t progressbar_param;
    textbox_private_t textbox_param;

    ui_precent_count = 0;
    ui_icon_count = ID_ICON_MAX;

    //设置字符串滚屏定时器

    /*显示底部图片*/
    plist_sty.style_id = PLAYLIST_BACKGROUND;
    plist_sty.type = UI_AP;
    ui_show_picbox(&plist_sty, NULL);

    // 显示locading图标
    plist_sty.style_id = PLAYLIST_LOAD_CYCLE;
    plist_sty.type = UI_AP;
    picbox_param.pic_id = -1;
    picbox_param.frame_id = ui_icon_count;
    ui_show_picbox(&plist_sty, &picbox_param);

    // 显示进度条
    plist_sty.style_id = PLAYLIST_PROGRESS;
    plist_sty.type = UI_AP;
    progressbar_param.total = 120;
    progressbar_param.value = 0;
    ui_show_progressbar(&plist_sty, &progressbar_param, PROGRESS_DRAW_ALL);

    // 显示search字符串
    plist_sty.style_id = PLAYLIST_STRING;
    plist_sty.type = UI_AP;
    textbox_param.lang_id = UNICODEID;
    textbox_param.str_id = S_PLIST_SEARCH;
    textbox_param.str_value = NULL;
    ui_show_textbox(&plist_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);
    ui_cur_strID = S_PLIST_SEARCH;

    // 显示百分比
    plist_sty.style_id = PLAYLIST_PERCENT;
    plist_sty.type = UI_AP;
    textbox_param.lang_id = ANSIDATAAUTO;
    textbox_param.str_value = (uint8*) display_percent_str[0];
    ui_show_textbox(&plist_sty, &textbox_param, TEXTBOX_DRAW_NORMAL);
}

