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
 * \file     *.c
 * \brief    文件选择器常驻数据
 * \author   liminxian
 * \par      GENERAL DESCRIPTION:
 *               这里对文件进行描述
 * \par      EXTERNALIZED FUNCTIONS:
 *
 * \version 1.0
 * \date  2011/9/15
 *******************************************************************************/

#include "eh_module.h"

fsel_module_e fsel_module_type; //模块类型

fsel_error_e fsel_error; //错误类型
uint8 dir_layer; //当前目录或列表的层次，0表示根目录，1表示根目录的子目录，以此类推
uint8 shuffle_sw; //SHUFFLE 开关
uint16 file_no; //当前文件在目录或列表中的序号
uint16 file_total; //当前目录或列表的总文件个数

uint32 cur_ext_name; //当前选择歌曲的扩展名
uint16 root_dir; //根目录的子目录指针，1,2，。。。
uint8 obj_open_mode; //文件打开的模式,若为0xee，则标记是VM的句柄（记录偏移）
uint8 obj_open_type; //文件打开的列表类型
handle obj_handle; //操作的对象句柄，列表文件
uint32 vfs_mount; //vFS 识别号
fsel_param_t fsel_init_val; //模块参数

uint16 save_read_offset; //保存最后读地址
bool write_data_flag; //回写数据标记

//userlist
uint16 uspl_file_total; //收藏夹文件的总数
uint16 uspl_first_offset; //第一文件序号偏移(0,1,...)
uint16 uspl_last_offset; //最后文件序号偏移(0,1,...)
uint16 uspl_file_offset; //文件序号偏移(0,1,...)

//playlist
plist_item_info_t plist_item_info; //item 信息记录
plist_type_e plist_type_selector; //当前管理的列表类型
uint16 plist_file_total; // 文件的总数
uint16 plist_info_offset; //文件信息的偏移
uint16 plist_list_layerno[4];
uint16 plist_start_sort_index; //播放队列的起始sort_index
uint16 plist_start_file_index; //起始的file_index
uint16 plist_cur_file_index; //当前播放歌曲的FILE_INDEX
uint16 plist_layer_offset; //当前浏览的list偏移

pfile_offset_t lib_pfile_offset; //列表偏移位置

// m3u
uint16 m3u_curlist_index;  //当前选择的M3U LIST 序号


