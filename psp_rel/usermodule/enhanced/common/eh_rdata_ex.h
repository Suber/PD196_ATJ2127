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
 * \file     eh_rdata_ex.h
 * \brief    数据结构和常量定义
 * \author   liminxian
 * \version 1.0
 * \date  2011/09/15
 *******************************************************************************/

#ifndef _eh_rdata_ex_h
#define _eh_rdata_ex_h

extern fsel_module_e fsel_module_type;
extern fsel_error_e fsel_error; //错误类型
extern uint8 dir_layer; //当前目录或列表的层次，0表示根目录，1表示根目录的子目录，以此类推
extern uint8 shuffle_sw; //SHUFFLE 开关
extern uint16 file_no; //当前文件在目录或列表中的序号
extern uint16 file_total; //当前目录或列表的总文件个数
extern uint32 cur_ext_name; //当前选择歌曲的扩展名
extern uint16 root_dir; //根目录的子目录指针，1,2，。。。
extern uint8 obj_open_mode; //文件打开的模式
extern uint8 obj_open_type; //文件打开的列表类型
extern handle obj_handle; //操作的对象句柄，列表文件
extern int32 vfs_mount; //FS 识别号
extern fsel_param_t fsel_init_val; //模块参数
extern uint16 save_read_offset; //保存最后读地址
extern bool write_data_flag; //回写数据标记

//userlist
extern uint16 uspl_file_total; //收藏夹文件的总数
extern uint16 uspl_first_offset; //第一文件序号偏移(0,1,...)
extern uint16 uspl_last_offset; //最后文件序号偏移(0,1,...)
extern uint16 uspl_file_offset; //文件序号偏移(0,1,...)

//playlist
extern plist_item_info_t plist_item_info; //item 信息记录
extern uint16 plist_start_sort_index; //播放队列的起始sort_index
extern uint16 plist_file_total; // 文件的总数
extern uint16 plist_info_offset; //文件信息的偏移
extern uint16 plist_list_layerno[4];
extern plist_type_e plist_type_selector;
extern uint16 plist_start_file_index;
extern uint16 plist_cur_file_index; //当前播放歌曲的FILE_INDEX
extern uint16 plist_layer_offset; //当前浏览的list偏移

extern pfile_offset_t lib_pfile_offset; //列表偏移位置

extern uint16 m3u_curlist_index;

#endif //#ifndef _eh_rdata_ex_h
