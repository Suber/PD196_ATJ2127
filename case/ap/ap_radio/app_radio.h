/********************************************************************************
 *                            Module: radio_ui
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang    2011-09-13 15:00     1.0             build this file
 ********************************************************************************/
#ifndef __APP_RADIO_H__
#define __APP_RADIO_H__

#include "psp_includes.h"
#include "case_include.h"
#include "radio_sty.h"
#include "radio_res.h"
#include "setting_common.h"
#include "fmengine.h"

/*!定义FM  应用的任务栈和主线程优先级*/
#define    AP_RADIO_STK_POS  AP_FRONT_LOW_STK_POS
#define    AP_RADIO_PRIO     AP_FRONT_LOW_PRIO

/* 定义预设电台列表中所支持的保存电台数，需开出相应大小buffer*/
#define    MAX_STATION_COUNT    30

/* 定义各波段最小频点*/
#define    MIN_US     0x55cc                //87500 = 0x155cc
#define    MIN_JP      0x28E0               //76000 = 0x128E0
#define    MIN_EU      0x55cc               //87500 = 0x155cc
#define    MIN_USER    0x55cc   //0x28E0             //用户电台最小频点87.5MHz
/* 定义各波段最大频点*/
#define    MAX_US         0xa5e0                //108000
#define    MAX_JP         0x5f90                //90000
#define    MAX_EU         0xa5e0               //108000
#define    MAX_USER      0xa5e0              //用户电台最大频点108MHz
/* 定义用户电台列表文件打开模式*/
#define FS_OPEN_NORMAL		0x01	//可以seek
#define FS_OPEN_NOSEEK		0x00	//不可以seek
#define FS_OPEN_MODIFY		0x02    //支持追加写
/* 定义文件读操作扇区大小*/
#define SECTOR_SIZE     512

/* 定义dialog 选项框待选项*/
#define  BUTTON_YESNO_YES     0
#define  BUTTON_YESNO_NO       1

/* 用户列表文件中每个频率字串的字节长度，如093900*/
#define FREQ_LENTH 6
/* 用户列表文件中每个电台号字串的字节长度，如CH01*/
#define STANUM_LENTH   4

/* 电台列表场景每个列表项长度，如05   FM  107.10 */
#define LIST_STR_LENGTH    16

/*定义播放场景刷新bitmap */
#define PAINT_ALL                           0xff
#define PAINT_VOL                           0x01
#define PAINT_FREQ                         0x02
#define PAINT_STATION_INFOR        0x04
#define CLEAR_PAINT_ALL                0x7f
#define CLEAR_PAINT_VOL                0xfe
#define CLEAR_PAINT_FREQ              0xfd
#define CLEAR_PAINT_INFOR            0xfb

/*定义play  界面频率图片坐标*/
#define PIC_X_F1START       29                           //整数位数为2  位时，起始x  坐标
#define PIC_X_F1START_1    24                          //整数位数为3  位时，起始x  坐标
#define PIC_Y_F1START       43

/* 定义频率数字图片之间的像素增加值*/
#define PIC_X_INC1      11
#define PIC_X_INC2      11
#define PIC_X_INC3      12
#define PIC_X_INC4      5
#define PIC_X_INC5      11

/*定义竖状频率条相关*/
#define FREQ_BAR_HEIGHT     103
#define FREQ_BAR_X           8

/* 支持的应用总数*/
#define    MAX_APP_NUM    11

/* 使用短名创建文件*/
//#define    USE_83NAME_CREAT

//进入radio 播放界面模式，决定了退出时的返回场景
typedef enum
{
    /*  radio 主菜单选择手动调谐后进入, 返回radio 主菜单*/
    ENTER_PLAY_FROM_MAIN = 1,
    /* 电台列表选择频率播放进入，返回电台列表*/
    ENTER_PLAY_FROM_LIST,
    /* 正在播放，上一次播放进入，返回mainmenu 应用*/
    ENTER_PLAY_FROM_NOWPLAY,
} radio_playwin_mode_e;

//radio AP场景定义
typedef enum
{
    /* FM 入口主菜单场景*/
    FM_SCENE_MAINMENU = 1,
    /* FM 预设电台和用户电台场景（各最多支持30个电台）*/
    FM_SCENE_STALIST,
    /* FM 播放场景*/
    FM_SCENE_PLAYING,
    /* FM 子菜单场景，包括FM播放界面和电台列表下子菜单 */
    FM_SCENE_OPTMENU,
    /* 场景退出*/
    FM_SCENE_EXIT,
} radio_scene_e;

//radio AP option子场景定义
typedef enum
{
    /* 初始化，未进入option */
    NO_ENTER_SUB = 0,
    /* 电台列表下的option 子菜单*/
    SUBS_OPT_STATION,
    /* 播放界面下的option 子菜单*/
    SUBS_OPT_PLAY
} radio_opt_subscene_e;

//radio AP 预设电台列表的进入模式
typedef enum
{
    /* 正常进入显示，确定后开始播放*/
    ENTER_NORMAL = 1,
    /* 保存进入，确定后选择保存位置*/
    ENTER_ADD_STATION,
    /* 删除进入，确定后选择需删除电台*/
    ENTER_DEL_STATION
} radio_list_entry_e;

//电台列表显示模式
typedef enum
{
    /* 预设电台列表*/
    STATIONLIST_SAVE = 0,
    /* 用户电台列表*/
    STATIONLIST_USER,
} radio_listtype_e;

//用户电台列表的解析模式
typedef enum
{
    /* 所有频率解析，用于用户电台列表显示*/
    ALL_FREQ = 1,
    /* 从文件头开始，指定索引号的电台解析*/
    INDEX_FROM_START,
    /* 当前电台的下一个电台解析，解析函数内部使用*/
    NEXT_FROM_CUR,
} userlist_parse_e;

//用户电台列表的编码方式
typedef enum
{
    /* 多国语言内码编码*/
    ENCODE_MUL = 1,
    /* unicode 编码*/
    ENCODE_UNICODE,
} userlist_encode_e;

//进入voice 的入口
typedef enum
{
    //默认初始状态，未进入voice
    NO_ERTER_VOICE = 0,
    //从radio 主菜单选择FM 录制进入voice
    FROM_RADIO_MAINMENU,
    //从radio 的option 子菜单选择FM 录制进入voice
    FROM_OPTION_MENU
} enter_voice_mode_e;

//radio ui配置参数
typedef struct
{
    /* 魔术数*/
    uint16 magic;
    /* 预设列表( 普通频段)*/
    uint16 fmstation_us[MAX_STATION_COUNT];
    /* 预设列表( 日本频段)*/
    uint16 fmstation_jp[MAX_STATION_COUNT];
    /* 预设列表( 欧洲频段)*/
    uint16 fmstation_eu[MAX_STATION_COUNT];
    /* 当前播放的电台信息*/
    fm_play_status_t FMStatus;
    /* 当前播放波段, Band_MAX 表示当前播放为用户电台*/
    radio_band_e band_mode;
    /* 进入放音的入口，决定了返回到radioUI 的入口*/
    enter_voice_mode_e enter_voice;
    /* 配置模组采用的搜台模式( 默认模式可通过配置项配置)	*/
    FM_SeekMode_e seek_mode;
} radio_config_t;

/* radio 用户电台结构*/
typedef struct
{
    /* 用户电台列表频点，数组下标为电台号-1*/
    uint16 fmstation_user[MAX_STATION_COUNT];
    /* 当前播放的用户电台名称 */
    char station_name[40];
} radio_userlist_t;

/* 电台列表控制结构*/
typedef struct
{
    /*! 当前列表每页项数 */
    uint8 one_page_count;
    /*! 当前列表总项数 */
    uint8 total;
    /*! 当前显示的列表首项序号 */
    uint8 top;
    /*! 当前显示的列表尾项序号 */
    uint8 bottom;
    /*! 当前激活项位置 */
    uint8 list_no;
    /*! 当前激活项序数 */
    uint8 active;
    /*! 之前激活项序数 */
    uint8 old;
    uint8 reserved;
    /*! 列表项更新模式，参见 list_update_mode_e 定义 */
    list_update_mode_e update_mode;
    /*! 列表UI绘制模式，参见 list_draw_mode_e 定义 */
    list_draw_mode_e draw_mode;
} radiolist_control_t;

//add by liuwzh
//for alarm_fm
/* radio进入模式 */
extern uint8 g_radio_from_alarm;
/* alarm_fm的结束时间 */
extern time_t alarmfm_end_time; 
/* alarm_fm频段 */
//uint16 alarmfm_frequency;
/* alarm_fm音量 */
extern uint8 alarmfm_volume;

//全局变量声明
extern radio_scene_e g_radio_scene;
extern radio_config_t g_radio_config;
extern uint16 *pfreq;
extern uint16 *pfreq_list;
extern bool first_wait;
extern uint8 ui_icon_count;
extern radio_band_e g_menu_band;
extern uint16 Auto_tab[MAX_STATION_COUNT];
extern int8 timer_auto_seek;
extern int8 timer_flush_radioinfo;
extern Engine_Status_t g_engine_status;
extern comval_t g_comval;

extern const uint16 max_freq[4];
extern const uint16 min_freq[4];
extern uint8 g_radiolist_item[LIST_NUM_ONE_PAGE_MAX][LIST_STR_LENGTH];
extern uint8 Freqtab[7];

extern radio_listtype_e g_radio_listtype;
extern radio_list_entry_e g_stalist_entry;
extern radio_playwin_mode_e g_playwin_mode;
extern radio_opt_subscene_e g_option_type;
extern uint8 from_where;
extern bool need_reinstall;
extern bool need_set_freq;

extern uint8 paint_flag;
extern uint8 FMFreNumOld;
extern uint16 startfreq;
extern listbox_private_t radiolist_private;
extern radiolist_control_t list_control;

extern uint8 g_userlist_index;
extern uint8 g_userlist_total;
extern userlist_encode_e encode_mode;
extern radio_userlist_t g_user_station;
extern const char default_name[7];
extern const uint16 default_freq[3];
extern bool g_use_default;
extern uint8 g_userlist_table[MAX_STATION_COUNT];

//for user station parse
extern uint32 file_total_byte;
extern char drv_type;
extern uint32 vfs_mount_radio;
extern handle fp_radio;
extern uint8 g_first_open_engine;

//函数声明
extern bool radio_open_engine(void)
__FAR__;
extern bool radio_get_modual_status(Module_Status_e* status)
__FAR__;
extern bool radio_set_mute(FM_MUTE_e mode)
__FAR__;
extern bool radio_modual_init(void)
__FAR__;
extern app_result_e show_dialog(uint16 id, uint8 dialog_type, uint8 default_icon)
__FAR__;
extern app_result_e radio_app_msg_callback(private_msg_t *pri_msg)
__FAR__;
extern bool radio_set_band(radio_band_e mode);
extern bool radio_set_freq(uint16 freq)
__FAR__;
extern bool radio_set_hardseek(uint16 freq, uint8 dir)
__FAR__;
extern bool radio_set_softseek(uint16 freq, uint8 dir)
__FAR__;
extern bool radio_get_seekflag(void)
__FAR__;
extern bool radio_get_freq(void)
__FAR__;
extern bool radio_get_antenna(void)
__FAR__;	
extern bool radio_get_vol_status(void);
extern bool radio_break_hardseek(void)
__FAR__;
extern app_result_e deal_soft_autoseek(FM_SeekDir_e dir);
extern app_result_e deal_hard_autoseek(FM_SeekDir_e dir);

extern void init_point_freq(void)
__FAR__;
extern uint8 get_index_from_num(uint8 num)
__FAR__;
extern uint8 get_station_count(void)
__FAR__;
extern bool freq_change(uint8* buf, uint16 freq, uint8 mode);
extern uint8 whether_in_list(uint16 freq, uint16 *p_freq)
__FAR__;
extern uint8 get_real_station_num(uint8 num, FM_SeekDir_e dir)
__FAR__;

extern app_result_e show_saved_station(radio_list_entry_e entry_mode, uint8 from);
extern app_result_e show_user_station(void);
extern bool radio_parse_userlist(char* station_name, userlist_parse_e mode, uint8 num);

extern app_result_e radioUI_mainmenu(void);
extern app_result_e radioUI_scene_playing(radio_playwin_mode_e mode);
extern app_result_e radioUI_scene_stationlist(radio_listtype_e list_type, radio_list_entry_e entry_mode, uint8 from);
extern app_result_e radioUI_scene_optionlist(radio_opt_subscene_e mode);
extern app_result_e radio_AutoSearch_scene(FM_SeekMode_e mode, FM_SeekDir_e dir);
extern void clear_preset_list(radio_band_e band);

extern bool radio_engine_init(void);
extern void show_freq_in_play(void);
extern void show_station_num(void);
extern app_result_e scene_play_sysmsg(void);
extern void alarmfm_in_playing(void)
__FAR__;
#endif //__APP_RADIO_H__
