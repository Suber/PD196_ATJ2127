#ifndef _MENU_CALLBACK_SOUND_SETTING_2_H
#define _MENU_CALLBACK_SOUND_SETTING_2_H
#include "case_include.h"

extern uint8 g_eq_restore;
extern param_com_data_t s_param_com;
extern parambox_private_t s_param_private;
extern parambox_one_t s_param_item[5];

extern app_result_e set_eq_custom_sure(void *menu_title_void);
extern app_result_e set_user_srs_on_sure(void *menu_title_void);
extern app_result_e set_volume_limit_callback(void *menu_title_void);
extern app_result_e get_key_tone_setting_callback(void * title_action);
extern app_result_e enable_key_tone_setting_callback(void *menu_title_void);
extern app_result_e disable_key_tone_setting_callback(void *menu_title_void);

#endif
