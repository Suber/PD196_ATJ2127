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
#include "plist.h"

#ifndef WIN32
OS_STK *ptos = (OS_STK *) AP_PLAYLIST_STK_POS;
#endif

INT8U prio = AP_PLAYLIST_PRIO;

uint8 language_id; //语言id
uint16 ap_pos_file_offset; //文件扇区的绝对偏移
handle ap_obj_handle;
int32 ap_vfs_mount;
comval_t g_comval;

uint8 utf8_flag;
//*lib file's info

//head

uint16 ap_plist_file_total;
uint16 ap_plist_file_offset;

//items 108
uint16 ap_file_index_offset[6];
uint16 ap_sort_index_offset[6];
uint16 ap_index_parent_offset[6];
uint16 ap_son_tree_num[6][3];
uint16 ap_son_tree_offset[6][3];

// sort file
uint8 *track_buf; //排序时存放TRACK_NUM的RAM地址,一个TRACK_NUM占1bytes
uint16 sort_file_total; //排序的个数
uint8 *sort_depend_buf; //compare 算法对象比较时依赖的排序依据


//vram写指针
uint8 *title_vram_pt;
uint8 *album_vram_pt;
uint8 *artist_vram_pt;
uint8 *genre_vram_pt;
uint8 *audible_vram_pt;
uint8 *track_vram_pt;

//ram 写指针
uint8 *title_data_pt;
uint8 *album_data_pt;
uint8 *artist_data_pt;
uint8 *genre_data_pt;
uint8 *audible_data_pt;
uint8 *fileinfo_data_pt;
uint8 *track_data_pt;

//ID3信息相同时，编号一致
//file_index为索引，存的是归类编号
uint16 *tidy_buf_a;//存ALBUM
uint16 *tidy_buf_b;//存ARTIST

//存放排序后的file_index，sort_index为索引，存的是file_index。
uint16 *file_index_buf;//

//file_index为索引，存排序位号sort_index。
uint16 *sort_index_buf;

//记录当前列表类的相同的子个数，最小值为1，如（1，3，2，...）
uint16 *count_buf_a;//
uint16 *count_buf_b;//
uint16 *count_buf_c;//

//file_index为索引，存parent_offset；使用前赋值，空间复用
uint16 *index_parent_buf;
//考虑与temp_ex_index合在一起存入file中

cmp_obj_f compare_obj;

id3_info_t id3_file_info;
plist_f_info_t *music_f_p;
plist_f_info_t *audible_f_p;
plist_f_info_t *other_f_p;

uint8 temp_plist_buf[SECTOR_SIZE];
uint8 file_buffer[256];

uint8 plist_lib_offset[PL_LIB_MAX + USERPL_MAX][8];//存放offset偏移

#ifdef  PLIST_SYS_DEF_SPACE
uint8 sys_vram_space[0x20000];//256k
uint8 sys_sdram_space[0x20000];//128k
#endif

app_timer_t playlist_app_timer_vector[APP_TIMER_TOTAL];

uint8 ui_precent_count; //百分比计算
uint8 ui_icon_count; //显示图标
uint8 ui_cur_strID; //显示id号

//int8  app_timer_id;      //软定时器id
uint8 cpu_release_count; //cpu释放计时器

uint8 lib_num; //当前建表类型
uint8 drv_sn; //当前存储介质

uint8 fsel_scan_flag; //扫描文件标记


/////////////m3u格式文件解析，定义的全局变量

uint16 m3u_cursec_num;    //当前读写文件的扇区偏移
uint16 m3u_curpos_offset; //当前读写文件的字节偏移
uint32 m3u_handle;        //m3u格式文件打开句柄

uint8 *m3u_get_name_buf;  //dir时获取文件名临时buf
uint8 *m3u_obj_name_buf; //m3u中的文件名，unicode格式
uint8 *m3u_ext_name;     //m3u中文件名的扩展名，unicode格式
m3ulist_f_info_t *m3u_dat_file_pt; //文件信息指针
file_pos_t *filepos_p;            //文件dir 信息指针


uint16 ebook_total;
uint16 temp_count;

