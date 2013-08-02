/*******************************************************************************
 *                              GL5110
 *                            Module: ENHANCED
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *      jiancailiu  2011-10-8 17:13    1.0             build this file
 *******************************************************************************/
/*!
 * \file     FS.H
 * \brief    数据结构和常量定义
 * \author   jiancailiu
 * \version 1.0
 * \date  2011/10/8
 *******************************************************************************/

#ifndef _FS_DEFINE_H
#define _FS_DEFINE_H

#ifndef _ASSEMBLER_

#include <vfs.h>
#include <vfs_interface.h>
#include <typeext.h>

extern bool fat_cd(vfs_mount_t *p_vfs_mount, uint8 mode, uint8* ptr_input_name,
		void *null3);
extern uint32 fat_dir(vfs_mount_t *p_vfs_mount, uint8 mode,
		uint8* ptr_input_name, uint32 ext_name_bitmap);
extern uint16 fat_get_name(vfs_mount_t *p_vfs_mount, uint8* ptr_output_name,
		uint16 longname_length, void *null3);
extern bool fat_file_dir_offset(vfs_mount_t *p_vfs_mount,
		pdir_layer_t* ptr_layer, pfile_offset_t* ptr_file_offset, uint8 mode);
extern bool fat_make_dir(vfs_mount_t *p_vfs_mount, uint8* ptr_input_name,
		void *null2, void *null3);
extern uint32 fat_file_open(vfs_mount_t *p_vfs_mount, uint8* ptr_input_name,
		uint8 mode, void *null3);
extern uint32 fat_file_create(vfs_mount_t *p_vfs_mount, uint8* ptr_input_name,
		uint32 lentgh, void *null3);
extern bool fat_file_close(vfs_mount_t *p_vfs_mount, uint32 fhandle,
		void *null2, void *null3);
extern bool fat_file_dir_remove(vfs_mount_t *p_vfs_mount,
		uint8* ptr_input_name, void *null2, void *null3);
extern bool fat_file_get_size(vfs_mount_t *p_vfs_mount, uint32* output_length,
		uint8* ptr_file, uint8 mode);
extern bool fat_get_time(vfs_mount_t *p_vfs_mount,
		file_time_t* ptr_output_time, uint8* ptr_input_name, uint8 type);
extern bool fat_file_seek(vfs_mount_t *p_vfs_mount, int32 offset, uint8 type,
		uint32 fhandle);
extern bool fat_file_tell(vfs_mount_t *p_vfs_mount, uint32* ptr_offset,
		uint32 fhandle, void *null3);
extern uint32 fat_file_read(vfs_mount_t *p_vfs_mount, uint8* ptr_data_buffer,
		uint32 byte_count, uint32 fhandle);
extern uint32 fat_file_write(vfs_mount_t *p_vfs_mount, uint8* ptr_data_buffer,
		uint32 byte_count, uint32 fhandle);
extern bool fat_get_space(vfs_mount_t *p_vfs_mount, uint32* ptr_sector_count,
		uint8 type, void *null3); //return sector count
extern uint8 fat_get_err_info(vfs_mount_t *p_vfs_mount, void *null1,
		void *null2, void *null3);
extern uint32 fat_file_dir_exist(vfs_mount_t *p_vfs_mount,
		uint8* ptr_input_name, uint8 type, void *null3);
extern uint32 fat_file_cut(vfs_mount_t *p_vfs_mount, uint32 fhandle_src,
		uint32 modify_length, uint32 fhandle_dst);
extern uint8 fat_file_attralter(vfs_mount_t *p_vfs_mount, uint8 attr,
		uint8* ptr_file, uint8 mode);
extern bool fat_file_rename(vfs_mount_t *p_vfs_mount, uint8* ptr_new_name,
		uint8* ptr_file, uint8 mode);
extern bool fat_create_volume(vfs_mount_t *p_vfs_mount, uint8* ptr_input_name,
		void *null2, void *null3);
extern bool fat_set_time(vfs_mount_t *p_vfs_mount, file_time_t* ptr_input_time,
		uint8* ptr_input_name, uint8 type);
extern bool fat_set_private_data(vfs_mount_t *p_vfs_mount, void *null1,
		void *null2, void *null3);
extern bool fat_clear_private_data(vfs_mount_t *p_vfs_mount, void *null1,
		void *null2, void *null3);

#endif

#endif /*_DRIVER_H */

