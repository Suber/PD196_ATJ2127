/*******************************************************************************
 *                              AS212A
 *                            Module: USB Host
 *                 Copyright(c) 2011-2016 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       gch      2011-12-19            1.0             build this file
*******************************************************************************/
/*!
 * \file     uhost_head.h
 * \brief    This file gathers the headfiles of USB Host driver.
 * \author   gch
 * \version  1.0
 * \date     2011-12-19
*******************************************************************************/
#ifndef _UHOST_H
#define _UHOST_H

/* if "HIGH_SPEED_DISABLE" is NOT defined, the uhost uses high speed mode as the default mode,
 * otherwise, full speed mode will be the default mode.
 */
#define     HIGH_SPEED_DISABLE

#define     RDATA_ADDR_BASE         0x9fc00000
#define     URAM_ADDRESS            0x17000
#define     RAM0_ADDRESS            0x18000
#define     RAM3_ADDRESS            0x22000
#define     RAM4_ADDRESS            0x26000
#define     RAM5_ADDRESS            0x2a000
#define     RAM6_ADDRESS            0x2e000
#define     RAM7_ADDRESS            0x32000
#define     RAM8_ADDRESS            0x34000
#define     JRAM5_ADDRESS           0x35800

#ifdef __UHOST_ONLY_DEBUG
// 单独调试uhost模块时使用RAM4作为DATA_BUFFER_ADDRESS的空间
// #define     DATA_BUFFER_ADDRESS     (RDATA_ADDR_BASE + RAM4_ADDRESS)

/*
 * 单模块调试时，g_uhost_state使用RAM7的空间
 */
#define     g_uhost_state           (*(uint8 *)(RDATA_ADDR_BASE + RAM7_ADDRESS))

#else

/* buffer info is changeable according to different platform */
// #define     DATA_BUFFER_ADDRESS     (RDATA_ADDR_BASE + 0x1c600)

/*
 * g_uhost_state是常驻的全局变量,占用系统的数据空间,类型如下(注意操作2byte的数据时必须按2byte对齐)：
 * uint8 g_uhost_state;
 */
#define     g_uhost_state           (*(uint8 *)(RDATA_ADDR_BASE + 0x19e78))

#endif  // #ifdef __UHOST_ONLY_DEBUG
 
// --------------------Configurable macros(配置项)----------------------------

// BULK_IN 和 BULK_OUT 端点缓冲区的最大大小
#define     EP_MAX_BUF_SIZE_BULK_IN                                         0x400 // 1KB
#define     EP_MAX_BUF_SIZE_BULK_OUT                                        0x200 // 512 bytes

/* BULK IN and BULK OUT endpoint number
 * Note: endpoint number must be '1' or '2', and must not be other value or other format, because they
 *       are used by other macros.
 *       e.g. "0x01" or "0x02" is the error format.
 */
#define     BULK_IN_EP_NUM                                                  1 // BULK IN 端点号,
#define     BULK_OUT_EP_NUM                                                 2 // BULK OUT 端点号


// BULK_IN 和 BULK_OUT 端点缓冲区的起始地址
#define     EP_BUF_STA_ADDR_BULK_IN     0x80 // 不可改变,因为前边0x80 bytes的空间给端点0的IN和OUT端点使用了
#define     EP_BUF_STA_ADDR_BULK_OUT    (EP_BUF_STA_ADDR_BULK_IN + EP_MAX_BUF_SIZE_BULK_IN)

 
#define     GL_CONTROLLER_ENABLED          (0x01 << 0x01)
#define     FSM_A_HOST                     0x03 /* The USB FSM State: A_host */

/* function parameter for reset_usb_controller() */
#define     CONTROLLER_INIT             0x00
#define     CONTROLLER_ENABLE           0x01

    
#define     LINESTATUS_DP                                   0x08
    
#define     MULTI_USED_UVDD_EN_OR                           (0x01 << MULTI_USED_UVDD_EN)
#define     MULTI_USED_UVDD_V_1_3                           (0x06 << MULTI_USED_UVDD_V_SHIFT) // 1.3V
#define     MRCR_USBReset2_OR                               (0x01 << MRCR_USBReset2)
#define     MRCR_USBReset_OR                                (0x01 << MRCR_USBReset)
#define     CLKENCTL_USBCLKEN_OR                            (0x01 << CLKENCTL_USBCLKEN)
#define     LINESTATUS_otgreset_OR                          (0x01 << LINESTATUS_otgreset)
#define     DPDMCTRL_LineDetEN_OR                           (0x01 << DPDMCTRL_LineDetEN)
#define     USB_PHYCTRL_Phy_PLLEN_OR                        (0x01 << USB_PHYCTRL_Phy_PLLEN)
#define     USB_PHYCTRL_Phy_DALLUALLEN_OR                   (0x01 << USB_PHYCTRL_Phy_DALLUALLEN)
#define     BKDOOR_hsdisable_OR                             (0x01 << BKDOOR_hsdisable)
#define     USBCtrl_abusdrop_OR                             (0x01 << USBCtrl_abusdrop)
#define     USBCtrl_busreq_OR                               (0x01 << USBCtrl_busreq)
#define     USBien_locsofien_OR                             (0x01 << USBien_locsofien)
#define     USBEIRQ_usbien_OR                               (0x01 << USBEIRQ_usbien)


#define     MEMCLKCTL1_URAMCLKSEL_OR                        (0x01 << MEMCLKCTL1_URAMCLKSEL)
#define     MEMCLKCTL0_URAMCLKEN_OR                         (0x01 << MEMCLKCTL0_URAMCLKEN)

/*
 * Common Used macros
 */
#define     _CONCATENATE_2_MACRO(x, y)                      x ## y
#define     CONCATENATE_2_MACRO(x, y)                       _CONCATENATE_2_MACRO(x, y) // 连接2个宏定义
#define     CONCATENATE_3_MACRO(x, y, z)                    CONCATENATE_2_MACRO(CONCATENATE_2_MACRO(x, y), z)
#define     CONCATENATE_4_MACRO(w, x, y, z)                 CONCATENATE_2_MACRO(CONCATENATE_3_MACRO(w, x, y), z)
#define     CONCATENATE_5_MACRO(v, w, x, y, z)              CONCATENATE_2_MACRO(CONCATENATE_4_MACRO(v, w, x, y), z)


#define     HCxSTADDR_BULK_OUT          CONCATENATE_3_MACRO(IN, BULK_OUT_EP_NUM, STADDR) // INxSTADDR is OUT for host
#define     HCxSTADDR_BULK_IN           CONCATENATE_3_MACRO(OUT, BULK_IN_EP_NUM, STADDR) // OUTxSTADDR is IN for host
 
/* close USB clock (这样USB controller部分可以省功耗，DP&DM的时钟源应该是AHBCLK，所以关闭的clock跟DP&DM的时钟源无关).
 * 关闭PLLEN即可,其他像PLLEN, USBRESET 和 GoSuspend都不用管,另外Phy_DALLUALLEN是内部用的,也可关闭
 */
#define     CLOSE_USB_CONTROLLER_CLOCK() \
            act_clearb(USB_PHYCTRL_Phy_PLLEN_OR | USB_PHYCTRL_Phy_DALLUALLEN_OR, USB_PHYCTRL)
#define     OPEN_USB_CONTROLLER_CLOCK()  \
            act_setb(USB_PHYCTRL_Phy_PLLEN_OR | USB_PHYCTRL_Phy_DALLUALLEN_OR, USB_PHYCTRL)

/* enable Line status detect, enable 15Kohm DP/DM pull down, disable 500Kohm DP/DM pull up resistor */
#define     ENABLE_DEVICE_DETECT() \
            act_writeb((act_readb(DPDMCTRL) & (uint8)0xf0) | DPDMCTRL_LineDetEN_OR, DPDMCTRL)
#define     DISABLE_DEVICE_DETECT() // ??? 需要这个函数不? 还是只要上电后enable一次就可以了

#endif /* _UHOST_H */
