/********************************************************************************
 *                            Module: music_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_MUSIC_SETMEN_H__
#define __APP_MUSIC_SETMEN_H__

#include "psp_includes.h"
#include "case_include.h"	

extern app_result_e set_scene_eqnormal_sure(void *param);

extern app_result_e set_scene_eqrock_sure(void *param);

extern app_result_e set_scene_eqfunk_sure(void *param);

extern app_result_e set_scene_eqhiphop_sure(void *param);

extern app_result_e set_scene_eqjazz_sure(void *param);

extern app_result_e set_scene_eqclassical_sure(void *param);

extern app_result_e set_scene_eqtechno_sure(void *param);

extern app_result_e set_scene_eqcustom_sure(void *param);

extern app_result_e set_scene_srs_wowhd_sure(void *param);

extern app_result_e set_scene_fullsoundon_sure(void *param);

extern bool set_scene_speed_callback(int16 speed_value);

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

#endif //__APP_MUSIC_SETMEN_H__
