/*******************************************************************************
 *                              US212A
 *                            Module: radio ui
 *                 Copyright(c) 2003-2012 Actions Semiconductor
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *     mikeyang   2011-09-15          1.0              create this file
 *******************************************************************************/

#include "app_radio.h"

/******************************************************************************/
/*!
 * \par  Description:
 * \radio UI ap 使用的部分全局数据
 * \param[in]    none
 * \param[out]   none
 * \return       void the result
 * \retval
 * \retval
 * \note
 */
/*******************************************************************************/

/*****************************************************
 * 只读类全局变量
 ******************************************************
 **/

//各波段最大频点
const uint16 max_freq[4] =
{ MAX_US, MAX_JP, MAX_EU, MAX_USER };

//各波段最小频点
const uint16 min_freq[4] =
{ MIN_US, MIN_JP, MIN_EU, MIN_USER };

//默认用户电台频点
const uint16 default_freq[3] =
{ 0x5AE0, 0x8B50, 0xA25C }; //88.8, 101.2, 107.1

//默认用户电台名称
const char default_name[7] = "_CH01_";

//用户电台列表文件名( 只在flash  或卡盘的固定位置查找)
//#ifdef USE_83NAME_CREAT
//const char userlist[12] = "FM_LIST LST";
//#else
//FM 用户电台文件固定使用长名
const char userlist[16] = "FM_USERLIST.LST";
//#endif

/*****************************************************
 * 配置类相关变量
 ******************************************************
 **/

//系统全局变量
comval_t g_comval;

//radio ui 的配置项信息
radio_config_t g_radio_config;

/*****************************************************
 * 定时器相关变量
 ******************************************************
 **/

/*用于滚动显示用户电台信息的定时器id ,  80ms 定时*/
int8 timer_flush_radioinfo = -1;

/* 用于自动搜台界面刷新的定时器id */
int8 timer_auto_seek = -1;

/*****************************************************
 * 用户电台列表相关变量
 ******************************************************
 **/

//radio 用户电台列表结构信息
radio_userlist_t g_user_station;

//用户电台列表索引号和电台号映射表，
//数组下标为索引号，数组值为电台号
//用户电台最多支持编辑数MAX_STATION_COUNT
uint8 g_userlist_table[MAX_STATION_COUNT];

//当前所播放的用户电台索引
//索引号和电台号可以不同
uint8 g_userlist_index = 0xff;

//当前用户电台列表中有效的电台总数
uint8 g_userlist_total;

//用户电台列表文件的编码方式
userlist_encode_e encode_mode;

//是否使用默认的用户电台列表
bool g_use_default;

/* vfs 挂载节点*/
uint32 vfs_mount_radio;

/* 用户列表文件长度，字节数*/
uint32 file_total_byte;

/* 用户电台列表文件句柄*/
handle fp_radio;

//用户电台列表文件所在的存储器类型
char drv_type;

/*****************************************************
 * 界面显示相关变量
 ******************************************************
 **/

//自动搜台显示图标
uint8 ui_icon_count;

//记录之前FM 频率值的位数
uint8 FMFreNumOld;

//刷新标识
uint8 paint_flag = 0xff;

//频率值，ascill 码
uint8 Freqtab[7] =
{ 0 }; //107.55

//电台列表listbox 私有数据结构
listbox_private_t radiolist_private;

//电台列表控制结构
radiolist_control_t list_control;

/*****************************************************
 * 全局buffer
 ******************************************************
 **/

//自动搜台中用于保存有效电台的临时buffer
uint16 Auto_tab[MAX_STATION_COUNT];

//电台列表表项存储
uint8 g_radiolist_item[LIST_NUM_ONE_PAGE_MAX][LIST_STR_LENGTH];

/*****************************************************
 * 流程控制相关及其他
 ******************************************************
 **/

/* 指向当前所播放的电台列表表首*/
uint16 *pfreq;

/* 指向当前所浏览的电台列表表首*/
uint16 *pfreq_list;

//标识当前所处应用场景
radio_scene_e g_radio_scene;

//标识所浏览的电台列表类型
radio_listtype_e g_radio_listtype;

//进入radioUI 的入口模式
//app_param_e  g_enter_mode;

//当前所处的option menu类型
radio_opt_subscene_e g_option_type;

//播放场景的进入模式
radio_playwin_mode_e g_playwin_mode;

//预设电台列表的入口模式
radio_list_entry_e g_stalist_entry;

//FM 引擎当前状态信息
Engine_Status_t g_engine_status;

//波段菜单所选择的波段模式
radio_band_e g_menu_band;

//备份进行半自动搜台前的频率
uint16 startfreq;

//模组是否刚初始化标识
bool first_wait = TRUE;

/*从哪个清除预设进入预设列表
 电台列表的option 菜单进入，from_where =1
 播放界面的option 菜单进入，from_where =2
 关系到返回后回到哪个场景*/
uint8 from_where = 0xff;

//需要重新安装fm 驱动标志
//因为fm 录音时会冲掉fm 驱动常驻代码，从fm 录音回到fm 后需重装fm 驱动
bool need_reinstall = FALSE;

//回到播放场景时是否需要设置频率, 解决杂音或声音断续问题
bool need_set_freq = TRUE;

//是否第一次打开radio引擎(需要设置频点)
uint8 g_first_open_engine;



