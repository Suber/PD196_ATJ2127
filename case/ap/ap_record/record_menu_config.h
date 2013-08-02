/********************************************************************************
 *                            Module: record
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      leiming    2011-10-14          1.0             build this file
 ********************************************************************************/

#ifndef _AP_RECORD_MENU_CONFIG_H_
#define _AP_RECORD_MENU_CONFIG_H_

#include "psp_includes.h"
#include "case_include.h"	//display.h°üº¬ÔÚcommon_ui.hÖÐ
#define ENTRY_CARD 			0
#define ENTRY_NOCARD		1
#define ENTRY_LINEIN_MODE       2
#define ENTRY_MICIN_MODE        3
#define ENTRY_BITRATE_MP3       4
#define ENTRY_BITRATE_WAV       5
#define ENTRY_BITRATE_ACT       6
#define ENTRY_ACT_CARD          7
#define ENTRY_ACT_NOCARD        8

extern app_result_e start_record(void* param);
extern app_result_e show_rec_dir(void* param);

extern app_result_e select_disk(void * title_action);
extern app_result_e select_disk_internal(void* param);
extern app_result_e select_disk_external(void* param);

extern app_result_e set_rec_format(void *title_action);
extern app_result_e set_rec_format_wav(void* param);
extern app_result_e set_rec_format_mp3(void* param);
extern app_result_e set_rec_format_act(void* param);

extern app_result_e set_rec_source(void *title_action);
extern app_result_e set_rec_source_mic(void* param);
extern app_result_e set_rec_source_linein(void* param);

extern app_result_e get_rec_bitrate_entry(void *title_action);
extern app_result_e set_rec_bitrate_8kbps(void* param);
extern app_result_e set_rec_bitrate_32kbps(void* param);
extern app_result_e set_rec_bitrate_64kbps(void* param);
extern app_result_e set_rec_bitrate_128kbps(void* param);
extern app_result_e set_rec_bitrate_192kbps(void* param);
extern app_result_e set_rec_bitrate_256kbps(void* param);
extern app_result_e set_rec_bitrate_512kbps(void* param);
extern app_result_e set_rec_bitrate_768kbps(void* param);
extern app_result_e set_rec_bitrate_1024kbps(void* param);
extern app_result_e set_rec_bitrate_1536kbps(void* param);

extern app_result_e get_avr_track_entry(void *title_action);
extern app_result_e set_track_mode(void* param);
extern app_result_e set_normal_mode(void* param);
extern app_result_e set_avr_mode(void* param);
extern app_result_e set_noise_cancel_sure(void *param);
extern app_result_e set_noise_cancel_normal_mode(void *param);
extern app_result_e set_noise_cancel_train_mode(void *param);
extern app_result_e set_noise_cancel_meeting_mode(void *param);
extern app_result_e set_noise_cancel_long_range_mode(void *param);
extern app_result_e set_noise_cancel_disable(void *param);



extern app_result_e show_now_playing(void* param);
extern app_result_e del_now_playing(void* param);
extern app_result_e del_all_files(void* param);

extern app_result_e menu_func_playing(void *param);
extern app_result_e rec_delect_file(void *param);

extern const conf_entry_head_t entrymenu_rec;
extern const conf_menu_entry_t entry_rec[];
extern const conf_item_head_t item_head_rec;
extern const conf_menu_item_t item_rec[];

#endif
