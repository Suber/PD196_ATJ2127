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
 * \file     eh_lrc_rdata.c
 * \brief    歌词常驻数据
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"

/*********************************
 *** rdata  define
 *********************************/
uint16 lrc_cur_num; //歌词的页号
uint32 lrc_prev_time; //前一个时间
uint32 lrc_cur_time; //当前页歌词的时间
uint32 lrc_next_time; //下一页歌词的时间

uint16 lrc_lab_count; //时间标签总数


