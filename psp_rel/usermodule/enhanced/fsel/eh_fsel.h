/*******************************************************************************
 *                              US212A
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     eh_fsel.h
 * \brief    数据结构和常量定义
 * \author   liminxian
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#ifndef _eh_fsel_h

#define _eh_fsel_h

#include "enhanced.h"
#include "eh_module.h"
#include "eh_rdata_ex.h"

//先随机定位到组，再随机定位到组内的成员，播放完一组再切到下一组

#define SET_GROUP_MAX   64
#define SET_MEMBER_MAX  256
#define SET_PREV_MAX    16

extern uint16 shuffle_total;//shuffle的总个数
extern uint8 group_map[SET_GROUP_MAX / 8];//分组，共64组
extern uint8 member_map[SET_MEMBER_MAX / 8];//组内成员，共256
extern uint8 member_avg;//成员平均值，商值
extern uint8 member_avg_group;//平均值成员的组号起始
extern uint8 group_max;//有效的组号
extern uint8 member_max;//当前组的最大成员个数
extern uint8 use_member_flag;//使用组内shuffle功能
extern uint8 cur_group;//当前的组号（1，2，。。。
extern uint16 cur_member;//当前组内的成员号（1，2，。。。
extern uint16 filtrate_shuffle_no; //shuffle过滤播放序号，避免重复

extern uint16 play_num_list[SET_PREV_MAX];
extern uint8 cur_save_pt; //当前存的指针
//extern uint8  cur_play_pos; //当前播的指针
//extern uint8  cur_save_count; //回溯保存的个数
extern uint16 dir_no;

extern uint8 cur_save_prev_count;
extern uint8 cur_save_next_count;
extern uint8 cur_overturn_flag; //表翻转标记


bool init_shuffle(uint16 num_total);
uint16 shuffle_next_file(void);
uint16 shuffle_prev_file(void);

void init_fsel_sysdata(void);
extern uint32 fsel_open_object(uint8 open_mode) __FAR__;
extern bool fsel_close_object(void) __FAR__;

#endif

