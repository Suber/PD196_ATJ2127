/********************************************************************************
*                              USDK(1100)
*                            Module: M32TLB
*                 Copyright(c) 2003-2008 Actions Semiconductor,
*                            All Rights Reserved. 
*
* History:         
*      <author>    <time>           <version >             <desc>
*        wuyueqian     2011-9-10 15:00     1.0             build this file 
********************************************************************************/
/*!
* \file     m32tlb.h
* \brief   m32 mmu∂®“Â
* \author   open source
* \version 1.0
* \date  2011-9-10
*******************************************************************************/
/* m32tlb.h : MIPS32 MMU definitions*/
 
#ifndef __M32TLB_H__
#define __M32TLB_H__

#ifdef __cplusplus
extern "C" {
#endif

/* XXX For the moment we just share the R4000 definitions, but
   we need to add support for BAT based processor variants. */
#assert mmu(r4k)

/*#include <mips/r4ktlb.h> ########## by jason 2005-9-21*/
#include "r4ktlb.h"

/* MIPS32r2 EntryLo extended page frame address bits */
#if __mips == 64 || __mips64
#define TLB_PFNXMASK	0x007fffffc0000000LL
#else
#define TLB_PFNXMASK	0xc0000000
#endif
#define TLB_PFNXSHIFT	29

#ifdef __cplusplus
}
#endif
#endif /* __M32TLB_H__ */

