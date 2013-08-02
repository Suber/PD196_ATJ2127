#ifndef _MENU_CALLBACK_PLAY_MODE_H
#define _MENU_CALLBACK_PLAY_MODE_H
#include "case_include.h"
   
extern app_result_e option_callback(void);
extern app_result_e set_music_from_all_songs_callback(void *menu_title_void);
extern app_result_e set_music_from_artist_callback(void *menu_title_void);
extern app_result_e set_music_from_album_callback(void *menu_title_void);
extern app_result_e set_music_from_genre_callback(void *menu_title_void);
extern app_result_e get_music_repeat_mode_id_callback(void *menu_title_void);
extern app_result_e set_music_repeat_off_callback(void *menu_title_void);
extern app_result_e set_music_repeat_one_callback(void *menu_title_void);
extern app_result_e set_music_repeat_all_callback(void *menu_title_void);
extern app_result_e set_music_intro_callback(void *menu_title_void);
extern app_result_e get_music_shuflle_option(void *menu_title_void);
extern app_result_e set_music_shuffle_off_callback(void *menu_title_void);
extern app_result_e set_music_shuffle_on_callback(void *menu_title_void);
extern app_result_e get_fullsound_option_callback(void *menu_title_void);
extern app_result_e fullsound_off_callback(void *menu_title_void);
extern app_result_e fullsound_on_callback(void *menu_title_void);
extern void restore_factory_setting_for_music(void);
#endif


