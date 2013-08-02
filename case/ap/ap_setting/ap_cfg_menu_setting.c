/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ap_cfg_menu_setting.c
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_setting.h"
#include "ap_setting_callback.h"

#define ENTRY_TOTAL 4
#define ITEM_TOTAL 129


/* define variable, 变量定义 */
/*定义入口菜单*/
const conf_item_head_t entrymenu =
{ "ENTRY MENU  ", ENTRY_TOTAL };

const conf_menu_entry_t entry[ENTRY_TOTAL] =
{
    { MENU_ENTRY_ID_MAIN, S_SETTING_TITLE, S_STORAGE_MSC },
    { MENU_ENTRY_ID_MAIN_MTP, S_SETTING_TITLE, S_STORAGE_MTP },
    { MENU_ENTRY_ID_SHUT_DOWN_ON, S_SHUT_DOWN_TIMER_TITLE, S_SHUT_DOWN_ON_TITLE },
    { MENU_ENTRY_ID_SHUT_DOWN_OFF, S_SHUT_DOWN_TIMER_TITLE, S_SHUT_DOWN_OFF_TITLE }
};

/*定义叶子菜单项*/
const conf_item_head_t item_head =\
{ "MENU ITEM   ", ITEM_TOTAL };

const conf_menu_item_t item[ITEM_TOTAL] =
{
/*一级菜单项-播放模式*/
    { S_PLAY_MODE, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,0,0}, /*播放模式*/
/*播放模式-音乐来源（二级菜单）*/
    { S_MUSIC_FROM, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,1,0}, /*音乐来源于...*/
/*播放模式-音乐来源方式（三级菜单）*/
 /*...来源于所有文件*/
    { S_FROM_ALL_SONGS, 0, set_music_from_all_songs_callback, NULL, option_callback, NORMAL_MENU_ITEM ,2,0},
/*...来源于此演唱者*/
    { S_FROM_ARTIST, 0, set_music_from_artist_callback, NULL, option_callback, NORMAL_MENU_ITEM ,3,0},
/*...来源于此专辑*/
    { S_FROM_ALBUM, 0, set_music_from_album_callback, NULL, option_callback, NORMAL_MENU_ITEM ,4,0},
/*...来源于此类风格*/
    { S_FROM_GENRE, 0, set_music_from_genre_callback, NULL, option_callback, NORMAL_MENU_ITEM ,5,0},
/*播放模式-重复播放（二级菜单）*/
/*重复播放*/
    {
        S_REPEAT, S_MUSIC_REPEAT_MODE, get_music_repeat_mode_id_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,6,0
    },
/*播放模式-重复播放方式（三级菜单）*/
/*重复播放关*/
    { S_REPEAT_OFF, S_MUSIC_REPEAT_OFF, set_music_repeat_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,7,0 },
    {
        S_REPEAT_ONE, 0, set_music_repeat_one_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,8,0
    }, /*单曲重复*/
    { S_REPEAT_ALL, 0, set_music_repeat_all_callback, NULL, option_callback, RAIDO_MENU_ITEM ,9,0 }, /*全部重复*/
    { S_REPEAT_INTRO, 0, set_music_intro_callback, NULL, option_callback, RAIDO_MENU_ITEM ,134,0 }, /*全部重复*/
/*播放模式-随机播放（二级菜单）*/
/*随机播放*/
    { S_SHUFFLE, S_MUSIC_SHUFFLE_OPTION, get_music_shuflle_option, NULL, option_callback, NORMAL_MENU_ITEM ,10,0 },
/*播放模式-随机播放方式（三级菜单）*/
/*随机播放关*/
    { S_OFF, S_MUSIC_SHUFFLE_OFF, set_music_shuffle_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,11,0 },
/*随机播放开*/
    { S_ON, S_MUSIC_SHUFFLE_ON, set_music_shuffle_on_callback, NULL, option_callback, RAIDO_MENU_ITEM ,12,0 },
/*一级菜单项-声音设置*/
    { S_SOUND_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,13,0 }, /*声音设置*/
/*声音设置-fullsound（二级菜单）*/
    {
        S_FULLSOUND, 0, get_fullsound_option_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,14,0
    }, /*fullsound*/
/*声音设置-fullsound选项（三级菜单）*/
    {
        S_OFF, S_FULLSOUND_OFF, fullsound_off_sure, set_eq_normal_callback,
        option_callback, RAIDO_MENU_ITEM ,15,0
    }, /*fullsound关*/
    {
        S_ON, S_FULLSOUND_ON, fullsound_on_sure, set_eq_fullsoundon_callback,
        option_callback, RAIDO_MENU_ITEM ,16,0
    }, /*fullsound开*/
/*声音设置-均衡器（二级菜单）*/
    { S_EQUALIZER, 0, get_eq_option_callback, NULL, option_callback, NORMAL_MENU_ITEM ,17,0 }, /*均衡器*/
/*声音设置-均衡器选项（三级菜单）*/
    { S_OFF, S_EQ_OFF, set_eq_normal_sure, set_eq_normal_callback, option_callback, RAIDO_MENU_ITEM ,18,0 }, /*均衡关*/
    { S_EQ_ROCK, 0, set_eq_rock_sure, set_eq_rock_callback, option_callback, RAIDO_MENU_ITEM ,19,0 }, /*摇滚*/
    { S_EQ_FUNK, 0, set_eq_funk_sure, set_eq_funk_callback, option_callback, RAIDO_MENU_ITEM ,20,0 }, /*放克*/
/*希普霍普*/
    { S_EQ_HIP_HOP, 0, set_eq_hip_hop_sure, set_eq_hiphop_callback, option_callback, RAIDO_MENU_ITEM ,21,0 },
    { S_EQ_JAZZ, 0, set_eq_jazz_sure, set_eq_jazz_callback, option_callback, RAIDO_MENU_ITEM ,22,0 }, /*爵士*/
/*古典*/
    {
        S_EQ_CLASSICAL, 0, set_eq_classical_sure, set_eq_classical_callback,
        option_callback, RAIDO_MENU_ITEM ,23,0
    },
/*电子乐*/
    { S_EQ_TECHNO, 0, set_eq_techno_sure, set_eq_techno_callback, option_callback, RAIDO_MENU_ITEM ,24,0 },
 /*自定义*/
    { S_EQ_CUSTOM, 0, set_eq_custom_sure, set_eq_custom_callback, option_callback, RAIDO_MENU_ITEM ,25,0 },
/*声音设置-srs（二级菜单）*/
    { S_SRS, 0, get_srs_option_callback, NULL, option_callback, NORMAL_MENU_ITEM ,26,0 }, /*srs*/
/*声音设置-srs选项（三级菜单）*/
    /*srs关*/
    { S_OFF, S_SRS_OFF, set_srs_off_sure, set_eq_normal_callback, option_callback, RAIDO_MENU_ITEM ,27,0 }, 
    /*wowhd开*/
    { S_WOWHD, 0, set_wowhd_on_sure, NULL, option_callback, RAIDO_MENU_ITEM ,28,0 }, 
    /*user srs开*/
    { S_USER_SRS, 0, set_user_srs_on_sure, set_user_srs_on_callback, option_callback, RAIDO_MENU_ITEM ,29,0 }, 
/*声音设置-音量限制（二级菜单）*/
    {
        S_VOLUME_LIMIT, 0, set_volume_limit_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,30,0
    }, /*音量限制*/
/*一级菜单项-定时关机*/
    {
        S_SHUT_DOWN_TIMER_TITLE, 0, get_shut_down_entry, NULL,
        option_callback, NORMAL_MENU_ITEM ,31,0
    }, /*定时关机*/
/*定时关机选项（二级菜单）*/
/*关闭定时关机*/
    { S_OFF, S_SHUT_DOWN_TIMER_OFF, shut_down_timer_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,32,0 },
/*设置定时关机*/
    { S_SHUT_DOWN_SETTING, 0, shut_down_time_adjust_callback, NULL, option_callback, RAIDO_MENU_ITEM ,33,0 },
/*一级菜单项-显示设置*/
    { S_DISPLAY_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,34,0 }, /*显示设置*/
/*显示设置-亮度（二级菜单）*/
    {
        S_BRIGHTNESS_SETTING, 0, set_brightness_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,35,0
    }, /*亮度调节*/
/*显示设置-主题（二级菜单）*/
    { S_THEME, 0, get_theme_option_callback, NULL, option_callback, NORMAL_MENU_ITEM ,36,0 }, /*主题*/
/*显示设置-主题选项（三级菜单）*/
    { S_THEME_BLUE, 0, set_theme_blue_callback, NULL, option_callback, RAIDO_MENU_ITEM ,37,0 }, /*蓝色主题*/
    { S_THEME_GREEN, 0, set_theme_green_callback, NULL, option_callback, RAIDO_MENU_ITEM ,38,0 }, /*绿色主题*/
    { S_THEME_GRAY, 0, set_theme_gray_callback, NULL, option_callback, RAIDO_MENU_ITEM ,39,0 }, /*灰色主题*/
    { S_THEME_PINK, 0, set_theme_pink_callback, NULL, option_callback, RAIDO_MENU_ITEM ,40,0 }, /*粉红色主题*/
    { S_THEME_RED, 0, set_theme_red_callback, NULL, option_callback, RAIDO_MENU_ITEM ,41,0 }, /*红色主题*/
/*显示设置-背光计时器（二级菜单）*/
/*背光计时器*/
    { S_BACKLIGHT_TIMER, 0, get_backlight_timer_option_callback, NULL, option_callback, NORMAL_MENU_ITEM ,42,0 },
/*显示设置-背光计时器选项（三级菜单）*/
    {
        S_LIGHT_TIME_10S, 0, set_light_time_10s_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,43,0
    }, /*背光10秒*/
    {
        S_LIGHT_TIME_20S, 0, set_light_time_20s_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,44,0
    }, /*背光20秒*/
    {
        S_LIGHT_TIME_30S, 0, set_light_time_30s_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,45,0
    }, /*背光30秒*/
/*始终亮起*/
    { S_LIGHT_TIME_ALWAYS, 0, set_light_time_always_callback, NULL, option_callback, RAIDO_MENU_ITEM ,46,0 },
/*显示设置-屏幕保护（二级菜单）*/
    {
        S_SCREEN_SAVER, 0, get_screen_saver_option_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,47,0
    }, /*屏幕保护*/
/*显示设置-屏幕保护选项（三级菜单）*/
/*关闭屏幕保护*/
    { S_WITHOUT_SCREEN_SAVER, 0, set_screen_saver_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,48,0 },
/*数字时钟*/
    { S_SCREEN_SAVER_CLOCK, 0, set_screen_saver_clock_callback, NULL, option_callback, RAIDO_MENU_ITEM ,49,0 },
/*相册图画*/
    { S_SCREEN_SAVER_PHOTOS, 0, set_screen_saver_photos_callback, NULL, option_callback, RAIDO_MENU_ITEM ,50,0 },
    { S_SCREEN_OFF, 0, set_screen_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,51,0 }, /*关闭屏幕*/
/*演示模式*/
    { S_SCREEN_SAVER_DEMO, 0, set_screen_saver_demo_callback, NULL, option_callback, RAIDO_MENU_ITEM ,52,0 },
/*一级菜单项-日期和时间*/
    { S_DATE_AND_TIME, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,53,0 }, /*日期和时间*/
/*日期和时间-时间设置（二级菜单）*/
    { S_TIME_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,54,0}, /*时间设置*/
/*日期和时间-时间设置-设置时间格式（三级菜单）*/
    {
        S_SET_TIME_FORMAT, 0, get_time_format_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,55,0
    }, /*设置时间格式*/
/*日期和时间-时间设置-设置时间格式-时间格式选项（四级菜单）*/
/*设置为12小时制*/
    { S_TIME_FORMAT_12H, 0, set_time_format_12h_callback, NULL, option_callback, RAIDO_MENU_ITEM ,56,0 },
/*设置为24小时制*/
    { S_TIME_FORMAT_24H, 0, set_time_format_24h_callback, NULL, option_callback, RAIDO_MENU_ITEM ,57,0 },
/*日期和时间-时间设置-设置时间（三级菜单）*/
    { S_SET_TIME, 0, set_time_callback, NULL, option_callback, NORMAL_MENU_ITEM ,58,0 }, /*设置时间*/
/*日期和时间-日期设置（二级菜单）*/
    { S_DATE_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,59,0 }, /*日期设置*/
/*日期和时间-日期设置格式（三级菜单）*/
    {
        S_SET_DATE_FORMAT, 0, get_date_format_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,60,0
    }, /*设置日期格式*/
/*日期和时间-日期设置-日期格式-日期格式选项（四级菜单）*/
/*设置日期为dd-mm-yyyy格式*/
    {
        S_DATE_FORMAT_DD_MM_YYYY, 0, set_date_format_day_first_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,61,0
    },
/*设置日期为mm-dd-yyyy格式*/
    {
        S_DATE_FORMAT_MM_DD_YYYY, 0, set_date_format_month_first_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,62 ,0
    },
/*设置日期为yyyy-mm-dd格式*/
    {
        S_DATE_FORMAT_YYYY_MM_DD, 0, set_date_format_year_first_callback, NULL,
        option_callback, RAIDO_MENU_ITEM ,63 ,0
    },
/*日期和时间-设置日期（三级菜单）*/
    { S_SET_DATE, 0, set_date_callback, NULL, option_callback, NORMAL_MENU_ITEM ,64,0 }, /*设置日期*/
/*一级菜单项-幻灯片放映设置*/
    { S_SLIDE_SHOW_SETTING, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,65,0}, /*幻灯片放映设置*/
/*幻灯片放映设置-每张幻灯片的时间（二级菜单）*/
/*每张幻灯片时间*/
    { S_TIME_PER_SLIDE, 0, set_time_per_slide_callback, NULL, option_callback, NORMAL_MENU_ITEM ,66,0 },
/*幻灯片放映设置-重复播放（二级菜单）*/
/*幻灯重复播放*/
    { S_REPEAT, S_SLIDE_REPEAT_MODE, get_slide_repeat_mode, NULL, option_callback, NORMAL_MENU_ITEM ,67,0 },
/*幻灯片放映设置-重复播放选项（三级菜单）*/
/*幻灯repeat关*/
    { S_OFF, S_SLIDE_REPEAT_OFF, set_slide_repeat_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,68,0 },
/*幻灯repeat开*/
    { S_ON, S_SLIDE_REPEAT_ON, set_slide_repeat_on_callback, NULL, option_callback, RAIDO_MENU_ITEM ,69,0 },
/*幻灯片放映设置-随机播放（二级菜单）*/
/*幻灯随机播放*/
    { S_SHUFFLE, S_SLIDE_SHUFFLE_OPTION, get_slide_shuffle_mode, NULL, option_callback, NORMAL_MENU_ITEM ,70,0 },
/*幻灯片放映设置-随机播放选项（三级菜单）*/
/*幻灯shuffle关*/
    { S_OFF, S_SLIDE_SHUFFLE_OFF, set_slide_shuffle_off_callback, NULL, option_callback, RAIDO_MENU_ITEM ,71,0 },
/*幻灯shuffle开*/
    { S_ON, S_SLIDE_SHUFFLE_ON, set_slide_shuffle_on_callback, NULL, option_callback, RAIDO_MENU_ITEM ,72,0 },
/*一级菜单项-language/语言*/
    { S_LANGUAGE, 0, get_language_id_callback, NULL, option_callback, NORMAL_MENU_ITEM ,73,0 }, /*language/语言*/
/*language/语言-语言选项（二级菜单）*/
    {
        S_LANGUAGE_CZECH, SN_LANGUAGE_CZECH, set_czech_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,74,0
    }, /*捷克语*/
    {
        S_LANGUAGE_GREEK, SN_LANGUAGE_GREEK, set_greek_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,75,0
    }, /*希腊语*/
    {
        S_LANGUAGE_ENGLISH, SN_LANGUAGE_ENGLISH, set_english_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,76,0
    }, /*英语*/
    {
        S_LANGUAGE_DENISH, SN_LANGUAGE_DENISH, set_denish_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,77,0
    }, /*丹麦语*/
    {
        S_LANGUAGE_GERMANY, SN_LANGUAGE_GERMANY, set_germany_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,78,0
    }, /*德语*/
    {
        S_LANGUAGE_SPANISH, SN_LANGUAGE_SPANISH, set_spanish_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,79,0
    }, /*西班牙语*/
    {
        S_LANGUAGE_FRENCH, SN_LANGUAGE_FRENCH, set_french_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,80,0
    }, /*法语*/
    {
        S_LANGUAGE_ITALIAN, SN_LANGUAGE_ITALIAN, set_italian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,81,0
    }, /*意大利语*/
    {
        S_LANGUAGE_HUNGARIAN, SN_LANGUAGE_HUNGARIAN, set_hungarian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,82,0
    }, /*匈牙利语*/
    {
        S_LANGUAGE_DUTCH, SN_LANGUAGE_DUTCH, set_dutch_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,83,0
    }, /*荷兰语*/
    {
        S_LANGUAGE_NORWEGIAN, SN_LANGUAGE_NORWEGIAN, set_norwegian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,84,0
    }, /*挪威语*/
    {
        S_LANGUAGE_POLAND, SN_LANGUAGE_POLAND, set_poland_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,85,0
    }, /*波兰语*/
    {
        S_LANGUAGE_PORTUGUESE_EUROPEAN, SN_LANGUAGE_PORTUGUESE_EUROPEAN, set_portuguese_european_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,86,0
    }, /*葡萄牙语-欧洲*/
    {
        S_LANGUAGE_PORTUGUESE_BRAZILIAN, SN_LANGUAGE_PORTUGUESE_BRAZILIAN, set_portuguese_brazilian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,87,0
    }, /*葡萄牙语-巴西*/
    {
        S_LANGUAGE_RUSSIAN, SN_LANGUAGE_RUSSIAN, set_russian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,88,0
    }, /*俄语*/
    {
        S_LANGUAGE_SLOVAK, SN_LANGUAGE_SLOVAK, set_slovak_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,89,0
    }, /*斯洛伐克语*/
    {
        S_LANGUAGE_FINNISH, SN_LANGUAGE_FINNISH, set_finnish_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,90,0
    }, /*芬兰语*/
    {
        S_LANGUAGE_SWEDISH, SN_LANGUAGE_SWEDISH, set_swedish_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,91,0
    }, /*瑞典语*/
    {
        S_LANGUAGE_TURKEY, SN_LANGUAGE_TURKEY, set_turkey_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,92,0
    }, /*土耳其语*/
    {
        S_LANGUAGE_SIMPLIFIED_CHINESE, SN_LANGUAGE_SIMPLIFIED_CHINESE, set_simplified_chinese_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,93,0
    },/*简体中文*/
    {
        S_LANGUAGE_TRAD_CHINESE, SN_LANGUAGE_TRAD_CHINESE, set_trad_chinese_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,94,0
    }, /*繁体中文*/
    {
        S_LANGUAGE_JAPANESE, SN_LANGUAGE_JAPANESE, set_japanese_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,95,0
    }, /*日语*/
    {
        S_LANGUAGE_KOREAN, SN_LANGUAGE_KOREAN, set_korean_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,96,0
    }, /*韩文*/
    {
        S_LANGUAGE_HEBREW, SN_LANGUAGE_HEBREW, set_hebrew_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,97,0
    }, /*希伯来语*/
    {
        S_LANGUAGE_ARABIC, SN_LANGUAGE_ARABIC, set_arabic_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,98,0
    }, /*阿拉伯语*/
    {
        S_LANGUAGE_THAI, SN_LANGUAGE_THAI, set_thai_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,99,0
    }, /*泰文*/
    {
        S_LANGUAGE_ROMANIAN, SN_LANGUAGE_ROMANIAN, set_romanian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,100,0
    }, /*罗马语*/
    {
        S_LANGUAGE_INDONESIAN, SN_LANGUAGE_INDONESIAN, set_indonesian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,101,0
    }, /*印尼语*/
    {
        S_LANGUAGE_CROATIAN, SN_LANGUAGE_CROATIAN, set_croatian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,102,0
    }, /*克罗地亚语*/
    {
        S_LANGUAGE_SLOVENIAN, SN_LANGUAGE_SLOVENIAN, set_slovenian_callback,
        NULL, option_callback, RAIDO_MENU_ITEM  ,103,0
    }, /*斯洛文尼亚语*/


/*一级菜单项-电脑连接首选项*/
/*电脑连接首选项*/
    {
        S_PC_CONNECT_OPTION, 0, get_pc_connect_information_id_callback, NULL,
        option_callback, NORMAL_MENU_ITEM,110 ,0
    },
/*电脑连接首选项-选择项（二级菜单）*/
    { S_STORAGE_MSC, 0, set_storage_msc_callback, NULL, option_callback, RAIDO_MENU_ITEM ,111,0 }, /*设置为msc*/
    { S_STORAGE_MTP, 0, set_storage_mtp_callback, NULL, option_callback, RAIDO_MENU_ITEM ,112,0 }, /*设置为mtp*/
    { S_EXPLAIN_MSC, 0, explain_msc_callback, NULL, option_callback, NORMAL_MENU_ITEM ,113,0 }, /*解释msc*/
    { S_EXPLAIN_MTP, 0, explain_mtp_callback, NULL, option_callback, NORMAL_MENU_ITEM ,114,0 }, /*解释mtp*/
/*一级菜单项-信息*/
    { S_INFORMATION, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,115,0 }, /*信息*/
/*信息-播放器信息（二级菜单）*/
/*播放器信息*/
    { S_PLAYER_INFORMATION, 0, show_player_information_callback, NULL, option_callback, NORMAL_MENU_ITEM ,116,0 },
/*信息-磁盘空间信息（二级菜单）*/
/*磁盘空间*/
    { S_DISK_INFORMATION, 0, show_disk_information_callback, NULL, option_callback, NORMAL_MENU_ITEM ,117,0 },
/*信息-支持文件（二级菜单）*/
    { S_SUPPORT_FILES, 0, NULL, NULL, option_callback, NORMAL_MENU_ITEM ,118,0 }, /*支持文件类型*/
/*信息-支持文件-文件类型（三级菜单）*/
    { S_MUSIC, 0, show_music_type_callback, NULL, option_callback, NORMAL_MENU_ITEM ,119 ,0}, /*音乐类型*/
    { S_PICTURE, 0, show_picture_type_callback, NULL, option_callback, NORMAL_MENU_ITEM ,120,0 }, /*图片类型*/
    { S_VIDEO, 0, show_video_type_callback, NULL, option_callback, NORMAL_MENU_ITEM ,121,0 }, /*视频类型*/
/*一级菜单项-法律信息*/
/*法律信息*/
    { S_LEGAL_INFORMATION, 0, show_legal_information_callback, NULL, option_callback, NORMAL_MENU_ITEM ,122,0 },
/*一级菜单项-格式化设备*/
/*格式化设备*/
    { S_FORMAT_DEVICE, 0, get_format_device_entry, NULL, option_callback, NORMAL_MENU_ITEM ,123,0 },
/*格式化设备-格式化选项（二级菜单）*/
    { S_FORMAT_DISK, 0, format_disk_callback, NULL, option_callback, NORMAL_MENU_ITEM ,124,0 }, /*格式化主盘*/
    { S_FORMAT_CARD, 0, format_card_callback, NULL, option_callback, NORMAL_MENU_ITEM ,125,0 }, /*格式化卡盘*/
    { S_DONT_FORMAT, 0, dont_format_callback, NULL, option_callback, NORMAL_MENU_ITEM ,126,0}, /*不格式化*/
/*一级菜单项-出厂设定*/
/*出厂设定*/
    {
        S_FACTORY_SETTING, 0, dialog_restore_factory_setting_callback, NULL,
        option_callback, NORMAL_MENU_ITEM ,127,0
    },
/*一级菜单项-CD安装程序*/
    { S_AUTORUN_SETTING, 0, get_autorun_setting_callback, NULL, option_callback, NORMAL_MENU_ITEM ,128,0 },
/*CD安装程序-设定选项（二级菜单）*/
/*禁止自动运行*/
    { S_DISABLE, S_DISABLE_AUTORUN, disable_autorun_callback, NULL, option_callback, RAIDO_MENU_ITEM ,129,0 },
/*使能自动运行*/
    { S_ENABLE, S_ENABLE_AUTORUN, enable_autorun_callback, NULL, option_callback, RAIDO_MENU_ITEM ,130,0 },
/*按键音*/
    {S_KEY_TONE, 0, get_key_tone_setting_callback, NULL, option_callback, NORMAL_MENU_ITEM ,131,0},
    {S_ON, SN_KEY_TONE_ON, enable_key_tone_setting_callback, NULL, option_callback, RAIDO_MENU_ITEM ,132,0},
    {S_OFF, SN_KEY_TONE_OFF, disable_key_tone_setting_callback, NULL, option_callback, RAIDO_MENU_ITEM ,133,0}
};



#ifdef PC
menu_cb_func _get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_func;
        }
    }

    return NULL;
}

menu_cb_leaf _get_callback_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].callback;
        }
    }

    return NULL;
}

menu_cb_option _get_menu_option(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_option;
        }
    }

    return NULL;
}
#endif

