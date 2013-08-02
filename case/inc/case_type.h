/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-27 14:06     1.0             build this file 
*******************************************************************************/
/*!
 * \file     case_type.h
 * \brief    case 基本数据结构，include case 的直接上层模块头文件
 * \author   lzcai
 * \version  1.0
 * \date     2011-11-27
*******************************************************************************/
#include "psp_includes.h"
#include "config_id.h"
#include "common_sty.h"
#include "common_res.h"
#include "lang_id.h"
#include "lcd_driver.h"
#include "display.h"
#include "key_interface.h"
#include "vm_def.h"

#ifndef _case_type_H_
#define _case_type_H_

/*!
 * \brief  
 *  方案应用ID，最多支持32个应用，ID对应 app_name_ram 中的名字。
 * \note
 * \li  前6个应用顺序不能够调整，对应于方案所支持的后缀列表。
 */
#define APP_ID_MUSIC        0x00
#define APP_ID_AUDIBLE      0x01
#define APP_ID_VIDEO        0x02
#define APP_ID_PICTURE      0x03
#define APP_ID_EBOOK        0x04
#define APP_ID_BROWSER      0x05
#define APP_ID_VOICE        0x06
#define APP_ID_RECORD       0x07
#define APP_ID_RADIO        0x08
#define APP_ID_TOOLS        0x09
#define APP_ID_SETTING      0x0a
#define APP_ID_USER1        0x0b
#define APP_ID_USER2        0x0c
#define APP_ID_USER3        0x0d
#define APP_ID_MANAGER      0x0e
#define APP_ID_MAINMENU     0x0f
#define APP_ID_PLAYLIST     0x10
#define APP_ID_CONFIG       0x11
#define APP_ID_UDISK        0x12
#define APP_ID_MENGINE      0x13
#define APP_ID_FMENGINE     0x14
#define APP_ID_ALARM        0x15
#define APP_ID_MTP          0x16
#define APP_ID_MTPSYNC      0x17
#define APP_ID_UPGRADE      0x18
#define APP_ID_MAX          0x19
//附上 app_name_ram 列表，放在 manager_get_name.c 中。
//const char app_name_ram[23][12] = 
//{
//  "music.ap", "audible.ap", "video.ap", "picture.ap", "ebook.ap", "browser.ap", "voice.ap", "record.ap",
//  "radio.ap", "tools.ap", "setting.ap", "user1.ap", "user2.ap", "user3.ap", "manager.ap", "mainmenu.ap",
//  "playlist.ap", "config.ap", "udisk.ap", "mengine.ap", "fmengine.ap", "alarm.ap", "mtp.ap", "mtpsync.ap",
//  "fwupdate.ap"
//};

/*!
 * \brief  
 *  应用后缀名 bitmap，bit<31> = 0h。
 * \note
 * \li  与后缀名数组对应关系：bit[30] 对应后缀名数组 rom_ext_name 首元素，即 MP3。
 */
#define MUSIC_BITMAP        0x7e010e80
//#define MUSIC_BITMAP        0x76000000
#define AUDIBLE_BITMAP      0x00003000
#define VIDEO_BITMAP        0x01004000
#define PICTURE_BITMAP      0x00780000
#define PICTURE_BITMAP1     0x00600000//后台音乐播放时的图片文件名过滤
#define TEXT_BITMAP         0x00800000
#define RECORD_BITMAP       0x50000080//MP3,WAV,ACT
#define ALLALL_BITMAP       0x00000004
#define ALLFILE_BITMAP      0x00000002
#define ALLDIR_BITMAP       0x00000001
//附上 rom_ext_name 列表，放在 fs_common_func.c 中。
//const char rom_ext_name[31][4] = 
//{      
//    "MP3",//对应 bit[30]
//    "WMA",
//    "WAV",   
//    "AAC",
//    "FLAC",
//    "APE", 
//    "AVI",//对应 bit[24]
//    "TXT", 
//    "JPG", 
//    "JPEG", 
//    "BMP", 
//    "GIF", 
//    "LRC",
//    "ASF",   
//    "OGG",//对应 bit[16]
//    "WMV", 
//    "AMV", 
//    "AAX",
//    "AA",  
//    "MP1", 
//    "MP2",
//    "M4A",
//    "POS",//对应 bit[8]
//    "ACT",
//    "** ",
//    "** ",
//    "** ",
//    "** ",
//    "**",
//    "*f",
//    "*d",//对应 bit[0]
//};

/*! 无符号32位整型数 -1 值宏定义 */
#define V_U32_INVALID   0xffffffff
/*! 无符号16位整型数 -1 值宏定义 */
#define V_U16_INVALID   0xffff
/*! 无符号8 位整型数 -1 值宏定义 */
#define V_U8_INVALID    0xff

/*! 文件系统扇区大小 */
#define FILE_SECTOR     512



/*!
 * \brief
 *  创建playlist播放列表传入参数，
    使用bit7~bit5表示盘符，定义如下
    而bit4~bit0依然传app_param_e
 */

#define PLIST_DISK_C      0x80
#define PLIST_DISK_H      0x40 
#define PLIST_DISK_U      0x20
#define PLIST_DISK_ALL    0xc0//0xe0,暂不支持Uhost

//#define FROM_ALARM    0x60

/*!
 * \brief
 *  app_param_e 进入应用传递参数类型枚举类型
 */
typedef enum
{
    /*! 空参数 */
    PARAM_NULL = 0x0,
    /*! 从 mainmenu ap 进入 */
    PARAM_FROM_MAINMENU,
    /*! 从 music ap 进入 */
    PARAM_FROM_MUSIC,
    /*! 从 audible ap 进入 */
    PARAM_FROM_AUDIBLE,
    /*! 从 video ap 进入 */
    PARAM_FROM_VIDEO,
    /*! 从 picture ap 进入 */
    PARAM_FROM_PICTURE,
    /*! 从 ebook ap 进入 */
    PARAM_FROM_EBOOK,
    /*! 从 browser ap 进入 */   
    PARAM_FROM_BROWSER,
    /*! 从 voice ap 进入 */ 
    PARAM_FROM_VOICE,
    /*! 从 record ap 进入 */    
    PARAM_FROM_RECORD,
    /*! 从 radio ap 进入 */ 
    PARAM_FROM_RADIO,
    /*! 从 tools ap 进入 */ 
    PARAM_FROM_TOOLS,
    /*! 从 setting ap 进入 */   
    PARAM_FROM_SETTING,
    /*! 从 playlist ap 进入 */  
    PARAM_FROM_PLAYLIST,
    /*! 从 config ap 进入 */    
    PARAM_FROM_CONFIG,
    /*! 从 udisk ap 进入 */ 
    PARAM_FROM_UDISK,   
    /*! 从正在播放菜单进入 */
    PARAM_BACKTO_PLAYING,
    /*! 从上次播放菜单进入*/
    PARAM_BACKTO_PLAYED,
    /*! 从alarm ap进入*/
    PARAM_FROM_ALARM,
} app_param_e;

/*!
 * \brief
 *  app_result_e 应用层接口返回结果枚举类型
 */
typedef enum
{
    /*! 没有任何需要通知调用者的情况返回，调用者不需要做任何特殊处理 */
    RESULT_NULL = 0x00,
    /*! 从0x8000开始，预留出 0x0000 ~ 0x7fff 给 msg_apps_type_e，以把
     *  msg_apps_type_e 当作 app_result_e 的子集
     */
    _RESERVE_FOR_APP_MSG_TYPE_ = 0x8000,
    
    /*! 音乐文件播放消息 0x8001*/
    RESULT_MUSIC_PLAY,
    /*! 语音书籍播放消息*/
    RESULT_AUDIBLE_PLAY,
    /*! 视频文件播放消息*/
    RESULT_MOVIE_PLAY,
    /*! 图片文件播放消息*/
    RESULT_PICTURE_PLAY,
    /*! 文本文件播放消息*/
    RESULT_EBOOK_PLAY,
    /*! 回到 MainMenu 应用 0x8006*/
    RESULT_MAIN_APP,
    /*!音乐进入消息  0x8007*/
    RESULT_MUSIC,
    /*!视频进入消息  0x8008*/
    RESULT_VIDEO,
    /*!录音进入消息  0x8009*/
    RESULT_RECORD,
    /*!图片进入消息  0x800a*/
    RESULT_PICTURE,
    /*!RADIO进入消息  0x800b*/
    RESULT_RADIO,
    /*!浏览器进入消息 0x800c*/
    RESULT_BROWSER,
    /*!设置进入消息  0x800d*/
    RESULT_SETTING,
    /*!工具进入消息  0x800e*/
    RESULT_TOOLS,
    /*!上一次播放进入消息  0x800f*/
    RESULT_LASTPLAY,
    /*!正在播放进入消息  0x8010*/
    RESULT_NOWPLAYING,
    /*! 确认某个事件或操作返回  0x8011*/
    RESULT_CONFIRM,
    /*! 取消某个事件或操作返回  0x8012*/
    RESULT_CANCEL,
    /*! 忽略返回，类似于RESULT_NULL，调用者不需要做任何特殊处理 0x8013*/
    RESULT_IGNORE,
    /*! 重绘UI返回，通常返回后需要重新绘制当前UI，但不需要退出当前消息循环 0x8014*/
    RESULT_REDRAW,
    /*! 应用退出消息 0x8015*/
    RESULT_APP_QUIT,
    /*! USB选项选择数据传输模式 */
    RESULT_USB_TRANS,
    /*! USB选项选择充电播放模式 */
    RESULT_CHARGE_PLAY,
    /*! USB菜单选择中拔出USB电缆 */
    RESULT_USB_UNSTICK,
    /*! 删除当个文件完成后返回 */
    RESULT_DELETE_FILE,
    /*! 删除文件夹或列表完成后返回 */
    RESULT_DELETE_DIRLIST,
    /*! 删除文件夹或列表（不删除自身）完成后返回 */
    RESULT_DELETE_DIRLIST_NOSELF,
    /*! 其他错误返回 0x801c*/
    RESULT_ERROR,
    
    /*! 正在播放 */
    RESULT_OPTION_PLAYING,
    /*! 上次播放 */
    RESULT_OPTION_LASTPLAY, 
    /*! 播放，自动选择列表中的一首歌播放 */
    RESULT_OPTION_PLAY,
    /*! 添加到收藏夹 */
    RESULT_OPTION_ADD_FAVOURITE,
    /*! 删除，根据激活项删除文件或目录 */
    RESULT_OPTION_DELETE,
    /*! 说明信息 */
    RESULT_OPTION_NULL,
    /*! 进入自动升级 */
    RESULT_AUTO_UPGRADE,
    

    /*! 文件浏览器返回结果，占位符 */
    RESULT_DIR_BASE = 0x8100,
    /*! 错误：进入目录错误，包括 root */
    RESULT_DIR_ERROR_ENTER_DIR,
    /*! 错误：根目录下没有文件和文件夹 */
    RESULT_DIR_ERROR_NO_FILE,
    /*! 错误：顶层不是根目录情况下，set location 失败 */
    RESULT_DIR_ERROR_SETLOC,
    
    /*! 菜单列表返回结果，占位符 */
    RESULT_MENU_NULL = 0x8200,
    /*! 返回上一级菜单 */
    RESULT_MENU_PARENT,
    /*! 进入下一级菜单，其实没什么实际意义 */
    RESULT_MENU_SON,
    /*! 重新进入当前级，会执行菜单头回调函数 menu_func */
    RESULT_MENU_CUR,
    /*! 重画UI后返回上一级菜单 */
    RESULT_MENU_REDRAW_PARENT,
    /*! 退出菜单管理器 */
    RESULT_MENU_EXIT,
    /*! 重新加载菜单，用于嵌套调用 menulist 返回 */
    RESULT_MENU_REDRAW,
    
    /*! 对话框返回结果，占位符 */
    RESULT_DIALOG_BASE = 0x8300,
    /*! 选择OK按钮 */
    RESULT_DIALOG_OK,   
    /*! 选择CANCEL按钮 */   
    RESULT_DIALOG_CANCEL,
    /*! 选择ABORT按钮 */
    RESULT_DIALOG_ABORT,
    /*! 选择RETRY按钮 */
    RESULT_DIALOG_RETRY,
    /*! 选择IGNORE按钮 */
    RESULT_DIALOG_IGNORE,
    /*! 选择YES按钮 */
    RESULT_DIALOG_YES,
    /*! 选择NO按钮 */
    RESULT_DIALOG_NO,
    
    /*! 参数设置框返回结果，占位符 */
    RESULT_PARAM_BASE = 0x8400,
    
    /*! 主菜单应用返回结果，占位符 */
    RESULT_MAINMENU_BASE = 0x8500,
    /*! 切换到下一个应用 */
    RESULT_MAIN_NEXT_ITEM,
    /*! 切换到上一个应用 */
    RESULT_MAIN_PREV_ITEM,
    /*! 进入选项菜单 */
    RESULT_OPTION_MAINMENU,   
    /*! 进入主菜单 */
    RESULT_ROOTMENU,
    
    /*! 音乐应用返回结果，占位符 */
    RESULT_MUSIC_BASE = 0x8600,
    
    /*! 正常开始播放 */
    RESULT_PLAYING_START,
    /*! 全部随机播放 */
    RESULT_PLAYING_SHUFFLE,
  
    /*! 进入播放场景保持原有状态 */
    RESULT_PLAYING_KEEP,
    /*! 回到fm播放 */
    RESULT_RADIO_PLAY,
    
    /*! 添加收藏夹1 */
    RESULT_ADD_FAVOR1,
    /*! 添加收藏夹2 */
    RESULT_ADD_FAVOR2,
    /*! 添加收藏夹3 */
    RESULT_ADD_FAVOR3,
    
    /*! 删除收藏夹 */
    RESULT_DEL_FAVOR,
    /*! 清除收藏夹 */
    RESULT_CLR_FAVOR,
    
    /*! 进入allsong列表 */
    RESULT_ENTER_TITLE_LIST,
    /*! 进入专辑列表 */
    RESULT_ENTER_ALBUM_LIST,
    /*! 进入歌手列表 */
    RESULT_ENTER_ARTIST_LIST,    
    /*! 进入风格列表 */
    RESULT_ENTER_GENRE_LIST,
    /*! 进入专辑图片列表 */    
    RESULT_ENTER_ALBUM_THUMBNAIL,

    /*! 进入收藏夹列表 */
    RESULT_ENTER_FAVOR_LIST,
    /*! 进入收藏夹1列表 */
    RESULT_ENTER_FAVOR1_LIST,
    /*! 进入收藏夹2列表 */
    RESULT_ENTER_FAVOR2_LIST,
    /*! 进入收藏夹3列表 */
    RESULT_ENTER_FAVOR3_LIST,
    
    /*! audible续播 */
    RESULT_AUDIBLE_RESUME,
    /*! 进入audible书籍列表 */
    RESULT_ENTER_BOOKS_LIST,
    /*! 进入audible作者列表 */
    RESULT_ENTER_AUTHORS_LIST,
    
    /*! 列表场景菜单 */
    RESULT_LISTMENU_MENU,
    /*! 列表场景列表 */
    RESULT_LISTMENU_LIST,
    /*! audible设置菜单 */
    RESULT_SETMENU_AUDIBLE,
    /*! music设置菜单 */
    RESULT_SETMENU_MUSIC,
    /*! 播放场景 */
    RESULT_PLAYING_SCENE,
    /*! 场景调度退出 */
    RESULT_SCENE_EXIT,
    /*! 进入创建播放列表ap */
    RESULT_CREATE_LIST_APP,
    
    /*! 进入新的入口菜单 */
    RESULT_ENTER_NEWENTRY,
    
    /*! card拔出出错 */
    RESULT_CARD_OUT_ERROR,

    /*! 进入跟读对比状态 */
    RESULT_MUSIC_RECORDING,
    /*! 跟读或对比读播放结束 */
    RESULT_PLAY_END,
    
    /*! 视屏应用返回结果，占位符 */
    RESULT_VIDEO_BASE = 0x8700,
    /*!video的播放界面*/
    RESULT_VIDEO_PLAY,
    /*!video的文件列表界面*/
    RESULT_VIDEO_FILELIST,
    /*!video的option菜单界面*/
    RESULT_VIDEO_OPTIONMENU,
    /*!video的菜单界面*/
    RESULT_VIDEO_MENU,    
    /*!video的返回*/
    RESULT_VIDEO_RETURN,
#if 0
    /*!video的全屏界面*/
    RESULT_VIDEO_FULL_SCREEN,
    /*!video进度条界面*/
    RESULT_VIDEO_PLAYBAR,
    /*!video音量条界面*/
    RESULT_VIDEO_VOLUMEBAR,
#endif

    /*! 录音应用返回结果，占位符 */
    RESULT_RECORD_BASE = 0x8800,
    /*!录音应用的录音界面*/
    RESULT_RECORD_REC,
    /*!录音应用的设置菜单*/
    RESULT_RECORD_MEMU,
    RESULT_RECORD_OPTION_MEMU,

    
    /*! RADIO应用返回结果，占位符 */
    RESULT_RADIO_BASE = 0x8900,
       /* 进入预设电台列表场景*/
    RESULT_RADIO_STATION_LIST,
    /* 进入用户电台列表场景*/
    RESULT_RADIO_USER_LIST,
    /* 进行自动调谐后再进入播放场景*/
       //RESULT_RADIO_AUTO_PLAY,
    /* 进入电台列表的option 菜单*/
    RESULT_RADIO_OPTION_STATION,
    /* 进入播放场景下的option 菜单*/
    RESULT_RADIO_OPTION_PLAYWIN,
    /* 开始FM  播放，直接进入播放场景*/
    RESULT_RADIO_START_PLAY,
    /* 进入FM 的一级主菜单场景*/
    RESULT_RADIO_MAINMENU,


    /*! 电子书应用返回结果，占位符 */
    RESULT_EBOOK_BASE = 0x8a00,
    /*!ebook 的播放界面*/
    RESULT_EBK_PLAY,
    /*!ebook 的文件列表界面*/
    RESULT_EBK_FILELIST, 
    /*!ebook 的菜单界面*/
    RESULT_EBK_SETTINGMENU,
    /*!ebook 的阅读菜单界面*/
    RESULT_EBK_READINGMENU,
    /*! 设置应用返回结果，占位符 */
    RESULT_SETTING_BASE = 0x8b00,
    
    /*! 工具应用返回结果，占位符 */
    RESULT_TOOLS_BASE = 0x8c00,
    /*TOOLS 的菜单界面*/
    RESULT_TOOLS_MENULIST,
    /*TOOLS 的退出返回*/
    RESULT_TOOLS_EXIT,
    /*TOOLS 的日历界面*/
    RESULT_CALENDAR,
    /*TOOLS 的秒表界面*/
    RESULT_STOPWATCH,
    /*Reserved*/
    RESULT_ALARMMENU,
    /*退出至数据交互*/
    RESULT_DATAEXCH,
    /*退出至U盘*/
    RESULT_UDISK,
    /*定时alarm消息 0x8c08*/
    RESULT_ALARM,
    /*定时FM消息 0x8c09*/
    RESULT_ALARMFM,
    /*定时FMREC消息 0x8c0a*/
    RESULT_FMRECORD,
    /*alarm退出消息 0x8c0b*/
    RESULT_ALARM_QUIT,

    /*! 文件导航应用返回结果，占位符 */
    RESULT_BROWSER_BASE = 0x8d00,
    /*! 进入browser文件浏览器 */
    RESULT_BROWSER_FILELIST,
    /*! 进入browser菜单 */
    RESULT_BROWSER_MENU,
    /*! 从其它ap返回browser */
    RESULT_RETURN_BROWSER,
    /*! 退出browser应用 */
    RESULT_BROWSER_EXIT,
    
    /*! 音乐引擎应用返回结果，占位符 */
    RESULT_MENGINE_BASE = 0x8e00,
    
    /*! FM引擎应用返回结果，占位符 */
    RESULT_FMENGINE_BASE = 0x8f00,
    
    /*!picture 模块返回的事件，占位符*/ 
    RESULT_PIC_BASE = 0x9000,   
    /*!picture 的播放界面*/
    RESULT_PIC_PLAY,
    /*!picture 的文件列表界面*/
    RESULT_PIC_FILELIST, 
    /*!picture 的磁盘选择菜单界面*/
    RESULT_PIC_MENU,
    /*!picture 的option菜单界面*/
    RESULT_PIC_OPTIONMENU,    
    /*!picture 返回*/
    RESULT_PIC_RETURN,
    /*!picture缩略图下一曲*/
    RESULT_PIC_NEXTITEM,
    /*!picture缩略图下一屏*/
    RESULT_PIC_NEXTSCREEN,
    
    /*!voice模块返回的事件，占位符*/ 
    RESULT_VOICE_BASE = 0x9100,
    /*!返回voice play场景*/ 
    RESULT_VOICE_PLAY,
    /*!返回voice menu场景*/ 
    RESULT_VOICE_MENU,
    /*!返回voice fm menu场景*/ 
    RESULT_VOICE_FM_MENU,
    /*!返回record root menu场景*/ 
    RESULT_RECORD_ROOT_MENU,
    /*!返回fm root menu场景*/ 
    RESULT_FM_ROOT_MENU,
    /*!返回fm 预设电台列表下的menu场景*/ 
    RESULT_FM_FREQ_MENU,

    /*! playlist应用返回结果，占位符 */
    RESULT_PLAYLIST_BASE = 0x9200,
} app_result_e;


/*!
 * \brief
 *  sys_counter_t 系统功能计时器数据结构，各计时器均以0.5 s为单位
 */
typedef struct
{
    /*! 背光变暗或关闭计时器 */
    uint16 light_counter;
    /*! 屏幕保护计时器 */
    uint16 screen_saver_counter;
    /*! 返回正在播放计时器 */
    uint16 playing_counter;
    /*! 获取电池电量计时器 */
    uint16 battery_counter;
    /*! 省电关机计时器 */
    uint16 poweroff_counter;
    /*! 睡眠关机计时器 */
    uint16 sleep_counter;
} sys_counter_t;

/*!
 *  \brief
 *  g_sys_counter （全局）系统功能计时器，存放在系统全局空间
 */
#ifndef PC
extern sys_counter_t g_sys_counter;
#else
extern DLL_IMP sys_counter_t g_sys_counter;
#endif

/*!
 *  \brief
 *  config_fp （全局）应用配置脚本文件句柄，存放在系统全局空间
 */
#ifndef PC
extern sd_handle config_fp;
#else
extern DLL_IMP sd_handle config_fp;
#endif

#endif
