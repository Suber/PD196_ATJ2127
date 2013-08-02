/*
 *******************************************************************************
 *                              USDK
 *                             Module: TOOLS AP
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ap_cfg_menu_tools.c
 * History:
 *      <johnsen>    <2011-9-2 9:18>           <v1.0 >             <desc>
 *
 *******************************************************************************
 */
/* include file of this application, 本应用的头文件 */
#include "ap_tools.h"

#define ENTRY_TOTAL 2
#define ITEM_TOTAL 34

/* define variable, 变量定义 */
/*定义入口菜单*/
const conf_item_head_t entrymenu =
{ "ENTRY MENU  ", ENTRY_TOTAL };

const conf_menu_entry_t entry[ENTRY_TOTAL] =
{
    { MENU_ENTRY_ID_MAIN, S_TOOLS, 0 },
    { MENU_ENTRY_ID_MAIN_NOCARD, S_TOOLS, 0 }
};
/*定义叶子菜单项*/
const conf_item_head_t item_head =
{ "MENU ITEM   ", ITEM_TOTAL };

const conf_menu_item_t item[ITEM_TOTAL] =
{
    { S_CALENDER, 0, &calendar_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 1, 0 }, /*日历*/
    { S_STOPWATCH, 0, &stopwatch_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 2, 0 }, /*秒表*/
    { S_ALARM, 0, &alarm_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 3, 0 }, /*闹钟*/

    //{ S_ALARM_1, NULL, NULL, NULL, NULL, NORMAL_MENU_ITEM,4,0}, /*闹钟1*/
    /*三级菜单 - 闹钟-闹钟1 - 闹钟开关*/
    { S_ALARM1_ONOFF, 0, &set_alarm1_onoff_callback, NULL, NULL, NORMAL_MENU_ITEM, 4, 0 },
    /*四级菜单 - 闹钟-闹钟1 - 闹钟关*/
    { S_ALARM1_OFF, 0, &set_alarm1_off_callback, NULL, NULL, RAIDO_MENU_ITEM, 5, 0 },
    /*四级菜单 - 闹钟-闹钟1 - 闹钟开*/
    { S_ALARM1_ON, 0, &set_alarm1_on_callback, NULL, NULL, RAIDO_MENU_ITEM, 6, 0 },
    /*三级菜单 - 闹钟-闹钟1 - 闹钟时间*/
    { S_ALARM1_TIME, 0, &set_alarm1_time_callback, NULL, NULL, NORMAL_MENU_ITEM, 7, 0 },
    /*三级菜单 - 闹钟-闹钟1 - 闹钟周期*/
    { S_ALARM1_CYC, 0, &set_alarm1_cyc_callback, NULL, NULL, NORMAL_MENU_ITEM, 8, 0 },
    /*四级菜单 - 闹钟-闹钟1 - 闹钟周期 - 单次*/
    { S_ALARM1_CYC_ONCE, 0, &alarm1_once_callback, NULL, NULL, RAIDO_MENU_ITEM, 9, 0 },
    /*四级菜单 - 闹钟-闹钟1 - 闹钟周期 - 每天*/
    { S_ALARM1_CYC_DAYS, 0, &alarm1_days_callback, NULL, NULL, RAIDO_MENU_ITEM, 10, 0 },
    /*四级菜单 - 闹钟-闹钟1 - 闹钟周期 - 按星期*/
    { S_ALARM1_CYC_WORK, 0, &alarm1_work_callback, NULL, NULL, RAIDO_MENU_ITEM, 11, 0 },
    /*三级菜单 - 闹钟-闹钟1 - 闹钟音乐*/
    { S_ALARM1_MUSIC, 0, &set_alarm1_ring_callback, NULL, NULL, NORMAL_MENU_ITEM, 12, 0 },
    /*四级菜单 - 闹钟-闹钟1 - 闹钟音乐 - 内置铃声*/
    { S_RING1_SD, 0, &alarm1_ring_sd_callback, NULL, NULL, RAIDO_MENU_ITEM, 13, 0 },
    /*四级菜单 - 闹钟-闹钟1 - 闹钟音乐 - 磁盘音乐*/
    { S_RING1_DISK, 0, &alarm1_ring_flash_callback, NULL, NULL, NORMAL_MENU_ITEM, 14, 0 },
    /*四级菜单 - 闹钟-闹钟1 - 闹钟音乐 - 卡盘音乐*/
    { S_RING1_CARD, 0, &alarm1_ring_card_callback, NULL, NULL, NORMAL_MENU_ITEM, 15, 0 },
    /*三级菜单 - 闹钟-闹钟1 - 闹钟音量*/
    { S_ALARM1_VOL, 0, &alarm1_volume_callback, NULL, NULL, NORMAL_MENU_ITEM, 16, 0 },
    /*三级菜单 - 闹钟-闹钟1 - 退出*/
    /*{ S_ALARM1_EXIT, 0, &alarm1_exit_callback, NULL, NULL, NORMAL_MENU_ITEM, 17, 0 },*/

    ///*二级菜单 - 闹钟-定时录音*/
    //    { S_ALARM_REC, NULL, NULL, NULL, NULL, NORMAL_MENU_ITEM,18,0}, /*定时录音*/
    ///*三级菜单 - 闹钟-定时录音 - 开关*/
    //    { S_AUREC_ONOFF, NULL, &set_rec_onoff_callback, NULL, NULL, NORMAL_MENU_ITEM,19,0},
    ///*四级菜单 - 闹钟-定时录音 - 关*/
    //    { S_AUREC_OFF, NULL, &set_rec_off_callback, NULL, NULL, RAIDO_MENU_ITEM,20,0},
    ///*四级菜单 - 闹钟-定时录音 - 开*/
    //    { S_AUREC_ON, NULL, &set_rec_on_callback, NULL, NULL, RAIDO_MENU_ITEM,21,0},
    ///*三级菜单 - 闹钟-定时录音 - 录音开始时间*/
    //    { S_AUREC_ON_TIME, NULL, &set_RECtime_ON_callback, NULL, NULL, NORMAL_MENU_ITEM,22,0},
    ///*三级菜单 - 闹钟-定时录音 - 录音结束时间*/
    //    { S_AUREC_OFF_TIME, NULL, &set_RECtime_OFF_callback, NULL, NULL, NORMAL_MENU_ITEM,23,0},
    ///*三级菜单 - 闹钟-定时录音 - 闹钟周期*/
    //    { S_AUREC_CYC, NULL, &set_rec_cyc_callback, NULL, NULL, NORMAL_MENU_ITEM,24,0},
    ///*四级菜单 - 闹钟-定时录音 - 闹钟周期 - 单次*/
    //    { S_AUREC_CYC_ONCE, NULL, &set_rec_once_callback, NULL, NULL, RAIDO_MENU_ITEM,25,0},
    ///*四级菜单 - 闹钟-定时录音 - 闹钟周期 - 每天*/
    //    { S_AUREC_CYC_DAYS, NULL, &set_rec_days_callback, NULL, NULL, RAIDO_MENU_ITEM,26,0},
    ///*四级菜单 - 闹钟-定时录音 - 闹钟周期 - 按星期*/
    //    { S_AUREC_CYC_WORK, NULL, &set_rec_work_callback, NULL, NULL, RAIDO_MENU_ITEM,27,0},

    /*二级菜单 - 闹钟-定时FM*/
    { S_ALARM_FM, 0, &fmrec_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 17, 0 }, /*定时FM*/
    /*三级菜单 - 闹钟-定时FM - 开关*/
    { S_AUFM_ONOFF, 0, &set_fm_onoff_callback, NULL, NULL, NORMAL_MENU_ITEM, 18, 0 },
    /*四级菜单 - 闹钟-定时FM - 关*/
    { S_AUFM_OFF, 0, &set_fm_off_callback, NULL, NULL, RAIDO_MENU_ITEM, 19, 0 },
    /*四级菜单 - 闹钟-定时FM - 开*/
    { S_AUFM_ON, 0, &set_fm_on_callback, NULL, NULL, RAIDO_MENU_ITEM, 20, 0 },
    /*三级菜单 - 闹钟-定时FM - 时间*/
    //    { S_AUFM_TIME, NULL, &set_FMtime_callback, NULL, NULL, NORMAL_MENU_ITEM,32,0},
    /*三级菜单 - 闹钟-定时录音 - 开始时间*/
    { S_START_TIME, 0, &set_fmtime_on_callback, NULL, NULL, NORMAL_MENU_ITEM, 21, 0 },
    /*三级菜单 - 闹钟-定时录音 - 结束时间*/
    { S_END_TIME, 0, &set_fmtime_off_callback, NULL, NULL, NORMAL_MENU_ITEM, 22, 0 },

    /*三级菜单 - 闹钟-定时FM - 周期*/
    { S_AUFM_CYC, 0, &set_fm_cyc_callback, NULL, NULL, NORMAL_MENU_ITEM, 23, 0 },
    /*四级菜单 - 闹钟-定时FM - 周期 - 单次*/
    { S_AUFM_CYC_ONCE, 0, &set_fm_once_callback, NULL, NULL, RAIDO_MENU_ITEM, 24, 0 },
    /*四级菜单 - 闹钟-定时FM - 周期 - 每天*/
    { S_AUFM_CYC_DAYS, 0, &set_fm_days_callback, NULL, NULL, RAIDO_MENU_ITEM, 25, 0 },
    /*四级菜单 - 闹钟-定时FM - 周期 - 按星期*/
    { S_AUFM_CYC_WORK, 0, &set_fm_work_callback, NULL, NULL, RAIDO_MENU_ITEM, 26, 0 },
    /*三级菜单 - 闹钟-定时FM - FM频段*/
    { S_AUFM_FQ, 0, &set_fm_fq_callback, NULL, NULL, NORMAL_MENU_ITEM, 27, 0 },
    /*三级菜单 - 闹钟-定时FM - 音量*/
    { S_AUFM_VOL, 0, &fm_volume_callback, NULL, NULL, NORMAL_MENU_ITEM, 28, 0 },
    /*三级菜单 - 闹钟-定时FM - 录音开关*/
    { S_RECORD_ONOFF, 0, &fmrec_onoff_callback, NULL, NULL, NORMAL_MENU_ITEM, 29, 0 },
    /*四级菜单 - 闹钟-定时录音 - 关*/
    { S_RECORD_OFF, 0, &fmrec_off_callback, NULL, NULL, RAIDO_MENU_ITEM, 30, 0 },
    /*四级菜单 - 闹钟-定时录音 - 开*/
    { S_RECORD_ON, 0, &fmrec_on_callback, NULL, NULL, RAIDO_MENU_ITEM, 31, 0 },
    /*三级菜单 - 闹钟-定时录音 - 退出*/
    /*{ S_ALARM2_EXIT, 0, &fmrec_exit_callback, NULL, NULL, NORMAL_MENU_ITEM, 33, 0 },*/

    /*一级菜单项-数据交互*/
    { S_DATAEXCH, 0, &dataexch_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 32, 0 }, /*数据交互*/
    /*一级菜单项-固件升级*/
    { S_FWUPDATA, 0, &fwupdata_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 33, 0 }, /*固件升级*/
    /*一级菜单项-自动升级*/
    { S_AUTOUPGRADE, 0, autoupgrade_menu_entry, NULL, NULL, NORMAL_MENU_ITEM, 34, 0 } /*自动升级*/    
};



#ifdef PC
menu_cb_func _get_menu_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;
   
    item_cnt = item_head.total;
    
    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_func;
        }
    }
    
    return NULL;
}
 
menu_cb_leaf _get_callback_func(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;
    
    item_cnt = item_head.total;
    
    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].callback;
        }
    }
    
    return NULL;
}

menu_cb_option _get_menu_option(uint16 str_id, uint16 str_id_sub)
{
    uint16 item_cnt, i;
    
    item_cnt = item_head.total;
    
    for(i = 0; i < item_cnt; i++)
    {
        if((item[i].major_id == str_id) && (item[i].minor_id == str_id_sub))
        {
            return item[i].menu_option;
        }
    }

    return NULL;
}
#endif

