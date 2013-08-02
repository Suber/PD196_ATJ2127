/*******************************************************************************
 *                              us212A
 *                            Module: commonUI-ext in rom
 *                 Copyright(c) 2003-2009 Actions Semiconductor,
 *                            All Rights Reserved. 
 *
 * History:         
 *      <author>    <time>           <version >             <desc>
 *       lzcai     2011-10-12 11:04     1.0             build this file 
*******************************************************************************/
/*!
 * \file     common_ui_data.c
 * \brief    common ui 全局数据
 * \author   lzcai
 * \par      GENERAL DESCRIPTION:
 *               
 * \par      EXTERNALIZED FUNCTIONS:
 *               
 * \version 1.0
 * \date  2011-10-12
*******************************************************************************/

#include "common_ui.h"

//公共全局变量
comval_t *sys_comval;       //系统参数指针，通过 com_set_sys_comval 接口传递给 common 并保存在全局指针
int8 sys_counter_timer_id;  //系统定时功能（包括背光，屏保，省电关机，定时关机，等）计时器 timer id
int8 sys_status_timer_id;   //系统状态处理计时器 timer id
gui_direct_e g_gui_direct;  //当前 gui 显示方向

///headbar 全局变量
uint8 g_battery_display;    //在充电时用来显示用的电量
uint8 g_icon_id;            //应用图标frame id
uint8 g_sys_minute;         //最近系统时间分钟值
uint8 g_battery_value;      //电池电量，用来无充电时显示电池电量
uint8 g_headbar_update;     //headbar 刷新模式
headbar_style_t this_headbar_style; //headbar style组合结构体
headbar_icon_e this_headbar_icon_id;//headbar 当前显示应用图标ID
headbar_mode_e this_headbar_mode;   //headbar 当前显示模式

///gui_menulist 全局变量
menu_title_data_t g_menu_title; //当前菜单头结构体
menu_item_data_t g_menu_items[LIST_NUM_ONE_PAGE_MAX];   //当前菜单列表项数组
back_to_parent g_back_to_parent_func;   //菜单设置按返回键是否要调用回调函数
uint8 *g_back_to_parant_arg;    //菜单设置按返回键是否要调用回调函数标志

///gui_directory 全局变量
dir_brow_t g_dir_browser;       //目录头或列表头结构体
menu_title_data_t dir_menu_title;   //嵌入菜单头部结构
dir_control_t dir_control;      //文件浏览器控制结构体
dir_control_t last_dir_control; //备份当前目录控制结构体，这样就可以在下一级目录里重新进入该目录
file_brow_t g_file_list[LIST_NUM_ONE_PAGE_MAX];//文件浏览器当前列表项数组
uint16 speed_control;   //用于控制列表切换速度的计数变量
uint16 line_cnt;        //当前列表切换要切换项数

///音量条全局变量
int16 volume_value_last;        //上一次绘制音量条时音量值
uint8 volume_enter_forbidden;   //音量达到音量限制，禁止继续调高音量
bool g_vol_time_up_flag;        //音量条 4 秒无按键自动退出标志

///USB 选项对话框全局变量
bool usb_selecting_state;   //是否正在 USB 选项对话框中
uint8 g_usb_auto_select;    //USB 选项自动选择计时变量
int8 g_usb_timer_id;        //USB 选项自动选择timer id

///动画显示全局变量，用于定时刷动画帧
animation_callback g_anm_callback;  //每帧动画刷新之后执行的回调函数
style_infor_t *g_anm_sty_infor;     //动画对应 picbox style结构体
uint8 g_anm_index;          //动画帧序号计数变量
uint8 g_anm_direction;      //动画显示方向，分从头到尾和从尾到头

///关机确定UI全局变量
uint8 g_so_time;        //关机确定计时变量

///屏幕保护全局变量
uint8 g_ss_tag_backup;  //进入屏保前场景tag备份，以便退出屏保时恢复
uint8 g_ss_mode_backup; //进入屏保前 headbar 模式备份，以便退出屏保时恢复
uint8 g_ss_icon_backup; //进入屏保前 headbar 应用图标ID备份，以便退出屏保时恢复
int8  g_ss_timer_id;    //屏保UI定时器timer id
uint8 g_ss_clk_backup;  //进入屏保前 MCU 主频备份，以便退出屏保时恢复
gui_direct_e g_ss_direct_bk;    //进入屏保前 gui 方向备份，以便退出屏保时恢复
screen_save_mode_e g_ss_mode;   //当前屏保模式
uint8 g_ss_demo_mode_phase;     //屏保模式为演示模式时，演示图片切换计数变量
uint8 g_ss_demo_frame_cnt;      //屏保模式为演示模式时，演示图片总数
uint8 g_ss_album_art_phase;     //屏保模式为专辑图片时，专辑图片切换计数变量
uint8 g_ss_last_date;   //屏保模式为数字时钟时，最近日期备份，以便判断是否有日期更新
uint8 g_ss_last_time;   //屏保模式为数字时钟时，最近时间备份，以便判断是否有时间更新
bool g_ss_vip_event;    //是否发生需要退出屏保的重要事件，比如USB插入，等
uint8 g_ss_delay_flag;  //是否推迟退出屏保，用于优化复杂场景下退出屏保效果
com_albumart_cb_t g_com_paint_cb; //进入屏保显示相册图画回调函数

///按键锁全局变量
bool g_kl_time_is_up;   //按键锁延时时间已到标志

///专供 common 用缓冲区（UI bank组中）
uint8 com_str_buffer[192] _BANK_DATA_ATTR_; //192字节字符串缓冲区
uint8 com_name_buffer[52] _BANK_DATA_ATTR_; //52字节名字处理缓冲区
alarm_alarm_t g_alarm_temp _BANK_DATA_ATTR_;//alarm 专用缓冲区
alarm_fm_t g_aufm_temp _BANK_DATA_ATTR_;    //alarm 专用缓冲区
