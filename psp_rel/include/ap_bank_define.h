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
* \file     ap_bank_define.h
* \brief    bank管理接口
* \author   wuyueqian
* \version  1.0
* \date  2011/9/10
*******************************************************************************/
#ifndef __AP_BANK_DEFINE_H__
#define __AP_BANK_DEFINE_H__

#include <typeext.h>
#include <attr.h>

#define FPGA_BUG


typedef struct
{
    unsigned int    bank_offset; 
    unsigned int    bank_addr;     
    unsigned int    bank_length;    
}ap_bank_head_t;

typedef struct
{
    unsigned int    ap_bank1_offset;
    unsigned int    ap_bank2_offset;
    unsigned int    ap_enhanced1_offset;
    unsigned int    ap_enhanced2_offset;
}ap_bank_offset_t;

typedef struct
{
    unsigned char    file_type;          //'P'
    unsigned char    ap_type;            //0:ap_system, 1:ap_user    
    unsigned char    major_version;      //3, for ACTOS 3.0
    unsigned char    minor_version;      //0
    unsigned char    magic[4];            //'W', 'G', 0x19, 0x97
    unsigned int    text_offset;		
    unsigned int    text_length;
    unsigned int    text_addr;          //in ram 代码段
    unsigned int    data_offset;		
    unsigned int    data_length;
    unsigned int    data_addr;          //有初始化的全局变量
    unsigned int    bss_length;
    unsigned int    bss_addr;
    unsigned int    entry;
//    ap_bank_offset_t   ap_bank_offset;
}ap_head_t;


typedef struct
{
    unsigned int    basal_offset;
    unsigned int    codec_offset;
}codec_bank_offset_t;

typedef struct
{
unsigned char file_type;
unsigned char media_type;
unsigned char major_version;
unsigned char minor_version;
unsigned char magic[4];
unsigned int text_offset;
unsigned int text_length;
unsigned int text_addr;
unsigned int data_offset;
unsigned int data_length;
unsigned int data_addr;
unsigned int bss_length;
unsigned int bss_addr;
unsigned int module_api_entry;
}codec_head_t;


typedef struct
{
    uint32 basal_entry;
    uint32 codec_entry;
}mid_entry_t;

/*
typedef struct
{
    uint32 ap_file_start;
    uint32 basal_file_start;
    uint32 codec_file_start;
}app_file_start_t;
*/
typedef uint32 app_file_start_t;

#define AP_FRONT_AP_FILE 0
#define AP_FRONT_BASAL_FILE 1
#define AP_FRONT_CODEC_FILE 2
#define AP_BACK_AP_FILE 3
#define AP_BACK_BASAL_FILE 4
#define AP_BACK_CODEC_FILE 5
#define AP_MANAGER_FILE 6


extern int exece_ap(char *ap_name, bool is_back_ap, void* arg) __FAR__;
extern int load_codec(char *codec_name, bool is_back_ap, void *null2) __FAR__;
extern int load_mmm(char *mmm_name, bool is_back_ap, void *null2) __FAR__;
extern void free_ap(bool is_back_ap, void* null1, void* null2) __FAR__;
extern void free_mmm(bool is_back_ap, void* null1, void* null2) __FAR__;
extern void free_codec(bool is_back_ap, void* null1, void* null2) __FAR__;
extern int bank_flush(uint8 bank_indication, void* null1, void *null2) __FAR__;
extern int kernel_bank_manager(unsigned int api_no);
extern int bank_manager(uint32 epc);

#endif


