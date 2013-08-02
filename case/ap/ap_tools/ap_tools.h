/*
 *******************************************************************************
 *                              USDK
 *                             Module: SETTING AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ap_setting.h
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */

#ifndef _TOOLS_H_
#define _TOOLS_H_
#include "tools_res.h"
#include "tools_sty.h"
#include "case_include.h"
#include "psp_includes.h"
#include "enhanced.h"
#include "mmm_id.h"
#include "alarm_common.h"


/*定义入口菜单编号*/
#define MENU_ENTRY_ID_MAIN        0x00
#define MENU_ENTRY_ID_MAIN_NOCARD 0x01

#define MAX_YEAR                  2098     /*万年历中最大年份*/
#define MIN_YEAR                  2000     /*万年历中最小年份*/

#define VALUE_12HOURS_FORMAT 0x00  //12小时制
#define VALUE_24HOURS_FORMAT 0x01  //24小时制

#define VALUE_TIME_HH_12H_MIN 1  //12小时制最小值
#define VALUE_TIME_HH_24H_MIN 0  //24小时制最小值
#define VALUE_TIME_HH_12H_MAX 12  //12小时制最大值
#define VALUE_TIME_HH_24H_MAX 23  //24小时制最大值
#define VALUE_TIME_HH_STEP 1  //小时调节步长

#define VALUE_TIME_MM_MIN 0  //分钟最小值
#define VALUE_TIME_MM_MAX 59  //分钟最大值
#define VALUE_TIME_MM_STEP 1  //分钟调节步长

#define VALUE_TIME_AM 0  //上午AM
#define VALUE_TIME_PM 1  //下午PM
#define VALUE_TIME_AMPM_STEP 1  //AMPM调节
#define POSITION_AMPM 2  //

#define VALUE_DD_MM_YYYY	0x00  //日期格式——日月年
#define VALUE_MM_DD_YYYY    0x01  //日期格式——月日年
#define VALUE_YYYY_MM_DD    0x02  //日期格式——年月日

#define FROM_MENU	  0  //从菜单进入日历
#define FROM_ALARM    1  //从闹钟进入日历
#define FROM_ALARMFM  2  //从定时FM进入日历

/*取绝对值*/
#define abs(x)  (((x)>0) ? (x) : -(x))
/*公历转阴历所用*/
#define baseYear   1901 //
#define baseMonth  1
#define baseDate   1
#define baseIndex  0
#define baseChineseYear   4597//4598-1
#define baseChineseMonth   11
#define baseChineseDate    11

/*TOOLS场景枚举*/
typedef enum
{
    //菜单场景
    SCENE_MENULIST = 0, 
    //日历场景
    SCENE_CALENDAR, 
    //秒表场景
    SCENE_STOPWATCH, 
    //闹钟场景
    //SCENE_ALARM, 
    //数据交互场景
    //SCENE_DATA_EXCHANGE, 
    //退出场景
    SCENE_EXIT
} scene_tools_e;

/*日期结构体*/
typedef struct
{
    uint16 year;
    uint8 month;
    uint8 day;
} DateVarType;

/*秒表状态*/
typedef enum
{   
    //关闭状态
    CloseMode = 0,
    //计时状态
    RunMode = 1,
    //停止状态
    StopMode = 2 
} sw_status_e;
//#define  CloseMode    0
//#define  RunMode      1
//#define  StopMode     2


/*变量声明*/
extern comval_t g_comval; //系统公共VM变量
extern alarm_vars_t  g_alarm_vars; //ALARM VM变量
extern uint8 alarm_set_flag;
extern uint8 fmrec_set_flag;
extern int8 g_halfsec_timer; //5s定时器
extern date_t date;
//extern time_t time;

//for stopwatch
extern StopWatch_time_t sw_realtime;
extern StopWatch_time_t swtime[5];
extern sw_status_e  sw_status;
//extern uint8  sw_itemnum;
extern uint8  sw_showflag;
extern uint8  CtcCntEnable;
//MSG_SD_OUT使用，GUI_DIRECTORY置起
extern bool g_dir_flag;

/*函数声明*/
extern app_result_e tools_menulist(void);
extern app_result_e calendar_menu_entry(void *menu_title_void);
extern app_result_e stopwatch_menu_entry(void *menu_title_void);
extern app_result_e dataexch_menu_entry(void *menu_title_void);
extern app_result_e fwupdata_menu_entry(void *menu_title_void);

extern app_result_e calendar_main(uint8 flag);
extern app_result_e stopwatch_main(void);

extern app_result_e option_callback(void *menu_title_void);
extern app_result_e tool_privmsg_deal(private_msg_t *private_msg) __FAR__;
extern void music_close_engine(void) __FAR__; //供rcode调用
//calendar callback
extern uint8 isLeapYear(uint16 y);
extern uint8 daysInMonth_Internal(uint16 year, uint8 month);

//stopwatch callback
extern void StopWatch_timer_proc(void);

//alarm callback
app_result_e set_alarm_time_callback(time_t *t_time);
app_result_e set_alarm_volume_callback(uint8 *v_volume);
app_result_e alarm_menu_entry(void *menu_title_void);
app_result_e fmrec_menu_entry(void *menu_title_void);
void alarm1_exit_callback(void *menu_title_void);
void fmrec_exit_callback(void *menu_title_void);
app_result_e alarm_ring_callback(uint8 disk_flag);
app_result_e browser_filelist_init(uint8 diskflag);
//for alarm
app_result_e set_alarm1_onoff_callback(void *menu_title_void);
app_result_e set_alarm1_off_callback(void *menu_title_void);
app_result_e set_alarm1_on_callback(void *menu_title_void);
app_result_e set_alarm1_time_callback(void *menu_title_void);
app_result_e set_alarm1_cyc_callback(void * title_action);
app_result_e alarm1_once_callback(void *menu_title_void);
app_result_e alarm1_days_callback(void *menu_title_void);
app_result_e alarm1_work_callback(void *menu_title_void);
app_result_e set_alarm1_ring_callback(void * title_action);
app_result_e alarm1_ring_sd_callback(void *menu_title_void);
app_result_e alarm1_ring_flash_callback(void *menu_title_void);
app_result_e alarm1_ring_card_callback(void *menu_title_void);
app_result_e alarm1_volume_callback(void *menu_title_void);
//for fmrec
app_result_e set_fm_onoff_callback(void *menu_title_void);
app_result_e set_fm_off_callback(void *menu_title_void);
app_result_e set_fm_on_callback(void *menu_title_void);
app_result_e set_fmtime_callback(void *menu_title_void);
app_result_e set_fmtime_on_callback(void *menu_title_void);
app_result_e set_fmtime_off_callback(void *menu_title_void);
app_result_e set_fm_cyc_callback(void * title_action);
app_result_e set_fm_once_callback(void *menu_title_void);
app_result_e set_fm_days_callback(void *menu_title_void);
app_result_e set_fm_work_callback(void *menu_title_void);
app_result_e fm_volume_callback(void *menu_title_void);
app_result_e set_fm_fq_callback(void *menu_title_void);
app_result_e fmrec_onoff_callback(void * title_action);
app_result_e fmrec_off_callback(void *menu_title_void);
app_result_e fmrec_on_callback(void *menu_title_void);
app_result_e set_fmfreq_main(void);
//for autoupgrade
extern app_result_e autoupgrade_menu_entry(void *menu_title_void);



#endif
