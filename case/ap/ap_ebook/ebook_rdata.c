/********************************************************************************
 *                              USDK
 *                             Module: ebook
 *                 Copyright(c) 2003-2012 Actions Semiconductor,
 *                            All Rights Reserved.
 * File : ebook_rdata.c
 * History:
 *      <author>    <time>           <version >             <desc>
 *      stevenluo		2011-11-11         v1.0              create this file
 ********************************************************************************/
/* include file of this application, 本应用的头文件 */
#include "ebook.h"

/****************************************************************************/

/* 常驻数据段 变量定义 */

//页数显示时当前的数字倍数，1-没有倍数;10-10倍;100-100倍
uint16 page_multiple_sel;
//正在阅读文件的书签总数
uint16 bookmark_total;

//系统公共变量
comval_t g_comval;
//应用存储变量结构体
g_ebook_var_t g_ebook_vars;
//当前阅读文本所处的页数
uint32 curpagenum;
//正在阅读文件的总页数
uint32 page_totalnum;
//正在应用文件系统的索引号
int32 g_ebook_mount_id;
//打开的书签文件句柄
uint32 bmk_modify_handle;
//用于缓存当前正在阅读的页面在对应文件中的偏移
uint32 file_offset;
//用于记录当前页数在对应文件中的偏移
uint32 decode_offset;
//用于计算页数时，缓存回写在书签文件中的最后一个页数
uint32 bmk_last_pagenum;
//提示当前buf中的数据所处的扇区,-1表示当前buf中的数据不可用。
int32 cur_BufSector;
//当前正在阅读的页面在对应的文件中所处的扇区数
uint32 decode_sector;
//当前文本文件中目录信息中的目录偏移
uint32 cur_file_diroffset;
//在计算页数时用于回写到书签文件的计数
uint16 temp_count;
//文本阅读数据存储和解码缓冲区结构体，主要用于阅读时使用
//view_decode_t view_decode;
vram_bmk_info_t vram_bmk_info;
decode_bmk_info_t decode_bmk_info;
view_file_t view_file;

bool numboxflag ;

//文本阅读数据存储和解码缓冲区结构体，主要计算页数使用
view_file_t page_count_file;

app_timer_t ebook_app_timer_vector[COMMON_TIMER_COUNT + APP_TIMER_COUNT];

//书签信息缓存空间
uint8 bmk_bufFile[FILE_SECTOR];

//书签文件名缓存空间
uint8 BOOKMARK_filename[BMK_NAME_LENGTH];
//界面是否刷新标志，0-不需要刷新,1-需要刷新
uint8 need_draw;
//页数信息是否刷新标志，0-不需要刷新,1-需要刷新
uint8 draw_page_num;

#if SHOW_FIFLNAME  
    //显示文件名                                     
    uint8 BOOK_filename[MAX_FILENAME_LENGTH];
#endif

//自动播放时间统计
uint8 auto_play_count;
//是否继续计算页数标记。0-继续计算，1-计算完毕
uint8 page_count_flag;
//刷新电子书自动播放图标标记
uint8 draw_auto_flag;
//书签有删除或增加标记
uint8 bmk_modify_flag;
//是否为阅读菜单标记
uint8 is_reading_menu;
//当前磁盘状态；1-flash没有文件；2-card无文件；3-flash和card都无文件
uint8 device_status;
//后台音乐播放的盘符
uint8 music_play_device;
//是否支持卡盘
uint8 support_card_flag;
//是否需要创建播放列表标志0-不需要;1-需要
uint8 need_creatlist;

scene_enter_mode_e enter_mode;
engine_type_e engine_type;

