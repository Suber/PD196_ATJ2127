/*******************************************************************************
 *                              us212A
 *                            Module: Applib
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.  
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-9 17:27     1.0             build this file 
*******************************************************************************/
/*! 
 * \file     applib.h
 * \brief    Applib 模块相关数据结构，宏定义，接口声明等。
 * \author   lzcai
 * \version  1.0
 * \date     2011-9-9
*******************************************************************************/

#ifndef _applib_H_
#define _applib_H_

#include "psp_includes.h"
#include "case_type.h"
#include "gui_msg.h"
#include "app_msg.h"
#include "common_func.h"

//#define SUPPORT_FILTER_GRAB

/*! 方案最多支持 3 个应用同时运行 */
#define MAX_APP_COUNT   3
/*! 应用抓取 app msg 最多数目 */
#define MAX_GRAB_MSG    4
/*! 应用过滤 app msg 最多数目 */
#define MAX_FILTER_MSG  4

/*! 公共控制类定时器，用于系统功能定时 */ 
#define CONTROL_TIMER_COUNT     1
/*! 公共 UI 类定时器：
 *  1）用于 关机UI，屏保UI，动画显示等；
 *  2）用于 headbar 刷新；
 *  3）用于 ui_directory, ui_menulist 等控件激活项字符串滚屏；
 */
#define GUI_TIMER_COUNT         3
/*! 公共定时器数目 */
#define COMMON_TIMER_COUNT      (CONTROL_TIMER_COUNT + GUI_TIMER_COUNT) 

/*! 软看门狗溢出时间默认为 5秒 */
#define SOFT_WATCHDOG_TIMEOUT   5000

/*! 按键音使能标志位，bit[7] */ 
#define SOUND_BIT_KT_ENABLE     0x80
#define SOUND_BIT_KT_DISABLE    0x7f
/*! 音频输出状态位，bit[0-3]，见 soundout_state_e */
#define SOUND_BIT_SOUND_OUT     0x0f
#define SOUND_BIT_SOUND_OUT_CLR 0xf0

/*! 电池电压采样次数 */
#define BAT_SAMPLE_TIME     7
/*! 电池电压采样进位数 */
#define BAT_SAMPLE_CARRY    4

/*! 
 * \brief
 *  app_type_e 应用（进程）类型枚举类型，对应 g_app_info_vector 数组
 */ 
typedef enum 
{
    /*! 进程管理器 */
    APP_TYPE_PMNG       = 0,
    /*! 前台应用（gui进程） */
    APP_TYPE_GUI        = 1,
    /*! 后台应用（console进程） */
    APP_TYPE_CONSOLE    = 2,
} app_type_e;

/*! 
 * \brief
 *  app_info_t 应用信息结构体
 */ 
typedef struct
{
    /*! 结构体使用标志，1表示已被使用，0表示未被使用 */ 
    uint8 used; 
    /*! 进程ID号，见 case_type.h 中定义的列表 */
    uint8 app_id;
    /*! 应用类型，见app_type_e 定义 */
    uint8 app_type; 
    /*! 进程私有消息队列ID，见mq_id_e 定义 */
    uint8 mq_id;
    /*! 是否禁止屏保 */ 
    uint8 screensaver_forbidden;
    /*! 是否禁止省电关机 */ 
    uint8 auto_standby_forbidden;
    /*! 是否禁止睡眠关机 */ 
    uint8 timed_shutdown_forbidden; 
    /*! 是否禁止看门狗 */
    uint8 soft_watchdog_forbidden;
    /*! 软看门狗下次超时的时间 */
    uint32 soft_watchdog_timeout_expires;
    
#ifdef SUPPORT_FILTER_GRAB
    /*! 是否filter所有的msg*/
    uint8 filter_all_msg;
    /*! 被grab的msg数组 */
    msg_apps_type_e msg_grabed[MAX_GRAB_MSG];
    /*! 被filter的msg数组 */
    msg_apps_type_e msg_filtered[MAX_FILTER_MSG];
#endif
} app_info_t;

/*! 
 * \brief
 *  engine_type_e 引擎类型枚举类型，用于应答用户请求后台引擎类型
 */ 
typedef enum
{
    /*! 没有引擎 */ 
    ENGINE_NULL     = 0,
    /*! music引擎 */
    ENGINE_MUSIC    = 1,
    /*! fm引擎 */
    ENGINE_RADIO    = 2,
} engine_type_e;

/*! 
 * \brief
 *  engine_state_e 引擎状态枚举类型 
 */ 
typedef enum
{
    /*! 没有引擎 */ 
    ENGINE_STATE_NULL       = 0,
    /*! 引擎正在播放 */ 
    ENGINE_STATE_PLAYING    = 1,
    /*! 引擎处于暂停状态 */ 
    ENGINE_STATE_PAUSE      = 2,
} engine_state_e;

/*! 
 * \brief
 *  app_state_e 前台应用 UI 状态枚举类型
 */ 
typedef enum
{
    /*! 前台AP处于非播放 UI 状态 */ 
    APP_STATE_NO_PLAY           = 0,
    /*! 前台AP处于播放 UI 状态 */
    APP_STATE_PLAYING           = 1,
    /*! 前台AP处于播放 UI 状态，且希望背光常亮 */
    APP_STATE_PLAYING_ALWAYS    = 2,
} app_state_e;

/*! 
 * \brief
 *  card_state_e 卡状态枚举类型 
 */ 
typedef enum
{
    /*! 没卡 */ 
    CARD_STATE_CARD_NULL        = 0,
    /*! 有卡但没播放 */ 
    CARD_STATE_CARD_IN          = 1,
    /*! 没卡 */ 
    CARD_STATE_CARD_PLAY        = 2,
} card_state_e; 

/*! 
 * \brief
 *  cable_state_e 线缆状态枚举类型
 */ 
typedef enum
{
    /*! 没插 USB 线 */
    CABLE_STATE_CABLE_NULL      = 0,
    /*! 有插 USB 线 */
    CABLE_STATE_CABLE_IN        = 1,
    /*! 有插 ADAPTOR 线 */
    CABLE_STATE_ADAPTOR_IN      = 2,
} cable_state_e;

/*! 
 * \brief
 *  backlight_state_e 背光状态枚举类型
 */ 
typedef enum
{
    /*! 背光正常状态，亮着 */
    BACKLIGHT_STATE_NORMAL      = 0,
    /*! 背光变暗状态，暗着 */
    BACKLIGHT_STATE_DARK        = 1,
    /*! 背光关闭状态，关着 */
    BACKLIGHT_STATE_OFF         = 2,
} backlight_state_e;

/*! 
 * \brief
 *  app_info_state_t 系统当前状态结构体 
 */ 
typedef struct
{
    /*! 当前运行的引擎类型，见 engine_type_e 定义 */
    uint8 type; 
    /*! 引擎当前状态，正在播放还是暂停，见 engine_state_e 定义 */
    uint8 state;
    /*! 前台应用 UI 状态，分正在播放UI和非播放UI，见 app_state_e 定义 */
    uint8 app_state;
    /*! 过滤掉按键长按后续动作，即 KEY_TYPE_HOLD & KEY_TYPE_SHORT_UP */ 
    uint8 filter_key;
    /*! 按键锁状态，TRUE表示锁住，FALSE表示没锁 */
    bool keylock_state; 
    /*! 声音输出状态字，bit[7]表示是否使能按键音，bit[0-3]表示音频输出状态，见soundout_state_e */
    uint8 sound_state;
    /*! 当前音量大小，可用于前后台应用共享 */
    uint8 volume;
    /*! 背光状态，亮，暗，和关，见 backlight_state_e 定义 */
    uint8 backlight_state;
    /*! 屏幕保护状态，TRUE表示在屏幕保护中，FALSE表示不在屏幕保护中，该状态由屏幕保护程序控制 */
    bool screensave_state;
    /*! 卡状态，没卡，有卡，有卡且播放，见 card_state_e 定义 */ 
    uint8 card_state;
    /*! 线缆状态，没插 USB 线，有插 USB 线，见 cable_state_e 定义 */
    uint8 cable_state;
    /*! 耳机（天线）状态 */
    bool antenna_state;
    /*! 充电状态，见 charge_state_e 定义 */
    uint8 charge_state;
    /*! 电池电量，包括低电和5格电，6种状态 */
    uint8 bat_value;
    /*! 卡是否已经创建播放列表，与 card_state 没有直接关系 */
    bool card_plist;
    /*! 卡是否已经更新簇链 */
    bool card_update;
    /*! Video 应用专用，记录 Video 应用退出时是否播放，1表示播放，0表示不播放 */
    uint8 video_state;
} app_info_state_t; 

/*! 
 * \brief
 *  app_last_state_t 系统最近状态结构体，用于自动检测状态变化并处理 
 */ 
typedef struct
{
    /*! 最近退出的前台AP，以便返回 */
    uint8 last_app; 
    /*! 最近引擎类型，见 engine_type_e 定义 */
    uint8 last_type;
    /*! 最近引擎状态，正在播放还是暂停，见 engine_state_e 定义 */
    uint8 last_state;
    /*! 最近前台AP UI 状态，见 app_state_e 定义（暂时只用于视频记录退出前的状态） */
    uint8 last_app_state;
    /*! 最近卡状态，没卡，有卡，有卡且播放，见 card_state_e 定义 */ 
    uint8 last_card_state;
    /*! 最近线缆状态，没插 USB 线，有插 USB 线，见 cable_state_e 定义 */
    uint8 last_cable_state; 
    /*! 最近设置的背光时间，以0.5s 为单位 */
    uint8 last_light_timer; 
    /*! 最近设置屏幕保护模式 */ 
    uint8 last_screen_save_mode;
    /*! 最近设置睡眠关机时间 */ 
    uint8 last_sleep_timer; 
} app_last_state_t; 


/*! 
 * \brief
 *  应用级定时器所属场景 tag id，用于把定时器捆绑于某个场景，更好地管理应用下所有定时器。
 */ 
/*! AP 主场景 tag */
#define APP_TIMER_TAG_MAIN          0x00
/*! 系统场景 tag，虚拟场景，任何场景下都会执行 */
#define APP_TIMER_TAG_SYS           0xff
/*! common 模块虚拟场景标志ID从0x80开始，0x01 - 0x7f预留给 ap */
#define APP_TIMER_TAG_SLEEP         0x80
#define APP_TIEMR_TAG_ANIM          0x81
#define APP_TIEMR_TAG_DELETE        0x82
#define APP_TIEMR_TAG_DIALOG        0x83
#define APP_TIEMR_TAG_DIR           0x84
#define APP_TIEMR_TAG_KEYLK         0x85
#define APP_TIEMR_TAG_MENU          0x86//带路径记忆功能菜单列表
#define APP_TIEMR_TAG_MENU_SIMPLE   0x87//不带路径记忆功能菜单列表
#define APP_TIEMR_TAG_PARAM         0x88
#define APP_TIEMR_TAG_SCREEN        0x89
#define APP_TIEMR_TAG_SHUT          0x8a
#define APP_TIEMR_TAG_TEXTRD        0x8b
#define APP_TIEMR_TAG_USBCN         0x8c
#define APP_TIEMR_TAG_VOLUME        0x8d

/*! 
 * \brief
 *  timer_type_e 软定时器类型枚举结构
 */ 
typedef enum
{
    /*! 普通模式的定时器，自动周期发送 */
    TIMER_TYPE_NORMAL,
    /*! 单发定时器，只定时一次后进入TIMER_STATE_STOPED 状态 */
    TIMER_TYPE_SINGLE_SHOT, 
} timer_type_e; 

/*! 
 * \brief
 *  timer_state_e 软定时器状态枚举结构
 */ 
typedef enum
{
    /*! 正常计时状态 */ 
    TIMER_STATE_RUNNING,
    /*! 停止计时状态，只能通过restart重新开始计时 */
    TIMER_STATE_STOPED, 
    /*! 标志可用状态，可用来创建定时器 */
    TIMER_STATE_NOUSED, 
} timer_state_e;

/*! 
 * \brief
 *  timer_attrb_e 软定时器属性枚举结构，用来对软定时器资源进行初始分配
 */ 
typedef enum
{
    /*! UI 类软定时器，在背光熄灭后不再执行 ISR */
    APP_TIMER_ATTRB_UI, 
    /*! control 类软定时器，任何时候都会执行 ISR */ 
    APP_TIMER_ATTRB_CONTROL,
} timer_attrb_e;

/*! 
 * \brief
 *  timer_proc 软定时器服务例程 
 */ 
typedef void (*timer_proc)(void);

/*! 
 * \brief
 *  app_timer_t 软定时器管理结构体
 */ 
typedef struct  
{
    /*! 软定时器状态，见timer_state_e 定义，TIMER_STATE_NOUSED表示可用 */
    uint8 state;
    /*! 软定时器特性，见timer_attrb_e 定义 */
    uint8 attrb;
    /*! 软定时器 定时周期，单位为 1ms */            
    uint16 timeout; 
    /*! 软定时器类型，见timer_type_e 定义 */
    uint8 type; 
    /*! 软定时器场景标志 tag */ 
    uint8 tag;
    uint8 reserve[2];
    /*! 软定时器 下一次超时绝对时间点，单位为 1ms */
    uint32 timeout_expires; 
    /*! 软定时器 服务例程 */
    timer_proc func_proc;
} app_timer_t;



/*! 
 *  \brief
 *  g_app_info_vector 全局应用信息结构体数组，存放在系统全局空间
 */ 
#ifndef PC
extern app_info_t g_app_info_vector[MAX_APP_COUNT]; 
#else
extern DLL_IMP app_info_t g_app_info_vector[];
#endif

/*! 
 *  \brief
 *  g_app_info_state 全局系统当前状态结构体，存放在系统全局空间 
 */ 
#ifndef PC
extern app_info_state_t g_app_info_state;
#else
extern DLL_IMP app_info_state_t g_app_info_state;
#endif

/*! 
 *  \brief
 *  g_app_last_state 全局系统最近状态结构体
 */ 
#ifndef PC
extern app_last_state_t g_app_last_state;
#else
extern DLL_IMP app_last_state_t g_app_last_state;
#endif

/*! 
 *  \brief
 *  g_bat_sample 电池电量采样值，采样7次取平均值
 */ 
#ifndef PC
extern uint8 g_bat_sample[BAT_SAMPLE_TIME];
#else
extern DLL_IMP uint8 g_bat_sample[BAT_SAMPLE_TIME];
#endif

/*! 
 *  \brief
 *  g_bat_index 电池电量采样序号
 */ 
#ifndef PC
extern uint8 g_bat_index;
#else
extern DLL_IMP uint8 g_bat_index;
#endif

/*! 
 *  \brief
 *  g_this_app_info 当前应用信息结构体指针
 */ 
extern const app_info_t *g_this_app_info;

/*! 
 *  \brief
 *  thread_mutex 线程互斥信号量指针 
 */ 
extern os_event_t *thread_mutex;

/*! 
 *  \brief
 *  g_this_app_msg_dispatch 应用私有消息分发函数
 */ 
extern app_msg_dispatch g_this_app_msg_dispatch;

/*! 
 *  \brief
 *  g_app_timer_vector 应用级定时器数组指针，指向应用空间的定时器数组
 */ 
extern app_timer_t *g_app_timer_vector; 

/*! 
 *  \brief
 *  g_app_timer_count 应用级软定时器数目
 */ 
extern uint8 g_app_timer_count; 

/*! 
 *  \brief
 *  g_this_app_timer_tag 软定时器场景标志，AP场景为0
 */ 
extern uint8 g_this_app_timer_tag;


#ifdef PC
#define inline
#endif

///获取后台引擎类型 
static inline engine_type_e get_engine_type(void)
{
    return g_app_info_state.type;
}

///修改后台引擎类型
static inline void change_engine_type(engine_type_e type)
{
    g_app_info_state.type = type;
}

///获取后台引擎状态 
static inline engine_state_e get_engine_state(void) 
{
    return g_app_info_state.state;
}

///修改后台引擎状态 
static inline void change_engine_state(engine_state_e state)
{
    g_app_info_state.state = state; 
}

///获取后台引擎状态 
static inline uint8 get_filter_key(void)
{
    return g_app_info_state.filter_key; 
}

///修改后台引擎状态 
static inline void change_filter_key(uint8 key) 
{
    g_app_info_state.filter_key = key;
}

///获取按键锁状态，TRUE表示锁住，FALSE表示没锁
static inline bool get_keylock_state(void)
{
    return g_app_info_state.keylock_state;
}

///修改按键锁状态，TRUE表示锁住，FALSE表示没锁
static inline void change_keylock_state(bool keylock)
{
    g_app_info_state.keylock_state = keylock;
}

///获取按键音使能与否，TRUE表示使能，FALSE表示禁止
static inline bool get_keytone_enable(void) 
{
    return ((g_app_info_state.sound_state & SOUND_BIT_KT_ENABLE) != 0);
}

///获取音频输出状态，见soundout_state_e
static inline uint8 get_soundout_state(void)
{
    return (g_app_info_state.sound_state & SOUND_BIT_SOUND_OUT);
}

///获取当前音量值
static inline uint8 get_current_volume(void)
{
    return g_app_info_state.volume;
}

///设置当前音量值
static inline void set_current_volume(uint8 volume) 
{
    g_app_info_state.volume = volume;
}

///获取背光状态，TRUE表示背光亮，FALSE表示背光灭
static inline backlight_state_e get_backlight_state(void)
{
    return g_app_info_state.backlight_state;
}

///修改背光状态 
static inline void change_backlight_state(backlight_state_e state)
{
    g_app_info_state.backlight_state = state;
}

///获取屏幕保护状态，TRUE表示在屏幕保护中，FALSE表示不在屏幕保护中
static inline bool get_screensave_state(void)
{
    return g_app_info_state.screensave_state;
}

///修改屏幕保护状态 
static inline void change_screensave_state(bool state)
{
    g_app_info_state.screensave_state = state;
}

///获取前台应用 UI 状态，分正在播放UI和非播放UI 
static inline app_state_e get_app_state(void)
{
    return g_app_info_state.app_state;
}

///修改前台应用 UI 状态，分正在播放UI和非播放UI 
static inline void change_app_state(app_state_e state)
{
    g_app_info_state.app_state = state; 
}

///获取软定时器场景标志 
static inline uint8 get_app_timer_tag(void) 
{
    return g_this_app_timer_tag;
}

///修改软定时器场景标志 
static inline void change_app_timer_tag(uint8 tag)
{
    g_this_app_timer_tag = tag; 
}

///获取系统卡状态
static inline card_state_e get_card_state(void) 
{
    return g_app_info_state.card_state; 
}

///修改系统卡状态
static inline void change_card_state(card_state_e card_state)
{
    g_app_info_state.card_state = card_state;
}

///获取系统 USB 线缆状态
static inline cable_state_e get_cable_state(void)
{
    return g_app_info_state.cable_state;
}

///修改系统 USB 线缆状态
static inline void change_cable_state(cable_state_e cable_state)
{
    g_app_info_state.cable_state = cable_state; 
}

///获取耳机（天线）状态
static inline bool get_antenna_state(void)
{
    return g_app_info_state.antenna_state;
}

///修改耳机（天线）状态
static inline void change_antenna_state(bool in_out)
{
    g_app_info_state.antenna_state = in_out; 
}

///获取充电状态
static inline uint8 get_charge_state(void)
{
    return g_app_info_state.charge_state;
}

///修改充电状态
static inline void change_charge_state(uint8 state)
{
    g_app_info_state.charge_state = state; 
}

///获取电池电量
static inline uint8 get_bat_value(void)
{
    return g_app_info_state.bat_value;
}

///修改电池电量
static inline void change_bat_value(uint8 value)
{
    g_app_info_state.bat_value = value; 
}

///获取卡是否已经创建播放列表
static inline bool get_card_plist(void)
{
    return g_app_info_state.card_plist;
}

///修改卡是否已经创建播放列表
static inline void change_card_plist(bool plist)
{
    g_app_info_state.card_plist = plist; 
}

///获取卡是否已经整理簇链
static inline bool get_card_update(void)
{
    return g_app_info_state.card_update;
}

///修改卡是否已经整理簇链
static inline void change_card_update(bool update)
{
    g_app_info_state.card_update = update; 
}


///获取最近前台AP
static inline uint8 get_last_app(void)
{
    return g_app_last_state.last_app;
}

///获取最近前台AP UI 状态
static inline app_state_e get_last_app_state(void)
{
    return g_app_last_state.last_app_state;
}

///设置最近前台AP UI 状态
static inline void set_last_app_state(app_state_e state)
{
    g_app_last_state.last_app_state = state;
}



//应用（进程）管理模块接口声明（详细说明见接口定义）
extern bool applib_init(uint8 app_id, app_type_e type) __FAR__; 
extern bool applib_quit(void) __FAR__;


//应用级定时器管理模块接口声明（详细说明见接口定义）
extern int8 set_app_timer(timer_attrb_e attrb, uint16 timeout, timer_proc func_proc) __FAR__;
extern int8 set_single_shot_app_timer(timer_attrb_e attrb, uint16 timeout, timer_proc func_proc) __FAR__;
extern bool modify_app_timer(int8 timer_id, uint16 timeout) __FAR__;
extern bool stop_app_timer(int8 timer_id) __FAR__;
extern bool restart_app_timer(int8 timer_id) __FAR__;
extern bool kill_app_timer(int8 timer_id) __FAR__;
extern bool init_app_timers(app_timer_t *timers, uint8 count) __FAR__;
extern void handle_timers(void) __FAR__;


//消息通信管理模块接口声明（详细说明见接口定义）
//接收消息接口
bool get_gui_msg(input_gui_msg_t *input_msg) __FAR__;
//bool get_sys_msg(sys_msg_t *sys_msg);（内部接口，由get_app_msg调用）
bool get_app_msg(private_msg_t *private_msg) __FAR__;
bool get_app_msg_for_engine(private_msg_t *private_msg) __FAR__;


//发送消息接口
//bool send_sys_msg(sys_msg_t *sys_msg);（由psp实现）
//bool post_key_msg(input_gui_msg_t *input_msg);（由key 驱动实现）
bool send_sync_msg(uint8 app_id, msg_apps_t *msg, msg_reply_t *reply, uint32 timeout) __FAR__;
bool send_async_msg(uint8 app_id, msg_apps_t *msg) __FAR__; 
bool broadcast_msg(msg_apps_t *msg) __FAR__;
bool broadcast_msg_sys(msg_apps_t *msg) __FAR__;
//广播消息辅助接口
#ifdef SUPPORT_FILTER_GRAB
bool filter_msg(msg_apps_type_e msg_type) __FAR__;
bool unfilter_msg(msg_apps_type_e msg_type) __FAR__;
bool filter_all_msg(void) __FAR__;
bool unfilter_all_msg(void) __FAR__;
bool grab_msg(msg_apps_type_e msg_type) __FAR__;
bool release_msg(msg_apps_type_e msg_type) __FAR__; 
#endif

//同步消息回执接口
bool reply_sync_msg(private_msg_t *private_msg) __FAR__;

//消息通信管理模块初始化接口
bool applib_message_init(app_msg_dispatch msg_dispatch) __FAR__;

#endif
