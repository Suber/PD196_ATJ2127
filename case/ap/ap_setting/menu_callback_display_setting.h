#ifndef _MENU_CALLBACK_DISPLAY_SETTING_H
#define _MENU_CALLBACK_DISPLAY_SETTING_H
#include "case_include.h"

extern app_result_e set_brightness_callback(void *menu_title_void);
extern app_result_e get_theme_option_callback(void *menu_title_void);
extern app_result_e set_theme_blue_callback(void *menu_title_void);
extern app_result_e set_theme_green_callback(void *menu_title_void);
extern app_result_e set_theme_gray_callback(void *menu_title_void);
extern app_result_e set_theme_pink_callback(void *menu_title_void);
extern app_result_e set_theme_red_callback(void *menu_title_void);
extern app_result_e get_backlight_timer_option_callback(void *menu_title_void);
extern app_result_e set_light_time_10s_callback(void *menu_title_void);
extern app_result_e set_light_time_20s_callback(void *menu_title_void);
extern app_result_e set_light_time_30s_callback(void *menu_title_void);
extern app_result_e set_light_time_always_callback(void *menu_title_void);
extern app_result_e get_screen_saver_option_callback(void *menu_title_void);
extern app_result_e set_screen_saver_off_callback(void *menu_title_void);
extern app_result_e set_screen_saver_clock_callback(void *menu_title_void);
extern app_result_e set_screen_saver_photos_callback(void *menu_title_void);
extern app_result_e set_screen_off_callback(void *menu_title_void);
extern app_result_e set_screen_saver_demo_callback(void *menu_title_void);

#endif
