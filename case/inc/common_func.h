/*******************************************************************************
 *                              us212A
 *                            Module: Common
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-9-8 16:53     1.0             build this file 
*******************************************************************************/
/*!
 * \file     common_func.h
 * \brief    公用控件模块相关数据结构，宏定义，接口声明等
 * \author   lzcai
 * \version  1.0
 * \date     2011-9-8
*******************************************************************************/

#ifndef _common_func_H_
#define _common_func_H_

#include "psp_includes.h"
#include "enhanced.h"
#include "case_type.h"
#include "gui_msg.h"
#include "app_msg.h"
#include "setting_common.h"

/*! 菜单路径记忆类型 */
#define PATH_HISTORY_MENU   0
/*! 文件浏览路径记忆类型 */
#define PATH_HISTORY_DIR    1

/*! 叶子菜单的类型值type[0-14] = 0x7fff */
#define MENU_TYPE_LEAF      0x7fff
/*! 入口菜单头的father_index值 -1表示入口菜单项 */
#define MENU_TYPE_ENTRY     0xffff

/*! “部分...”字符串最大长度，暂为 84 像素点 */
#define ELLIPSIS_MAX_LEN    84

/*!
 * \brief  
 *  menu_cb_func 菜单项确定执行回调函数
 * \li  param: 指向根菜单项确定函数返回的执行结构体 menu_title_action_t
 */
typedef app_result_e (*menu_cb_func)(void *param);

/*!
 * \brief  
 *  menu_cb_leaf 叶子菜单即时回调函数
 */
typedef void (*menu_cb_leaf)(void);

/*!
 * \brief  
 *  menu_cb_option 菜单项option子菜单回调函数
 */
typedef app_result_e (*menu_cb_option)(void);

/*!
 * \brief  
 *  back_to_parent 返回父菜单回调函数
 */
typedef void (*back_to_parent)(void);

/*!
 * \brief  
 *  list_cb_option 文件浏览器option子菜单回调函数
 * \li  path_info 文件浏览器获取到的当前文件/文件夹 location
 * \li  list_no 文件浏览器当前激活项
 */
typedef app_result_e (*list_cb_option)(file_path_info_t *path_info, uint16 list_no);

/*!
 * \brief  
 *  param_adjust_func 参数值检测适配回调函数，比如用来限制日期设置等
 * \li  params 参数数组或指针
 * \li  active 当前正在设置参数的序号
 * \li  inc 当前操作是否为参数值递增
 */
typedef adjust_result_e (*param_adjust_func)(parambox_one_t *params, uint8 active, bool inc);

/*!
 * \brief  
 *  param_cb_func 设置即时回调函数，比如声音设置等
 * \li  value 当前参数值
 */
typedef bool (*param_cb_func)(int16 value);

/*!
 * \brief  
 *  sound_start_func 启动设备声音输出回调函数
 * \li  volume 当前音量值
 */
typedef bool (*sound_start_func)(uint8 volume);

/*!
 * \brief  
 *  sound_stop_func 停止设备声音输出回调函数
 * \li  volume 当前音量值
 */
typedef bool (*sound_stop_func)(void);

/*!
 * \brief
 *  history_item_t 路径记忆项结构体
 */
typedef struct
{
    /*! 当前显示列表首项编号，0xffff表示无效 */
    uint16 top;
    /*! 激活项在当前显示列表中的位置，0xffff表示无效 */
    uint16 list_no;
} history_item_t;

/*!
 * \brief
 *  history_index_t 路径记忆索引结构体
 */
typedef struct
{
    /*! 路径类型，0表示菜单列表，1表示文件浏览 */
    uint8 type;
    /*! 应用ID，取值范围：0 ~ 31 */
    uint8 app_id;
    /*！路径记忆ID，取值范围：0 ~ 7(3) */
    uint8 path_id;
    /*! 当前层级，取值范围：0 ~ 7(15) */
    uint8 layer;
} history_index_t;

/*!
 * \brief  
 *  menu_item_type_e：菜单项类型枚举类型
 */
typedef enum
{
    /*! 普通菜单项 */
    NORMAL_MENU_ITEM        = 0,
    /*! 单选按钮菜单项 */
    RAIDO_MENU_ITEM         = 1,
} menu_item_type_e;

//仅供应用开发人员编写菜单配置项使用，pc菜单配置工具可识别
/*!
 * \brief  
 *  conf_entry_head_t 菜单配置脚本文件的入口菜单头部结构体
 */
typedef struct
{
    /*! 入口菜单标识字符串，表示入口菜单配置项的开始，默认为 ENTRY MENU */
    uint8  key_string[14];
    /*! 入口菜单配置项总数 */
    uint16 total;
} conf_entry_head_t;

/*!
 * \brief  
 *  conf_menu_entry_t 入口菜单配置项结构体
 */
typedef struct
{
    /*! 入口菜单配置项的索引号 */
    uint16 menu_id;
    /*! 入口菜单配置项显示标题ID--主ID */
    uint16 major_id;
    /*! 入口菜单配置项显示标题ID--副ID，如果minor_id非0，pc工具则使用minor_id显示，否则使用major_id进行显示 */
    uint16 minor_id;
} conf_menu_entry_t;

/*!
 * \brief  
 *  conf_item_head_t 菜单配置脚本文件的菜单项头部结构体
 */
typedef struct
{
    /*! 菜单项识别标志字符串，表示菜单项定义的开始 */
    uint8  key_string[14];
    /*! 菜单项总数 */
    uint16 total;
} conf_item_head_t;

/*!
 * \brief  
 *  conf_menu_item_t 菜单项结构体
 */
typedef struct
{
    /*! 菜单项显示字符ID--主ID */
    uint16 major_id;
    /*! 菜单项显示字符ID--副ID，如果minor_id非0，pc工具则使用minor_id显示，否则使用major_id进行显示 */
    uint16 minor_id;
    /*! 菜单功能函数 */    
    menu_cb_func menu_func;  
    /*! 实时菜单回调函数,为NULL 表示非实时菜单 */      
    menu_cb_leaf callback; 
    /*! 菜单option的回调函数 */
    menu_cb_option menu_option;
    /*! 菜单项类型，详细参见 menu_item_type_e 定义 */
    uint8 type;
    /*! 菜单项的索引号，供菜单编辑保存时使用*/
    uint8 item_id;
    /*! 图片列表index，用于前置图标与具体菜单项绑定的情况*/
    uint8 pic_index;
    /*! 保留字节 */
    uint8 reserve;
} conf_menu_item_t;

//小机内部使用的数据结构，由pc菜单配置工具生成
/*!
 * \brief
 *  menu_title_data_t 菜单头数据结构
 */
typedef struct
{
    /*! 菜单头资源字符串ID */
    uint16 str_id;
    /*! 当前菜单的菜单项个数 */
    uint16 count;
    /*! 菜单item data 开始地址 */
    uint16 offset;
    /*! 默认的菜单活动项，通过工具设置 */
    uint16 active_default;
    /*! 当前菜单的上一级菜单的索引，如果当前菜单为入口根菜单，此项为0x7fff */
    uint16 father_index;
    /*! 当前菜单列表对应的根菜单项在父菜单中的编号 */
    uint16 father_active;
} menu_title_data_t;

/*!
 * \brief
 *  menu_title_data_t 菜单项数据结构
 */
typedef struct
{
    /*! bit<15>表示菜单是否为radio按钮，0表示否，1表示是
     *  bit<14-0>表示菜单项类型：=0x7fff，表示叶子菜单; 其他值表示根菜单，值表示指向的菜单头索引值
     */
    uint16 child_index;     
    /*! 菜单项资源字符串ID */
    uint16 str_id;
    /*! 菜单显示字符副ID，在主ID相同的情况下，模拟器上需要使用此ID找到各个回调函数的地址 */
    uint16 str_id_sub; 
    /*! 图片列表index，用于前置图标与具体菜单项绑定的情况 */
    uint8 pic_index;
    /*! 保留字节 */
    uint8 reserve; 
    /*! 菜单项确定执行回调函数
     *  对于菜单头项，返回值不处理，返回 RESULT_NULL 即可；而叶子菜单项则需要谨慎选择返回值
     */
    menu_cb_func menu_func;
    /*! 即时叶子菜单回调函数，非NULL表示该叶子菜单为即时叶子菜单，NULL表示不是即时叶子菜单 */
    menu_cb_leaf callback;
    
    /*! PHILIPS特性支持，菜单项option回调函数 */
    menu_cb_option menu_option;  
} menu_item_data_t;


/*!
 * \brief
 *  key_map_t 按键（gui）消息映射结构体
 */
typedef struct
{
    /*! 源按键事件 */
    key_event_t key_event;
    /*! 映射 gui 事件 */
    msg_apps_type_e event;
} key_map_t;

/*!
 * \brief
 *  conf_type_e 配置脚本文件句柄
 */
typedef enum
{
    /*! 字符串参数 */
    CONF_TYPE_STRING        = 0,
    /*! 线性数值参数 */
    CONF_TYPE_NUM_LINEAR    = 1,
    /*! 非线性数值参数 */
    CONF_TYPE_NUM_NONLINEAR = 2,
} conf_type_e;

/*!
 * \brief
 *  config_index_t 配置索引项数据结构
 */
typedef struct
{
    /*! 配置项数据相对文件头的偏移 */
    uint32 offset;
    /*! 配置项数据长度 */
    uint32 length;
} config_index_t;

/*!
 * \brief
 *  config_string_t 字符串配置项数据结构
 */
typedef struct
{
    /*! 字符串内容，可变长数组，内容与txt输入一致，以'\0'结束 */
    uint8 value[1];
} config_string_t;

/*!
 * \brief
 *  config_linear_t 线性数值配置项数据结构
 */
typedef struct
{
    /*! 默认数值 */
    uint16 default_value;
    /*! 取值区间的最小值 */
    uint16 min;
    /*! 取值区间的最大值 */
    uint16 max;
    /*! 步长 */
    uint16 step;
} config_linear_t;

/*!
 * \brief
 *  config_nonlinear_t 非线性数值配置项数据结构
 */
typedef struct
{
    /*! 默认数值 */
    uint16 default_value;
    /*! 有效值个数 */
    uint16 total;
    /*! 有效值数组，可变长数组 */
    uint16 value[1];
} config_nonlinear_t;

/*!
 * \brief
 *  soundout_state_e 声音输出状态枚举类型
 */
typedef enum
{
    /*! 开始输出声音，开启pa */
    SOUND_OUT_START         = 0,
    /*! 停止输出声音，关掉pa */
    SOUND_OUT_STOP          = 1,
    /*! 暂停输出声音，不关pa */
    SOUND_OUT_PAUSE         = 2,
    /*! 恢复输出声音，不开pa */
    SOUND_OUT_RESUME        = 3,
} soundout_state_e;


//可配置菜单接口声明
extern sd_handle confmenu_fp;
extern uint32 confmenu_offset;
extern bool com_open_confmenu_file(const char* file_name) __FAR__;
extern bool com_close_confmenu_file(void) __FAR__;
extern bool com_get_confmenu_title(uint8 menu_id, menu_title_data_t* confmenu_title) __FAR__;
extern bool com_get_confmenu_item(menu_title_data_t* confmenu_title, uint16 item_index, \
menu_item_data_t *confmenu_item) __FAR__;
extern uint16 com_get_confmenu_active(menu_title_data_t* confmenu_title, uint16 key_str) __FAR__;

//路径记忆类接口声明
extern bool dir_history_valid;
extern bool menu_history_valid;
extern uint8 *dir_history_addr;
extern uint8 *menu_history_addr;
extern bool com_get_history_item(history_index_t *index, history_item_t *history) __FAR__;
extern bool com_set_history_item(history_index_t *index, history_item_t *history) __FAR__;
extern bool com_update_path_history(history_index_t *index) __FAR__;
extern bool com_clear_all_history(uint8 type) __FAR__;
extern bool com_clear_app_history(uint8 type, uint8 app_id, uint8 disk) __FAR__;

//按键映射接口声明
extern bool com_key_mapping(input_gui_msg_t *input_msg, msg_apps_type_e *gui_event, \
const key_map_t *key_map_list) __FAR__;

//配置项接口声明
extern bool com_open_config_file(const char* config_file) __FAR__;
extern bool com_close_config_file(void) __FAR__;
extern bool com_get_config_struct(uint16 config_id, uint8 *buf, uint16 buf_len) __FAR__;
extern uint16 com_get_config_default(uint16 config_id) __FAR__;//仅用于数值型参数

//字符串处理接口声明
extern bool com_string_format(string_desc_t *pattern, string_desc_t *strings) __FAR__;
extern void com_dot_to_shortname(uint8 *short_name) __FAR__;
extern void com_ansi_to_unicode(uint8 *file_name) __FAR__;
extern uint16 com_ellipsis_to_longname(uint8 *long_name, uint8 *ellipsis_name, uint16 ellipsis_len) __FAR__;
extern void com_dump_debug(uint16 x, uint16 y, uint8 *format, uint32 value) __FAR__;

//common key处理接口声明
extern uint8 this_filter_key_value;
extern bool com_gui_msg_hook(input_gui_msg_t *input_msg) __FAR__;
extern void com_filter_key_hold(void) __FAR__;

//声音输出管理接口声明
extern bool g_keytone_backup;
extern bool com_set_sound_keytone(uint8 kt_enable) __FAR__;
extern bool com_set_sound_out(bool sound_out, soundout_state_e state, void *sound_func) __FAR__;
extern bool com_set_sound_volume(uint8 volume) __FAR__;
extern bool com_set_sound_record(bool on_off, uint8 type) __FAR__;

//按键映射表
extern const key_map_t delete_key_map_list[];
extern const key_map_t dialog_key_map_list[];
extern const key_map_t directory_key_map_list[];
extern const key_map_t keylock_list[];
extern const key_map_t menulist_key_map_list[];
extern const key_map_t parameter_key_map_list[];
extern const key_map_t shutoff_key_map_list[];
extern const key_map_t textread_key_map_list[];
extern const key_map_t usbconnect_key_map_list[];
extern const key_map_t volume_key_map_list[];
extern const key_map_t limit_key_map_list[];
extern const key_map_t key_shortcut_list[];

#endif
