/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MUSIC_NENU_CONFIG_H__
#define __APP_MUSIC_NENU_CONFIG_H__

#include "psp_includes.h"
#include "case_include.h"	


//第一级入口菜单
#define  LISTMENU_ENTRY         0
#define  LISTMENU_ENTRY_DIR     1

//第一级option菜单
#define  OPTION_MENU_MPLAYING   2   //正在播放
#define  OPTION_MENU_MPAUSE     3  //上一次播放
#define  OPTION_MENU_RPLAYING   4   //正在播放
#define  OPTION_MENU_RPAUSE     5  //radio上一次播放
//现在播放列表1 option菜单
#define  OPTION_FAVOR1_NORMAL   6
#define  OPTION_FAVOR1_MPLAYING 7
#define  OPTION_FAVOR1_MPAUSE   8
#define  OPTION_FAVOR1_RPLAYING 9
#define  OPTION_FAVOR1_RPAUSE   10

//现在播放列表2 1option菜单
#define  OPTION_FAVOR2_NORMAL   11
#define  OPTION_FAVOR2_MPLAYING 12
#define  OPTION_FAVOR2_MPAUSE   13
#define  OPTION_FAVOR2_RPLAYING 14
#define  OPTION_FAVOR2_RPAUSE   15

//现在播放列表1option菜单
#define  OPTION_FAVOR3_NORMAL   16
#define  OPTION_FAVOR3_MPLAYING 17
#define  OPTION_FAVOR3_MPAUSE   18
#define  OPTION_FAVOR3_RPLAYING 19
#define  OPTION_FAVOR3_RPAUSE   20

//各级播放列表option菜单
#define  OPTION_PLIST_NORMAL    21   //播放 + 加入播放列表
#define  OPTION_PLIST_MPLAING   22   //正在播放 + 播放 +加入播放列表
#define  OPTION_PLIST_MPAUSE    23   //上一次播放 + 播放 + 加入播放列表
#define  OPTION_PLIST_RPLAYING  24   //radio 正在播放 + 播放 + 加入播放列表
#define  OPTION_PLIST_RPAUSE    25   //radio 上一次播放 + 播放 + 加入播放列表
//收藏夹1列表option
#define  OPTION_FAVORLIST1_NORMAL    26
#define  OPTION_FAVORLIST1_MPLAYING  27
#define  OPTION_FAVORLIST1_MPAUSE    28
#define  OPTION_FAVORLIST1_RPLAYING  29
#define  OPTION_FAVORLIST1_RPAUSE    30

//收藏夹2列表option
#define  OPTION_FAVORLIST2_NORMAL    31
#define  OPTION_FAVORLIST2_MPLAYING  32
#define  OPTION_FAVORLIST2_MPAUSE    33
#define  OPTION_FAVORLIST2_RPLAYING  34
#define  OPTION_FAVORLIST2_RPAUSE    35

//收藏夹3列表option
#define  OPTION_FAVORLIST3_NORMAL    36
#define  OPTION_FAVORLIST3_MPLAYING  37
#define  OPTION_FAVORLIST3_MPAUSE    38
#define  OPTION_FAVORLIST3_RPLAYING  39
#define  OPTION_FAVORLIST3_RPAUSE    40

//audible的入口菜单
//#define  LISTMENU_AUDIBLE_NORMAL  40        //书籍+作者
#define  LISTMENU_AUDIBLE_RESUME     41       //续播[xxx] + 书籍 + 作者

//第一级入口菜单 设置菜单入口菜单
#define  MUSICSET_MENU               42

//audible设置菜单
#define  AUDIBLESET_MENU             43

//定时关机设置菜单
#define  SLEEP_TIMER_SETMENU         44

//列表中菜单项入口菜单
#define SHUFFLE_PLAY                 45

//所有歌曲
#define ALLSONG_PLAY                 46

//record voice播放入口菜单
#define VOICE_MENU_PLAYING           47

//record voice播放入口菜单
#define VOICE_MENU_LASTPLAY          48

//m3u列表嵌套收藏夹入口菜单
#define M3ULIST_MENU_FAVOR           49
//m3u列表option
#define OPTION_M3ULIST_NORMAL        50 //播放
#define OPTION_M3ULIST_MPLAYING      51 //播放 + 正在播放
#define OPTION_M3ULIST_MPAUSE        52 //播放 + 上一次播放
#define OPTION_M3ULIST_RPLAYING      53
#define OPTION_M3ULIST_RPAUSE        54
//menulist path_id定义
#define MUSIC_PATH_ID_MAIN            0
#define MUSIC_PATH_ID_MENU_PLAYLIST   1
#define MUSIC_PATH_ID_MENU_FAVORLIST  2
#define MUSIC_PATH_ID_LIST_PLAYLIST   3
#define MUSIC_PATH_ID_LIST_FAVORLIST  4
#define MUSIC_PATH_ID_SETMENU_MUSIC   5
#define MUSIC_PATH_ID_SETMENU_AUDIBLE 6
#define MUSIC_PATH_ID_LIST_M3ULIST    7



//for menu config functions
extern app_result_e list_scene_menu_option(void);

extern app_result_e list_scene_allsong_sure(void* param);

extern app_result_e list_scene_artist_sure(void* param);

extern app_result_e list_scene_album_sure(void* param);

extern app_result_e list_scene_genre_sure(void* param);

extern app_result_e list_scene_albumlist_sure(void *param);

extern app_result_e list_scene_favor_sure(void *param);

extern app_result_e list_scene_favor1_sure(void* param);

extern app_result_e list_scene_favor2_sure(void* param);

extern app_result_e list_scene_favor3_sure(void* param);

extern app_result_e list_scene_favor1_option(void);

extern app_result_e list_scene_favor2_option(void);

extern app_result_e list_scene_favor3_option(void);

extern app_result_e list_scene_audible_sure(void *param);

extern app_result_e list_scene_audresume_sure(void *param);

extern app_result_e list_scene_books_sure(void *param);

extern app_result_e list_scene_authors_sure(void *param);

extern app_result_e list_scene_option_option(void);

extern app_result_e list_scene_musicplay_sure(void *param);

extern app_result_e list_scene_musicpause_sure(void *param);

extern app_result_e list_scene_radioplay_sure(void *param);

extern app_result_e list_scene_radiomute_sure(void *param);

extern app_result_e list_scene_favor1play_sure(void *param);

extern app_result_e list_scene_favor2play_sure(void *param);

extern app_result_e list_scene_favor3play_sure(void *param);

extern app_result_e list_scene_shuffleplay_sure(void* param);

extern app_result_e list_scene_allsongplay_sure(void* param);

extern app_result_e list_scene_listplay_sure(void *param);

extern app_result_e list_scene_addfavor1_sure(void *param);

extern app_result_e list_scene_addfavor2_sure(void *param);

extern app_result_e list_scene_addfavor3_sure(void *param);

extern app_result_e list_scene_delfavor_sure(void *param);

extern app_result_e list_scene_clrfavor_sure(void *param);

extern app_result_e set_scene_leveltop_option(void);

extern app_result_e set_scene_levelother_option(void);

extern app_result_e set_scene_sleeptimer_sure(void *param);

extern app_result_e set_scene_delete_sure(void *param);

extern app_result_e set_scene_fromallsong_sure(void *param);

extern app_result_e set_scene_fromartist_sure(void *param);

extern app_result_e set_scene_fromalbum_sure(void *param);

extern app_result_e set_scene_fromgenre_sure(void *param);

extern app_result_e set_scene_repeat_sure(void *param);

extern app_result_e set_scene_repeatoff_sure(void *param);

extern app_result_e set_scene_repeatone_sure(void *param);

extern app_result_e set_scene_repeatall_sure(void *param);

extern app_result_e set_scene_repeatintro_sure(void *param);

extern app_result_e set_scene_shuffle_sure(void* param);

extern app_result_e set_scene_shuffleoff_sure(void *param);

extern app_result_e set_scene_shuffleon_sure(void *param);

extern app_result_e set_scene_abmode_sure(void *param);

extern app_result_e set_scene_setabcount_sure(void *param);

extern app_result_e set_scene_setabgap_sure(void *param);

extern app_result_e set_scene_fullsound_sure(void* param);

extern app_result_e set_scene_fullsoundoff_sure(void *param);

extern app_result_e set_scene_fullsoundon_sure(void *param);

extern app_result_e set_scene_eq_sure(void* param);

extern app_result_e set_scene_eqnormal_sure(void *param);

extern app_result_e set_scene_eqrock_sure(void *param);

extern app_result_e set_scene_eqfunk_sure(void *param);

extern app_result_e set_scene_eqhiphop_sure(void *param);

extern app_result_e set_scene_eqjazz_sure(void *param);

extern app_result_e set_scene_eqclassical_sure(void *param);

extern app_result_e set_scene_eqtechno_sure(void *param);

extern app_result_e set_scene_eqcustom_sure(void *param);

extern app_result_e set_scene_speed_sure(void *param);

extern app_result_e set_scene_vollimit_sure(void *param);

extern app_result_e set_scene_addfavor1_sure(void *param);

extern app_result_e set_scene_addfavor2_sure(void *param);

extern app_result_e set_scene_addfavor3_sure(void *param);

extern app_result_e set_scene_delfavor1_sure(void *param);

extern app_result_e set_scene_delfavor2_sure(void *param);

extern app_result_e set_scene_delfavor3_sure(void *param);

extern app_result_e set_scene_closesleep_sure(void *param);

extern app_result_e set_scene_setsleep_sure(void *param);

extern app_result_e set_scene_bookmark_add_sure(void *param);

extern app_result_e set_scene_bookmark_sel_sure(void *param);

extern app_result_e set_scene_bookmark_del_sure(void *param);

extern app_result_e set_scene_delaudible_sure(void *param);

extern app_result_e set_scene_switchmode_sure(void *param);

extern app_result_e set_scene_setsection_sure(void *param);

extern app_result_e set_scene_settitle_sure(void *param);

extern app_result_e set_scene_srs_sure(void* param);

extern app_result_e set_scene_srsoff_sure(void *param);

extern app_result_e set_scene_srs_wowhd_sure(void *param);

extern app_result_e set_scene_srs_user_sure(void *param);

//实时叶子菜单回调函数
extern void set_scene_eqnormal_callback(void);

extern void set_scene_fullsoundon_callback(void);

extern void set_scene_eqrock_callback(void);

extern void set_scene_eqfunk_callback(void);

extern void set_scene_eqhiphop_callback(void);

extern void set_scene_eqjazz_callback(void);

extern void set_scene_eqclassic_callback(void);

extern void set_scene_eqtechno_callback(void);

extern void set_scene_equser_callback(void);

extern void set_scene_srswowhd_callback(void);

extern void set_scene_srsuser_callback(void);

extern app_result_e list_scene_create_sure(void *param);

extern app_result_e list_scene_maindisk_sure(void *param);

extern app_result_e list_scene_carddisk_sure(void *param);

extern app_result_e list_scene_list_option_callback(file_path_info_t *path_info, uint16 active);

extern app_result_e list_scene_addfavor_sure(void *param);

extern app_result_e set_scene_addfavor_sure(void *param);

#ifdef PC

extern menu_cb_option get_menu_option(uint16 str_id);

extern menu_cb_leaf get_callback_func(uint16 str_id);

extern menu_cb_func get_menu_func(uint16 str_id);

#endif

#endif //__APP_MUSIC_NENU_CONFIG_H__
