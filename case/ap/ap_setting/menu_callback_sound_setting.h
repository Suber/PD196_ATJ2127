#ifndef _MENU_CALLBACK_SOUND_SETTING_H
#define _MENU_CALLBACK_SOUND_SETTING_H
#include "case_include.h"

extern uint8 g_eq_restore;

extern bool check_close_fullsound(void);
extern bool check_close_srs(void);
extern bool set_scene_speed_callback(int16 speed_value);

extern void set_eq_normal_callback(void);
extern void set_eq_rock_callback(void);
extern void set_eq_funk_callback(void);
extern void set_eq_hiphop_callback(void);
extern void set_eq_jazz_callback(void);
extern void set_eq_classical_callback(void);
extern void set_eq_techno_callback(void);
extern void set_eq_custom_callback(void);
extern void set_eq_wowhd_callback(void);
extern void set_user_srs_on_callback(void);
extern void set_eq_fullsoundon_callback(void);

extern app_result_e get_eq_option_callback(void *menu_title_void);
extern app_result_e get_srs_option_callback(void *menu_title_void);
extern app_result_e get_fullsound_option_callback(void * title_action);

extern app_result_e set_eq_normal_sure(void *menu_title_void);
extern app_result_e set_eq_rock_sure(void *menu_title_void);
extern app_result_e set_eq_funk_sure(void *menu_title_void);
extern app_result_e set_eq_hip_hop_sure(void *menu_title_void);
extern app_result_e set_eq_jazz_sure(void *menu_title_void);
extern app_result_e set_eq_classical_sure(void *menu_title_void);
extern app_result_e set_eq_techno_sure(void *menu_title_void);
extern app_result_e set_srs_off_sure(void *menu_title_void);
extern app_result_e set_wowhd_on_sure(void *menu_title_void);
extern app_result_e fullsound_off_sure(void *menu_title_void);
extern app_result_e fullsound_on_sure(void *menu_title_void);

extern app_result_e setting_callback(private_msg_t *private_msg);
#endif
