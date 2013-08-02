/*******************************************************************************
 *                              US212A
 *                            Module: PLAYLSIT
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       liminxian  2011-9-15 15:37    1.0             build this file
 *******************************************************************************/
/*!
 * \file     *.c
 * \brief    这里填写文件的概述
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *               这里描述调用到外面的模块
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#ifndef  _rdata_ex_h
#define  _rdata_ex_h

#include "psp_includes.h"
#include "case_include.h"

extern app_timer_t playlist_app_timer_vector[APP_TIMER_TOTAL];

extern uint8 language_id; //语言id
extern uint16 ap_pos_file_offset; //文件扇区的绝对偏移
extern handle ap_obj_handle;
extern int32 ap_vfs_mount;
extern uint8 utf8_flag;
extern comval_t g_comval;

//*lib file's info

//head

extern uint16 ap_plist_file_total;
extern uint16 ap_plist_file_offset;

//items 108
extern uint16 ap_file_index_offset[6];
extern uint16 ap_sort_index_offset[6];
extern uint16 ap_index_parent_offset[6];
extern uint16 ap_son_tree_num[6][3];
extern uint16 ap_son_tree_offset[6][3];

// sort file
extern uint8 *track_buf; //排序时存放TRACK_NUM的RAM地址
extern uint16 sort_file_total; //排序的个数
extern uint8 *sort_depend_buf; //compare 算法对象比较时依赖的排序依据


//vram写指针
extern uint8 *title_vram_pt;
extern uint8 *album_vram_pt;
extern uint8 *artist_vram_pt;
extern uint8 *genre_vram_pt;
extern uint8 *audible_vram_pt;
extern uint8 *track_vram_pt;

//ram 写指针
extern uint8 *title_data_pt;
extern uint8 *album_data_pt;
extern uint8 *artist_data_pt;
extern uint8 *genre_data_pt;
extern uint8 *audible_data_pt;
extern uint8 *fileinfo_data_pt;
extern uint8 *track_data_pt;

//ID3信息相同时，编号一致
//file_index为索引，存的是归类编号
extern uint16 *tidy_buf_a;//存ALBUM
extern uint16 *tidy_buf_b;//存ARTIST

//存放排序后的file_index，sort_index为索引，存的是file_index。
extern uint16 *file_index_buf;//

//file_index为索引，存排序位号sort_index。
extern uint16 *sort_index_buf;

//记录当前列表类的相同的子个数，最小值为1，如（1，3，2，...）
extern uint16 *count_buf_a;//
extern uint16 *count_buf_b;//
extern uint16 *count_buf_c;//

//file_index为索引，存parent_offset。
extern uint16 *index_parent_buf;
//考虑与temp_ex_index合在一起存入file中

//extern cmp_obj_f compare_obj;

extern id3_info_t id3_file_info;
extern plist_f_info_t *music_f_p;
extern plist_f_info_t *audible_f_p;
extern plist_f_info_t *other_f_p;

extern uint8 temp_plist_buf[SECTOR_SIZE];
extern uint8 file_buffer[256];
extern uint8 plist_lib_offset[PL_LIB_MAX + USERPL_MAX][8];

extern uint8 ui_precent_count; //百分比计算
extern uint8 ui_icon_count; //显示图标
extern uint8 ui_cur_strID; //显示id号
//extern int8  app_timer_id;      //软定时器id
extern uint8 cpu_release_count; //cpu释放计时器

extern uint8 lib_num; //当前建表类型
extern uint8 drv_sn; //当前存储介质

extern uint8 fsel_scan_flag; //扫描文件标记

#define  SCAN_FILTRATE_BIT    0x01

/////////////m3u格式文件解析，定义的全局变量
extern uint16 m3u_cursec_num; //当前读写文件的扇区偏移
extern uint16 m3u_curpos_offset; //当前读写文件的字节偏移
extern uint32 m3u_handle; //m3u格式文件打开句柄

extern uint8 *m3u_get_name_buf; //dir时获取文件名临时buf
extern uint8 *m3u_obj_name_buf; //m3u中的文件名，unicode格式
extern uint8 *m3u_ext_name; //m3u中文件名的扩展名，unicode格式
extern m3ulist_f_info_t *m3u_dat_file_pt; //文件信息指针
extern file_pos_t *filepos_p;            //文件dir 信息指针

extern uint16 ebook_total;
extern uint16 temp_count;

#endif //#ifndef  _rdata_ex_h
