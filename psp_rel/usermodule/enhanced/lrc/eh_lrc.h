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
 * \file     eh_lrc.h
 * \brief    数据结构和常量定义
 * \author   liminxian
 * \version 1.0
 * \date  2011/09/15
 *******************************************************************************/

#ifndef _eh_lrc_h

#define _eh_lrc_h

#include "eh_module.h"
#include "eh_rdata_ex.h"

#define MINUTE_VALID  2   //分有效个数
#define SECOND_VALID  3  //秒级有效位
#define MS_VALID  3  //10MS级有效位
#define MAX_MINUTE    99
#define MAX_LAB_COUNT 800  //7k空间

#ifndef _VC_DEBUG

#ifndef PC
#define LRC_LABLE_ADDR      0x9fc34000 //用RDATA_AL(JPEG)(8k)作临时缓存
#endif

#define LRC_CONTENT_VM      VM_LYRIC_INFO//0x0000c000 //8k (case\inc, VM_LYRIC_INFO)
#define LRC_INFO_VM         (LRC_CONTENT_VM+0x2000)   //0.5k
#define LRC_LABLE_VM        (LRC_INFO_VM+0x200)       //7.5k
#define LRC_CONTENT_MAX      LRC_INFO_VM		//vm max address
#endif

#define LRC_TIME_MAX        0xff000000 //标签结束时间
typedef enum
{
    READ_ZERO = 0, //读到字符结束符\0
    READ_NEXT, //调下个接口
    READ_LAB_START, // 读到歌词的不止一个标签时间起始标记
    READ_DATA_END, // 读完当前歌词数据结束
    READ_INVALID_LAB, //读标签时间异常
    READ_FILE_END, //读到文件尾
    READ_FILE_ERROR
//读文件出错了
} ret_read_e;//

typedef enum
{
    LRC_FIND_NEXT, LRC_FIND_CUR, LRC_FIND_PREV
} lrc_find_e;

typedef struct
{
    uint8 p_ms; //  0.01s
    uint8 sec; //  秒
    uint8 min; // 分
    uint8 reserve;//
    uint16 offset_byte; //歌词在歌词库中的字节偏移
    uint16 lrc_len; //歌词的长度
} lrc_lable_t;

//extern global
//load lrc
extern handle lrc_handle;
extern uint16 time_lab_count; //时间标签总数

//get lrc
extern uint16 lrc_cur_num; //歌词的页号
extern uint32 lrc_prev_time; //前一个时间
extern uint32 lrc_cur_time; //当前页歌词的时间
extern uint32 lrc_next_time; //下一页歌词的时间
extern uint16 lrc_lab_count; //时间标签总数

#ifdef _VC_DEBUG
extern uint8 LRC_CONTENT_ADDR[0x800];
extern uint8 LRC_LABLE_ADDR[0x800];
extern uint8 LRC_CONTENT_VM[0x2000];
extern uint8 LRC_LABLE_VM[0x2000];
#else

#ifdef PC
extern uint8 LRC_CONTENT_ADDR[0x800];
extern uint8 LRC_LABLE_ADDR[0x800];
//extern uint8 LRC_CONTENT_VM[0x2000];
//extern uint8 LRC_LABLE_VM[0x2000];
#endif

#endif

#endif

