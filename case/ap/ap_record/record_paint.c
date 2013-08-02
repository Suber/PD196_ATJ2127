/*******************************************************************************
 *                              us212a
 *                             Module: record
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : record_paint.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming       2011-10-27          v1.0
 *******************************************************************************/
/*include file of this application, 本应用的头文件*/
#include "ap_record.h"

headbar_icon_e headbar_callback(void)
{
    return HEADBAR_ICON_RECORD;
}

/******************************************************************************/
/*!
 * \par  Description:
 * \void _record_scene_paint(uint32 display_bitmap)
 * \record场景UI显示函数
 * \param[in]    uint32* pdisplay_bitmap，显示的区域
 * \param[out]   none
 * \return       none
 * \retval       none
 * \retval       ......
 * \ingroup      record_paint.c
 * \note
 */
/*******************************************************************************/
void _record_scene_paint(uint32* pdisplay_bitmap)
{
    style_infor_t style;
    picbox_private_t picbox_data;
    textbox_private_t textbox_data;

    /*显示record主界面图片*/
    if ((*pdisplay_bitmap & up_recordBackGDPic) != 0)
    {
        style.style_id = STY_RECORD_BG;
        style.type = UI_AP;

        ui_show_picbox((void*) &style, NULL);

        //清显示标志
        *pdisplay_bitmap &= clr_recordBackGDPic;
    }

    /*显示record HeadBar*/
    if ((*pdisplay_bitmap & up_recordHeadBar) != 0)
    {
        //gui_headbar(UpdateAppIcon|UpdateBattery);
        gui_set_headbar_mode(HEADBAR_MODE_NORMAL, HEADBAR_ICON_RECORD);

        //清显示标志
        *pdisplay_bitmap &= clr_recordHeadBar;
    }

    /*显示录音状态图标*/
    if (((*pdisplay_bitmap & up_recordPlayIcon) != 0) || ((*pdisplay_bitmap & up_recordPauseIcon) != 0))
    {
        style.style_id = STY_RECORD_STATE;
        style.type = UI_AP;

        picbox_data.pic_id = -1;

        if ((*pdisplay_bitmap & up_recordPlayIcon) != 0)
        {
            picbox_data.frame_id = 0;
        }
        else if ((*pdisplay_bitmap & up_recordPauseIcon) != 0)
        {
            picbox_data.frame_id = 1;
        }
        else
        {
        }

        ui_show_picbox((void*) &style, (void*) &picbox_data);

        /*清显示标志*/
        if ((*pdisplay_bitmap & up_recordPlayIcon) != 0)
        {
            *pdisplay_bitmap &= clr_recordPlayIcon;
        }
        else if ((*pdisplay_bitmap & up_recordPauseIcon) != 0)
        {
            *pdisplay_bitmap &= clr_recordPauseIcon;
        }
        else
        {
        }
    }

    /*显示文件名*/
    if ((*pdisplay_bitmap & up_recordFileTxt) != 0)
    {
        style.style_id = STY_RECORD_FILENAME;
        style.type = UI_AP;

        textbox_data.lang_id = ENGLISH;
        textbox_data.str_value = g_fs_param.show_filename;
        textbox_data.str_id = -1;

        ui_show_textbox((void*) &style, (void*) &textbox_data, TEXTBOX_DRAW_NORMAL);

        /*清显示标志*/
        *pdisplay_bitmap &= clr_recordFileTxt;
    }

    /*FM录音需刷新FM频点*/
    if ((*pdisplay_bitmap & up_recordFMFreq) != 0)
    {
        style.style_id = STY_RECORD_FMFREQ;
        style.type = UI_AP;

        textbox_data.lang_id = ENGLISH;
        textbox_data.str_value = g_scene_param.fm_freq; //此处需要取FM频点
        textbox_data.str_id = -1;

        ui_show_textbox((void*) &style, (void*) &textbox_data, TEXTBOX_DRAW_NORMAL);

        /*清显示标志*/
        *pdisplay_bitmap &= clr_recordFMFreq;
    }

    /*刷录音当前时间*/
    if ((*pdisplay_bitmap & up_recordTime) != 0)
    {
        _record_show_time_progress(pdisplay_bitmap, TRUE, 0);
    }

    /*显示录音时间进度条*/
    if ((*pdisplay_bitmap & up_recordProgress) != 0)
    {
        _record_show_time_progress(pdisplay_bitmap, TRUE, 1);
    }
}

