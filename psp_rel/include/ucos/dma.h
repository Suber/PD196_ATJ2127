/********************************************************************************
 *                               USDK(1100)
 *                            Module: DMA
 *                 Copyright(c) 2001-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *        wuyueqian     2011-9-10 15:00     1.0             build this file
 ********************************************************************************/
/*!
 * \file     dma.h
 * \brief    dma管理头文件
 * \author   wuyueqian
 * \version 1.0
 * \date  2011-9-10
 *******************************************************************************/
#ifndef __DMA_H__
#define __DMA_H__

#include <ucos/irq.h>
#include <asm-mips/exception.h>

#ifndef _ASSEMBLER_
/*!
 *  DMA类型 \n
 *  \li DMA_CHAN_TYPE_SPECIAL : 专业通道DMA
 *  \li DMA_CHAN_TYPE_BUS: 总线通道DMA
 *
 */
enum {
	DMA_CHAN_TYPE_SPECIAL, DMA_CHAN_TYPE_BUS
};

struct dma_chan {
	unsigned int io;
	int irq;
};

/* These are in arch/mips/atj213x/common/dma.c */
//extern struct dma_chan act213x_dma_table[];
extern int request_dma_chan(interrupt_handle_t dma_isr, void* null1,
		void* null2);
extern int free_dma_chan(unsigned int dmanr, void* null1, void* null2);

#include <asm-mips/mach-gl5110/dma.h>

#endif /*_ASSEMBLER_*/
#endif /* __DMA_H__ */
