/*******************************************************************************
 *                              us211A
 *                            Module: Applib
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-11-27 22:06     1.0             build this file 
*******************************************************************************/
/*!
 * \file     app_msg.h
 * \brief    应用层消息处理模块相关数据结构，宏定义等
 * \author   lzcai
 * \version  1.0
 * \date     2011-11-27
*******************************************************************************/

#ifndef _app_msg_H_
#define _app_msg_H_

#include "psp_includes.h"
#include "case_type.h"

/*!
 * \brief
 *  msg_apps_type_e 应用消息枚举类型
 */
typedef enum
{
    /*! 空消息 */
    MSG_NULL = 0x0000,
    
    /****************************** shortcut事件 ****************************/
    /*! 快捷键占位符，保留空间 0x0020 ~ 0x003f */
    SHORTCUT_RESERVE_BEGIN = 0x001f,
    /*! 长按 RETURN 键返回 mainmenu 应用事件 */
    EVENT_RETURN_MAINMENU,
    /*! 按下 VOL+ 或 VOL- 进入音量调节界面事件 */
    EVENT_ENTER_VOLUMEBAR,
    /*! 拨动 LOCK 键到常关，进入按键锁处理事件 */
    EVENT_ENTER_KEYLOCK,
    /*! 拨动 LOCK 键到常开，进入按键锁处理事件 */
    EVENT_ENTER_KEYLOCK_UN,
    /*! 按键锁住状态下，按任意按键进入按键锁保持事件 */
    EVENT_ENTER_KEYLOCK_HOLD,
    /*! 长按 PLAY 键进入关机请求确认事件 */
    EVENT_ENTER_SHUTOFF_ASK,
    SHORTCUT_RESERVE_END = 0x0040,

    /******************************system message****************************/
    /*! system manager消息占位符，其中 0x0080 ~ 0x00ff 保留给系统*/
    SYSTEM_RESERVE_BEGIN = 0x007f,
    /* for qac 重定义为enum类型，消除 qac 警告 */
    MSG_RTC2HZ         = MSG_SYS_RTC2HZ,
    MSG_BAT_V_CHG      = MSG_SYS_BAT_V_CHG,
    MSG_LOW_POWER      = MSG_SYS_LOW_POWER,
    MSG_RTCALARM       = MSG_SYS_RTCALARM,
    MSG_POWER_OFF      = MSG_SYS_POWER_OFF,
    MSG_RECLAIM        = MSG_SYS_RECLAIM,
                                
    MSG_USB_STICK      = MSG_SYS_USB_STICK,
    MSG_USB_UNSTICK    = MSG_SYS_USB_UNSTICK,
                                
    MSG_ADAPTOR_IN     = MSG_SYS_ADAPTOR_IN,
    MSG_ADAPTOR_OUT    = MSG_SYS_ADAPTOR_OUT,
                                
    MSG_SD_IN          = MSG_SYS_SD_IN,
    MSG_SD_OUT         = MSG_SYS_SD_OUT,
                                
    MSG_UH_IN          = MSG_SYS_UH_IN,
    MSG_UH_OUT         = MSG_SYS_UH_OUT,
    
    /*! 耳机（天线）插入 */
    MSG_EARPHONE_IN    = 0x00f0,
    /*! 耳机（天线）拔出 */
    MSG_EARPHONE_OUT,
    /*! 充电已满消息 */
    MSG_FULL_CHARGE,
    SYSTEM_RESERVE_END = 0x0100,

    /*! 询问应用是否 ready 的同步消息 */
    MSG_APP_ACTIVE_SYNC,
    /*! 通知应用程序退出消息，应用程序接受到此消息后，进入退出流程，该消息只能由 manager.app 发送 */
    MSG_APP_QUIT,
    /*! 通知应用回到正在播放界面，由 sys_counter 在检测到45秒没按键操作后发送 */
    MSG_APP_BACKTO_PLAYING,
    /*! 进入屏幕保护 */
    MSG_APP_ENTER_SCREEN_SAVE,
    /*! 进入屏幕保护，关掉背光 */
    MSG_APP_ENTER_SCREEN_SAVE2,
    /*! 退出屏幕保护 */
    MSG_APP_EXIT_SCREEN_SAVE,

    /******************************manager message****************************/
    /*! process manager消息占位符*/
    MANAGER_BASE = 0x0200,
    /*! 异步创建应用程序消息 */
    MSG_CREAT_APP,
    /*! 同步创建应用程序消息 */
    MSG_CREAT_APP_SYNC,  
    /*! 异步杀死应用程序消息 */
    MSG_KILL_APP,
    /*! 同步杀死应用程序消息 */
    MSG_KILL_APP_SYNC, 
    /*! 进入 Udisk 数据传输 */
    MSG_USB_TRANS,
    /*! 停止manager看门狗（检测软定时器）消息 */
    MSG_STOP_HARD_WATCHDG_SYNC,
    /*! 重启manager看门狗（检测软定时器）消息 */
    MSG_RESTART_HARD_WATCHDG_SYNC,

    /***************************Music engine message**************************/
    /*! Music engine消息占位符 */
    MENGINE_BASE        = 0x0300,
    /*! 获取引擎信息，音乐播放界面定时获取引擎信息 */
    MSG_MENGINE_GET_ENGINE_INFO_SYNC,
    /*! 获取当前文件路径 */
    MSG_MENGINE_GET_FILEPATH_SYNC,
    /*! 设置EQ模式 */
    MSG_MENGINE_SET_EQ_SYNC,
    /*! 设置播放模式 */
    MSG_MENGINE_SET_PLAYMODE_SYNC,
    /*! 设置显示模式 */
    MSG_MENGINE_SET_DISPMODE_SYNC,
    /*! 设置淡入淡出 */
    MSG_MENGINE_SET_FADE_SYNC,
    /*! 设置播放速度 */
    MSG_MENGINE_SET_PLAYSPEED_SYNC,
    /*! 设置AB复读模式: 自动或手动 */
    MSG_MENGINE_SET_AB_AUTO_SYNC,
    /*! 设置AB复读次数 */
    MSG_MENGINE_SET_AB_COUNT_SYNC,
    /*! 设置AB复读间隔 */
    MSG_MENGINE_SET_AB_GAP_SYNC,    
    /*! 设置AB复读A点 */
    MSG_MENGINE_SET_APOINT_SYNC,
    /*! 设置AB复读B点 */
    MSG_MENGINE_SET_BPOINT_SYNC,
    /*! 开始AB复读 */
    MSG_MENGINE_START_AB_SYNC,
    /*! 取消AB复读 */
    MSG_MENGINE_CLEAR_AB_SYNC,
    /*! 设置AB复读下一步: A点--> B点 */
    MSG_MENGINE_SET_AB_NEXT_SYNC,
    /*! 设置AB复读上一步: A点<-- B点 */
    MSG_MENGINE_SET_AB_PREV_SYNC,
    /*! 播放音乐 */
    MSG_MENGINE_PLAY_SYNC,
    /*! 停止音乐 */
    MSG_MENGINE_STOP_SYNC,
    /*! 暂停音乐 */
    MSG_MENGINE_PAUSE_SYNC,
    /*! 恢复音乐 */
    MSG_MENGINE_RESUME_SYNC,
    /*! 跳到指定时间播放音乐 */
    MSG_MENGINE_SEEK_SYNC,
    /*! 设置音乐播放文件(文件选择器方式) */
    MSG_MENGINE_SET_FILEPATH_SYNC,
    /*! 设置音乐播放文件(playlist方式)  */
    MSG_MENGINE_SET_FILEINDEX_SYNC,
    /*! 切换到下一首 */
    MSG_MENGINE_PLAY_NEXT_SYNC,
    /*! 切换到上一首 */
    MSG_MENGINE_PLAY_PREV_SYNC,
    /*! 删除文件 */
    MSG_MENGINE_DELETE_FILE_SYNC,
    /*! 快进 */
    MSG_MENGINE_FFWD_SYNC,
    /*! 快退 */
    MSG_MENGINE_FBWD_SYNC,
    /*! 取消快进或快退 */
    MSG_MENGINE_CANCEL_FFB_SYNC,
    /*! 设置快进或快退步长 */
    MSG_MENGINE_SET_FFBSTEP_SYNC,
    /*! 设置快进或快退播放时间 */
    MSG_MENGINE_SET_FFBTIME_SYNC,
    /*! 获取当前时间的书签信息 */
    MSG_MENGINE_MAKE_BOOKMARK_SYNC,
    /*! 设置上次播放文件 */
    MSG_MENGINE_SET_LAST_FILE_SYNC,
    /*! 清除错误状态: UI应用处理后清除 */
    MSG_MENGINE_CLEAR_ERR_SYNC,
    /*! 删除收藏夹，更改收藏夹序号和总数 */
    MSG_MENGINE_DEL_FAVORITE_SYNC,  
    /*! 添加收藏夹，更改收藏夹总数 */
    MSG_MENGINE_ADD_FAVORITE_SYNC,
    //for audible 
    /*! 切换到下一首 */
    MSG_MENGINE_CAHPTER_NEXT_SYNC,
    /*! 切换到上一首 */
    MSG_MENGINE_CHAPTER_PREV_SYNC,   
    /*! 获取当前的图片信息 */
    MSG_MENGINE_AUDIBLE_GETIMAGE_SYNC,  
    //===========music ui 新增消息 ======================
    
    /*! 获取信息 */
    MSG_MENGINE_GET_ENGINE_CFG_SYNC,
    /*! 获取循环模式+shuffle */
    MSG_MENGINE_GET_PLAYMODE_SYNC,
    /*! 获取播放模式 */
    MSG_MENGINE_GET_ENGINE_PLAYMODE_SYNC,
    /*! 获取文件格式 */
    MSG_MENGINE_GET_FILEINFO_SYNC,
    /*! 获取当前播放状态 */
    MSG_MENGINE_GET_STATUS_SYNC,
    /*! 获取当前播放时间和比特率 */
    MSG_MENGINE_GET_PLAYINFO_SYNC,  
    /*! 获取当前播放时间断点 */
    MSG_MENGINE_GET_BKINFO_SYNC, 
    /*! 获取当前章节总数和序数 */
    MSG_MENGINE_GET_SECTION_INFO_SYNC,   
    /*! 设置音量 */
    MSG_MENGINE_SET_VOLUME_SYNC,
    /*! 设置audible章节模式 */
    MSG_MENGINE_SET_SECTION_MODE_SYNC,
    /*! 播放书签 */
    MSG_MENGINE_PLAY_BOOKMARK_SYNC,
    /*! music播放场景删除文件 */
    MSG_MENGINE_DELFILE_MUSUI_SYNC,
    /*! music播放场景删除文件 */
    MSG_MENGINE_DELFILE_OTER_SYNC,
    /*! music改变文件播放路径 */
    MSG_MENGINE_CHANGE_FILEPATH_SYNC,
    /*! music standby播歌 */    
    MSG_MENGINE_STANDBY_PLAY_SYNC,
    /****************************fm engine message****************************/
    /*! fm engine消息占位符*/
    FMENGINE_BASE = 0x0400,

    /* 初始化及卸载相关*/
    /* 安装FM drv ,  执行FM 引擎进程后即安装*/
    MSG_FMENGINE_OPEN,                   
    /* FM 驱动IC  初始化配置*/
    MSG_FMENGINE_INIT,   
    /* FM 驱动IC 进入Standby 状态*/
    MSG_FMENGINE_STANDBY,    
    /* 接收到此命令后，卸载FM 驱动，并退出引擎*/
    MSG_FMENGINE_CLOSE,                       

    /* 设置相关*/   
    /* 设置频点播放( 异步) */
    MSG_FMENGINE_SETFREQ,          
    /* 设置频点播放( 同步)  */
    MSG_FMENGINE_SETFREQ_SYNC,       
    /* 设置静音或取消静音*/
    MSG_FMENGINE_SETMUTE,        
    /* 设置电台波段*/
    MSG_FMENGINE_SETBAND,
    /* 设置搜台门限值*/
    MSG_FMENGINE_SETTHRED,  
    /* 启动软件搜台*/
    MSG_FMENGINE_SET_SOFTSEEK,   
    /* 启动硬件搜台*/
    MSG_FMENGINE_SET_HARDSEEK,     
    /* 退出硬件搜台*/
    MSG_FMENGINE_SET_SEEKBREAK,     
    /* 设置音量大小*/
    MSG_FMENGINE_SET_VOLUME,           

    /* 获取信息相关*/
    /* 获取硬件自动搜台是否完成，对应Engine_Status_t.STC_flag */
    MSG_FMENGINE_AUTO_SEEK_OVER,     
    /* 获取全部状态信息，对应Engine_Status_t  结构*/
    MSG_FMENGINE_GET_STATUS, 
    /* 获取当前波段，对应Engine_Status_t.FM_CurBand */
    MSG_FMENGINE_GET_BAND,          
    /* 获取当前频率，对应Engine_Status_t.FM_CurrentFreq */
    MSG_FMENGINE_GET_FREQ,     
    /* 获取电台信号强度，对应Engine_Status_t.FM_SignalStg */
    MSG_FMENGINE_GET_INTENSITY, 
    /* 获取天线状态( 耳机是否插入) */
    MSG_FMENGINE_GET_ANTENNA,     
    /* 获取立体声状态信息，对应Engine_Status_t.FM_Stereo_Status */
    MSG_FMENGINE_GET_STEREO,       
    /* 获取引擎播放状态信息，对应Engine_Status_t.FM_VolStatus */
    MSG_FMENGINE_GET_PLAYSTATUS,   
    /* 获取音量大小*/
    MSG_FMENGINE_GET_VOLUME,       
    /* 获取当前模组状态*/
    MSG_FMENGINE_GET_MODUAL_STATUS,
    /* 重建FM模组全局数组内容*/
    MSG_FMENGINE_RESINTALL,
    
    /****************************user1 message****************************/
    /*! user1消息占位符*/
    USER1_BASE = 0x0500,
    /*! 测试消息 */
    MSG_USER1_PLAY,
    MSG_USER1_STOP,
    MSG_USER1_ASK_SYNC,
    
    /****************************** dialog事件 ****************************/
    /*! 对话框事件占位 */
    EVENT_DIALOG_BASE = 0x2000,
    /*! 对话框切换到上一个按钮事件 */
    EVENT_DIALOG_PREV_BUTTON,
    /*! 对话框切换到下一个按钮事件 */
    EVENT_DIALOG_NEXT_BUTTON,
    /*! 对话框确定按钮事件 */
    EVENT_DIALOG_CONFIRM,
    /*! 对话框取消选择事件 */
    EVENT_DIALOG_CANCEL,
    
    /****************************** user1事件 ****************************/
    /*! USER1事件占位 */
    EVENT_USER1_BASE = 0x2100,
    /*! 测试事件 */
    EVENT_USER1_MODE,
    EVENT_USER1_PLAY,
    
    /****************************** play scene 事件 ****************************/
    /*! 播放场景事件占位 */
    EVENT_PLAYING_BASE = 0x2200,
    /*! 快进事件 */
    EVENT_PLAYING_FFOREWORD,
    /*! 快退事件 */
    EVENT_PLAYING_FBACKWORD,
    /*! 下一曲 or AB 事件 */
    EVENT_PLAYING_NEXTANDAB,
    /*! 上一曲事件 */
    EVENT_PLAYING_PREVSONG,
    /*! option事件 */
    EVENT_PLAYING_OPTION,       
    /*! 播放/暂停事件 */
    EVENT_PLAYING_PAUSE,    
    /*! 返回事件 */
    EVENT_PLAYING_RETURN,           
    
    /****************************** music bookmark 事件 ****************************/
    /*! 音乐书签事件占位 */
    EVENT_BOOKMARK_BASE = 0x2280,
    /*! 上一个书签 */
    EVENT_BOOKMARK_SELECT_PREV,
    /*! 下一个书签 */
    EVENT_BOOKMARK_SELECT_NEXT,
    /*! 选择书签确认 */
    EVENT_BOOKMARK_SELECT_SURE,
    /*! option事件 */
    EVENT_BOOKMARK_OPTION,      
    /*! 返回事件 */
    EVENT_BOOKMARK_RETURN,      
    
    /****************************** video事件 ****************************/
    /*! 视频播放场景事件占位 */
    EVENT_VIDEO_PLAYING_BASE = 0x2300,  
    /*! 视频切下一曲或进入非全屏状态 */
    EVENT_VIDEO_PLAYING_NEXT,
    /*! 视频切上一曲或进入非全屏状态 */
    EVENT_VIDEO_PLAYING_PREV,
    /*! 视频播放/暂停或进入非全屏状态*/
    EVENT_VIDEO_PLAYING_PAUSE,
    /*! 视频结束快进/快退*/
    EVENT_VIDEO_PLAYING_END_SORTING,
    /*! 视频快进*/
    EVENT_VIDEO_PLAYING_FFORWORD,
    /*! 视频快退 */
    EVENT_VIDEO_PLAYING_FBACKWORD,
    /*! 视频返回*/
    EVENT_VIDEO_PLAYING_RETURN,
    /*! 视频option*/
    EVENT_VIDEO_PLAYING_OPTION, 
    /*! 视频音量*/
    EVENT_VIDEO_PLAYING_VOLUME,
    /*! 视频锁键*/
    EVENT_VIDEO_PLAYING_HOLD,

    /****************************** picture事件 ****************************/
    /*! 图片播放场景事件占位 */
    EVENT_PICTURE_PLAYING_BASE = 0x2400,
    /*! 图片切下一曲*/
    EVENT_PICTURE_PLAYING_NEXT,
    /*! 图片切上一曲*/
    EVENT_PICTURE_PLAYING_PREV,
    /*! 图片自动播放/手动播放*/
    EVENT_PICTURE_PLAYING_PAUSE,
    /*! 图片缩略图up键*/
    EVENT_PICTURE_PREVIEW_MOVEUP,
    /*! 图片缩略图down键*/
    EVENT_PICTURE_PREVIEW_MOVEDOWN,
    /*! 图片缩略图prev键*/
    EVENT_PICTURE_PREVIEW_MOVEPREV,
    /*! 图片缩略图next键*/
    EVENT_PICTURE_PREVIEW_MOVENEXT,
    /*! 图片返回*/
    EVENT_PICTURE_RETURN,
    /*! 图片option*/
    EVENT_PICTURE_OPTION,
    
    /****************************** volumebar事件 ****************************/
    /*! volumebar事件占位 */
    EVENT_VOLUME_BASE = 0x2500,
    /*! 音量递增事件 */
    EVENT_VOLUME_INC,
    /*! 音量递减事件 */
    EVENT_VOLUME_DEC,
    /*! 音量调节确认退出事件 */
    EVENT_VOLUME_CONFIRM,
    /*! 音量调节取消退出事件 */
    EVENT_VOLUME_CANCEL,
    
    /****************************** menulist事件 ****************************/
    /*! menulist事件占位 */
    EVENT_MENULIST_BASE = 0x2600,
    /*! 菜单列表选择下个选项事件 */
    EVENT_MENULIST_SELECT_NEXT,
    /*! 菜单列表选择上个选项事件 */
    EVENT_MENULIST_SELECT_PREV,
    /*! 菜单列表进入下一级事件 */
    EVENT_MENULIST_ENTER_SON,
    /*! 菜单列表返回上一级事件 */
    EVENT_MENULIST_BACK_PARENT,
    /*! 菜单列表进入 option 快捷菜单事件 */
    EVENT_MENULIST_ENTER_OPTION,
    
    /****************************** directory事件 ****************************/
    /*! directory事件占位 */
    EVENT_DIRECTORY_BASE = 0x2700,
    /*! 文件浏览器选择下个选项事件 */
    EVENT_DIRECTORY_SELECT_NEXT,
    /*! 文件浏览器选择上个选项事件 */
    EVENT_DIRECTORY_SELECT_PREV,
    /*! 文件浏览器选择暂停事件 */
    EVENT_DIRECTORY_SELECT_STOP,
    /*! 文件浏览器进入下一级事件 */
    EVENT_DIRECTORY_ENTER_SON,
    /*! 文件浏览器返回上一级事件 */
    EVENT_DIRECTORY_BACK_PARENT,
    /*! 文件浏览器进入 option 快捷菜单事件 */
    EVENT_DIRECTORY_ENTER_OPTION,
    
    /****************************** delete事件 ****************************/
    /*! delete事件占位 */
    EVENT_DELETE_BASE = 0x2800,
    /*! 删除操作停止删除后续文件事件 */
    EVENT_DELETE_STOP,
    
    /****************************** parameter事件 ****************************/
    /*! parameter事件占位 */
    EVENT_PARAMETER_BASE = 0x2900,
    /*! 参数设置框选择下个参数事件 */
    EVENT_PARAMETER_SELECT_NEXT,
    /*! 参数设置框选择上个参数事件 */
    EVENT_PARAMETER_SELECT_PREV,
    /*! 参数设置框值递增事件 */
    EVENT_PARAMETER_VALUE_INC,
    /*! 参数设置框值递减事件 */
    EVENT_PARAMETER_VALUE_DEC,
    /*! 参数设置框确定退出事件 */
    EVENT_PARAMETER_CONFIRM,
    /*! 参数设置框取消退出事件 */
    EVENT_PARAMETER_CANCEL,
    
    /****************************** text_read 事件 ****************************/
    /*! text_read事件占位 */
    EVENT_TEXTREAD_BASE = 0x2a00,
    /*! 文本阅读选择下页事件 */
    EVENT_TEXTREAD_NEXT_PAGE,
    /*! 文本阅读选择上页事件 */
    EVENT_TEXTREAD_PREV_PAGE,
    /*! 文本阅读选择下个选项事件 */
    EVENT_TEXTREAD_KEY_DOWN,
    /*! 文本阅读选择上个选项事件 */
    EVENT_TEXTREAD_KEY_UP,
    /*! 文本阅读选择播放事件 */
    EVENT_TEXTREAD_PLAY,
    /*! 文本阅读选择确定事件 */
    EVENT_TEXTREAD_CONFIRM,
    /*! 文本阅读选择取消退出事件 */
    EVENT_TEXTREAD_CANCEL,
    /*! 文本阅读选择菜单选项事件 */
    EVENT_TEXTREAD_OPTION,
    /*! 文本阅读选择返回事件 */
    EVENT_TEXTREAD_RETURN,
    
    /****************************** shutoff事件 ****************************/
    /*! shutoff事件占位 */
    EVENT_SHUTOFF_BASE = 0x2b00,
    /*! 关机UI退出事件 */
    EVENT_SHUTOFF_QUIT,
    
    /****************************** mainmenu事件 ****************************/
    /*! mainmenu事件占位 */
    EVENT_MAINMENU_BASE = 0x2c00,
    /*! 切换到下一个应用事件 */
    EVENT_MAINMENU_NEXT_ITEM,
    /*! 切换到上一个应用事件 */
    EVENT_MAINMENU_PREV_ITEM,
    /*! 选择创建应用事件 */
    EVENT_MAINMENU_CREATE_APP,
    /*! 进入 option 菜单事件 */
    EVENT_MAINMENU_OPTIONMENU,

    /****************************** udisk事件 ****************************/
    /*! udisk事件占位 */
    EVENT_UDISK_BASE = 0x2d00,
    /*! 退回到上一个应用事件 */
    EVENT_UDISK_EXIT_LAST,
    /*! 切换到MTP应用事件 */
    EVENT_UDISK_EXIT_MTP,

    /****************************** radio 事件 ****************************/
    /*! radio 事件占位 */
    EVENT_RADIO_BASE = 0x2e00,
    /*! 切换到上一个频率点事件 */
    EVENT_RADIO_PREV_FREQ,
    /*! 切换到下一个频率点事件 */
    EVENT_RADIO_NEXT_FREQ,
    /*! 切换到上一个预存电台事件 */
    EVENT_RADIO_PREV_PRESET,
    /*! 切换到下一个预存电台事件 */
    EVENT_RADIO_NEXT_PRESET,
    /*! 切换到播放或暂停事件 */
    EVENT_RADIO_PLAY_MUTE,
    /*! 向前自动搜台事件*/
    EVENT_RADIO_SEEK_PREV,
    /*! 向后自动搜台事件 */
    EVENT_RADIO_SEEK_NEXT,
    /*! 返回事件 */
    EVENT_RADIO_RETURN,
    /*! 操作option 事件 */
    EVENT_RADIO_OPTION,
    /*! 电台列表选择上一项事件*/
    EVENT_RADIO_SELECT_PREV,
    /*! 电台列表选择下一项事件 */
    EVENT_RADIO_SELECT_NEXT,
    /*! 电台列表选择当前项开始播放事件*/
    EVENT_RADIO_SELECT_CUR,
    
    /****************************** tools 事件 ****************************/
    /*! tools 事件占位 */
    EVENT_TOOLS_BASE = 0x2f00,
    /*! 日历切换年月日事件 */
    EVENT_TOOLS_CAL_SEL_YMD,
    /*! 日历向前调整年月日事件 */
    EVENT_TOOLS_CAL_YMD_NEXT,
    /*! 日历向后调整年月日事件 */
    EVENT_TOOLS_CAL_YMD_PREV,
    /*! 日历退出事件 */
    EVENT_TOOLS_RETURN,
     /****************************** mtp事件 ****************************/
    /*! mtp事件占位 */
    EVENT_MTP_BASE = 0x3000,
    /*! 退回到上一个应用事件 */
    EVENT_MTP_EXIT_LAST,
    
    /****************************** config事件 ****************************/
    /*! mainmenu事件占位 */
    EVENT_CONFIG_BASE = 0x3100,
    /*! 按下某个键事件 */
    EVENT_CONFIG_KEY_UP,
    /*! 长按某个键事件 */
    EVENT_CONFIG_KEY_HOLD,
} msg_apps_type_e;

/*!
 * \brief  
 *  sys_msg_t 系统消息结构体
 */
typedef struct
{
    /*! 应用消息类型 */
    msg_apps_type_e type;
} sys_msg_t;


/*!
 * \brief  
 *  msg_apps_t 应用消息结构体
 */
typedef struct
{
    /*! 应用消息类型，参见 msg_apps_type_e 定义 */
    uint32 type;
    /*! 应用消息内容 */
    union
    {
        /*! 消息内容真实数据 */
        uint8 data[4];
        /*! 消息内容缓冲区指针，指向消息发送方的地址空间 */
        void *addr; 
    }content;
} msg_apps_t;

/*!
 * \brief
 *  msg_reply_type_e 同步消息回执枚举类型
 */
typedef enum
{   
    /*! 同步消息无回复，用于应用退出时应答尚未处理的同步消息 */
    MSG_REPLY_NO_REPLY,
    /*! 回复同步消息发送者，同步消息要求功能成功完成 */
    MSG_REPLY_SUCCESS,
    /*! 回复同步消息发送者，同步消息要求功能完成失败 */
    MSG_REPLY_FAILED,
} msg_reply_type_e;
    
/*!
 * \brief
 *  msg_reply_t 同步消息回执结构体
 */
typedef struct
{
    /*! 同步消息回执枚举类型，参见 msg_reply_type_e 定义 */
    uint8 type;
    uint8 reserve[3];
    /*! 回执内容缓冲区指针，指向消息发送方的地址空间 */
    void *content;
} msg_reply_t;

/*!
 * \brief  
 *  private_msg_t （应用间）私有消息结构体
 */
typedef struct
{
    /*! 私有消息消息内容 */
    msg_apps_t msg;
    /*! 同步信号量 */
    os_event_t *sem;
    /*! 同步消息回执指针 */
    msg_reply_t *reply;
} private_msg_t;

/*!
 * \brief  
 *  msg_cb_func 应用消息处理回调函数
 */
typedef app_result_e (*msg_cb_func)(private_msg_t *private_msg);

/*!
 * \brief  
 *  app_msg_dispatch 应用消息处理回调函数，用于传递给 common 模块使用
 */
typedef app_result_e (*app_msg_dispatch)(private_msg_t *msg_dispatch);

#endif
