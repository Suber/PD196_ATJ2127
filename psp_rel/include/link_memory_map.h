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
 * \file     link_memory_map.h
 * \brief    提供一些特殊用途的地址
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/

#ifndef _LINK_MEMORY_MAP_H
#define _LINK_MEMORY_MAP_H

#define BASE_MAP 0x40000000
#define BASE_NON_MAP 0x00000000

#define SD_SWAP_ADDR_NO_VIDEO BASE_NON_MAP+0x16000
#define SD_SWAP_ADDR_WITH_VIDEO BASE_NON_MAP+0x1dc00
#define SD_SWAP_ADDR_WITH_PICTURE BASE_NON_MAP+0x18000

#endif
