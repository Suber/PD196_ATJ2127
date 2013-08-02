/*******************************************************************************
 *                              US212A
 *                            Module: Key driver
 *                 Copyright(c) 2003-2008 Actions Semiconductor,
 *                            All Rights Reserved.
 *
 * History:
 *      <author>    <time>           <version >             <desc>
 *       reagan     2011-9-2 14:45    1.0             build this file
 *******************************************************************************/
/*!
 * \file     key_init.c
 * \brief    key driver initial functions
 * \author   reagan
 * \version 1.0
 * \date  2011-9-2
 *******************************************************************************/

//#include "gpio_opr.h"
#include "key_inner.h"

/******************************************************************************/
/*!
 * \par  Description:
 *	  KEY驱动装载，按键初始化，负责中断向量安装等。
 * \param[in]    none
 * \param[in]    none
 * \param[in]    none
 * \param[out]   none
 * \ingroup      key_install
 * \note
 * \li  该接口在驱动装载时由系统自动调用。
 *******************************************************************************/
int key_init(void)
{
    unsigned int tmp;

    //初始化线控按键 LRADC1 设置后需要等待10ms才能读出正确的数据，
    //否则可能全0,建议放到welcome进行初始化，则不需要等待10ms
    tmp = act_readl(PMUADC_CTL);
    tmp |= (unsigned int) (0x1 << 5); //enable LRADC1/3/4/5
    act_writel(tmp, PMUADC_CTL);
    sys_mdelay(20);
    //默认Play1s开机，使用默认值
    time_irq = sys_set_irq_timer1(key_scan, 0x02);

    for (tmp = 0; tmp < 16; tmp++)
    {
        key_map[tmp] = tmp;
    }

    g_rtc_counter = 2900;
    act_writeb(act_readb(CHG_CTL) & 0xDF, CHG_CTL);//停止充电
    TheCharge = 0;
    act_writel(act_readl(GPIO_AOUTEN) |0x02, GPIO_AOUTEN);//enable speak control
    act_writel(act_readl(GPIO_ADAT) &0xfffffffd, GPIO_ADAT);//enable speak control mute
    g_speakonoff = 0;
    act_writel(act_readl(GPIO_ADAT) &0xfffffffd, GPIO_ADAT);//enable speak control mute
    
    return 0;
}

module_init(key_init)
