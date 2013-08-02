/********************************************************************************
 *                              USDK(GL5110)
 *                            Module: KERNEL
 *                 Copyright(c) 2003-2011 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>      <time>           <version >             <desc>
 *      wuyueqian     2011-9-10 09:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     storage_device.h
 * \brief    块设备驱动接口
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef _STORAGE_DEVICE_H
#define _STORAGE_DEVICE_H

#define PLUG_OUT  -1
#define PLUG_IN 0
#define CHARGING 1

#include <ucos/init.h>

#ifndef _ASSEMBLER_

typedef void* (*device_op_i)(void*, void*, void*);

typedef enum {
	DEVICE_READ = 0,
	DEVICE_WRITE,
	DEVICE_UPDATE,
	DEVICE_GET_CAP,
	DEVICE_GET_SEC_SIZE,
	DEVICE_IS_WRITE_PROTECT,
} device_cmd_e;

/*non-base storage drviver struct*/
struct block_device_operations {
	device_op_i block_read;
	device_op_i block_write;
	device_op_i update_cache;
	device_op_i get_cap;
	device_op_i get_sector_size;
	device_op_i is_write_protect;
};

typedef enum {
	BASE_READ = 0,
	BASE_WRITE,
	BASE_UPDATE,
	BASE_GET_CAP,
	BASE_GET_SEC_SIZE,
	BASE_IS_WRITE_PROTECT,
	BASE_GET_INFO,
	BASE_MERGE_LOGBLOCK,
	BASE_MBREC_WRITE,
	BASE_BREC_WRITE,
	BASE_FW_WRITE,
	BASE_FW_READ,
	BASE_SD_READ
} base_stg_cmd_e;

struct base_block_device_operations {
	device_op_i block_read;
	device_op_i block_write;
	device_op_i update_cache;
	device_op_i get_cap;
	device_op_i get_sector_size;
	device_op_i is_write_protect;
	device_op_i get_info;
	device_op_i merge_logblock;
	device_op_i mbrec_write;
	device_op_i brec_write;
	device_op_i fw_write;
	device_op_i fw_read;
	device_op_i sd_read;
};

#define     BASE_GET_PAGE_SIZE          (0)
#define     BASE_GET_BASE_TYPE          (1)
#define     BASE_GET_BLOCK_SIZE         (2)
#define     BASE_GET_FLASH_ID           (3)

/*3.对上层提供的总接口函数*/
extern void * card_op_entry(void *param1, void *param2, void *param3, device_cmd_e cmd)__FAR__;
extern void * uhost_op_entry(void *param1, void *param2, void *param3, device_cmd_e cmd)__FAR__;
extern void * nand_op_entry(void *param1, void *param2, void *param3, device_cmd_e cmd)__FAR__;
extern void * base_op_entry(void *param1, void *param2, void *param3, device_cmd_e cmd)__FAR__;

#define base_read(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_READ)
#define base_write(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_WRITE)
#define base_update() base_op_entry((void*)0, (void*)0, (void*)0, BASE_UPDATE)
#define base_get_cap() base_op_entry((void*)0, (void*)0, (void*)0, BASE_GET_CAP)
#define base_get_sec_size() base_op_entry((void*)0, (void*)0, (void*)0, BASE_GET_SEC_SIZE)
#define base_is_write_protect() base_op_entry((void*)0, (void*)0, (void*)0, BASE_IS_WRITE_PROTECT)
#define base_get_page_size() base_op_entry((void*)BASE_GET_PAGE_SIZE, (void*)0, (void*)0, BASE_GET_INFO)
#define base_get_base_type() base_op_entry((void*)BASE_GET_BASE_TYPE, (void*)0, (void*)0, BASE_GET_INFO)
#define base_get_block_size() base_op_entry((void*)BASE_GET_BLOCK_SIZE, (void*)0, (void*)0, BASE_GET_INFO)
#define base_get_flash_id() base_op_entry((void*)BASE_GET_FLASH_ID, (void*)0, (void*)0, BASE_GET_INFO)
#define base_merge_logblock(a) base_op_entry((void*)(a), (void*)0, (void*)0, BASE_MERGE_LOGBLOCK)
#define base_enter_compact_mode() base_op_entry((void*)(0), (void*)1, (void*)0, BASE_MERGE_LOGBLOCK)
#define base_exit_compact_mode() base_op_entry((void*)(0), (void*)2, (void*)0, BASE_MERGE_LOGBLOCK)
#define base_mbrec_write(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_MBREC_WRITE)
#define base_brec_write(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_BREC_WRITE)
#define base_fw_write(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_FW_WRITE)
#define base_fw_read(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_FW_READ)
#define base_sd_read(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_SD_READ)

#ifndef NOR_BASE
#define nand_read(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_READ)
#define nand_write(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_WRITE)
#define nand_update() base_op_entry((void*)0, (void*)0, (void*)0, BASE_UPDATE)
#define nand_get_cap() base_op_entry((void*)0, (void*)0, (void*)0, BASE_GET_CAP)
#define nand_get_sec_size() base_op_entry((void*)0, (void*)0, (void*)0, BASE_GET_SEC_SIZE)
#define nand_is_write_protect() base_op_entry((void*)0, (void*)0, (void*)0, BASE_IS_WRITE_PROTECT)
#else
#define nor_read(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_READ)
#define nor_write(a, b, c) base_op_entry((void*)(a), (void*)(b), (void*)(c), BASE_WRITE)
#define nor_update() base_op_entry((void*)0, (void*)0, (void*)0, BASE_UPDATE)
#define nor_get_cap() base_op_entry((void*)0, (void*)0, (void*)0, BASE_GET_CAP)
#define nor_get_sec_size() base_op_entry((void*)0, (void*)0, (void*)0, BASE_GET_SEC_SIZE)
#define nor_is_write_protect() base_op_entry((void*)0, (void*)0, (void*)0, BASE_IS_WRITE_PROTECT)
#endif

#define card_read(a, b, c) card_op_entry((void*)(a), (void*)(b), (void*)(c), DEVICE_READ)
#define card_write(a, b, c) card_op_entry((void*)(a), (void*)(b), (void*)(c), DEVICE_WRITE)
#define card_update() card_op_entry((void*)0, (void*)0, (void*)0, DEVICE_UPDATE)
#define card_get_cap() card_op_entry((void*)0, (void*)0, (void*)0, DEVICE_GET_CAP)
#define card_get_sec_size() card_op_entry((void*)0, (void*)0, (void*)0, DEVICE_GET_SEC_SIZE)
#define card_is_write_protect() card_op_entry((void*)0, (void*)0, (void*)0, DEVICE_IS_WRITE_PROTECT)

#define uhost_read(a, b, c) uhost_op_entry((void*)(a), (void*)(b), (void*)(c), DEVICE_READ)
#define uhost_write(a, b, c) uhost_op_entry((void*)(a), (void*)(b), (void*)(c), DEVICE_WRITE)
#define uhost_update() uhost_op_entry((void*)0, (void*)0, (void*)0, DEVICE_UPDATE)
#define uhost_get_cap() uhost_op_entry((void*)0, (void*)0, (void*)0, DEVICE_GET_CAP)
#define uhost_get_sec_size() uhost_op_entry((void*)0, (void*)0, (void*)0, DEVICE_GET_SEC_SIZE)
#define uhost_is_write_protect() uhost_op_entry((void*)0, (void*)0, (void*)0, DEVICE_IS_WRITE_PROTECT)

#endif

#endif

