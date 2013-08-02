/******************************************************************************
 *                               US212A
 *                            Module: SHOW_LYRIC
 *                 Copyright(c) 2003-2011 Actions Semiconductor
 *                            All Rights Reserved.
 * History:
 *      <author>         <time>             <version>             <desc>
 *      wuyufan      2011-9-25 19:46:36          1.0              build this file
 ******************************************************************************/
/*!
 * \file     show_lyric.h
 * \brief    显示书签内容模块函数
 * \author   wuyufan
 * \version  1.0
 * \date     2011-9-25
 *******************************************************************************/
#include "psp_includes.h"
#include "case_include.h"

/*--------------------------------------------------------*/

//以下为歌词显示的宏配置，通过配置不同的宏，实现是否断词，
//是否显示多屏的控制

/****应用需要配置的内容************************/

#define LYRIC_MAX_SCREEN    4       //当前歌词允许最多几屏显示，1为不分屏，截断处理
#define LYRIC_LINE_COUNT_ONE_PAGE 4 //一页歌词最多允许几行显示
#define LYRIC_MAX_WIDTH_ONE_LINE  92//一行允许显示最多像素点 

//这两个用于断词和分屏控制
#define NEXT_LINE    0x0a     //换行
#define NEXT_SCREEN  0x00     //换屏
#define LRC_BUF_LEN 200

/*---------------------------------------------------------*/

//过滤空行属性
#define LRC_FILTER_BLANK_OPTION     (0x01 << 0)
#define LRC_FILTER_BLANK_DISABLE    (0x00 << 0)
#define LRC_FILTER_BLANK_ENABLE     (0x01 << 0)
//断词属性
#define LRC_DIVIDE_WORD_OPTION      (0x01 << 1)
#define LRC_DIVIDE_WORD_DISABLE     (0x00 << 1)
#define LRC_DIVIDE_WORD_ENABLE      (0x01 << 1)
//换屏属性
#define LRC_DIVIDE_SCREEN_OPTION    (0x01 << 2)
#define LRC_DIVIDE_SCREEN_DISABLE   (0x00 << 2)
#define LRC_DIVIDE_SCREEN_ENABLE    (0x01 << 2)

#ifndef PC
#define _LCDBUF_DATA_ATTR_    __attribute__((section(".lcdbuffer")))
#else
#define _LCDBUF_DATA_ATTR_
#endif

typedef struct
{
    /*! 显示一页行数 */
    uint8 line_count_one_page;
    /*! 显示一行像素点宽度 */
    uint8 max_width_one_line;
    /*! 显示模式，包括过滤空行，分词显示等 */
    uint8 mode;
    /*! 语言类型 */
    int8 language;
} lyric_show_param_t;

typedef struct
{
    /*! 输入buffer地址 */
    uint8 *input_buffer;
    /*! 输入buffer已解析长度 */
    uint16 input_length;
    /*! 输入buffer剩余长度 */
    uint16 input_remain;
    /*! 输出buffer地址 */
    uint8 *output_buffer;
    /*! 输出buffer已解析长度 */
    uint16 output_length;
    /*! 输出分屏地址 */
    uint16 output_start[LYRIC_MAX_SCREEN];
    /*! 输出buffer分屏时间 */
    uint32 output_time[LYRIC_MAX_SCREEN];
    /*! 文本显示参数结构体 */
    lyric_show_param_t param;
} lyric_decode_t;

/* 读取一个时间标签歌词内容缓冲区 */
extern uint8 lrc_buf[LRC_BUF_LEN] _LCDBUF_DATA_ATTR_;
/* 一个时间标签歌词显示内容缓冲区 */
extern uint8 display_buf[LRC_BUF_LEN] _LCDBUF_DATA_ATTR_;
/* 歌词显示解码控制变量 */
extern lyric_decode_t g_lyric_decode _LCDBUF_DATA_ATTR_;



extern void lyric_param_init(void);

extern void parse_lyric(lyric_decode_t *lyric_decode);

extern void display_lrc_str(uint8 *cur_lrc_str);

/*! \endcond */

