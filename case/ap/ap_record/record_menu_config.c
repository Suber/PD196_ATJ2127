/*
 *******************************************************************************
 *                              USDK(1100)
 *                             Module: app
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ap_cfg_rec_menu.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_record.h"
#include "record_menu_config.h"

#define MENU_ENTRY_SIZE 9
#define MENU_ITEM_SIZE   34

/* define variable, 变量定义 */
/*定义入口菜单*/

const conf_entry_head_t entrymenu_rec =
{ "ENTRY MENU  ", MENU_ENTRY_SIZE };

const conf_menu_entry_t entry_rec[MENU_ENTRY_SIZE] =
{
    { ENTRY_CARD, REC_CARD_S, 0 },
    { ENTRY_NOCARD, REC_NOCARD_S, 0 },
    { ENTRY_LINEIN_MODE, SET_AVR_TRACK_MODE, SET_REC_SOURCE_LINEIN },
    { ENTRY_MICIN_MODE, SET_AVR_TRACK_MODE, SET_REC_SOURCE_MIC },
    { ENTRY_BITRATE_MP3, SET_REC_BITRATE, SET_REC_FORMAT_MP3 },
    { ENTRY_BITRATE_WAV, SET_REC_BITRATE, SET_REC_FORMAT_WAV },
    { ENTRY_BITRATE_ACT, SET_REC_BITRATE, SET_REC_FORMAT_ACT },
    { ENTRY_ACT_CARD, REC_ACT_CARD_S, 0},
    { ENTRY_ACT_NOCARD, REC_ACT_NOCARD_S, 0}
};

const conf_item_head_t item_head_rec =
{ "MENU ITEM   ", MENU_ITEM_SIZE };

/*定义叶子菜单项*/
const conf_menu_item_t item_rec[MENU_ITEM_SIZE] =
{
    { START_RECORD, 0, start_record, NULL, NULL, NORMAL_MENU_ITEM, 0, 0 },
    { RECORD_DIR, 0, show_rec_dir, NULL, NULL, NORMAL_MENU_ITEM, 1, 0 },
    { SEL_DISK, 0, select_disk, NULL, NULL, NORMAL_MENU_ITEM, 2, 0 },
    { SEL_DISK_INTERNAL, 0, select_disk_internal, NULL, NULL, RAIDO_MENU_ITEM, 3, 0 },
    { SEL_DISK_EXTERNAL, 0, select_disk_external, NULL, NULL, RAIDO_MENU_ITEM, 4, 0 },
    { SET_REC_FORMAT, 0, set_rec_format, NULL, NULL, NORMAL_MENU_ITEM, 5, 0 },
    { SET_REC_FORMAT_MP3, 0, set_rec_format_mp3, NULL, NULL, RAIDO_MENU_ITEM, 6, 0 },
    { SET_REC_FORMAT_WAV, 0, set_rec_format_wav, NULL, NULL, RAIDO_MENU_ITEM, 7, 0 },
    { SET_REC_TYPE, 0, NULL, NULL, NULL, NORMAL_MENU_ITEM, 8, 0 },
    { SET_REC_SOURCE, 0, set_rec_source, NULL, NULL, NORMAL_MENU_ITEM, 9, 0 },
    { SET_REC_SOURCE_MIC, 0, set_rec_source_mic, NULL, NULL, RAIDO_MENU_ITEM, 10, 0 },
    { SET_REC_SOURCE_LINEIN, 0, set_rec_source_linein, NULL, NULL, RAIDO_MENU_ITEM, 11, 0 },
    { SET_REC_BITRATE, 0, get_rec_bitrate_entry, NULL, NULL, NORMAL_MENU_ITEM, 12, 0 },
    { SET_REC_BITRATE_32KBPS, 0, set_rec_bitrate_32kbps, NULL, NULL, RAIDO_MENU_ITEM, 13, 0 },
    { SET_REC_BITRATE_64KBPS, 0, set_rec_bitrate_64kbps, NULL, NULL, RAIDO_MENU_ITEM, 14, 0 },
    { SET_REC_BITRATE_128KBPS, 0, set_rec_bitrate_128kbps, NULL, NULL, RAIDO_MENU_ITEM, 15, 0 },
    { SET_REC_BITRATE_192KBPS, 0, set_rec_bitrate_192kbps, NULL, NULL, RAIDO_MENU_ITEM, 16, 0 },
    { SET_REC_BITRATE_256KBPS, 0, set_rec_bitrate_256kbps, NULL, NULL, RAIDO_MENU_ITEM, 17, 0 },
    { SET_AVR_TRACK_MODE, 0, get_avr_track_entry, NULL, NULL, NORMAL_MENU_ITEM, 18, 0 },
    { SET_TRACK_MODE, 0, set_track_mode, NULL, NULL, RAIDO_MENU_ITEM, 19, 0 },
    { SET_NORMAL_MODE, 0, set_normal_mode, NULL, NULL, RAIDO_MENU_ITEM, 20, 0 },
    { SET_AVR_MODE, 0, set_avr_mode, NULL, NULL, RAIDO_MENU_ITEM, 21, 0 },
    { SET_REC_BITRATE_512KBPS, 0, set_rec_bitrate_512kbps, NULL, NULL, RAIDO_MENU_ITEM, 22, 0 },
    { SET_REC_BITRATE_768KBPS, 0, set_rec_bitrate_768kbps, NULL, NULL, RAIDO_MENU_ITEM, 23, 0 },
    { SET_REC_BITRATE_1024KBPS, 0, set_rec_bitrate_1024kbps, NULL, NULL, RAIDO_MENU_ITEM, 24, 0 },
    { SET_REC_BITRATE_1536KBPS, 0, set_rec_bitrate_1536kbps, NULL, NULL, RAIDO_MENU_ITEM, 25, 0 },
    { S_SET_RECORDING_SCENE , 0, set_noise_cancel_sure           , NULL, NULL, NORMAL_MENU_ITEM, 26, 0 },
    { S_REC_NORMAL_SCENE    , 0, set_noise_cancel_normal_mode    , NULL, NULL, RAIDO_MENU_ITEM, 27, 0 },
    { S_REC_TRAIN_SCENE     , 0, set_noise_cancel_train_mode     , NULL, NULL, RAIDO_MENU_ITEM, 28, 0 },
    { S_REC_MEETING_SCENE   , 0, set_noise_cancel_meeting_mode   , NULL, NULL, RAIDO_MENU_ITEM, 29, 0 },
    { S_REC_LONG_RANGE_SCENE, 0, set_noise_cancel_long_range_mode, NULL, NULL, RAIDO_MENU_ITEM, 30, 0 },
    { S_REC_STOP_NOISE_CANCELLATION, 0, set_noise_cancel_disable , NULL, NULL, RAIDO_MENU_ITEM, 31, 0 },
    { SET_REC_FORMAT_ACT, 0, set_rec_format_act, NULL, NULL, RAIDO_MENU_ITEM, 32, 0 },
    { SET_REC_BITRATE_8KBPS, 0, set_rec_bitrate_8kbps, NULL, NULL, RAIDO_MENU_ITEM, 33, 0 }

};

#ifdef PC
menu_cb_func _get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head_rec.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item_rec[i].major_id == str_id) && (item_rec[i].minor_id == str_id_sub))
        {
            return item_rec[i].menu_func;
        }

    }

    return NULL;
}

menu_cb_leaf _get_callback_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head_rec.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item_rec[i].major_id == str_id) && (item_rec[i].minor_id == str_id_sub))
        {

            return item_rec[i].callback;
        }
    }

    return NULL;
}

menu_cb_option _get_menu_option(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head_rec.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item_rec[i].major_id == str_id) && (item_rec[i].minor_id == str_id_sub))
        {
            return item_rec[i].menu_func;

        }

    }

    return NULL;
}

#endif
