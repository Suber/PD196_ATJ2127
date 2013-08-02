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
 * \file     typeext.h
 * \brief    类型定义
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _TYPEEXT_H
#define _TYPEEXT_H

#include <ucos_types.h>

#ifndef _ASSEMBLER_

//#define  bool unsigned char
#define  uchar unsigned char
typedef unsigned char uint8;
typedef signed char int8;
typedef unsigned short uint16;
typedef signed short int16;
typedef unsigned int uint32;
typedef signed int int32;
typedef unsigned long ulong32;
typedef signed long long32;

#define  HWORD unsigned short
#define  hword unsigned short
//#define  WORD unsigned int
#define  word unsigned int
#ifndef PC
#define  DWORD unsigned long long
#define  dword unsigned long long
#endif
#define  HANDLE unsigned int
#define  handle unsigned int
#define  ulong unsigned long

typedef char INT8;
typedef unsigned char UINT8;

typedef short INT16;
typedef unsigned short UINT16;

typedef int INT32;
typedef unsigned int UINT32;

#ifndef PC
typedef long long INT64;
typedef unsigned long long UINT64;
#else
typedef _int64 INT64;
typedef _int64 UINT64;
#endif

#ifndef  NULL
#define  NULL ((void *)0)
#endif

#endif

#define  TRUE   1
#define  FALSE  0

#define  FOREVER 1
#define  NEVER 0

#ifndef NULL
#define  NULL 0
#endif

#define VOID_SIGNED(value) ((void*)((int32)(value)))
#define VOID_UNSIGNED(value) ((void*)((uint32)(value)))

#ifndef PC
#define __ATTRIBUTE_PACKED__  __attribute__ ((packed))
#else
#define __ATTRIBUTE_PACKED__
#endif
#endif  /* _TYPEEXT_H*/
