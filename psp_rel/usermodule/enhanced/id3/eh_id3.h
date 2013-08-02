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
 * \file     eh_id3.h
 * \brief    数据结构和常量定义
 * \author   liminxian
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#ifndef _eh_id3_h

#define _eh_id3_h

#include "eh_module.h"
#include "eh_rdata_ex.h"

#define  FILTRATE_SPACE_SW   1

#define  UNICODE_FLAG  0xfeff
#define  UTF8_FLAG_0   0xef
#define  UTF8_FLAG_1   0xbb
#define  UTF8_FLAG_2   0xbf

extern handle id3_handle;
/*
 extern uint8 id3_temp_buf[SECTOR_SIZE];
 extern uint8 key_buf[KEY_BUF_LEN];
 extern uint8 check_count;
 extern uint8 check_flag[8];     //查找ID3要素标记
 extern uint16 id3_pos_buffer;     //当前TempBuf未处理字符的索引
 extern uint32 id3_pos_file;       //当前文件的精确指针

 extern id3_save_t *id3_save_p;     //FrameID存储位置
 extern id3_info_t *id3_info_p;     //ap层提供的存储结构
 */

bool get_init(void);
bool read_buf_data(uint8 *Tbuff, uint32 size);
char reset_file_pos(uint32 size, uint32 last_pos_file);
//int8 check_tag_list(uint8 *check_str,uint8 *tab_str,uint8 size_str,uint8 tab_count);
//int8 check_pic_flag(uint8 *buf);
uint8 str_to_capital(char *str, char end_char);
//uint32 get_frame_size(uint8 *data,uint8 byte_cn,uint8 bit7_flag);
extern uint32 id3_read_utf8_deal(id3_save_t* dest_p, uint32 read_size,uint8 *src_buf) __FAR__;

bool get_id3_aa(void);
bool get_id3_aac(void);
bool get_id3_aax(void);
bool get_id3_ape(void);
bool get_id3_flac(void);
bool get_id3_mp3(void);
bool get_id3_ogg(void);
bool get_id3_wma(void);

#endif//#ifndef _eh_id3_h
