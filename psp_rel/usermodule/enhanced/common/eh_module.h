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
 * \file     eh_module.h
 * \brief    数据结构和常量定义
 * \author   liminxian
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#ifndef   _eh_module_h
#define   _eh_module_h

#include "enhanced.h"

//引用case目录的头文件
#include "../../../../case/inc/vm_def.h"
#include "../../../../case/inc/case_type.h"

//系统默认创建文件使用长名

//#define USE_83NAME_CREAT  //创建文件使用短名定义

//记录LIB的offset位置
#define NOTE_LIB_OFFSET

#define FILE_SIZE_MIN    512
#define SECTOR_SIZE      EH_SECTOR_SIZE//512
#define KEY_BUF_LEN        64

#define PLIST_DIR_MAX    4  //浏览层级的深度（0~3）
#define PLIST_INVALID_INDEX  0xffff
//#define USERPL_MAX        3

//note:与playlist模块的plist.h定义保持一致
#define  VM_USERPL_INFO    VM_FAVORITE_INFO//0x0E0000
#define  USERPL_VM_SIZE    0x8000   //32K

//mainmenu显示总数VM地址
#define  VM_MAINMENU_CNT_INFO      VM_FILE_NUM

//const char ext_vec_rom[32][4] =
//{
//	"MP3", "WMA", "WAV", "MP1", "MP2", "OGG", "APE", "FLAC", "AAC", "ASF", "AA", "AAX",
//	"AA", "AAX",
//    "AMV", "AVI",
//    "JPG", "JPEG", "BMP", "GIF",
//    "TXT", "LRC",
//    "*  ",
//};


//#define FS_EXT_DIR     0
#define EH_GET_EXT_FILE EXT_NAME_ALL_FILE    //前两字节为“*f”，表示dir所有文件
#define EH_GET_EXT_DIR  EXT_NAME_ALL_DIR     //前两字节为“*d”，表示dir所有目录
#define EH_GET_EXT_ALL  EXT_NAME_ALL         //前两字节为“**”，表示dir所有目录和文件
#define EH_DIR_FLAG   ('d'*0x100+'*')
#define EH_FILE_FLAG  ('f'*0x100+'*')

typedef enum
{
    CHAR_TYPE_NUM = 0, //数字（0-9）
    CHAR_TYPE_UPER, //大写字母（A-Z）
    CHAR_TYPE_DNER, //小写字母（a-z）
    CHAR_TYPE_SPACE, //空格 (" ")
    CHAR_TYPE_OTHER
//其他值
} char_type_e; //

typedef enum
{
    TYPE_TITLE = 0, TYPE_ABLUM, TYPE_ARTIST, TYPE_GENRE, TYPE_AUTHOR, TYPE_BOOK
} id3info_type_e;

//typedef struct
//{
//    char file_name[8];     //收藏夹的名称 例如：USERPL1
//    uint16 file_num;       //收藏夹内的文件个数
//    uint16 first_offset;   //第一个文件序号(0,1)
//    uint16 last_offset;    //最后一个文件序号(0,1)
//    uint16 reserve;        //
//}userpl_head_t;
//
//
//typedef struct
//{
//    uint8   flag;                //  0-未使用，1- 存在文件 2: 文件删除，空间空间回收
//    uint8   reserve[3];           //  对齐
//    uint16  prev_offset;          // 前一个文件存放的偏移量
//    uint16  next_offset;          // 下一个文件存放的偏移量
//    flist_location_t  location;  //
//}userpl_file_t;


//ID3解析数据结构

typedef struct
{
    uint8 find_flag[6]; //查找ID3的标记
    uint16 buffer_offset; //BUF的offset位置
    uint32 item_offset; //在当前item的offset
    handle file_hdl; //文件句柄
    uint8 *key_buf; //数据解析buf
    uint8 *data_buf; //读取文件数据的buf
} id3_parse_t;

//lyric
//typedef struct
//{
//    uint8  p1s;           //  0.1s
//    uint8  sec;           //  秒
//    uint8  min;           // 分
//    uint8  reserve;    //
//    uint16 offset_byte;   //歌词在歌词库中的字节偏移
//    uint16 lrc_len;          //歌词的长度
//} lrc_lable_t;


//playlist
typedef struct
{
    uint16 son_tree_num[3]; //级子表的tree 的个数
    uint16 son_tree_offset[3]; // 3级子表的tree存放位置的偏移，以扇区为单位
    uint16 file_index_offset; //排序后的file_index偏移位置，以字节为单位
    uint16 sort_index_offset; // map的字节偏移，file_index为索引，存排序位号sort_index
    uint16 index_parent_offset; // parent的字偏移，file_index为索引，存parent_offset
    uint16 reserve;
    //uint8 layer;
    //uint8 reserve;
} plist_item_info_t;

typedef struct
{
    uint8 savelen; // FrameID保存的最长长度
    uint8* saveaddr; // FrameID保存地址
} id3_save_t;

//同步main_menu.h定义
#define    MAX_APP_NUM    11
typedef struct
{
    uint16 active_item;//活动AP菜单项编号
    uint16 total_item; //AP菜单总数
    uint16 ap_id[MAX_APP_NUM]; //支持应用编号的数组
} cfg_mainmenu_var_t;

typedef struct
{
    uint16 magic; //VM 变量魔数
    cfg_mainmenu_var_t cfg_var;
    //uint16 total_music;//音乐文件总数
    //uint16 total_video;//视频文件总数
    //uint16 total_picture;//图片文件总数
    //uint16 total_ebook;//电子书文件总数
    //uint16 total_station;//FM预设电台总数
    uint16 file_num[MAX_APP_NUM];//每个应用的文件总数
} g_mainmenu_var_t;

#endif

