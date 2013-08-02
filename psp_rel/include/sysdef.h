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
 * \file     sysdef.h
 * \brief    系统定义
 * \author   wuyueqian
 * \version  1.0
 * \date  2011/9/10
 *******************************************************************************/
#ifndef _SYSDEF_H
#define _SYSDEF_H

#include <storage_device.h>
#include <typeext.h>



/* card-gpio-detect address.  */
#define GLOBAL_DETECT_CARD_ISR_ADDR     (RCODE_ADDR_BASE + 0x7e59)


/* for uart-gpio, 0x9fc19a88--uart_tx_gpio_cfg_mask,  0x9fc19a8c--uart_tx_gpio_cfg_value.  */
#define GLOBAL_UART_TX_GPIO_CFG_MASK     (*(uint32 *)(RDATA_ADDR_BASE + 0x19a88))
#define GLOBAL_UART_TX_GPIO_CFG_VALUE    (*(uint32 *)(RDATA_ADDR_BASE + 0x19a8c))


/* check map, must not change address of following variables, if add new variables after them.  */
#define GLOBAL_AP_BANK_HANDLE_HANDLER     (*(void *)(RDATA_ADDR_BASE + 0x19a94))
#define GLOBAL_KERNEL_BANK_HANDLE_HANDLER (*(void *)(RDATA_ADDR_BASE + 0x19a98))

/* bit0----if is 1, for pass EMI, not change nand_pad_drv according to frequency;   */
/* bit1----if is 0(default), if exception, will reset; if 1, not reset.  */
/* bit2----if is 0(default), card-gpio-detec; if 1, card-cmd-detect for sys_detect_disk(). must not to request timer irq to detect card.  */
/* bit3----only for US280A, if is 0(default), disable isr_card-cmd-detec; if 1, enable isr_card-cmd-detec.*/
/* bit4----if is 1, for pass EMI, Nand/Card clock is fixed HOSC, not according to MCUPLL;    */
/* bit5----if is 1, for pass EMI, UDisk AP set clock and USB LIB not;  */
#define GLOBAL_KERNEL_BACKDOOR_FLAG     (*(uint32 *)(RDATA_ADDR_BASE + 0x19a9c))

/* when used as uhost, check if device is in.  */
#define GLOBAL_KERNEL_UHOST_STATUS     (*(int8 *)(RDATA_ADDR_BASE + 0x19aa0))


#endif  /*_SYSDEF_H*/

