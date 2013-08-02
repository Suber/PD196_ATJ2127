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
* \file     kernel_sd_drv.h
* \brief    SD×Ö½Ú¶Á½Ó¿Ú
* \author   wuyueqian
* \version  1.0
* \date  2011/9/10
*******************************************************************************/
#ifndef _KERNEL_SD_H
#define _KERNEL_SD_H

#include <typeext.h>
#include <ucos/init.h>

//typedef unsigned int uint32;

typedef struct 
{
    unsigned int file_offset;
    uint32 sram_addr;
    uint32 len;
}sd_byte_param_t;


extern int sd_byte_read(sd_byte_param_t *sd_param) __FAR__;


typedef struct 
{
    uint32 file_offset;
    uint32 sram_addr;
    uint32 sec_num;
}sd_sec_param_t;

#define NAND_BASE_PLATFORM 0
#define CARD_BASE_PLATFORM 1
#define NOR_BASE_PLATFORM 2

typedef struct
{  
    int (*sd_sec_read)(uint32 file_offset, uint32 sram_addr, uint32 sec_num);
    int (*sd_sec_write)(uint32 file_offset, uint32 sram_addr, uint32 sec_num);
    int (*vm_sec_read)(uint32 file_offset, uint32 sram_addr, uint32 sec_num);
    int (*vm_sec_write)(uint32 file_offset, uint32 sram_addr, uint32 sec_num);
    int8 base_type;
    int8 reserse[3];
}base_op_t;



#define VM_512_BUFFER 0x0

extern int vm_read_fix(void *pbuf, unsigned int address, unsigned int len)__FAR__;
extern int vm_write_multi(void *pbuf, unsigned int address, unsigned int len)__FAR__;
extern int vm_write(void *pbuf, unsigned int address, void* null3)__FAR__;



typedef struct{
unsigned int SD_cap;
unsigned int VM_cap;
unsigned int HID_cap;
unsigned int AUTORUN_cap;
unsigned int ENCRYPT_cap;
unsigned int UNKNOWN_cap;
}sysdef_info_t;

extern int get_sys_info(sysdef_info_t *sys_info, void *null2, void* null3);

 #endif

