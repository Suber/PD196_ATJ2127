/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-8 16:53     1.0             build this file 
*******************************************************************************/
/*!
 * \file     common_ui.h
 * \brief    公用控件模块相关数据结构，宏定义，接口声明等
 * \author   lzcai
 * \version  1.0
 * \date     2011-9-8
*******************************************************************************/

#ifndef _common_ui_H_
#define _common_ui_H_

#include "psp_includes.h"
#include "enhanced.h"
#include "applib.h"
#include "case_type.h"
#include "common_func.h"
#include "setting_common.h"
#include "alarm_common.h"
#include "key_interface.h"

/* 列表项（包括列表标题项）最长字符串缓冲区大小，52字节 */
#define LIST_ITEM_BUFFER_LEN    52

/* album art 图片显示位置数 */
#define ALBUM_ART_MAX_COUNT     6

/*! 音量设置模式 */
#define SET_VOLUME_VALUE    0
#define SET_VOLUME_LIMIT    1
#define SET_VOLUME_ALARM    2
//专门给视频应用使用
#define SET_VOLUME_ENTER    0
#define SET_VOLUME_INC      1
#define SET_VOLUME_DEC      2

/*! 音量最大值 31，总共 32 级 */
#define VOLUME_VALUE_MAX    31

/*! 电池电压等级数，表示显示低电，0~4 格电 6 个状态 */
#define BATTERY_GRADE_MAX   5

/*! 动画显示回调函数定义 */
typedef void (*animation_callback)(void);

/*!
 * \brief
 *  com_str_buffer ui bank 组共用 bank data，用于bank 间函数传递参数
 */
extern uint8 com_str_buffer[192] _BANK_DATA_ATTR_;
extern uint8 com_name_buffer[52] _BANK_DATA_ATTR_;
extern alarm_alarm_t g_alarm_temp _BANK_DATA_ATTR_;
extern alarm_fm_t g_aufm_temp _BANK_DATA_ATTR_;

/*!
 * \brief
 *  del_mode_e 删除模式枚举类型
 */
typedef enum
{
    /*! 删除单个文件 */
    DEL_MODE_FILE       = 0,
    /*! 删除整个文件夹或列表 */
    DEL_MODE_LIST       = 1,
} del_mode_e;

/*!
 * \brief
 *  list_update_mode_e 文件列表更新模式枚举类型
 */
typedef enum
{
    /*! 无需更新 */
    LIST_UPDATE_NULL        = 0,
    /*! 向下切页（行） */
    LIST_UPDATE_NEXT        = 1,
    /*! 向上切页（行） */
    LIST_UPDATE_PREV        = 2,
    /*! 重新更新当前页 */
    LIST_UPDATE_REUPDATE    = 3,
    /*! 从头更新，或从尾切换到头 */
    LIST_UPDATE_HEAD        = 4,
    /*! 从尾更新，或从头切换到尾 */
    LIST_UPDATE_TAIL        = 5,
} list_update_mode_e;

/*!
 * \brief
 *  list_menu_t 文件浏览器菜单参数数据结构
 */
typedef struct
{
    /*! 层级条件，-1表示不理会层级条件 */
    uint8  layer;
    /*! 应用ID */
    uint8  app_id;
    /*! 列表菜单入口ID */
    uint16 list_menu;
} list_menu_t;

/*!
 * \brief
 *  dir_com_data_t 文件浏览器控件初始化结构体
 */
typedef struct
{
    /*! 
     * 浏览模式，0表示从路径的第一级目录浏览，1表示直接跳到路径指向目录浏览；
     * bit7 为1表示允许根目录没有文件项而又菜单项的浏览；
     */
    uint8 browse_mode;
    /*! 文件浏览菜单列表的列表项数目，一般为1 */
    uint8 menulist_cnt;
    /*! 当前浏览目录树根节点层级，默认为0；比如为1，则从第1级子目录返回时要退出浏览器 */
    uint8 root_layer;
    /*! 是否删除成功后返回，TRUE表示是，FALSE表示否 */
    bool del_back;
    
    /*! option子菜单回调函数 */
    list_cb_option list_option_func;
    /*! 文件浏览器菜单列表 */
    list_menu_t *menulist;
} dir_com_data_t;

/*!
 * \brief
 *  del_com_data_t 文件（文件夹，列表）删除控件初始化结构体
 */
typedef struct
{
    /*! 删除对象名称，Unicode编码必须有0xfeff前缀 */
    uint8 *filename;
    /*! 删除文件或文件夹序号，为文件同文件夹总和中的序号 */
    uint16 del_no;
    /*! 删除模式，0表示删除文件，1表示删除文件夹或列表，见 del_mode_e 定义 */
    uint8 del_mode;
    /*! 是否删除正在播放文件，TRUE表示是，FALSE表示否（仅对删除文件有效） */
    bool del_playing;
    /*! 是否删除文件夹或列表本身，只对删除文件夹或列表有效 */
    bool del_self;
    uint8 reserve[3];
    /*! delete 对话框中选择“是”后执行的函数，为NULL表示无需执行 */
    void (*del_func)(void);
} del_com_data_t;

/*!
 * \brief
 *  dir_control_t 文件浏览器控制结构体
 */
typedef struct
{
    /*! 正在浏览磁盘盘符，DISK_C表示主盘，DISK_H表示卡盘 */
    uint8 dir_disk;
    /*! 文件选择（来源）类型，见 fsel_type_e 定义 */
    uint8 file_source;
    /*! 目录模板每页项数 */
    uint8 one_page_count;
    uint8 reserve;
    
    /*! 当前文件浏览器控制结构体 */
    dir_com_data_t *dir_com;

    /*! 项目列表有效区域top项序号，相对整个列表的序号，值从1开始 */
    uint16 top;
    /*! 项目列表有效区域bottom项序号，相对整个列表的序号，值从1开始 */
    uint16 bottom;
    /*! 项目列表项总数 */
    uint16 list_total;
    
    /*! 当前激活文件信息 */
    /*! 当前文件序号，相对整个列表的序号，值从1开始 */
    uint16 list_no;
    /*! 当前列表的激活项，值从1 ~ one_page_count*/
    uint16 active;
    /*! 当前列表旧的激活项，值从1 ~ one_page_count */
    uint16 old;
    
    /*! 文件列表更新模式，参见 list_update_mode_e 定义 */
    list_update_mode_e update_mode;
    /*! 列表控件绘制模式，参见 list_draw_mode_e 定义 */
    list_draw_mode_e draw_mode;
    /*! 列表需要更新目录头数据标志 */
    bool enter_dir;
} dir_control_t;


/*!
 * \brief
 *  menu_title_action_t 根菜单项确定函数返回的执行结构体
 */
typedef struct
{
    /*! 返回本级菜单style_id，可以更换ui_menulist传进来的默认style_id进行
     *  显示，-1表示沿用默认style_id
     */
    uint16 style_id;
    /*! 返回动态菜单入口ID，-1表示采用菜单头默认的根菜单ID */
    uint16 menu_entry;
    /*! 菜单标题（更准确说是菜单描述符）ID，-1表示采用菜单头字符串ID */
    uint16 title_id;
    /*! 返回激活项多国语言字符串ID，-1表示采用默认激活项 */
    uint16 str_id;
    /*! 返回动态菜单项字符串指针，NULL表示无效，并且不用理会source_id的值 */
    uint8 *ret_str;
    /*! 返回菜单配置工具中配置字符串ID，指向动态菜单项，-1表示菜单标题 */
    uint16 source_id;
    /*! 非激活项前置图标列表首项ID，只对 MENULIST_HEAD 模板有效 */
    uint16 head_id;
    /*! 激活项前置图标列表首项ID，只对 MENULIST_HEAD 模板有效 */
    uint16 head_act_id;
} menu_title_action_t;

/*!
 * \brief
 *  menu_com_data_t 菜单列表控件初始化结构体
 */
typedef struct
{
    /*! 应用ID */
    uint8 app_id;
    /*! 入口菜单ID */
    uint8 menu_entry;
    /*! 路径记忆ID */
    uint8 path_id;
    /*! 浏览模式，0表示从入口菜单0层，1表示直接跳到之前退出时最深层次 */
    uint8 browse_mode;
    /*! 首层菜单回调函数 */
    menu_cb_func menu_func;
} menu_com_data_t;

/*!
 * \brief
 *  menulist_stack_t 菜单列表嵌套堆栈结构体
 */
typedef struct
{
    /*! 当前入口菜单ID */
    uint8 cur_entry;
    /*! 当前入口菜单对应逻辑菜单数节点ID */
    uint8 ori_entry;
    /*! 当前根菜单ID */
    uint8 menu_id;
    /*! 当前菜单层次 */
    uint8 layer;
} menulist_stack_t;

/*!
 * \brief
 *  menulist_control_t 菜单列表控制结构体
 */
typedef struct
{
    /*! 默认菜单style类型，即调用 gui_menulist 时传递进来的style_id */
    uint16 style_id;
    /*! 当前入口菜单栈指针 */
    uint8 stack_pointer;
    /*! 当前级菜单每页项数 */
    uint8 one_page_count;
    
    /*! 当前菜单列表控制结构体 */
    menu_com_data_t *menu_com;
    
    /*! 路径记忆ID */
    uint8 path_id;
    /*! 路径记忆层级ID */
    uint8 layer_no;
    
    /*! 当前级菜单列表总项数 */
    uint16 total;
    /*! 当前显示的菜单列表首项序号 */
    uint16 top;
    /*! 当前显示的菜单列表尾项序号 */
    uint16 bottom;
    /*! 当前激活项位置 */
    uint16 list_no;
    /*! 当前选中项，仅对 RADIO 菜单有效，select为-1表示非 RADIO 菜单 */
    uint16 select;
    /*! 当前激活项序数 */
    uint16 active;
    /*! 之前激活项序数 */
    uint16 old;
    /*! 菜单项更新模式，参见 list_update_mode_e 定义 */
    list_update_mode_e update_mode;
    /*! 列表UI绘制模式，参见 list_draw_mode_e 定义 */
    list_draw_mode_e draw_mode;
    /*! 列表需要更新菜单头数据标志 */
    bool enter_menu;
} menulist_control_t;


/*!
 * \brief
 *  dialog_type_e 对话框类型枚举类型
 */
typedef enum
{
    /*! 含有按键选择的对话框 */
    DIALOG_BUTTON_INCLUDE       = 0x00,
    /*! 不含按键的简单提示对话框，弹出后按任意按键退出 */
    DIALOG_BUTTON_ANYKEY        = 0x01,
    /*! 不含按键的简单提示对话框，弹出后等待2秒钟后退出 */
    DIALOG_INFOR_WAIT           = 0x02,
    /*! 不含按键的简单提示对话框，弹出后立即退出 */
    DIALOG_INFOR_IMMEDIATE      = 0x03,
} dialog_type_e;

/*!
 * \brief
 *  button_type_e 对话框按键组合类型枚举类型
 */
typedef enum
{
    /*! 确定和取消 */
    BUTTON_OKCANCEl             = 0x00,
    /*! 终止、重试和忽略 */
    BUTTON_ABORTRETRYIGNORE     = 0x01,
    /*! 是、否和取消 */
    BUTTON_YESNOCANCEL          = 0x02,//由于历史原因，暂时挪用为BUTTON_YESNO，其NO按钮消息为RESULT_DIALOG_NO
    /*! 是和否 */
    BUTTON_YESNO                = 0x03,
    /*! 重试和取消 */
    BUTTON_RETRYCANCEL          = 0x04,
    /*! 没有按钮 */
    BUTTON_NO_BUTTON            = 0x05,
} button_type_e;

/*!
 * \brief
 *  button_list_item_t 按键列表项结构体
 */
typedef struct
{
    /*! 按键总数 */
    uint8 button_cnt;
    /*! 按键-消息映射数组，最多3个按键 */
    struct
    {
        /*! 按键资源字符串ID */
        uint16 str_id;
        /*! 按键资源字符串ID */
        uint16 result;
    } buttons[3];
} button_list_item_t;

/*!
 * \brief
 *  dialog_com_data_t 对话框控件初始化结构体
 */
typedef struct
{
    /*! 对话框类型，参见 dialog_type_e 定义 */
    uint8 dialog_type;
    /*! 对话框按键组合类型，参见 button_type_e 定义 */
    uint8 button_type;
    /*! 对话框标识图标ID，值为-1表示不显示 */
    uint16 icon_id;
    /*! 详细说明字符串联合体 */
    string_desc_t *string_desc;
    /*! 默认激活按键项 */
    uint8 active_default;
    /*! 对话框刷新模式，仅供应用层使用 */
    dialog_draw_mode_e draw_mode;
    /*! 保留字节 */
    uint8 reserve[2];
} dialog_com_data_t;


/*!
 * \brief
 *  param_com_data_t 参数设置控件初始化结构体
 */
typedef struct
{
    /*! ParamBox私有数据结构指针 */
    parambox_private_t *private_data;
    /*! ParamBox刷新模式，仅供应用层使用 */
    parambox_draw_mode_e draw_mode;
} param_com_data_t;


/*!
 * \brief
 *  headbar_icon_e 状态条图标ID枚举类型
 */
typedef enum
{
    /*! 音乐图标 */
    HEADBAR_ICON_MUSIC      = 0,
    /*! 视频图标 */
    HEADBAR_ICON_VIDEO      = 1,
    /*! 图片图标 */
    HEADBAR_ICON_PICTURE    = 2,
    /*! 收音图标 */
    HEADBAR_ICON_RADIO      = 3,
    /*! 浏览器图标 */
    HEADBAR_ICON_BROWSER    = 4,
    /*! 设置图标 */
    HEADBAR_ICON_SETTING    = 5,
    /*! 录音图标 */
    HEADBAR_ICON_RECORD     = 6,
    /*! 按键锁图标 */
    HEADBAR_ICON_KEYLOCK    = 7,
    /*! 视频图标 for video */
    HEADBAR_ICON_VIDEO_V    = 8,
    /*! 浏览器图标 for video */
    HEADBAR_ICON_BROWSER_V  = 9,
    /*! 按键锁图标 for video */
    HEADBAR_ICON_KEYLOCK_V  = 10,
    /*! 不显示，留给MainMenu */
    HEADBAR_ICON_NONE       = 11,
} headbar_icon_e;

/*!
 * \brief
 *  headbar_style_t 状态条显示元素结构体
 */
typedef struct
{
    /*! 背景图标 style id，-1表示不显示背景图标 */
    uint16 bg_sty;
    /*! 应用图标 style id，-1表示不显示应用图标 */
    uint16 icon_sty;
    /*! 系统时间 style id，-1表示不显示系统时间 */
    uint16 time_sty;
    /*! 电池图标 style id，-1表示不显示电池图标 */
    uint16 battery_sty;
    /*! 电缆状态图标 style id，-1表示不显示电缆状态图标 */
    uint16 cable_sty;
    /*! 卡状态图标 style id，-1表示不显示卡状态图标 */
    uint16 card_sty;
} headbar_style_t;

/*!
 * \brief
 *  headbar_mode_e 状态条显示模式枚举类型
 */
typedef enum
{
    /*! 不显示状态栏 */
    HEADBAR_MODE_NULL       = 0,
    /*! 显示默认状态栏 */
    HEADBAR_MODE_NORMAL     = 1,
} headbar_mode_e;

/*!
 * \brief
 *  headbar_update_e 状态条更新模式枚举类型
 */
typedef enum
{
    /*! 初始更新状态栏 */
    HEADBAR_UPDATE_INIT     = 0,
    /*! 全部全新状态栏 */
    HEADBAR_UPDATE_ALL      = 1,
    /*! 更新变化了的状态栏 */
    HEADBAR_UPDATE_CHANGE   = 2,
} headbar_update_e;


/*!
 * \brief
 *  screen_save_mode_e 屏幕保护类型枚举类型
 */
typedef enum
{
    /*! 关闭屏幕保护 */
    SCREEN_NULL             = 0x00,
    /*! 数字时钟，显示日期和时间 */
    SCREEN_DIGIT_CLOCK      = 0x01,
    /*! 音乐专辑图片，如果不是前台应用不是音乐，切换到数字时钟 */
    SCREEN_ALBUM_ART        = 0x02,
    /*! 关掉lcd背光灯 */
    SCRREN_SCREEN_OFF       = 0x03,
    /*! 演示模式，显示各种应用Demo界面 */
    SCREEN_DEMO_MODE        = 0x04,
} screen_save_mode_e;

/*!
 * \brief
 *  key_lock_mode_e 按键锁UI类型枚举类型
 */
typedef enum
{
    /*! 没有按键锁 */
    KEY_LOCK_MODE_NULL      = 0x00,
    /*! 按键锁上锁动画 */
    KEY_LOCK_MODE_LOCKING   = 0x01,
    /*! 按键锁锁住提示 */
    KEY_LOCK_MODE_HOLD      = 0x02,
    /*! 按键锁解锁动画 */
    KEY_LOCK_MODE_UNLOCKING = 0x03, 
} key_lock_mode_e;


/*!
 * \brief
 *  animation_com_data_t 动画显示初始化结构体
 */
typedef struct
{
    /*! 每帧图片显示时间间隔，单位是1ms */
    uint16 interval;
    /*! 动画方向，可以选择正向（0）和方向（1） */
    uint8 direction;
    /*! 是否允许中止，TRUE表示不允许中止，FALSE表示允许中止 */
    uint8 forbid;
    /*! 终止动画显示的按键映射列表，为NULL表示允许所有快捷键终止动画 */
    const key_map_t *key_map_list;
    /*! 每帧动画中执行回调函数 */
    animation_callback callback;
} animation_com_data_t;


/*!
 * \brief
 *  ext_show_param_t 文本显示参数结构体
 */
typedef struct
{
    /*! 显示一页行数 */
    uint8 line_count_one_page;
    /*! 显示一行像素点宽度 */
    uint8 max_width_one_line;
    /*! 显示模式，包括过滤空行，分词显示等 */
    uint8 mode;
    /*! 语言类型 */
    int8  language;
} text_show_param_t;

/*!
 * \brief
 *  text_buf_t 文本阅读解码缓冲区结构体
 */
typedef struct
{
    /*! 一行字符串缓冲区 */
    uint8 text_prev[BUFF_ONE_ROW];
    /*! 一个扇区缓冲区，512个字节 */
    uint8 text_buf[FILE_SECTOR];
    /*! 缓冲区中有效数据指针 */
    uint8 *text_buf_valid;
    /*! 缓冲区中有效数据长度 */
    uint16 text_buf_length;
    /*! 缓冲区中剩余字节数 */
    uint8 remain;
    /*! buffer 数据是否有效 */
    bool valid;
    /*! 文本显示参数结构体 */
    text_show_param_t param;
    /*! text 显示行回调函数 */
    void (*text_show_line)(string_desc_t *string_desc, uint8 index);
} text_decode_t;

/*!
 * \brief
 *  text_file_t 文本阅读虚拟文件结构体，为了兼容SD文件和文件系统文件
 */
typedef struct
{
    /*! 打开的文件句柄 */
    void* file_handle;
    /*! 文件总扇区数 */
    uint32 file_sectors;
    /*! 文件总字节数 */
    uint32 file_length;
    /*！文件偏移地址，以字节为单位 */
    uint32 file_offset;
    /*! 文件定位接口函数指针，以扇区为单位 */
    bool (*fseek) (void* file_handle, uint8 where, int32 sector_offset);
    /*! 文件数据读接口函数指针，以扇区为单位 */
    bool (*fread) (void* file_handle, uint8 *text_buf, uint32 sector_count);
} text_file_t;

/*!
 * \brief
 *  gui_direct_e gui 显示方向枚举类型
 */
typedef enum
{
    /*! 小机正向方向，在此方案为竖向（宽128*高160） */
    GUI_DIRECT_NORMAL       = 0,
    /*! 小机顺时针旋转90度方向，横向（宽160*高128） */
    GUI_DIRECT_HORIZONTAL   = 1,
} gui_direct_e;


/*!
 * \brief
 *  sys_comval 指向全局 comval 变量，用于在 Common 模块访问 comval
 */
extern comval_t *sys_comval;
/*!
 * \brief
 *  sys_counter_timer_id 系统功能定时器id
 */
extern int8 sys_counter_timer_id;
/*!
 * \brief
 *  sys_status_timer_id 系统状态更新定时器id
 */
extern int8 sys_status_timer_id;
/*!
 * \brief
 *  g_gui_direct 系统 gui 方向，横屏还是竖屏
 */
extern gui_direct_e g_gui_direct;

/*!
 * \brief
 *  g_light_timer_bk 备份背光时间
 */
extern uint8 g_light_timer_bk;

 
//文件浏览和文件删除接口声明（详细说明见接口定义）
extern dir_brow_t g_dir_browser;
extern file_brow_t g_file_list[LIST_NUM_ONE_PAGE_MAX];
extern menu_title_data_t dir_menu_title;
extern dir_control_t dir_control;
extern dir_control_t last_dir_control;
extern uint16 speed_control;
extern uint16 line_cnt;
extern app_result_e gui_directory (uint16 style_id, file_path_info_t *path_info, dir_com_data_t *dir_com) __FAR__;
extern app_result_e gui_delete (file_path_info_t *path_info, del_com_data_t *del_com) __FAR__;

//菜单列表接口声明（详细说明见接口定义）
extern menu_title_data_t g_menu_title;
extern menu_item_data_t g_menu_items[LIST_NUM_ONE_PAGE_MAX];
extern back_to_parent g_back_to_parent_func;
extern uint8 *g_back_to_parant_arg;
extern app_result_e gui_menulist (uint16 style_id, menu_com_data_t *menu_com) __FAR__;
extern app_result_e gui_menulist_simple (uint16 style_id, menu_com_data_t *menu_com) __FAR__;

#ifdef PC
#define inline
#endif
//设置返回父菜单回调函数
static inline void gui_menulist_set_back2parent(back_to_parent func, uint8 *arg_addr)
{
    g_back_to_parent_func = func;
    g_back_to_parant_arg = arg_addr;
}

/*!
 * \brief
 *  专辑图片结构体定义
 */
typedef struct
{
    /*! 非内嵌图片目录项簇号 */
    uint32 cluster_no;
    /*! 非内嵌图片相对目录项偏移，内嵌图片相对文件偏移 */
    uint32 offset;
    /*! 显示窗尺寸坐标 */
    region_t region;
    /*! 图片类型 */
    uint32 apic_type;
} album_picinfo_t;

/*!
 * \brief  
 *  刷专辑图片回调函数
 */
typedef struct
{
    void (*func)(album_picinfo_t *album_picinfo);
    album_picinfo_t *arg;
}com_albumart_cb_t;

#ifdef PC
typedef menu_cb_func (*__get_menu_func)(uint16 major_id, uint16 minor_id);
typedef menu_cb_leaf (*__get_callback_func)(uint16 major_id, uint16 minor_id);
typedef menu_cb_option (*__get_menu_option)(uint16 major_id, uint16 minor_id);

extern __get_menu_func this_get_menu_func;
extern __get_callback_func this_get_callback_func;
extern __get_menu_option this_get_menu_option;

extern void load_menulist_simfunc(__get_menu_func func1, __get_callback_func func2, __get_menu_option func3);
#endif

//对话框接口声明（详细说明见接口定义）
extern bool usb_selecting_state;
extern uint8 g_usb_auto_select;
extern int8 g_usb_timer_id;
extern app_result_e gui_dialog (uint16 style_id, dialog_com_data_t *dialog_com) __FAR__;
extern app_result_e gui_dialog_msg (uint16 style_id, dialog_type_e type, uint16 str_id) __FAR__;
extern app_result_e gui_usbconnect (void) __FAR__;

//参数设置框接口声明（详细说明见接口定义）
extern int16 volume_value_last;
extern uint8 volume_enter_forbidden;
extern bool g_vol_time_up_flag;
extern app_result_e gui_set_parameter (uint16 style_id, param_com_data_t *param_com) __FAR__;
extern app_result_e gui_volumebar (uint8 *volume, uint8 *limit, uint8 mode) __FAR__;

//文本阅读控件接口声明（详细说明见接口定义）
extern text_end_mode_e text_decode_one_page(text_decode_t *text_decode, text_file_t *text_file, \
uint16 *page_bytes) __FAR__;
extern app_result_e gui_text_read(uint16 style_id, const char* file_name) __FAR__;

//状态栏接口声明（详细说明见接口定义）
extern uint8 g_battery_display;
extern uint8 g_icon_id;
extern uint8 g_sys_minute;
extern uint8 g_battery_value;
extern uint8 g_headbar_update;
extern headbar_style_t this_headbar_style;
extern headbar_icon_e this_headbar_icon_id;
extern headbar_mode_e this_headbar_mode;
extern void gui_set_headbar_mode(headbar_mode_e mode, headbar_icon_e icon_id) __FAR__;
extern void gui_get_headbar_mode(headbar_mode_e *mode, headbar_icon_e *icon_id) __FAR__;
extern app_result_e gui_headbar(headbar_update_e update) __FAR__;
extern void gui_headbar_handle(void) __FAR__;

//屏幕保护，按键锁，关机UI接口声明（详细说明见接口定义）
extern uint8 g_so_time;
extern uint8 g_ss_tag_backup;
extern uint8 g_ss_mode_backup;
extern uint8 g_ss_icon_backup;
extern gui_direct_e g_ss_direct_bk;
extern int8  g_ss_timer_id;
extern uint8 g_ss_clk_backup;
extern screen_save_mode_e g_ss_mode;
extern uint8 g_ss_demo_mode_phase;
extern uint8 g_ss_demo_frame_cnt;
extern uint8 g_ss_album_art_phase;
extern uint8 g_ss_last_date;
extern uint8 g_ss_last_time;
extern bool g_ss_vip_event;
extern uint8 g_ss_delay_flag;
extern bool g_kl_time_is_up;
extern com_albumart_cb_t g_com_paint_cb;
extern app_result_e gui_screen_save_enter (screen_save_mode_e ss_mode) __FAR__;
extern app_result_e gui_screen_save_exit (void) __FAR__;
extern app_result_e gui_keylock (bool lockkey) __FAR__; 
extern app_result_e gui_shut_off (void) __FAR__;

//动画显示接口声明（详细说明见接口定义）
extern animation_callback g_anm_callback;
extern style_infor_t *g_anm_sty_infor;
extern uint8 g_anm_index;
extern uint8 g_anm_direction;
extern app_result_e gui_animation(style_infor_t *style_infor, animation_com_data_t *anm_com) __FAR__;
extern app_result_e gui_logo (style_infor_t *style_infor, animation_com_data_t *anm_com) __FAR__;

//杂项功能接口声明（详细说明见接口定义）
extern app_result_e com_app_sleep(uint32 sleep_timer, const key_map_t *key_map_list) __FAR__;
extern void sys_timer_init(void) __FAR__;
extern void sys_timer_exit(void) __FAR__;
extern void sys_counter_handle(void) __FAR__;
extern void sys_status_handle(void) __FAR__;
extern app_result_e com_message_box(msg_apps_type_e msg_type) __FAR__;
extern void com_get_default_setting(setting_comval_t *setting_comval) __FAR__;
extern void com_set_sys_comval(comval_t *comval) __FAR__;
extern void com_set_gui_direction(gui_direct_e direct) __FAR__;
extern gui_direct_e com_get_gui_direction(void) __FAR__;
extern app_result_e com_alarm_msg_dispatch(void) __FAR__;
extern void set_timer_msg(alarm_fm_t *aufm_temp, alarm_alarm_t *alarm_temp, uint8 *timer_flag) __FAR__;
extern uint8 com_get_battery_grade(bool now) __FAR__;
extern void com_set_contrast(uint8 lightness) __FAR__;
extern void com_app_msg_hook(private_msg_t *private_msg) __FAR__;

#endif
