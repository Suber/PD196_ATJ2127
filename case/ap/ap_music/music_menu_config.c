/*******************************************************************************
 *                              US212A
 *                            Module: music ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     fiona.yang   2011-09-26          1.0              create this file
 *******************************************************************************/

#include "app_music.h"

#include  "app_music_menu_config.h"

#define ENTRY_TOTAL 55
#define ITEM_TOTAL  89

//定义入口菜单
const conf_entry_head_t music_entrymenu =
{ "ENTRY MENU  ", ENTRY_TOTAL};

//定义各入口菜单项目
const conf_menu_entry_t entryitems[] =
{
    { LISTMENU_ENTRY, S_ALLSONG, 0 }, //list menu入口菜单

    { LISTMENU_ENTRY_DIR, S_LOCAL_FOLDER, 0 }, //list menu入口菜单(卡存在的情况下)

    { OPTION_MENU_MPLAYING, S_NOW_PLAYING, S_MUSIC },//list menu option music正在播放 (audible列表 相同)
    { OPTION_MENU_MPAUSE, S_LAST_PLAY, S_MUSIC },//list menu option music上一次播放(audible列表 相同)
    { OPTION_MENU_RPLAYING, S_NOW_PLAYING, S_RADIO },//list menu option radio正在播放(audible列表 相同)
    { OPTION_MENU_RPAUSE, S_LAST_PLAY, S_RADIO },//list menu option radio上一次播放(audible列表 相同)

    { OPTION_FAVOR1_NORMAL, S_NOW_FAVORITE1, S_MENU_OPTION1 },//favor1 menu option play
    { OPTION_FAVOR1_MPLAYING, S_NOW_FAVORITE1, S_MENU_OPTION2 },//favor1 menu option music playing+paly
    { OPTION_FAVOR1_MPAUSE, S_NOW_FAVORITE1, S_MENU_OPTION3 },//favor1 menu option music lastplay+play
    { OPTION_FAVOR1_RPLAYING, S_NOW_FAVORITE1, S_MENU_OPTION4 },//favor1 menu option radio playing+play
    { OPTION_FAVOR1_RPAUSE, S_NOW_FAVORITE1, S_MENU_OPTION5 },//favor1 menu option radio lastplay+play

    { OPTION_FAVOR2_NORMAL, S_NOW_FAVORITE2, S_MENU_OPTION1 },//favor2 menu option play
    { OPTION_FAVOR2_MPLAYING, S_NOW_FAVORITE2, S_MENU_OPTION2 },//favor2 menu option music playing+paly
    { OPTION_FAVOR2_MPAUSE, S_NOW_FAVORITE2, S_MENU_OPTION3 },//favor2 menu option music lastplay+play
    { OPTION_FAVOR2_RPLAYING, S_NOW_FAVORITE2, S_MENU_OPTION4 },//favor2 menu option radio playing+paly
    { OPTION_FAVOR2_RPAUSE, S_NOW_FAVORITE2, S_MENU_OPTION5 },//favor2 menu option radio lastplay+play

    { OPTION_FAVOR3_NORMAL, S_NOW_FAVORITE3, S_MENU_OPTION1 },//favor3 menu option play
    { OPTION_FAVOR3_MPLAYING, S_NOW_FAVORITE3, S_MENU_OPTION2 },//favor3 menu option music playing+paly
    { OPTION_FAVOR3_MPAUSE, S_NOW_FAVORITE3, S_MENU_OPTION3 },//favor3 menu option music lastplay+play
    { OPTION_FAVOR3_RPLAYING, S_NOW_FAVORITE3, S_MENU_OPTION4 },//favor3 menu option radio playing+paly
    { OPTION_FAVOR3_RPAUSE, S_NOW_FAVORITE3, S_MENU_OPTION5 },//favor3 menu option radio lastplay+play

    { OPTION_PLIST_NORMAL, S_PLAY, S_LIST_OPTION1 },//music list option +play+add favor
    { OPTION_PLIST_MPLAING, S_NOW_PLAYING, S_LIST_OPTION2 },//music list option music playing+play+add favor
    { OPTION_PLIST_MPAUSE, S_LAST_PLAY, S_LIST_OPTION3 },//music list option music lastplay+play+add favor
    { OPTION_PLIST_RPLAYING, S_MUSIC, S_LIST_OPTION4 },//music list option radio playing+play+add favor
    { OPTION_PLIST_RPAUSE, S_RADIO, S_LIST_OPTION5 },//music list option radiolastplay+play+add favor

    { OPTION_FAVORLIST1_NORMAL, S_NOW_FAVORITE1, S_LIST_OPTION1 },//favor1 list option play+del favor+clr favor
    //favor1 listoption music playing+play+del favor+clr favor
    { OPTION_FAVORLIST1_MPLAYING, S_NOW_FAVORITE1, S_LIST_OPTION2 },
    //favor1 list option music lastplay+play+del favor+clr favor
    { OPTION_FAVORLIST1_MPAUSE, S_NOW_FAVORITE1, S_LIST_OPTION3 },
    //favor1 list option radio playing+play+del favor+clr favor
    { OPTION_FAVORLIST1_RPLAYING, S_NOW_FAVORITE1, S_LIST_OPTION4 },
    //favor1 list option radio lastplay+play+del favor+clr favor
    { OPTION_FAVORLIST1_RPAUSE, S_NOW_FAVORITE1, S_LIST_OPTION5 },

    { OPTION_FAVORLIST2_NORMAL, S_NOW_FAVORITE2, S_LIST_OPTION1 },//favor2 list option play+del favor+clr favor
    //favor2 listoption music playing+play+del favor+clr favor
    { OPTION_FAVORLIST2_MPLAYING, S_NOW_FAVORITE2, S_LIST_OPTION2 },
    //favor2 list option music lastplay+play+del favor+clr favor
    { OPTION_FAVORLIST2_MPAUSE, S_NOW_FAVORITE2, S_LIST_OPTION3 },
    //favor2 list option radio playing+play+del favor+clr favor
    { OPTION_FAVORLIST2_RPLAYING, S_NOW_FAVORITE2, S_LIST_OPTION4 },
    //favor2 list option radio lastplay+play+del favor+clr favor
    { OPTION_FAVORLIST2_RPAUSE, S_NOW_FAVORITE2, S_LIST_OPTION5 },

    { OPTION_FAVORLIST3_NORMAL, S_NOW_FAVORITE3, S_LIST_OPTION1 },//favor3 list option play+del favor+clr favor
    //favor3 listoption music playing+play+del favor+clr favor
    { OPTION_FAVORLIST3_MPLAYING, S_NOW_FAVORITE3, S_LIST_OPTION2 },
    //favor3 list option music lastplay+play+del favor+clr favor
    { OPTION_FAVORLIST3_MPAUSE, S_NOW_FAVORITE3, S_LIST_OPTION3 },
    //favor3 list option radio playing+play+del favor+clr favor
    { OPTION_FAVORLIST3_RPLAYING, S_NOW_FAVORITE3, S_LIST_OPTION4 },
    //favor3 list option radio lastplay+play+del favor+clr favor
    { OPTION_FAVORLIST3_RPAUSE, S_NOW_FAVORITE3, S_LIST_OPTION5 },

    //{LISTMENU_AUDIBLE_NORMAL, AUDIBLE_BOOKS,0},//audible menu 书籍 + 菜单
    { LISTMENU_AUDIBLE_RESUME, S_PLAY_RESUME, 0 },//audible menu 续播菜单+books+author

    { MUSICSET_MENU, S_SET_MENU, 0 }, //music set menu入口菜单

    { AUDIBLESET_MENU, S_BOOKMARK, 0 }, //audible set menu入口菜单

    { SLEEP_TIMER_SETMENU, S_SLEEP_TIMER, 0 }, //已设置定时器入口菜单

    { SHUFFLE_PLAY, S_SHUFFLE_ALL, 0 }, //title列表中菜单项入口菜单

    { ALLSONG_PLAY, S_ALLSONG, S_LIST_OPTION1 }, //artist & genre列表中菜单项入口菜单

    { VOICE_MENU_PLAYING, S_NOW_PLAYING, 0},

    { VOICE_MENU_LASTPLAY, S_LAST_PLAY, 0},
    
    { M3ULIST_MENU_FAVOR, S_NOW_FAVORITE, 0}, //m3u list嵌套favorlist入口菜单
    { OPTION_M3ULIST_NORMAL, S_M3ULIST_OPTION1, 0}, 
    { OPTION_M3ULIST_MPLAYING, S_M3ULIST_OPTION2, 0},
    { OPTION_M3ULIST_MPAUSE, S_M3ULIST_OPTION3, 0},
    { OPTION_M3ULIST_RPLAYING, S_M3ULIST_OPTION4, 0},
    { OPTION_M3ULIST_RPAUSE, S_M3ULIST_OPTION5, 0},
};

/*定义叶子菜单项*/
const conf_item_head_t item_head =
{ "MENU ITEM   ", ITEM_TOTAL };

//叶子菜单配置
const conf_menu_item_t leafitems[] =
{
    //strid,sub_id, 确认函数, 即时函数 ,option 函数
    //所有歌曲
    { S_ALLSONG, 0, list_scene_allsong_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 0, 0},
    { S_ARTIST, 0, list_scene_artist_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 1, 0}, //演唱者
    { S_ALBUM, 0, list_scene_album_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 2, 0}, //专辑
    { S_GENRE, 0, list_scene_genre_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 3, 0}, //风格
    //播放列表(根菜单)
    { 
        S_NOW_FAVORITE, 0, list_scene_favor_sure, NULL, list_scene_menu_option, 
        NORMAL_MENU_ITEM , 4, 0
    }, 
    //播放列表1
    {
        S_NOW_FAVORITE1, S_NOW_FAVORITE, list_scene_favor1_sure, NULL, list_scene_favor1_option,
        NORMAL_MENU_ITEM, 5, 0
    },
    //播放列表2
    {
        S_NOW_FAVORITE2, S_NOW_FAVORITE, list_scene_favor2_sure, NULL, list_scene_favor2_option,
        NORMAL_MENU_ITEM, 6, 0
    },
    //播放列表3
    {
        S_NOW_FAVORITE3, S_NOW_FAVORITE, list_scene_favor3_sure, NULL, list_scene_favor3_option,
        NORMAL_MENU_ITEM, 7, 0
    },

    //语音书籍(根菜单)
    { S_AUDIBLE_BOOKS, 0, list_scene_audible_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 8, 0},
    //audible续播
    { S_PLAY_RESUME, 0, list_scene_audresume_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 9, 0},
    //audible书籍
    { S_BOOKS, 0, list_scene_books_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 10, 0},
    //audible作者
    { S_AUTHOR, 0, list_scene_authors_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 11, 0},

    //创建播放列表
    { S_CREATE_PLAYLIST, 0, list_scene_create_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 12, 0},
    //主盘目录
    { S_LOCAL_FOLDER, 0, list_scene_maindisk_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 13, 0},
    //插卡目录
    { S_CARD_FOLDER, 0, list_scene_carddisk_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 14, 0},

    //music正在播放
    { S_NOW_PLAYING, S_MUSIC, list_scene_musicplay_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 15, 0},
    //music上一次播放
    { S_LAST_PLAY, S_MUSIC, list_scene_musicpause_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 16, 0},
    //radio正在播放
    { S_NOW_PLAYING, S_RADIO, list_scene_radioplay_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 17, 0},
    //radio上一次播放
    { S_LAST_PLAY, S_RADIO, list_scene_radiomute_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 18, 0},

    //播放(收藏夹1)
    { S_PLAY, S_NOW_FAVORITE1, list_scene_favor1play_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 19, 0},
    //播放(收藏夹2)
    { S_PLAY, S_NOW_FAVORITE2, list_scene_favor2play_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 20, 0},
    //播放(收藏夹3)
    { S_PLAY, S_NOW_FAVORITE3, list_scene_favor3play_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 21, 0},
    //播放(播放列表)
    { S_PLAY, S_MUSIC, list_scene_listplay_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 22, 0},

    //加入播放列表(根菜单)
    {
        S_ADD_FAVORITE, S_ADD_TO_WHICH, list_scene_addfavor_sure, NULL, list_scene_option_option,
        NORMAL_MENU_ITEM, 23, 0
    },

    //添加(收藏夹1)
    {
        S_NOW_FAVORITE1, S_LIST_OPTION1, list_scene_addfavor1_sure, NULL, list_scene_option_option,
        NORMAL_MENU_ITEM, 24, 0
    },
    //添加(收藏夹2)
    {
        S_NOW_FAVORITE2, S_LIST_OPTION1, list_scene_addfavor2_sure, NULL, list_scene_option_option,
        NORMAL_MENU_ITEM, 25, 0
    },
    //添加(收藏夹3)
    {
        S_NOW_FAVORITE3, S_LIST_OPTION1, list_scene_addfavor3_sure, NULL, list_scene_option_option,
        NORMAL_MENU_ITEM, 26, 0
    },

    //从收藏夹中删除
    { S_DEL_FROM_FAVOR, 0, list_scene_delfavor_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 27, 0},
    //清除
    { S_CLEAR_FAVORITE, 0, list_scene_clrfavor_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 28, 0},

    //music设置菜单项

    { S_PLAY_MODE, 0, NULL, NULL, set_scene_leveltop_option, NORMAL_MENU_ITEM , 29, 0}, //播放模式菜单(是根菜单)
    { S_SOUND_SET, 0, NULL, NULL, set_scene_leveltop_option, NORMAL_MENU_ITEM , 30, 0}, //声音设置菜单(是根菜单)
    //添加收藏夹菜单(是根菜单)
    { S_ADD_FAVORITE, S_SET_MENU, set_scene_addfavor_sure, NULL, set_scene_leveltop_option, NORMAL_MENU_ITEM , 31, 0},
    //删除收藏夹菜单(是根菜单)
    { S_DEL_FROM_FAVOR, S_SET_MENU, NULL, NULL, set_scene_leveltop_option, NORMAL_MENU_ITEM , 32, 0},
    //定时关机菜单(是根菜单)
    { S_SLEEP_TIMER, 0, set_scene_sleeptimer_sure, NULL, set_scene_leveltop_option, NORMAL_MENU_ITEM , 33, 0},
     //删除菜单
    { S_DELETE, 0, set_scene_delete_sure, NULL, set_scene_leveltop_option, NORMAL_MENU_ITEM , 34, 0},

    //音乐来源于...(是根菜单)
    { S_MUSIC_FROM, 0, NULL, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 35, 0},
    //...所有歌曲
    { S_THIS_TITLE, 0, set_scene_fromallsong_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM, 36, 0 },
    //...此演唱者
    { S_THIS_ARTIST, 0, set_scene_fromartist_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 37, 0},
    //...此专辑
    { S_THIS_ALBUM, 0, set_scene_fromalbum_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 38, 0},
    //...此风格
    { S_THIS_GENRE, 0, set_scene_fromgenre_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 39, 0},

    //重复播放(是根菜单)
    { S_REPEAT_MODE, 0, set_scene_repeat_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 40, 0},
    //重复播放关
    { S_REPEAT_OFF, 0, set_scene_repeatoff_sure, NULL, set_scene_levelother_option, RAIDO_MENU_ITEM , 41, 0},
    //单曲重复
    { S_REPEAT_ONE, 0, set_scene_repeatone_sure, NULL, set_scene_levelother_option, RAIDO_MENU_ITEM , 42, 0},
    //全部重复
    { S_REPEAT_ALL, 0, set_scene_repeatall_sure, NULL, set_scene_levelother_option, RAIDO_MENU_ITEM , 43, 0},
    //浏览播放
    { S_REPEAT_INTRO, 0, set_scene_repeatintro_sure, NULL, set_scene_levelother_option, RAIDO_MENU_ITEM , 44, 0},

    //随机播放(是根菜单)
    { S_SHUFFLE, 0, set_scene_shuffle_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 45, 0},
    //随机播放关
    { S_OFF, S_SHUFFLE, set_scene_shuffleoff_sure, NULL, set_scene_levelother_option, RAIDO_MENU_ITEM , 46, 0},
     //随机播放开
    { S_ON, S_SHUFFLE, set_scene_shuffleon_sure, NULL, set_scene_levelother_option, RAIDO_MENU_ITEM , 47, 0},

    //复读设置(是根菜单)
    { S_AB_SET, 0, NULL, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 48, 0},
    //复读模式
    { S_AB_MODE, 0, set_scene_abmode_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 49, 0},
    //复读次数
    { S_AB_COUNT, 0, set_scene_setabcount_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 50, 0},
    //复读间隔
    { S_AB_GAP, 0, set_scene_setabgap_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 51, 0},

    //fullsound设置菜单(是根菜单)
    { S_FULLSOUND, 0, set_scene_fullsound_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 52, 0},
    //fullsound关
    {
        S_OFF, S_FULLSOUND, set_scene_fullsoundoff_sure, set_scene_eqnormal_callback, set_scene_levelother_option,
        RAIDO_MENU_ITEM, 53, 0
    },
    //fullsound开
    {
        S_ON, S_FULLSOUND, set_scene_fullsoundon_sure, set_scene_fullsoundon_callback, set_scene_levelother_option,
        RAIDO_MENU_ITEM, 54, 0
    },

    //均衡器(是根菜单)
    { 
        S_EQUALIZE, 0, set_scene_eq_sure, NULL, set_scene_levelother_option, 
        NORMAL_MENU_ITEM , 55, 0
    },
    //normal
    { 
        S_OFF, S_EQUALIZE, set_scene_eqnormal_sure, set_scene_eqnormal_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 56, 0
    },
    //rock
    { 
        S_ROCK, 0, set_scene_eqrock_sure, set_scene_eqrock_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 57, 0
    },
    //funk
    { 
        S_FUNK, 0, set_scene_eqfunk_sure, set_scene_eqfunk_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 58, 0
    }, 
    //hiphop
    {
        S_HIPHOP, 0, set_scene_eqhiphop_sure, set_scene_eqhiphop_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 59, 0
    },
    //jazz
    { 
        S_JAZZ, 0, set_scene_eqjazz_sure, set_scene_eqjazz_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 60, 0
    }, 
    //classical
    { 
        S_CLASSIC, 0, set_scene_eqclassical_sure, set_scene_eqclassic_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 61, 0
    },
    //techno
    { 
        S_TECHNO, 0, set_scene_eqtechno_sure, set_scene_eqtechno_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 62, 0
    }, 
    //custom
    { 
        S_CUSTOM, 0, set_scene_eqcustom_sure, set_scene_equser_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 63, 0
    },
    //播放速度speed
    { S_PLAY_SPEED, 0, set_scene_speed_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM, 64, 0 },

    //srs 设置是根菜单
    { 
        S_SRS_SET, 0, set_scene_srs_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 65, 0
    },
    //srs关
    { 
        S_OFF, S_SRS_SET, set_scene_srsoff_sure, set_scene_eqnormal_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 66, 0
    },
    //srs wowhd
    { 
        S_SRS_WOWHD, 0, set_scene_srs_wowhd_sure, set_scene_srswowhd_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 67, 0
    },
    //srs user
    { 
        S_SRS_USER, 0, set_scene_srs_user_sure, set_scene_srsuser_callback, set_scene_levelother_option, 
        RAIDO_MENU_ITEM , 68, 0
    },

    //音量限制(均衡器平级)
    { S_VOLUME_LIMIT, 0, set_scene_vollimit_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM, 69, 0 },

    //添加到收藏夹1
    {
        S_NOW_FAVORITE1, S_SET_MENU, set_scene_addfavor1_sure, NULL, set_scene_levelother_option,
        NORMAL_MENU_ITEM, 70, 0
    },
    //添加到收藏夹2
    {
        S_NOW_FAVORITE2, S_SET_MENU, set_scene_addfavor2_sure, NULL, set_scene_levelother_option,
        NORMAL_MENU_ITEM, 71, 0
    },
    //添加到收藏夹3
    {
        S_NOW_FAVORITE3, S_SET_MENU, set_scene_addfavor3_sure, NULL, set_scene_levelother_option,
        NORMAL_MENU_ITEM, 72, 0
    },
    //从收藏夹1删除
    {
        S_NOW_FAVORITE1, S_DELETE, set_scene_delfavor1_sure, NULL, set_scene_levelother_option,
        NORMAL_MENU_ITEM, 73, 0
    },
    //从收藏夹2删除
    {
        S_NOW_FAVORITE2, S_DELETE, set_scene_delfavor2_sure, NULL, set_scene_levelother_option,
        NORMAL_MENU_ITEM, 74, 0
    },
    //从收藏夹3删除
    {
        S_NOW_FAVORITE3, S_DELETE, set_scene_delfavor3_sure, NULL, set_scene_levelother_option,
        NORMAL_MENU_ITEM, 75, 0
    },

    //定时关机--关
    {
        S_OFF, S_SLEEP_TIMER, set_scene_closesleep_sure, NULL, set_scene_levelother_option,
        RAIDO_MENU_ITEM, 76, 0
    },

    //设置定时关机时间
    {
        S_SET_SLEEP_TIMER, S_SLEEP_TIMER, set_scene_setsleep_sure, NULL, set_scene_levelother_option,
        RAIDO_MENU_ITEM, 77, 0
    },

    //audible set menu
    { S_BOOKMARK, 0, NULL, NULL, set_scene_leveltop_option, NORMAL_MENU_ITEM , 78, 0}, //书签菜单(是根菜单)
    //添加书签菜单
    { S_ADD_BOOKMARK, 0, set_scene_bookmark_add_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM, 79, 0 },
    //选择书签菜单
    { S_SEL_BOOKMARK, 0, set_scene_bookmark_sel_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM, 80, 0 },
    //删除书签菜单
    { S_DEL_BOOKMARK, 0, set_scene_bookmark_del_sure, NULL, set_scene_levelother_option, NORMAL_MENU_ITEM , 81, 0},

    //删除audible文件菜单
    { S_DEL_AUDIBLE, 0, set_scene_delaudible_sure, NULL, set_scene_leveltop_option, NORMAL_MENU_ITEM , 82, 0},

    //导航模式(是根菜单)
    { S_SWITCH_MODE, 0, set_scene_switchmode_sure, NULL, set_scene_leveltop_option, NORMAL_MENU_ITEM , 83, 0},
    //章节模式
    { S_SWITCH_SECTION, 0, set_scene_setsection_sure, NULL, set_scene_leveltop_option, RAIDO_MENU_ITEM , 84, 0},
    //标题模式
    { S_SWITCH_SONG, 0, set_scene_settitle_sure, NULL, set_scene_leveltop_option, RAIDO_MENU_ITEM , 85, 0},

    //列表中全部随机播放菜单项
    { S_SHUFFLE_ALL, 0, list_scene_shuffleplay_sure, NULL, list_scene_option_option, NORMAL_MENU_ITEM , 86, 0},

     //列表中所有歌曲菜单项
    {
        S_ALLSONG, S_LIST_OPTION1, list_scene_allsongplay_sure, NULL, list_scene_option_option,
        NORMAL_MENU_ITEM, 87, 0
    },
    { S_ALBUM_ART, 0, list_scene_albumlist_sure, NULL, list_scene_menu_option, NORMAL_MENU_ITEM , 88, 0},
};

#ifdef PC
menu_cb_func get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((leafitems[i].major_id == str_id) && (leafitems[i].minor_id == str_id_sub))
        {
            return leafitems[i].menu_func;
        }
    }

    return NULL;
}

menu_cb_leaf get_callback_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((leafitems[i].major_id == str_id) && (leafitems[i].minor_id == str_id_sub))
        {
            return leafitems[i].callback;
        }
    }

    return NULL;
}

menu_cb_option get_menu_option(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;

    item_cnt = item_head.total;

    for(i = 0; i < item_cnt; i++)
    {
        if((leafitems[i].major_id == str_id) && (leafitems[i].minor_id == str_id_sub))
        {
            return leafitems[i].menu_option;
        }
    }

    return NULL;
}
#endif
