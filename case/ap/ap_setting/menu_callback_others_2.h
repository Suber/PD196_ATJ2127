#ifndef _MENU_CALLBACK_OTHERS_2_H
#define _MENU_CALLBACK_OTHERS_2_H
#include "case_include.h"

extern app_result_e get_shut_down_entry(void *menu_title_void);
extern app_result_e shut_down_timer_off_callback(void *menu_title_void);
extern app_result_e shut_down_time_adjust_callback(void *menu_title_void);

extern app_result_e get_format_device_entry(void *menu_title_void);
extern app_result_e format_disk_callback(void *menu_title_void);
extern app_result_e format_card_callback(void *menu_title_void);
extern app_result_e dont_format_callback(void *menu_title_void);
extern app_result_e dialog_restore_factory_setting_callback(void *menu_title_void);
extern app_result_e get_autorun_setting_callback(void * title_action);
extern app_result_e disable_autorun_callback(void *menu_title_void);
extern app_result_e enable_autorun_callback(void *menu_title_void);
#endif
