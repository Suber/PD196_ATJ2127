/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ap_setting.h
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */

#ifndef _AP_SETTING_H
#define _AP_SETTING_H
#include "setting_res.h"
#include "setting_sty.h"
#include "case_include.h"
#include "psp_includes.h"
#include "enhanced.h"


#define MAX_APP_NUM 11
#define RAIDO_BUTTON 0x01
#define MSC_ONLINE 0x00
#define MTP_ONLINE 0x01
#define EQ_TOTAL         8    //含关闭选项
//////////
#define THEME_TOTAL          0x05
#define VALUE_THEME_BLUE     0x00
#define VALUE_THEME_GREEN    0x01
#define VALUE_THEME_GRAY     0x02
#define VALUE_THEME_PINK     0x03
#define VALUE_THEME_RED      0x04
#define LIGHT_TIMER_TOTAL    0x04
//////////
#define SCREEN_SAVER_TOTAL   0x05
#define VALUE_SCREEN_SAVER_OFF	0x00
#define VALUE_SCREEN_SAVER_CLOCK  0x01
#define VALUE_SCREEN_SAVER_PHOTOS 0x02
#define VALUE_SCREEN_OFF  0x03
#define VALUE_SCREEN_SAVER_DEMO 0x04

#define VALUE_12HOURS_FORMAT 0x00
#define VALUE_24HOURS_FORMAT 0x01

#define VALUE_TIME_HH_12H_MIN 1
#define VALUE_TIME_HH_24H_MIN 0
#define VALUE_TIME_HH_12H_MAX 12
#define VALUE_TIME_HH_24H_MAX 23
#define VALUE_TIME_HH_STEP 1

#define VALUE_TIME_MM_MIN 0
#define VALUE_TIME_MM_MAX 59
#define VALUE_TIME_MM_STEP 1

#define VALUE_DD_MM_YYYY	0x00
#define VALUE_MM_DD_YYYY    0x01
#define VALUE_YYYY_MM_DD    0x02

#define VALUE_DATE_DD_MIN   1
#define VALUE_DATE_DD_MAX   31
#define VALUE_DATE_DD_STEP  1

#define VALUE_DATE_MM_MIN  1
#define VALUE_DATE_MM_MAX  12
#define VALUE_DATE_MM_STEP 1 

#define VALUE_DATE_YY_MIN  0
#define VALUE_DATE_YY_MAX  98
#define VALUE_DATE_YY_STEP 1

#define VALUE_TIME_AM 0
#define VALUE_TIME_PM 1
#define VALUE_TIME_AMPM_STEP 1

#define VALUE_SLIDE_TIME_MIN  2
#define VALUE_SLIDE_TIME_MAX  30
#define VALUE_SLIDE_TIME_STEP 1 

#define VALUE_SLIDE_REPEAT_OFF   0x00     //此值由phioto AP决定
#define VALUE_SLIDE_REPEAT_ON    0x02

#define VALUE_SLIDE_SHUFFLE_OFF   0x00	  //此值由phioto AP决定
#define VALUE_SLIDE_SHUFFLE_ON    0x01

#define POSITION_AMPM 2

#define LANGUAGE_TOTAL 28
#define LANGUAGE_ID_SIMPLIFIED_CHINESE    CHINESE_SIMPLIFIED
#define LANGUAGE_ID_ENGLISH               ENGLISH
#define LANGUAGE_ID_TRAD_CHINESE          CHINESE_TRADITIONAL
#define LANGUAGE_ID_JAPANESE   JAPANESE
#define LANGUAGE_ID_KOREAN     KOREAN
#define LANGUAGE_ID_FRENCH     FRENCH
#define LANGUAGE_ID_GERMANY    GERMAN
#define LANGUAGE_ID_ITALIAN    ITALIAN
#define LANGUAGE_ID_DUTCH      DUTCH
#define LANGUAGE_ID_PORTUGUESE_EUROPEAN PORTUGUESE_EUROPEAN
#define LANGUAGE_ID_PORTUGUESE_BRAZILIAN PORTUGUESE_BRAZILIAN
#define LANGUAGE_ID_SPANISH    SPANISH
#define LANGUAGE_ID_SWEDISH    SWEDISH
#define LANGUAGE_ID_CZECH      CZECH
#define LANGUAGE_ID_DENISH     DANISH
#define LANGUAGE_ID_POLAND     POLISH
#define LANGUAGE_ID_RUSSIAN    RUSSIAN
#define LANGUAGE_ID_TURKEY     TURKISH
#define LANGUAGE_ID_THAI       THAI
#define LANGUAGE_ID_HUNGARIAN  HUNGARIAN
#define LANGUAGE_ID_ARABIC     ARABIC
#define LANGUAGE_ID_SLOVAK     SLOVAK
#define LANGUAGE_ID_GREEK      GREEK
#define LANGUAGE_ID_NORWEGIAN     NORWEGIAN
#define LANGUAGE_ID_FINNISH     FINNISH
#define LANGUAGE_ID_INDONESIAN     INDONESIAN
#define LANGUAGE_ID_HEBREW     HEBREW
#define LANGUAGE_ID_ROMANIAN   ROMANIAN

#define LANGUAGE_ID_CROATIAN   CHINESE_SIMPLIFIED
#define LANGUAGE_ID_SLOVENIAN  CHINESE_SIMPLIFIED

#define VALUE_STORAGE_MSC 0x00
#define VALUE_STORAGE_MTP 0x01

#define VALUE_DISABLE_AUTORUN 0x00
#define VALUE_ENABLE_AUTORUN 0x01

#define VALUE_DONT_SUPPORT_CARD 0x00
#define VALUE_SUPPORT_CARD 0x01


/*定义入口菜单编号*/
#define MENU_ENTRY_ID_MAIN        0x00
#define MENU_ENTRY_ID_MAIN_MTP 0x01
#define MENU_ENTRY_ID_SHUT_DOWN_ON      0x02
#define MENU_ENTRY_ID_SHUT_DOWN_OFF        0x03
#define MENU_ENTRY_ID_FORMAT_WITH_CARD        0x04
#define MENU_ENTRY_ID_FORMAT_WITHOUT_CARD    0x05


#define APP_TIMER_COUNT 0

#define TIPS_BUFFER_SIZE 100


typedef enum
{
    SETTING_MENU_LAYER, SCENE_EXIT
} scene_e;


typedef struct
{
    uint16 active_item;//活动AP菜单项编号
    uint16 total_item; //AP菜单总数
    uint16 ap_id[MAX_APP_NUM]; //支持应用编号的数组
} cfg_mainmenu_var_t;


typedef struct
{
    uint16 magic; //VM 变量魔数
    cfg_mainmenu_var_t cfg_var;
    //uint16 total_music;//音乐文件总数
    //uint16 total_video;//视频文件总数
    //uint16 total_picture;//图片文件总数
    //uint16 total_ebook;//电子书文件总数
    //uint16 total_station;//FM预设电台总数
    uint16 file_num[MAX_APP_NUM];//每个应用的文件总数
} g_mainmenu_var_t;


/////////////////////////
extern setting_comval_t g_setting_vars; //系统公共变量
extern style_infor_t style; //风格文件参数
extern textbox_private_t textbox_param; //文本控件参数
extern uint8 string_buffer[TIPS_BUFFER_SIZE];
extern menu_title_action_t menu_title;
extern uint8 *res_filename;
extern res_type_e res_type;
extern menu_com_data_t menu_com;
extern date_t date;
extern time_t time;
extern engine_type_e music_exist_flag;
extern uint8 playlist_exist_flag;
extern const uint8 theme_setting_res[5][13];
extern const uint8 themes_com[5][13];
extern uint8 date_dd_location, date_mm_location, date_yy_location;
extern int32 g_vfs_mount_id;
extern uint8 need_quit_ap_flag;
extern uint8 set_language_id_argv;

/////////////////////////

#endif
