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
 * \file     ENHANCED.H
 * \brief    数据结构和常量定义
 * \author   liminxian
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#ifndef  _enhanced_h
#define  _enhanced_h

#include <typeext.h>
#include <api.h>
#include <driver_manager.h>
#include <kernel_interface.h>
#include <vfs_interface.h>
#include <libc_interface.h>
//#include <>
//#include <>
//#include <>


//c宏定义
/*!
 * \brief
 *      Device driver's system separating layer headfile
 * \li  v: pointer of type atomic_t
 */
//#define atomic_read(v)	((v)->counter)

#define  ID3_ITEM_SIZE    28    //ID3信息长度
#define  DIRLAYER_MAX     9     //目录深度,root + 8级子目录
#define  EH_SECTOR_SIZE   512   //读写扇区大小
//end

#define  USERPL_MAX        3    //收藏夹的个数
#define  USERPL_FILE_MAX   100  //收藏夹支持文件总数
#define  USERPL_FILE_SIZE  256  //收藏夹每条信息的大小
#define  USERPL_PRE_SEC    (EH_SECTOR_SIZE/USERPL_FILE_SIZE) //每扇区的信息条数
#define  USERPL_HEAD_SEC   1   //收藏夹头扇区
#define  USERPL_TITLE_MAX  (ID3_ITEM_SIZE * 2)  //收藏夹显示标题
//
#define  PLIST_HEAD_SEC    1    //列表头大小
#define  PLIST_FILE_SIZE   256  //列表文件信息大小
#define  FILENAME_LENGTH   32   //ebook file‘s namebuf length; uint:UNICODE

// M3U 定义,M3U.LIB包含 LIST 和 DAT 两部分
#define  M3U_LIST_MAX          50    //m3u列表文件总数, (VALUE < ID3_ITEM_SIZE*3;see m3ulist_f_info_t's m3u_index)
#define  M3U_LIST_SIZE         ((M3U_LIST_MAX/2 + M3U_LIST_MAX/4) * EH_SECTOR_SIZE)//list部分大小,unit:byte,0x4a00
#define  M3U_DAT_HEADOFFSET    0xa0  //unit:byte，指在PLIST_HEAD_SEC中的偏移

//M3U DAT
#define  M3U_DAT_FILE_MAX         4000             //DAT 包含文件的总数
#define  M3U_DAT_FILESIZE     	  PLIST_FILE_SIZE  //M3U列表中文件信息大小
#define  M3U_DAT_INDEXSIZE    	  0x2000           //文件FILE_INDEX的大小，unit:byte
#define  M3U_DAT_POSINDEX_SIZE 	 (8 * 0x1000)
#define  M3U_DAT_SIZE         	 (M3U_DAT_INDEXSIZE * M3U_LIST_MAX + M3U_DAT_FILE_MAX * M3U_DAT_FILESIZE + M3U_DAT_POSINDEX_SIZE) //DAT 部分大小,//0x166000
  
#define  M3U_DAT_INDEXOFFSET  	 (M3U_LIST_SIZE ) //文件FILE_INDEX的偏移起始,unit:byte,0x4a00
#define  M3U_DAT_FILEOFFSET   	 (M3U_DAT_INDEXOFFSET + (M3U_LIST_MAX * M3U_DAT_INDEXSIZE))   //文件信息偏移,unit:byte,0x68a00
#define  M3U_DAT_POS_INDEXOFFSET (M3U_DAT_FILEOFFSET + M3U_DAT_FILE_MAX * M3U_DAT_FILESIZE) //文件位置索引偏移,unit:byte,0x162a00

//c类型定义
/*!
 *  \brief
 *      ssize_t - unsigned int
 */
//typedef unsigned int ssize_t;


//c枚举变量
/*!
 *  \brief
 *  enumValue test
 *  \li TEST_1 : enumValue_1
 *  \li TEST_2 : enumValue_2
 */

//文件选择器属性
typedef enum
{
    FSEL_MODULE_DIR = 0, 
    FSEL_MODULE_PLIST, 
    FSEL_MODULE_FAVOR,
//	FSEL_MODULE_OTHER
} fsel_module_e;

//文件选择器选择类型
typedef enum
{
    FSEL_TYPE_COMMONDIR = 0, //目录播放
    FSEL_TYPE_DISKSEQUNCE,   //全盘序号播放
    FSEL_TYPE_SDFILE,        //sd区文件    
    FSEL_TYPE_PLAYLIST,      //播放列表
    FSEL_TYPE_LISTAUDIBLE,   //AUDIBLE播放列表
    FSEL_TYPE_LISTVIDEO,     //VIDEO列表
    FSEL_TYPE_LISTPICTURE,   //PICTURE列表
    FSEL_TYPE_LISTEBOOK,     //EBOOK列表
    FSEL_TYPE_M3ULIST,       //m3u列表
    FSEL_TYPE_LISTFAVOR1,    //收藏夹1
    FSEL_TYPE_LISTFAVOR2,    //收藏夹2
    FSEL_TYPE_LISTFAVOR3     //收藏夹3
} fsel_type_e;

//播放模式
typedef enum
{
    FSEL_MODE_NORMAL = 0,   //普通播放
    FSEL_MODE_LOOPONE,      //循环放当前首
    FSEL_MODE_LOOPALL,      //循环所有
    FSEL_MODE_INTRO,        //浏览播放
    FSEL_MODE_LOOPONEDIR,   //循环放当前目录
    FSEL_MODE_DIRNOR,       //顺序播放目录
    FSEL_MODE_BROWSER       //文件浏览器
} fsel_mode_e;

//browser 显示
typedef enum
{
    FSEL_BROW_DIR = 0,  //只显示目录
    FSEL_BROW_FILE,     //只显示文件
    FSEL_BROW_ALL       //显示文件和目录
} fsel_brow_e;

//typedef enum
//{
//    FSEL_DEL_FILE = 0, //删除文件
//    FSEL_DEL_DIRFILE,  //删除目录
//} fsel_del_e;

//ID3获取支持类型
typedef enum
{
    ID3_TYPE_AA = 0,
    ID3_TYPE_AAC,
    ID3_TYPE_AAX,
    ID3_TYPE_APE,
    ID3_TYPE_FLAC,
    ID3_TYPE_MP3,
    ID3_TYPE_OGG,
    ID3_TYPE_WMA,
    ID3_TYPE_END
} id3_type_e;

//播放列表子表类型
typedef enum
{
    PLIST_TITLE = 0x00,  //
    PLIST_ALBUM = 0x01,  //
    PLIST_ARTIST = 0x02, //
    PLIST_GENRE = 0x03,  //
    PLIST_BOOK = 0x10,
    PLIST_AUTHOR = 0x12,
    PLIST_M3U   = 0x50
} plist_type_e;

/*
 typedef enum
 {
 TYPE_TITLE=0,
 TYPE_ABLUM,
 TYPE_ARTIST,
 TYPE_GENRE,
 TYPE_AUTHOR,
 TYPE_BOOK
 }id3info_type_e;
 */

//playlist 表的个数
typedef enum
{
    PL_LIB_MUSIC = 0, 
    PL_LIB_AUDIBLE, 
    PL_LIB_VEDIO, 
    PL_LIB_PICTURE, 
    PL_LIB_EBOOK, 
    PL_LIB_PIC2, 
    PL_LIB_MAX
} plist_lib_e;

//browser
typedef enum
{
    CUR_DIR = 0,        //重新解释当前目录只需要获取dir_browse_t
    SON_DIR,            //进入子目录
    PARENT_DIR,         //返回父目录
    ROOT_DIR            //进入根目录
} brow_dir_e;

//删除类型
typedef enum
{
    BROW_DEL_FILE = 0,      //删除文件
    BROW_DEL_DIR            //删除空目录
} brow_del_e;

//获取当前歌词时间标签
typedef enum
{
    LYRIC_CUR_TIME = 0,     //当前时间标签
    LYRIC_PREV_TIME,        //上一个时间标签
    LYRIC_NEXT_TIME         //下一个时间标签
} lrc_time_e;

//接口调试错误类型
typedef enum
{
    FSEL_NO_ERR = 0,        //没有报错
    FSEL_ERR_OUTOF,         //选择溢出
    FSEL_ERR_EMPTY,         //目录空报错
    FSEL_ERR_SETDIR,        //操作目录路径出错
    FSEL_ERR_SETFILE,       //操作文件信息出错
    FSEL_ERR_EXCEPTION,     //异常有误
    FSEL_ERR_FAIL,          //接口操作失败
    FSEL_ERR_DISK,          //盘符出错
    FSEL_ERR_EXIST,         //文件已存在
    FSEL_ERR_NO_EXIST,      //文件不存在
    FSEL_ERR_USING,         //文件在使用中
    FSEL_ERR_STG,           //安装驱动失败
    FSEL_ERR_FS,            //文件系统接口报错
    FSEL_ERR_LIST_NOEXIST,  //收藏夹或播放列表不存在
    FSEL_ERR_SHUFFLE,       //shuffle机制出错
    FSEL_ERR_INPUT          //输入参数有误
} fsel_error_e;

//c结构体定义
/*!
 * \brief
 *      Device driver's system separating layer headfile.
 */
//struct list_head
//{
//       /*! next*/
//       struct list_head *next;
//       /*! prev*/
//       struct list_head *prev;
//};

//ENHANCED 初始化参数
typedef struct
{
    uint8 fsel_type;            //选择类型      fsel_type_e
    uint8 fsel_mode;            //播放模式      fsel_mode_e
    uint8 brow_type;            //browser的模式 fsel_brow_e
    uint8 disk;                 //DISK_C;DISK_H;DISK_U
    uint32 file_type_bit;       //文件后缀名bitmap
} fsel_param_t;

//目录下的文件信息
typedef struct
{
    uint8 disk;                 //DISK_C;DISK_H;DISK_U
    uint8 dir_layer;            //当前目录层次，从0开始，0表示根目录
    uint16 resever;             //保留对齐
    uint8 filename[4];          //文件的后缀名，全填0代表文件信息为空
    uint32 cluster_no;          //文件的目录项所在的簇号
    uint32 dir_entry;           //文件的目录项在所在簇号内的偏移
    pdir_layer_t dir_layer_info;//目录层次信息
    uint8 res_3byte[3];         //保留对齐
    uint16 file_total;          //当前目录文件总数
    uint16 file_num;            //当前文件在目录中的序号
} file_location_t;

//播放列表下的文件信息
typedef struct
{
    uint8 disk;                 //DISK_C;DISK_H;DISK_U
    uint8 list_layer;           //列表层数
    uint16 list_type;           //子表类型 plist_type_e
    uint8 filename[4];          //文件的后缀名，全填0代表文件信息为空
    uint32 cluster_no;          //文件的目录项所在的簇号
    uint32 dir_entry;           //文件的目录项在所在簇号内的偏移
    pdir_layer_t dir_layer_info;//目录层次信息
    uint8 res_3byte[3];         //保留对齐
    uint16 file_total;          //当前文件总数 (在当前列表下文件层的总数)
    uint16 file_num;            //当前文件序号 (在当前列表下文件层的序号)
    uint16 file_index;          //当前文件索引号（在数据区的序号）
    uint16 reserved;            //保留对齐
    uint16 list_layer_no[4];    //记录文件所在的每层列表中的位置，（分别记录在每层排序后的位置）
    uint16 layer_list_offset;   //list偏移位置
    uint16 reserve;             //保留对齐

} plist_location_t;

//收藏夹下的文件信息
typedef struct
{
    uint8 disk;         //'DISK_C;DISK_H;DISK_U
    uint8 reserved;     //保留对齐
    uint16 file_index;  //no used
    uint8 filename[4];  //文件的后缀名，全填0代表文件信息为空
    uint32 cluster_no;  //文件的目录项所在的簇号
    uint32 dir_entry;   //文件的目录项在所在簇号内的偏移
    pdir_layer_t dir_layer_info;//目录层次信息
    uint8 res_3byte[3]; //保留对齐
    uint16 file_total;  //当前收藏夹文件总数
    uint16 file_num;    //当前文件在收藏夹中的序号
    uint8 title[USERPL_TITLE_MAX]; // 收藏夹显示的文件标题
} flist_location_t;

//获取ID3的信息结构
typedef struct
{
    uint8 *tit2_buffer;  //待存文件标题buffer
    uint8 *tpe1_buffer;  //待存作者buffer
    uint8 *talb_buffer;  //待存专辑buffer
    uint8 *genre_buffer; //待存流派buffer
    uint8 *drm_buffer;   //待存drm的buffer
    uint8 *track_buffer; //待存音轨buffer
    uint8 tit2_length;   //文件标题长度
    uint8 tpe1_length;   //作者长度
    uint8 talb_length;   //专辑长度
    uint8 genre_length;  //流派长度
    uint8 drm_length;    //drm长度
    uint8 track_length;  //音轨长度
    uint8 apic_type;     // 0-无图片，1-jpeg
    uint8 apic_flag;     //获取图片偏移标记
    uint8 ldb_flag;      //获取ldb歌词偏移标记
    uint8 reserve;       //保留对齐
    uint16 track_num;    //音轨号
    uint32 ldb_offset;   //ldb歌词在文件中的字节偏移
    uint32 apic_offset;  //apic图片在文件中的字节偏移
} id3_info_t;

//文件信息联合体
typedef union
{
    file_location_t  dirlocation;       //目录模式下的文件信息
    plist_location_t plist_location;    //播放列表模式下的文件信息
    flist_location_t flist_location;    //收藏夹文模式下的文件信息
} musfile_location_u;

//文件信息
typedef struct
{
    uint8 file_source;              //文件信息类型：目录、播放列表和收藏夹
    uint8 reserved[3];              //保留对齐
    musfile_location_u file_path;   //文件信息结构
} file_path_info_t;

//browser
typedef struct
{
    uint16 dir_total;   //当前目录的子文件夹总数
    uint16 file_total;  //当前目录过滤后的文件总数
    uint8 name_len;     //名字长度，单位（bytes）
    uint8 layer_no;     //当前目录所处层次
    uint8 reserve[2];   //保留对齐
    uint8 *name_buf;    //名字缓冲区
} dir_brow_t;

//browser
typedef struct
{
    uint8 option; //解释选项
    uint8 num;    //项数
    uint16 top;   //首项位置
} brow_decode_t;

// 文件记录结构
typedef struct
{
    uint8 ext[4];     //文件后缀名
    uint8 name_len;   //名字的长度，单位（bytes）
    uint8 reserve[3]; //4字节对齐
    uint8 *name_buf;  //名字缓冲区
} file_brow_t;

//专辑图片记录结构
typedef struct
{
    uint8 album_flag;  //是否有专辑图片标志，0 - 无 1 - 有
    uint8 name_len;    //名字的长度，单位（bytes）
    uint8 reserve[2];  //4字节对齐
    uint8 *name_buf;   //名字缓冲区
    uint32 cluster_no; // 图片文件目录项簇号
    uint32 dir_entry;  // 图片文件目录项偏移 
} album_brow_t;


//文件ID3信息结构
typedef struct
{
    char title_string[ID3_ITEM_SIZE * 2]; //标题&文件长名 （排序16bytes）
    char artist_string[ID3_ITEM_SIZE];    //艺术家名称    （排序8bytes）
    char album_string[ID3_ITEM_SIZE];     //专辑名称      （排序8bytes）
    char gener_string[ID3_ITEM_SIZE];     //流派名称      （排序8bytes）
    char track_string[8];                 //track num string
    uint8 filename[4];                    //文件的后缀名
    uint8 id3_tag_flag;                   //是否存在ID3信息的标志：1--有，0--无
    uint8 flag;                           //该文件是否存在 1-有，0-无，2-已删除
    uint16 track_num;                     //专辑曲目号
    uint32 cluster_no;                    //文件目录项所在簇号
    uint32 dir_entry;                     //文件的目录项在当前簇内的偏移
    uint16 prev_offset;                   //上一个存放位置的偏移
    uint16 next_offset;                   //下一个存放位置的偏移
    pdir_layer_t dir_layer_info;          //目录层次信息
} plist_f_info_t; // 256 bytes

//播放列表tree结构
typedef struct
{
    uint8 flag;             //list标志 :0-no used,1-exist,2-deleted
    uint8 type;             //当前列表显示ID3信息类
    uint16 son_num;         //下级子列表的个数
    uint16 file_total;      //列表下所有文件的个数
    uint16 file_index;      //起始文件的序号，可读出出sort_index值
    uint16 son_offset;      //子list的存放位置偏移个数
    uint16 parent_offset;   //父list的存放位置偏移个数
    uint16 prev_offset;     //上一个的偏移
    uint16 next_offset;     //下一个的偏移
} plist_tree_t;

//播放列表头信息
typedef struct
{
    char plist_name[8];         //LIB 识别标记
    uint16 file_total;          //文件的总数
    uint16 file_info_offset;    //文件信息存放的偏移，以扇区为单位
    uint16 reserve[2];          //保留对齐
} plist_head_t;                 //（16bytes）

typedef struct
{
    char item_name[8];          //item名称
    uint16 son_tree_num[3];     //子级表的tree 的个数
    uint16 son_tree_offset[3];  //3级子表的tree存放位置的偏移，以扇区为单位
    uint16 file_index_offset;   //排序后的file_index偏移位置，以字节为单位
    uint16 sort_index_offset;   //map的字节偏移，file_index为索引，存排序位号sort_index
    uint16 index_parent_offset; //parent的字偏移，file_index为索引，存parent_offset
    uint16 reserve;             //保留对齐
} plist_item_t; //（28bytes）


typedef struct
{
    uint16 num;     //表项选择位置 1~n
    uint16 total;   //表项总数
}plist_layer_t;


//收藏夹头信息
typedef struct
{
    char file_name[8];      //收藏夹的名称 例如：USERPL1
    uint16 file_num;        //收藏夹内的文件个数
    uint16 first_offset;    //第一个文件序号(0,1)
    uint16 last_offset;     //最后一个文件序号(0,1)
    uint16 cover_offset;    //填满后，将要被覆盖的文件序号(0,1,...)
} userpl_head_t;

//收藏夹存储的文件信息
typedef struct
{
    uint8 flag;                 //0-未使用，1- 存在文件 2: 文件删除，空间空间回收
    uint8 reserve[3];           //保留对齐
    uint16 prev_offset;         //前一个文件存放的偏移量
    uint16 next_offset;         //下一个文件存放的偏移量
    flist_location_t location;  //文件信息
} userpl_file_t;

//EBOOK信息结构
typedef struct
{
    char file_name[FILENAME_LENGTH * 2];    //标题&文件长名 （排序16bytes）
    char file_length[4];                    //艺术家名称    （排序8bytes）
    char filename_lenth;                    //专辑名称      （排序8bytes）
} plist_ebook_info_t;// 256 bytes


//m3u 列表文件信息结构
typedef struct
{
    char  title_string[ID3_ITEM_SIZE * 2];//标题&文件长名 （排序16bytes）
    uint8 m3u_index[ID3_ITEM_SIZE*3];	  //使用该文件的M3U 表index，0	xFF为空;删除时作逆向查找
    char  track_string[8];                //track num string
    uint8 filename[4];                    //文件的后缀名
    uint8 id3_tag_flag;                   //是否存在ID3信息的标志：1--有，0--无
    uint8 flag;                           //该文件是否存在 1-有，0-无，2-已删除
    uint16 track_num;                     //专辑曲目号
    uint32 cluster_no;                    //文件目录项所在簇号
    uint32 dir_entry;                     //文件的目录项在当前簇内的偏移
    uint16 prev_offset;                   //上一个存放位置的偏移
    uint16 next_offset;                   //下一个存放位置的偏移
    pdir_layer_t dir_layer_info;          //目录层次信息
} m3ulist_f_info_t; // 256 bytes,与plist_f_info_t 大小一致

//m3u 列表头信息
typedef struct
{
    uint16 dat_file_total;        //DAT中文件的总数    
    uint16 list_file_num[M3U_LIST_MAX];    //对应每个M3U表中文件的个数
    uint16 audible_file_num[M3U_LIST_MAX]; //对应每个M3U表中audible文件的个数
} m3udat_head_t;  //只能增加成员，不能修改成员位置


/*******
 文件夹遍历枚举和结构定义
 ********/

//文件夹遍历枚举类型
typedef enum
{
    /*! 删除文件遍历 */
    SCANFILE_TYPE_DELETE = 0,
    /*! 数据交互（复制）遍历 */
    SCANFILE_TYPE_DXCH = 1,
} scanfile_type_e;

///浏览文件夹返回结果枚举类型
typedef enum
{
    /*! 返回根文件夹 */
    SCANFILE_RESULT_SELF = 0,
    /*! 返回匹配文件 */
    SCANFILE_RESULT_FILE = 1,
    /*! 返回子文件夹 */
    SCANFILE_RESULT_SONDIR = 2,
    /*! 返回父文件夹，不包括根文件夹 */
    SCANFILE_RESULT_PARENTDIR = 3,
    /*! 参数或接口出错 */
    SCANFILE_RESULT_ERR = 4,
} scanfile_result_e;

//bs 模块浏览路径管理结构体
typedef struct
{
    /*! 8级路径记录，只需从要浏览的文件夹层级开始记起 */
    uint8 bs_dir_no[DIRLAYER_MAX];
    /*! 当前浏览层次 */
    uint8 bs_layer;
    /*! 所浏览子文件夹所在层次，该值在浏览过程中不变 */
    uint8 root_layer;
    /*! 当前层次文件序号 */
    uint16 list_no;
} bs_dir_location_t;

/********************************************************************************
 *               ENHANCED 模块对外提供的接口列表
 *********************************************************************************/

/**********************************
 (1)模块系统接口
 目录：\COMMON
 **********************************/
extern fsel_error_e fsel_get_error(void) __FAR__;
extern int32 fsel_init(fsel_param_t *init_param,uint8 drv_mode) __FAR__;
extern bool fsel_exit(void) __FAR__;

/**********************************
 (2)功能性接口
 目录：\FUNC
 **********************************/
extern bool fsel_favorite_add(flist_location_t *location, fsel_type_e target_fsel_type) __FAR__;
extern bool fsel_favorite_clear(fsel_type_e target_fsel_type) __FAR__;
extern bool fsel_favorite_delete(fsel_type_e target_fsel_type,uint16 list_no,flist_location_t *location) __FAR__;//收藏删除文件
extern uint16 fsel_favorite_get_total(fsel_type_e target_fsel_type) __FAR__;//获取指定收藏夹内文件的总数
extern bool fsel_change_filepath(void* source_location,plist_location_t *dest_location,fsel_type_e source_type,plist_type_e dest_type) __FAR__;
extern bool fsel_change_fav_location(void *source_location,flist_location_t *dest_location,fsel_type_e source_type,id3_info_t *id3_info_buf) __FAR__;
extern bool fsel_make_plistlayer(plist_layer_t *layer_buffer,plist_location_t * location) __FAR__;
/**********************************
 (3)磁盘/播放列表/收藏夹 文件定位播放接口
 目录：\fsel
 **********************************/
extern bool fsel_set_mode(fsel_mode_e mode) __FAR__;
extern bool fsel_set_typebit(uint32 file_type_bit,bool reset_dir_flag) __FAR__;
extern bool fsel_get_location(void *location,fsel_type_e type) __FAR__;
extern bool fsel_set_location(void *location,fsel_type_e type) __FAR__;
extern bool fsel_get_nextfile (char *strfile) __FAR__;
extern bool fsel_get_prevfile(char *strfile) __FAR__;
extern bool fsel_get_byno(char *strfile, uint16 num) __FAR__;
extern uint16 fsel_get_total(void) __FAR__;
extern uint16 fsel_get_fileno(void) __FAR__;

//以下为目录播放时，定位目录播放接口(预留)
extern bool fsel_get_nextdir (char *strfile) __FAR__;
extern bool fsel_get_prevdir(char *strfile) __FAR__;
extern bool fsel_enter_dir(uint8 mode,char *dir_name) __FAR__;

/**********************************
 (4)磁盘/播放列表/收藏夹 文件信息浏览接口
 目录：\bs
 **********************************/
extern bool fsel_browser_get_location(void *location,fsel_type_e type) __FAR__;
extern bool fsel_browser_set_location(void *location,fsel_type_e type) __FAR__;
extern bool fsel_browser_get_items(brow_decode_t *decode_param, file_brow_t *file_list) __FAR__;
extern bool fsel_browser_enter_dir(brow_dir_e type,uint16 list_no, dir_brow_t *dir_brow) __FAR__;
extern bool fsel_browser_select(uint16 list_no) __FAR__;
extern bool fsel_browser_delete(brow_del_e type,uint16 list_no ,void *location) __FAR__;
extern bool fsel_browser_modify_location(file_location_t *location,uint32 bitmap) __FAR__;
extern bool fsel_browser_set_file(void *location,fsel_type_e type) __FAR__;
extern bool fsel_browser_get_albums(brow_decode_t *decode_param, album_brow_t *album_list) __FAR__;
//文件夹下遍历文件
extern bool fsel_browser_scanfile_init (dir_brow_t *dir_browser) __FAR__;
extern scanfile_result_e fsel_browser_scanfile(file_path_info_t *path_info, dir_brow_t *dir_browser, scanfile_type_e type) __FAR__;

/**********************************
 (5)歌词打开和显示接口
 目录：\lrc
 **********************************/
extern bool lyric_open(char * music_filename) __FAR__;
extern bool lyric_open_dlb(char * music_filename,uint32 dlb_offset) __FAR__;
//extern bool lyric_close(void) __FAR__;

extern bool lyric_get_init(void) __FAR__;
extern bool lyric_get_next(char * lyric_str, uint8 len) __FAR__;
extern bool lyric_get_prev(char * lyric_str, uint8 len) __FAR__;
extern bool lyric_get_time(lrc_time_e time_type,uint32 *time) __FAR__;
extern bool lyric_check_query_time(uint32 time) __FAR__;
extern bool lyric_seek_for(char *lyric_str, uint8 len, uint32 time) __FAR__;

/**********************************
 (6)id3信息获取
 目录：\id3
 **********************************/
extern bool get_id3_info(id3_info_t *id3_info,const char *filename,id3_type_e file_type) __FAR__;

#endif//#ifndef  _enhanced_h
