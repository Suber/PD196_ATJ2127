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
#ifndef _VM_DEF_H
#define _VM_DEF_H

/* AP VRAM 数据，0x00000000 - 0x00008000，支持32个AP（1KB） */
#define  VM_AP_SETTING          0x00000000
#define  VM_AP_MAINMENU         0x00000400
#define  VM_AP_VIDEO            0x00000800
#define  VM_AP_PICTURE          0x00000C00  
#define  VM_AP_BROWSER          0x00001000
#define  VM_AP_MUSIC            0x00001400
#define  VM_AP_MUSIC_AUDBK      0x00001600
#define  VM_AP_MENGINE          0x00001800
#define  VM_AP_EBOOK            0x00001c00
#define  VM_AP_RADIO            0x00002000
#define  VM_AP_FMENGINE         0x00002400
#define  VM_AP_RECORD           0x00002800
#define  VM_AUD_SYS             0x00002c00//保存audible sys文件 1K
#define  VM_AP_USER1            0x00003000//测试专用
#define  VM_AP_CONFIG           0x00003400

#define  VM_FILE_NUM            0x00003800
#define  VM_ALARM               0x00003A00
#define  VM_AP_DSC              0x00003c00

//书签起始地址从VM区的1M位置开始
#define  VM_BOOKMARK_START      0x00100000

/* AP VRAM 数据区标志，可以统一为 0x55AA */
#define  MAGIC_SETTING          0x55AA
#define  MAGIC_USER1            0x55AA
#define  MAGIC_MUSIC            0x55AA
#define  MAGIC_MENGINE          0x55AA
#define  MAGIC_CONFIG           0x55AA
#define  MAGIC_BROWSER          0x55AA
#define  MAGIC_PICTURE          0x55AA
#define  MAGIC_VIDEO            0x55AA
#define  MAGIC_ALARM            0x55AA
#define  MAGIC_FMRECORD         0x55AA
#define MAGIC_BOOKMARK          0x55AA
#define  MAGIC_AUDBK            0x55AA

/* 文件浏览路径记录（每次创建播放列表时清除），4~8KB，0x00008000 - 0x0000a000 */
#define  VM_DIR_HISTORY         0x00008000
/* 菜单路径记录（每次开机后清除），4~8KB，0x0000a000 - 0x0000c000 */
#define  VM_MENU_HISTORY        0x0000a000

/* ENHANCED 的歌词显示时的时间戳和内容(显示歌词时占用) ,16KB,0x0000c000 - 0x00010000*/
#define  VM_LYRIC_INFO          0x0000c000

/* PLAYLIST 的排序信息缓存(建表过程时占用),256KB,0x00010000 - 0x00050000*/
#define  VM_PLAYLIST_INFO       0x00010000

//for album art info 23K, 分配32K，以留作扩展，0x00050000 - 0x00058000
#define  VM_ALBUMART_INFO       0x00050000

//for nand 介质U盘，存放收藏夹信息，分配100K,0x0e0000 - 0x100000,而card和host的收藏夹仍然存在磁盘上
#define  VM_FAVORITE_INFO   0x0e0000

#define  VM_BMK_INFO	    0x00200000
#endif
