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
* \file     sdfs.h
* \brief    sd文件系统接口
* \author   wuyueqian
* \version  1.0
* \date  2011/9/10
*******************************************************************************/


#ifndef _SDFS_H
#define _SDFS_H
#include <attr.h>
#include <typeext.h>

#define MAX_HEANDLE_NUM 8

#define MODE_READ   0
#define SEEK_SET    0
#define SEEK_CUR    1
#define SEEK_END    2
#define SDFILE_CLOSED   0xFF
#define  MFSLOCATEPAGE  0xf0  


#define SD_DIR_OFFSET 0x200
#define SD_DIR_NAME_LEN 0xb
#define SD_DIR_ENTRY_LEN 0x20

#ifndef PC
typedef struct
{
    unsigned char   isvalid;
    unsigned char   reserve[3];
    unsigned int    startaddr;
    unsigned int   endaddr;
    unsigned int   rwpointer;
} sd_file_t;
#else
typedef struct
{
    unsigned char   isvalid;
    unsigned char   reserve[3];
    unsigned int    startaddr;
    unsigned int   endaddr;
    unsigned int   rwpointer;
    char    filename[13];
} sd_file_t;
#endif

typedef struct
{
    unsigned char     fname[11];
    unsigned char     fattr;
    unsigned char     reserve0[2];
    unsigned short    version;
    unsigned int   offset;
    unsigned int   size;
    unsigned int   reserve1;
    unsigned int   checksum;
} sd_dir_t;

typedef sd_file_t* sd_handle;

extern void sd_fs_init(void);
extern sd_file_t *sd_fopen (const char *filename, void *null1, void* null2) __FAR__;
extern int sd_fclose (sd_file_t *fp, void *null1, void* null2) __FAR__;
extern int sd_fseek (sd_file_t *fp, uint8 whence, int32 offset ) __FAR__;
extern uint32 sd_ftell (sd_file_t *fp, void *null1, void* null2) __FAR__;
extern int32 sd_fread (sd_file_t *fp, void *buffer, uint32 len) __FAR__;

extern sd_file_t *sd_fopen_autotest (char *filename , void *null1, void* null2) __FAR__;
extern int32 sd_write_autotest (sd_file_t *fp, void *buffer, uint32 len) __FAR__;
extern int set_video_app_fix(uint8 is_jpeg_decode, void *null1, void* null2) __FAR__;

#endif
