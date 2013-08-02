/*******************************************************************************
 *                              us211A
 *                            Module: case
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       zhangxs     2011-10-24 16:20     1.0             build this file
 *******************************************************************************/
/*!
 * \file     vm_def.h
 * \brief    包含各个应用所占VM空间宏定义
 * \author   zhangxs
 * \version 1.0
 * \date  2011-10-24
 *******************************************************************************/
#ifndef _VM_FWSP_DEF_H
#define _VM_FWSP_DEF_H

#define VM_FWSP_START 0x00080000
#define FS_VM VM_FWSP_START
#define FS_VM_LEN 0x2000
#define UDISK_VM (FS_VM+FS_VM_LEN)
#define UDISK_VM_LEN 0x200
#define UDISK_ENCRYPT_VM (UDISK_VM+UDISK_VM_LEN)
#define UDISK_ENCRYPT_VM_LEN 0x200
#define UDISKB_START_ADDR_OFFSET 0 //加密盘B盘起始地址在加密盘vram区信息中的偏移
#define UDISKC_START_ADDR_OFFSET 4 //加密盘C盘起始地址在加密盘vram区信息中的偏移
#define UDISKA_CAP_OFFSET        8 //加密盘A盘容量在加密盘vram区信息中的偏移    
#define UDISKB_CAP_OFFSET        12 //加密盘B盘容量在加密盘vram区信息中的偏移    
#define UDISKC_CAP_OFFSET        16 //加密盘C盘容量在加密盘vram区信息中的偏移    
#endif
