#ifndef _MENU_CALLBACK_OTHERS_H
#define _MENU_CALLBACK_OTHERS_H
#include "case_include.h"

extern app_result_e get_shut_down_entry(void *menu_title_void);
extern app_result_e shut_down_timer_off_callback(void *menu_title_void);
extern app_result_e shut_down_time_adjust_callback(void *menu_title_void);

extern app_result_e get_pc_connect_information_id_callback(void *menu_title_void);
extern app_result_e set_storage_msc_callback(void *menu_title_void);
extern app_result_e set_storage_mtp_callback(void *menu_title_void);
extern app_result_e explain_msc_callback(void *menu_title_void);
extern app_result_e explain_mtp_callback(void *menu_title_void);
extern app_result_e show_player_information_callback(void *menu_title_void);
extern app_result_e show_disk_information_callback(void *menu_title_void);
extern app_result_e show_music_type_callback(void *menu_title_void);
extern app_result_e show_picture_type_callback(void *menu_title_void);
extern app_result_e show_video_type_callback(void *menu_title_void);
extern app_result_e show_legal_information_callback(void *menu_title_void);

#endif
